#ifndef __INPUT_CONFIG_H__
#define __INPUT_CONFIG_H__

#include "func_proto.h"
#include "stl.h"


//#define DEBUG_LOG /*Enable this if you want to print each ref and dut output and errors */

// Modify_here  :  Configure  3 things 
//      1) No of test pattern  : Will modify NUM_TEST_PATTERN_1 if we add more test-patterns.
//      2) No of basic ops     : Increment TOTAL_NUM_FUNC for every new baseop added to test.  
//      3) No of samples       : Modify N_PREC to change the number of samples per test.

#define NUM_TEST_PATTERN_1          4
#define TOTAL_NUM_FUNC              67
#define N_PREC                      1000  /* No. of inputs per pattern for precision analysis. */
#define MAX_NUM_INP                 3     /*  CHnage this if some new basic op has more thna 3 inputs. */


#define NUM_DEFAULT_TESTCASES       (TOTAL_NUM_FUNC) 
#define NUM_PREC_TESTS              (TOTAL_NUM_FUNC * NUM_TEST_PATTERN_1 )

#define BASOP_NAME_SIZE             100
#define RAND_SEED_1                 1234    /* Seed for random number generation */
#define RAND_SEED_2                 5678    /* Seed for random number generation */

#define MAX_THRESHOLD               (0.001)                 /* percent absolute error threshold */
#define MASK_SP_EXP_D               (0x7ff0000000000000LL) /* Mask for extracting exponent bits */
#define MASK_SP_EXP_F               (0x7f800000)           /* Mask for extracting exponent bits */
#define MAX_TRIES                   3       /* Number of tries for generating a non special number exceeding which zero will be assigned */

