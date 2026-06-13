/* basop platform include file, v2.0 - 5 Oct 2023

  platform definitions to support basop32.c and .h, enh40.c and .h

    -assumes stl.h has been included first
    -note for EVS codec: stl.h includes typedef.h which includes options.h
*/

#ifndef _BASIC_PLATFORM_H
#define _BASIC_PLATFORM_H

#ifndef _STL_H
  #include "stl.h"
  #pragma message ( " stl.h not yet included in file " __FILE__ "!" )
#endif

/* codec specific definitions, Oct 2023 */

#define _GENERIC_CODEC_  0
#define _EVS_CODEC_      1
#define _AMR_CODEC_      2
#define _AMR_WB_CODEC_   3

#ifdef SUPPORT_JBM_TRACEFILE  /* "SUPPORT_JBM_TRACEFILE" is defined in ETSI 3GPP EVS source code options.h file, Oct 2023 */
  #ifndef _CODEC_TYPE_
    #define _CODEC_TYPE_ _EVS_CODEC_  /* _CODEC_TYPE_ could also be defined in the Makefile if needed */
  #endif
#else  /* EVS developers can put debug compile-time messages here. But needs to be removed for production codes to avoid ITU GPL license violation */
/*
  #pragma message ( " options.h not yet included in file " __FILE__ "!" )
*/
#endif

#endif  /* _BASIC_PLATFORM_H */
