/*
  ===========================================================================
   File: BASOP32.H                                       v.2.3 - 30.Nov.2009
  ===========================================================================

            ITU-T STL  BASIC OPERATORS

            GLOBAL FUNCTION PROTOTYPES

   History:
   26.Jan.00   v1.0     Incorporated to the STL from updated G.723.1/G.729
                        basic operator library (based on basic_op.h) and
                        G.723.1's basop.h.
   05.Jul.00   v1.1     Added 32-bit shiftless mult/mac/msub operators

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

   13 Mar 12            Add Overflow2 flag for additional overflow checking.
  ============================================================================
*/

/* 
Made thread-safe and optimized for EVS codec floating-point use. Modifications can be expanded or edited for other ITU and 3GPP ETSI codecs, as needed

Copyright (C) Signalogic, 2017-2023

Revision History

  Mar 2017 CKJ, modified to remove usage of global flags so that the lib can be thread-safe
  Sep 2022 - Mar 2023 JHB, implement and enable static inline basops in basop32.h if USE_BASOPS_INLINE is defined. Enable/disable Overflow and Carry global variables with USE_BASOPS_OVERFLOW_GLOBAL_VAR and USE_BASOPS_CARRY_GLOBAL_VAR. These can be defined in options.h or as needed
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

#ifndef _BASIC_OP_H
#define _BASIC_OP_H

#define BASOP_OVERFLOW2

#define BASOP_SATURATE_WARNING_ON
#define BASOP_SATURATE_WARNING_OFF
#define BASOP_SATURATE_ERROR_ON
#define BASOP_SATURATE_ERROR_OFF
#define BASOP_CHECK()

/*___________________________________________________________________________
 |                                                                           |
 |   Constants and Globals                                                   |
 |___________________________________________________________________________|
*/

/* Remove global flags, replaced with stack vars as needed - CKJ Mar 2017 */
/* Note - Chris demonstrates here that no EVS floating-point sources read Overflow, Overflow2, or Carry; i.e. these vars are "write-only" global flags written only by basop32.c, JHB Mar 2023 */

#ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR
extern Flag Overflow, Overflow2;  /* Overflow2 seems to be referenced only in EVS fixed-point source, JHB Mar 2023 */
#endif

#ifdef USE_BASOPS_CARRY_GLOBAL_VAR
extern Flag Carry;
#endif

#define MAX_32 (Word32)0x7fffffffL
#define MIN_32 (Word32)0x80000000L

#define MAX_16 (Word16)0x7fff
#define MIN_16 (Word16)0x8000

#include <stdlib.h>

#include "options.h"
#include "typedef.h"

/* functions inlined and no longer declared in basop32.c */

static inline Word32 L_deposit_l(Word16 var1) { return (Word32)var1; }
static inline Word32 L_deposit_h(Word16 var1) { return (Word32)var1 << 16; }

static inline Word16 extract_l(Word32 L_var1) { return (Word16)L_var1; }
static inline Word16 extract_h(Word32 L_var1) { return (Word16)(L_var1 >> 16); }

#ifdef USE_BASOPS_INLINE  /* defined in options.h, JHB Mar 2023 */

#include <stdio.h>  /* needed for a few fprintf(stderr ...) calls. In extensive testing over 6 year period these have never fired, but we keep them in case some day it should happen and we need forensics, JHB Mar 2023 */

static inline Word16 saturate(Word32 L_var1) {

Word16 var_out;

   if (L_var1 > 0x00007fffL) {
      #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR  /* not needed; see above comments, JHB Mar 2023 */
      Overflow = 1;
      #endif
      var_out = MAX_16;
   }
   else if (L_var1 < (Word32) 0xffff8000L) {
      #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR  /* not needed, see above comments, JHB Mar 2023 */
      Overflow = 1;
      #endif
      var_out = MIN_16;
   }
   else var_out = extract_l(L_var1);

   return var_out;
}

static inline Word16 negate(Word16 var1) { return (var1 == MIN_16) ? MAX_16 : -var1; }

