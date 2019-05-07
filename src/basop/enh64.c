/*****************************************************************************
*
*  Enhanced 64 bit operators :
*
*    W_mac_32_16()
*    W_mac0_16_16()
*    W_msu0_16_16()
*
*****************************************************************************/


/*****************************************************************************
*
*  Include-Files
*
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "enh64.h"

#if (WMOPS)
extern BASIC_OP multiCounter[MAXCOUNTERS];
extern int currCounter;
#endif /* if WMOPS */


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

#ifdef ENH_64_BIT_OPERATOR


/*___________________________________________________________________________
|                                                                           |
|   Function Name : W_add_nosat                                           |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
|   64 bits addition of the two 64 bits variables (L64_var1+L64_var2)       |
|                                                                           |
|   Complexity weight : 1                                                   |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|   L64_var1   64 bit long signed integer (Word64) whose value falls in the |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|                                                                           |
|   L64_var2   64 bit long signed integer (Word64) whose value falls in the |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|    L64_var_out                                                            |
|             64 bit long signed integer (Word64) whose value falls in the  |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|___________________________________________________________________________|
*/
Word64 W_add_nosat (Word64 L64_var1, Word64 L64_var2) { 
  Word64 L64_var_out;
 
  L64_var_out = L64_var1 + L64_var2;

#if (WMOPS) 
  multiCounter[currCounter].W_add_nosat++;
#endif
 
  return L64_var_out;
}


/*___________________________________________________________________________
|                                                                           |
|   Function Name : W_sub_nosat                                             |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
|   64 bits subtraction of the two 64 bits variables (L64_var1-L64_var2)    |
|                                                                           |
|   Complexity weight : 1                                                   |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|   L64_var1   64 bit long signed integer (Word64) whose value falls in the |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|                                                                           |
|   L64_var2   64 bit long signed integer (Word64) whose value falls in the |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|    L64_var_out                                                            |
|             64 bit long signed integer (Word64) whose value falls in the  |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|___________________________________________________________________________|
*/
Word64 W_sub_nosat (Word64 L64_var1, Word64 L64_var2) { 
  Word64 L64_var_out;
 
  L64_var_out = L64_var1 - L64_var2;

#if (WMOPS) 
  multiCounter[currCounter].W_sub_nosat++;
#endif
 
  return L64_var_out;
}


/*___________________________________________________________________________
|                                                                           |
|   Function Name : W_shl                                                   |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
|   Arithmetically shift the 64 bit input L64_var1 left var2 positions. Zero|
|   fill the var2 LSB of the result. If var2 is negative, arithmetically    |
|   shift L64_var1 right by -var2 with sign extension. Saturate the result  |
|   in case of underflows or overflows.                                     |
|                                                                           |
|   Complexity weight : 1                                                   |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|    L64_var1   64 bit long signed integer (Word64) whose value falls in the|
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|                                                                           |
|    var2                                                                   |
|             16 bit short signed integer (Word16) whose value falls in the |
|             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|    L64_var_out                                                            |
|             64 bit long signed integer (Word64) whose value falls in the  |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|___________________________________________________________________________|
*/
Word64 W_shl (Word64 L64_var1, Word16 var2) {
 
  Word64 L64_var_out = 0LL;
 
  if (var2 <= 0) { 
    if (var2 < -64) 
      var2 = -64; 
    var2 = -var2; 
    L64_var_out = L64_var1 >> var2; 
  } 
  else { 
    for (; var2 > 0; var2--) { 
      if (L64_var1 > (Word64) 0X3fffffffffffffffLL) { 
        Overflow = 1; 
        L64_var_out = (Word64) 0X7fffffffffffffffLL; 
        break; 
      } 
      else { 
        if (L64_var1 < (Word64) 0xc000000000000000LL) { 
          Overflow = 1; 
          L64_var_out = (Word64)0x8000000000000000LL; 
          break; 
        } 
      } 
      L64_var1 *= 2; 
      L64_var_out = L64_var1; 
    } 
  }
#if (WMOPS) 
  multiCounter[currCounter].W_shl++;
#endif
 
//  BASOP_CHECK();
 
  return (L64_var_out);
}


/*___________________________________________________________________________
|                                                                           |
|   Function Name : W_shr                                                   |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
|   Arithmetically shift the 64 bit input L64_var1 right var2 positions.    |
|   Zero fill the var2 LSB of the result. If var2 is negative,              |
|   arithmetically shift L64_var1 left by -var2 with sign extension.        |
|   Saturate the result in case of underflows or overflows.                 |
|                                                                           |
|   Complexity weight : 1                                                   |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|    L64_var1   64 bit long signed integer (Word64) whose value falls in the|
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|                                                                           |
|    var2                                                                   |
|             16 bit short signed integer (Word16) whose value falls in the |
|             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|    L64_var_out                                                            |
|             64 bit long signed integer (Word64) whose value falls in the  |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|___________________________________________________________________________|
*/
Word64 W_shr (Word64 L64_var1, Word16 var2) {
  Word64 L64_var_out;
 
  if (var2 < 0) { 
    var2 = -var2; 
    L64_var_out  = W_shl (L64_var1, var2);

#if (WMOPS) 
    multiCounter[currCounter].W_shl--;
#endif /* if WMOPS */
 
  } else { 
    L64_var_out = L64_var1 >> var2;
  }

#if (WMOPS) 
  multiCounter[currCounter].W_shr++;
#endif /* if WMOPS */
 
  return (L64_var_out);
}


