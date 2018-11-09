


#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <complex.h>
#include <time.h>
#include "input_config.h"


#define ABS_VAL(x) (((x)>0)?(x):(-(x)))
#define HEX_VAL_F(x) (*((unsigned *)(&x)))
#define HEX_VAL_D(x) (*((long long unsigned *)(&x)))

FILE *fp_prec;
FILE *fp_prec_debug;
FILE *fp_ip_op_plot;
FILE *fp_spcl;
FILE *fp_test_baseop[TOTAL_NUM_FUNC][NUM_TEST_PATTERN_1];

// 1 input 1 output
#define FUNC_TEST_1ISO_REF(func_ptr, in1, out, loop_cnt) { \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           out[i] = func_ptr(in1[i]);                 \
		         }                                           \
}

// 2 input 1 output
#define FUNC_TEST_2ISO_REF(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           out[i] = func_ptr(in1[i], in2[i]);                 \
         }                                           \
}

// 2 input 1 output
#define FUNC_TEST_2ISO_SHIFT_REF(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           out[i] = func_ptr((double)in1[i], in2[i]);                 \
                 }                                           \
}

// 3 input 1 output
#define FUNC_TEST_3ISO_REF(func_ptr, in1, in2, in3, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           out[i] = func_ptr(in1[i], in2[i], in3[i]);                 \
         }                                           \
}

// 3 input 1 output
#define FUNC_TEST_3ISO_INT_REF(func_ptr, in1, in2, in3, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           out[i] = func_ptr((double)in1[i], (float)in2[i], (float)in3[i]);                 \
                 }                                           \
}


// Constants for float <-> fixed point conversion.
#define DUBLE_MULT_FACT 2147483648.0
#define SHORT_MULT_FACT 32768.0
#define Q47_MULT_FACT 140737488355328.0
#define Q63_MULT_FACT 9223372036854775808.0

// Modify_here  : add new function type.  Do appropriate data conversion.
#define FUNC_TEST_OL_IW_dut(func_ptr, in1, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
		    out[i] = func_ptr((long long)(in1[i]*(Q63_MULT_FACT))); \
    } \
}

#define FUNC_TEST_OS_IW_dut(func_ptr, in1, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
		    out[i] = (short)func_ptr((long long)(in1[i]*(Q63_MULT_FACT))); \
    } \
}

#define FUNC_TEST_OL_IW32_dut(func_ptr, in1, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
		    out[i] = func_ptr((long long)(in1[i]*(DUBLE_MULT_FACT))); \
            out[i] = ((out[i]*1.0)/DUBLE_MULT_FACT); \
    } \
}
#define FUNC_TEST_OL_IW32_IL_dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
		    out[i] = func_ptr((long long)(in1[i]*(DUBLE_MULT_FACT)), in2[i]); \
            out[i] = ((out[i]*1.0)/DUBLE_MULT_FACT); \
    } \
}


#define FUNC_TEST_OS_IW48_dut(func_ptr, in1, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
		    out[i] = func_ptr((long long)(in1[i]*(Q47_MULT_FACT))); \
            out[i] = ((out[i]*1.0)/SHORT_MULT_FACT); \
    } \
}

#define FUNC_TEST_OL_IW48_dut(func_ptr, in1, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
		    out[i] = func_ptr((long long)(in1[i]*(Q47_MULT_FACT))); \
            out[i] = ((out[i]*1.0)/DUBLE_MULT_FACT); \
    } \
}
#define FUNC_TEST_OL_IW64_dut(func_ptr, in1, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
		    out[i] = func_ptr((long long)(in1[i]*(Q63_MULT_FACT))); \
            out[i] = ((out[i]*1.0)/DUBLE_MULT_FACT); \
    } \
}

#define FUNC_TEST_OW_IW_dut(func_ptr, in1, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
		    Word64 Val = func_ptr((long long)(in1[i]*(DUBLE_MULT_FACT))); \
            out[i] = ((Val*1.0)/DUBLE_MULT_FACT); \
    } \
}

#define FUNC_TEST_OW_IL_IS_dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
		    Word64 Val = func_ptr((long )(in1[i]*(DUBLE_MULT_FACT)), (short)(in2[i]*(SHORT_MULT_FACT))); \
            out[i] = ((Val*1.0)/Q47_MULT_FACT); \
    } \
}

#define FUNC_TEST_OW_IS_IS_0_dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
		    out[i] = (double)(func_ptr((short )(in1[i]), (short)(in2[i]))); \
        } \
}

#define FUNC_TEST_OW_IS_IS_dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
		    Word64 Val = func_ptr((short)(in1[i]*(SHORT_MULT_FACT)), (short)(in2[i]*(SHORT_MULT_FACT))); \
            out[i] = ((Val*1.0)/DUBLE_MULT_FACT); \
        } \
}
#define FUNC_TEST_OCL_ICL_ICL_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx in1_tmp, in2_tmp, out_tmp;  \
			in1_tmp.re = (int)(in1[i].re*DUBLE_MULT_FACT); in1_tmp.im = (int)(in1[i].im*DUBLE_MULT_FACT); \
			in2_tmp.re = (int)(in2[i].re*DUBLE_MULT_FACT); in2_tmp.im = (int)(in2[i].im*DUBLE_MULT_FACT); \
            out_tmp = func_ptr(in1_tmp, in2_tmp);                 \
            out[i].re = (float)((out_tmp.re*1.0)/DUBLE_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/DUBLE_MULT_FACT); \
         }                                           \
}

#define FUNC_TEST_OCL_ICL_Dut(func_ptr, in1, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx in1_tmp, out_tmp;  \
			in1_tmp.re = (int)(in1[i].re*DUBLE_MULT_FACT); in1_tmp.im = (int)(in1[i].im*DUBLE_MULT_FACT); \
			out_tmp = func_ptr(in1_tmp);                 \
            out[i].re = (float)((out_tmp.re*1.0)/DUBLE_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/DUBLE_MULT_FACT); \
		         }                                           \
}

#define FUNC_TEST_OCL_ICL_IC_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx in1_tmp, out_tmp;  \
			cmplx_s in2_tmp; \
			in1_tmp.re = (int)(in1[i].re*DUBLE_MULT_FACT); in1_tmp.im = (int)(in1[i].im*DUBLE_MULT_FACT); \
			in2_tmp.re = (short int)(in2[i].re*SHORT_MULT_FACT); in2_tmp.im = (short int)(in2[i].im*SHORT_MULT_FACT); \
            out_tmp = func_ptr(in1_tmp, in2_tmp);                 \
            out[i].re = (float)((out_tmp.re*1.0)/DUBLE_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/DUBLE_MULT_FACT); \
         }                                           \
}

#define FUNC_TEST_OCL_ICL_IL_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx in1_tmp, out_tmp;  \
			int in2_tmp; \
			in1_tmp.re = (int)(in1[i].re*DUBLE_MULT_FACT); in1_tmp.im = (int)(in1[i].im*DUBLE_MULT_FACT); \
			in2_tmp = (int)(in2[i]*DUBLE_MULT_FACT); \
            out_tmp = func_ptr(in1_tmp, in2_tmp);                 \
            out[i].re = (float)((out_tmp.re*1.0)/DUBLE_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/DUBLE_MULT_FACT); \
		         }                                           \
}

#define FUNC_TEST_OCL_ICL_IS_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx in1_tmp, out_tmp;  \
			short int in2_tmp; \
			in1_tmp.re = (int)(in1[i].re*DUBLE_MULT_FACT); in1_tmp.im = (int)(in1[i].im*DUBLE_MULT_FACT); \
			in2_tmp = (short int)(in2[i]*SHORT_MULT_FACT); \
            out_tmp = func_ptr(in1_tmp, in2_tmp);                 \
            out[i].re = (float)((out_tmp.re*1.0)/DUBLE_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/DUBLE_MULT_FACT); \
				         }                                           \
}

#define FUNC_TEST_OCL_ICL_IS_IS_Dut(func_ptr, in1, in2, in3, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx in1_tmp, out_tmp;  \
			short int in2_tmp, in3_tmp; \
			in1_tmp.re = (int)(in1[i].re*DUBLE_MULT_FACT); in1_tmp.im = (int)(in1[i].im*DUBLE_MULT_FACT); \
			in2_tmp = (short int)(in2[i]*SHORT_MULT_FACT); \
			in3_tmp = (short int)(in3[i]*SHORT_MULT_FACT); \
            out_tmp = func_ptr(in1_tmp, in2_tmp, in3_tmp);                 \
            out[i].re = (float)((out_tmp.re*1.0)/DUBLE_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/DUBLE_MULT_FACT); \
						         }                                           \
}

#define FUNC_TEST_OCL_ICL_IL_IL_Dut(func_ptr, in1, in2, in3, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx in1_tmp, out_tmp;  \
			int in2_tmp, in3_tmp; \
			in1_tmp.re = (int)(in1[i].re*DUBLE_MULT_FACT); in1_tmp.im = (int)(in1[i].im*DUBLE_MULT_FACT); \
			in2_tmp = (int)(in2[i]*DUBLE_MULT_FACT); \
			in3_tmp = (int)(in3[i]*DUBLE_MULT_FACT); \
            out_tmp = func_ptr(in1_tmp, in2_tmp, in3_tmp);                 \
            out[i].re = (float)((out_tmp.re*1.0)/DUBLE_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/DUBLE_MULT_FACT); \
								         }                                           \
}

#define FUNC_TEST_OC_ICL_Dut(func_ptr, in1, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx in1_tmp;  \
			cmplx_s out_tmp; \
			in1_tmp.re = (int)(in1[i].re*DUBLE_MULT_FACT); in1_tmp.im = (int)(in1[i].im*DUBLE_MULT_FACT); \
            out_tmp = func_ptr(in1_tmp);                 \
            out[i].re = (float)((out_tmp.re*1.0)/SHORT_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/SHORT_MULT_FACT); \
										         }                                           \
}



#define FUNC_TEST_OCL_ICL_IL0_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx in1_tmp, out_tmp;  \
			in1_tmp.re = (int)(in1[i].re*DUBLE_MULT_FACT); in1_tmp.im = (int)(in1[i].im*DUBLE_MULT_FACT); \
			out_tmp = func_ptr(in1_tmp, in2[i]);                 \
            out[i].re = (float)((out_tmp.re*1.0)/DUBLE_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/DUBLE_MULT_FACT); \
		         }                                           \
}

