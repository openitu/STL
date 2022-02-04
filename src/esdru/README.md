Implementation of ESDRU as defined in ITU-T Recommendation P.811

Matches output of MATLAB version esdru.m, if the random number generator and cos 
function is synchronized.

See LICENSE.md for terms.
Author: erik.norvell@ericsson.com

Example command lines:
Run ESDRU with alpha=0.5:
    esdru.exe 0.5 input.pcm output.pcm

Run ESDRU with alpha=0.0, 32000 Hz sampling rate, modulation step during high energy = 1.0, seed 10:
esdru.exe -sf 32000 -e_step 1.0 -seed 10 0.0 input.pcm output.pcm - Run ESDRU with alpha=0.0,
