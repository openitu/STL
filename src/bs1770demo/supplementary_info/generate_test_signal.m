fs = 48000;
t = 0:1/fs:3-1/fs;
f = 997;

y = 32767 * sin(2*pi*f*t);

Y = zeros(6,length(t));
Y(1,:) = y;


fid = fopen('sine997_0dBFS.pcm','w');
fwrite(fid,Y,'short');
fclose(fid);

%%

rng(1);

A = 0.070880495302845;

N = randn(size(y));
AN = 1.636408779315202e+03;

Y(1,:) = A * 32767 * sin(2*pi*997*t);
Y(2,:) = 0.58 * A * 32767 * sin(2*pi*1033*t);
Y(3,:) = 1.33 * A * 32767 * sin(2*pi*782*t);
Y(4,:) = AN * randn(size(y));
Y(5,:) = 0.45*AN * randn(size(y));
Y(6,:) = 2.01*AN * randn(size(y));

fid = fopen('sine_noise_test.pcm','w');
fwrite(fid,Y,'short');
fclose(fid);

%%

x = sin(2*pi*f*t) .* logspace(0,4,3*fs);

fid = fopen('sine_ramp.pcm','w');
fwrite(fid,x,'short');
fclose(fid);