// TBD :  We may need to trim this 
typedef enum input_data_pattern {
    INP_DATA_PAT_MPI_TO_PPI_F = 0,         /* -PI to +PI */
    RANDOM_M1_P1_F,               /* -1 to +1 */
    INP_DATA_PAT_M10_TO_P10_F,             /* -10 to +10 */
    INP_DATA_PAT_M100_TO_P100_F,           /* -100 to +100 */
    INP_DATA_PAT_M1000_TO_P1000_F,         /* -1000 to +1000 */
    RANDOM_M10k_P10k_F,       /* -10000 to +10000 */
    INP_DATA_PAT_M100000_TO_P100000_F,     /* -100000 to +100000 */
    INP_DATA_PAT_RAND_F,                   /* Randomely generated sign, exp and mantissa */
    INP_DATA_PAT_EXP_LIM_F,                /* Exp limited */
    INP_DATA_PAT_MANT_LIM_F,               /* Manntissa limited */
    INP_DATA_PAT_SUBNORMAL_F,              /* Sub-normal inputs */
    INP_DATA_PAT_SPECIAL_F,                /* Hardcoded special test-cases followed by random data */
    INP_DATA_PAT_0_TO_P100000_F,           /* +ve inputs */
    INP_DATA_PAT_M100000_TO_0_F,           /* -ve inputs */
    INP_DATA_PAT_CUSTOM_F,
    RANDOM_M05_P05_F,               /* -0.5 to + 0.5 */

    /*************** double patterns ******************/
    INP_DATA_PAT_MPI_TO_PPI_D = 100,         /* -PI to +PI */
    RANDOM_M1_P1_D,               /* -1 to +1 */
    INP_DATA_PAT_M10_TO_P10_D,             /* -10 to +10 */
    INP_DATA_PAT_M100_TO_P100_D,           /* -100 to +100 */
    INP_DATA_PAT_M1000_TO_P1000_D,         /* -1000 to +1000 */
    RANDOM_M10k_P10k_D,       /* -10000 to +10000 */
    INP_DATA_PAT_M100000_TO_P100000_D,     /* -100000 to +100000 */
    INP_DATA_PAT_RAND_D,                   /* Randomely generated sign, exp and mantissa */
    INP_DATA_PAT_EXP_LIM_D,                /* Exp limited */
    INP_DATA_PAT_MANT_LIM_D,               /* Manntissa limited */
    INP_DATA_PAT_SUBNORMAL_D,              /* Sub-normal inputs */
    INP_DATA_PAT_SPECIAL_D,                /* Hardcoded special test-cases followed by random data */
    INP_DATA_PAT_0_TO_P100000_D,           /* +ve inputs */
    INP_DATA_PAT_M100000_TO_0_D,           /* -ve inputs */
    INP_DATA_PAT_CUSTOM_D,
    SWEEP_M05_P05_F,   /* -0.5 to +0.5 with linear steps */
    SWEEP_M1_P1_F,		/* -1 to +1 with linear steps float */
    SWEEP_M1_P1_D,		/* -1 to +1 with linear steps double precision */
    SWEEP_M10k_P10k_D,	/* -10000 to +10000 with linear steps double precision */
    SWEEP_M10k_P10k_F,	/* -10000 to +10000 with linear steps float precision  */
    SWEEP_M05_P05_piecewise_F,   /* -0.5 to +0.5 with piecewise linear steps (4 chunks) */
    SWEEP_M1_P1_piecewise_F,		/* -1 to +1 with piecewise linear steps float (4 chunks) */
    SWEEP_M1_P1_piecewise_D,		/* -1 to +1 with piecewise linear steps double precision (4 chunks) */
    SWEEP_M10k_P10k_piecewise_D,	/* -10000 to +10000 with piecewise linear steps double precision (4 chunks) */
    SWEEP_M10k_P10k_piecewise_F,	/* -10000 to +10000 with piecewise linear steps float precision (4 chunks) */

    /************* int32 patterns **********************/
    RANDOM_M32768_P32767_I = 200,
    RANDOM_SHIFT,              /* Random number for shift value : range-31 to 31*/
    SWEEP_M32768_P32767_I, 		/* 32768 to -32767 in linear steps */
    SWEEP_M32768_P32767_piecewise_I, 		/* 32768 to -32767 in piecewise linear steps (4 chunks) */

    CUSTOM_INP_M05_P05_F,
	CUSTOM_INP_M05_P05_F_N,
    CUSTOM_INP_M1_P1_F,
    CUSTOM_INP_M1_P1_D,
	CUSTOM_INP_M1_P1_D_N,
    CUSTOM_INP_M32768_P32767_I,
	CUSTOM_INP_M32768_P32767_I_N,
    CUSTOM_INP_M10k_P10k_F,
    CUSTOM_INP_M10k_P10k_D,
    /************* No input ****************************/
    NA = 1000

} input_data_pattern_t;

/**************************************************************/

// Modify_here  :  Add all new function types here.
/*
a.	OW_IW_IS_IS   ; for  W_MAC_16_16
b.	OCL_ICL_CS_ICS  :  for complex to clearly separate out 16 bit and 32 bit data types.
*/

