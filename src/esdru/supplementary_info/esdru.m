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

if nargin < 6
    seed = 1;  
else
    rng('shuffle');
end
if nargin < 5
    e_step = 0.5;
end

fid = fopen(infile,'rb');
x = reshape(fread(fid,'integer*2'),2,[])';
fclose(fid);

step = 1.5*fs/50; % 30 ms transition
m  = g_mod_nrg(x,step,e_step,seed);

g1 = 1-m*(1-alpha);
g2 = 1+alpha-g1;

y = [(x(:,1)*alpha + x(:,2)*(1-alpha)).*g1 (x(:,1)*(1-alpha) + x(:,2)*alpha).*g2 ];

fid = fopen(outfile,'wb');
fwrite(fid, y','integer*2');
fclose(fid);


function m = g_mod_nrg(x,step,e_step,seed)

% m = g_mod_nrg(x,step,e_step,seed)
%
% Random function with step-wise pattern and smooth transitions
%
% Considers energy of input signal X to avoid switching during stationary
% segments.

% Energy analysis
N = length(x);
e = sum(x.^2,2);

RAN16_32C_MAX = 65535;

% Short-term energy envelope
alpha1 = 0.001;
[B,A] = iir1(alpha1);
es = filter(B,A,flipud(filter(B,A,flipud(e))));

% Long-term energy envelope
alpha2 = 0.0001;
beta = 0.77813;
[B,A] = iir1(alpha2);
el = beta*filter(B,A,flipud(filter(B,A,flipud(es))));

f_energy = fopen('es_el_matlab.double','wb');
fwrite(f_energy, es, 'double');
fwrite(f_energy, el, 'double');
fclose(f_energy);

i = 1;
m_prev = 1.0;
m = zeros(N,1);
while i <= N
    [r,seed] = ran16_32c( seed );
    if  double(r) / RAN16_32C_MAX < 0.2 
        if es(i) < el(i)
            m_delta = 1.0;
        else
            m_delta = e_step;
        end
        [r,seed] = ran16_32c( seed );
        m_new = double(r)/RAN16_32C_MAX * m_delta + m_prev * (1.0 - m_delta);
    else
        m_new = m_prev;
    end
    
    j = 1;
    while j <= step && i <= N
        xf_win = 0.5 * (1.0 - cos( pi * j / step ));
        m(i) = m_new * xf_win + m_prev * (1.0 - xf_win);
        i = i + 1;
        j = j + 1;
    end
    m_prev = m_new;
end

binwrite(m,'mm.double');

function [B,A] = iir1(alpha)

% [B,A] = iir1(alpha)
%
% Generate first order one-pole iir filter of the form 
% y(n) = alpha * x(n) + (1-alpha) * y(n-1)
%


B = alpha;
A = [1 (alpha-1)];

function [r,seed] = ran16_32c(seed)
% 
% Converted from ITU-T G.191 STL compfile.c, function ran16_32c()

% [r,seed] = ran16_32c(seed)
seed = single(seed);
BIT24 =	16777216.0;
BIT8  = 256.0;

buffer1 = ((253.0 * double(seed)) + 1.0);
buffer2 = (buffer1 / BIT24);
seedl = bitand(floor(buffer2), hex2dec('FFFFFF'));
buffer1 = buffer1 - single(seedl) * BIT24;
seed = buffer1;
r = uint16(floor(buffer1 / BIT8));


