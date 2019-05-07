/*****************************************************************************

*

*  Complex arithmetic operators

*

*****************************************************************************/


#include "typedef.h"
#include "basop32.h"

#include "complex_basop.h"

#include "stl.h"



#if (WMOPS)

extern BASIC_OP multiCounter[MAXCOUNTERS];

extern int currCounter;

#endif /* if WMOPS */



#ifdef COMPLEX_OPERATOR

/* ================================ New Complex Basops ========================= */

cmplx CL_shr (cmplx inp, Word32 shift_val) {
  cmplx out;
  out.re = L_shr (inp.re, shift_val);
  out.im = L_shr (inp.im, shift_val);
#if (WMOPS)
  multiCounter[currCounter].CL_shr++;
  multiCounter[currCounter].L_shr--;
  multiCounter[currCounter].L_shr--;
#endif  
  return out;
}

cmplx CL_shl (cmplx inp, Word32 shift_val) {
  cmplx out;
  out.re = L_shl (inp.re, shift_val);
  out.im = L_shl (inp.im, shift_val);
#if (WMOPS)
  multiCounter[currCounter].CL_shl++;
  multiCounter[currCounter].L_shl--;
  multiCounter[currCounter].L_shl--;
#endif
  return out;
}

cmplx CL_add (cmplx inp1, cmplx inp2) {
  cmplx out;
  out.re = L_add (inp1.re, inp2.re);
  out.im = L_add (inp1.im, inp2.im);
#if (WMOPS)
  multiCounter[currCounter].CL_add++;
  multiCounter[currCounter].L_add--;
  multiCounter[currCounter].L_add--;
#endif
  return out;
}

cmplx CL_sub (cmplx inp1, cmplx inp2) {
  cmplx out;
  out.re = L_sub (inp1.re, inp2.re);
  out.im = L_sub (inp1.im, inp2.im);
#if (WMOPS)
  multiCounter[currCounter].CL_sub++;
  multiCounter[currCounter].L_sub--;
  multiCounter[currCounter].L_sub--;
#endif
  return out;
}

cmplx CL_scale(cmplx x, Word16 y) {
  cmplx result;
  result.re = Mpy_32_16_r (x.re, y);
  result.im = Mpy_32_16_r (x.im, y);
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_16_r--;
  multiCounter[currCounter].Mpy_32_16_r--;
  multiCounter[currCounter].CL_scale++;
#endif/* #if (WMOPS) */
  return (result);
}

cmplx CL_dscale (cmplx x, Word16 y1, Word16 y2) {
  cmplx result;
  result.re = Mpy_32_16_r (x.re, y1);
  result.im = Mpy_32_16_r (x.im, y2);
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_16_r--;
  multiCounter[currCounter].Mpy_32_16_r--;
  multiCounter[currCounter].CL_dscale++;
#endif/* #if (WMOPS) */
  return (result);
}

cmplx CL_msu_j (cmplx x, cmplx y) {
  cmplx result;
  result.re = L_add (x.re, y.im);
  result.im = L_sub (x.im, y.re);
#if (WMOPS)
  multiCounter[currCounter].CL_msu_j++;
  multiCounter[currCounter].L_add--;
  multiCounter[currCounter].L_sub--;
#endif          
  return result;
}

cmplx CL_mac_j (cmplx x, cmplx y) {
  cmplx result;
  result.re = L_sub (x.re, y.im );
  result.im = L_add (x.im, y.re );
#if (WMOPS)
  multiCounter[currCounter].CL_mac_j++;
  multiCounter[currCounter].L_add--;
  multiCounter[currCounter].L_sub--;
#endif            
  return result;
}

cmplx CL_move (cmplx x) {
#if (WMOPS)
  multiCounter[currCounter].CL_move++;
#endif            
  return x;
}

Word32 CL_Extract_real (cmplx x) {
#if (WMOPS)
  multiCounter[currCounter].CL_Extract_real++;
#endif            
  return x.re;
}

Word32 CL_Extract_imag (cmplx x) {
#if (WMOPS)
  multiCounter[currCounter].CL_Extract_imag++;
#endif            
  return x.im;
}

cmplx CL_form (Word32 re, Word32 im) {
  cmplx result;
  result.re = re;
  result.im = im;
#if (WMOPS)
  multiCounter[currCounter].CL_form++;
#endif          
  return result;
}

