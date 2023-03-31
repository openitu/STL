/*                                                      v.2.3 - 30.Nov.2009
  =============================================================================

                          U    U   GGG    SSSS  TTTTT
                          U    U  G       S       T
                          U    U  G  GG   SSSS    T
                          U    U  G   G       S   T
                           UUU     GG     SSS     T

                   ========================================
                    ITU-T - USER'S GROUP ON SOFTWARE TOOLS
                   ========================================

       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
       CODING STANDARDS".
       =============================================================

MODULE:         BASOP32, BASIC OPERATORS

ORIGINAL BY:
                Incorporated from anonymous contributions for 
                ETSI Standards as well as G.723.1, G.729, and G.722.1

DESCRIPTION:
        This file contains the definition of 16- and 32-bit basic
        operators to be used in the implementation of signal
        processing algorithms. The basic operators try to resemble
        assembly language instructions that are commonly found in
        digital signal processor (DSP) CPUs, thus allowing algorithm
        C-code implementations more directly mapeable to DSP assembly
        code.

        *********************************************************
         NOTE: so far, this module does not have a demo program!
        *********************************************************

FUNCTIONS:
  Defined in basop32.h. Self-documentation within each function.

HISTORY:
  26.Jan.00    v1.0     Incorporated to the STL from updated G.723.1/G.729 
                        basic operator library (based on basicop2.c) and 
                        G.723.1's basop.c [L_mls(), div_l(), i_mult()]

  05.Jul.00    v1.1     Added 32-bit shiftless accumulation basic 
                        operators (L_msu0, L_mac0, L_mult0). Improved
                        documentation for i_mult().

   03 Nov 04   v2.0     Incorporation of new 32-bit / 40-bit / control
                        operators for the ITU-T Standard Tool Library as 
                        described in Geneva, 20-30 January 2004 WP 3/16 Q10/16
                        TD 11 document and subsequent discussions on the
                        wp3audio@yahoogroups.com email reflector.
                        norm_s()      weight reduced from 15 to 1.
                        norm_l()      weight reduced from 30 to 1.
                        L_abs()       weight reduced from  2 to 1.
                        L_add()       weight reduced from  2 to 1.
                        L_negate()    weight reduced from  2 to 1.
                        L_shl()       weight reduced from  2 to 1.
                        L_shr()       weight reduced from  2 to 1.
                        L_sub()       weight reduced from  2 to 1.
                        mac_r()       weight reduced from  2 to 1.
                        msu_r()       weight reduced from  2 to 1.
                        mult_r()      weight reduced from  2 to 1.
                        L_deposit_h() weight reduced from  2 to 1.
                        L_deposit_l() weight reduced from  2 to 1.
                        L_mls() weight of 5.
                        div_l() weight of 32.
                        i_mult() weight of 3.

   30 Nov 09   v2.3     round() function is now round_fx().
                        saturate() is not referencable from outside application
  =============================================================================
*/

/* 
Made thread-safe and optimized for EVS codec floating-point use. Modifications can be expanded or edited for other ITU and 3GPP ETSI codecs, as needed

Copyright (C) Signalogic, 2017-2023

Revision History

  Mar 2017 CKJ, thread-safe modifications:
                -remove/disable usage of global flags
                -add "xxx_ovf()" versions of some functions and pass Overflow as a stack param; e.g. sub_ovf()
  Sep 2022 - Mar 2023 JHB, implement mods for 3GPP EVS build:
                -if _EVS_ defined in Makefile, code looks for basop build control defines "USE_BASOPS_xxx" in options.h
                -enable static inline basops in basop32.h if USE_BASOPS_INLINE is defined
                -enable/disable Overflow and Carry global variables with USE_BASOPS_OVERFLOW_GLOBAL_VAR and USE_BASOPS_CARRY_GLOBAL_VAR
                -no indentation/formatting mods to original STL file outside of EVS related changes 
  Mar 2023 JHB, improve comments

EVS floating-point source files that reference basop32 fixed-point functions, Signalogic Mar 2023

encoder

-rw-r--r-- 1 root 58284 Mar  9 00:17 hq_lr_enc.c
-rw-r--r-- 1 root 15215 Mar  9 00:03 tcq_core_enc.c
-rw-r--r-- 1 root 53585 Mar  9 00:03 swb_bwe_enc.c
-rw-r--r-- 1 root 36691 Mar  9 00:03 gain_enc.c
-rw-r--r-- 1 root 21027 Mar  8 20:44 arith_coder_enc.c
-rw-r--r-- 1 root 23612 Mar  8 20:10 acelp_core_enc.c

decoder

-rw-r--r-- 1 root 15181 Mar  9 00:05 tcq_core_dec.c
-rw-r--r-- 1 root 19869 Mar  9 00:05 swb_bwe_dec.c
-rw-r--r-- 1 root 14462 Mar  9 00:05 pitch_extr.c
-rw-r--r-- 1 root 45447 Mar  9 00:04 hq_lr_dec.c
-rw-r--r-- 1 root 18228 Mar  9 00:04 FEC.c
-rw-r--r-- 1 root 38138 Mar  8 19:42 fd_cng_dec.c
-rw-r--r-- 1 root  6426 Mar  8 16:28 lead_deindexing.c

common

-rw-r--r-- 1 root   28177 Mar  9 00:08 basop_util.c
-rw-r--r-- 1 root   11576 Mar  9 00:08 basop_tcx_utils.c
-rw-r--r-- 1 root    1089 Mar  9 00:07 basop_mpy.c
-rw-r--r-- 1 root    8918 Mar  9 00:07 basop_lsf_tools.c  <-- uses xxx_ovf() functions
-rw-r--r-- 1 root    6188 Mar  9 00:06 basop_com_lpc.c
-rw-r--r-- 1 root   17546 Mar  9 00:06 arith_coder.c
-rw-r--r-- 1 root    8096 Mar  9 00:06 ari_hm.c
-rw-r--r-- 1 root   27969 Mar  9 00:02 bitalloc.c
-rw-r--r-- 1 root   15202 Mar  9 00:02 enh1632.c
-rw-r--r-- 1 root   30312 Mar  9 00:01 enh40.c
-rw-r--r-- 1 root  101268 Mar  9 00:01 fft.c
-rw-r--r-- 1 root   30888 Mar  9 00:00 hq2_bit_alloc.c
-rw-r--r-- 1 root   53042 Mar  9 00:00 tcq_position_arith.c
-rw-r--r-- 1 root   25352 Mar  9 00:00 pvq_com.c
-rw-r--r-- 1 root    3282 Mar  8 23:59 lerp.c
-rw-r--r-- 1 root   16471 Mar  8 23:59 hq2_core_com.c
-rw-r--r-- 1 root   20590 Mar  8 19:53 mslvq_com.c
-rw-r--r-- 1 root    8634 Mar  8 15:31 bits_alloc.c

*/