/*___________________________________________________________________________
|                                                                           |
|   Function Name : W_shl_nosat                                             |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
| Arithmetically shifts left the 64-bit variable W_var1 by var2 positions.  |
|  if var2 is negative, W_var1 is shifted to the least significant bits by  | 
|  (�var2) positions with extension of the sign bit .                       |
|  if var2 is positive, W_var1 is shifted to the most significant bits by   | 
|   (var2) positions without saturation control on 64 bits.                 |
|                                                                           |
|   Complexity weight : 1                                                   |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|    L64_var1   64 bit long signed integer (Word64) whose value falls in the|
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|                                                                           |
|    var2                                                                   |
|             16 bit short signed integer (Word16) whose value falls in the |
|             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|    L64_var_out                                                            |
|             64 bit long signed integer (Word64) whose value falls in the  |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|___________________________________________________________________________|
*/
Word64 W_shl_nosat (Word64 L64_var1, Word16 var2) {
 
  Word64 L64_var_out = 0LL;
 
  if (var2 <= 0) { 
    var2 = -var2; 
    L64_var_out = L64_var1 >> var2; 
  } 
  else { 
    L64_var_out = L64_var1 << var2; 
  }
#if (WMOPS) 
  multiCounter[currCounter].W_shl_nosat++;
#endif
 
//  BASOP_CHECK ();
 
  return (L64_var_out);
}


/*___________________________________________________________________________
|                                                                           |
|   Function Name : W_shr_nosat                                             |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
| Arithmetically shifts right the 64-bit variable W_var1 by var2 positions. |
|  if var2 is negative, W_var1 is shifted to the most significant bits by   | 
|  (�var2) positions without saturation control on 64 bits.                 |
|  if var2 is positive, W_var1 is shifted to the least significant bits by  | 
|   (var2) positions with extension of the sign bit .                       |
|                                                                           |
|   Complexity weight : 1                                                   |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|    L64_var1   64 bit long signed integer (Word64) whose value falls in the|
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|                                                                           |
|    var2                                                                   |
|             16 bit short signed integer (Word16) whose value falls in the |
|             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|    L64_var_out                                                            |
|             64 bit long signed integer (Word64) whose value falls in the  |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
|___________________________________________________________________________|
*/
Word64 W_shr_nosat (Word64 L64_var1, Word16 var2) { 
  Word64 L64_var_out;
 
  if (var2 < 0) { 
    var2 = -var2; 
    L64_var_out  =  L64_var1 << var2;
 
  } else { 
    L64_var_out = L64_var1 >> var2; 
  }

#if (WMOPS) 
  multiCounter[currCounter].W_shr_nosat++;
#endif /* if WMOPS */
 
  return (L64_var_out);
}


/*_________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_mac_32_16                                                                   |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply L_var2 by var3 and shift the result left by 1. Add the 64 bit                        |
|   result to L64_var1, return a 64 bit result.                                                   |
|                                                                                                 |
|   Complexity weight : 1                                                                         |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    L64_var1                                                                                     |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.                 |
|                                                                                                 |
|    L_var2                                                                                       |
|             32 bit signed integer (Word32) whose value falls in the                             |
|             range : 0x8000 0000 <= L_var2 <= 0x7fff ffff                                        |
|                                                                                                 |
|    var3                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var3 <= 0x0000 7fff.                                         |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    L64_var_out                                                                                  |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var_out <= 0x7fffffff ffffffffLL.              |
|_________________________________________________________________________________________________|
*/
Word64 W_mac_32_16 (Word64  L64_var1, Word32 L_var2, Word16 var3) { 
  Word64  L64_var_out = ((Word64) L_var2*var3) << 1; 
  L64_var_out += L64_var1;
#if (WMOPS) 
  multiCounter[currCounter].W_mac_32_16++;
#endif /* if WMOPS */ 
  return L64_var_out;
} 


/*_________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_msu_32_16                                                                   |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply L_var2 by var3 and shift the result left by 1. Subtract the 64 bit                   |
|   result from L64_var1, return a 64 bit result.                                                 |
|                                                                                                 |
|   Complexity weight : 1                                                                         |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    L64_var1                                                                                     |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.                 |
|                                                                                                 |
|    L_var2                                                                                       |
|             32 bit signed integer (Word32) whose value falls in the                             |
|             range : 0x8000 0000 <= L_var2 <= 0x7fff ffff                                        |
|                                                                                                 |
|    var3                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var3 <= 0x0000 7fff.                                         |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    L64_var_out                                                                                  |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var_out <= 0x7fffffff ffffffffLL.              |
|_________________________________________________________________________________________________|
*/
Word64 W_msu_32_16(Word64  L64_var1, Word32 L_var2, Word16 var3) { 
  Word64  L64_var_out = ((Word64 )L_var2*var3) << 1; 
  L64_var_out = L64_var1 - L64_var_out;
#if (WMOPS) 
  multiCounter[currCounter].W_msu_32_16++;
#endif /* if WMOPS */
  return L64_var_out;
} 


/*_________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_mult_32_16                                                             |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply L_var1 by var2 and shift the result left by 1. Return a 64 bit result.               |
|                                                                                                 |
|   Complexity weight : 1                                                                         |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    L_var1                                                                                       |
|             32 bit signed integer (Word32) whose value falls in the                             |
|             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff                                        |
|                                                                                                 |
|    var2                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var2 <= 0x0000 7fff.                                         |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    L64_var_out                                                                                  |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var_out <= 0x7fffffff ffffffffLL.              |
|_________________________________________________________________________________________________|
*/
Word64 W_mult_32_16(Word32 L_var1, Word16 var2) { 
  Word64  L64_var_out = ((Word64 )L_var1*var2) << 1;
#if (WMOPS) 
  multiCounter[currCounter].W_mult_32_16++;
#endif /* if WMOPS */
  return L64_var_out;
} 