typedef enum func_type {
    FUNC_TYPE_OW_IW,            /* 1 Double input :: Double output */
    FUNC_TYPE_OW_IW_IS_IS,     /* 1 Double input, 2 float input :: Double output */
    FUNC_TYPE_OW_IS_IS,         /* 2 float input :: Double output */
    FUNC_TYPE_OW_IS_IS_0,         /* 2 float input :: Double output  integer math */
    FUNC_TYPE_OW_IW_IW,     /* 2 Double inputs :: Double output */
    FUNC_TYPE_OW_IL_IS,         /* 1 long input, 1 short input :: Double output */
    FUNC_TYPE_OW_IW_IS,         /* 1 Double input, 1 short input :: Double output */
    FUNC_TYPE_OW_IW64_IL,         /* 1 Word64 input, 1 int input :: Double output */
    FUNC_TYPE_OW_IW_IS_IS_0,  /* 1 Double input, 2 short input :: Double output */
    FUNC_TYPE_OW_IW_IL_IS,  /* 1 Double input, 1 long input, 1 short input :: Double output */
    FUNC_TYPE_OW_IL_IL,     /* 2 long input :: Double output */
    FUNC_TYPE_OW_IL_IL_0,   /* 2 long input :: Double output  :  Integer version */
    FUNC_TYPE_OL_IW,            /* 1 double input :: long output */
    FUNC_TYPE_OS_IW,            /* 1 double input :: short output */
    FUNC_TYPE_OL_IW32,          /* 1 double input in Q31 format :: long output Q31 */
    FUNC_TYPE_OL_IW32_IL,       /* 1 double input in Q31 format, 1 long input :: long output Q31 */
    FUNC_TYPE_OL_IW48,          /* 1 double input in Q47 format:: long output Q31 */
    FUNC_TYPE_OL_IW64,          /* 1 double input in Q63 format:: long output Q31 */
    FUNC_TYPE_OS_IW48,          /* 1 double input in Q32 format:: long output Q15 */
    FUNC_TYPE_OL_IL_IL_IS,     /* 2 long inputs, 1 short input :: long output  :  fractional version */
    FUNC_TYPE_OL_IL_IL_IL,        /* 3 long inputs, :: long output  :  fractional version */
    FUNC_TYPE_OCL_ICL_ICL,  /* 2 32bit complex inputs, :: 32 bit complex output  :  fractional version */
    FUNC_TYPE_OCL_ICL_IC,   /* 1 32bit complex input, 1 16bit complex input :: 32 bit complex output  :  fractional version */
    FUNC_TYPE_OC_IC_IC,     /* 2 16bit complex inputs :: 16 bit complex output  :  fractional version */
    FUNC_TYPE_OCL_IC_IS,     /* 1 16bit complex input, 1 16 bit input:: 32 bit complex output  :  fractional version */
	FUNC_TYPE_OCL_ICL,       /* 1 32bit complex input  :: 32 bit complex output  :  fractional version */
	FUNC_TYPE_OC_IC,         /* 1 16bit complex input  :: 16 bit complex output  :  fractional version */
	FUNC_TYPE_OC_ICL_IC_IS,  /* 1 32bit complex input, 1 16bit complex input and 1 16 bit input  :: 16 bit complex output  :  fractional version */
	FUNC_TYPE_OCL_ICL_IL0,   /* 1 32bit complex input, 1 32 bit input  :: 32 bit complex output  :  shift operation */
	FUNC_TYPE_OC_IC_IS0,	 /* 1 16bit complex input, 1 16 bit input  :: 16 bit complex output  :  shift operation */
	FUNC_TYPE_OCL_ICL_IL,    /* 1 32bit complex input, 1 32 bit input:: 32 bit complex output  :  fractional version */
	FUNC_TYPE_OCL_ICL_IS,    /* 1 32bit complex input, 1 16 bit input:: 32 bit complex output  :  fractional version */
	FUNC_TYPE_OCL_ICL_IS_IS, /* 1 32bit complex input, 2 16 bit inputs:: 32 bit complex output  :  fractional version */
	FUNC_TYPE_OCL_ICL_IL_IL, /* 1 32bit complex input, 2 32 bit inputs:: 32 bit complex output  :  fractional version */
	FUNC_TYPE_OC_ICL,		 /* 1 32bit complex input :: 1 bit complex output  :  fractional version */
    FUNC_TYPE_OL_IL_IS,		 /* 1 long input, 1 short input :: long output  :  fractional version */
    FUNC_TYPE_OL_IL_IL		 /* 1 long input, 1 long input :: long output  :  fractional version */
}func_type_t;

/* Different types of inputs.*/

