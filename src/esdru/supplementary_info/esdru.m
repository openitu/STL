function esdru(infile,outfile,fs,alpha,e_step,seed)

% ESDRU(infile,outfile,fs,alpha,e_step,seed))
%
%
% infile    - input file name
% outfile   - output file name
% fs        - sampling frequency (Hz)
% alpha     - spectral distortion value, [0,1]
% e_step    - energy step in high energy segments
% seed      - random number generator seed
%
% To call the esdru from a shell script, use e.g.:
% 
%   matlab -nosplash -nodesktop -wait -r "esdru('input32k.raw','output32k.raw',32000,$3);exit"
%
%

if nargin == 6
    rng(seed);  
else
    rng('shuffle');
end
if nargin < 4
    e_step = 0.5;
end

fid = fopen(infile,'rb');
x = reshape(fread(fid,'integer*2'),2,[])';
fclose(fid);

step = 1.5*fs/50; % 30 ms transition
m  = g_mod_nrg(x,step,e_step);

g1 = 1-m*(1-alpha);
g2 = 1+alpha-g1;

y = [(x(:,1)*alpha + x(:,2)*(1-alpha)).*g1 (x(:,1)*(1-alpha) + x(:,2)*alpha).*g2 ];

fid = fopen(outfile,'wb');
fwrite(fid, y','integer*2');
fclose(fid);


function g = g_mod_nrg(x,step,e_step)

% G = G_MOD_NRG(X,STEP)
%
% Random function with step-wise pattern and smooth transitions
%
% Considers energy of input signal X to avoid switching during stationary
% segments.

% Energy analysis
N = length(x);
e = sum(x.^2,2);

% Short-term energy envelope
alpha1 = 0.001;
[B,A] = iir1(alpha1);
es = filter(B,A,flipud(filter(B,A,flipud(e))));

% Long-term energy envelope
alpha2 = 0.0001;
beta = 0.77813;
[B,A] = iir1(alpha2);
el = beta*filter(B,A,flipud(filter(B,A,flipud(es))));

gamma = 0.2;   % Probability to switch to new value. Proportional to alpha?

energy_low = es(1:step:end) < el(1:step:end);

M = ceil(N/step);

if(length(energy_low)<M)
    energy_low(M) = 0;
end

I_switch = [find((rand(1,M-1) < gamma)) M];
n = ones(1,M);

n_prev = 1;
for i=1:length(I_switch)-1
    b2 = e_step + (1-e_step)*energy_low(I_switch(i));
    n_new = rand*b2 + n_prev*(1-b2);
    n(I_switch(i):I_switch(i+1)) = n_new;
    n_prev = n_new;
end

xf_win = 0.5 * (1 - cos(pi*(0:step-1)/step))';

n2 = xf_win*n + (1-xf_win)*[1 n(1:end-1)];
g = n2(1:N)';

function [B,A] = iir1(alpha)

% [B,A] = iir1(alpha)
%
% Generate first order one-pole iir filter of the form 
% y(n) = alpha * x(n) + (1-alpha) * y(n-1)
%

B = alpha;
A = [1 (alpha-1)];