/*________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_mult0_16_16                                                            |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply var1 by var2 and return a 64 bit result.                                             |
|                                                                                                 |
|   Complexity weight : 1                                                                         |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    var1                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff                                        |
|                                                                                                 |
|    var2                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                                         |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    L64_var_out                                                                                  |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_acc <= 0x7fffffff ffffffffLL.                  |
|_________________________________________________________________________________________________|
*/
Word64 W_mult0_16_16(Word16 var1, Word16 var2) { 
  Word64  L64_var_out = (Word64 )var1*var2;
#if (WMOPS) 
  multiCounter[currCounter].W_mult0_16_16++;
#endif /* if WMOPS */
  return L64_var_out;
} 

/*________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_mac0_16_16                                                                  |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply var2 by var3 and add the 64 bit result to L64_var1, return a 64 bit result.          |
|                                                                                                 |
|   Complexity weight : 1                                                                         |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    L64_var1                                                                                     |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.                 |
|                                                                                                 |
|    var2                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var2 <= 0x0000 7fff.                                         |
|                                                                                                 |
|    var3                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var3 <= 0x0000 7fff.                                         |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    L64_var_out                                                                                  |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var_out <= 0x7fffffff ffffffffLL.              |
|_________________________________________________________________________________________________|
*/
Word64 W_mac0_16_16(Word64 L64_var1, Word16 var2, Word16 var3) { 
  Word64  L64_var_out = (Word64 )var2*var3; 
  L64_var_out += L64_var1;
#if (WMOPS) 
  multiCounter[currCounter].W_mac0_16_16++;
#endif /* if WMOPS */
  return L64_var_out;
} 


/*________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_msu0_16_16                                                                  |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply var2 by var3 and subtract the 64 bit result from L64_var1, return a 64 bit result.   |
|                                                                                                 |
|   Complexity weight : 1                                                                         |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    L64_var1                                                                                     |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.                 |
|                                                                                                 |
|    var2                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var2 <= 0x0000 7fff.                                         |
|                                                                                                 |
|    var3                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var3 <= 0x0000 7fff.                                         |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    L64_var_out                                                                                  |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var_out <= 0x7fffffff ffffffffLL.              |
|_________________________________________________________________________________________________|
*/
Word64 W_msu0_16_16 (Word64 L64_var1, Word16 var2, Word16 var3) { 
  Word64  L64_var_out = (Word64) var2*var3; 
  L64_var_out = L64_var1 - L64_var_out;
#if (WMOPS) 
  multiCounter[currCounter].W_msu0_16_16++;
#endif /* if WMOPS */
  return L64_var_out;
} 



/*_____________________________________________________________________________________
|                                                                                     |
|   Function Name : W_sat_l                                                           | 
|                                                                                     |
|   Purpose :                                                                         |
|                                                                                     |
|   Saturate the lower 32 bits of the 64 bit input number L64_var into 32 bits.       |
|                                                                                     |
|   Complexity weight : 1                                                             |
|                                                                                     |
|   Inputs :                                                                          |
|                                                                                     |
|    L64_var                                                                          |
|             64 bit long long signed integer (Word64) whose value falls in the      |
|             range : 0x80000000 00000000LL <= L64_var <= 0x7fffffff ffffffffLL.      |
|                                                                                     |
|   Outputs :                                                                         |
|                                                                                     |
|    none                                                                             |
|                                                                                     |
|   Return Value :                                                                    |
|                                                                                     |
|    L_var_out                                                                        |
|             32 bit signed integer (Word32) whose value falls in the                 |
|             range : 0x8000 0000 <= L_var_out <= 0x7fff 0000.                        |
|_____________________________________________________________________________________|
*/
Word32 W_sat_l (Word64 L64_var) {
  Word32 L_var_out;
 
  if (L64_var > 0x7FFFFFFF) { 
    L_var_out = 0x7FFFFFFF; 
  } 
  else if (L64_var < (int)0x80000000) { 
    L_var_out = 0x80000000; 
  } 
  else { 
    L_var_out = (Word32)L64_var; 
  }

#if (WMOPS) 
  multiCounter[currCounter].W_sat_l++;
#endif /* if WMOPS */
 
  return L_var_out;
}



/*___________________________________________________________________________________
|                                                                                     |
|   Function Name : W_sat_m                                                           | 
|                                                                                     |
|   Purpose :                                                                         |
|                                                                                     |
|   Truncates the lower 16 bits of the 64 bit input L64_var                           |
|   and saturates the number into 32 bits.                                            |
|                                                                                     |
|   Complexity weight : 1                                                             |
|                                                                                     |
|   Inputs :                                                                          |
|                                                                                     |
|    L64_var                                                                          |
|             64 bit long long signed integer (Word64) whose value falls in the      |
|             range : 0x80000000 00000000LL <= L64_var <= 0x7fffffff ffffffffLL.      |
|                                                                                     |
|   Outputs :                                                                         |
|                                                                                     |
|    none                                                                             |
|                                                                                     |
|   Return Value :                                                                    |
|                                                                                     |
|    L_var_out                                                                        |
|             32 bit signed integer (Word32) whose value falls in the                 |
|             range : 0x8000 0000 <= L_var_out <= 0x7fff 0000.                        |
|_____________________________________________________________________________________|
*/
Word32 W_sat_m (Word64 L64_var) {
  Word32 L_var_out;
 
  L64_var = L64_var >> 16; 
  L_var_out = W_sat_l (L64_var);

#if (WMOPS) 
  multiCounter[currCounter].W_sat_l--; 
  multiCounter[currCounter].W_sat_m++;
#endif /* if WMOPS */
 
  return L_var_out;
}


