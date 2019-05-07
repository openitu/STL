/*
  ============================================================================
   File: ENHUL32.H                                       v.1.0 - 01.July.2018
  ============================================================================


  ============================================================================
*/

#ifndef _ENHUL32_H
#define _ENHUL32_H

/*****************************************************************************
 *
 *  Constants and Globals
 *
 *****************************************************************************/
#include "stl.h"

#ifndef UWord64 
#define UWord64 unsigned long long          /* for local use inside UL_Mpy_32_* */
#endif

#if (WMOPS)
#include "count.h"
extern BASIC_OP multiCounter[MAXCOUNTERS];  /* existing signed counters are reused for unsigedn operators */
extern int currCounter;
#endif /* if WMOPS */

/*****************************************************************************
 *
 *  Prototypes for enhanced unsigned 32 bit arithmetic operators
 *
 *****************************************************************************/
#ifdef ENH_U_32_BIT_OPERATOR
UWord32 UL_addNs (UWord32 a, UWord32 b, UWord16* wrap);
UWord32 UL_subNs (UWord32 a, UWord32 b, UWord16* sgn);

UWord32 UL_Mpy_32_32 (UWord32 a, UWord32 b);
void Mpy_32_32_uu (UWord32 a, UWord32 b, UWord32 *c_h, UWord32 *c_l);    /* does not saturate */
void Mpy_32_16_uu (UWord32 a, UWord16 b, UWord32 *c_h, UWord16 *c_l);    /* does not saturate */

Word16 norm_ul (UWord32 UL_var1);
UWord32 UL_deposit_l (UWord16);             /* deposit low without sign extension */
#endif /* ENH_U_32_BIT_OPERATOR */

#endif /*_ENHUL32_H*/

/* end of file */
