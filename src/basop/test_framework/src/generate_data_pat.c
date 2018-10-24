
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "input_config.h"
#include "custom_data.h"

/***********double number generation*****************************/

#define Q63_MULT_FACT ( (double)9223372036854775808.0 )
#define Q47_MULT_FACT ( (double)140737488355328.0 )
#define DUBLE_MULT_FACT 2147483648.0f
#define SHORT_MULT_FACT 32768.0f

#define GET_IDENTICAL_FLOAT_Q31(x) (((float)((int)(x*(DUBLE_MULT_FACT))))/DUBLE_MULT_FACT)
#define GET_IDENTICAL_FLOAT_Q15(x) (((float)((short int)(x*(SHORT_MULT_FACT))))/SHORT_MULT_FACT)

#define GET_IDENTICAL_DOUBLE_Q63(x) (((double)((long long int)(x*(Q63_MULT_FACT))))/Q63_MULT_FACT)
#define GET_IDENTICAL_DOUBLE_Q47(x) (((double)((long long int)(x*(Q47_MULT_FACT))))/Q47_MULT_FACT)
#define GET_IDENTICAL_DOUBLE_Q31(x) (((double)((long long int)(x*(DUBLE_MULT_FACT))))/DUBLE_MULT_FACT)
#define GET_IDENTICAL_DOUBLE_Q15(x) (((double)((long long int)(x*(SHORT_MULT_FACT))))/SHORT_MULT_FACT)

static float identical_float_val(float x, int n)
{
    if (n)
    {
        return GET_IDENTICAL_FLOAT_Q15(x);
    }
    else
    {
        return GET_IDENTICAL_FLOAT_Q31(x);
    }
}

static double identical_double_val(double x, int n)
{
    if (n == 1)
    {
        return GET_IDENTICAL_DOUBLE_Q15(x);
    }
	else if (n == 2)
    {
        return GET_IDENTICAL_DOUBLE_Q31(x);
    }
	else if (n == 3)
	{
		return GET_IDENTICAL_DOUBLE_Q47(x);
	}
	else if (n == 4)
	{
		return GET_IDENTICAL_DOUBLE_Q63(x);
	}
    else 
    {
        return x;
    }
}
double get_double_no(void);
double prepare_double_no(long long int sign_bit, long long int exp, long long int mant);
int check_special_double(double in);


