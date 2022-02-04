fs = 48000;
t = 0:1/fs:3-1/fs;
f = 997;

y = 32767 * sin(2*pi*f*t);

Y = zeros(6,length(t));
Y(1,:) = y;

binwrite(Y,'sine997_0dBFS.raw','short');