#define FUNC_TEST_OC_IC_IS0_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx_s in1_tmp, out_tmp;  \
			in1_tmp.re = (int)(in1[i].re*SHORT_MULT_FACT); in1_tmp.im = (int)(in1[i].im*SHORT_MULT_FACT); \
			out_tmp = func_ptr(in1_tmp, in2[i]);                 \
            out[i].re = (float)((out_tmp.re*1.0)/SHORT_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/SHORT_MULT_FACT); \
				         }                                           \
}



#define FUNC_TEST_OC_IC_IC_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx_s in1_tmp, in2_tmp, out_tmp;  \
			in1_tmp.re = (short int)(in1[i].re*SHORT_MULT_FACT); in1_tmp.im = (short int)(in1[i].im*SHORT_MULT_FACT); \
			in2_tmp.re = (short int)(in2[i].re*SHORT_MULT_FACT); in2_tmp.im = (short int)(in2[i].im*SHORT_MULT_FACT); \
            out_tmp = func_ptr(in1_tmp, in2_tmp);                 \
            out[i].re = (float)((out_tmp.re*1.0)/SHORT_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/SHORT_MULT_FACT); \
         }                                           \
}

#define FUNC_TEST_OC_IC_Dut(func_ptr, in1, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx_s in1_tmp, out_tmp;  \
			in1_tmp.re = (short int)(in1[i].re*SHORT_MULT_FACT); in1_tmp.im = (short int)(in1[i].im*SHORT_MULT_FACT); \
			out_tmp = func_ptr(in1_tmp);                 \
            out[i].re = (float)((out_tmp.re*1.0)/SHORT_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/SHORT_MULT_FACT); \
		         }                                           \
}


#define FUNC_TEST_OCL_IC_IS_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx_s in1_tmp;  \
			short int in2_tmp;  \
			cmplx out_tmp;  \
			in1_tmp.re = (short int)(in1[i].re*SHORT_MULT_FACT); in1_tmp.im = (short int)(in1[i].im*SHORT_MULT_FACT); \
			in2_tmp = (short int)(in2[i]*SHORT_MULT_FACT);\
            out_tmp = func_ptr(in1_tmp, in2_tmp);                 \
            out[i].re = (float)((out_tmp.re*1.0)/DUBLE_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/DUBLE_MULT_FACT); \
         }                                           \
}

#define FUNC_TEST_OC_ICL_IC_IS_Dut(func_ptr, in1, in2, in3, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
			cmplx in1_tmp;  \
			cmplx_s in2_tmp;  \
			short int in3_tmp;  \
			cmplx_s out_tmp;  \
			in1_tmp.re = (int)(in1[i].re*DUBLE_MULT_FACT); in1_tmp.im = (int)(in1[i].im*DUBLE_MULT_FACT); \
			in2_tmp.re = (short int)(in2[i].re*SHORT_MULT_FACT); in2_tmp.im = (short int)(in2[i].im*SHORT_MULT_FACT); \
			in3_tmp = (short int)(in3[i]*SHORT_MULT_FACT);\
            out_tmp = func_ptr(in1_tmp, in2_tmp,in3_tmp);                 \
            out[i].re = (float)((out_tmp.re*1.0)/SHORT_MULT_FACT); \
			out[i].im = (float)((out_tmp.im*1.0)/SHORT_MULT_FACT); \
		         }                                           \
}

// DUT function which takes 3 input (float, float, float) and returns 1  float output
#define FUNC_TEST_3ISO_enh32_Dut(func_ptr, in1, in2, in3, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           Word32 val = func_ptr((long)(in1[i]*(DUBLE_MULT_FACT)), (long)(in2[i]*(DUBLE_MULT_FACT)), (short)(in3[i]*(SHORT_MULT_FACT)));                 \
            out[i] = (float)((val*1.0)/DUBLE_MULT_FACT); \
         }                                           \
}
#define FUNC_TEST_3ISO_enh32_32_Dut(func_ptr, in1, in2, in3, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           Word32 val = func_ptr((long)(in1[i]*(DUBLE_MULT_FACT)), (long)(in2[i]*(DUBLE_MULT_FACT)), (long)(in3[i]*(DUBLE_MULT_FACT)));                 \
            out[i] = (float)(val/DUBLE_MULT_FACT); \
         }                                           \
}

// DUT function which takes 2 input (float, float) and returns 1  float output
#define FUNC_TEST_2ISO_enh32_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           Word32 Val = func_ptr((long)(in1[i]*(DUBLE_MULT_FACT)), (short)(in2[i]*(SHORT_MULT_FACT)));                 \
            out[i] = (float)((Val*1.0)/DUBLE_MULT_FACT); \
         }                                           \
}

#define FUNC_TEST_2ISO_enh32_32_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           Word32 val = func_ptr((long)(in1[i]*(DUBLE_MULT_FACT)), (long)(in2[i]*(DUBLE_MULT_FACT)));                 \
            out[i] = (float)((val*1.0)/DUBLE_MULT_FACT); \
         }                                           \
}

#define FUNC_TEST_OD_ID_IS_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           Word64 Val = func_ptr((long long)(in1[i]*(DUBLE_MULT_FACT)), in2[i] );                 \
            out[i] = (Val * 1.0)/DUBLE_MULT_FACT; \
         }                                           \
}

#define FUNC_TEST_OD_ID64_IS_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           Word64 Val = func_ptr((in1[i]), in2[i] );                 \
            out[i] = (Val * 1.0); \
         }                                           \
}
#define FUNC_TEST_OD_ID_ID_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           Word64 Val = func_ptr((long long)(in1[i]*(DUBLE_MULT_FACT)), (long long)(in2[i]*(DUBLE_MULT_FACT)));                 \
            out[i] = (Val * 1.0)/DUBLE_MULT_FACT; \
         }                                           \
}

#define FUNC_TEST_OD_ID_IF_IF_Dut(func_ptr, in1, in2, in3, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           Word64 Val = func_ptr((long long)((in1[i])), (short)((in2[i])),(short)((in3[i])));                 \
            out[i] = (double)(Val); \
         }                                           \
}

#define FUNC_TEST_OD_ID_IFL_IF_Dut(func_ptr, in1, in2, in3, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           Word64 Val = func_ptr((long long)(in1[i]*(Q47_MULT_FACT)), (long)(in2[i]*(DUBLE_MULT_FACT)),(short)(in3[i]*(SHORT_MULT_FACT)));                 \
            out[i] = (Val * 1.0)/Q47_MULT_FACT; \
         }                                           \
}

#define FUNC_TEST_OD_IFL_IFL_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           Word64 Val = func_ptr((long )(in1[i]*(DUBLE_MULT_FACT)), (long )(in2[i]*(DUBLE_MULT_FACT)));                 \
            out[i] = (Val * 1.0)/Q63_MULT_FACT; \
         }                                           \
}

#define FUNC_TEST_OD_IFL_IFL_0_Dut(func_ptr, in1, in2, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           out[i] = (double)(func_ptr((long long)(in1[i]), (long long)(in2[i])));                 \
         }                                           \
}

// DUT function which takes 3 input (Double, float, float) and returns 1  Double output
#define FUNC_TEST_3ISO_Dut(func_ptr, in1, in2, in3, out, loop_cnt){ \
	int i;                                       \
        for (i=0; i<loop_cnt;i++){                   \
           Word64 Val = func_ptr((long long)((double)in1[i]*(DUBLE_MULT_FACT)), (short)(in2[i]*(SHORT_MULT_FACT)), (short)(in3[i]*(SHORT_MULT_FACT)));                 \
            out[i] = (Val*1.0)/DUBLE_MULT_FACT; \
         }                                           \
}


void print_test_out(int test_index, FILE * fp, int index, int f_type1, char *fnm1, int f_type2, char *fnm2,  void *out_dut) //void *in1_1, void *in2_1, void *in3_1, void *out_ref,

{

    switch (input_prec_tests[test_index].out_data)
    {
    case DATA_TYPE_DOUBLE:
	case DATA_TYPE_DOUBLE_Q31:
	case DATA_TYPE_DOUBLE_Q47:
	case DATA_TYPE_DOUBLE_Q63:
    {
        double * outf_d = (double *)((char *)out_dut + (sizeof(double)*index));
	unsigned long long * res = (unsigned long long *)outf_d;
	fprintf(fp, "%llX,\n", *res);
    }
    break;
    case DATA_TYPE_FLOAT:
    case DATA_TYPE_FLOAT_L:
    {
        float * outf_d = (float *)((char *)out_dut + (sizeof(float)*index));
	unsigned long * res = (unsigned long *)outf_d;
	fprintf(fp, "%X,\n", *res);
    }
    break;
    case DATA_TYPE_LONG:
    {
        int * outf_d = (int *)((char *)out_dut + (sizeof(int)*index));
	unsigned long * res = (unsigned long *)outf_d;
	fprintf(fp, "%X,\n", *res);
    }
    break;
    case DATA_TYPE_SHORT:
    {
        short * outf_d = (short *)((char *)out_dut + (sizeof(short)*index));
	fprintf(fp, "%X,\n", (unsigned int)*outf_d);
    }
    break;
    case DATA_TYPE_CMPLX_S:
    {
        f_cmplx * outf_d = (f_cmplx *)((char *)out_dut + (sizeof(f_cmplx)*index));
	float res = (sqrt(outf_d->re*outf_d->re + outf_d->im*outf_d->im));
	unsigned long *res1 = (unsigned long *)&res;
	fprintf(fp, "%X,\n", *res1);
    }
    break;
    case DATA_TYPE_CMPLX_L:
    {
        f_cmplx * outf_d = (f_cmplx *)((char *)out_dut + (sizeof(f_cmplx)*index));
	float res = sqrt((outf_d->re*outf_d->re + outf_d->im*outf_d->im));
	unsigned long *res1 = (unsigned long *)&res;
	fprintf(fp, "%X,\n", *res1);
    }
    break;
    case DATA_TYPE_WORD64:
    {
        long long  * outf_d = (long long  *)((char *)out_dut + (sizeof(long long)*index));
        fprintf(fp, "%llX,\n" , *outf_d);

    }
    break;
    default:
        printf("Error: Invalid output datatype \n");
    }

}

void print_io_plot(int test_index, FILE * fp, int index, int f_type1, char *fnm1, int f_type2, char *fnm2, void *in1_1, void *in2_1, void *in3_1, void *out_dut, void *out_ref)