/* Generate floating point numbers within different ranges */
#ifdef IDENTICAL_FLOAT_INP
void initd(int pat, double *ptr_out, int len, int Q_format) {
#else /* #ifdef IDENTICAL_FLOAT_INP */
void initd(int pat, double *ptr_out, int len) {
#endif /* #ifdef IDENTICAL_FLOAT_INP */

    int i;
    long long int sign_bit;
    long long int exp;
    long long int mant;
    double val;
    int cnt;
    switch (pat)
    {
    case INP_DATA_PAT_MPI_TO_PPI_D:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_double[i];
                ptr_out[i] = val*22.0l / 7.0l;
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_double(ptr_out[i]));
        }
        break;

    case RANDOM_M1_P1_D:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_double[i];
#ifdef IDENTICAL_FLOAT_INP
                ptr_out[i] = identical_double_val(val, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
                ptr_out[i] = val;
#endif /* #ifdef IDENTICAL_FLOAT_INP */
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_double(ptr_out[i]));
        }
        break;

    case CUSTOM_INP_M1_P1_D:
        for (i = 0; i < len; ++i) {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_double_val(CUSTOM_DATA_M1_P1_D_nnpp[i], Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = CUSTOM_DATA_M1_P1_D_nnpp[i];
#endif /* #ifdef IDENTICAL_FLOAT_INP */

        }		
        break;

    case CUSTOM_INP_M1_P1_D_N:
        for (i = 0; i < len; ++i) {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_double_val(CUSTOM_DATA_M1_P1_D_npnp[i], Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = CUSTOM_DATA_M1_P1_D_npnp[i];
#endif /* #ifdef IDENTICAL_FLOAT_INP */

        }		

        break;

    case INP_DATA_PAT_M10_TO_P10_D:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_double[i];
                ptr_out[i] = val*10.0;
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_double(ptr_out[i]));
        }
        break;

    case INP_DATA_PAT_M100_TO_P100_D:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_double[i];
                ptr_out[i] = val*100.0;
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_double(ptr_out[i]));
        }
        break;

    case INP_DATA_PAT_M1000_TO_P1000_D:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_double[i];
                ptr_out[i] = val*1000.0;
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_double(ptr_out[i]));
        }
        break;

    case RANDOM_M10k_P10k_D:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_double[i];
                ptr_out[i] = val*10000.0;
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_double(ptr_out[i]));
        }

        break;


    case CUSTOM_INP_M10k_P10k_D:
        for (i = 0; i < len; ++i) {
            ptr_out[i] = CUSTOM_DATA_M10k_P10k_D[i];
        }
        break;

    case SWEEP_M10k_P10k_D:
    {
        double step = 1.96 / len;
        double start = -0.99;
        for (i = 0; i < len; ++i)
        {
            ptr_out[i] = start * 10000;
            start += step;
        }
        // Force middile point to zero
        ptr_out[len / 2] = (double)0;
    }
    break;

    case SWEEP_M10k_P10k_piecewise_D:
    {
        double step = 0.1 / (len / 4);
        double start = -0.99;
        for (i = 0; i < len / 4; ++i)
        {
            ptr_out[i] = start * 10000;
            start += step;
        }
        start = -0.1;
        for (i = len / 4; i < 2 * len / 4; ++i)
        {
            ptr_out[i] = start * 10000;
            start += step;
        }
        start = 0;
        for (i = 2 * len / 4; i < 3 * len / 4; ++i)
        {
            ptr_out[i] = start * 10000;
            start += step;
        }
        start = 0.9;
        for (i = 3 * len / 4; i < 4 * len / 4; ++i)
        {
            ptr_out[i] = start * 10000;
            start += step;
        }
        ptr_out[len / 2] = (double)0;
    }
    break;

    case INP_DATA_PAT_M100000_TO_P100000_D:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_double[i];
                ptr_out[i] = val*100000.0;
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_double(ptr_out[i]));
        }
        break;

    case INP_DATA_PAT_RAND_D:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                sign_bit = RANDOM_IGET32_NO[i]& 0x1;
                mant = RANDOM_IGET32_NO[i]>> 12;
                exp = RANDOM_IGET32_NO[i] % 2048;

                ptr_out[i] = prepare_double_no(sign_bit, exp, mant);
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_double(ptr_out[i]));

        }
        break;

    case INP_DATA_PAT_EXP_LIM_D:
        for (i = 0; i < len; ++i) {
            sign_bit =RANDOM_IGET32_NO[i] & 0x1;
            mant = RANDOM_IGET32_NO[i] >> 12;
            exp = RANDOM_IGET32_NO[i] % 4;
            if (exp == 2)
            {
                exp = 2046;
            }
            else if (exp == 3)
            {
                exp = 2047;
            }
            ptr_out[i] = prepare_double_no(sign_bit, exp, mant);
        }
        break;

    case INP_DATA_PAT_MANT_LIM_D:
        for (i = 0; i < len; ++i) {
            sign_bit = RANDOM_IGET32_NO[i] & 0x1;
            mant = RANDOM_IGET32_NO[i] % 4;
            exp = RANDOM_IGET32_NO[i] % 2048;
            if (mant == 2)
            {
                mant = 0x8000000000000LL;
            }
            else if (mant == 3)
            {
                mant = 0xfffffffffffffLL;
            }
            ptr_out[i] = prepare_double_no(sign_bit, exp, mant);
        }
        break;

    case INP_DATA_PAT_SUBNORMAL_D:
        exp = 0;
        for (i = 0; i < len; ++i) {
            sign_bit = rand() & 0x1;
            mant =(long long int)rand() >> 52;
            if (mant == 0)
            {
                mant = 0x1;
            }
            ptr_out[i] = prepare_double_no(sign_bit, exp, mant);
        }
        break;

    case INP_DATA_PAT_SPECIAL_D:
        for (i = 0; i < len; ++i) {
            val = RANDOM_M1_P1_double[i];
            ptr_out[i] = val;
        }
        if (len >= 27)
        {
            mant = 0x00; /* +0 */
            ptr_out[0] = *((double *)(&mant));
            mant = 0x8000000000000000LL; /* -0 */
            ptr_out[1] = *((double *)(&mant));

            ptr_out[2] = 1.0;
            ptr_out[3] = -1.0;

            mant = 0xfff0000000000000LL; /* -Inf */
            ptr_out[4] = *((double *)(&mant));
            mant = 0x7ff0000000000000LL; /* Inf */
            ptr_out[5] = *((double *)(&mant));

            mant = 0xfff0000000000005LL; /* -Nan */
            ptr_out[6] = *((double *)(&mant));
            mant = 0x7ff0000000000005LL; /* Nan */
            ptr_out[7] = *((double *)(&mant));

            mant = 0x8010000000000000LL; /* - smallest normalized no */
            ptr_out[8] = *((double *)(&mant));
            mant = 0x0010000000000000LL; /* + smallest normalized no */
            ptr_out[9] = *((double *)(&mant));

            mant = 0xffefffffffffffffLL; /* - largest normalized no */
            ptr_out[10] = *((double *)(&mant));
            mant = 0x7fefffffffffffffLL; /* + largest normalized no */
            ptr_out[11] = *((double *)(&mant));

            mant = 0x8000000000000001LL; /* - smallest denormalized no */
            ptr_out[12] = *((double *)(&mant));
            mant = 0x0000000000000001LL; /* + smallest denormalized no */
            ptr_out[13] = *((double *)(&mant));

            mant = 0x800fffffffffffffLL; /* - largest denormalized no */
            ptr_out[14] = *((double *)(&mant));
            mant = 0x000fffffffffffffLL; /* + largest denormalized no */
            ptr_out[15] = *((double *)(&mant));

            mant = 0x47154fc2;
            ptr_out[16] = (double)(*((float *)(&mant)));

            /* BF boundary conditions for sinf */
            ptr_out[17] = 100000.0;
            ptr_out[18] = 102930.0;
            ptr_out[19] = 102940.0;
            ptr_out[20] = 102950.0;
            ptr_out[21] = 110000.0;

            ptr_out[22] = -100000.0;
            ptr_out[23] = -102930.0;
            ptr_out[24] = -102940.0;
            ptr_out[25] = -102950.0;
            ptr_out[26] = -110000.0;
        }
        break;

    case INP_DATA_PAT_0_TO_P100000_D:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_double[i];
                if (val < 0.0f)
                {
                    ptr_out[i] = -val*100000.0;
                }
                else {
                    ptr_out[i] = val*100000.0;
                }
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_double(ptr_out[i]));
        }
        break;

    case INP_DATA_PAT_M100000_TO_0_D:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_double[i];
                if (val > 0.0f)
                {
                    ptr_out[i] = -val*100000.0;
                }
                else {
                    ptr_out[i] = val*100000.0;
                }
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_double(ptr_out[i]));
        }
        break;
    case INP_DATA_PAT_CUSTOM_D:
        printf("\n Error : Not Supported !! \n");
        break;

    case SWEEP_M1_P1_D:
    {
        double step = 1.96 / len;
        double start = -0.99;
        for (i = 0; i < len; ++i)
        {

#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_double_val(start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = start;
#endif /* #ifdef IDENTICAL_FLOAT_INP */

            start += step;

        }
        // Force middile point to zero
        ptr_out[len / 2] = (double)0;
    }
    break;

    case SWEEP_M1_P1_piecewise_D:
    {
        double step = 0.1 / (len / 4);
        double start = -0.9;
        for (i = 0; i < len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_double_val(start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = start;
#endif /* #ifdef IDENTICAL_FLOAT_INP */	
            start += step;
        }
        start = -0.1;
        for (i = len / 4; i < 2 * len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_double_val(start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = start;
#endif /* #ifdef IDENTICAL_FLOAT_INP */			
            start += step;
        }
        start = 0;
        for (i = 2 * len / 4; i < 3 * len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_double_val(start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = start;
#endif /* #ifdef IDENTICAL_FLOAT_INP */				
            start += step;
        }
        start = 0.9;
        for (i = 3 * len / 4; i < 4 * len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_double_val(start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = start;
#endif /* #ifdef IDENTICAL_FLOAT_INP */					
            start += step;
        }
        ptr_out[len / 2] = (double)0;
    }
    break;

    default:
        printf("Incorrect input data pattern for double inputs: Input pattern no. %d\n", pat);
        exit(0);
    }
}

double get_double_no(void)
{
    int sign_bit;
    double val;
    val = (double)rand() / (double)RAND_MAX;
    sign_bit = rand() & 0x1;
    if (sign_bit)
    {
        val = -val;
    }
    return(val);
}

double prepare_double_no(long long int sign_bit, long long int exp, long long int mant)
{
    long long int  val;
    double flt_val;
    val = mant & 0x000fffffffffffffLL;
    val = val | (exp << 52);
    val = val | (sign_bit << 63);
    flt_val = *((double *)&val);
    return(flt_val);
}

int check_special_double(double in)
{
    long long int e;
    long long int int_val;
    int_val = *((long long int *)&(in));
    e = ((int_val & 0x7ff0000000000000LL) >> 52);
    if ((e == 0x00) || (e == 0x00000000000007ffLL))
    {
        //      printf(" the number is  = %.25f\t%x\n",in,int_val); 
        return 0;
    }
    return 1;
}



/***********float number generation*****************************/

float get_float_no(void);
float prepare_float_no(int sign_bit, int exp, int mant);
int check_special_float(float in);

/* Generate floating point numbers within different ranges */
#ifdef IDENTICAL_FLOAT_INP
void initf(int pat, float *ptr_out, int len, int Q_format) {
#else /* #ifdef IDENTICAL_FLOAT_INP */
void initf(int pat, float *ptr_out, int len) {
#endif /* #ifdef IDENTICAL_FLOAT_INP */

    int i;
    int sign_bit;
    int exp;
    int mant;
    float val;
    int cnt;
    switch (pat)
    {
    case INP_DATA_PAT_MPI_TO_PPI_F:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_float[i];
                ptr_out[i] = val*22.0f / 7.0f;
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i]));
        }
        break;

    case  RANDOM_M05_P05_F:
    {
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_float[i] * 0.5f;
#ifdef IDENTICAL_FLOAT_INP
                ptr_out[i] = identical_float_val(val, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
                ptr_out[i] = val;
#endif /* #ifdef IDENTICAL_FLOAT_INP */

                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i]));
        }
    }
    break;


    case CUSTOM_INP_M05_P05_F:
        for (i = 0; i < len; ++i) {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)CUSTOM_DATA_M05_P05_F_nnpp[i], Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = CUSTOM_DATA_M05_P05_F_nnpp[i];