static inline Word16 sub(Word16 var1, Word16 var2) {

Word16 var_out;
Word32 L_diff;

   L_diff = (Word32)var1 - var2;
   var_out = saturate(L_diff);

   return (var_out);
}

static inline Word16 add(Word16 var1, Word16 var2) {

Word16 var_out;
Word32 L_sum;

   L_sum = (Word32)var1 + var2;
   var_out = saturate(L_sum);

   return var_out;
}

static inline Word32 L_sub(Word32 L_var1, Word32 L_var2) {

Word32 L_var_out;

   L_var_out = L_var1 - L_var2;

   if (((L_var1 ^ L_var2) & MIN_32) != 0) {

      if ((L_var_out ^ L_var1) & MIN_32) {

         L_var_out = (L_var1 < 0L) ? MIN_32 : MAX_32;
         #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR  /* not needed; see above comments, JHB Mar 2023 */
         Overflow = 1;
         #endif
      }
  }

  return L_var_out;
}

static inline Word32 L_add(Word32 L_var1, Word32 L_var2) {

Word32 L_var_out;

   L_var_out = L_var1 + L_var2;

   if (((L_var1 ^ L_var2) & MIN_32) == 0) {

      if ((L_var_out ^ L_var1) & MIN_32) {

         L_var_out = (L_var1 < 0) ? MIN_32 : MAX_32;
         #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR  /* not needed; see above comments, JHB Mar 2023 */
         Overflow = 1;
         #endif
      }
  }

   return L_var_out;
}

static inline Word16 div_s(Word16 var1, Word16 var2) {

Word16 var_out = 0, iteration;
Word32 L_num, L_denom;

   if (var2 == 0) {

   /* printf ("Division by 0, Fatal error in "); printStack(); */
      fprintf(stderr, "Division by 0 in divs_s in basop32");
      #ifdef USE_BASOPS_EXIT  /* disable exit() unless defined in options.h or otherwise, JHB Mar 2023 */
      exit(0);
      #else
      return MAX_16;  /* return max possible value, JHB Mar 2023 */
      #endif
   }

   if (var1 > var2 || var1 < 0 || var2 < 0) {

   /* printf ("Division Error var1=%d  var2=%d in ", var1, var2); printStack(); */
      fprintf(stderr, "Division error var1 = %d  var2 = %d in div_s in basop32", var1, var2);
      #ifdef USE_BASOPS_EXIT  /* disable exit() unless defined in options.h or otherwise, JHB Mar 2023 */
      exit(0);
      #else
      if (var1 < 0) var1 = -var1;  /* make positive values and proceed, JHB Mar 2023 */
      if (var2 < 0) var2 = -var2;
      #endif
   }

   if (var1) {

      if (var1 >= var2) var_out = MAX_16;  /* use >= check here, in case "make positive" above happened, JHB Mar 2023 */
      else {

         L_num = L_deposit_l(var1);
         L_denom = L_deposit_l(var2);

         for (iteration = 0; iteration < 15; iteration++) {

            var_out <<= 1;
            L_num <<= 1;

            if (L_num >= L_denom) {
               L_num = L_sub(L_num, L_denom);
               var_out = add(var_out, 1);
            }
         }
      }
   }

   return var_out;
}

static inline Word16 mult_r(Word16 var1, Word16 var2) {

Word16 var_out;
Word32 L_product_arr;

   L_product_arr = (Word32) var1 *(Word32) var2;  /* product */
   L_product_arr += (Word32) 0x00004000L;         /* round */
   L_product_arr &= (Word32) 0xffff8000L;
   L_product_arr >>= 15;                          /* shift */

   if (L_product_arr & (Word32) 0x00010000L) L_product_arr |= (Word32) 0xffff0000L;  /* sign extend when necessary */

   var_out = saturate(L_product_arr);

   return var_out;
}