{
    int idx;
    for (idx = 0; idx < MAX_NUM_INP; idx++)
    {
        char * in1 = NULL;
        if (idx == 0)
        {
            in1 = in1_1;
        }
        else if (idx == 1)
        {
            in1 = in2_1;
        }
        else if (idx == 2)
        {
            in1 = in3_1;
        }
        switch (input_prec_tests[test_index].inp_data[idx])
        {
        case DATA_TYPE_DOUBLE:
		case DATA_TYPE_DOUBLE_Q31:
		case DATA_TYPE_DOUBLE_Q47:
		case DATA_TYPE_DOUBLE_Q63:
        {
            double * inf1 = (double *)((char *)in1 + (sizeof(double)*index));
            fprintf(fp, "%0.10f,", *inf1);
        }
        break;
        case DATA_TYPE_FLOAT_L:
        case DATA_TYPE_FLOAT:
        {
            float * inf1 = (float *)((char *)in1 + (sizeof(float)*index));
            fprintf(fp, "%0.10f,", *inf1);
        }
        break;
        case DATA_TYPE_LONG:
        {
            int * inf1 = (int *)((char *)in1 + (sizeof(int)*index));
            fprintf(fp, "%d,", *inf1);
        }
        break;
        case DATA_TYPE_SHORT:
        {
            short * inf1 = (short *)((char *)in1 + (sizeof(short)*index));
            fprintf(fp, "%d,", *inf1);
        }
        break;
        case DATA_TYPE_WORD64:
        {
            long long * inf1 = (long long *)((char *)in1 + (sizeof(long long)*index));
            fprintf(fp, "%lld,", *inf1);
        }
        break;
        case DATA_TYPE_CMPLX_S:
        {
            f_cmplx * inf1 = (f_cmplx *)((char *)in1 + (sizeof(f_cmplx)*index));
            fprintf(fp, "%.10f,", sqrt(inf1->re*inf1->re + inf1->im*inf1->im));
        }
        break;
        case DATA_TYPE_CMPLX_L:
        {
            f_cmplx * inf1 = (f_cmplx *)((char *)in1 + (sizeof(f_cmplx)*index));
            fprintf(fp, "%.10f,", sqrt(inf1->re*inf1->re + inf1->im*inf1->im));
        }
        break;
        case DATA_TYPE_INV:
        {
            fprintf(fp, "-00,");
        }
        break;
        default:
            printf("Error: Invalid input datatype \n");
        }
    }


    switch (input_prec_tests[test_index].out_data)
    {
    case DATA_TYPE_DOUBLE:
	case DATA_TYPE_DOUBLE_Q31:
	case DATA_TYPE_DOUBLE_Q47:
	case DATA_TYPE_DOUBLE_Q63:
    {
        double * outf_d = (double *)((char *)out_dut + (sizeof(double)*index));
        double * outf_r = (double *)((char *)out_ref + (sizeof(double)*index));
        fprintf(fp, "%f,%f,%.10f\n", *outf_d, *outf_r, (*outf_d - *outf_r));
    }
    break;
    case DATA_TYPE_FLOAT:
    case DATA_TYPE_FLOAT_L:
    {
        float * outf_d = (float *)((char *)out_dut + (sizeof(float)*index));
        float * outf_r = (float *)((char *)out_ref + (sizeof(float)*index));
        fprintf(fp, "%f,%f,%.10f\n", *outf_d, *outf_r, (*outf_d - *outf_r));
    }
    break;
    case DATA_TYPE_LONG:
    {
        int * outf_d = (int *)((char *)out_dut + (sizeof(int)*index));
        int * outf_r = (int *)((char *)out_ref + (sizeof(int)*index));
        fprintf(fp, "%d, %d, %d\n", *outf_d, *outf_r, (*outf_d - *outf_r));
    }
    break;
    case DATA_TYPE_SHORT:
    {
        short * outf_d = (short *)((char *)out_dut + (sizeof(short)*index));
        short * outf_r = (short *)((char *)out_ref + (sizeof(short)*index));
        fprintf(fp, "%d, %d, %d\n", *outf_d, *outf_r, (*outf_d - *outf_r));
    }
    break;
    case DATA_TYPE_CMPLX_S:
    {
        f_cmplx * outf_d = (f_cmplx *)((char *)out_dut + (sizeof(f_cmplx)*index));
        f_cmplx * outf_r = (f_cmplx *)((char *)out_ref + (sizeof(f_cmplx)*index));
        fprintf(fp, "%f, %f, %.10f\n", sqrt(outf_d->re*outf_d->re + outf_d->im*outf_d->im), sqrt(outf_r->re*outf_r->re + outf_r->im*outf_r->im), sqrt(outf_d->re*outf_d->re + outf_d->im*outf_d->im) - sqrt(outf_r->re*outf_r->re + outf_r->im*outf_r->im));
    }
    break;
    case DATA_TYPE_CMPLX_L:
    {
        f_cmplx * outf_d = (f_cmplx *)((char *)out_dut + (sizeof(f_cmplx)*index));
        f_cmplx * outf_r = (f_cmplx *)((char *)out_ref + (sizeof(f_cmplx)*index));
        fprintf(fp, "%f, %f, %.10f\n", sqrt(outf_d->re*outf_d->re + outf_d->im*outf_d->im), sqrt(outf_r->re*outf_r->re + outf_r->im*outf_r->im), sqrt(outf_d->re*outf_d->re + outf_d->im*outf_d->im) - sqrt(outf_r->re*outf_r->re + outf_r->im*outf_r->im));
    }
    break;
    case DATA_TYPE_WORD64:
    {
        long long  * outf_d = (long long  *)((char *)out_dut + (sizeof(long long)*index));
        long long  * outf_r = (long long  *)((char *)out_ref + (sizeof(long long)*index));
        fprintf(fp, "%lld, %lld, %lld\n", *outf_d, *outf_r, (*outf_d - *outf_r));

    }
    break;
    default:
        printf("Error: Invalid output datatype \n");
    }

}

// The debug print for the input/putput types.
void print_debug(int test_index, FILE * fp, int index, int f_type1, char *fnm1, int f_type2, char *fnm2, void *in1_1, void *in2_1, void *in3_1, void *out_dut, void *out_ref, double err)
{
    int idx;

    fprintf(fp, "%s,%s,", fnm1, fnm2);

    for (idx = 0; idx < 3; idx++)
    {
        char * in1 = NULL;
        if (idx == 0)
        {
            in1 = in1_1;
        }
        else if (idx == 1)
        {
            in1 = in2_1;
        }
        else if (idx == 2)
        {
            in1 = in3_1;
        }
        switch (input_prec_tests[test_index].inp_data[idx])
        {
        case DATA_TYPE_DOUBLE:
		case DATA_TYPE_DOUBLE_Q31:
		case DATA_TYPE_DOUBLE_Q47:
		case DATA_TYPE_DOUBLE_Q63:
        {
            double * inf1 = (double *)((char *)in1 + (sizeof(double)*index));
            fprintf(fp, "%f,", *inf1);
        }
        break;
        case DATA_TYPE_FLOAT:
        {
            float * inf1 = (float *)((char *)in1 + (sizeof(float)*index));
            fprintf(fp, "%f,", *inf1);
        }
        break;
        case DATA_TYPE_FLOAT_L:
        {
            float * inf1 = (float *)((char *)in1 + (sizeof(float)*index));
            fprintf(fp, "%f,", *inf1);
        }
        break;
        case DATA_TYPE_LONG:
        {
            int * inf1 = (int *)((char *)in1 + (sizeof(int)*index));
            fprintf(fp, "%d,", *inf1);
        }
        break;
        case DATA_TYPE_SHORT:
        {
            short * inf1 = (short *)((char *)in1 + (sizeof(short)*index));
            fprintf(fp, "%d,", *inf1);
        }
        break;
        case DATA_TYPE_WORD64:
        {
            long long * inf1 = (long long *)((char *)in1 + (sizeof(long long)*index));
            fprintf(fp, "%lld,", *inf1);
        }
        break;
        case DATA_TYPE_CMPLX_S:
        {
            f_cmplx * inf1 = (f_cmplx *)((char *)in1 + (sizeof(long long)*index));
            fprintf(fp, "%f,", sqrt(inf1->re*inf1->re + inf1->im*inf1->im));
        }
        break;
        case DATA_TYPE_CMPLX_L:
        {
            f_cmplx * inf1 = (f_cmplx *)((char *)in1 + (sizeof(long long)*index));
            fprintf(fp, "%f,", sqrt(inf1->re*inf1->re + inf1->im*inf1->im));
        }
        break;
		default:
		{
		
		}
		break;
        }

    }


    switch (input_prec_tests[test_index].out_data)
    {
    case DATA_TYPE_DOUBLE:
	case DATA_TYPE_DOUBLE_Q31:
	case DATA_TYPE_DOUBLE_Q47:
	case DATA_TYPE_DOUBLE_Q63:
    {
        double * outf_d = (double *)((char *)out_dut + (sizeof(double)*index));
        double * outf_r = (double *)((char *)out_ref + (sizeof(double)*index));
        fprintf(fp, "%f,%f,%.10f\n", *outf_d, *outf_r, err);
    }
    break;
    case DATA_TYPE_FLOAT_L:
    case DATA_TYPE_FLOAT:
    {
        float * outf_d = (float *)((char *)out_dut + (sizeof(float)*index));
        float * outf_r = (float *)((char *)out_ref + (sizeof(float)*index));
        fprintf(fp, "%f,%f,%.10f\n", *outf_d, *outf_r, err);
    }
    break;
    case DATA_TYPE_LONG:
    {
        int * outf_d = (int *)((char *)out_dut + (sizeof(int)*index));
        int * outf_r = (int *)((char *)out_ref + (sizeof(int)*index));
        fprintf(fp, "%d, %d, %f\n", *outf_d, *outf_r, err);
    }
    break;
    case DATA_TYPE_SHORT:
    {
        short * outf_d = (short *)((char *)out_dut + (sizeof(short)*index));
        short * outf_r = (short *)((char *)out_ref + (sizeof(short)*index));
        fprintf(fp, "%d, %d, %f\n", *outf_d, *outf_r, err);
    }
    break;
    case DATA_TYPE_CMPLX_S:
    {
        f_cmplx * outf_d = (f_cmplx *)((char *)out_dut + (sizeof(f_cmplx)*index));
        f_cmplx * outf_r = (f_cmplx *)((char *)out_ref + (sizeof(f_cmplx)*index));
        fprintf(fp, "%f, %f, %.10f\n", outf_d->re, outf_r->re, err);
        fprintf(fp, "%f, %f, %.10f\n", outf_d->im, outf_r->im, err);
    }
    break;
    case DATA_TYPE_CMPLX_L:
    {
        f_cmplx * outf_d = (f_cmplx *)((char *)out_dut + (sizeof(f_cmplx)*index));
        f_cmplx * outf_r = (f_cmplx *)((char *)out_ref + (sizeof(f_cmplx)*index));
        fprintf(fp, "%f, %f, %.10f\n", outf_d->re, outf_r->re, err);
        fprintf(fp, "%f, %f, %.10f\n", outf_d->im, outf_r->im, err);				}
    break;
    case DATA_TYPE_WORD64:
    {
        long long  * outf_d = (long long  *)((char *)out_dut + (sizeof(long long)*index));
        long long  * outf_r = (long long  *)((char *)out_ref + (sizeof(long long)*index));
        fprintf(fp, "%lld, %lld, %f\n", *outf_d, *outf_r, err);
    }
    break;
    default:
    { 
    }	
    break;
    }
}

