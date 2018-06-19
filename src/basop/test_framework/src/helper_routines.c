

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "input_config.h"


/************* create default test cases *********************/

input_data_pattern_t test_pattern_F[ NUM_TEST_PATTERN_1+1 ] = {	
	RANDOM_M05_P05_F,
	SWEEP_M05_P05_F,	
	SWEEP_M05_P05_piecewise_F,
	CUSTOM_INP_M05_P05_F_N,
	CUSTOM_INP_M05_P05_F
};

input_data_pattern_t test_pattern_D[ NUM_TEST_PATTERN_1+1 ] = {	
	RANDOM_M1_P1_D,
	SWEEP_M1_P1_D,
	SWEEP_M1_P1_piecewise_D,
	CUSTOM_INP_M1_P1_D_N,
	CUSTOM_INP_M1_P1_D
};

input_data_pattern_t test_pattern_R[ NUM_TEST_PATTERN_1+1 ] = {	
	RANDOM_M32768_P32767_I,
	SWEEP_M32768_P32767_I,
	SWEEP_M32768_P32767_piecewise_I,
	CUSTOM_INP_M32768_P32767_I_N,
	CUSTOM_INP_M32768_P32767_I
};

/************* create default precision tests ****************/
void create_default_tests_prec(){
  int i,j;
  
  for (i=0; i<NUM_DEFAULT_TESTCASES; i++)
  {
    {
        strcpy(input_prec_tests[i].fname_dut, def_list[i].name);
        strcpy(input_prec_tests[i].fname_ref, def_list[i].name);
        input_prec_tests[i].ftype = def_list[i].ftype;
        input_prec_tests[i].func_ptr_dut = def_list[i].func_ptr_dut;
        input_prec_tests[i].func_ptr_ref = def_list[i].func_ptr_ref;

        input_prec_tests[i].inp_data[0] = def_list[i].inp_data[0];
        input_prec_tests[i].inp_data[1] = def_list[i].inp_data[1];
        input_prec_tests[i].inp_data[2] = def_list[i].inp_data[2];

        input_prec_tests[i].out_data = def_list[i].out_data;
     
        for (j = 0; j<(NUM_TEST_PATTERN_1-1 ); j++) 
        {
          
			if ((input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q63) )
                input_prec_tests[i].dpat1[j] = test_pattern_D[j];
            else if ((input_prec_tests[i].inp_data[0] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat1[j] = test_pattern_F[j];
            else if (input_prec_tests[i].inp_data[0] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat1[j] = test_pattern_R[j];
            else if (input_prec_tests[i].inp_data[0] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat1[j] = test_pattern_R[j];  // TBD : define array to rpovide 16 bit INT i/p
			else if (input_prec_tests[i].inp_data[0] == DATA_TYPE_WORD64)
				input_prec_tests[i].dpat1[j] = test_pattern_R[j];  // TBD : define array to rpovide 64 bit INT i/p

			if ((input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat2[j] = test_pattern_D[j];
            else if ((input_prec_tests[i].inp_data[1] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat2[j] = test_pattern_F[j];
            else if (input_prec_tests[i].inp_data[1] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat2[j] = test_pattern_R[j];
            else if (input_prec_tests[i].inp_data[1] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat2[j] = test_pattern_R[j];  // TBD : define array to rpovide 16 bit INT i/p
			else if (input_prec_tests[i].inp_data[1] == DATA_TYPE_WORD64)
				input_prec_tests[i].dpat2[j] = test_pattern_R[j];  // TBD : define array to rpovide 64 bit INT i/p

			if ((input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat3[j] = test_pattern_D[j];
            else if ((input_prec_tests[i].inp_data[2] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat3[j] = test_pattern_F[j];
            else if (input_prec_tests[i].inp_data[2] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat3[j] = test_pattern_R[j];
            else if (input_prec_tests[i].inp_data[2] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat3[j] = test_pattern_R[j];  // TBD : define array to rpovide 16 bit INT i/p
			else if (input_prec_tests[i].inp_data[2] == DATA_TYPE_WORD64)
				input_prec_tests[i].dpat3[j] = test_pattern_R[j];  // TBD : define array to rpovide 64 bit INT i/p

            }
		 for (j = (NUM_TEST_PATTERN_1-1); j<(NUM_TEST_PATTERN_1 ); j++) 
        {
          
			if ((input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat1[j] = test_pattern_D[j];
            else if ((input_prec_tests[i].inp_data[0] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat1[j] = test_pattern_F[j];
            else if (input_prec_tests[i].inp_data[0] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat1[j] = test_pattern_R[j];
            else if (input_prec_tests[i].inp_data[0] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat1[j] = test_pattern_R[j];  // TBD : define array to rpovide 16 bit INT i/p
			else if (input_prec_tests[i].inp_data[0] == DATA_TYPE_WORD64)
				input_prec_tests[i].dpat1[j] = test_pattern_R[j];  // TBD : define array to rpovide 64 bit INT i/p

			if ((input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat2[j] = test_pattern_D[j+1];
            else if ((input_prec_tests[i].inp_data[1] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat2[j] = test_pattern_F[j+1];
            else if (input_prec_tests[i].inp_data[1] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat2[j] = test_pattern_R[j+1];
            else if (input_prec_tests[i].inp_data[1] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat2[j] = test_pattern_R[j+1];  // TBD : define array to rpovide 16 bit INT i/p
			else if (input_prec_tests[i].inp_data[1] == DATA_TYPE_WORD64)
				input_prec_tests[i].dpat2[j] = test_pattern_R[j+1];  // TBD : define array to rpovide 64 bit INT i/p

			if ((input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat3[j] = test_pattern_D[j];
            else if ((input_prec_tests[i].inp_data[2] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat3[j] = test_pattern_F[j];
            else if (input_prec_tests[i].inp_data[2] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat3[j] = test_pattern_R[j];
            else if (input_prec_tests[i].inp_data[2] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat3[j] = test_pattern_R[j];  // TBD : define array to rpovide 16 bit INT i/p
			else if (input_prec_tests[i].inp_data[2] == DATA_TYPE_WORD64)
				input_prec_tests[i].dpat3[j] = test_pattern_R[j];  // TBD : define array to rpovide 64 bit INT i/p

            }
#ifdef INPUT_PAT_COMBINATIONS			
		for (j = NUM_TEST_PATTERN_1; j<(NUM_TEST_PATTERN_1*2 ); j++) 
        {
          
            if ((input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat1[j] = test_pattern_D[j%4];
            else if ((input_prec_tests[i].inp_data[0] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat1[j] = test_pattern_F[j%4];
            else if (input_prec_tests[i].inp_data[0] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat1[j] = test_pattern_R[j%4];
            else if (input_prec_tests[i].inp_data[0] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat1[j] = test_pattern_R[j%4];  // TBD : define array to rpovide 16 bit INT i/p

            if ((input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat2[j] = test_pattern_D[(j+1)%4];
            else if ((input_prec_tests[i].inp_data[1] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat2[j] = test_pattern_F[(j+1)%4];
            else if (input_prec_tests[i].inp_data[1] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat2[j] = test_pattern_R[(j+1)%4];
            else if (input_prec_tests[i].inp_data[1] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat2[j] = test_pattern_R[(j+1)%4];  // TBD : define array to rpovide 16 bit INT i/p

            if ((input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat3[j] = test_pattern_D[j%4];
            else if ((input_prec_tests[i].inp_data[2] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat3[j] = test_pattern_F[j%4];
            else if (input_prec_tests[i].inp_data[2] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat3[j] = test_pattern_R[j%4];
            else if (input_prec_tests[i].inp_data[2] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat3[j] = test_pattern_R[j%4];  // TBD : define array to rpovide 16 bit INT i/p

            }
		for (j = NUM_TEST_PATTERN_1*2; j<(NUM_TEST_PATTERN_1*3 ); j++) 
        {
          
            if ((input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat1[j] = test_pattern_D[j%4];
            else if ((input_prec_tests[i].inp_data[0] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat1[j] = test_pattern_F[j%4];
            else if (input_prec_tests[i].inp_data[0] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat1[j] = test_pattern_R[j%4];
            else if (input_prec_tests[i].inp_data[0] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat1[j] = test_pattern_R[j%4];  // TBD : define array to rpovide 16 bit INT i/p

            if ((input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat2[j] = test_pattern_D[(j+2)%4];
            else if ((input_prec_tests[i].inp_data[1] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat2[j] = test_pattern_F[(j+2)%4];
            else if (input_prec_tests[i].inp_data[1] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat2[j] = test_pattern_R[(j+2)%4];
            else if (input_prec_tests[i].inp_data[1] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat2[j] = test_pattern_R[(j+2)%4];  // TBD : define array to rpovide 16 bit INT i/p

            if ((input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat3[j] = test_pattern_D[(j+1)%4];
            else if ((input_prec_tests[i].inp_data[2] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat3[j] = test_pattern_F[(j+1)%4];
            else if (input_prec_tests[i].inp_data[2] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat3[j] = test_pattern_R[(j+1)%4];
            else if (input_prec_tests[i].inp_data[2] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat3[j] = test_pattern_R[(j+1)%4];  // TBD : define array to rpovide 16 bit INT i/p

            }
		for (j = NUM_TEST_PATTERN_1*3; j<(NUM_TEST_PATTERN_1*4 ); j++) 
        {
          
            if ((input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat1[j] = test_pattern_D[j%4];
            else if ((input_prec_tests[i].inp_data[0] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[0] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat1[j] = test_pattern_F[j%4];
            else if (input_prec_tests[i].inp_data[0] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat1[j] = test_pattern_R[j%4];
            else if (input_prec_tests[i].inp_data[0] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat1[j] = test_pattern_R[j%4];  // TBD : define array to rpovide 16 bit INT i/p

            if ((input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat2[j] = test_pattern_D[(j+3)%4];
            else if ((input_prec_tests[i].inp_data[1] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[1] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat2[j] = test_pattern_F[(j+3)%4];
            else if (input_prec_tests[i].inp_data[1] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat2[j] = test_pattern_R[(j+3)%4];
            else if (input_prec_tests[i].inp_data[1] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat2[j] = test_pattern_R[(j+3)%4];  // TBD : define array to rpovide 16 bit INT i/p

            if ((input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q31) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q47) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_DOUBLE_Q63))
                input_prec_tests[i].dpat3[j] = test_pattern_D[(j+2)%4];
            else if ((input_prec_tests[i].inp_data[2] == DATA_TYPE_FLOAT) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_FLOAT_L) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_CMPLX_S) || (input_prec_tests[i].inp_data[2] == DATA_TYPE_CMPLX_L))
                input_prec_tests[i].dpat3[j] = test_pattern_F[(j+2)%4];
            else if (input_prec_tests[i].inp_data[2] == DATA_TYPE_LONG)
                input_prec_tests[i].dpat3[j] = test_pattern_R[(j+2)%4];
            else if (input_prec_tests[i].inp_data[2] == DATA_TYPE_SHORT)
                input_prec_tests[i].dpat3[j] = test_pattern_R[(j+2)%4];  // TBD : define array to rpovide 16 bit INT i/p

            }
#endif /* INPUT_PAT_COMBINATIONS */			
    }
  }
}

void get_error_statistics(double *p_err, int length, double *p_avg_abs_err, double *p_avg_err, double *p_std_err)
{
   int i;
   double sum = 0.0f;
   double abs_sum = 0.0f;
   double std_err;
   double err;
   unsigned long long cnt;
    
   /* compute average absolute error and average error */
   cnt = 0;
   for(i=0; i<length; i++)
   {
       if(0 == (isnan(p_err[i]))) {
           if(p_err[i] < 0.0f) {
             abs_sum = abs_sum - p_err[i];
           } else {
             abs_sum = abs_sum + p_err[i];
           }
           sum = sum + p_err[i];
           cnt++;
       }
   }
   abs_sum = abs_sum/cnt;
   sum = sum/cnt;

   /* Compute standard deviation */
   cnt = 0;
   std_err = 0.0f;
   for(i=0; i<length; i++)
   {
       if(0 == (isnan(p_err[i]))) {
           err = p_err[i] - sum;
           std_err += (err * err);
           cnt++;
       }
   }
   std_err = std_err/cnt;
   std_err = sqrt(std_err);

   *p_avg_abs_err = abs_sum;
   *p_avg_err = sum;
   *p_std_err = std_err;
}

/* End of file */