static inline Word16 shl_pos(Word16 var1, Word16 var2) {

Word16 var_out;
Word32 result;

   result = (Word32) var1 *((Word32) 1 << var2);

   if ((var2 > 15 && var1 != 0) || (result != (Word32)((Word16)result))) {

      #ifdef USE_BASOPS_OVERFLOW_GLOBAL_VAR  /* not needed; see above comments, JHB Mar 2023 */
      Overflow = 1;
      #endif
      var_out = (var1 > 0) ? MAX_16 : MIN_16;
   }
   else var_out = extract_l(result);

   return var_out;
}

static inline Word16 shr(Word16 var1, Word16 var2) {

Word16 var_out;

   if (var2 < 0) {
      if (var2 < -16) var2 = -16;
      var2 = -var2;
      var_out = shl_pos(var1, var2);  /* shl_pos() assumes positive input, avoids circular inline dependency, JHB Mar 2023 */
   }
   else {
      if (var2 >= 15) var_out = (var1 < 0) ? -1 : 0;
      else {
         if (var1 < 0) var_out = ~((~var1) >> var2);
         else var_out = var1 >> var2;
      }
   }

   return var_out;
}

static inline Word16 shl(Word16 var1, Word16 var2) {

Word16 var_out;

   if (var2 < 0) {
      if (var2 < -16) var2 = -16;
      var2 = -var2;
      var_out = shr(var1, var2);
   }
   else var_out = shl_pos(var1, var2);

   return var_out;
}

static inline Word16 norm_l(Word32 L_var1) {

Word16 var_out;

   if (L_var1 == 0) var_out = 0;
   else {

      if (L_var1 == (Word32) 0xffffffffL) var_out = 31;
      else {
         if (L_var1 < 0) L_var1 = ~L_var1;
         for (var_out = 0; L_var1 < (Word32) 0x40000000L; var_out++) L_var1 <<= 1;
      }
   }

   return var_out;
}

static inline Word16 norm_s(Word16 var1) {

Word16 var_out;

   if (var1 == 0) var_out = 0;
   else {
      if (var1 == (Word16) 0xffff) var_out = 15;
      else {
         if (var1 < 0) var1 = ~var1;
         for (var_out = 0; var1 < 0x4000; var_out++) var1 <<= 1;
      }
   }

   return var_out;
}
#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Prototypes for basic arithmetic operators                               |
 |___________________________________________________________________________|
*/

#ifndef USE_BASOPS_INLINE

Word16 negate(Word16 var1);                /* Short negate,        1   */
Word16 sub(Word16 var1, Word16 var2);      /* Short sub,           1   */
Word16 add(Word16 var1, Word16 var2);      /* Short add,           1   */
Word32 L_sub(Word32 L_var1, Word32 L_var2); /* Long sub,        1 */
Word16 mult_r(Word16 var1, Word16 var2);  /* Mult with round, 1 */
Word16 norm_l(Word32 L_var1);             /* Long norm,             1  */
Word16 div_s(Word16 var1, Word16 var2);   /* Short division,       18  */
Word16 norm_s(Word16 var1);               /* Short norm,            1  */

#endif

Word16 sub_ovf (Word16 var1, Word16 var2, Flag *Overflow);  /* Identical to sub() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */
Word16 abs_s (Word16 var1);               /* Short abs,           1   */
Word16 shl (Word16 var1, Word16 var2);    /* Short shift left,    1   */
Word16 shl_ovf (Word16 var1, Word16 var2, Flag *Overflow);  /* Identical to shl() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */
Word16 shr (Word16 var1, Word16 var2);    /* Short shift right,   1   */
Word16 mult (Word16 var1, Word16 var2);   /* Short mult,          1   */
Word32 L_mult (Word16 var1, Word16 var2); /* Long mult,           1   */
Word32 L_mult_ovf (Word16 var1, Word16 var2, Flag *Overflow);  /* Identical to L_mult() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */

#if 0  /* declared above as static inline, JHB Mar 2023 */
Word16 extract_h (Word32 L_var1);         /* Extract high,        1   */
Word16 extract_l (Word32 L_var1);         /* Extract low,         1   */
#endif

Word16 round_fx (Word32 L_var1);          /* Round,               1   */
Word32 L_mac (Word32 L_var3, Word16 var1, Word16 var2);   /* Mac,  1  */
Word32 L_msu (Word32 L_var3, Word16 var1, Word16 var2);   /* Msu,  1  */

