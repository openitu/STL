 /*****************************************************************************

 *

 *  Complex arithmetic operators

 *

 *****************************************************************************/
#ifndef _COMPLEX_BASIC_OP_H
#define _COMPLEX_BASIC_OP_H

#include "typedef.h"
#include "basop32.h"


#ifdef COMPLEX_OPERATOR

typedef struct
{
  Word32 re;
  Word32 im;
}cmplx;

typedef struct
{
  Word16 re;
  Word16 im;
}cmplx_s;

/*___________________________________________________________________________
 |                                                                           |
 |   Prototypes for complex arithmetic operators                             |
 |___________________________________________________________________________|
*/

cmplx CL_shr (cmplx inp, Word32 shift_val);
cmplx CL_shl (cmplx inp, Word32 shift_val);
cmplx CL_add (cmplx inp1, cmplx inp2);
cmplx CL_sub (cmplx inp1, cmplx inp2);
cmplx CL_scale (cmplx x, Word16 y);
cmplx CL_dscale (cmplx x, Word16 y1, Word16 y2);
cmplx CL_msu_j (cmplx x, cmplx y);
cmplx CL_mac_j (cmplx x, cmplx y);
cmplx CL_move (cmplx x);
Word32 CL_Extract_real (cmplx x);
Word32 CL_Extract_imag (cmplx x);
cmplx CL_form (Word32 re, Word32 im);
cmplx CL_multr_32x16 (cmplx input, cmplx_s coeff);
cmplx CL_negate (cmplx x);
cmplx CL_conjugate (cmplx x);
cmplx CL_mul_j (cmplx input);
cmplx CL_swap_real_imag (cmplx input);
cmplx_s C_add (cmplx_s inp1, cmplx_s inp2);
cmplx_s C_sub (cmplx_s inp1, cmplx_s inp2);
cmplx_s C_mul_j (cmplx_s input);
cmplx_s C_multr (cmplx_s x, cmplx_s c);
cmplx_s C_form (Word16 re, Word16 im );

cmplx C_scale (cmplx_s x, Word16 y);
cmplx_s CL_round32_16 (cmplx x);
cmplx CL_scale_32 (cmplx x, Word32 y);
cmplx CL_dscale_32 (cmplx x, Word32 y1, Word32 y2);
cmplx CL_multr_32x32 (cmplx x, cmplx y);
cmplx_s C_mac_r (cmplx x, cmplx_s y, Word16 c);
cmplx_s C_msu_r (cmplx x, cmplx_s y, Word16 c);
Word16 C_Extract_real (cmplx_s x );
Word16 C_Extract_imag (cmplx_s x );
cmplx_s C_negate (cmplx_s x);
cmplx_s C_conjugate (cmplx_s x);
cmplx_s C_shr (cmplx_s inp, Word16 shift_val);
cmplx_s C_shl (cmplx_s inp, Word16 shift_val);

#endif /* #ifdef COMPLEX_OPERATOR */

#endif /* ifndef _COMPLEX_BASIC_OP_H */