#endif /* #ifdef IDENTICAL_FLOAT_INP */
        }
        break;

    case CUSTOM_INP_M05_P05_F_N:
        for (i = 0; i < len; ++i) {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)CUSTOM_DATA_M05_P05_F_npnp[i], Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = CUSTOM_DATA_M05_P05_F_npnp[i];
#endif /* #ifdef IDENTICAL_FLOAT_INP */
        }
        break;		

    case RANDOM_M1_P1_F:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_float[i];
#ifdef IDENTICAL_FLOAT_INP
                ptr_out[i] = identical_float_val((float)val, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
                ptr_out[i] = val;
#endif /* #ifdef IDENTICAL_FLOAT_INP */

                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i]));
        }
        break;

    case CUSTOM_INP_M1_P1_F:
        for (i = 0; i < len; ++i) {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)CUSTOM_DATA_M1_P1_F[i], Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = CUSTOM_DATA_M1_P1_F[i];
#endif /* #ifdef IDENTICAL_FLOAT_INP */
        }
        break;

    case INP_DATA_PAT_M10_TO_P10_F:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_float[i];
                ptr_out[i] = val*10.0f;
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i]));
        }
        break;

    case INP_DATA_PAT_M100_TO_P100_F:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_float[i];
                ptr_out[i] = val*100.0f;
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i]));
        }
        break;

    case INP_DATA_PAT_M1000_TO_P1000_F:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_float[i];
                ptr_out[i] = val*1000.0f;
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i]));
        }
        break;

    case RANDOM_M10k_P10k_F:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_float[i];
                ptr_out[i] = val*10000.0f;
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i]));
        }
        break;

    case CUSTOM_INP_M10k_P10k_F:
        for (i = 0; i < len; ++i) {
            ptr_out[i] = CUSTOM_DATA_M10k_P10k_F[i];
        }
        break;

    case SWEEP_M10k_P10k_F:
    {
        double step = 1.96 / len;
        double start = -0.99;
        for (i = 0; i < len; ++i)
        {
            ptr_out[i] = (float)(start*10000.0f);
            start += step;
        }
        ptr_out[len / 2] = (float)0;
    }
    break;

    case SWEEP_M10k_P10k_piecewise_F:
    {
        double step = 0.1 / (len / 4);
        double start = -0.99;
        for (i = 0; i < len / 4; ++i)
        {
            ptr_out[i] = (float)(start*10000.0f);
            start += step;
        }
        start = -0.1;
        for (i = len / 4; i < 2 * len / 4; ++i)
        {
            ptr_out[i] = (float)(start*10000.0f);
            start += step;
        }
        start = 0;
        for (i = 2 * len / 4; i < 3 * len / 4; ++i)
        {
            ptr_out[i] = (float)(start*10000.0f);
            start += step;
        }
        start = 0.9;
        for (i = 3 * len / 4; i < 4 * len / 4; ++i)
        {
            ptr_out[i] = (float)(start*10000.0f);
            start += step;
        }
        ptr_out[len / 2] = (float)0;
    }
    break;

    case INP_DATA_PAT_M100000_TO_P100000_F:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_float[i];
                ptr_out[i] = val*100000.0f;
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i]));
        }
        break;

    case INP_DATA_PAT_RAND_F:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                sign_bit = RANDOM_IGET32_NO[i] & 0x1;
                mant = RANDOM_IGET32_NO[i] >> 9;
                exp = RANDOM_IGET32_NO[i] % 256;

                ptr_out[i] = prepare_float_no(sign_bit, exp, mant);
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i]));
        }
        break;

    case INP_DATA_PAT_EXP_LIM_F:
        for (i = 0; i < len; ++i) {
            sign_bit = RANDOM_IGET32_NO[i] & 0x1;
            mant = RANDOM_IGET32_NO[i] >> 9;
            exp = RANDOM_IGET32_NO[i] % 4;
            if (exp == 2)
            {
                exp = 254;
            }
            else if (exp == 3)
            {
                exp = 255;
            }
            ptr_out[i] = prepare_float_no(sign_bit, exp, mant);
        }
        break;

    case INP_DATA_PAT_MANT_LIM_F:
        for (i = 0; i < len; ++i) {
            sign_bit = RANDOM_IGET32_NO[i] & 0x1;
            mant = RANDOM_IGET32_NO[i] % 4;
            exp = RANDOM_IGET32_NO[i] % 256;
            if (mant == 2)
            {
                mant = 0x400000;
            }
            else if (mant == 3)
            {
                mant = 0x7fffff;
            }
            ptr_out[i] = prepare_float_no(sign_bit, exp, mant);
        }
        break;

    case INP_DATA_PAT_SUBNORMAL_F:
        exp = 0;
        for (i = 0; i < len; ++i) {
            sign_bit = RANDOM_IGET32_NO[i] & 0x1;
            mant = RANDOM_IGET32_NO[i] >> 23;
            if (mant == 0)
            {
                mant = 0x1;
            }

            ptr_out[i] = prepare_float_no(sign_bit, exp, mant);
        }
        break;

    case INP_DATA_PAT_SPECIAL_F:
        for (i = 0; i < len; ++i) {
            val = RANDOM_M1_P1_float[i];
            ptr_out[i] = val;
        }
        if (len >= 27) {
            mant = 0x00; /* +0 */
            ptr_out[0] = *((float *)(&mant));
            mant = 0x80000000; /* -0 */
            ptr_out[1] = *((float *)(&mant));

            ptr_out[2] = 1.0f;
            ptr_out[3] = -1.0f;

            mant = 0xff800000; /* -Inf */
            ptr_out[4] = *((float *)(&mant));
            mant = 0x7f800000; /* Inf */
            ptr_out[5] = *((float *)(&mant));

            mant = 0xff800005; /* -Nan */
            ptr_out[6] = *((float *)(&mant));
            mant = 0x7f800005; /* Nan */
            ptr_out[7] = *((float *)(&mant));

            mant = 0x80800000; /* - smallest normalized no */
            ptr_out[8] = *((float *)(&mant));
            mant = 0x00800000; /* + smallest normalized no */
            ptr_out[9] = *((float *)(&mant));

            mant = 0xff7fffff; /* - largest normalized no */
            ptr_out[10] = *((float *)(&mant));
            mant = 0x7f7ffffF; /* + largest normalized no */
            ptr_out[11] = *((float *)(&mant));

            mant = 0x80000001; /* - smallest denormalized no */
            ptr_out[12] = *((float *)(&mant));
            mant = 0x00000001; /* + smallest denormalized no */
            ptr_out[13] = *((float *)(&mant));

            mant = 0x807fffff; /* - largest denormalized no */
            ptr_out[14] = *((float *)(&mant));
            mant = 0x007fffff; /* + largest denormalized no */
            ptr_out[15] = *((float *)(&mant));

            mant = 0x47154fc2;
            ptr_out[16] = *((float *)(&mant));

            /* BF boundary conditions for sinf */
            ptr_out[17] = 100000.0f;
            ptr_out[18] = 102930.0f;
            ptr_out[19] = 102940.0f;
            ptr_out[20] = 102950.0f;
            ptr_out[21] = 110000.0f;

            ptr_out[22] = -100000.0f;
            ptr_out[23] = -102930.0f;
            ptr_out[24] = -102940.0f;
            ptr_out[25] = -102950.0f;
            ptr_out[26] = -110000.0f;
        }
        break;

    case INP_DATA_PAT_0_TO_P100000_F:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_float[i];
                if (val < 0.0f)
                {
                    ptr_out[i] = -val*100000.0f;
                }
                else {
                    ptr_out[i] = val*100000.0f;
                }
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i]));
        }
        break;

    case INP_DATA_PAT_M100000_TO_0_F:
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_float[i];
                if (val > 0.0f)
                {
                    ptr_out[i] = -val*100000.0f;
                }
                else {
                    ptr_out[i] = val*100000.0f;
                }
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i] = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i]));
        }
        break;
    case INP_DATA_PAT_CUSTOM_F:
        printf("\n Error : Not Supported !! \n");
        break;

    case SWEEP_M05_P05_F:
    {

        double step = (double)1.0 / len;
        double start = -0.5;
        for (i = 0; i < len; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = (float)start;
#endif /* #ifdef IDENTICAL_FLOAT_INP */
            start += step;
        }
        ptr_out[len / 2] = (float)0;
    }
    break;

    case SWEEP_M05_P05_piecewise_F:
    {
        double step = (double)0.05 / (len / 4);
        double start = -0.5;
        for (i = 0; i < (int)len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = (float)start;
#endif  /* #ifdef IDENTICAL_FLOAT_INP */

            start += step;
        }
        start = -0.05;
        for (i = len / 4; i < (int)2 * len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = (float)start;
#endif  /* #ifdef IDENTICAL_FLOAT_INP */
            start += step;
        }
        start = 0.0;
        for (i = (int)2 * len / 4; i < (int)3 * len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = (float)start;
#endif  /* #ifdef IDENTICAL_FLOAT_INP */
            start += step;
        }
        start = 0.45;
        for (i = (int)3 * len / 4; i < len; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = (float)start;
#endif  /* #ifdef IDENTICAL_FLOAT_INP */
            start += step;
        }
        ptr_out[len / 2] = (float)0;
    }
    break;

    case SWEEP_M1_P1_F:
    {

        double step = 1.96 / len;
        double start = -0.99;
        for (i = 0; i < len; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = (float)start;
#endif /* #ifdef IDENTICAL_FLOAT_INP */

            start += step;
        }
        ptr_out[len / 2] = (float)0;
    }
    break;

    case SWEEP_M1_P1_piecewise_F:
    {

        double step = 0.1 / (len / 4);
        double start = (float)(-0.99);
        for (i = 0; i < len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = (float)start;
#endif /* #ifdef IDENTICAL_FLOAT_INP */
            start += step;
        }
        start = (float)(-0.1);
        for (i = len / 4; i < 2 * len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = (float)start;
#endif /* #ifdef IDENTICAL_FLOAT_INP */			
            start += step;
        }
        start = (float)(0);
        for (i = 2 * len / 4; i < 3 * len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = (float)start;
#endif /* #ifdef IDENTICAL_FLOAT_INP */				
            start += step;
        }
        start = (float)(0.9);
        for (i = 3 * len / 4; i < 4 * len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i] = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i] = (float)start;
