
#include "input_config.h"
#include <stdio.h>

/*
#define MAX_NUM_SANITY_VALS 4

typedef union {

    float f[MAX_NUM_SANITY_VALS];

    double d[MAX_NUM_SANITY_VALS];

} ref_out;



ref_out ref[2] = {

    {.f = { 30467.14153,5910.1427,15846.23837,7271.22705 }},

    {.d = { 30467.14153,5910.1427,15846.23837,7271.22705 }}
};

#define REF_OUT_W_mult_16_16    15
#define REF_OUT_W_mac_16_16     15
#define REF_OUT_W_msu_16_16     15
#define REF_OUT_W_add           15
#define REF_OUT_W_sub           15
*/

Word64 Ref64[] =
{
    0x915baLL,  			//W_mult_16_16
    0x007fffffff0915baLL, 	//W_mac_16_16();
    0x007ffffffef6ea46LL, 	//W_msu_16_16();
    0x8000003EFFFFFFLL, 	//W_add();
    0x7FFFFFBF000001LL, 	//W_sub();
	0x8000003EFFFFFFLL, 	//W_add_nosat();
    0x7FFFFFBF000001LL, 	//W_sub_nosat();
	0x1FFFFFFFC000000LL,	//W_shl();
	0x1FFFFFFFC00000LL, 	//W_shr();
	0x1FFFFFFFC000000LL,  	//W_shl_nosat();
	0x1FFFFFFFC00000LL, 	//W_shr_nosat();
	0x1FFFFFFFC000000LL,	//W_lshl();
	0x1FFFFFFFC00000LL, 	//W_lshr();
	0x7FFFFFFFLL, 			//W_shl_sat_l();
	0x7FFFFFFFLL, 			//W_sat_l();
	0x7FFFFFFFLL, 			//W_sat_m();
	0x4000LL,  				//W_round48_L 
	0x800000LL,  			//W_round64_L
	0x7FFFLL,  				//W_round32_s
	0x8LL,  				//W_norm		
	0x48ADDLL, 				//W_mult0_16_16
    0x7fffffff048ADDLL, 	//W_mac0_16_16();
    0x7FFFFFFEFB7523LL,   	//W_msu0_16_16();		
	0x80000242BA0000LL, 	//W_mac_32_16();
	0x7FFFFDBB460000LL, 	//W_msu_32_16();
	0x243BA0000LL, 			//W_mult_32_16();
	0x7F7FFFE020000LL, 		//W_mult_32_32();
	0x3FBFFFF010000LL,		//W_mult0_32_32();
	0xFF80000001000000LL, 	//W_neg();
	0x007fffffff000000LL,  	//W_abs();
	
	0x40243b9, 				//Madd_32_16
	0x407f7fe, 				//Madd_32_32
	0x40243b9,				//Madd_32_16_r
	0x407f7ff, 				//Madd_32_32_r	
	0x243ba, 				//Mpy_32_16_1
	0x7f7ff, 				//Mpy_32_32
	0x7f800, 				//Mpy_32_32_r	
	0x243ba, 				//Mpy_32_16_r
	0x3FDBC45, 				//Msub_32_16
	0x3FDBC45, 				//Msub_32_16_r
	0x3F80800,				//Msub_32_32
	0x3F807FF,				//Msub_32_32_r	
};

cmplx CRef64[] =
{
	{.re = 0x4FEFFFF, .im = 0x4FEFFFF},			//CL_add
	{.re = 0xFCFF0001, .im = 0x300FFFF},			//CL_sub
	{.re = 0x1fe0000, .im = 0x0},				//CL_msu_j
	{.re = 0x0, .im = 0x7fffffe},				//CL_mac_j
	{.re = 0x0, .im = 0x21fc02},					//CL_multr_32x32
	{.re = 0xffe24bba, .im = 0x110e02},			//CL_multr_32x16
	{.re = 0x522, .im = 0x522},					//C_add
	{.re = 0xfffffd24, .im = 0x2dc},	 			//C_sub
	{.re = 0x0, .im = 0x23},	 					//C_multr
	{.re = 0x48c, .im = 0xFFC},	 				//C_multr
	{.re = 0xff010000, .im = 0xfc000001},	 	//CL_negate
	{.re = 0xfc000001, .im = 0xff0000},	 		//CL_mul_j
	{.re = 0xfffffedd, .im = 0xfffffc01},	 	//C_negate
	{.re = 0xfffffc01, .im = 0x123},	 			//C_mul_j
	{.re = 0xff, .im = 0x400},	 				//C_mac_r
	{.re = 0xff, .im = 0x400},	 				//C_msu_r
	{.re = 0x3FC000, .im = 0xFFFFFF},	 		//CL_shr
	{.re = 0x7f80000, .im = 0x1ffffff8},	 		//CL_shl
	{.re = 0x48, .im = 0xFF},	 				//C_shr
	{.re = 0x918, .im = 0x1ff8},	 				//C_shl
	{.re = 0xfee020, .im = 0x3ff7fff},	 		//CL_scale_32
	{.re = 0x3fc, .im = 0x1000},	 				//CL_scale
	{.re = 0x3fc, .im = 0x1800},	 				//CL_dscale
	{.re = 0xfee020, .im = 0x3ff7fff},	 		//CL_dscale_32
	{.re = 0xff, .im = 0x400},					//CL_round32_16
};

