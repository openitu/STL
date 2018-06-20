
#ifndef __FUNC_PROTO_H__
#define __FUNC_PROTO_H__

// Modify_here  :  Add proto for REF fucntion.

// 64 bit accumulator BASOP
double W_mult_16_16_ref(float var1, float var2);
double W_mac_16_16_ref(double  D_acc, float var1, float var2);
double W_msu_16_16_ref(double  D_acc, float var1, float var2);
double W_add_ref(double var1, double var2);
double W_sub_ref(double var1, double var2);
double W_shl_ref(double var1, short var2);
double W_shr_ref(double var1, short var2);
double W_mac0_16_16_ref(double  D_acc, float var1, float var2);
double W_msu0_16_16_ref(double  D_acc, float var1, float var2);
double W_mult0_16_16_ref(float var1, float var2);
double W_sat_ref(double var1);
double W_shl_ref_sat(double var1, int var2);
double W_lshr_ref(double var1, short var2);
double W_lshl_ref(double var1, short var2);

int W_norm_ref(double var1);

double W_mac_32_16_ref(double  D_acc, float var1, float var2);
double W_msu_32_16_ref(double  D_acc, float var1, float var2);
double W_mult_32_16_ref(float var1, float var2);

double W_mult_32_32_ref(float var1, float var2);
double W_mult0_32_32_ref(double var1, double var2);

double W_neg_ref(double var1);
double W_abs_ref(double var1);

// Enhanced 32 bit BASOP
float Madd_32_16_ref(float  D_acc, float var1, float var2);
float Madd_32_32_ref(float  D_acc, float var1, float var2);
float Mpy_32_16_1_ref(float var1, float var2);
float Mpy_32_32_ref(float var1, float var2);
float Msub_32_16_ref(float  D_acc, float var1, float var2);
float Msub_32_32_ref(float  D_acc, float var1, float var2);

//  Structure for Complex Data type.
typedef struct
{
    float re;
    float im;
}f_cmplx;


f_cmplx CL_add_ref(f_cmplx inp1, f_cmplx inp2);
f_cmplx CL_sub_ref(f_cmplx inp1, f_cmplx inp2);
f_cmplx CL_msu_j_ref(f_cmplx inp1, f_cmplx inp2);
f_cmplx CL_mac_j_ref(f_cmplx inp1, f_cmplx inp2);
f_cmplx CL_multr_32x16_ref(f_cmplx input, f_cmplx coeff);

f_cmplx C_add_ref(f_cmplx inp1, f_cmplx inp2);
f_cmplx C_sub_ref(f_cmplx inp1, f_cmplx inp2);
f_cmplx C_multr_ref(f_cmplx input, f_cmplx coeff);

f_cmplx CL_multr_32x32_ref(f_cmplx input, f_cmplx coeff);
f_cmplx C_scale_ref(f_cmplx x, float y);

f_cmplx CL_negate_ref(f_cmplx x);
f_cmplx CL_conjugate_ref(f_cmplx x);
f_cmplx CL_mul_j_ref(f_cmplx x);

f_cmplx C_mac_r_ref(f_cmplx x, f_cmplx y, float c);
f_cmplx C_msu_r_ref(f_cmplx x, f_cmplx y, float c);
f_cmplx CL_shr_ref(f_cmplx inp, int shift_val);
f_cmplx CL_shl_ref(f_cmplx inp, int shift_val);
f_cmplx C_shr_ref(f_cmplx inp, short int shift_val);
f_cmplx C_shl_ref(f_cmplx inp, short int shift_val);
f_cmplx CL_dscale_ref(f_cmplx x, float y, float z);
f_cmplx CL_round32_16_ref(f_cmplx x);

#endif // __FUNC_PROTO_H__

/* End of file */