#endif /* #ifdef IDENTICAL_FLOAT_INP */			
            start += step;
        }
        ptr_out[len / 2] = (float)0;
    }
    break;


    default:
        printf("Incorrect input data pattern for float inputs: Input pat no. %d\n", pat);
    }
}

#ifdef IDENTICAL_FLOAT_INP
void initfc(int pat, f_cmplx *ptr_out, int len, int Q_format) {
#else /* #ifdef IDENTICAL_FLOAT_INP */
void initfc(int pat, f_cmplx *ptr_out, int len) {
#endif /* #ifdef IDENTICAL_FLOAT_INP */

    int i;
    float val;
    int cnt;
    double step, start;
    switch (pat)
    {
    case SWEEP_M05_P05_F:
    {

        step = (double)1.0 / len;
        start = -0.5;
        for (i = 0; i < len; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i].re = identical_float_val((float)start, Q_format);
            ptr_out[i].im = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i].re = (float)start;
            ptr_out[i].im = (float)start;
#endif /* #ifdef IDENTICAL_FLOAT_INP */
            start += step;
        }
        ptr_out[len / 2].re = (float)0;
        ptr_out[len / 2].im = (float)0;
    }
    break;
    case SWEEP_M05_P05_piecewise_F:
    {
        step = (double)0.05 / (len / 4);
        start = -0.5;
        for (i = 0; i < 1 * len / 4; ++i)
        {

#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i].re = identical_float_val((float)start, Q_format);
            ptr_out[i].im = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i].re = (float)start;
            ptr_out[i].im = (float)start;
#endif  /* #ifdef IDENTICAL_FLOAT_INP */
            start += step;
        }
        start = -0.05;
        for (i = len / 4; i < 2 * len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i].re = identical_float_val((float)start, Q_format);
            ptr_out[i].im = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i].re = (float)start;
            ptr_out[i].im = (float)start;
#endif  /* #ifdef IDENTICAL_FLOAT_INP */       
            start += step;
        }
        start = 0.0;
        for (i = 2 * len / 4; i < 3 * len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i].re = identical_float_val((float)start, Q_format);
            ptr_out[i].im = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i].re = (float)start;
            ptr_out[i].im = (float)start;
#endif  /* #ifdef IDENTICAL_FLOAT_INP */       
            start += step;
        }
        start = 0.45;
        for (i = 3 * len / 4; i < 4 * len / 4; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i].re = identical_float_val((float)start, Q_format);
            ptr_out[i].im = identical_float_val((float)start, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i].re = (float)start;
            ptr_out[i].im = (float)start;