#define DOUBLE_ZERO_VAL 0.0

//  o/p difference calculation method for the particualr data type.
void compute_diff(int test_index, int f_type1, void * out_dut, void * out_ref, double * abs_diff, double * percent_err)
{
    float *outf_d = NULL, *outf_r = NULL;
    double *outd_d = NULL, *outd_r = NULL;
    f_cmplx *outfc_d = NULL, *outfc_r = NULL;
    int   *outi_d = NULL, *outi_r = NULL;

    int n;

    switch (input_prec_tests[test_index].out_data)
    {

    case DATA_TYPE_LONG:
    case DATA_TYPE_SHORT:
        outi_d = (int *)(out_dut);

        outi_r = (int *)(out_ref);

        for (n = 0; n < N_PREC; n++)
        {
            abs_diff[n] = ABS_VAL(((double)outi_r[n]) - ((double)outi_d[n]));

            if (outi_r[n] == DOUBLE_ZERO_VAL)
            {
                percent_err[n] = 0;
            }
            else
            {
                percent_err[n] = (abs_diff[n] * 100.0) / ((ABS_VAL((double)outi_r[n])));
            }
#ifdef DEBUG_LOG
            printf("ref=%f\tdut=%f\tabsdiff=%f\n", outf_r[n], outf_d[n], abs_diff[n]);
#endif
        }
        break;

        break;
    case DATA_TYPE_FLOAT_L:
    case DATA_TYPE_FLOAT:
        outf_d = (float *)(out_dut);

        outf_r = (float *)(out_ref);
        for (n = 0; n < N_PREC; n++)
        {
            abs_diff[n] = ABS_VAL(((double)outf_r[n]) - ((double)outf_d[n]));

            if (outf_r[n] == DOUBLE_ZERO_VAL)
            {
                percent_err[n] = 0;
            }
            else
            {
                percent_err[n] = (abs_diff[n] * 100.0) / ((ABS_VAL((double)outf_r[n])));
            }
#ifdef DEBUG_LOG
            printf("ref=%f\tdut=%f\tabsdiff=%f\n", outf_r[n], outf_d[n], abs_diff[n]);
#endif
        }
        break;

    case DATA_TYPE_DOUBLE:
	case DATA_TYPE_DOUBLE_Q31:
	case DATA_TYPE_DOUBLE_Q47:
	case DATA_TYPE_DOUBLE_Q63:
        outd_d = (double *)(out_dut);
        outd_r = (double *)(out_ref);
        for (n = 0; n < N_PREC; n++)
        {
            abs_diff[n] = ABS_VAL(((double)outd_r[n]) - ((double)outd_d[n]));
            if (outd_r[n] == DOUBLE_ZERO_VAL)
            {
                percent_err[n] = 0;
            }
            else
            {
                percent_err[n] = (abs_diff[n] * 100.0) / (ABS_VAL(outd_r[n]));
            }
#ifdef DEBUG_LOG
            printf("ref=%f\tdut=%f\tabsdiff=%f\n", outd_r[n], outd_d[n], abs_diff[n]);
#endif
        }
        break;

    case DATA_TYPE_CMPLX_S:
    case DATA_TYPE_CMPLX_L:
        outfc_d = (f_cmplx *)(out_dut);
        outfc_r = (f_cmplx *)(out_ref);
        for (n = 0; n < N_PREC; n++)
        {
            float diff_val = ((outfc_r[n].re - outfc_d[n].re)*(outfc_r[n].re - outfc_d[n].re)) + ((outfc_r[n].im - outfc_d[n].im)*(outfc_r[n].im - outfc_d[n].im));
            abs_diff[n] = sqrtf(diff_val);

            double ref_mag = sqrt(((double)outfc_r[n].re*outfc_r[n].re) + ((double)outfc_r[n].im*outfc_r[n].im));

            // TBD : need to review this..
            if (ref_mag == DOUBLE_ZERO_VAL)
            {
                percent_err[n] = 0;
            }
            else
            {
                percent_err[n] = ((abs_diff[n] * 100.0) / ref_mag);
            }

#ifdef DEBUG_LOG
            printf("ref=%f\tdut=%f\tabsdiff=%f\tpercerr=%f \n", outfc_r[n].re, outfc_d[n].re, abs_diff[n], percent_err[n]);
#endif
        }
        break;

    default:
        printf("ERROR-2 !!! Incorrect or unsupported DUT output type\n");
        exit(0);
    }
}

// Generate inputs as appropriate for the new function type.
#ifdef IDENTICAL_FLOAT_INP
void init_inputs(int test_index, int f_type1, int data_pat1, int data_pat2, int data_pat3, void * in1, void * in2, void *in3)
{
    srand(RAND_SEED_1);

    int data_pat[MAX_NUM_INP];
    void *in_ptr[MAX_NUM_INP];
    int i;

    data_pat[0] = data_pat1;
    data_pat[1] = data_pat2;
    data_pat[2] = data_pat3;
    in_ptr[0] = in1;
    in_ptr[1] = in2;
    in_ptr[2] = in3;

    // pass last argument of init functions as 1 for Q15
    // pass last argument of init functions as 0 for Q31

    for (i = 0; i < MAX_NUM_INP; i++)
    {
        switch (input_prec_tests[test_index].inp_data[i])
        {
        case DATA_TYPE_DOUBLE:
			initd(data_pat[i], (double *)in_ptr[i], N_PREC, 2);
			break;
		case DATA_TYPE_DOUBLE_Q31:
			initd(data_pat[i], (double *)in_ptr[i], N_PREC, 2);
			break;
		case DATA_TYPE_DOUBLE_Q47:
			initd(data_pat[i], (double *)in_ptr[i], N_PREC, 3);
			break;
		case DATA_TYPE_DOUBLE_Q63:
            initd(data_pat[i], (double *)in_ptr[i], N_PREC, 4);
            break;

        case DATA_TYPE_FLOAT_L:
            initf(data_pat[i], (float *)in_ptr[i], N_PREC, 0);
            break;
        case DATA_TYPE_FLOAT:
            initf(data_pat[i], (float *)in_ptr[i], N_PREC, 1);
            break;

        case DATA_TYPE_CMPLX_L:
            initfc(data_pat[i], (f_cmplx *)in_ptr[i], N_PREC, 0);
            break;

        case DATA_TYPE_CMPLX_S:
            initfc(data_pat[i], (f_cmplx *)in_ptr[i], N_PREC, 1);
            break;

        case DATA_TYPE_SHORT:
            initi16(data_pat[i], (short int *)in_ptr[i], N_PREC);
            break;
        case DATA_TYPE_LONG:
            initi32(data_pat[i], (int *)in_ptr[i], N_PREC);
            break;

        case DATA_TYPE_WORD64:
            initi64(data_pat[i], (long long *)in_ptr[i], N_PREC);
            break;
        case DATA_TYPE_INV:

            break;
        default:
            printf("ERROR-0 !!! Incorrect or unsupported function type\n");
            exit(0);
        }
    }
}
#else /* #ifdef IDENTICAL_FLOAT_INP */
void init_inputs(int test_index, int f_type1, int data_pat1, int data_pat2, int data_pat3, void * in1, void * in2, void *in3)
{
    srand(RAND_SEED_1);

    int data_pat[MAX_NUM_INP];
    void *in_ptr[MAX_NUM_INP];
    int i;

    data_pat[0] = data_pat1;
    data_pat[1] = data_pat2;
    data_pat[2] = data_pat3;
    in_ptr[0] = in1;
    in_ptr[1] = in2;
    in_ptr[2] = in3;

    for (i = 0; i < MAX_NUM_INP; i++)
    {
        switch (input_prec_tests[test_index].inp_data[i])
        {
        case DATA_TYPE_DOUBLE:
		case DATA_TYPE_DOUBLE_Q31:
		case DATA_TYPE_DOUBLE_Q47:
		case DATA_TYPE_DOUBLE_Q63:
            initd(data_pat[i], (double *)in_ptr[i], N_PREC);
            break;

        case DATA_TYPE_FLOAT_L:
        case DATA_TYPE_FLOAT:
            initf(data_pat[i], (float *)in_ptr[i], N_PREC);
            break;

        case DATA_TYPE_CMPLX_L:
        case DATA_TYPE_CMPLX_S:
            initfc(data_pat[i], (f_cmplx *)in_ptr[i], N_PREC);
            break;

        case DATA_TYPE_SHORT:
			initi16(data_pat[i], (short int *)in_ptr[i], N_PREC);
			break;
        case DATA_TYPE_LONG:
            initi32(data_pat[i], (int *)in_ptr[i], N_PREC);
            break;
		case DATA_TYPE_WORD64:
			initi64(data_pat[i], (long long *)in_ptr[i], N_PREC);
			break;

        case DATA_TYPE_INV:

            break;
        default:
            printf("ERROR-0 !!! Incorrect or unsupported function type\n");
            exit(0);
        }
    }
}
#endif /* #ifdef IDENTICAL_FLOAT_INP */

double BitsInErrFactor = 1.0;
#define BITS_IN_ERR_FACTOR_S    (32768.0)
#define BITS_IN_ERR_FACTOR_L    (2147483648.0)
#define BITS_IN_ERR_FACTOR_W64    (9223372036854775808.0)
#define BITS_IN_ERR_FACTOR_W48    (140737488355328.0)