/*__________________________________________________________________________________
|                                                                                  |
|   Function Name : W_deposit32_l                                                    |
|                                                                                  |
|   Purpose :                                                                      |
|                                                                                  |
|   Deposit the 32 bit L_var1 into the 32 LS bits of the 64 bit output. The        |
|   32 MS bits of the output are sign extended.                                    |
|                                                                                  |
|   Complexity weight : 1                                                          |
|                                                                                  |
|   Inputs :                                                                       |
|                                                                                  |
|    L_var1                                                                        |
|             32 bit long signed integer (Word32) whose value falls in the         |
|             range : 0x8000 0000 <= var_out <= 0x7fff 0000.                       |                                                                           |
|   Outputs :                                                                      |
|                                                                                  |
|    none                                                                          |
|                                                                                  |
|   Return Value :                                                                 |
|                                                                                  |
|    L64_var_out                                                                   |
|             64 bit long long signed integer (Word64) whose value falls in        |
|             range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.  |
|__________________________________________________________________________________|
*/
Word64 W_deposit32_l (Word32 L_var1) { 
  Word64 L64_var_out;
 
  L64_var_out = (Word64) L_var1;

#if (WMOPS) 
  multiCounter[currCounter].W_deposit32_l++;
#endif
 
  return (L64_var_out);
}


/*__________________________________________________________________________________
|                                                                                  |
|   Function Name : W_deposit32_h                                                    |
|                                                                                  |
|   Purpose :                                                                      |
|                                                                                  |
|   Deposit the 32 bit L_var1 into the 32 MS bits of the 64 bit output. The        |
|   32 LS bits of the output are zeroed.                                           |
|                                                                                  |
|   Complexity weight : 1                                                          |
|                                                                                  |
|   Inputs :                                                                       |
|                                                                                  |
|    L_var1                                                                        |
|             32 bit long signed integer (Word32) whose value falls in the         |
|             range : 0x8000 0000 <= var_out <= 0x7fff 0000.                       |                                                                           |
|   Outputs :                                                                      |
|                                                                                  |
|    none                                                                          |
|                                                                                  |
|   Return Value :                                                                 |
|                                                                                  |
|    L64_var_out                                                                   |
|             64 bit long long signed integer (Word64) whose value falls in        |
|             range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.  |
|__________________________________________________________________________________|
*/
Word64 W_deposit32_h (Word32 L_var1) { 
  Word64 L64_var_out;
 
  L64_var_out = (Word64) L_var1 << 32;

#if (WMOPS) 
  multiCounter[currCounter].W_deposit32_h++;
#endif
 
  return (L64_var_out);
}


/*__________________________________________________________________________________
|                                                                                  |
|   Function Name : W_extract_l                                                    |
|                                                                                  |
|   Purpose :                                                                      |
|                                                                                  |
|   Return the 32 LSB of L64_var1.                                                 |
|                                                                                  |
|   Complexity weight : 1                                                          |
|                                                                                  |
|   Inputs :                                                                       |
|                                                                                  |
|    L64_var1                                                                      |
|             64 bit long long signed integer (Word64) whose value falls in        |
|             range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.  |
|                                                                                  |
|   Outputs :                                                                      |
|                                                                                  |
|    none                                                                          |
|                                                                                  |
|   Return Value :                                                                 |
|                                                                                  |
|    L_var_out                                                                     |
|             32 bit long signed integer (Word32) whose value falls in the         |
|             range : 0x8000 0000 <= L_var_out <= 0x7fff 0000.                     |
|__________________________________________________________________________________|
*/
Word32 W_extract_l (Word64 L64_var1) {
  Word32 L_var_out;
 
  L_var_out = (Word32) L64_var1;

#if (WMOPS) 
  multiCounter[currCounter].W_extract_l++;
#endif /* if WMOPS */
 
  return (L_var_out);
}



/*__________________________________________________________________________________
|                                                                                  |
|   Function Name : W_extract_h                                                    |
|                                                                                  |
|   Purpose :                                                                      |
|                                                                                  |
|   Return the 32 MSB of L64_var1.                                                 |
|                                                                                  |
|   Complexity weight : 1                                                          |
|                                                                                  |
|   Inputs :                                                                       |
|                                                                                  |
|    L64_var1                                                                      |
|             64 bit long long signed integer (Word64) whose value falls in        |
|             range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.  |
|                                                                                  |
|   Outputs :                                                                      |
|                                                                                  |
|    none                                                                          |
|                                                                                  |
|   Return Value :                                                                 |
|                                                                                  |
|    L_var_out                                                                     |
|             32 bit long signed integer (Word32) whose value falls in the         |
|             range : 0x8000 0000 <= L_var_out <= 0x7fff 0000.                     |
|__________________________________________________________________________________|
*/
Word32 W_extract_h (Word64 L64_var1) {
  Word32 L_var_out;
 
  L_var_out = (Word32) (L64_var1 >> 32);

#if (WMOPS) 
  multiCounter[currCounter].W_extract_h++;
#endif /* if WMOPS */
 
  return (L_var_out);
}