typedef enum data_type {
    DATA_TYPE_SHORT = 0,         /* Word16 */
    DATA_TYPE_LONG,              /* Word32 */
    DATA_TYPE_WORD64,            /* Word64 */
    DATA_TYPE_FLOAT,             /* float */
    DATA_TYPE_FLOAT_L,           /* float */
    DATA_TYPE_DOUBLE,            /* double */
	DATA_TYPE_DOUBLE_Q31,        /* double in ref, but this will be converted to Q31 for DUT */
	DATA_TYPE_DOUBLE_Q47,        /* double in ref, but this will be converted to Q47 for DUT */
	DATA_TYPE_DOUBLE_Q63,        /* double in ref, but this will be converted to Q63 for DUT */
    DATA_TYPE_CMPLX_S,           /* Word16 complex */
    DATA_TYPE_CMPLX_L,           /* Word32 complex */
    DATA_TYPE_INV                /* Invalid data type.*/
  
}data_type_t;


/*************TESTCASES STRUCTS ******************************/
typedef struct input_struct_prec {
    char fname_dut[BASOP_NAME_SIZE];
    char fname_ref[BASOP_NAME_SIZE];
     
    func_type_t ftype;
    data_type_t out_data;
    data_type_t inp_data[MAX_NUM_INP];

    void            *func_ptr_dut;          /* Function pointer for DUT */
    void            *func_ptr_ref;          /* Function pointer for REF */
  
#ifdef INPUT_PAT_COMBINATIONS	
    input_data_pattern_t dpat1[NUM_TEST_PATTERN_1*4];
    input_data_pattern_t dpat2[NUM_TEST_PATTERN_1*4];
    input_data_pattern_t dpat3[NUM_TEST_PATTERN_1*4];
#else /* INPUT_PAT_COMBINATIONS */	
    input_data_pattern_t dpat1[NUM_TEST_PATTERN_1];
    input_data_pattern_t dpat2[NUM_TEST_PATTERN_1];
    input_data_pattern_t dpat3[NUM_TEST_PATTERN_1];
#endif /* INPUT_PAT_COMBINATIONS */	
}input_struct_prec_t;

extern input_struct_prec_t input_prec_tests[TOTAL_NUM_FUNC];


typedef struct default_list {
    char            name[BASOP_NAME_SIZE];  /* Name of BASOP */
    func_type_t     ftype;                  /* Type of BASOP */
    data_type_t     out_data;               /* Type of Output argument of BASOP */
    data_type_t     inp_data[MAX_NUM_INP];  /* Type of Input arguments of BASOP */
    void            *func_ptr_dut;          /* Function pointer for DUT */
    void            *func_ptr_ref;          /* Function pointer for REF */
  
}default_list_t;

extern default_list_t def_list[TOTAL_NUM_FUNC];

void create_default_tests_prec();

/*********************************************************/
#include <complex.h>

// Modify_here  : Define a function pointer to the new function Type

/* REF function prototypes.. */
typedef double(*fptr_OD_ID_IF_IF) (double, float, float);
typedef double(*fptr_OD_ID_ID) (double, double);
typedef double(*fptr_OD_ID_IL) (double, int);
typedef double(*fptr_OD_ID_IS) (double, short);
typedef double(*fptr_OD_ID_IF) (double, float);
typedef double(*fptr_OD_IF_IF) (float, float);
typedef double(*fptr_OD_ID) (double);
typedef long (*fptr_OL_ID) (double);

typedef float(*fptr_OF_IF_IF_IF) (float, float, float);
typedef float(*fptr_OF_IF_IF) (float, float);

typedef f_cmplx(*fptr_OFC_IFC_IFC) (f_cmplx, f_cmplx);
typedef f_cmplx(*fptr_OFC_IFC) (f_cmplx);
typedef f_cmplx(*fptr_OFC_IFC_IF) (f_cmplx, float);
typedef f_cmplx(*fptr_OFC_IFC_IF_IF) (f_cmplx, float, float);
typedef f_cmplx(*fptr_OFC_IFC_IFC_IF) (f_cmplx, f_cmplx, float);
typedef f_cmplx(*fptr_OFC_IFC_IL0) (f_cmplx, int);
typedef f_cmplx(*fptr_OFC_IFC_IS0) (f_cmplx, short int);