short Val16_1 = 0x0123;
short Val16_2 = 0x03ff;
Word32 Val32_1 = 0x00ff0000;
Word32 Val32_2 = 0x03ffffff;
Word64 Val64_1 = 0x007fffffff000000LL;
Word64 Val64_2 = 0x000000003fffffffLL;


cmplx CVal32_1 = {.re = 0x00ff0000, .im = 0x03ffffff};
cmplx CVal32_2 = {.re = 0x03ffffff, .im = 0x00ff0000};
cmplx_s CVal16_1 = {.re = 0x0123, .im = 0x03ff};
cmplx_s CVal16_2 = {.re = 0x03ff, .im = 0x0123};


#define COMP_RES(ref, cur_out) { \
    if (ref[RefIdx] == cur_out) \
    {\
        SuccessCnt++;\
    }\
    else\
    {\
        printf(" Error : BASOP  %s o/p mismatch !!\n", def_list[BasopCnt].name);\
    }\
    BasopCnt++; RefIdx++;\
}

#define COMP_CMPLX_RES(ref, cur_out) { \
    if ((CRef64[CRefIdx].re == cur_out.re) &&(ref[CRefIdx].im == cur_out.im)) \
    {\
        SuccessCnt++;\
    }\
    else\
    {\
        printf(" Error : BASOP  %s o/p mismatch !!\n", def_list[BasopCnt].name);\
    }\
    BasopCnt++; CRefIdx++;\
}


