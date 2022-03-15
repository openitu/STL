function bs1770(input_file,output_file,nchan,lev_target,conf)
%% BS-1770 implementation

STEP_SIZE = 4800;         % 100 ms step;
BLOCK_SIZE = 4*STEP_SIZE; % 400 ms gating block;

default_conf = '000011000011000000000000';

if exist('conf','var') && exist('nchan','var')
   if length(conf) ~= nchan
     fprintf( '*** Number of channels %d does not match configuration %s, exiting..\n\n', nchan, conf );  
     exit(-1);
   end
end

if nargin < 5 
    conf = default_conf;
end
if nargin < 4 
    lev_target = -26;
end
if nargin < 3 
    nchan = 1;
end

fprintf( 'Input file:       %s\n', input_file );
fprintf( 'Output file:      %s\n', output_file );
fprintf( 'nchan:            %d\n', nchan );

G = parse_conf(conf,nchan);

fp = fopen(input_file,'rb');
x = reshape(fread(fp,'short'),nchan,[])'/32768;
fclose(fp);

N = length(x);
M = floor(4 * (N - BLOCK_SIZE) / BLOCK_SIZE);

% R-REC-BS.1770-2-201103.pdf, Table 1, Filter coefficients for stage 1 of the pre-filter to model a spherical head
b = [ 1.53512485958697 -2.69169618940638 1.19839281085285];
a = [1 -1.69065929318241 0.73248077421585];

% R-REC-BS.1770-2-201103.pdf, Table 2, Filter coefficients for the RLB weighting curve
b2 = [1.0 -2.0 1.0];
a2 = [1 -1.99004745483398 0.99007225036621];

y = zeros(size(x));

z = zeros(nchan,M);

for i=1:nchan
    y(:,i) = filter(b2,a2,filter(b,a,x(:,i)));
    tmp = frame(y(:,i),BLOCK_SIZE,BLOCK_SIZE-STEP_SIZE);
    tmp = tmp(:,1:M);
    z(i,:) = sum(tmp.^2);
end
l = sum(z .* (G * ones(1,M)),1)/BLOCK_SIZE;

[lev_in,lev_out,fac] = find_scaling_factor(l, lev_target);


fprintf('Input level:      %.2f\n',lev_in);
fprintf('Target level:     %.2f\n',lev_target);
fprintf('Obtained level:   %.2f\n',lev_out);
fprintf('Scaling factor:   %.2f\n',fac);

fp = fopen(output_file,'wb');
fwrite(fp,x'*fac*32768,'short');
fclose(fp);

function [lev_in,lev_out,fac] = find_scaling_factor(energy, lev_target)     

RELATIVE_DIFF = 0.0001;
MAX_ITERATIONS = 10;
last_fac = 100;
fac = 1;
itr = 0;

while( (abs( 1.0 - fac / last_fac ) > RELATIVE_DIFF) && (itr < MAX_ITERATIONS) )
    I = find(-0.691 + 10*log10(energy*fac^2) > -70);
    thr = -0.691 + 10*log10(mean(energy(I)*fac^2)) - 10;
    I = find(-0.691 + 10*log10(energy*fac^2) > thr);
    lev = -0.691 + 10*log10(mean(energy(I)*fac^2));
    last_fac = fac;
    fac = fac * 10.0^((lev_target - lev) / 20 );
    if itr == 0
        lev_in = lev;
    end
    itr = itr + 1;
end

lev_out = lev;

function [F,N] = frame(X,len,overlap)

if(nargin < 3)
    overlap = 0;
end

step = len - overlap;

L = length(X);
N = ceil((L - len)/step);

% Create index matrix
indx = ones(len,1) * (1:step:N*step+1) + (0:len-1)' * ones(1,N+1);

% Pad with zeros at the end of the input vector if necessary
if(L < N*step+len)
    X(N*step+len) = 0;
end

% Form output using indices
F = X(indx);

% Special case for single frame output
if(numel(F) == length(F))
	F = F';
end

N = size(indx,2);

function G = parse_conf(conf,nchan)
G = zeros(nchan,1);
for i=1:nchan
    if conf(i) == '1'
        G(i) = 1.41;
    else
        if conf(i) == '0'
            G(i) = 1;
        else
            fprintf('*** Invalid configuration %s, exiting..\n\n',conf);
        end
    end
end