/* DUT function prototypes.. */
typedef Word64(*fptr_OW_IW_IW)  (Word64, Word64);
typedef Word64(*fptr_OW_IW_IS)  (Word64, Word16);
typedef Word32(*fptr_OL_IW_IL)  (Word64, Word32);

typedef Word64(*fptr_OW_IW_IS_IS_0) (Word64, Word16, Word16);
typedef Word64(*fptr_OW_IW_IL_IS) (Word64, Word32, Word16);
typedef Word64(*fptr_OW_IL_IL) (Word32, Word32);
typedef Word64(*fptr_OW_IL_IS) (Word32, Word16);
typedef Word64(*fptr_OW_IW) (Word64);
typedef Word64(*fptr_OW_IS_IS_0) ( Word16, Word16);
typedef Word64(*fptr_OW_IS_IS) (Word16, Word16);
typedef Word32(*fptr_OL_IW) (Word64);
typedef Word16(*fptr_OS_IW) (Word64);

typedef Word32(*fptr_OL_IL_IL_IS) (Word32, Word32, Word16);
typedef Word32(*fptr_OL_IL_IL_IL) (Word32, Word32, Word32);
typedef Word32(*fptr_OL_IL_IS) (Word32, Word16);
typedef Word32(*fptr_OL_IL_IL) (Word32, Word32);

typedef cmplx(*fptr_OCL_ICL_ICL) (cmplx, cmplx);
typedef cmplx(*fptr_OCL_ICL) (cmplx);
typedef cmplx_s(*fptr_OC_ICL) (cmplx);
typedef cmplx(*fptr_OCL_ICL_IC) (cmplx, cmplx_s);
typedef cmplx(*fptr_OCL_ICL_IL) (cmplx, Word32);
typedef cmplx(*fptr_OCL_ICL_IS) (cmplx, Word16);
typedef cmplx(*fptr_OCL_ICL_IS_IS) (cmplx, Word16, Word16);
typedef cmplx(*fptr_OCL_ICL_IL_IL) (cmplx, Word32, Word32);
typedef cmplx_s(*fptr_OC_IC_IS) (cmplx_s, Word16);
typedef cmplx_s(*fptr_OC_IC_IC) (cmplx_s, cmplx_s);
typedef cmplx_s(*fptr_OC_IC) (cmplx_s);
typedef cmplx(*fptr_OCL_IC_IS) (cmplx_s, Word16);
typedef cmplx_s(*fptr_OC_ICL_IC_IS) (cmplx, cmplx_s, Word16);

/**********Helper Functions declarations ****************/
#ifdef IDENTICAL_FLOAT_INP
void initf(int pat, float  *ptr_out, int len, int Q_format);
void initd(int pat, double *ptr_out, int len, int Q_format);
void initfc(int pat, f_cmplx *ptr_out, int len, int Q_format);

#else /* #ifdef IDENTICAL_FLOAT_INP */
void initf(int pat, float  *ptr_out, int len);
void initd(int pat, double *ptr_out, int len);
void initfc(int pat, f_cmplx *ptr_out, int len);

#endif  /* #ifdef IDENTICAL_FLOAT_INP */

void initi16(int pat, void  *ptr_out, int len);
void initi32(int pat, void  *ptr_out, int len);
void initi64(int pat, void  *ptr_out, int len);
void init_exp_f(float  *ptr_out, char inp_name[][100]);
void init_exp_d(double *ptr_out, char inp_name[][100]);
void init_exp_i(int    *ptr_out, char inp_name[][100]);

/********************************************************/
int test_precision();
void get_data_pat(int cnt, char *ptr_name);
void get_error_statistics(double *p_err, int length, double *p_avg_abs_err, double *p_avg_err, double *p_std_err);
/******************************************************/

#endif /* __INPUT_CONFIG_H__ */

/* End of file */