cmplx CL_multr_32x16 (cmplx input, cmplx_s coeff) {
  cmplx result;
  result.re = W_round48_L (W_sub_nosat (W_mult_32_16 (input.re, coeff.re), W_mult_32_16 (input.im, coeff.im) ) );
  result.im = W_round48_L (W_add_nosat (W_mult_32_16 (input.re, coeff.im), W_mult_32_16 (input.im, coeff.re) ) );
#if (WMOPS)
  multiCounter[currCounter].CL_multr_32x16++;
  multiCounter[currCounter].W_mult_32_16--;
  multiCounter[currCounter].W_mult_32_16--;
  multiCounter[currCounter].W_mult_32_16--;
  multiCounter[currCounter].W_mult_32_16--;
  multiCounter[currCounter].W_sub_nosat--;
  multiCounter[currCounter].W_add_nosat--;
  multiCounter[currCounter].W_round48_L--;
  multiCounter[currCounter].W_round48_L--;
#endif          
  return result;
}

cmplx CL_negate(cmplx x) {
  cmplx result;
  result.re = L_negate (x.re);
  result.im = L_negate (x.im);
#if (WMOPS)
  multiCounter[currCounter].CL_negate++;
  multiCounter[currCounter].L_negate--;
  multiCounter[currCounter].L_negate--;
#endif          
  return result;
}

cmplx CL_conjugate (cmplx x) {
  cmplx result;
  result.re = x.re;
  result.im = L_negate (x.im);
#if (WMOPS)
  multiCounter[currCounter].CL_conjugate++;
  multiCounter[currCounter].L_negate--;
#endif          
  return result;
}


cmplx CL_mul_j (cmplx input) {
  cmplx temp, result;
  temp = CL_negate (input);
  result.re = temp.im;
  result.im = input.re;
#if (WMOPS)
  multiCounter[currCounter].CL_mul_j++;  
  multiCounter[currCounter].CL_negate--;  
#endif          
  return result;
}

cmplx CL_swap_real_imag (cmplx input) {
  cmplx result;
  result.re = input.im;
  result.im = input.re;
#if (WMOPS)
  multiCounter[currCounter].CL_swap_real_imag++;  
#endif            
  return result;
}

cmplx_s C_add (cmplx_s inp1, cmplx_s inp2) {
  cmplx_s out;
  out.re = add (inp1.re, inp2.re);
  out.im = add (inp1.im, inp2.im);

#if (WMOPS)
  multiCounter[currCounter].C_add++;
  multiCounter[currCounter].add--;
  multiCounter[currCounter].add--;
#endif    
  return out;
}

cmplx_s C_sub (cmplx_s inp1, cmplx_s inp2) {
  cmplx_s out;
  out.re = sub (inp1.re, inp2.re);
  out.im = sub (inp1.im, inp2.im);

#if (WMOPS)
  multiCounter[currCounter].C_sub++;
  multiCounter[currCounter].sub--;
  multiCounter[currCounter].sub--;
#endif      
  return out;
}

cmplx_s C_mul_j (cmplx_s input) {
  cmplx_s result;
  Word16 temp;
  temp = negate(input.im);
  result.re = temp;
  result.im = input.re;

#if (WMOPS)
  multiCounter[currCounter].C_mul_j++;
  multiCounter[currCounter].negate--;
#endif      
  return result;
}

cmplx_s C_multr (cmplx_s x, cmplx_s c) {
  cmplx_s result;
  result.re = round_fx (W_sat_l (W_sub_nosat (W_mult_16_16 (x.re, c.re), W_mult_16_16 (x.im, c.im) ) ) );
  result.im = round_fx (W_sat_l (W_add_nosat (W_mult_16_16 (x.im, c.re), W_mult_16_16 (x.re, c.im) ) ) );

#if (WMOPS)
  multiCounter[currCounter].C_multr++;
  multiCounter[currCounter].W_mult_16_16--;
  multiCounter[currCounter].W_mult_16_16--;
  multiCounter[currCounter].W_mult_16_16--;
  multiCounter[currCounter].W_mult_16_16--;
  multiCounter[currCounter].W_sub_nosat--;
  multiCounter[currCounter].W_add_nosat--;
  multiCounter[currCounter].W_sat_l--;
  multiCounter[currCounter].W_sat_l--;  
  multiCounter[currCounter].round--;
  multiCounter[currCounter].round--;
#endif      
  return result;
}

cmplx_s C_form ( Word16 re, Word16 im) {
  cmplx_s result;
  result.re = re;
  result.im = im;
#if (WMOPS)
  multiCounter[currCounter].C_form++;
#endif          
  return result;
}

cmplx C_scale(cmplx_s x, Word16 y) {
  cmplx result;
  result.re = L_mult(x.re, y);
  result.im = L_mult(x.im, y);
#if (WMOPS)
  multiCounter[currCounter].L_mult--;
  multiCounter[currCounter].L_mult--;
  multiCounter[currCounter].C_scale++;
#endif/* #if (WMOPS) */
  return (result);
}

cmplx_s CL_round32_16 (cmplx x) {
  cmplx_s result;
  result.re = round_fx (x.re);
  result.im = round_fx (x.im);

#if (WMOPS)
  multiCounter[currCounter].CL_round32_16++;
  multiCounter[currCounter].round--;
  multiCounter[currCounter].round--;
#endif      
  return result;
}

