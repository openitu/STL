
#ifndef _ENH32_H
#define _ENH32_H

#include "stl.h"
 
#ifndef Word64 
#define Word64 long long int
#endif

 /*****************************************************************************
 *
 *  Prototypes for enhanced 32 bit arithmetic operators
 *
 *****************************************************************************/
#ifdef ENH_32_BIT_OPERATOR

Word32 Mpy_32_16_1(Word32 L_var1, Word16 var2);
Word32 Mpy_32_16_r(Word32 L_var1, Word16 var2);
Word32 Mpy_32_32(Word32 L_var1, Word32 L_var2);
Word32 Mpy_32_32_r(Word32 L_var1, Word32 L_var2);
Word32 Madd_32_16(Word32 L_var3, Word32 L_var1, Word16 var2);
Word32 Madd_32_16_r(Word32 L_var3, Word32 L_var1, Word16 var2);
Word32 Msub_32_16(Word32 L_var3, Word32 L_var1, Word16 var2);
Word32 Msub_32_16_r(Word32 L_var3, Word32 L_var1, Word16 var2);
Word32 Madd_32_32(Word32 L_var3, Word32 L_var1, Word32 L_var2);
Word32 Madd_32_32_r(Word32 L_var3, Word32 L_var1, Word32 L_var2);
Word32 Msub_32_32(Word32 L_var3, Word32 L_var1, Word32 L_var2);
Word32 Msub_32_32_r(Word32 L_var3, Word32 L_var1, Word32 L_var2);

#endif /* #ifdef ENH_32_BIT_OPERATOR */

#endif /*_ENH32_H*/


/* end of file */


