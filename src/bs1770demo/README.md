Implementation of BS.1770-4 as defined in Recommendation ITU-R BS.1770-4
Author: erik.norvell@ericsson.com

Example command line for a 5.1 audio input file with channel order [FL FR C LFE BL BR]
bs1770demo.exe -nchan 6 -conf 000L11 -lev -26 sine997_0dBFS.raw sine997_-26LKFS.raw

To verify the algorithm, please set up and run the BS.2217 conformance test as specified in
supplementary_info/run_conformance.bash