/*___________________________________________________________________________
 |                                                                           |
 | Basic arithmetic operators.                                               |
 |                                                                           |
 | $Id $                                                                     |
 |                                                                           |
 |       saturate()                                                          |
 |       add()                                                               |
 |       sub()                                                               |
 |       abs_s()                                                             |
 |       divide_s()                                                          |
 |       extract_h()                                                         |
 |       extract_l()                                                         |
 |       L_abs()                                                             |
 |       L_add()                                                             |
 |       L_deposit_h()                                                       |
 |       L_deposit_l()                                                       |
 |       L_mac()                                                             |
 |       L_msu()                                                             |
 |       L_mult()                                                            |
 |       L_negate()                                                          |
 |       L_shl()                                                             |
 |       L_shr()                                                             |
 |       L_sub()                                                             |
 |       mac_r()                                                             |
 |       msu_r()                                                             |
 |       mult()                                                              |
 |       mult_r()                                                            |
 |       negate()                                                            |
 |       norm_l()                                                            |
 |       norm_s()                                                            |
 |       round_fx()                                                          |
 |       shl()                                                               |
 |       shr()                                                               |
 |___________________________________________________________________________|
*/


/*___________________________________________________________________________
 |                                                                           |
 |   Include-Files                                                           |
 |___________________________________________________________________________|
*/
#ifdef _EVS_  /* _EVS_ should be defined in Makefile when building 3GPP EVS codec */
/*  added by 3GPP EVS authors, only applicable to reference code test program command-line operations; e.g. scanf()
  #ifndef _CRT_SECURE_NO_WARNINGS
  #define _CRT_SECURE_NO_WARNINGS
  #endif
*/
  #include "options.h"  /* see options.h for descriptions of USE_BASOPS_INLINE, EXCLUDE_BASOPS_NOT_USED_EVS, USE_BASOPS_OVERFLOW_GLOBAL_VAR, USE_BASOPS_CARRY_GLOBAL_VAR, and USE_BASOPS_EXIT, JHB Mar 2023 */
#endif

#include <stdio.h>  /* fprintf() */
#ifdef USE_BASOPS_EXIT
  #include <stdlib.h>  /* abort(), exit() */
#endif
#include "stl.h"


#if (WMOPS)
extern BASIC_OP multiCounter[MAXCOUNTERS];
extern int currCounter;
#endif


/*___________________________________________________________________________
 |                                                                           |
 |   Local Functions                                                         |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE  /* defined in options.h, JHB Mar 2023 */
static Word16 saturate (Word32 L_var1);
#endif
#ifdef _EVS_
static Word16 saturate_ovf(Word32 L_var1, Flag* Overflow);  /* Identical to saturate() but doesn't use global Overflow flag to be thread-safe - CKJ Mar 2017 */
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Constants and Globals                                                   |
 |___________________________________________________________________________|
*/
#ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
/* Remove global flags, replaced with stack vars as needed - CJ Mar 2017 */
Flag Overflow = 0;
#endif
#ifdef USE_BASOPS_CARRY_GLOBAL_VAR
Flag Carry = 0;
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Functions                                                               |
 |___________________________________________________________________________|
*/

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : saturate                                                |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Limit the 32 bit input to the range of a 16 bit word. Must NOT be      |
 |    referenced from outside applications.                                  |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE
static Word16 saturate (Word32 L_var1)
{
    Word16 var_out;

    if (L_var1 > 0X00007fffL)
    {
        #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
        Overflow = 1;
        #endif
        var_out = MAX_16;
    }
    else if (L_var1 < (Word32) 0xffff8000L)
    {
        #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
        Overflow = 1;
        #endif
        var_out = MIN_16;
    }
    else
    {
        var_out = extract_l (L_var1);
#if (WMOPS)
        multiCounter[currCounter].extract_l--;
#endif
    }

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}
#endif