extern input_struct_prec_t input_prec_tests[TOTAL_NUM_FUNC];
int test_sanity(void)
{
    int retval = 1;
    int SuccessCnt = 0;
    int BasopCnt = 0;
    Word64 res64;
	Word32 res32;
	Word16 res16;
	cmplx Cres32;
	cmplx_s Cres16;
    int RefIdx = 0;
	int CRefIdx = 0;
       
    //W_mult_16_16();
    res64 = W_mult_16_16(Val16_1, Val16_2);
    COMP_RES(Ref64, res64);

    //W_mac_16_16();
    res64 = W_mac_16_16(Val64_1, Val16_1, Val16_2);
    COMP_RES(Ref64, res64);

    //W_msu_16_16();
    res64 = W_msu_16_16(Val64_1, Val16_1, Val16_2);
    COMP_RES(Ref64, res64);
	
    //W_add();
    res64 = W_add(Val64_1, Val64_2);
    COMP_RES(Ref64, res64);

    //W_sub();
    res64 = W_sub(Val64_1, Val64_2);
    COMP_RES(Ref64, res64);

    //W_add_nosat();
    res64 = W_add_nosat(Val64_1, Val64_2);
    COMP_RES(Ref64, res64);

    //W_sub_nosat();
    res64 = W_sub_nosat(Val64_1, Val64_2);
    COMP_RES(Ref64, res64);

    //W_shl();
    res64 = W_shl(Val64_1, 2);
    COMP_RES(Ref64, res64);

    //W_shr();
    res64 = W_shr(Val64_1, 2);
    COMP_RES(Ref64, res64);

    //W_shl_nosat();
    res64 = W_shl_nosat(Val64_1, 2);
    COMP_RES(Ref64, res64);

    //W_shr_nosat();
    res64 = W_shr_nosat(Val64_1, 2);
    COMP_RES(Ref64, res64);

    //W_lshl();
    res64 = W_lshl(Val64_1, 2);
    COMP_RES(Ref64, res64);

    //W_lshr();
    res64 = W_lshr(Val64_1, 2);
    COMP_RES(Ref64, res64);
	
	//W_shl_sat_l();
    res32 = W_shl_sat_l(Val64_1, 2);
    COMP_RES(Ref64, res32);
	
	//W_sat_l();
    res32 = W_sat_l(Val64_1);
    COMP_RES(Ref64, res32);

	//W_sat_m();
    res32 = W_sat_m(Val64_1);
    COMP_RES(Ref64, res32);	

	//W_round48_L();
    res32 = W_round48_L(Val64_2);
    COMP_RES(Ref64, res32);		
		
	
	//W_round64_L();
    res32 = W_round64_L(Val64_1);
    COMP_RES(Ref64, res32);		

	//W_round32_s
	res16 = W_round32_s(Val64_1);
    COMP_RES(Ref64, res16);	
	
	//W_norm
	res16 = W_norm(Val64_1);
    COMP_RES(Ref64, res16);		
		
    //W_mult0_16_16();
    res64 = W_mult0_16_16(Val16_1, Val16_2);
    COMP_RES(Ref64, res64);

    //W_mac0_16_16();
    res64 = W_mac0_16_16(Val64_1, Val16_1, Val16_2);
    COMP_RES(Ref64, res64);

    //W_msu0_16_16();
    res64 = W_msu0_16_16(Val64_1, Val16_1, Val16_2);
    COMP_RES(Ref64, res64);	

	
    //W_mac_32_16();
    res64 = W_mac_32_16(Val64_1, Val32_1, Val16_1);
    COMP_RES(Ref64, res64);		
	
    //W_msu_32_16();
    res64 = W_msu_32_16(Val64_1, Val32_1, Val16_1);
    COMP_RES(Ref64, res64);		
	
    //W_mult_32_16();
    res64 = W_mult_32_16(Val32_1, Val16_1);
    COMP_RES(Ref64, res64);	

    //W_mult_32_32();
    res64 = W_mult_32_32(Val32_1, Val32_2);
    COMP_RES(Ref64, res64);		

    //W_mult0_32_32();
    res64 = W_mult0_32_32(Val32_1, Val32_2);
    COMP_RES(Ref64, res64);		
	
	//W_neg
	res64 = W_neg(Val64_1);
    COMP_RES(Ref64, res64);	
	
	//W_abs
	res64 = W_abs(Val64_1);
    COMP_RES(Ref64, res64);		

	
	//Madd_32_16();
    res32 = Madd_32_16(Val32_2, Val32_1, Val16_1);
    COMP_RES(Ref64, res32);
	
	//Madd_32_32();
    res32 = Madd_32_32(Val32_2, Val32_1, Val32_2);
    COMP_RES(Ref64, res32);
	
	//Madd_32_16_r();
    res32 = Madd_32_16_r(Val32_2, Val32_1, Val16_1);
    COMP_RES(Ref64, res32);
	
	//Madd_32_32_r();
    res32 = Madd_32_32_r(Val32_2, Val32_1, Val32_2);
    COMP_RES(Ref64, res32);	
	
	//Mpy_32_16_1();
    res32 = Mpy_32_16_1(Val32_1, Val16_1);
    COMP_RES(Ref64, res32);	
	
	//Mpy_32_32();
    res32 = Mpy_32_32(Val32_2, Val32_1);
    COMP_RES(Ref64, res32);	
	
	//Mpy_32_32_r();
    res32 = Mpy_32_32_r(Val32_2, Val32_1);
    COMP_RES(Ref64, res32);	

	//Mpy_32_16_r();
    res32 = Mpy_32_16_r(Val32_1, Val16_1);
    COMP_RES(Ref64, res32);	

	//Msub_32_16();
    res32 = Msub_32_16(Val32_2, Val32_1, Val16_1);
    COMP_RES(Ref64, res32);	

	//Msub_32_16_r();
    res32 = Msub_32_16_r(Val32_2, Val32_1, Val16_1);
    COMP_RES(Ref64, res32);	
	
	//Msub_32_32();
    res32 = Msub_32_32(Val32_2, Val32_2, Val32_1);
    COMP_RES(Ref64, res32);

	//Msub_32_32_r();
    res32 = Msub_32_32_r(Val32_2, Val32_2, Val32_1);
    COMP_RES(Ref64, res32);	


	
	//CL_add();
    Cres32 = CL_add(CVal32_1, CVal32_2);
    COMP_CMPLX_RES(CRef64, Cres32);	

	//CL_sub();
    Cres32 = CL_sub(CVal32_1, CVal32_2);
    COMP_CMPLX_RES(CRef64, Cres32);	

	//CL_msu_j();
    Cres32 = CL_msu_j(CVal32_1, CVal32_2);
    COMP_CMPLX_RES(CRef64, Cres32);	

	//CL_mac_j();
    Cres32 = CL_mac_j(CVal32_1, CVal32_2);
    COMP_CMPLX_RES(CRef64, Cres32);	
	
	//CL_multr_32x32();
    Cres32 = CL_multr_32x32(CVal32_1, CVal32_2);
    COMP_CMPLX_RES(CRef64, Cres32);	
	
	//CL_multr_32x16();
    Cres32 = CL_multr_32x16(CVal32_1, CVal16_1);
    COMP_CMPLX_RES(CRef64, Cres32);	

	
	//C_add();
    Cres16 = C_add(CVal16_1, CVal16_2);
    COMP_CMPLX_RES(CRef64, Cres16);	

	//C_sub();
    Cres16 = C_sub(CVal16_1, CVal16_2);
    COMP_CMPLX_RES(CRef64, Cres16);	
	
	//C_multr();
    Cres16 = C_multr(CVal16_1, CVal16_2);
    COMP_CMPLX_RES(CRef64, Cres16);

	//C_scale();
    Cres32 = C_scale(CVal16_1, 2);
    COMP_CMPLX_RES(CRef64, Cres32);	

	//CL_negate();
    Cres32 = CL_negate(CVal32_1);
    COMP_CMPLX_RES(CRef64, Cres32);		
	
	//CL_mul_j();
    Cres32 = CL_mul_j(CVal32_1);
    COMP_CMPLX_RES(CRef64, Cres32);
	
	//C_negate();
    Cres16 = C_negate(CVal16_1);
    COMP_CMPLX_RES(CRef64, Cres16);
	
	//C_mul_j();
    Cres16 = C_mul_j(CVal16_1);
    COMP_CMPLX_RES(CRef64, Cres16);	
	
	//C_mac_r();
    Cres16 = C_mac_r(CVal32_1, CVal16_1, 2);
    COMP_CMPLX_RES(CRef64, Cres16);	

	//C_msu_r();
    Cres16 = C_msu_r(CVal32_1,CVal16_1, 2);
    COMP_CMPLX_RES(CRef64, Cres16);	

	//CL_shr();
    Cres32 = CL_shr(CVal32_1, 2);
    COMP_CMPLX_RES(CRef64, Cres32);	
	
	//CL_shl();
    Cres32 = CL_shl(CVal32_1, 3);
    COMP_CMPLX_RES(CRef64, Cres32);	

	//C_shr();
    Cres16 = C_shr(CVal16_1, 2);
    COMP_CMPLX_RES(CRef64, Cres16);	
	
	//C_shl();
    Cres16 = C_shl(CVal16_1, 3);
    COMP_CMPLX_RES(CRef64, Cres16);	
	
	//CL_scale_32();
    Cres32 = CL_scale_32(CVal32_1, 0x7FF00000);
    COMP_CMPLX_RES(CRef64, Cres32);	

	//CL_scale();
    Cres32 = CL_scale(CVal32_1, 2);
    COMP_CMPLX_RES(CRef64, Cres32);		
	
	//CL_dscale();
    Cres32 = CL_dscale(CVal32_1, 2, 3);
    COMP_CMPLX_RES(CRef64, Cres32);	

	//CL_dscale_32();
    Cres32 = CL_dscale_32(CVal32_1, 0x7FF00000, 0x7FF00000);
    COMP_CMPLX_RES(CRef64, Cres32);	

	//CL_round32_16();
    Cres16 = CL_round32_16(CVal32_1);
    COMP_CMPLX_RES(CRef64, Cres16);		
		
	
    printf("BASOP Sanity test complete :  Total pass  = %d : Total fail = %d : Total test = %d !! \n", SuccessCnt, (TOTAL_NUM_FUNC-SuccessCnt), TOTAL_NUM_FUNC);

    /* Returns non-zero when check fails */
    retval = (TOTAL_NUM_FUNC-SuccessCnt);
    return retval;
}

/* End of file */