/*________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_mult_16_16                                                                  |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply var1 by var2 and shift the result left by 1. Return a 64 bit result.                 |
|                                                                                                 |
|   Complexity weight : 1                                                                         |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    var1                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff                                        |
|                                                                                                 |
|    var2                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                                         |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    L64_var_out                                                                                  |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_acc <= 0x7fffffff ffffffffLL.                  |
|_________________________________________________________________________________________________|
*/
Word64 W_mult_16_16 (Word16 var1, Word16 var2) { 
  Word64  L64_var_out = ((Word64) var1*var2) << 1;
#if (WMOPS) 
  multiCounter[currCounter].W_mult_16_16++;
#endif /* if WMOPS */
  return L64_var_out;
} 
/*________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_mac_16_16                                                                   |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply var1 by var2 and shift the result left by 1 and add the 64 bit result to L64_acc,    |
|   return a 64 bit result.                                                                       |
|                                                                                                 |
|   Complexity weight : 1                                                                         |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    L64_acc                                                                                      |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_acc <= 0x7fffffff ffffffffLL.                  |
|                                                                                                 |
|    var1                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                                         |
|                                                                                                 |
|    var2                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var2 <= 0x0000 7fff.                                         |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    L64_var_out                                                                                  |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var_out <= 0x7fffffff ffffffffLL.              |
|_________________________________________________________________________________________________|
*/
Word64 W_mac_16_16 (Word64 L64_acc, Word16 var1, Word16 var2) { 
  Word64  L64_var_out = ((Word64) var1*var2) << 1;
  L64_acc = L64_acc + L64_var_out;

#if (WMOPS) 
  multiCounter[currCounter].W_mac_16_16++;
#endif /* if WMOPS */
  return L64_acc;
} 



/*________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_msu_16_16                                                                   |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply var2 by var3 and shift the result left by1 and subtract the 64 bit result            |
|   from L64_var1, return a 64 bit result.                                                        |
|                                                                                                 |
|   Complexity weight : 1                                                                         |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    L64_var1                                                                                     |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.                 |
|                                                                                                 |
|    var2                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var2 <= 0x0000 7fff.                                         |
|                                                                                                 |
|    var3                                                                                         |
|             16 bit short signed integer (Word16) whose value falls in the                       |
|             range : 0xffff 8000 <= var3 <= 0x0000 7fff.                                         |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    L64_var_out                                                                                  |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var_out <= 0x7fffffff ffffffffLL.              |
|_________________________________________________________________________________________________|
*/
Word64 W_msu_16_16 (Word64 L64_var1, Word16 var2, Word16 var3) { 
  Word64  L64_var_out = ((Word64)var2*var3) << 1; 
  L64_var_out = L64_var1 - L64_var_out;
#if (WMOPS) 
  multiCounter[currCounter].W_msu_16_16++;
#endif /* if WMOPS */
  return L64_var_out;
} 


/*___________________________________________________________________________
|                                                                           |
|   Function Name : W_shl_sat_l                                             |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
|   Arithmetically shift the 64 bit input L64_var left by n positions with  | 
|   lower 32 bit saturation and return the 32 LSB of 64 bit result          |
|                                                                           |
|   Complexity weight : 1                                                   |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|    L64_var                                                                |
|       64 bit long signed integer (Word64) whose value falls in the        |
|       range : 0x80000000 00000000LL <= L64_var <= 0x7fffffff ffffffffLL.  |
|                                                                           |
|    n                                                                      |
|             32 bit signed integer (Word32) whose value falls in the       |
|             range : 0x8000 0000 <= n <= 0x7fff 0000.                      |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|    L_result                                                               |
|             32 bit long signed integer (Word32) whose value falls in the  |
|             range : 0x8000 0000 <= L_result <= 0x7fff 0000.               |
|___________________________________________________________________________|
*/
Word32 W_shl_sat_l (Word64 L64_var, Word32 n) { 
  Word32 L_result;
  Word64 d_var_64;
 
  d_var_64 = W_shl (L64_var, n);
  L_result = W_sat_l (d_var_64);

#if (WMOPS) 
  multiCounter[currCounter].W_shl_sat_l++; 
  multiCounter[currCounter].W_shl--; 
  multiCounter[currCounter].W_sat_l--;
#endif /* if WMOPS */
 
  return L_result;
}


/*__________________________________________________________________________________
|                                                                                  |
|   Function Name : W_round48_L                                                    |
|                                                                                  |
|   Purpose :                                                                      |
|                                                                                  |
|  Round asymmetrically lower 16 bits, and                                         | 
|  saturate the 17.47-bit values to 1.31-bit values                                | 
|   Complexity weight : 1                                                          |
|                                                                                  |
|   Inputs :                                                                       |
|                                                                                  |
|    L64_var1                                                                      |
|             64 bit long long signed integer (Word64) whose value falls in        |
|             range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.  |
|                                                                                  |
|   Outputs :                                                                      |
|                                                                                  |
|    none                                                                          |
|                                                                                  |
|   Return Value :                                                                 |
|                                                                                  |
|    L_var_out                                                                     |
|             32 bit long signed integer (Word32) whose value falls in the         |
|             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.                     |
|__________________________________________________________________________________|
*/
Word32 W_round48_L (Word64 L64_var1) { 
  Word64 L64_var_out; 
  Word32 L_result;
 
  Word64 L64_var2 = 0x80000000; 
  Word64 L64_MIN  = 0x8000000000000000LL; 
  Word64 L64_MAX  = 0x7FFFFFFFFFFFFFFFLL;
 
  L64_var1  = W_shl (L64_var1, 16);
 
  L64_var_out = L64_var1 + L64_var2;
 
  if ( ( (L64_var1 ^ L64_var2) & L64_MIN) == 0) { 
    if ( (L64_var_out ^ L64_var1) & L64_MIN) { 
      L64_var_out = (L64_var1 < 0) ? L64_MIN : L64_MAX; 
      Overflow = 1; 
    } 
  }
  L_result = W_extract_h (L64_var_out);
#if (WMOPS) 
  multiCounter[currCounter].W_round48_L++; 
  multiCounter[currCounter].W_extract_h--; 
  multiCounter[currCounter].W_shl--;
#endif /* if WMOPS */
 
  return (L_result);
}

