    =============================================================
    COPYRIGHT NOTE: This source code, and all of its derivations,
    is subject to the "ITU-T General Public License". Please have
    it  read  in    the  distribution  disk,   or  in  the  ITU-T
    Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
    CODING STANDARDS".
    =============================================================


The ITU-T/amr-eid module contains the following files:

# C code
```
 eid-amr.c: ....... demonstration program
```

# Description

This module provides an Error Insertion Device (EID) tool for AMR operating with G.192 error patterns to simulate packet erasures.
The bitstream of the 3GPP AMR codec follows a specific ETSI/3GPP format detailed in Clause 6.3 of 3GPP TS 26.073, which is not compliant with ITU-T G.192.
The eid-xor tool from ITU-T STL is not directly applicable to AMR bitstreams.
This modules fixes this issue with a tool derived from the EID-3G tool (see http://www.3gpp.org/ftp/tsg_sa/WG4_CODEC/AMR-NB_3G-Characterization/EID_Error_Patterns/eid-3g.zip)
The EID-AMR tool operates in a way similar to the EID-3G tool with the following functional differences:
- No position parameter is provided (this restriction is to operate consistently with the eid-xor tool).
- Bit errors are not taken into account.
- Statistics are reported as in the eid-xor tool.
- All data bits in an erased frame are forced to zero.

This module has been originally submitted to 3GPP in Tdoc S4-120998 (Aug. 2012)