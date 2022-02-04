%%

input_file = 'sine997_0dBFS.raw';
% input_file = 'sine997_-26dbov.raw';
output_file = 'out_m.raw';
channels = 6;

bs1770(input_file,output_file,channels)

