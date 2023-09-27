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
  ============================================================================
*/


#ifndef _BASIC_OP_H
#define _BASIC_OP_H

#ifdef BASOP_NOGLOB
#include <assert.h>
#endif /* BASOP_NOGLOB */


/*___________________________________________________________________________
 |                                                                           |
 |   Constants and Globals                                                   |
 | $Id $
 |___________________________________________________________________________|
*/
#ifdef BASOP_NOGLOB
/* DISABLED TO AVOID GLOBAL VARIABLES */
/*
extern Flag BASOP_Overflow, BASOP_Overflow2;
extern Flag BASOP_Carry;
*/
#else /* BASOP_NOGLOB */
extern Flag Overflow, Overflow2;
extern Flag Carry;
#endif /* BASOP_NOGLOB */

#define MAX_32 (Word32)0x7fffffffL
#define MIN_32 (Word32)0x80000000L

#define MAX_16 (Word16)0x7fff
#define MIN_16 (Word16)0x8000

#ifdef BASOP_NOGLOB
static inline void set_flag(Flag* flag) { if (flag) *flag = 1; else assert(0); }
static inline void unset_flag(Flag* flag) { if (flag) *flag = 0; else assert(0); }
static inline Flag get_flag(Flag* flag) { if (flag) return *flag; else assert(0); }
#endif /* BASOP_NOGLOB */

/*___________________________________________________________________________
 |                                                                           |
 |   Prototypes for basic arithmetic operators                               |
 |___________________________________________________________________________|
*/

Word16 add (Word16 var1, Word16 var2);  /* Short add, 1 */
Word16 sub (Word16 var1, Word16 var2);  /* Short sub, 1 */
Word16 abs_s (Word16 var1);     /* Short abs, 1 */
Word16 shl (Word16 var1, Word16 var2);  /* Short shift left, 1 */
Word16 shr (Word16 var1, Word16 var2);  /* Short shift right, 1 */
Word16 mult (Word16 var1, Word16 var2); /* Short mult, 1 */
Word32 L_mult (Word16 var1, Word16 var2);       /* Long mult, 1 */
Word16 negate (Word16 var1);    /* Short negate, 1 */
Word16 extract_h (Word32 L_var1);       /* Extract high, 1 */
Word16 extract_l (Word32 L_var1);       /* Extract low, 1 */
Word16 round_fx (Word32 L_var1);        /* Round, 1 */
Word32 L_mac (Word32 L_var3, Word16 var1, Word16 var2); /* Mac, 1 */
Word32 L_msu (Word32 L_var3, Word16 var1, Word16 var2); /* Msu, 1 */
Word32 L_macNs (Word32 L_var3, Word16 var1, Word16 var2);       /* Mac without sat, 1 */
Word32 L_msuNs (Word32 L_var3, Word16 var1, Word16 var2);       /* Msu without sat, 1 */
Word32 L_add (Word32 L_var1, Word32 L_var2);    /* Long add, 1 */
Word32 L_sub (Word32 L_var1, Word32 L_var2);    /* Long sub, 1 */
Word32 L_add_c (Word32 L_var1, Word32 L_var2);  /* Long add with c, 2 */
Word32 L_sub_c (Word32 L_var1, Word32 L_var2);  /* Long sub with c, 2 */
Word32 L_negate (Word32 L_var1);        /* Long negate, 1 */
Word16 mult_r (Word16 var1, Word16 var2);       /* Mult with round, 1 */
Word32 L_shl (Word32 L_var1, Word16 var2);      /* Long shift left, 1 */
Word32 L_shr (Word32 L_var1, Word16 var2);      /* Long shift right, 1 */
Word16 shr_r (Word16 var1, Word16 var2);        /* Shift right with round, 2 */
Word16 mac_r (Word32 L_var3, Word16 var1, Word16 var2); /* Mac with rounding, 1 */
Word16 msu_r (Word32 L_var3, Word16 var1, Word16 var2); /* Msu with rounding, 1 */
Word32 L_deposit_h (Word16 var1);       /* 16 bit var1 -> MSB, 1 */
Word32 L_deposit_l (Word16 var1);       /* 16 bit var1 -> LSB, 1 */