#ifdef _EVS_
/* Identical to saturate() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */
static Word16 saturate_ovf(Word32 L_var1, Flag *Overflow)
{

Word16 var_out;

   if (L_var1 > 0X00007fffL)
   {
      *Overflow = 1;
      var_out = MAX_16;
   }
   else if (L_var1 < (Word32) 0xffff8000L)
   {
      *Overflow = 1;
      var_out = MIN_16;
   }
   else
   {
      var_out = extract_l(L_var1);
   }

   BASOP_CHECK();

   return (var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : add                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the addition (var1+var2) with overflow control and saturation;|
 |    the 16 bit result is set at +32767 when overflow occurs or at -32768   |
 |    when underflow occurs.                                                 |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE
Word16 add (Word16 var1, Word16 var2)
{
    Word16 var_out;
    Word32 L_sum;

    L_sum = (Word32) var1 + var2;
    var_out = saturate (L_sum);

#if (WMOPS)
    multiCounter[currCounter].add++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : sub                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the subtraction (var1+var2) with overflow control and satu-   |
 |    ration; the 16 bit result is set at +32767 when overflow occurs or at  |
 |    -32768 when underflow occurs.                                          |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE
Word16 sub (Word16 var1, Word16 var2)
{
    Word16 var_out;
    Word32 L_diff;

    L_diff = (Word32) var1 - var2;
    var_out = saturate (L_diff);

#if (WMOPS)
    multiCounter[currCounter].sub++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}
#endif

#ifdef _EVS_
/* Identical to sub() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */
Word16 sub_ovf(Word16 var1, Word16 var2, Flag *Overflow) {

Word16 var_out;
Word32 L_diff;

   L_diff = (Word32)var1 - var2;
   var_out = saturate_ovf(L_diff, Overflow);

   BASOP_CHECK();

   return var_out;
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : abs_s                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Absolute value of var1; abs_s(-32768) = 32767.                         |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
Word16 abs_s (Word16 var1)
{
    Word16 var_out;

    if (var1 == (Word16) MIN_16)	
    {
        var_out = MAX_16;
    }
    else
    {
        if (var1 < 0)
        {
            var_out = -var1;
        }
        else
        {
            var_out = var1;
        }
    }

#if (WMOPS)
    multiCounter[currCounter].abs_s++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : shl                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 16 bit input var1 left var2 positions.Zero fill|
 |   the var2 LSB of the result. If var2 is negative, arithmetically shift   |
 |   var1 right by -var2 with sign extension. Saturate the result in case of |
 |   underflows or overflows.                                                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE
Word16 shl (Word16 var1, Word16 var2)
{
    Word16 var_out;
    Word32 result;

    if (var2 < 0)
    {
        if (var2 < -16)
            var2 = -16;
        var2 = -var2;
        var_out = shr (var1, var2);

#if (WMOPS)
        multiCounter[currCounter].shr--;
#endif
    }
    else
    {
        result = (Word32) var1 *((Word32) 1 << var2);

        if ((var2 > 15 && var1 != 0) || (result != (Word32) ((Word16) result)))
        {
            #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
            Overflow = 1;
            #endif
            var_out = (var1 > 0) ? MAX_16 : MIN_16;
        }
        else
        {
            var_out = extract_l (result);

#if (WMOPS)
            multiCounter[currCounter].extract_l--;
#endif
        }
    }

#if (WMOPS)
    multiCounter[currCounter].shl++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}
#endif

#ifdef _EVS_
/* Identical to shl() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */
Word16 shl_ovf(Word16 var1, Word16 var2, Flag *Overflow)
{
Word16 var_out;
Word32 result;

   if (var2 < 0)
   {
      if (var2 < -16) var2 = -16;
      var2 = -var2;
      var_out = shr (var1, var2);
   }
   else
   {
      result = (Word32) var1 *((Word32) 1 << var2);

      if ((var2 > 15 && var1 != 0) || (result != (Word32) ((Word16) result)))
      {
         *Overflow = 1;
         var_out = (var1 > 0) ? MAX_16 : MIN_16;
      }
      else
      {
         var_out = extract_l (result);
      }
   }

   BASOP_CHECK();

   return (var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : shr                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 16 bit input var1 right var2 positions with    |
 |   sign extension. If var2 is negative, arithmetically shift var1 left by  |
 |   -var2 with sign extension. Saturate the result in case of underflows or |
 |   overflows.                                                              |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE
Word16 shr (Word16 var1, Word16 var2)
{
    Word16 var_out;

    if (var2 < 0)
    {
        if (var2 < -16)
            var2 = -16;
        var2 = -var2;
        var_out = shl (var1, var2);

#if (WMOPS)
        multiCounter[currCounter].shl--;
#endif
    }
    else
    {
        if (var2 >= 15)
        {
            var_out = (var1 < 0) ? -1 : 0;
        }
        else
        {
            if (var1 < 0)
            {
                var_out = ~((~var1) >> var2);
            }
            else
            {
                var_out = var1 >> var2;
            }
        }
    }

#if (WMOPS)
    multiCounter[currCounter].shr++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : mult                                                    |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the multiplication of var1 by var2 and gives a 16 bit result  |
 |    which is scaled i.e.:                                                  |
 |             mult(var1,var2) = extract_l(L_shr((var1 times var2),15)) and  |
 |             mult(-32768,-32768) = 32767.                                  |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
Word16 mult (Word16 var1, Word16 var2)
{
    Word16 var_out;
    Word32 L_product;

    L_product = (Word32) var1 *(Word32) var2;

    L_product = (L_product & (Word32) 0xffff8000L) >> 15;

    if (L_product & (Word32) 0x00010000L)
        L_product = L_product | (Word32) 0xffff0000L;

    var_out = saturate (L_product);

#if (WMOPS)
    multiCounter[currCounter].mult++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_mult                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   L_mult is the 32 bit result of the multiplication of var1 times var2    |
 |   with one shift left i.e.:                                               |
 |        L_mult(var1,var2) = L_shl((var1 times var2),1) and                 |
 |        L_mult(-32768,-32768) = 2147483647.                                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
Word32 L_mult (Word16 var1, Word16 var2)
{
    Word32 L_var_out;

    L_var_out = (Word32) var1 *(Word32) var2;

    if (L_var_out != (Word32) 0x40000000L)
    {
        L_var_out *= 2;
    }
    else
    {
        #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
        Overflow = 1;
        #endif
        L_var_out = MAX_32;
    }

#if (WMOPS)
    multiCounter[currCounter].L_mult++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}

#ifdef _EVS_
/* Identical to L_mult() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */
Word32 L_mult_ovf (Word16 var1, Word16 var2, Flag *Overflow)
{
Word32 L_var_out;

   L_var_out = (Word32) var1 *(Word32) var2;

   if (L_var_out != (Word32) 0x40000000L)
   {
      L_var_out *= 2;
   }
   else
   {
      *Overflow = 1;
      L_var_out = MAX_32;
   }

   BASOP_CHECK();

   return (L_var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : negate                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Negate var1 with saturation, saturate in the case where input is -32768:|
 |                negate(var1) = sub(0,var1).                                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE
Word16 negate (Word16 var1)
{
    Word16 var_out;

    var_out = (var1 == MIN_16) ? MAX_16 : -var1;

#if (WMOPS)
    multiCounter[currCounter].negate++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : extract_h                                               |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Return the 16 MSB of L_var1.                                            |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32 ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
#ifndef _EVS_  /* for EVS build declared as static inline in basop32.h, JHB Mar 2023 */
Word16 extract_h (Word32 L_var1)
{
    Word16 var_out;

    var_out = (Word16) (L_var1 >> 16);

#if (WMOPS)
    multiCounter[currCounter].extract_h++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : extract_l                                               |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Return the 16 LSB of L_var1.                                            |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32 ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
#ifndef _EVS_  /* for EVS build declared as static inline in basop32.h, JHB Mar 2023 */
Word16 extract_l (Word32 L_var1)
{
    Word16 var_out;

    var_out = (Word16) L_var1;

#if (WMOPS)
    multiCounter[currCounter].extract_l++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : round_fx                                                |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Round the lower 16 bits of the 32 bit input number into the MS 16 bits  |
 |   with saturation. Shift the resulting bits right by 16 and return the 16 |
 |   bit number:                                                             |
 |               round_fx(L_var1) = extract_h(L_add(L_var1,32768))           |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32 ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
Word16 round_fx (Word32 L_var1)
{
    Word16 var_out;
    Word32 L_rounded;

    L_rounded = L_add (L_var1, (Word32) 0x00008000L);
    var_out = extract_h (L_rounded);

#if (WMOPS)
    multiCounter[currCounter].L_add--;
    multiCounter[currCounter].extract_h--;
    multiCounter[currCounter].round++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_mac                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Add the 32 bit    |
 |   result to L_var3 with saturation, return a 32 bit result:               |
 |        L_mac(L_var3,var1,var2) = L_add(L_var3,L_mult(var1,var2)).         |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
Word32 L_mac (Word32 L_var3, Word16 var1, Word16 var2)
{
    Word32 L_var_out;
    Word32 L_product;

    L_product = L_mult (var1, var2);
    L_var_out = L_add (L_var3, L_product);

#if (WMOPS)
    multiCounter[currCounter].L_mult--;
    multiCounter[currCounter].L_add--;
    multiCounter[currCounter].L_mac++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_msu                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Subtract the 32   |
 |   bit result from L_var3 with saturation, return a 32 bit result:         |
 |        L_msu(L_var3,var1,var2) = L_sub(L_var3,L_mult(var1,var2)).         |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
Word32 L_msu (Word32 L_var3, Word16 var1, Word16 var2)
{
    Word32 L_var_out;
    Word32 L_product;

    L_product = L_mult (var1, var2);
    L_var_out = L_sub (L_var3, L_product);

#if (WMOPS)
    multiCounter[currCounter].L_mult--;
    multiCounter[currCounter].L_sub--;
    multiCounter[currCounter].L_msu++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_macNs                                                 |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Add the 32 bit    |
 |   result to L_var3 without saturation, return a 32 bit result. Generate   |
 |   carry and overflow values :                                             |
 |        L_macNs(L_var3,var1,var2) = L_add_c(L_var3,L_mult(var1,var2)).     |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |                                                                           |
 |   Caution :                                                               |
 |                                                                           |
 |    In some cases the Carry flag has to be cleared or set before using     |
 |    operators which take into account its value.                           |
 |___________________________________________________________________________|
*/
#ifndef EXCLUDE_BASOPS_NOT_USED_EVS
/* Exclude L_macNs() from build as it calls L_add_c() which reads/sets global Overflow/Carry flags. These functions aren't called anywhere in 3GPP EVS floating-point code - CJ March 2017 */
Word32 L_macNs (Word32 L_var3, Word16 var1, Word16 var2)
{
    Word32 L_var_out;

    L_var_out = L_mult (var1, var2);
    L_var_out = L_add_c (L_var3, L_var_out);

#if (WMOPS)
    multiCounter[currCounter].L_mult--;
    multiCounter[currCounter].L_add_c--;
    multiCounter[currCounter].L_macNs++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_msuNs                                                 |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Subtract the 32   |
 |   bit result from L_var3 without saturation, return a 32 bit result. Ge-  |
 |   nerate carry and overflow values :                                      |
 |        L_msuNs(L_var3,var1,var2) = L_sub_c(L_var3,L_mult(var1,var2)).     |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |                                                                           |
 |   Caution :                                                               |
 |                                                                           |
 |    In some cases the Carry flag has to be cleared or set before using     |
 |    operators which take into account its value.                           |
 |___________________________________________________________________________|
*/
#ifndef EXCLUDE_BASOPS_NOT_USED_EVS
/* Exclude L_msuNs() from build as it calls L_sub_c() which reads/sets global Overflow/Carry flags. These functions aren't called anywhere in 3GPP EVS floating-point code - CJ March 2017 */
Word32 L_msuNs (Word32 L_var3, Word16 var1, Word16 var2)
{
    Word32 L_var_out;

    L_var_out = L_mult (var1, var2);
    L_var_out = L_sub_c (L_var3, L_var_out);

#if (WMOPS)
    multiCounter[currCounter].L_mult--;
    multiCounter[currCounter].L_sub_c--;
    multiCounter[currCounter].L_msuNs++;

#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_add                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   32 bits addition of the two 32 bits variables (L_var1+L_var2) with      |
 |   overflow control and saturation; the result is set at +2147483647 when  |
 |   overflow occurs or at -2147483648 when underflow occurs.                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    L_var2   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE
Word32 L_add (Word32 L_var1, Word32 L_var2)
{
    Word32 L_var_out;

    L_var_out = L_var1 + L_var2;

    if (((L_var1 ^ L_var2) & MIN_32) == 0)
    {
        if ((L_var_out ^ L_var1) & MIN_32)
        {
            L_var_out = (L_var1 < 0) ? MIN_32 : MAX_32;
            #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
            Overflow = 1;
            #endif
        }
    }

#if (WMOPS)
    multiCounter[currCounter].L_add++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}
#endif

#ifdef _EVS_
/* Identical to L_add() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */
Word32 L_add_ovf (Word32 L_var1, Word32 L_var2, Flag *Overflow)
{
Word32 L_var_out;

   L_var_out = L_var1 + L_var2;

   if (((L_var1 ^ L_var2) & MIN_32) == 0)
   {
      if ((L_var_out ^ L_var1) & MIN_32)
      {
         L_var_out = (L_var1 < 0) ? MIN_32 : MAX_32;
         *Overflow = 1;
      }
   }

   BASOP_CHECK();

   return (L_var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_sub                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   32 bits subtraction of the two 32 bits variables (L_var1-L_var2) with   |
 |   overflow control and saturation; the result is set at +2147483647 when  |
 |   overflow occurs or at -2147483648 when underflow occurs.                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    L_var2   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE
Word32 L_sub (Word32 L_var1, Word32 L_var2)
{
    Word32 L_var_out;

    L_var_out = L_var1 - L_var2;

    if (((L_var1 ^ L_var2) & MIN_32) != 0)
    {
        if ((L_var_out ^ L_var1) & MIN_32)
        {
            L_var_out = (L_var1 < 0L) ? MIN_32 : MAX_32;
            #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
            Overflow = 1;
            #endif
        }
    }

#if (WMOPS)
    multiCounter[currCounter].L_sub++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}
#endif

#ifdef _EVS_
/* Identical to L_sub() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */
Word32 L_sub_ovf (Word32 L_var1, Word32 L_var2, Flag *Overflow)
{
Word32 L_var_out;

   L_var_out = L_var1 - L_var2;

   if (((L_var1 ^ L_var2) & MIN_32) != 0)
   {
      if ((L_var_out ^ L_var1) & MIN_32)
      {
         L_var_out = (L_var1 < 0L) ? MIN_32 : MAX_32;
         *Overflow = 1;
      }
   }

   BASOP_CHECK();

   return (L_var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_add_c                                                 |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Performs 32 bits addition of the two 32 bits variables (L_var1+L_var2+C)|
 |   with carry. No saturation. Generate carry and Overflow values. The car- |
 |   ry and overflow values are binary variables which can be tested and as- |
 |   signed values.                                                          |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    L_var2   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |                                                                           |
 |   Caution :                                                               |
 |                                                                           |
 |    In some cases the Carry flag has to be cleared or set before using     |
 |    operators which take into account its value.                           |
 |___________________________________________________________________________|
*/
#ifndef EXCLUDE_BASOPS_NOT_USED_EVS
/* Exclude L_add_c() from build as it reads/sets global Overflow/Carry flags, L_add_c() isn't called anywhere in 3GPP EVS floating-point code - CJ March 2017 */
Word32 L_add_c (Word32 L_var1, Word32 L_var2)
{
    Word32 L_var_out;
    Word32 L_test;
    Flag carry_int = 0;

    #ifdef USE_BASOPS_CARRY_GLOBAL_VAR
    L_var_out = L_var1 + L_var2 + Carry;
    #else
    L_var_out = L_var1 + L_var2;
    #endif

    L_test = L_var1 + L_var2;

    if ((L_var1 > 0) && (L_var2 > 0) && (L_test < 0))
    {
        #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
        Overflow = 1;
        #endif
        carry_int = 0;
    }
    else
    {
        if ((L_var1 < 0) && (L_var2 < 0))
        {
            if (L_test >= 0)
            {
                #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
                Overflow = 1;
                #endif
                carry_int = 1;
            }
            else
            {
                #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
                Overflow = 0;
                #endif
                carry_int = 1;
            }
        }
        else
        {
            if (((L_var1 ^ L_var2) < 0) && (L_test >= 0))
            {
                #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
                Overflow = 0;
                #endif
                carry_int = 1;
            }
            else
            {
                #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
                Overflow = 0;
                #endif
                carry_int = 0;
            }
        }
    }

    #ifdef USE_BASOPS_CARRY_GLOBAL_VAR
    if (Carry)
    {
        if (L_test == MAX_32)
        {
            #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
            Overflow = 1;
            #endif
            Carry = carry_int;
        }
        else
        {
            if (L_test == (Word32) 0xFFFFFFFFL)
            {
                Carry = 1;
            }
            else
            {
                Carry = carry_int;
            }
        }
    }
    else
    {
        Carry = carry_int;
    }
    #endif

#if (WMOPS)
    multiCounter[currCounter].L_add_c++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_sub_c                                                 |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Performs 32 bits subtraction of the two 32 bits variables with carry    |
 |   (borrow) : L_var1-L_var2-C. No saturation. Generate carry and Overflow  |
 |   values. The carry and overflow values are binary variables which can    |
 |   be tested and assigned values.                                          |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    L_var2   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |                                                                           |
 |   Caution :                                                               |
 |                                                                           |
 |    In some cases the Carry flag has to be cleared or set before using     |
 |    operators which take into account its value.                           |
 |___________________________________________________________________________|
*/
#ifndef EXCLUDE_BASOPS_NOT_USED_EVS
/* Exclude L_sub_c() from build as it reads/sets global Overflow/Carry flags, L_sub_c() isn't called anywhere in 3GPP EVS floating-point coded - CJ March 2017 */
Word32 L_sub_c (Word32 L_var1, Word32 L_var2)
{
    Word32 L_var_out;
    Word32 L_test;
    Flag carry_int = 0;

    #ifdef USE_BASOPS_CARRY_GLOBAL_VAR
    if (Carry)
    {
        Carry = 0;
        if (L_var2 != MIN_32)
        {
            L_var_out = L_add_c (L_var1, -L_var2);
#if (WMOPS)
            multiCounter[currCounter].L_add_c--;
#endif
        }
        else
        {
            L_var_out = L_var1 - L_var2;
            if (L_var1 > 0L)
            {
                #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
                Overflow = 1;
                #endif
                Carry = 0;
            }
        }
    }
    else
    #endif
    {
        L_var_out = L_var1 - L_var2 - (Word32) 0X00000001L;
        L_test = L_var1 - L_var2;

        if ((L_test < 0) && (L_var1 > 0) && (L_var2 < 0))
        {
            #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
            Overflow = 1;
            #endif
            carry_int = 0;
        }
        else if ((L_test > 0) && (L_var1 < 0) && (L_var2 > 0))
        {
            #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
            Overflow = 1;
            #endif
            carry_int = 1;
        }
        else if ((L_test > 0) && ((L_var1 ^ L_var2) > 0))
        {
            #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
            Overflow = 0;
            #endif
            carry_int = 1;
        }

        #ifdef USE_BASOPS_CARRY_GLOBAL_VAR
        if (L_test == MIN_32)
        {
            #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
            Overflow = 1;
            #endif
            Carry = carry_int;
        }
        else
        {
            Carry = carry_int;
        }
        #endif
    }

#if (WMOPS)
    multiCounter[currCounter].L_sub_c++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_negate                                                |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Negate the 32 bit variable L_var1 with saturation; saturate in the case |
 |   where input is -2147483648 (0x8000 0000).                               |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
Word32 L_negate (Word32 L_var1)
{
    Word32 L_var_out;

    L_var_out = (L_var1 == MIN_32) ? MAX_32 : -L_var1;

#if (WMOPS)
    multiCounter[currCounter].L_negate++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : mult_r                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Same as mult with rounding, i.e.:                                       |
 |     mult_r(var1,var2) = extract_l(L_shr(((var1 * var2) + 16384),15)) and  |
 |     mult_r(-32768,-32768) = 32767.                                        |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0x8000 <= var_out <= 0x7fff.                          |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE
Word16 mult_r (Word16 var1, Word16 var2)
{
    Word16 var_out;
    Word32 L_product_arr;

    L_product_arr = (Word32) var1 *(Word32) var2;       /* product */
    L_product_arr += (Word32) 0x00004000L;      /* round */
    L_product_arr &= (Word32) 0xffff8000L;
    L_product_arr >>= 15;       /* shift */

    if (L_product_arr & (Word32) 0x00010000L)   /* sign extend when necessary */
    {
        L_product_arr |= (Word32) 0xffff0000L;
    }
    var_out = saturate (L_product_arr);

#if (WMOPS)
    multiCounter[currCounter].mult_r++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_shl                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 32 bit input L_var1 left var2 positions. Zero  |
 |   fill the var2 LSB of the result. If var2 is negative, arithmetically    |
 |   shift L_var1 right by -var2 with sign extension. Saturate the result in |
 |   case of underflows or overflows.                                        |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
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
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
Word32 L_shl (Word32 L_var1, Word16 var2)
{

    Word32 L_var_out = 0L;

    if (var2 <= 0)
    {
        if (var2 < -32)
            var2 = -32;
        var2 = -var2;
        L_var_out = L_shr (L_var1, var2);
#if (WMOPS)
        multiCounter[currCounter].L_shr--;
#endif
    }
    else
    {
        for (; var2 > 0; var2--)
        {
            if (L_var1 > (Word32) 0X3fffffffL)
            {
                #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
                Overflow = 1;
                #endif
                L_var_out = MAX_32;
                break;
            }
            else
            {
                if (L_var1 < (Word32) 0xc0000000L)
                {
                    #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
                    Overflow = 1;
                    #endif
                    L_var_out = MIN_32;
                    break;
                }
            }
            L_var1 *= 2;
            L_var_out = L_var1;
        }
    }
    #if (WMOPS)
    multiCounter[currCounter].L_shl++;
	  #endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}

#ifdef _EVS_
/* Identical to L_shl() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */
Word32 L_shl_ovf (Word32 L_var1, Word16 var2, Flag *Overflow)
{

    Word32 L_var_out = 0L;

    if (var2 <= 0)
    {
        if (var2 < -32)
            var2 = -32;
        var2 = -var2;
        L_var_out = L_shr (L_var1, var2);
    }
    else
    {
        for (; var2 > 0; var2--)
        {
            if (L_var1 > (Word32) 0X3fffffffL)
            {
                *Overflow = 1;
                L_var_out = MAX_32;
                break;
            }
            else
            {
                if (L_var1 < (Word32) 0xc0000000L)
                {
                    *Overflow = 1;
                    L_var_out = MIN_32;
                    break;
                }
            }
            L_var1 *= 2;
            L_var_out = L_var1;
        }
    }

    BASOP_CHECK();

    return (L_var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_shr                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 32 bit input L_var1 right var2 positions with  |
 |   sign extension. If var2 is negative, arithmetically shift L_var1 left   |
 |   by -var2 and zero fill the -var2 LSB of the result. Saturate the result |
 |   in case of underflows or overflows.                                     |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
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
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/
Word32 L_shr (Word32 L_var1, Word16 var2)
{
    Word32 L_var_out;

    if (var2 < 0)
    {
        if (var2 < -32)
            var2 = -32;
        var2 = -var2;
        L_var_out = L_shl (L_var1, var2);
#if (WMOPS)
        multiCounter[currCounter].L_shl--;
#endif
    }
    else
    {
        if (var2 >= 31)
        {
            L_var_out = (L_var1 < 0L) ? -1 : 0;
        }
        else
        {
            if (L_var1 < 0)
            {
                L_var_out = ~((~L_var1) >> var2);
            }
            else
            {
                L_var_out = L_var1 >> var2;
            }
        }
    }
    #if (WMOPS)
    multiCounter[currCounter].L_shr++;
	  #endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : shr_r                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Same as shr(var1,var2) but with rounding. Saturate the result in case of|
 |   underflows or overflows :                                               |
 |    - If var2 is greater than zero :                                       |
 |          if (sub(shl(shr(var1,var2),1),shr(var1,sub(var2,1))))            |
 |          is equal to zero                                                 |
 |                     then                                                  |
 |                     shr_r(var1,var2) = shr(var1,var2)                     |
 |                     else                                                  |
 |                     shr_r(var1,var2) = add(shr(var1,var2),1)              |
 |    - If var2 is less than or equal to zero :                              |
 |                     shr_r(var1,var2) = shr(var1,var2).                    |
 |                                                                           |
 |   Complexity weight : 3                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
Word16 shr_r (Word16 var1, Word16 var2)
{
    Word16 var_out;

    if (var2 > 15)
    {
        var_out = 0;
    }
    else
    {
        var_out = shr (var1, var2);

#if (WMOPS)
        multiCounter[currCounter].shr--;
#endif

        if (var2 > 0)
        {
            if ((var1 & ((Word16) 1 << (var2 - 1))) != 0)
            {
                var_out++;
            }
        }
    }

#if (WMOPS)
    multiCounter[currCounter].shr_r++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : mac_r                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Add the 32 bit    |
 |   result to L_var3 with saturation. Round the LS 16 bits of the result    |
 |   into the MS 16 bits with saturation and shift the result right by 16.   |
 |   Return a 16 bit result.                                                 |
 |            mac_r(L_var3,var1,var2) = round_fx(L_mac(L_var3,var1,var2))    |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0x0000 8000 <= L_var_out <= 0x0000 7fff.              |
 |___________________________________________________________________________|
*/
Word16 mac_r (Word32 L_var3, Word16 var1, Word16 var2)
{
    Word16 var_out;

    L_var3 = L_mac (L_var3, var1, var2);
    L_var3 = L_add (L_var3, (Word32) 0x00008000L);
    var_out = extract_h (L_var3);

#if (WMOPS)
    multiCounter[currCounter].L_mac--;
    multiCounter[currCounter].L_add--;
    multiCounter[currCounter].extract_h--;
    multiCounter[currCounter].mac_r++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : msu_r                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Subtract the 32   |
 |   bit result from L_var3 with saturation. Round the LS 16 bits of the res-|
 |   ult into the MS 16 bits with saturation and shift the result right by   |
 |   16. Return a 16 bit result.                                             |
 |            msu_r(L_var3,var1,var2) = round_fx(L_msu(L_var3,var1,var2))    |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
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
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0x0000 8000 <= L_var_out <= 0x0000 7fff.              |
 |___________________________________________________________________________|
*/
Word16 msu_r (Word32 L_var3, Word16 var1, Word16 var2)
{
    Word16 var_out;

    L_var3 = L_msu (L_var3, var1, var2);
    L_var3 = L_add (L_var3, (Word32) 0x00008000L);
    var_out = extract_h (L_var3);

#if (WMOPS)
    multiCounter[currCounter].L_msu--;
    multiCounter[currCounter].L_add--;
    multiCounter[currCounter].extract_h--;
    multiCounter[currCounter].msu_r++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_deposit_h                                             |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Deposit the 16 bit var1 into the 16 MS bits of the 32 bit output. The   |
 |   16 LS bits of the output are zeroed.                                    |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= var_out <= 0x7fff 0000.                |
 |___________________________________________________________________________|
*/
#ifndef _EVS_  /* declared as static inline in basop32.h, JHB Mar 2023 */
Word32 L_deposit_h (Word16 var1)
{
    Word32 L_var_out;

    L_var_out = (Word32) var1 << 16;

#if (WMOPS)
    multiCounter[currCounter].L_deposit_h++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_deposit_l                                             |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Deposit the 16 bit var1 into the 16 LS bits of the 32 bit output. The   |
 |   16 MS bits of the output are sign extended.                             |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0xFFFF 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
#ifndef _EVS_  /* declared as static inline in basop32.h, JHB Mar 2023 */
Word32 L_deposit_l (Word16 var1)
{
    Word32 L_var_out;

    L_var_out = (Word32) var1;

#if (WMOPS)
    multiCounter[currCounter].L_deposit_l++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_shr_r                                                 |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Same as L_shr(L_var1,var2) but with rounding. Saturate the result in    |
 |   case of underflows or overflows :                                       |
 |    - If var2 is greater than zero :                                       |
 |          if (L_sub(L_shl(L_shr(L_var1,var2),1),L_shr(L_var1,sub(var2,1))))|
 |          is equal to zero                                                 |
 |                     then                                                  |
 |                     L_shr_r(L_var1,var2) = L_shr(L_var1,var2)             |
 |                     else                                                  |
 |                     L_shr_r(L_var1,var2) = L_add(L_shr(L_var1,var2),1)    |
 |    - If var2 is less than or equal to zero :                              |
 |                     L_shr_r(L_var1,var2) = L_shr(L_var1,var2).            |
 |                                                                           |
 |   Complexity weight : 3                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
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
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= var_out <= 0x7fff ffff.                |
 |___________________________________________________________________________|
*/
Word32 L_shr_r (Word32 L_var1, Word16 var2)
{
    Word32 L_var_out;

    if (var2 > 31)
    {
        L_var_out = 0;
    }
    else
    {
        L_var_out = L_shr (L_var1, var2);

#if (WMOPS)
        multiCounter[currCounter].L_shr--;
#endif
        if (var2 > 0)
        {
            if ((L_var1 & ((Word32) 1 << (var2 - 1))) != 0)
            {
                L_var_out++;
            }
        }
    }

#if (WMOPS)
    multiCounter[currCounter].L_shr_r++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_abs                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Absolute value of L_var1; Saturate in case where the input is          |
 |                                                               -214783648  |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x0000 0000 <= var_out <= 0x7fff ffff.                |
 |___________________________________________________________________________|
*/
Word32 L_abs (Word32 L_var1)
{
    Word32 L_var_out;

    if (L_var1 == MIN_32)
    {
        L_var_out = MAX_32;
    }
    else
    {
        if (L_var1 < 0)
        {
            L_var_out = -L_var1;
        }
        else
        {
            L_var_out = L_var1;
        }
    }

#if (WMOPS)
    multiCounter[currCounter].L_abs++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_sat                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    32 bit L_var1 is set to 2147483647 if an overflow occured or to        |
 |    -2147483648 if an underflow occured on the most recent L_add_c,        |
 |    L_sub_c, L_macNs or L_msuNs operations. The carry and overflow values  |
 |    are binary values which can be tested and assigned values.             |
 |                                                                           |
 |   Complexity weight : 4                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= var_out <= 0x7fff ffff.                |
 |___________________________________________________________________________|
*/
#ifndef EXCLUDE_BASOPS_NOT_USED_EVS
/* Exclude L_sat() from build as it reads/sets global Overflow/Carry flags, L_sat() isn't called anywhere in 3GPP EVS floating-point code - CJ March 2017 */
Word32 L_sat (Word32 L_var1)
{
    Word32 L_var_out;

    L_var_out = L_var1;

    #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
    if (Overflow)
    {

        #ifdef USE_BASOPS_CARRY_GLOBAL_VAR
        if (Carry)
        {
            L_var_out = MIN_32;
        }
        else
        {
            L_var_out = MAX_32;
        }
        #endif

        Carry = 0;
        Overflow = 0;
    }
    #endif

#if (WMOPS)
    multiCounter[currCounter].L_sat++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (L_var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : norm_s                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Produces the number of left shift needed to normalize the 16 bit varia- |
 |   ble var1 for positive values on the interval with minimum of 16384 and  |
 |   maximum of 32767, and for negative values on the interval with minimum  |
 |   of -32768 and maximum of -16384; in order to normalize the result, the  |
 |   following operation must be done :                                      |
 |                    norm_var1 = shl(var1,norm_s(var1)).                    |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 000f.                |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE
Word16 norm_s (Word16 var1)
{
    Word16 var_out;

    if (var1 == 0)
    {
        var_out = 0;
    }
    else
    {
        if (var1 == (Word16) 0xffff)
        {
            var_out = 15;
        }
        else
        {
            if (var1 < 0)
            {
                var1 = ~var1;
            }
            for (var_out = 0; var1 < 0x4000; var_out++)
            {
                var1 <<= 1;
            }
        }
    }

#if (WMOPS)
    multiCounter[currCounter].norm_s++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : div_s                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Produces a result which is the fractional integer division of var1  by  |
 |   var2; var1 and var2 must be positive and var2 must be greater or equal  |
 |   to var1; the result is positive (leading bit equal to 0) and truncated  |
 |   to 16 bits.                                                             |
 |   If var1 = var2 then div(var1,var2) = 32767.                             |
 |                                                                           |
 |   Complexity weight : 18                                                  |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0x0000 0000 <= var1 <= var2 and var2 != 0.            |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : var1 <= var2 <= 0x0000 7fff and var2 != 0.            |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 7fff.                |
 |             It's a Q15 value (point between b15 and b14).                 |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE
Word16 div_s (Word16 var1, Word16 var2) 
{
    Word16 var_out = 0;
    Word16 iteration;
    Word32 L_num;
    Word32 L_denom;

    if (var2 == 0)
    {
    /* printf ("Division by 0, Fatal error \n"); */
       fprintf(stderr, "Division by 0 in div_s in basop32");
       #ifdef USE_BASOPS_EXIT  /* disable exit() unless defined in options.h or otherwise, JHB Mar 2023 */
       abort(); /* exit (0); */
       #else
       #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
       BASOP_CHECK();
       #endif
       return MAX_16;  /* return max possible value, JHB Mar 2023 */
       #endif
    }

    if (var1 > var2 || var1 < 0 || var2 < 0)
    {
    /* printf ("Division Error var1=%d  var2=%d\n", var1, var2); */
       fprintf(stderr, "Division error in div_s in basop32, var1 = %d var2 = %d", var1, var2);
       #ifdef USE_BASOPS_EXIT  /* disable exit() unless defined in options.h or otherwise, JHB Mar 2023 */
       abort(); /* exit (0); */
       #else
       if (var1 < 0) var1 = -var1;  /* make positive values and proceed, JHB Mar 2023 */
       if (var2 < 0) var2 = -var2;
       #endif
    }

    if (var1)
    {
        if (var1 >= var2)  /* use >= check here, in case "make positive" above happened, JHB Mar 2023 */
        {
            var_out = MAX_16;
        }
        else
        {
            L_num = L_deposit_l (var1);
            L_denom = L_deposit_l (var2);

#if (WMOPS)
            multiCounter[currCounter].L_deposit_l--;
            multiCounter[currCounter].L_deposit_l--;
#endif

            for (iteration = 0; iteration < 15; iteration++)
            {
                var_out <<= 1;
                L_num <<= 1;

                if (L_num >= L_denom)
                {
                    L_num = L_sub (L_num, L_denom);
                    var_out = add (var_out, 1);
#if (WMOPS)
                    multiCounter[currCounter].L_sub--;
                    multiCounter[currCounter].add--;
#endif
                }
            }
        }
    }

#if (WMOPS)
    multiCounter[currCounter].div_s++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : norm_l                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Produces the number of left shifts needed to normalize the 32 bit varia-|
 |   ble L_var1 for positive values on the interval with minimum of          |
 |   1073741824 and maximum of 2147483647, and for negative values on the in-|
 |   terval with minimum of -2147483648 and maximum of -1073741824; in order |
 |   to normalize the result, the following operation must be done :         |
 |                   norm_L_var1 = L_shl(L_var1,norm_l(L_var1)).             |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 001f.                |
 |___________________________________________________________________________|
*/
#ifndef USE_BASOPS_INLINE
Word16 norm_l (Word32 L_var1)
{
    Word16 var_out;

    if (L_var1 == 0)
    {
        var_out = 0;
    }
    else
    {
        if (L_var1 == (Word32) 0xffffffffL)
        {
            var_out = 31;
        }
        else
        {
            if (L_var1 < 0)
            {
                L_var1 = ~L_var1;
            }
            for (var_out = 0; L_var1 < (Word32) 0x40000000L; var_out++)
            {
                L_var1 <<= 1;
            }
        }
    }

#if (WMOPS)
    multiCounter[currCounter].norm_l++;
#endif

    #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
    BASOP_CHECK();
    #endif
    return (var_out);
}
#endif
/*
 ******************************************************************************
 * Additional operators extracted from the G.723.1 Library
 * Adapted for WMOPS calculations
 ******************************************************************************
*/

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_mls                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiplies a 16 bit word v by a 32 bit word Lv and returns a 32 bit     |
 |   word (multiplying 16 by 32 bit words gives 48 bit word; the function    |
 |   extracts the 32 MSB and shift the result to the left by 1).             |
 |                                                                           |
 |   A 32 bit word can be written as                                         |
 |    Lv = a  +  b * 2^16                                                    |
 |   where a= unsigned 16 LSBs and b= signed 16 MSBs.                        |
 |   The function returns v * Lv  /  2^15  which is equivalent to            |
 |        a*v / 2^15 + b*v*2                                                 |
 |                                                                           |
 |   Complexity weight : 5													 |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |   Lv                                                                      |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
 |   v                                                                       |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0x8000 <= var1 <= 0x7fff.                             |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= var_out <= 0x7fff ffff.                |
 |                                                                           |
 |___________________________________________________________________________|
*/
Word32 L_mls (Word32 Lv, Word16 v)
{
   Word32   Temp  ;

   Temp = Lv & (Word32) 0x0000ffff ;
   Temp = Temp * (Word32) v ;
   Temp = L_shr( Temp, (Word16) 15 ) ;
   Temp = L_mac( Temp, v, extract_h(Lv) ) ;

#if (WMOPS)
   multiCounter[currCounter].L_shr--;
   multiCounter[currCounter].L_mac--;
   multiCounter[currCounter].extract_h--;
   multiCounter[currCounter].L_mls++;
#endif

   #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
   BASOP_CHECK();
   #endif
   return Temp ;
}


/*__________________________________________________________________________
|                                                                           |
|   Function Name : div_l                                                   |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
|   Produces a result which is the fractional integer division of L_var1 by |
|   var2; L_var1 and var2 must be positive and var2 << 16 must be greater or|
|   equal to L_var1; the result is positive (leading bit equal to 0) and    |
|   truncated to 16 bits.                                                   |
|   If L_var1 == var2 << 16 then div_l(L_var1,var2) = 32767.                |
|                                                                           |
|   Complexity weight : 32                                                  |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|    L_var1                                                                 |
|             32 bit long signed integer (Word32) whose value falls in the  |
|             range : 0x0000 0000 <= var1 <= (var2 << 16)  and var2 != 0.   |
|             L_var1 must be considered as a Q.31 value                     |
|                                                                           |
|    var2                                                                   |
|             16 bit short signed integer (Word16) whose value falls in the |
|             range : var1 <= (var2<< 16) <= 0x7fff0000 and var2 != 0.      |
|             var2 must be considered as a Q.15 value                       |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|    var_out                                                                |
|             16 bit short signed integer (Word16) whose value falls in the |
|             range : 0x0000 0000 <= var_out <= 0x0000 7fff.                |
|             It's a Q15 value (point between b15 and b14).                 |
|___________________________________________________________________________|
*/
Word16 div_l (Word32  L_num, Word16 den)
{
    Word16   var_out = (Word16)0;
    Word32   L_den;
    Word16   iteration;

#if (WMOPS)
    multiCounter[currCounter].div_l++;
#endif

    if ( den == (Word16) 0 ) {
     /* printf("Division by 0 in div_l, Fatal error in "); printStack(); */
        fprintf(stderr, "Division by 0 in div_l in basop32");
        #ifdef USE_BASOPS_EXIT  /* disable exit() unless defined in options.h or otherwise, JHB Mar 2023 */
        exit(0);
        #else
        #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
        BASOP_CHECK();
        #endif
        #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
        BASOP_CHECK();
        #endif
        return MAX_16;  /* return max possible value, JHB Mar 2023 */
        #endif
    }

    if ( (L_num < (Word32) 0) || (den < (Word16) 0) ) {
     /* printf("Division Error in div_l, Fatal error in "); printStack(); */
        fprintf(stderr, "Division error in div_l in basop32, L_num = %d den = %d", L_num, den);
        #ifdef USE_BASOPS_EXIT  /* disable exit() unless defined in options.h or otherwise, JHB Mar 2023 */
        exit(0);
        #else
        if (L_num < 0) L_num = -L_num;  /* make positive values and proceed, JHB Mar 2023 */
        if (den < 0) den = -den;
        #endif
    }

    L_den = L_deposit_h( den ) ;
#if (WMOPS)
    multiCounter[currCounter].L_deposit_h--;
#endif

    if ( L_num >= L_den ){
        #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
        BASOP_CHECK();
        #endif
        return MAX_16 ;
    }
    else {
        L_num = L_shr(L_num, (Word16)1) ;
        L_den = L_shr(L_den, (Word16)1);
#if (WMOPS)
        multiCounter[currCounter].L_shr-=2;
#endif
        for(iteration=(Word16)0; iteration< (Word16)15;iteration++) {
            var_out = shl( var_out, (Word16)1);
            L_num   = L_shl( L_num, (Word16)1);
#if (WMOPS)
            multiCounter[currCounter].shl--;
            multiCounter[currCounter].L_shl--;
#endif
            if (L_num >= L_den) {
                L_num = L_sub(L_num,L_den);
                var_out = add(var_out, (Word16)1);
#if (WMOPS)
            multiCounter[currCounter].L_sub--;
            multiCounter[currCounter].add--;
#endif
            }
        }

        #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
        BASOP_CHECK();
        #endif
        return var_out;
    }
}


/*__________________________________________________________________________
|                                                                           |
|   Function Name : i_mult                                                  |
|                                                                           |
|   Purpose :                                                               |
|                                                                           |
|   Integer 16-bit multiplication with overflow control.                    |
|   No overflow protection is performed if ORIGINAL_G7231 is defined.       |
|                                                                           |
|   Complexity weight : 3 (it is performing something equivalent to         |
|                          extract_h( L_shl( L_mult0( v1, v2), 16))         |
|                                                                           |
|   Inputs :                                                                |
|                                                                           |
|    a                                                                      |
|             16 bit short signed integer (Word16).                         |
|                                                                           |
|    b                                                                      |
|             16 bit short signed integer (Word16).                         |
|                                                                           |
|   Outputs :                                                               |
|                                                                           |
|    none                                                                   |
|                                                                           |
|   Return Value :                                                          |
|                                                                           |
|             16 bit short signed integer (Word16). No overflow checks      |
|             are performed if ORIGINAL_G7231 is defined.                   |
|___________________________________________________________________________|
*/
Word16 i_mult (Word16 a, Word16 b)
{
#ifdef ORIGINAL_G7231
   return a*b ;
#else
   #ifndef _EVS_
   Word32 register c=a*b;
   #else
   register Word32 c=a*b;  /* fix warning for 3GPP EVS build, CKJ Mar 2017 */
   #endif
#if (WMOPS)
    multiCounter[currCounter].i_mult++;
#endif
   return saturate(c) ;
#endif
}


/*
 ******************************************************************************
 * The following three operators are not part of the original 
 * G.729/G.723.1 set of basic operators and implement shiftless
 * accumulation operation.
 ******************************************************************************
*/

/*___________________________________________________________________________
 |
 |   Function Name : L_mult0
 |
 |   Purpose :
 |
 |   L_mult0 is the 32 bit result of the multiplication of var1 times var2
 |   without one left shift.
 |
 |   Complexity weight : 1
 |
 |   Inputs :
 |
 |    var1     16 bit short signed integer (Word16) whose value falls in the
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 |
 |    var2     16 bit short signed integer (Word16) whose value falls in the
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 |
 |   Return Value :
 |
 |    L_var_out
 |             32 bit long signed integer (Word32) whose value falls in the
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 |___________________________________________________________________________
*/
Word32 L_mult0 (Word16 var1,Word16 var2)
{
  Word32 L_var_out;

  L_var_out = (Word32)var1 * (Word32)var2;

#if (WMOPS)
    multiCounter[currCounter].L_mult0++;
#endif

  #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
  BASOP_CHECK();
  #endif
  return(L_var_out);
}


/*___________________________________________________________________________
 |
 |   Function Name : L_mac0
 |
 |   Purpose :
 |
 |   Multiply var1 by var2 (without left shift) and add the 32 bit result to
 |   L_var3 with saturation, return a 32 bit result:
 |        L_mac0(L_var3,var1,var2) = L_add(L_var3,(L_mult0(var1,var2)).
 |
 |   Complexity weight : 1
 |
 |   Inputs :
 |
 |    L_var3   32 bit long signed integer (Word32) whose value falls in the
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.
 |
 |    var1     16 bit short signed integer (Word16) whose value falls in the
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 |
 |    var2     16 bit short signed integer (Word16) whose value falls in the
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 |
 |   Return Value :
 |
 |    L_var_out
 |             32 bit long signed integer (Word32) whose value falls in the
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 |___________________________________________________________________________
*/
Word32 L_mac0 (Word32 L_var3, Word16 var1, Word16 var2)
{
  Word32 L_var_out;
  Word32 L_product;

  L_product = L_mult0(var1,var2);
  L_var_out = L_add(L_var3,L_product);

#if (WMOPS)
    multiCounter[currCounter].L_mac0++;
    multiCounter[currCounter].L_mult0--;
    multiCounter[currCounter].L_add--;
#endif

  #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
  BASOP_CHECK();
  #endif
  return(L_var_out);
}


/*___________________________________________________________________________
 |
 |   Function Name : L_msu0
 |
 |   Purpose :
 |
 |   Multiply var1 by var2 (without left shift) and subtract the 32 bit
 |   result to L_var3 with saturation, return a 32 bit result:
 |        L_msu0(L_var3,var1,var2) = L_sub(L_var3,(L_mult0(var1,var2)).
 |
 |   Complexity weight : 1
 |
 |   Inputs :
 |
 |    L_var3   32 bit long signed integer (Word32) whose value falls in the
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.
 |
 |    var1     16 bit short signed integer (Word16) whose value falls in the
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 |
 |    var2     16 bit short signed integer (Word16) whose value falls in the
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 |
 |   Return Value :
 |
 |    L_var_out
 |             32 bit long signed integer (Word32) whose value falls in the
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 |___________________________________________________________________________
*/
Word32 L_msu0 (Word32 L_var3, Word16 var1, Word16 var2)
{
  Word32 L_var_out;
  Word32 L_product;

  L_product = L_mult0(var1,var2);
  L_var_out = L_sub(L_var3,L_product);

#if (WMOPS)
    multiCounter[currCounter].L_msu0++;
    multiCounter[currCounter].L_mult0--;
    multiCounter[currCounter].L_sub--;
#endif

  #ifdef _EVS_  /* EVS authors added BASOP_CHECK() */
  BASOP_CHECK();
  #endif
  return(L_var_out);
}


/* end of file */