#ifndef EXCLUDE_BASOPS_NOT_USED_EVS

/* Exclude L_msuNs() and L_msuNS() from build as they call L_add_c() and L_sub_c() which read/set global Overflow/Carry vars. These functions aren't called anywhere in 3GPP EVS floating-point code - CJ March 2017 */

Word32 L_macNs (Word32 L_var3, Word16 var1, Word16 var2); /* Mac without sat, 1   */
Word32 L_msuNs (Word32 L_var3, Word16 var1, Word16 var2); /* Msu without sat, 1   */
#endif
Word32 L_add (Word32 L_var1, Word32 L_var2);    /* Long add,        1 */
Word32 L_add_ovf (Word32 L_var1, Word32 L_var2, Flag *Overflow);  /* Identical to L_add() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */
Word32 L_sub_ovf (Word32 L_var1, Word32 L_var2, Flag *Overflow);  /* Identical to L_sub() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */

#ifndef EXCLUDE_BASOPS_NOT_USED_EVS
/* Exclude L_add_c() and L_sub_c() from build as they read/set global Overflow/Carry vars, these functions aren't called anywhere in 3GPP EVS floating-point code - CJ March 2017 */
Word32 L_add_c (Word32 L_var1, Word32 L_var2);  /* Long add with c, 2 */
Word32 L_sub_c (Word32 L_var1, Word32 L_var2);  /* Long sub with c, 2 */
#endif

Word32 L_negate (Word32 L_var1);                /* Long negate,     1 */
Word32 L_shl (Word32 L_var1, Word16 var2);      /* Long shift left, 1 */
Word32 L_shl_ovf (Word32 L_var1, Word16 var2, Flag *Overflow); /* Identical to L_shl() but doesn't use global Overflow flag to be thread-safe - CJ MAR2017 */
Word32 L_shr (Word32 L_var1, Word16 var2);      /* Long shift right, 1 */
Word16 shr_r (Word16 var1, Word16 var2);        /* Shift right with round, 2 */
Word16 mac_r (Word32 L_var3, Word16 var1, Word16 var2); /* Mac with rounding, 1 */
Word16 msu_r (Word32 L_var3, Word16 var1, Word16 var2); /* Msu with rounding, 1 */
#if 0  /* declared above as static inline, JHB Mar 2023 */
Word32 L_deposit_h (Word16 var1);        /* 16 bit var1 -> MSB,     1 */
Word32 L_deposit_l (Word16 var1);        /* 16 bit var1 -> LSB,     1 */
#endif

Word32 L_shr_r (Word32 L_var1, Word16 var2); /* Long shift right with round, 3  */
Word32 L_abs (Word32 L_var1);                /* Long abs,              1  */

#ifndef EXCLUDE_BASOPS_NOT_USED_EVS
/* Exclude L_sat() from build as it reads/sets global Overflow/Carry vars, isn't called anywhere in 3GPP EVS floating-point code - CJ March 2017 */
Word32 L_sat (Word32 L_var1);            /* Long saturation,       4  */
#endif

/*
 * Additional G.723.1 operators
*/
Word32 L_mls( Word32, Word16 ) ;    /* Weight FFS; currently assigned 5 */
Word16 div_l( Word32, Word16 ) ;    /* Weight FFS; currently assigned 32 */
Word16 i_mult(Word16 a, Word16 b);  /* Weight FFS; currently assigned 3 */

/*
 *  New shiftless operators, not used in G.729/G.723.1
*/
Word32 L_mult0(Word16 v1, Word16 v2); /* 32-bit Multiply w/o shift         1 */
Word32 L_mac0(Word32 L_v3, Word16 v1, Word16 v2); /* 32-bit Mac w/o shift  1 */
Word32 L_msu0(Word32 L_v3, Word16 v1, Word16 v2); /* 32-bit Msu w/o shift  1 */

#endif /* ifndef _BASIC_OP_H */

/* end of file */