/*__________________________________________________________________________________
|                                                                                  |
|   Function Name : W_round32_s                                                    |
|                                                                                  |
|   Purpose :                                                                      |
|                                                                                  |
|  Round asymmetrically lower 32 bits, and                                         | 
|  saturate the 17.47-bit values to 1.15-bit values                                | 
|   Complexity weight : 1                                                          |
|                                                                                  |
|   Inputs :                                                                       |
|                                                                                  |
|    L64_var1                                                                      |
|             64 bit long long signed integer (Word64) whose value falls in        |
|             range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.  |
|                                                                                  |
|   Outputs :                                                                      |
|                                                                                  |
|    none                                                                          |
|                                                                                  |
|   Return Value :                                                                 |
|                                                                                  |
|    var_out                                                                       |
|             16 bit short signed integer (Word16) whose value falls in the        |
|             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                       |
|__________________________________________________________________________________|
*/
Word16 W_round32_s (Word64 L64_var1) { 
  Word64 L64_var_out; 
  Word32 L_var; 
  Word16 var_out;

 
  Word64 L64_var2 = 0x800000000000LL; 
  Word64 L64_MIN  = 0x8000000000000000LL; 
  Word64 L64_MAX  = 0x7FFFFFFFFFFFFFFFLL;
 
  L64_var1  = W_shl (L64_var1, 16);
  L64_var_out = L64_var1 + L64_var2;
 
  if (((L64_var1 ^ L64_var2) & L64_MIN) == 0) { 
    if ((L64_var_out ^ L64_var1) & L64_MIN) { 
      L64_var_out = (L64_var1 < 0) ? L64_MIN : L64_MAX; 
      Overflow = 1; 
    } 
  } 
  L_var = W_extract_h (L64_var_out); 
  var_out = extract_h (L_var);
#if (WMOPS) 
  multiCounter[currCounter].W_round32_s ++; 
  multiCounter[currCounter].W_extract_h--; 
  multiCounter[currCounter].extract_h--; 
  multiCounter[currCounter].W_shl--;
#endif /* if WMOPS */
 
  return (var_out);
}
/*___________________________________________________________________________
|                                                                           |
|   Function Name : W_norm                                                  |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
|   Produces the number of left shifts needed to normalize the 64 bit varia-|
|   ble L64_var1.
|                                                                           |
|   Complexity weight : 1                                                   |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|  L64_var1                                                                 |
|      64 bit long long signed integer (Word64) whose value falls in        |
|      range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.  |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|    var_out                                                                |
|             16 bit short signed integer (Word16) whose value falls in the |
|             range : 0x0000 0000 <= var_out <= 0x0000 003f.                |
|___________________________________________________________________________|
*/
Word16 W_norm (Word64 L64_var1) { 
  Word16 var_out; 
  if (L64_var1 == 0) { 
    var_out = 0; 
  } 
  else { 
    if (L64_var1 == (Word64) 0xffffffffffffffffLL) { 
      var_out = 63; 
    } 
    else { 
      if (L64_var1 < 0) { 
        L64_var1 = ~L64_var1; 
      }
      for (var_out = 0; L64_var1 < (Word64) 0x4000000000000000LL; var_out++) { 
        L64_var1 <<= 1; 
      } 
    } 
  }
#if (WMOPS) 
  multiCounter[currCounter].W_norm ++;
#endif /* if WMOPS */
  return (var_out);
}



/*______________________________________________________________________________
|                                                                               |
|   Function Name : W_add                                                       |
|                                                                               |
|   Purpose :                                                                   |
|                                                                               |
|   64 bits addition of the two 64 bits variables (L64_var1+L64_var2)  with     |
|   overflow control and saturation; the result is set at 0x7fffffffffffffffLL  |
|   when overflow occurs or at 0x8000000000000000LL when underflow occurs.      |
|                                                                               |
|   Complexity weight : 1                                                       |
|                                                                               |
|   Inputs :                                                                    |
|                                                                               |
|   L64_var1   64 bit long signed integer (Word64) whose value falls in the     |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.     |
|                                                                               |
|   L64_var2   64 bit long signed integer (Word64) whose value falls in the     |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.     |
|                                                                               |
|   Outputs :                                                                   |
|                                                                               |
|    none                                                                       |
|                                                                               |
|   Return Value :                                                              |
|                                                                               |
|    L64_var_out                                                                |
|             64 bit long signed integer (Word64) whose value falls in the      |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.     |
|_______________________________________________________________________________|
*/
Word64 W_add (Word64 L64_var1, Word64 L64_var2) { 
  Word64 L64_var_out;
 
  L64_var_out = L64_var1 + L64_var2;

    if (((L64_var1 ^ L64_var2) & MIN_64) == 0) {
        if ((L64_var_out ^ L64_var1) & MIN_64) {
            L64_var_out = (L64_var1 < 0) ? MIN_64 : MAX_64;
            Overflow = 1;
        }
    }

#if (WMOPS) 
  multiCounter[currCounter].W_add++;
#endif 
  return L64_var_out;
}