// Modify_here  : Add execute macros for the new function type here.
void run_func(int test_index, int ftype, char *fnm1, char *fnm2, void *out_dut, void *out_ref, void *in1, void *in2, void *in3, int N_PREC_Val)
{
    int i;
    {
        switch (ftype)
        {


        case FUNC_TYPE_OL_IW32_IL:
        {
            {
                double * ind1 = (double *)in1;
                int * ind2 = (int *)in2;

                // Restrict the shift to 5 bits..
                for (i = 0; i < N_PREC_Val; i++)
                {
					ind2[i] = ((ind2[i] << 27) >> 27);
                }

                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OL_IW_IL dut_fptr = input_prec_tests[test_index].func_ptr_dut;
					FUNC_TEST_OL_IW32_IL_dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
					fptr_OD_ID_IL ref_fptr = input_prec_tests[test_index].func_ptr_ref;
					FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;

            }
        }
        break;

        case FUNC_TYPE_OS_IW48:
        {
            {
                double * ind1 = (double *)in1;
                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

					fptr_OS_IW dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OS_IW48_dut(dut_fptr, ind1, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_ID ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_1ISO_REF(ref_fptr, ind1, outd_r, N_PREC_Val);
                }
				BitsInErrFactor = BITS_IN_ERR_FACTOR_W48;

            }
        }
        break;
        case FUNC_TYPE_OL_IW64:
        {
            {
                double * ind1 = (double *)in1;
                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OL_IW dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OL_IW64_dut(dut_fptr, ind1, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_ID ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_1ISO_REF(ref_fptr, ind1, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_W64;

            }
        }
        break;

        case FUNC_TYPE_OL_IW48:
        {
            {
                double * ind1 = (double *)in1;
                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OL_IW dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OL_IW48_dut(dut_fptr, ind1, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_ID ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_1ISO_REF(ref_fptr, ind1, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_W48;

            }
        }
        break;

        case FUNC_TYPE_OL_IW32:
        {
            {
                double * ind1 = (double *)in1;
                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OL_IW dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OL_IW32_dut(dut_fptr, ind1, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_ID ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_1ISO_REF(ref_fptr, ind1, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;

            }
        }
        break;
        case FUNC_TYPE_OS_IW:
        {
            {
                double * ind1 = (double *)in1;

               {
                    // Execute DUT
                    {
                        short * outd_d = (short *)out_dut;

                        fptr_OL_IW dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                        FUNC_TEST_OS_IW_dut(dut_fptr, ind1, outd_d, N_PREC_Val);
                    }

                    // Execute REF
                    {
                        short * outd_r = (short *)out_ref;
                        fptr_OL_ID ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                        FUNC_TEST_1ISO_REF(ref_fptr, ind1, outd_r, N_PREC_Val);
                    }
                    BitsInErrFactor = 1;
                }

            }
        }
        break;
        case FUNC_TYPE_OL_IW:
        {
            {
                double * ind1 = (double *)in1;

               {
                    // Execute DUT
                    {
                        long * outd_d = (long *)out_dut;

                        fptr_OL_IW dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                        FUNC_TEST_OL_IW_dut(dut_fptr, ind1, outd_d, N_PREC_Val);
                    }

                    // Execute REF
                    {
                        long * outd_r = (long *)out_ref;
                        fptr_OL_ID ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                        FUNC_TEST_1ISO_REF(ref_fptr, ind1, outd_r, N_PREC_Val);
                    }
                    BitsInErrFactor = 1;
                }

            }
        }
        break;
        case FUNC_TYPE_OL_IL_IL_IS:
        {
            {
                float * ind1 = (float *)in1;
                float * ind2 = (float *)in2;
                float * ind3 = (float *)in3;

                // Execute DUT
                {
                    float * outd_d = (float *)out_dut;

                    fptr_OL_IL_IL_IS dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_3ISO_enh32_Dut(dut_fptr, ind1, ind2, ind3, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    float * outd_r = (float *)out_ref;
                    fptr_OF_IF_IF_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_3ISO_REF(ref_fptr, ind1, ind2, ind3, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;

        case FUNC_TYPE_OL_IL_IS:
        {
            {
                float * ind1 = (float *)in1;
                float * ind2 = (float *)in2;

                // Execute DUT
                {
                    float * outd_d = (float *)out_dut;

                    fptr_OL_IL_IS dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_2ISO_enh32_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    float * outd_r = (float *)out_ref;
                    fptr_OF_IF_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;

        case FUNC_TYPE_OL_IL_IL_IL:
        {
            {
                float * ind1 = (float *)in1;
                float * ind2 = (float *)in2;
                float * ind3 = (float *)in3;

                // Execute DUT
                {
                    float * outd_d = (float *)out_dut;

                    fptr_OL_IL_IL_IL dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_3ISO_enh32_32_Dut(dut_fptr, ind1, ind2, ind3, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    float * outd_r = (float *)out_ref;
                    fptr_OF_IF_IF_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_3ISO_REF(ref_fptr, ind1, ind2, ind3, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;

        case FUNC_TYPE_OL_IL_IL:
        {
            {
                float * ind1 = (float *)in1;
                float * ind2 = (float *)in2;

                // Execute DUT
                {
                    float * outd_d = (float *)out_dut;

                    fptr_OL_IL_IL dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_2ISO_enh32_32_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    float * outd_r = (float *)out_ref;
                    fptr_OF_IF_IF  ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;

        case FUNC_TYPE_OCL_ICL_ICL:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;
                f_cmplx * ind2 = (f_cmplx *)in2;

                // Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;

                    fptr_OCL_ICL_ICL dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OCL_ICL_ICL_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC_IFC ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;

        case FUNC_TYPE_OCL_ICL:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;

                // Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;

                    fptr_OCL_ICL dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OCL_ICL_Dut(dut_fptr, ind1, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_1ISO_REF(ref_fptr, ind1, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;
        case FUNC_TYPE_OCL_ICL_IC:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;
                f_cmplx * ind2 = (f_cmplx *)in2;

                // Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;
                    fptr_OCL_ICL_IC dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OCL_ICL_IC_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC_IFC ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;


        case FUNC_TYPE_OCL_ICL_IL0:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;
                int * ind2 = (int *)in2;

                // Restrict the shift to 5 bits..
                for (i = 0; i < N_PREC_Val; i++)
                {
                    ind2[i] = ((ind2[i] << 27) >> 27);
                }

                // Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;
                    fptr_OCL_ICL_IL dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OCL_ICL_IL0_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC_IL0 ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;

        case FUNC_TYPE_OC_IC_IS0:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;
                short int * ind2 = (short int *)in2;


				// Restrict the shift to 4 bits..
				for (i = 0; i < N_PREC_Val; i++)
				{
					ind2[i] = ((short int)(ind2[i] << 12) >> 12);
				}
				
				// Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;
                    fptr_OC_IC_IS dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OC_IC_IS0_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC_IS0 ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_S;
            }
        }
        break;

        case FUNC_TYPE_OCL_ICL_IL:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;
                float * ind2 = (float *)in2;

                // Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;
                    fptr_OCL_ICL_IL dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OCL_ICL_IL_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;

        case FUNC_TYPE_OCL_ICL_IS:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;
                float * ind2 = (float *)in2;

                // Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;
                    fptr_OCL_ICL_IS dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OCL_ICL_IS_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;

        case FUNC_TYPE_OCL_ICL_IS_IS:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;
                float * ind2 = (float *)in2;
                float * ind3 = (float *)in3;

                // Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;
                    fptr_OCL_ICL_IS_IS dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OCL_ICL_IS_IS_Dut(dut_fptr, ind1, ind2, ind3, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC_IF_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_3ISO_REF(ref_fptr, ind1, ind2, ind3, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;

        case FUNC_TYPE_OCL_ICL_IL_IL:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;
                float * ind2 = (float *)in2;
                float * ind3 = (float *)in3;

                // Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;
                    fptr_OCL_ICL_IL_IL dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OCL_ICL_IL_IL_Dut(dut_fptr, ind1, ind2, ind3, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC_IF_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_3ISO_REF(ref_fptr, ind1, ind2, ind3, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;

		case FUNC_TYPE_OC_ICL:
		{
			{
				f_cmplx * ind1 = (f_cmplx *)in1;

				// Execute DUT
				{
					f_cmplx * outd_d = (f_cmplx *)out_dut;
					fptr_OC_ICL dut_fptr = input_prec_tests[test_index].func_ptr_dut;
					FUNC_TEST_OC_ICL_Dut(dut_fptr, ind1, outd_d, N_PREC);
				}

				// Execute REF
				{
					f_cmplx * outd_r = (f_cmplx *)out_ref;
					fptr_OFC_IFC ref_fptr = input_prec_tests[test_index].func_ptr_ref;
					FUNC_TEST_1ISO_REF(ref_fptr, ind1, outd_r, N_PREC);
				}
				BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
			}
		}
		break;

        case FUNC_TYPE_OC_IC_IC:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;
                f_cmplx * ind2 = (f_cmplx *)in2;

                // Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;
                    fptr_OC_IC_IC dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OC_IC_IC_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC_IFC ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_S;
            }
        }
        break;

        case FUNC_TYPE_OC_IC:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;

                // Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;
                    fptr_OC_IC dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OC_IC_Dut(dut_fptr, ind1, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_1ISO_REF(ref_fptr, ind1, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_S;
            }
        }
        break;
        case FUNC_TYPE_OCL_IC_IS:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;
                float * ind2 = (float *)in2;

                // Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;
                    fptr_OCL_IC_IS dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OCL_IC_IS_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;

        case FUNC_TYPE_OC_ICL_IC_IS:
        {
            {
                f_cmplx * ind1 = (f_cmplx *)in1;
                f_cmplx * ind2 = (f_cmplx *)in2;
                float * ind3 = (float *)in3;

                // Execute DUT
                {
                    f_cmplx * outd_d = (f_cmplx *)out_dut;
                    fptr_OC_ICL_IC_IS dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OC_ICL_IC_IS_Dut(dut_fptr, ind1, ind2, ind3, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    f_cmplx * outd_r = (f_cmplx *)out_ref;
                    fptr_OFC_IFC_IFC_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_3ISO_REF(ref_fptr, ind1, ind2, ind3, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_S;
            }
        }
        break;



        case FUNC_TYPE_OW_IS_IS:
        {
            {
                float * ind1 = (float *)in1;
                float * ind2 = (float *)in2;


                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OW_IS_IS dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OW_IS_IS_dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_IF_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;
        case FUNC_TYPE_OW_IW_IS_IS:
        {
            {
                double * ind1 = (double *)in1;
                float * ind2 = (float *)in2;
                float * ind3 = (float *)in3;

                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OW_IW_IS_IS_0 dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_3ISO_Dut(dut_fptr, ind1, ind2, ind3, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_ID_IF_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_3ISO_REF(ref_fptr, ind1, ind2, ind3, outd_r, N_PREC_Val);
                }
                BitsInErrFactor = BITS_IN_ERR_FACTOR_L;
            }
        }
        break;


        case FUNC_TYPE_OW_IS_IS_0:
        {
            {
				short int * ind1 = (short  int *)in1;
				short int * ind2 = (short  int *)in2;

                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OW_IS_IS_0 dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OW_IS_IS_0_dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_IF_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
            }
            BitsInErrFactor = 1;
        }
        break;

        case FUNC_TYPE_OW_IW_IW:
        {
            {
                double * ind1 = (double *)in1;
                double * ind2 = (double *)in2;

                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OW_IW_IW dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OD_ID_ID_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_ID_ID ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
            }
            BitsInErrFactor = BITS_IN_ERR_FACTOR_W64;
        }
        break;

        case FUNC_TYPE_OW_IL_IS:
        {
            {
                float * ind1 = (float *)in1;
                float * ind2 = (float *)in2;

                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OW_IL_IS dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OW_IL_IS_dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_IF_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
            }
            BitsInErrFactor = BITS_IN_ERR_FACTOR_W48;
        }
        break;
        case FUNC_TYPE_OW_IW:
        {
            {
                double * ind1 = (double *)in1;

                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OW_IW dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OW_IW_dut(dut_fptr, ind1, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_ID ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_1ISO_REF(ref_fptr, ind1, outd_r, N_PREC_Val);
                }
            }
            BitsInErrFactor = BITS_IN_ERR_FACTOR_W64;
        }
        break;


        case FUNC_TYPE_OW_IL_IL:
        {
            {
                float * ind1 = (float *)in1;
                float * ind2 = (float *)in2;

                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OW_IL_IL dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OD_IFL_IFL_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_IF_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }
            }
            BitsInErrFactor = BITS_IN_ERR_FACTOR_W64;
        }
        break;


        case FUNC_TYPE_OW_IL_IL_0:
        {
            {
                int * ind1 = (int *)in1;
                int * ind2 = (int *)in2;

                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OW_IL_IL dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OD_IFL_IFL_0_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_ID_ID ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }

                BitsInErrFactor = 1;
            }
        }
        break;

        case FUNC_TYPE_OW_IW_IS_IS_0:
        {
            {
				long long int * ind1 = (long long int *)in1;
				short int * ind2 = (short int *)in2;
				short int * ind3 = (short int *)in3;

				// Shifted to match presion available with mantissa of double datatype in reference code.
				for (i = 0; i < N_PREC; i++)
				{
					ind1[i] = ind1[i] >> 11;
				}

                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;
                    fptr_OW_IW_IS_IS_0 dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OD_ID_IF_IF_Dut(dut_fptr, ind1, ind2, ind3, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_ID_IF_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_3ISO_INT_REF(ref_fptr, ind1, ind2, ind3, outd_r, N_PREC_Val);
                }

                BitsInErrFactor = 1;
            }
        }
        break;


        case FUNC_TYPE_OW_IW_IL_IS:
        {
            {
                double * ind1 = (double *)in1;
                float * ind2 = (float *)in2;
                float * ind3 = (float *)in3;

                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OW_IW_IL_IS dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OD_ID_IFL_IF_Dut(dut_fptr, ind1, ind2, ind3, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_ID_IF_IF ref_fptr = input_prec_tests[test_index].func_ptr_ref;
                    FUNC_TEST_3ISO_REF(ref_fptr, ind1, ind2, ind3, outd_r, N_PREC_Val);
                }

                BitsInErrFactor = BITS_IN_ERR_FACTOR_W48;
            }
        }
        break;

 case FUNC_TYPE_OW_IW64_IL:
        {
            {
                Word64 * ind1 = (Word64 *)in1;
                short * ind2 = (short *)in2;

                // Restrict the shift to 5 bits..
                for (i = 0; i < N_PREC_Val; i++)
                {
					ind1[i] = ind1[i] >> 32;
                    ind2[i] = ((short int)(ind2[i] << 10) >> 10);
                }

                // Execute DUT
                {
                    double * outd_d = (double *)out_dut;

                    fptr_OW_IW_IS dut_fptr = input_prec_tests[test_index].func_ptr_dut;
                    FUNC_TEST_OD_ID64_IS_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
                }

                // Execute REF
                {
                    double * outd_r = (double *)out_ref;
                    fptr_OD_ID_IS ref_fptr = input_prec_tests[test_index].func_ptr_ref;

                    FUNC_TEST_2ISO_SHIFT_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
                }

                BitsInErrFactor = 1.0;
            }
        }
        break;

		case FUNC_TYPE_OW_IW_IS:
		{
			{
				double * ind1 = (double *)in1;
				short int * ind2 = (short int *)in2;

				// Restrict the shift to 6 bits..
				for (i = 0; i < N_PREC_Val; i++)
				{
					ind2[i] = ((short int)(ind2[i] << 10) >> 10);
				}

				// Execute DUT
				{
					double * outd_d = (double *)out_dut;

					fptr_OW_IW_IS dut_fptr = input_prec_tests[test_index].func_ptr_dut;
					FUNC_TEST_OD_ID_IS_Dut(dut_fptr, ind1, ind2, outd_d, N_PREC_Val);
				}

				// Execute REF
				{
					double * outd_r = (double *)out_ref;
					fptr_OD_ID_IS ref_fptr = input_prec_tests[test_index].func_ptr_ref;


					FUNC_TEST_2ISO_REF(ref_fptr, ind1, ind2, outd_r, N_PREC_Val);
				}

				BitsInErrFactor = BITS_IN_ERR_FACTOR_L; /* input/Output are in Q31 format*/
			}
		}
		break;


        default:
            printf("ERROR-1 !!! Incorrect or unsupported function type\n");
            exit(0);
        }
    }

}

//  main precision function : modify if required..
void execute_each_prec_test(int test_index, char *fnm1, int f_type, char *fnm2, int data_pat1, int data_pat2, int data_pat3,int type)
{
    double avg_bits_in_err, max_bits_in_err, abs_val;
    void * in1 = (void *)malloc((sizeof(double))*(N_PREC));
    void * in2 = (void *)malloc((sizeof(double))*(N_PREC));
    void * in3 = (void *)malloc((sizeof(double))*(N_PREC));
    void * out_dut = (void *)malloc((sizeof(double))*(N_PREC));
    void * out_ref = (void *)malloc((sizeof(double))*(N_PREC));

    if ((in1 == NULL) || (in2 == NULL) || (in3 == NULL) || (out_dut == NULL) || (out_ref == NULL)) {
        printf("ERROR!!! Memory not allocated\n");
        return;
    }
    //TODO: check if enough memory is available

    char pat1[100];
    char pat2[100];
    char pat3[100];
    get_data_pat(data_pat1, pat1);
    get_data_pat(data_pat2, pat2);
    get_data_pat(data_pat3, pat3);

    init_inputs(test_index, f_type, data_pat1, data_pat2, data_pat3, in1, in2, in3);

    run_func(test_index, f_type, fnm1, fnm2, out_dut, out_ref, in1, in2, in3, N_PREC);

    double abs_diff[N_PREC], percent_err[N_PREC];

    compute_diff(test_index, f_type, out_dut, out_ref, abs_diff, percent_err);

    int n, err_cnt = 0, index_max = 0, index_min = 0;
    double max_abs_err = 0.0, min_abs_err = 0.0;
    double sum = 0.0;
    double avg;
    for (n = 0; n < N_PREC; n++) {
        if (abs_diff[n] > max_abs_err) {
            max_abs_err = abs_diff[n];
            index_max = n;
        }
        if (abs_diff[n] < min_abs_err) {
            min_abs_err = abs_diff[n];
            index_min = n;
        }
        sum += abs_diff[n];
    }
    avg = (sum / ((double)N_PREC));

    if (avg < 0.0)
        abs_val = -avg;
    else
        abs_val = avg;

    abs_val = fabs(abs_val * BitsInErrFactor);
    if (abs_val >= 0.5)
    {

        avg_bits_in_err = log2(abs_val) + 1;
    }
    else
    {
        avg_bits_in_err = 0;
    }

    if (max_abs_err < 0.0)
        abs_val = -max_abs_err;
    else
        abs_val = max_abs_err;

    abs_val =fabs(abs_val * BitsInErrFactor);

    if (abs_val >= 0.5)
    {
        max_bits_in_err = log2(abs_val) + 1;
    }
    else
    {
        max_bits_in_err = 0;
    }


    printf(" :: Bits in error : Avg =%f  : Max =%f \n", avg_bits_in_err, max_bits_in_err);
#ifdef DEBUG_LOG
    printf("sum = %f\t%x\tavg = %20.20f\t%x\n", sum, *((unsigned *)&sum), avg, *((unsigned *)&avg));
#endif

    int  index_max_p = 0, index_min_p = 0;
    double max_per_err = 0.0, min_per_err = 0.0;
    double sum_p = 0.0;
    double avg_p;
    for (n = 0; n < N_PREC; n++) {
        if (percent_err[n] > max_per_err) {
            max_per_err = percent_err[n];
            index_max_p = n;
        }
        if (percent_err[n] < min_per_err) {
            min_per_err = percent_err[n];
            index_min_p = n;
        }
        sum_p += percent_err[n];
    }
    avg_p = (sum_p / ((double)N_PREC));

    fprintf(fp_prec, "%s,%s,%s,%s,%s,%f,%f,%f, %d, %d\n", fnm1, fnm2, pat1, pat2, pat3, max_abs_err, min_abs_err, avg, (int)fabs(max_bits_in_err), (int)fabs(avg_bits_in_err));

    fprintf(fp_prec_debug, "%s,%s,%s, ,%s\n", fnm1, fnm2, pat1, pat2);
    for (n = 0; n < N_PREC; n++) {
        if (percent_err[n] > MAX_THRESHOLD)
        {
            err_cnt++;
            print_debug(test_index, fp_prec_debug, n, f_type, fnm1, f_type, fnm2, (void *)in1, (void *)in2, (void *)in3, (void *)out_dut, (void *)out_ref, percent_err[n]);
        }
    }
    for (n = 0; n < N_PREC; n++) {
      
        print_io_plot(test_index, fp_ip_op_plot, n, f_type, fnm1, f_type, fnm2, (void *)in1, (void *)in2, (void *)in3, (void *)out_dut, (void *)out_ref);

        print_test_out(test_index, fp_test_baseop[test_index][type], n, f_type, fnm1, f_type, fnm2,(void *)out_dut);    //  (void *)in1, (void *)in2, (void *)in3, (void *)out_ref, 
    }

    fprintf(fp_prec_debug, "Number of testcases exceeding threshold err = %d\n", err_cnt);


    free(in1);
    free(in2);
    free(in3);
    free(out_dut);
    free(out_ref);
}

// Change this if you want to add any special value
#define MAX_NUM_SPL_VALS 3
double spl_values_d[MAX_NUM_SPL_VALS] = {1.0, -1.0 , 0.0};
float spl_values_f[MAX_NUM_SPL_VALS] = { 1.0f, -1.0f , 0.0 };
short spl_values_s[MAX_NUM_SPL_VALS] = { 0x7fff, 0x8000 , 0 };
int spl_values_i[MAX_NUM_SPL_VALS] = { 0x7fffffff, 0x80000000 , 0};
Word64 spl_values_w[MAX_NUM_SPL_VALS] = { 0x7fffffffffffffff, 0x8000000000000000 , 0 };

void init_spl_inputs(int test_index, void * in1, void * in2, void *in3)
{
    void *in_ptr[MAX_NUM_INP];
    int i, j;

    in_ptr[0] = in1;
    in_ptr[1] = in2;
    in_ptr[2] = in3;

    // pass last argument of init functions as 1 for Q15
    // pass last argument of init functions as 0 for Q31

    for (i = 0; i < MAX_NUM_INP; i++)
    {
        switch (input_prec_tests[test_index].inp_data[i])
        {
        case DATA_TYPE_DOUBLE:
        case DATA_TYPE_DOUBLE_Q31:
        case DATA_TYPE_DOUBLE_Q47:
        case DATA_TYPE_DOUBLE_Q63:
        {
            double *in = (double*)in_ptr[i];
            for (j = 0; j < MAX_NUM_SPL_VALS; j++)
            {
                in[j] = spl_values_d[j];
            }
        }
            break;

        case DATA_TYPE_FLOAT_L:
        {
            float *in = (float*)in_ptr[i];
            for (j = 0; j < MAX_NUM_SPL_VALS; j++)
            {
                in[j] = spl_values_f[j];
            }
        }
            break;
        case DATA_TYPE_FLOAT:
        {
            float *in = (float*)in_ptr[i];
            for (j = 0; j < MAX_NUM_SPL_VALS; j++)
            {
                in[j] = spl_values_f[j];
            }
        }
            break;

        case DATA_TYPE_CMPLX_L:
        {
            f_cmplx *in = (f_cmplx*)in_ptr[i];
            for (j = 0; j < MAX_NUM_SPL_VALS; j++)
            {
                in[j].im = spl_values_f[j];
                in[j].re = spl_values_f[j];
            }
        }
            break;

        case DATA_TYPE_CMPLX_S:
        {
            f_cmplx *in = (f_cmplx*)in_ptr[i];
            for (j = 0; j < MAX_NUM_SPL_VALS; j++)
            {
                in[j].im = spl_values_f[j];
                in[j].re = spl_values_f[j];
            }
        }
            break;

        case DATA_TYPE_SHORT:
        {
            short *in = (short*)in_ptr[i];
            for (j = 0; j < MAX_NUM_SPL_VALS; j++)
            {
                in[j] = spl_values_s[j];
            }
        }
            break;
        case DATA_TYPE_LONG:
        {
            int *in = (int*)in_ptr[i];
            for (j = 0; j < MAX_NUM_SPL_VALS; j++)
            {
                in[j] = spl_values_i[j];
            }
        }
            break;

        case DATA_TYPE_WORD64:
        {
            Word64 *in = (Word64*)in_ptr[i];
            for (j = 0; j < MAX_NUM_SPL_VALS; j++)
            {
                in[j] = spl_values_w[j];
            }
        }
            break;
        case DATA_TYPE_INV:

            break;
        default:
            printf("ERROR-0 !!! Incorrect or unsupported function type\n");
            exit(0);
        }
    }
}

int test_precision(void)
{
    srand(RAND_SEED_1);
    int retval = 1;
    int i;
    char fname[100];

    char *test_files[TOTAL_NUM_FUNC][NUM_TEST_PATTERN_1] = {
      {"./W_mult_16_16_random.csv", "./W_mult_16_16_sweep.csv", "./W_mult_16_16_sweep_piecewise.csv","./W_mult_16_16_custom.csv"},
      {"./W_mac_16_16_random.csv", "./W_mac_16_16_sweep.csv","./W_mac_16_16_sweep_piecewise.csv","./W_mac_16_16_custom.csv"},
      {"./W_msu_16_16_random.csv", "./W_msu_16_16_sweep.csv","./W_msu_16_16_sweep_piecewise.csv","./W_msu_16_16_custom.csv"},
      {"./W_add_random.csv", "./W_add_sweep.csv","./W_add_sweep_piecewise.csv","./W_add_custom.csv"},
      {"./W_sub_random.csv", "./W_sub_sweep.csv","./W_sub_sweep_piecewise.csv","./W_sub_custom.csv"},
      {"./W_add_nosat_random.csv", "./W_add_nosat_sweep.csv","./W_add_nosat_sweep_piecewise.csv","./W_add_nosat_custom.csv"},
      {"./W_sub_nosat_random.csv", "./W_sub_nosat_sweep.csv","./W_sub_nosat_sweep_piecewise.csv","./W_sub_nosat_custom.csv"},
      {"./W_shl_random.csv", "./W_shl_sweep.csv","./W_shl_sweep_piecewise.csv","./W_shl_custom.csv"},
      {"./W_shr_random.csv", "./W_shr_sweep.csv","./W_shr_sweep_piecewise.csv","./W_shr_custom.csv"},
      {"./W_shl_nosat_random.csv", "./W_shl_nosat_sweep.csv","./W_shl_nosat_sweep_piecewise.csv","./W_shl_nosat_custom.csv"},
      {"./W_shr_nosat_random.csv", "./W_shr_nosat_sweep.csv","./W_shr_nosat_sweep_piecewise.csv","./W_shr_nosat_custom.csv"},
      {"./W_lshl_random.csv", "./W_lshl_sweep.csv","./W_lshl_sweep_piecewise.csv","./W_lshl_custom.csv"},
      {"./W_lshr_random.csv", "./W_lshr_sweep.csv","./W_lshr_sweep_piecewise.csv","./W_lshr_custom.csv"},
      {"./W_shl_sat_l_random.csv", "./W_shl_sat_l_sweep.csv","./W_shl_sat_l_sweep_piecewise.csv","./W_shl_sat_l_custom.csv"},
      {"./W_sat_l_random.csv", "./W_sat_l_sweep.csv","./W_sat_l_sweep_piecewise.csv","./W_sat_l_custom.csv"},
      {"./W_sat_m_random.csv", "./W_sat_m_sweep.csv","./W_sat_m_sweep_piecewise.csv","./W_sat_m_custom.csv"},
      {"./W_round48_L_random.csv", "./W_round48_L_sweep.csv","./W_round48_L_sweep_piecewise.csv","./W_round48_L_custom.csv"},
      {"./W_round64_L_random.csv", "./W_round64_L_sweep.csv","./W_round64_L_sweep_piecewise.csv","./W_round64_L_custom.csv"},
      {"./W_round32_s_random.csv", "./W_round32_s_sweep.csv","./W_round32_s_sweep_piecewise.csv","./W_round32_s_custom.csv"},
      {"./W_norm_random.csv", "./W_norm_sweep.csv","./W_norm_sweep_piecewise.csv","./W_norm_custom.csv"},
      {"./W_mult0_16_16_random.csv", "./W_mult0_16_16_sweep.csv","./W_mult0_16_16_sweep_piecewise.csv","./W_mult0_16_16_custom.csv"},
      {"./W_mac0_16_16_random.csv", "./W_mac0_16_16_sweep.csv","./W_mac0_16_16_sweep_piecewise.csv","./W_mac0_16_16_custom.csv"},
      {"./W_msu0_16_16_random.csv", "./W_msu0_16_16_sweep.csv","./W_msu0_16_16_sweep_piecewise.csv","./W_msu0_16_16_custom.csv"},
      {"./W_mac_32_16_random.csv", "./W_mac_32_16_sweep.csv","./W_mac_32_16_sweep_piecewise.csv","./W_mac_32_16_custom.csv"},
      {"./W_msu_32_16_random.csv", "./W_msu_32_16_sweep.csv","./W_msu_32_16_sweep_piecewise.csv","./W_msu_32_16_custom.csv"},
      {"./W_mult_32_16_random.csv", "./W_mult_32_16_sweep.csv","./W_mult_32_16_sweep_piecewise.csv","./W_mult_32_16_custom.csv"},
      {"./W_mult_32_32_random.csv", "./W_mult_32_32_sweep.csv","./W_mult_32_32_sweep_piecewise.csv","./W_mult_32_32_custom.csv"},
      {"./W_mult0_32_32_random.csv", "./W_mult0_32_32_sweep.csv","./W_mult0_32_32_sweep_piecewise.csv","./W_mult0_32_32_custom.csv"},
      {"./W_neg_random.csv", "./W_neg_sweep.csv","./W_neg_sweep_piecewise.csv","./W_neg_custom.csv"},
      {"./W_abs_random.csv", "./W_abs_sweep.csv","./W_abs_sweep_piecewise.csv","./W_abs_custom.csv"},
      {"./Madd_32_16_random.csv", "./Madd_32_16_sweep.csv","./Madd_32_16_sweep_piecewise.csv","./Madd_32_16_custom.csv"},
      {"./Madd_32_32_random.csv", "./Madd_32_32_sweep.csv","./Madd_32_32_sweep_piecewise.csv","./Madd_32_32_custom.csv"},
      {"./Madd_32_16_r_random.csv", "./Madd_32_16_r_sweep.csv","./Madd_32_16_r_sweep_piecewise.csv","./Madd_32_16_r_custom.csv"},
      {"./Madd_32_32_r_random.csv", "./Madd_32_32_r_sweep.csv","./Madd_32_32_r_sweep_piecewise.csv","./Madd_32_32_r_custom.csv"},
      {"./Mpy_32_16_1_random.csv", "./Mpy_32_16_1_sweep.csv","./Mpy_32_16_1_sweep_piecewise.csv","./Mpy_32_16_1_custom.csv"},
      {"./Mpy_32_32_random.csv", "./Mpy_32_32_sweep.csv","./Mpy_32_32_sweep_piecewise.csv","./Mpy_32_32_custom.csv"},
      {"./Mpy_32_32_r_random.csv", "./Mpy_32_32_r_sweep.csv","./Mpy_32_32_r_sweep_piecewise.csv","./Mpy_32_32_r_custom.csv"},
      {"./Mpy_32_16_r_random.csv", "./Mpy_32_16_r_sweep.csv","./Mpy_32_16_r_sweep_piecewise.csv","./Mpy_32_16_r_custom.csv"},
      {"./Msub_32_16_random.csv", "./Msub_32_16_sweep.csv","./Msub_32_16_sweep_piecewise.csv","./Msub_32_16_custom.csv"},
      {"./Msub_32_16_r_random.csv", "./Msub_32_16_r_sweep.csv","./Msub_32_16_r_sweep_piecewise.csv","./Msub_32_16_r_custom.csv"},
      {"./Msub_32_32_random.csv", "./Msub_32_32_sweep.csv","./Msub_32_32_sweep_piecewise.csv","./Msub_32_32_custom.csv"},
      {"./Msub_32_32_r_random.csv", "./Msub_32_32_r_sweep.csv","./Msub_32_32_r_sweep_piecewise.csv","./Msub_32_32_r_custom.csv"},
      {"./CL_add_random.csv", "./CL_add_sweep.csv","./CL_add_sweep_piecewise.csv","./CL_add_custom.csv"},
      {"./CL_sub_random.csv", "./CL_sub_sweep.csv","./CL_sub_sweep_piecewise.csv","./CL_sub_custom.csv"},
      {"./CL_msu_j_random.csv", "./CL_msu_j_sweep.csv","./CL_msu_j_sweep_piecewise.csv","./CL_msu_j_custom.csv"},
      {"./CL_mac_j_random.csv", "./CL_mac_j_sweep.csv","./CL_mac_j_sweep_piecewise.csv","./CL_mac_j_custom.csv"},
      {"./CL_multr_32x32_random.csv", "./CL_multr_32x32_sweep.csv","./CL_multr_32x32_sweep_piecewise.csv","./CL_multr_32x32_custom.csv"},
      {"./CL_multr_32x16_random.csv", "./CL_multr_32x16_sweep.csv","./CL_multr_32x16_sweep_piecewise.csv","./CL_multr_32x16_custom.csv"},
      {"./C_add_random.csv", "./C_add_sweep.csv","./C_add_sweep_piecewise.csv","./C_add_custom.csv"},
      {"./C_sub_random.csv", "./C_sub_sweep.csv","./C_sub_sweep_piecewise.csv","./C_sub_custom.csv"},
      {"./C_multr_random.csv", "./C_multr_sweep.csv","./C_multr_sweep_piecewise.csv","./C_multr_custom.csv"},
      {"./C_scale_random.csv", "./C_scale_sweep.csv","./C_scale_sweep_piecewise.csv","./C_scale_custom.csv"},
      {"./CL_negate_random.csv", "./CL_negate_sweep.csv","./CL_negate_sweep_piecewise.csv","./CL_negate_custom.csv"},
      {"./CL_mul_j_random.csv", "./CL_mul_j_sweep.csv","./CL_mul_j_sweep_piecewise.csv","./CL_mul_j_custom.csv"},
      {"./C_negate_random.csv", "./C_negate_sweep.csv","./C_negate_sweep_piecewise.csv","./C_negate_custom.csv"},
      {"./C_mul_j_random.csv", "./C_mul_j_sweep.csv","./C_mul_j_sweep_piecewise.csv","./C_mul_j_custom.csv"},
      {"./C_mac_r_random.csv", "./C_mac_r_sweep.csv","./C_mac_r_sweep_piecewise.csv","./C_mac_r_custom.csv"},
      {"./C_msu_r_random.csv", "./C_msu_r_sweep.csv","./C_msu_r_sweep_piecewise.csv","./C_msu_r_custom.csv"},
      {"./CL_shr_random.csv", "./CL_shr_sweep.csv","./CL_shr_sweep_piecewise.csv","./CL_shr_custom.csv"},
      {"./CL_shl_random.csv", "./CL_shl_sweep.csv","./CL_shl_sweep_piecewise.csv","./CL_shl_custom.csv"},
      {"./C_shr_random.csv", "./C_shr_sweep.csv","./C_shr_sweep_piecewise.csv","./C_shr_custom.csv"},
      {"./C_shl_random.csv", "./C_shl_sweep.csv","./C_shl_sweep_piecewise.csv","./C_shl_custom.csv"},
      {"./CL_scale_32_random.csv", "./CL_scale_32_sweep.csv","./CL_scale_32_sweep_piecewise.csv","./CL_scale_32_custom.csv"},
      {"./CL_scale_random.csv", "./CL_scale_sweep.csv","./CL_scale_sweep_piecewise.csv","./CL_scale_custom.csv"},
      {"./CL_dscale_random.csv", "./CL_dscale_sweep.csv","./CL_dscale_sweep_piecewise.csv","./CL_dscale_custom.csv"},
      {"./CL_dscale_32_random.csv", "./CL_dscale_32_sweep.csv","./CL_dscale_32_sweep_piecewise.csv","./CL_dscale_32_custom.csv"},
      {"./CL_round32_16_random.csv", "./CL_round32_16_sweep.csv","./CL_round32_16_sweep_piecewise.csv","./CL_round32_16_custom.csv"}
    };

    fp_prec = fopen("precision_abs_err_report.csv", "w");
    if (fp_prec == NULL) {
        printf("Could not open precision_abs_err_report.csv\n");
        return retval;
    }
    fprintf(fp_prec, "DUT_FUNC,REF_FUNC,INPUT_PAT_1,INPUT_PAT_2,INPUT_PAT_3,MAX_ABS_ERR,MIN_ABS_ERR,AVG_ABS_ERR,MAX_BITS_ERR,AVG_BITS_ERR\n\n");

    fp_spcl = fopen("special_values_abs_err_report.csv", "w");
    if (fp_spcl == NULL) {
        printf("Could not open special_values_abs_err_report.csv\n");
        return retval;
    }

    fprintf(fp_spcl, "input1, input2, input3, out_dut, out_ref, error (out_dut-out_ref)\n\n");

    

    fp_prec_debug = fopen("precision_abs_err_debug_report.csv", "w");
    if (fp_prec_debug == NULL) {
        printf("Could not open precision_abs_err_debug_report.csv\n");
        return retval;
    }
    fprintf(fp_prec_debug, "DUT_FUNC,REF_FUNC,INPUT_1,INPUT_2,INPUT_3,OUT_DUT,OUT_REF,ABS_ERR\n\n");

    fprintf(fp_prec_debug, "DUT_FUNC,REF_FUNC,INPUT_1,INPUT_2,INPUT_3,OUT_DUT,OUT_REF,ABS_ERR\n\n");


    fp_ip_op_plot = fopen("basop_ip_op_plot.csv", "w");
    if (fp_ip_op_plot == NULL) {
        printf("Could not open basop_ip_op_plot.csv\n");
        return retval;
    }
    fprintf(fp_ip_op_plot, "input1, input2, input3, out_dut, out_ref, error (out_dut-out_ref)\n");
    fprintf(fp_ip_op_plot, "NA, NA, NA, NA, NA, NA\n");



    for (i = 0; i < NUM_DEFAULT_TESTCASES; i++)
    {
        int j = 0;
        printf(" dut name = %s \n", input_prec_tests[i].fname_dut);
        strcpy(fname, input_prec_tests[i].fname_dut);
#ifdef DEBUG_LOG
        printf("dut name = %s\t ref name =%s\t dut type = %d\t ref type = %d\n", input_prec_tests[i].fname_dut,
              input_prec_tests[i].fname_ref, input_prec_tests[i].ftype, input_prec_tests[i].ftype);
#endif

#ifdef INPUT_PAT_COMBINATIONS
       for (j = 0; j < (NUM_TEST_PATTERN_1 * 4); j++)
       {

          fp_test_baseop[i][j] = fopen(test_files[i][j],"w");

          if (fp_test_baseop[i][j] == NULL)
          {
              printf("Could not open %s\n",test_files[i][j]);
              return retval;
          }

            execute_each_prec_test(i, input_prec_tests[i].fname_dut, input_prec_tests[i].ftype,
                input_prec_tests[i].fname_ref, input_prec_tests[i].dpat1[j], input_prec_tests[i].dpat2[j],
                input_prec_tests[i].dpat3[j],j);


          fclose(fp_test_baseop[i][j]);
      }
#else /* INPUT_PAT_COMBINATIONS */
        for (j = 0; j < (NUM_TEST_PATTERN_1); j++) {



    fp_test_baseop[i][j] = fopen(test_files[i][j],"w");

    if (fp_test_baseop[i][j] == NULL) 
    {
        printf("Could not open %s\n",test_files[i][j]);
        return retval;
    }
            execute_each_prec_test(i, input_prec_tests[i].fname_dut, input_prec_tests[i].ftype, input_prec_tests[i].fname_ref, input_prec_tests[i].dpat1[j], input_prec_tests[i].dpat2[j], input_prec_tests[i].dpat3[j],j);


    fclose(fp_test_baseop[i][j]);
        }
#endif 	/* INPUT_PAT_COMBINATIONS */


        if (i < (NUM_PREC_TESTS - 1)) {
            if (strcmp(fname, input_prec_tests[i + 1].fname_dut)) {
                fprintf(fp_prec, "\n");
                fprintf(fp_prec_debug, "\n");
                //fprintf(fp_ip_op_plot, "\n");
            }
        }
    }

    // Check special values..
    for (i = 0; i < NUM_DEFAULT_TESTCASES; i++)
    {
	int n;
        void * in1 = (void *)malloc((sizeof(double))*(N_PREC));
        void * in2 = (void *)malloc((sizeof(double))*(N_PREC));
        void * in3 = (void *)malloc((sizeof(double))*(N_PREC));
        void * out_dut = (void *)malloc((sizeof(double))*(N_PREC));
        void * out_ref = (void *)malloc((sizeof(double))*(N_PREC));
        
        // Init special values..
        init_spl_inputs(i, in1, in2, in3);

        // Run test cases with special values.
        run_func(i, input_prec_tests[i].ftype, input_prec_tests[i].fname_dut, input_prec_tests[i].fname_ref, out_dut, out_ref, in1, in2, in3, MAX_NUM_SPL_VALS);

        fprintf(fp_spcl, "%s \n", input_prec_tests[i].fname_dut);
        for (n = 0; n < MAX_NUM_SPL_VALS; n++) 
        {
            print_io_plot(i, fp_spcl, n, input_prec_tests[i].ftype, input_prec_tests[i].fname_dut, input_prec_tests[i].ftype, input_prec_tests[i].fname_ref, (void *)in1, (void *)in2, (void *)in3, (void *)out_dut, (void *)out_ref);
        }

        free(in1);
        free(in2);
        free(in3);
        free(out_dut);
        free(out_ref);
    }

    fclose(fp_spcl);
    fclose(fp_prec);
    fclose(fp_prec_debug);
    fclose(fp_ip_op_plot);

    retval = 0;
    return retval;

}


/* End of file */