#endif  /* #ifdef IDENTICAL_FLOAT_INP */      
            start += step;
        }
        ptr_out[len / 2].re = (float)0;
        ptr_out[len / 2].im = (float)0;
    }
    break;
    case  RANDOM_M05_P05_F:
    {
        for (i = 0; i < len; ++i) {
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_float[i] * 0.5f;
#ifdef IDENTICAL_FLOAT_INP
                ptr_out[i].re = identical_float_val(val, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
                ptr_out[i].re = val;
#endif /* #ifdef IDENTICAL_FLOAT_INP */
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i].re = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i].re));
            cnt = 0;
            do
            {
                val = RANDOM_M1_P1_float[i] * 0.5f;
#ifdef IDENTICAL_FLOAT_INP
                ptr_out[i].im = identical_float_val(val, Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
                ptr_out[i].im = val;
#endif /* #ifdef IDENTICAL_FLOAT_INP */
                if (cnt > MAX_TRIES)
                {
                    ptr_out[i].im = 0.0f;
                    printf("max number of tries to suppress special nos reached\n");
                    break;
                }
                cnt++;
            } while (0 == check_special_float(ptr_out[i].im));
        }
    }
    break;

    case CUSTOM_INP_M05_P05_F:
        for (i = 0; i < len; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i].re = identical_float_val((float)CUSTOM_DATA_M05_P05_F_nnpp[i], Q_format);
            ptr_out[i].im = identical_float_val((float)CUSTOM_DATA_M05_P05_F_nnpp[i], Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i].re = (float)CUSTOM_DATA_M05_P05_F_nnpp[i];
            ptr_out[i].im = (float)CUSTOM_DATA_M05_P05_F_nnpp[i];
#endif /* #ifdef IDENTICAL_FLOAT_INP */
        }
        break;
		
	    case CUSTOM_INP_M05_P05_F_N:
        for (i = 0; i < len; ++i)
        {
#ifdef IDENTICAL_FLOAT_INP
            ptr_out[i].re = identical_float_val((float)CUSTOM_DATA_M05_P05_F_npnp[i], Q_format);
            ptr_out[i].im = identical_float_val((float)CUSTOM_DATA_M05_P05_F_npnp[i], Q_format);
#else /* #ifdef IDENTICAL_FLOAT_INP */
            ptr_out[i].re = (float)CUSTOM_DATA_M05_P05_F_npnp[i];
            ptr_out[i].im = (float)CUSTOM_DATA_M05_P05_F_npnp[i];
#endif /* #ifdef IDENTICAL_FLOAT_INP */
        }
        break;	

    default:
        printf("Incorrect input data pattern for float inputs: Input pat no. %d\n", pat);
    }
}
float get_float_no(void)
{
    int sign_bit;
    float val;

    val = (float)((double)rand() / RAND_MAX);

    sign_bit = rand() & 0x1;
    if (sign_bit)
    {
        val = -val;
    }
    return(val);
}

float prepare_float_no(int sign_bit, int exp, int mant)
{
    int  val;
    float flt_val;

    val = mant & 0x7fffff;
    val = val | (exp << 23);
    val = val | (sign_bit << 31);
    flt_val = *((float *)&val);
    return(flt_val);
}

int check_special_float(float in)
{
    int e;
    int int_val;
    int_val = *((int *)&(in));
    e = ((int_val & 0x7f800000) >> 23);
    if ((e == 0x0000000) || (e == 0x00000ff))
    {
        //      printf(" the number is  = %.25f\t%x\n",in,int_val); 
        return 0;
    }
    return 1;
}

int get_i32_no(void)
{
    int sign_bit;
	int val1,val2;
    int val;

	val1 = rand();
	val2 = rand();

	val = (val1 & 0xFFFF) | ((val2 & 0xFFFF) << 16);

    sign_bit = rand() & 0x1;
    if (sign_bit)
    {
        val = -val;
    }
    return(val);
}


/*
long long int get_i64_no(void)
{
    long long sign_bit;
    long long val;

    srand(time(0));
	val = ((long long)RANDOM_IGET32_NO[i]<< 32) | ((long long)RANDOM_IGET32_NO[i]& 0xffffffff);

    sign_bit = rand() & 0x1;
    if (sign_bit)
    {
        val = -val;
    }
    return(val);
}*/
void initi16(int pat, void *ptr_out, int len) {
    int i;
    short int val;
    
    switch (pat)
    {
    case RANDOM_M32768_P32767_I:
    {
		short int* pOut = (short int *)ptr_out;
        for (i = 0; i < len; ++i) {
            val = (short int)(RANDOM_IGET32_NO[i]>>16);
            pOut[i] = val;
        }
    }
    break;

    case CUSTOM_INP_M32768_P32767_I:
    {
		short int* pOut = (short int *)ptr_out;
        for (i = 0; i < len; ++i) {
			pOut[i] = CUSTOM_DATA_M32768_P32767_S_I[i];
        }
    }
    break;

    case CUSTOM_INP_M32768_P32767_I_N:
    {
        short int* pOut = (short int *)ptr_out;
        for (i = 0; i < len; ++i) {
            pOut[i] = CUSTOM_DATA_M32768_P32767_npnp[i];
        }
    }
    break;


    case SWEEP_M32768_P32767_I:
    {
		short int step = 32767 * 2 / len;
		short int start = -32768;
		short int* pOut = (short int *)ptr_out;
        for (i = 0; i < len; ++i)
        {
            pOut[i] = (short int)start;
            start += step;
        }
        pOut[len / 2] = 0;
    }
    break;

    case SWEEP_M32768_P32767_piecewise_I:
    {
		short int step = (32767/10) / (len / 4);
		short int start = -32768;
		short int* pOut = (short int *)ptr_out;
        for (i = 0; i < len / 4; ++i)
        {
			pOut[i] = start;
            start += step;
        }
		start = (short int)(-32768 / 10);
        for (i = len / 4; i < 2 * len / 4; ++i)
        {
			pOut[i] = start;
            start += step;
        }
        start = 0;
        for (i = 2 * len / 4; i < 3 * len / 4; ++i)
        {
            pOut[i] = start;
            start += step;
        }
        start = (short int)(9 * 32768 / 10);;
        for (i = 3 * len / 4; i < 4 * len / 4; ++i)
        {
            pOut[i] = start;
            start += step;
        }
        pOut[len / 2] = 0;
    }
    break;

    case RANDOM_SHIFT:
    {
        short int* pOut = (short int*)ptr_out;
        for (i = 0; i < len; ++i) {
            int val32 = RANDOM_IGET32_NO[i];
            pOut[i] = (short )((val32 << 27) >> 27); //  take only last 5 bits
        }
    }
    break;
    default:
        printf("Incorrect input data pattern for int16: Input pat no. %d\n", pat);
    }
}

