 /*****************************************************************************
 *
 *  Enhanced 32 bit operators :
 *
 *    Mpy_32_16_1()
 *    Mpy_32_16_r()
 *    Mpy_32_32()
 *    Mpy_32_32_r()
 *    Madd_32_16()
 *    Msub_32_16()
 *    Madd_32_32()
 *    Msub_32_32()
 *
 *****************************************************************************/


/*****************************************************************************
 *
 *  Include-Files
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "enh32.h"

#if (WMOPS)
extern BASIC_OP multiCounter[MAXCOUNTERS];
extern int currCounter;
#endif /* if WMOPS */

#ifdef ENH_32_BIT_OPERATOR
/*****************************************************************************
 *
 *  Local Functions
 *
 *****************************************************************************/
 
/*****************************************************************************
 *
 *  Constants and Globals
 *
 *****************************************************************************/


/*****************************************************************************
 *
 *  Functions
 *
 *****************************************************************************/

Word32 Mpy_32_16_1 (Word32 L_var1, Word16 var2) {
  Word32 L_var_out = W_sat_m (W_mult_32_16 (L_var1, var2) );
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_16_1++;
  multiCounter[currCounter].W_mult_32_16--;
  multiCounter[currCounter].W_sat_m--;
#endif /* if WMOPS */ 
    return L_var_out;
}

Word32 Mpy_32_16_r (Word32 L_var1, Word16 var2) {
  Word32 L_var_out = W_round48_L (W_mult_32_16 (L_var1, var2 ) );
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_16_r++;
  multiCounter[currCounter].W_mult_32_16--;
  multiCounter[currCounter].W_round48_L--;
#endif /* if WMOPS */ 
  return L_var_out;
}

Word32 Mpy_32_32 (Word32 L_var1, Word32 L_var2) {
  Word32 L_var_out;
  Word64 L64_var1; 

  L64_var1 = ((Word64)L_var1 * L_var2); 
  L64_var1 = W_shl (L64_var1, 1);
  L_var_out = W_extract_h (L64_var1 );
#if (WMOPS)
    multiCounter[currCounter].Mpy_32_32++;
  multiCounter[currCounter].W_shl--;
  multiCounter[currCounter].W_extract_h--;
#endif /* if WMOPS */ 
    return L_var_out;
}

Word32 Mpy_32_32_r (Word32 L_var1, Word32 L_var2) {
  Word32 L_var_out;
  Word64 L64_var1; 

  L64_var1 = ((Word64) L_var1 * L_var2);
  L64_var1 = W_shr (L64_var1, 15);
  L_var_out = W_round48_L (L64_var1 );
#if (WMOPS)
    multiCounter[currCounter].Mpy_32_32_r++;
  multiCounter[currCounter].W_shr--;
  multiCounter[currCounter].W_round48_L--;
#endif /* if WMOPS */ 
    return L_var_out;
}

Word32 Madd_32_16 (Word32 L_var3, Word32 L_var1, Word16 var2) {
  Word32 L_var_out;

  L_var_out = Mpy_32_16_1 (L_var1, var2);
  L_var_out = L_add (L_var3, L_var_out);
  
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_16_1--;
  multiCounter[currCounter].L_add--; 
  multiCounter[currCounter].Madd_32_16++; 
#endif /* if WMOPS */	 
  return L_var_out;
}

Word32 Madd_32_16_r (Word32 L_var3, Word32 L_var1, Word16 var2) {
  Word32 L_var_out;

  L_var_out = Mpy_32_16_r (L_var1, var2);
  L_var_out = L_add (L_var3, L_var_out);
  
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_16_r--;
  multiCounter[currCounter].L_add--;
  multiCounter[currCounter].Madd_32_16_r++; 
#endif /* if WMOPS */	 
    return L_var_out;
}

Word32 Msub_32_16 (Word32 L_var3, Word32 L_var1, Word16 var2) {
  Word32 L_var_out;

  L_var_out = Mpy_32_16_1 (L_var1, var2);
  L_var_out = L_sub(L_var3, L_var_out);
  
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_16_1--;
  multiCounter[currCounter].L_sub--;
  multiCounter[currCounter].Msub_32_16++; 
#endif /* if WMOPS */	 
  return L_var_out;
}

Word32 Msub_32_16_r (Word32 L_var3, Word32 L_var1, Word16 var2) {
  Word32 L_var_out;

  L_var_out = Mpy_32_16_r (L_var1, var2);
  L_var_out = L_sub (L_var3, L_var_out);
  
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_16_r--;
  multiCounter[currCounter].L_sub--;
  multiCounter[currCounter].Msub_32_16_r++; 
#endif /* if WMOPS */	 
  return L_var_out;
}

Word32 Madd_32_32 (Word32 L_var3, Word32 L_var1, Word32 L_var2) {
  Word32 L_var_out;

  L_var_out = Mpy_32_32 (L_var1, L_var2);
  L_var_out = L_add (L_var3, L_var_out);
  
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_32--;
  multiCounter[currCounter].L_add--;
  multiCounter[currCounter].Madd_32_32++; 
#endif /* if WMOPS */	 
  return L_var_out;
}

Word32 Madd_32_32_r (Word32 L_var3, Word32 L_var1, Word32 L_var2) {
  Word32 L_var_out;

  L_var_out = Mpy_32_32_r (L_var1, L_var2);
  L_var_out = L_add (L_var3, L_var_out);
  
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_32_r--;
  multiCounter[currCounter].L_add--;
  multiCounter[currCounter].Madd_32_32_r++; 
#endif /* if WMOPS */	 
  return L_var_out;
}

Word32 Msub_32_32 (Word32 L_var3, Word32 L_var1, Word32 L_var2) {
  Word32 L_var_out;

  L_var_out = Mpy_32_32 (L_var1, L_var2);
  L_var_out = L_sub (L_var3, L_var_out);
  
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_32--;
  multiCounter[currCounter].L_sub--;
  multiCounter[currCounter].Msub_32_32++; 
#endif /* if WMOPS */	 
  return L_var_out;
}

Word32 Msub_32_32_r (Word32 L_var3, Word32 L_var1, Word32 L_var2) {
  Word32 L_var_out;

  L_var_out = Mpy_32_32_r (L_var1, L_var2);
  L_var_out = L_sub (L_var3, L_var_out);
  
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_32_r--;
  multiCounter[currCounter].L_sub--;
  multiCounter[currCounter].Msub_32_32_r++; 
#endif /* if WMOPS */	 
  return L_var_out;
}

#endif /* #ifdef ENH_32_BIT_OPERATOR */
 
/* end of file */