Word32 L_shr_r (Word32 L_var1, Word16 var2);    /* Long shift right with round, 3 */
Word32 L_abs (Word32 L_var1);   /* Long abs, 1 */
Word32 L_sat (Word32 L_var1);   /* Long saturation, 4 */
Word16 norm_s (Word16 var1);    /* Short norm, 1 */
Word16 div_s (Word16 var1, Word16 var2);        /* Short division, 18 */
Word16 norm_l (Word32 L_var1);  /* Long norm, 1 */


/*
 * Additional G.723.1 operators
*/
Word32 L_mls (Word32, Word16);  /* Weight FFS; currently assigned 5 */
Word16 div_l (Word32, Word16);  /* Weight FFS; currently assigned 32 */
Word16 i_mult (Word16 a, Word16 b);     /* Weight FFS; currently assigned 3 */

/*
 *  New shiftless operators, not used in G.729/G.723.1
*/
Word32 L_mult0 (Word16 v1, Word16 v2);  /* 32-bit Multiply w/o shift 1 */
Word32 L_mac0 (Word32 L_v3, Word16 v1, Word16 v2);      /* 32-bit Mac w/o shift 1 */
Word32 L_msu0 (Word32 L_v3, Word16 v1, Word16 v2);      /* 32-bit Msu w/o shift 1 */

#ifdef BASOP_NOGLOB
/*
 * Overflowing operators
 */
Word16 add_o (Word16 var1, Word16 var2, Flag * Overflow);
Word16 sub_o (Word16 var1, Word16 var2, Flag * Overflow);
Word16 shl_o (Word16 var1, Word16 var2, Flag * Overflow);
Word16 shr_o (Word16 var1, Word16 var2, Flag * Overflow);
Word16 shr_ro(Word16 var1, Word16 var2, Flag* Overflow);
Word16 mult_o (Word16 var1, Word16 var2, Flag * Overflow);
Word32 L_mult_o (Word16 var1, Word16 var2, Flag * Overflow);
Word16 round_fx_o (Word32 L_var1, Flag * Overflow);
Word32 L_mac_o (Word32 L_var3, Word16 var1, Word16 var2, Flag * Overflow);
Word32 L_msu_o (Word32 L_var3, Word16 var1, Word16 var2, Flag * Overflow);
Word32 L_macNs_co (Word32 L_var3, Word16 var1, Word16 var2, Flag * Carry, Flag* Overflow);
Word32 L_msuNs_co (Word32 L_var3, Word16 var1, Word16 var2, Flag * Carry, Flag* Overflow);
Word32 L_add_o (Word32 L_var1, Word32 L_var2, Flag * Overflow);
Word32 L_sub_o (Word32 L_var1, Word32 L_var2, Flag * Overflow);
Word32 L_add_co (Word32 L_var1, Word32 L_var2, Flag * Carry, Flag *Overflow);
Word32 L_sub_co (Word32 L_var1, Word32 L_var2, Flag * Carry, Flag *Overflow);
Word32 L_shr_o (Word32 L_var1, Word16 var2, Flag * Overflow);
Word32 L_shl_o (Word32 L_var1, Word16 var2, Flag * Overflow);
Word32 L_mls_o (Word32 Lv, Word16 v, Flag * Overflow);
Word32 L_mac0_o (Word32 L_var3, Word16 var1, Word16 var2, Flag * Overflow);
Word32 L_msu0_o (Word32 L_var3, Word16 var1, Word16 var2, Flag * Overflow);
Word16 mult_ro (Word16 var1, Word16 var2, Flag * Overflow);
Word16 mac_ro (Word32 L_var3, Word16 var1, Word16 var2, Flag * Overflow);
Word16 msu_ro (Word32 L_var3, Word16 var1, Word16 var2, Flag * Overflow);
Word32 L_sat_co (Word32 L_var1, Flag Carry, Flag Overflow);
#endif /* BASOP_NOGLOB */

#endif /* ifndef _BASIC_OP_H */


/* end of file */