void initi32(int pat, void *ptr_out, int len) {
    int i;
    int val;
    
    switch (pat)
    {
    case RANDOM_M32768_P32767_I:
    {
        int* pOut = (int *)ptr_out;
        for (i = 0; i < len; ++i) {
            val = RANDOM_IGET32_NO[i];
            pOut[i] = val;
        }
    }
    break;

    case CUSTOM_INP_M32768_P32767_I:
    {
        int* pOut = (int *)ptr_out;
        for (i = 0; i < len; ++i) {
            pOut[i] = CUSTOM_DATA_M2147483648_P2147483647_nnpp[i];
        }
    }
    break;
	
	case CUSTOM_INP_M32768_P32767_I_N:
    {
        int* pOut = (int *)ptr_out;
        for (i = 0; i < len; ++i) {
            pOut[i] = CUSTOM_DATA_M2147483648_P2147483647_npnp[i];
        }
    }
    break;
    case SWEEP_M32768_P32767_I:
    {
        long step = (int)((long long)2147483647*2 / len);
        long start = -(2147483648LL);
        int* pOut = (int *)ptr_out;
        for (i = 0; i < len; ++i)
        {
            pOut[i] = (int)start;
            start += step;
        }
        pOut[len / 2] = 0;
    }
    break;

    case SWEEP_M32768_P32767_piecewise_I:
    {
        int step = (int)((long long)(2147483647/10) / (len / 4));
        int start = -2147483648LL;
        int* pOut = (int *)ptr_out;
        for (i = 0; i < len / 4; ++i)
        {
            pOut[i] = (int)start;
            start += step;
        }
        start = (int)(-2147483648LL / 10);
        for (i = len / 4; i < 2 * len / 4; ++i)
        {
            pOut[i] = (int)start;
            start += step;
        }
        start = 0;
        for (i = 2 * len / 4; i < 3 * len / 4; ++i)
        {
            pOut[i] = (int)start;
            start += step;
        }
        start = (int)(1 * 2147483648LL *9/ 10);;
        for (i = 3 * len / 4; i < 4 * len / 4; ++i)
        {
            pOut[i] = (int)start;
            start += step;
        }
        pOut[len / 2] = 0;
    }
    break;

    case RANDOM_SHIFT:
    {
        short* pOut = (short *)ptr_out;
        for (i = 0; i < len; ++i) {
            val = RANDOM_IGET32_NO[i];
            pOut[i] = (val << 27) >> 27; //  take only last 5 bits
        }
    }
    break;
    default:
        printf("Incorrect input data pattern for int32: Input pat no. %d\n", pat);
    }
}

void initi64(int pat, void *ptr_out, int len) {
	int i;
	long long int val;

	switch (pat)
	{
	case RANDOM_M32768_P32767_I:
	{
		long long int* pOut = (long long int *)ptr_out;
		for (i = 0; i < len; ++i) {
			val = RANDOM_IGET64_NO[i];
			pOut[i] = val;
		}
	}
	break;

	case CUSTOM_INP_M32768_P32767_I:
	{
		long long int* pOut = (long long int *)ptr_out;
		for (i = 0; i < len; ++i) {
			pOut[i] = CUSTOM_DATA_MQ63_PQ3_nnpp[i];
		}
	}
	break;

	case CUSTOM_INP_M32768_P32767_I_N:
	{
		long long int* pOut = (long long int *)ptr_out;
		for (i = 0; i < len; ++i) {
			pOut[i] = CUSTOM_DATA_MQ63_PQ3_npnp[i];
		}
	}
	break;
	case SWEEP_M32768_P32767_I:
	{
		long long int step = (9223372036854775807LL/len) * 2 ;
		//long long int start = (long long int)-9223372036854775808LL;
		long long int start = 0x8000000000000000LL;
		long long int* pOut = (long long int *)ptr_out;
		for (i = 0; i < len; ++i)
		{
			pOut[i] = start;
			start += step;
		}
		pOut[len / 2] = 0;
	}
	break;

	case SWEEP_M32768_P32767_piecewise_I:
	{
		long long int step = (9223372036854775807LL/10) / (len / 4);
		long long int start = 0x8000000000000000LL;
		long long int* pOut = (long long int *)ptr_out;
		for (i = 0; i < len / 4; ++i)
		{
			pOut[i] = start;
			start += step;
		}
		start = ((long long int)-9223372036854775807 / 10);
		for (i = len / 4; i < 2 * len / 4; ++i)
		{
			pOut[i] = start;
			start += step;
		}
		start = 0;
		for (i = 2 * len / 4; i < 3 * len / 4; ++i)
		{
			pOut[i] = start;
			start += step;
		}
		start = ( (long long int )9 * (9223372036854775807 / 10));
		for (i = 3 * len / 4; i < 4 * len / 4; ++i)
		{
			pOut[i] = start;
			start += step;
		}
		pOut[len / 2] = 0;
	}
	break;

	default:
		printf("Incorrect input data pattern for int64: Input pat no. %d\n", pat);
	}
}