/*______________________________________________________________________________
|                                                                               |
|   Function Name : W_sub                                                       |
|                                                                               |
|   Purpose :                                                                   |
|                                                                               |
|   64 bits subtraction of the two 64 bits variables (L64_var1-L64_var2)  with  |
|   overflow control and saturation; the result is set at 0x7fffffffffffffffLL  |
|   when overflow occurs or at 0x8000000000000000LL when underflow occurs.      |
|                                                                               |
|   Complexity weight : 1                                                       |
|                                                                               |
|   Inputs :                                                                    |
|                                                                               |
|   L64_var1   64 bit long signed integer (Word64) whose value falls in the     |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.     |
|                                                                               |
|   L64_var2   64 bit long signed integer (Word64) whose value falls in the     |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.     |
|                                                                               |
|   Outputs :                                                                   |
|                                                                               |
|    none                                                                       |
|                                                                               |
|   Return Value :                                                              |
|                                                                               |
|    L64_var_out                                                                |
|             64 bit long signed integer (Word64) whose value falls in the      |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.     |
|_______________________________________________________________________________|
*/
Word64 W_sub (Word64 L64_var1, Word64 L64_var2) { 
  Word64 L64_var_out;
 
  L64_var_out = L64_var1 - L64_var2;

    if (((L64_var1 ^ L64_var2) & MIN_64) != 0) {
        if ((L64_var_out ^ L64_var1) & MIN_64) {
            L64_var_out = (L64_var1 < 0) ? MIN_64 : MAX_64;
            Overflow = 1;
        }
    }

#if (WMOPS) 
  multiCounter[currCounter].W_add++;
#endif 
  return L64_var_out;
}


/*______________________________________________________________________________
|                                                                               |
|   Function Name : W_neg                                                       |
|                                                                               |
|   Purpose :                                                                   |
|                                                                               |
|   Negate the 64 bit variable L64_var1 with overflow control and saturation;   |
|   Saturate and set overflow in the case where input is 0x8000000000000000LL.  |
|                                                                               |
|   Complexity weight : 1                                                       |
|                                                                               |
|   Inputs :                                                                    |
|                                                                               |
|   L64_var1   64 bit long signed integer (Word64) whose value falls in the     |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.     |
|                                                                               |
|   Outputs :                                                                   |
|                                                                               |
|    none                                                                       |
|                                                                               |
|   Return Value :                                                              |
|                                                                               |
|    L64_var_out                                                                |
|             64 bit long signed integer (Word64) whose value falls in the      |
|       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.     |
|_______________________________________________________________________________|
*/
Word64 W_neg (Word64 L64_var1) {
    Word64 L64_var_out;
     
  if (L64_var1 == MIN_64) { 
    L64_var_out = MAX_64;
        Overflow = 1; 
  } 
  else { 
    L64_var_out = -L64_var1; 
  }

#if (WMOPS)
    multiCounter[currCounter].W_neg++;
#endif
    
    return (L64_var_out);

}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : W_abs                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Absolute value of L64_var1; Saturate in case where the input is        |
 |    0x8000000000000000LL                                                   |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |   L64_var1   64 bit long signed integer (Word64) whose value falls in the |
 |       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L64_var_out                                                            |
 |             64 bit long signed integer (Word64) whose value falls in the  |
 |       range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL. |
 |___________________________________________________________________________|
*/
Word64 W_abs (Word64 L64_var1) {
    Word64 L64_var_out;

    if (L64_var1 == MIN_64) {
        L64_var_out = MAX_64; 
    Overflow = 1;
    }
    else {
        if (L64_var1 < 0) {
            L64_var_out = -L64_var1;
        }
        else {
            L64_var_out = L64_var1;
        }
    }

#if (WMOPS)
    multiCounter[currCounter].W_abs++;
#endif

    return (L64_var_out);
}

/*_________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_mult_32_32                                                                  |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply L_var1 by L_var2 and shift the result left by 1.Saturate and set overflow in case    |
|    where both inputs are 0x80000000 .  Return a 64 bit result.                                  |
|                                                                                                 |
|   Complexity weight : 1                                                                         |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    L_var1                                                                                       |
|             32 bit signed integer (Word32) whose value falls in the                             |
|             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff                                        |
|                                                                                                 |
|    L_var2                                                                                       |
|             32 bit signed integer (Word32) whose value falls in the                             |
|             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff                                        |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    L64_var_out                                                                                  |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var_out <= 0x7fffffff ffffffffLL.              |
|_________________________________________________________________________________________________|
*/
Word64 W_mult_32_32(Word32 L_var1, Word32 L_var2) { 
  Word64  L64_var_out; 
   
  if ((L_var1 == MIN_32) && (L_var2 == MIN_32)) { 
    L64_var_out = MAX_64; 
    Overflow = 1; 
  } 
  else { 
    L64_var_out = ((Word64 )L_var1*L_var2) << 1; 
  }

#if (WMOPS) 
  multiCounter[currCounter].W_mult_32_32++;
#endif /* if WMOPS */ 
  return L64_var_out;
}

/*_________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_mult0_32_32                                                                 |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply L_var1 by L_var2.  Return a 64 bit result.                                           |
|                                                                                                 |
|   Complexity weight : 1                                                                         |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    L_var1                                                                                       |
|             32 bit signed integer (Word32) whose value falls in the                             |
|             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff                                        |
|                                                                                                 |
|    L_var2                                                                                       |
|             32 bit signed integer (Word32) whose value falls in the                             |
|             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff                                        |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    L64_var_out                                                                                  |
|             64 bit long long signed integer (Word64) whose value falls in the                   |
|             range : 0x80000000 00000000LL <= L64_var_out <= 0x7fffffff ffffffffLL.              |
|_________________________________________________________________________________________________|
*/
Word64 W_mult0_32_32 (Word32 L_var1, Word32 L_var2) { 
  Word64  L64_var_out;
 
  L64_var_out = (Word64) L_var1*L_var2;


#if (WMOPS) 
  multiCounter[currCounter].W_mult0_32_32++;
#endif /* if WMOPS */
  return L64_var_out;
}


