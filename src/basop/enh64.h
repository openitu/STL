
#ifndef _ENH64_H
#define _ENH64_H

#include "stl.h"
 
#ifndef Word64 
#define Word64 long long int
#endif

#ifndef UWord64
#define UWord64 unsigned long long int
#endif

#define MAX_64 (Word64)0x7fffffffffffffffLL
#define MIN_64 (Word64)0x8000000000000000LL

 /*****************************************************************************
 *
 *  Prototypes for enhanced 64 bit arithmetic operators
 *
 *****************************************************************************/
#ifdef ENH_64_BIT_OPERATOR
Word64 W_add_nosat (Word64 L64_var1, Word64 L64_var2);
Word64 W_sub_nosat (Word64 L64_var1, Word64 L64_var2);
Word64 W_shl (Word64 L64_var1, Word16 var2);
Word64 W_shr (Word64 L64_var1, Word16 var2);
Word64 W_shl_nosat (Word64 L64_var1, Word16 var2);
Word64 W_shr_nosat (Word64 L64_var1, Word16 var2);
Word64 W_mult_32_16 (Word32 L_var1, Word16 var2);
Word64 W_mac_32_16 (Word64  L64_acc, Word32 L_var1, Word16 var2);
Word64 W_msu_32_16 (Word64  L64_acc, Word32 L_var1, Word16 var2);
Word64 W_mult0_16_16 (Word16 var1, Word16 var2);
Word64 W_mac0_16_16 (Word64  L64_acc, Word16 var1, Word16 var2);
Word64 W_msu0_16_16 (Word64  L64_acc, Word16 var1, Word16 var2);
Word64 W_mult_16_16 (Word16 var1, Word16 var2);
Word64 W_mac_16_16 (Word64  L64_acc, Word16 var1, Word16 var2);
Word64 W_msu_16_16 (Word64  L64_acc, Word16 var1, Word16 var2);

Word64 W_deposit32_l (Word32 L_var1);
Word64 W_deposit32_h (Word32 L_var1);

Word32 W_sat_l (Word64 L64_var);
Word32 W_sat_m (Word64 L64_var);
Word32 W_shl_sat_l (Word64 L64_var, Word32 n);

Word32 W_extract_l (Word64 L64_var1);
Word32 W_extract_h (Word64 L64_var1);

Word32 W_round48_L (Word64 L64_var1);
Word16 W_round32_s (Word64 L64_var1);

Word16 W_norm (Word64 L_var1);


Word64 W_add (Word64 L64_var1, Word64 L64_var2);
Word64 W_sub (Word64 L64_var1, Word64 L64_var2);
Word64 W_neg (Word64 L64_var1);
Word64 W_abs (Word64 L64_var1);
Word64 W_mult_32_32 (Word32 L_var1, Word32 L_var2);
Word64 W_mult0_32_32 (Word32 L_var1, Word32 L_var2);
UWord64 W_lshl (UWord64 L64_var1, Word16 var2);
UWord64 W_lshr (UWord64 L64_var1, Word16 var2);
Word32 W_round64_L (Word64 L64_var1) ;

#endif /* #ifdef ENH_64_BIT_OPERATOR */

#endif /*_ENH64_H*/


/* end of file */