void get_data_pat(int cnt, char *ptr_name)
{
    switch (cnt)
    {
    case INP_DATA_PAT_MPI_TO_PPI_F:
        strcpy(ptr_name, "INP_DATA_PAT_MPI_TO_PPI_F");
        break;
    case SWEEP_M05_P05_F:
        strcpy(ptr_name, "SWEEP_M05_P05_F");
        break;
    case SWEEP_M05_P05_piecewise_F:
        strcpy(ptr_name, "SWEEP_M05_P05_piecewise_F");
        break;
    case SWEEP_M1_P1_D:
        strcpy(ptr_name, "SWEEP_M1_P1_D");
        break;
    case SWEEP_M1_P1_piecewise_D:
        strcpy(ptr_name, "SWEEP_M1_P1_piecewise_D");
        break;
    case SWEEP_M1_P1_F:
        strcpy(ptr_name, "SWEEP_M1_P1_F");
        break;
    case SWEEP_M1_P1_piecewise_F:
        strcpy(ptr_name, "SWEEP_M1_P1_piecewise_F");
        break;
    case RANDOM_M1_P1_F:
        strcpy(ptr_name, "RANDOM_M1_P1_F");
        break;

    case RANDOM_M05_P05_F:
        strcpy(ptr_name, "RANDOM_M05_P05_F");
        break;

    case INP_DATA_PAT_M10_TO_P10_F:
        strcpy(ptr_name, "INP_DATA_PAT_M10_TO_P10_F");
        break;
    case INP_DATA_PAT_M100_TO_P100_F:
        strcpy(ptr_name, "INP_DATA_PAT_M100_TO_P100_F");
        break;
    case INP_DATA_PAT_M1000_TO_P1000_F:
        strcpy(ptr_name, "INP_DATA_PAT_M1000_TO_P1000_F");
        break;
    case RANDOM_M10k_P10k_F:
        strcpy(ptr_name, "RANDOM_M10k_P10k_F");
        break;
    case SWEEP_M10k_P10k_F:
        strcpy(ptr_name, "SWEEP_M10k_P10k_F");
        break;
    case SWEEP_M10k_P10k_piecewise_F:
        strcpy(ptr_name, "SWEEP_M10k_P10k_piecewise_F ");
        break;
    case SWEEP_M10k_P10k_D:
        strcpy(ptr_name, "SWEEP_M10k_P10k_D");
        break;
    case SWEEP_M10k_P10k_piecewise_D:
        strcpy(ptr_name, "SWEEP_M10k_P10k_piecewise_D");
        break;
    case INP_DATA_PAT_M100000_TO_P100000_F:
        strcpy(ptr_name, "INP_DATA_PAT_M100000_TO_P100000_F");
        break;
    case INP_DATA_PAT_RAND_F:
        strcpy(ptr_name, "INP_DATA_PAT_RAND_F");
        break;
    case INP_DATA_PAT_EXP_LIM_F:
        strcpy(ptr_name, "INP_DATA_PAT_EXP_LIM_F");
        break;
    case INP_DATA_PAT_MANT_LIM_F:
        strcpy(ptr_name, "INP_DATA_PAT_MANT_LIM_F");
        break;
    case INP_DATA_PAT_SUBNORMAL_F:
        strcpy(ptr_name, "INP_DATA_PAT_SUBNORMAL_F");
        break;
    case INP_DATA_PAT_SPECIAL_F:
        strcpy(ptr_name, "INP_DATA_PAT_SPECIAL_F");
        break;
    case INP_DATA_PAT_0_TO_P100000_F:
        strcpy(ptr_name, "INP_DATA_PAT_0_TO_P100000_F");
        break;
    case INP_DATA_PAT_M100000_TO_0_F:
        strcpy(ptr_name, "INP_DATA_PAT_M100000_TO_0_F");
        break;
    case INP_DATA_PAT_CUSTOM_F:
        strcpy(ptr_name, "INP_DATA_PAT_CUSTOM_F");
        break;
    case INP_DATA_PAT_MPI_TO_PPI_D:
        strcpy(ptr_name, "INP_DATA_PAT_MPI_TO_PPI_D");
        break;
    case RANDOM_M1_P1_D:
        strcpy(ptr_name, "RANDOM_M1_P1_D");
        break;
    case INP_DATA_PAT_M10_TO_P10_D:
        strcpy(ptr_name, "INP_DATA_PAT_M10_TO_P10_D");
        break;
    case INP_DATA_PAT_M100_TO_P100_D:
        strcpy(ptr_name, "INP_DATA_PAT_M100_TO_P100_D");
        break;
    case INP_DATA_PAT_M1000_TO_P1000_D:
        strcpy(ptr_name, "INP_DATA_PAT_M1000_TO_P1000_D");
        break;
    case RANDOM_M10k_P10k_D:
        strcpy(ptr_name, "RANDOM_M10k_P10k_D");
        break;
    case INP_DATA_PAT_M100000_TO_P100000_D:
        strcpy(ptr_name, "INP_DATA_PAT_M100000_TO_P100000_D");
        break;
    case INP_DATA_PAT_RAND_D:
        strcpy(ptr_name, "INP_DATA_PAT_RAND_D");
        break;
    case INP_DATA_PAT_EXP_LIM_D:
        strcpy(ptr_name, "INP_DATA_PAT_EXP_LIM_D");
        break;
    case INP_DATA_PAT_MANT_LIM_D:
        strcpy(ptr_name, "INP_DATA_PAT_MANT_LIM_D");
        break;
    case INP_DATA_PAT_SUBNORMAL_D:
        strcpy(ptr_name, "INP_DATA_PAT_SUBNORMAL_D");
        break;
    case INP_DATA_PAT_SPECIAL_D:
        strcpy(ptr_name, "INP_DATA_PAT_SPECIAL_D");
        break;
    case INP_DATA_PAT_0_TO_P100000_D:
        strcpy(ptr_name, "INP_DATA_PAT_0_TO_P100000_D");
        break;
    case INP_DATA_PAT_M100000_TO_0_D:
        strcpy(ptr_name, "INP_DATA_PAT_M100000_TO_0_D");
        break;
    case INP_DATA_PAT_CUSTOM_D:
        strcpy(ptr_name, "INP_DATA_PAT_CUSTOM_D");
        break;
    case RANDOM_M32768_P32767_I:
        strcpy(ptr_name, "RANDOM_M32768_P32767_I");
        break;
    case SWEEP_M32768_P32767_I:
        strcpy(ptr_name, "SWEEP_M32768_P32767_I");
        break;
    case SWEEP_M32768_P32767_piecewise_I:
        strcpy(ptr_name, "SWEEP_M32768_P32767_piecewise_I");
        break;
    case RANDOM_SHIFT:
        strcpy(ptr_name, "RANDOM_SHIFT");
        break;
    case NA:
        strcpy(ptr_name, "NO_INPUT");
        break;
    case CUSTOM_INP_M05_P05_F:
        strcpy(ptr_name, "CUSTOM_INP_M05_P05_F");
        break;
    case CUSTOM_INP_M05_P05_F_N:
        strcpy(ptr_name, "CUSTOM_INP_M05_P05_F_N");
        break;
    case CUSTOM_INP_M1_P1_F:
        strcpy(ptr_name, "CUSTOM_INP_M1_P1_F");
        break;
    case CUSTOM_INP_M1_P1_D:
        strcpy(ptr_name, "CUSTOM_INP_M1_P1_D");
        break;
    case CUSTOM_INP_M1_P1_D_N:
        strcpy(ptr_name, "CUSTOM_INP_M1_P1_D_N");
        break;		
    case CUSTOM_INP_M32768_P32767_I:
        strcpy(ptr_name, "CUSTOM_INP_M32768_P32767_I");
        break;
    case CUSTOM_INP_M32768_P32767_I_N:
        strcpy(ptr_name, "CUSTOM_INP_M32768_P32767_I_N");
        break;		    case CUSTOM_INP_M10k_P10k_F:
        strcpy(ptr_name, "CUSTOM_INP_M10k_P10k_F");
        break;
    case CUSTOM_INP_M10k_P10k_D:
        strcpy(ptr_name, "CUSTOM_INP_M10k_P10k_D");
        break;
    default:
        printf("ERROR!!! Non recognizable input pattern in get_data_pat()\nFor no input, use NA");
        exit(0);
    }
}