/*_____________________________________________________________________________
|                                                                              |
|   Function Name : W_lshl                                                     |
|                                                                              |
|   Purpose :                                                                  |
|                                                                              |
|   Logically shift the 64 bit unsigned input L64_var1 left by var2 positions. | 
|   Zero fill the var2 LSB of the result. If var2 is negative, logically shift |
|   L64_var1 right by -var2 with zero fill in the MSB.                         |
|                                                                              |
|   Complexity weight : 1                                                      |
|                                                                              |
|   Inputs :                                                                   |
|                                                                              |
|    L64_var1  64 bit long unsigned signed integer (UWord64) whose value falls | 
|    in the range : 0LL <= L64_var1 <= 0xffffffff ffffffffLL.                  |
|                                                                              |
|    var2                                                                      |
|             16 bit short signed integer (Word16) whose value falls in the    |
|             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                      |
|                                                                              |
|   Outputs :                                                                  |
|                                                                              |
|    none                                                                      |
|                                                                              |
|   Return Value :                                                             |
|                                                                              |
|    L64_var_out                                                               |
|             64 bit long unsigned signed integer (UWord64) whose value falls  | 
|    in the range : 0LL <= L64_var1 <= 0xffffffff ffffffffLL.                  |
|______________________________________________________________________________|
*/
UWord64 W_lshl (UWord64 L64_var1, Word16 var2) {
 
  UWord64 L64_var_out = 0LL;
 
  if (var2 < 0) { 
    L64_var_out = L64_var1 >> (-var2); 
  } 
  else { 
    L64_var_out = L64_var1 << var2 ; 
  }
#if (WMOPS) 
  multiCounter[currCounter].W_lshl++;
#endif
 
  return (L64_var_out);
}

/*_____________________________________________________________________________
|                                                                              |
|   Function Name : W_lshr                                                     |
|                                                                              |
|   Purpose :                                                                  |
|                                                                              |
|   Logically shift the 64 bit unsigned input L64_var1 right by var2 positions.|
|   Zero fill the var2 MSB of the result. If var2 is negative, logically shift |
|   L64_var1 left by -var2 with zero fill in the LSB.                          |
|                                                                              |
|   Complexity weight : 1                                                      |
|                                                                              |
|   Inputs :                                                                   |
|                                                                              |
|    L64_var1  64 bit long unsigned signed integer (UWord64) whose value falls | 
|    in the range : 0LL <= L64_var1 <= 0xffffffff ffffffffLL.                  |
|                                                                              |
|    var2                                                                      |
|             16 bit short signed integer (Word16) whose value falls in the    |
|             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                      |
|                                                                              |
|   Outputs :                                                                  |
|                                                                              |
|    none                                                                      |
|                                                                              |
|   Return Value :                                                             |
|                                                                              |
|    L64_var_out                                                               |
|             64 bit long unsigned signed integer (UWord64) whose value falls  | 
|    in the range : 0LL <= L64_var1 <= 0xffffffff ffffffffLL.                  |
|______________________________________________________________________________|
*/
UWord64 W_lshr (UWord64 L64_var1, Word16 var2) {
 
  UWord64 L64_var_out = 0LL;
 
  if (var2 < 0) { 
    L64_var_out = L64_var1 << (-var2); 
  } 
  else { 
    L64_var_out = L64_var1 >> var2 ; 
  }
#if (WMOPS) 
  multiCounter[currCounter].W_lshr++;
#endif
 
  return (L64_var_out);
}


/*__________________________________________________________________________________
|                                                                                  |
|   Function Name : W_round64_L                                                    |
|                                                                                  |
|   Purpose :                                                                      |
|                                                                                  |
|  Round asymmetrically lower 32 bits, and                                         | 
|  saturate the 1.63-bit values to 1.31-bit values                                 | 
|   Complexity weight : 1                                                          |
|                                                                                  |
|   Inputs :                                                                       |
|                                                                                  |
|    L64_var1                                                                      |
|             64 bit long long signed integer (Word64) whose value falls in        |
|             range : 0x80000000 00000000LL <= L64_var1 <= 0x7fffffff ffffffffLL.  |
|                                                                                  |
|   Outputs :                                                                      |
|                                                                                  |
|    none                                                                          |
|                                                                                  |
|   Return Value :                                                                 |
|                                                                                  |
|    L_var_out                                                                     |
|             32 bit long signed integer (Word32) whose value falls in the         |
|             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.                     |
|__________________________________________________________________________________|
*/
Word32 W_round64_L (Word64 L64_var1) { 
  Word64 L64_var_out; 
  Word32 L_result;

 
  Word64 L64_tmp = 0x80000000; 
  L64_var_out = W_add (L64_var1, L64_tmp);
  L_result = W_extract_h (L64_var_out);

#if (WMOPS) 
  multiCounter[currCounter].W_round64_L++; 
  multiCounter[currCounter].W_extract_h--; 
  multiCounter[currCounter].W_add--;
#endif /* if WMOPS */
 
  return (L_result);
}

#endif /* #ifdef ENH_64_BIT_OPERATOR */

/* end of file */