cmplx CL_scale_32 (cmplx x, Word32 y) {
  cmplx result;
  result.re = Mpy_32_32_r (x.re, y);
  result.im = Mpy_32_32_r (x.im, y);
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_32_r--;
  multiCounter[currCounter].Mpy_32_32_r--;
  multiCounter[currCounter].CL_scale_32++;
#endif/* #if (WMOPS) */
  return (result);
}

cmplx CL_dscale_32 (cmplx x, Word32 y1, Word32 y2) {
  cmplx result;
  result.re = Mpy_32_32_r (x.re, y1);
  result.im = Mpy_32_32_r (x.im, y2);
#if (WMOPS)
  multiCounter[currCounter].Mpy_32_32_r--;
  multiCounter[currCounter].Mpy_32_32_r--;
  multiCounter[currCounter].CL_dscale_32++;
#endif/* #if (WMOPS) */
  return (result);
}

cmplx CL_multr_32x32 (cmplx x, cmplx y) {
  cmplx result;
  result.re = W_round64_L (W_sub (W_mult_32_32 (x.re, y.re), W_mult_32_32(x.im, y.im) ) ) ;
  result.im = W_round64_L (W_add (W_mult_32_32 (x.im, y.re), W_mult_32_32(x.re, y.im) ) ) ;

#if (WMOPS)
  multiCounter[currCounter].CL_multr_32x32++;
  multiCounter[currCounter].W_mult_32_32--;
  multiCounter[currCounter].W_mult_32_32--;
  multiCounter[currCounter].W_mult_32_32--;
  multiCounter[currCounter].W_mult_32_32--;
  multiCounter[currCounter].W_round64_L--;
  multiCounter[currCounter].W_round64_L--;
  multiCounter[currCounter].W_sub--;
  multiCounter[currCounter].W_add--;  
#endif      
  return result;
}

cmplx_s C_mac_r (cmplx x, cmplx_s y, Word16 c) {
  cmplx_s result;
  cmplx temp =  CL_add (x, C_scale (y, c) );
  result = CL_round32_16 (temp);

#if (WMOPS)
  multiCounter[currCounter].C_mac_r++;
  multiCounter[currCounter].CL_add--;
  multiCounter[currCounter].C_scale--;
  multiCounter[currCounter].CL_round32_16--;  
#endif      
  return result;
}

cmplx_s C_msu_r (cmplx x, cmplx_s y, Word16 c) {
  cmplx_s result;
  cmplx temp =  CL_sub (x, C_scale (y, c) );
  result = CL_round32_16 (temp);

#if (WMOPS)
  multiCounter[currCounter].C_msu_r++;
  multiCounter[currCounter].CL_sub--;
  multiCounter[currCounter].C_scale--;
  multiCounter[currCounter].CL_round32_16--;  
#endif      
  return result;
}

Word16 C_Extract_real (cmplx_s x) {
#if (WMOPS)
  multiCounter[currCounter].C_Extract_real++;
#endif            
  return x.re;
}

Word16 C_Extract_imag (cmplx_s x) {
#if (WMOPS)
  multiCounter[currCounter].C_Extract_imag++;
#endif            
  return x.im;
}

cmplx_s C_negate (cmplx_s x) {
  cmplx_s result;
  result.re = negate (x.re);
  result.im = negate (x.im);
#if (WMOPS)
  multiCounter[currCounter].C_negate++;
  multiCounter[currCounter].negate--;
  multiCounter[currCounter].negate--;
#endif          
  return result;
}

cmplx_s C_conjugate (cmplx_s x) {
  cmplx_s result;
  result.re = x.re;
  result.im = negate (x.im);
#if (WMOPS)
  multiCounter[currCounter].C_conjugate++;
  multiCounter[currCounter].negate--;
#endif          
  return result;
}

cmplx_s C_shr (cmplx_s inp, Word16 shift_val) {
  cmplx_s out;
  out.re = shr (inp.re, shift_val);
  out.im = shr (inp.im, shift_val);
#if (WMOPS)
  multiCounter[currCounter].C_shr++;
  multiCounter[currCounter].shr--;
  multiCounter[currCounter].shr--;
#endif  
  return out;
}

cmplx_s C_shl (cmplx_s inp, Word16 shift_val) {
  cmplx_s out;
  out.re = shl (inp.re, shift_val);
  out.im = shl (inp.im, shift_val);
#if (WMOPS)
  multiCounter[currCounter].C_shl++;
  multiCounter[currCounter].shl--;
  multiCounter[currCounter].shl--;
#endif  
  return out;
}

#endif /* #ifdef COMPLEX_OPERATOR */

/* end of file */