/**************exception data inputs *************************************/
void init_exp_f(float *ptr_out, char inp_name[][100]) {
    int val;

    val = 0x00; /* +0 */
    ptr_out[0] = *((float *)(&val));
    strcpy(inp_name[0], "+zero");
    val = 0x80000000; /* -0 */
    ptr_out[1] = *((float *)(&val));
    strcpy(inp_name[1], "-zero");

    ptr_out[2] = 1.0f;
    strcpy(inp_name[2], "+one");
    ptr_out[3] = -1.0f;
    strcpy(inp_name[3], "-one");

    val = 0x7f800000; /* +Inf */
    ptr_out[4] = *((float *)(&val));
    strcpy(inp_name[4], "+Inf");
    val = 0xff800000; /* -Inf */
    ptr_out[5] = *((float *)(&val));
    strcpy(inp_name[5], "-Inf");

    val = 0x7fc00005; /* Nan */
    ptr_out[6] = *((float *)(&val));
    strcpy(inp_name[6], "Nan");
    val = 0xffc00005; /* -Nan */
    ptr_out[7] = *((float *)(&val));
    strcpy(inp_name[7], "-Nan");

    val = 0x7fc00000; /* qNan */
    ptr_out[8] = *((float *)(&val));
    strcpy(inp_name[8], "qNan");
    val = 0x7f800001; /* sNan */
    ptr_out[9] = *((float *)(&val));
    strcpy(inp_name[9], "sNan");

    val = 0x00800000; /* + smallest normalized no */
    ptr_out[10] = *((float *)(&val));
    strcpy(inp_name[10], "+smallest_norm");
    val = 0x80800000; /* - smallest normalized no */
    ptr_out[11] = *((float *)(&val));
    strcpy(inp_name[11], "-smallest_norm");

    val = 0x7f7fffff; /* + largest normalized no */
    ptr_out[12] = *((float *)(&val));
    strcpy(inp_name[12], "+largest_norm");
    val = 0xff7ffffF; /* - largest normalized no */
    ptr_out[13] = *((float *)(&val));
    strcpy(inp_name[13], "-largest_norm");

    val = 0x00000001; /* + smallest denormalized no */
    ptr_out[14] = *((float *)(&val));
    strcpy(inp_name[14], "+smallest_denorm");
    val = 0x80000001; /* - smallest denormalized no */
    ptr_out[15] = *((float *)(&val));
    strcpy(inp_name[15], "-smallest_denorm");

    val = 0x007fffff; /* + largest denormalized no */
    ptr_out[16] = *((float *)(&val));
    strcpy(inp_name[16], "+largest_denorm");
    val = 0x807fffff; /* - largest denormalized no */
    ptr_out[17] = *((float *)(&val));
    strcpy(inp_name[17], "-largest_denorm");

}

void init_exp_d(double *ptr_out, char inp_name[][100]) {
    long long int val;

    val = 0x00LL; /* +0 */
    ptr_out[0] = *((double *)(&val));
    strcpy(inp_name[0], "+zero");
    val = 0x8000000000000000LL; /* -0 */
    ptr_out[1] = *((double *)(&val));
    strcpy(inp_name[1], "-zero");

    ptr_out[2] = 1.0f;
    strcpy(inp_name[2], "+one");
    ptr_out[3] = -1.0f;
    strcpy(inp_name[3], "-one");

    val = 0x7ff0000000000000LL; /* Inf */
    ptr_out[4] = *((double *)(&val));
    strcpy(inp_name[4], "+Inf");
    val = 0xfff0000000000000LL; /* -Inf */
    ptr_out[5] = *((double *)(&val));
    strcpy(inp_name[5], "-Inf");

    val = 0x7ff8000000000005LL; /* Nan */
    ptr_out[6] = *((double *)(&val));
    strcpy(inp_name[6], "Nan");
    val = 0xfff8000000000005LL; /* -Nan */
    ptr_out[7] = *((double *)(&val));
    strcpy(inp_name[7], "-Nan");

    val = 0x7ff8000000000001LL; /* qNan */
    ptr_out[8] = *((double *)(&val));
    strcpy(inp_name[8], "qNan");
    val = 0x7ff0000000000001LL; /* sNan */
    ptr_out[9] = *((double *)(&val));
    strcpy(inp_name[9], "sNan");

    val = 0x0010000000000000LL; /* + smallest normalized no */
    ptr_out[10] = *((double *)(&val));
    strcpy(inp_name[10], "+smallest_norm");
    val = 0x8010000000000000LL; /* - smallest normalized no */
    ptr_out[11] = *((double *)(&val));
    strcpy(inp_name[11], "-smallest_norm");

    val = 0x7fefffffffffffffLL; /* + largest normalized no */
    ptr_out[12] = *((double *)(&val));
    strcpy(inp_name[12], "+largest_norm");
    val = 0xffefffffffffffffLL; /* - largest normalized no */
    ptr_out[13] = *((double *)(&val));
    strcpy(inp_name[13], "-largest_norm");

    val = 0x0000000000000001LL; /* + smallest denormalized no */
    ptr_out[14] = *((double *)(&val));
    strcpy(inp_name[14], "+smallest_denorm");
    val = 0x8000000000000001LL; /* + smallest denormalized no */
    ptr_out[15] = *((double *)(&val));
    strcpy(inp_name[15], "-smallest_denorm");

    val = 0x000fffffffffffffLL; /* + largest denormalized no */
    ptr_out[16] = *((double *)(&val));
    strcpy(inp_name[16], "+largest_denorm");
    val = 0x800fffffffffffffLL; /* - largest denormalized no */
    ptr_out[17] = *((double *)(&val));
    strcpy(inp_name[17], "-largest_denorm");

}

void init_exp_i(int *ptr_out, char inp_name[][100]) {
    int val;

    val = 0; /* +0 */
    ptr_out[0] = val;
    strcpy(inp_name[0], "+zero");
    val = -0; /* -0 */
    ptr_out[1] = val;
    strcpy(inp_name[1], "-zero");

    ptr_out[2] = 1;
    strcpy(inp_name[2], "+one");
    ptr_out[3] = -1;
    strcpy(inp_name[3], "-one");

    val = RAND_MAX; /* max*/
    ptr_out[4] = val;
    strcpy(inp_name[4], "+RAND_MAX");
    val = -RAND_MAX; /* min */
    ptr_out[5] = val;
    strcpy(inp_name[5], "-RAND_MAX");

    val = rand();
    ptr_out[6] = val;
    strcpy(inp_name[6], "random");
    val = rand();
    ptr_out[7] = val;
    strcpy(inp_name[7], "random");

    val = rand();
    ptr_out[8] = val;
    strcpy(inp_name[8], "random");
    val = rand();
    ptr_out[9] = val;
    strcpy(inp_name[9], "random");

    val = rand();
    ptr_out[10] = val;
    strcpy(inp_name[10], "random");
    val = rand();
    ptr_out[11] = val;
    strcpy(inp_name[11], "random");

    val = rand();
    ptr_out[12] = val;
    strcpy(inp_name[12], "random");
    val = rand();
    ptr_out[13] = val;
    strcpy(inp_name[13], "random");

    val = rand();
    ptr_out[14] = val;
    strcpy(inp_name[14], "random");
    val = rand();
    ptr_out[15] = val;
    strcpy(inp_name[15], "random");

    val = rand();
    ptr_out[16] = val;
    strcpy(inp_name[16], "random");
    val = rand();
    ptr_out[17] = val;
    strcpy(inp_name[17], "random");

}
/* End of file */
