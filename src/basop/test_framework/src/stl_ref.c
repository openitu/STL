#include "func_proto.h"

/*________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_mac_16_16_ref                                                               |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply 2 float type varaibles var1 by var2 and add the result to double type variable D_acc |
|	 return a double variable result.											    			  |
|                                                                                                 |
|                                                                                                 |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    D_acc                                                                                        |
|			   Double variable                                                                    |
|                                                                                                 |
|    var1                                                                                         |
|			   float variable                                                                     |
|                                                                                                 |
|    var2                                                                                         |
|			   float variable                                                                     |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    D_var_out                                                                                    |
|			   Double variable                                                                    |
|_________________________________________________________________________________________________|
*/
double W_mac_16_16_ref(double  D_acc, float var1, float var2)
{
    double  D_var_out = ((double)var1*var2) ;
    D_acc = D_acc + D_var_out;

    return D_acc;
}


/*________________________________________________________________________________________________
|                                                                                                 |
|   Function Name : W_msu_16_16_ref                                                               |
|                                                                                                 |
|   Purpose :                                                                                     |
|                                                                                                 |
|   Multiply 2 float type varaibles var1 by var2 and subtract the result from double type variable D_acc |
|	 return a double variable result.											    			  |
|                                                                                                 |
|                                                                                                 |
|                                                                                                 |
|   Inputs :                                                                                      |
|                                                                                                 |
|    D_acc                                                                                        |
|			   Double variable                                                                    |
|                                                                                                 |
|    var1                                                                                         |
|			   float variable                                                                     |
|                                                                                                 |
|    var2                                                                                         |
|			   float variable                                                                     |
|                                                                                                 |
|   Outputs :                                                                                     |
|                                                                                                 |
|    none                                                                                         |
|                                                                                                 |
|   Return Value :                                                                                |
|                                                                                                 |
|    D_var_out                                                                                    |
|			   Double variable                                                                    |
|_________________________________________________________________________________________________|
*/
double W_msu_16_16_ref(double  D_acc, float var1, float var2)
{
    double  D_var_out = ((double)var1*var2);
    D_acc = D_acc - D_var_out;

    return D_acc;
}

double W_mult_16_16_ref(float var1, float var2)
{
    double  D_var_out = ((double)var1*var2);
   
    return D_var_out;
}

double W_mac0_16_16_ref(double  D_acc, float var1, float var2)
{
    double  D_var_out = ((double)var1*var2);
    D_acc = D_acc + D_var_out;

    return D_acc;
}

double W_msu0_16_16_ref(double  D_acc, float var1, float var2)
{
    double  D_var_out = ((double)var1*var2);
    D_acc = D_acc - D_var_out;

    return D_acc;
}

double W_mult0_16_16_ref( float var1, float var2)
{
    double  D_var_out = ((double)var1*var2);
   
    return D_var_out;
}

double W_mac_32_16_ref(double  D_acc, float var1, float var2)
{
    double  D_var_out = ((double)var1*var2);
    D_acc = D_acc + D_var_out;

    return D_acc;
}

double W_msu_32_16_ref(double  D_acc, float var1, float var2)
{
    double  D_var_out = ((double)var1*var2);
    D_acc = D_acc - D_var_out;

    return D_acc;
}

double W_mult_32_16_ref(float var1, float var2)
{
    double  D_var_out = ((double)var1*var2);
    return D_var_out;
}

double W_mult_32_32_ref( float var1, float var2)
{
    double  D_var_out = ((double)var1*var2);
   
    return D_var_out;
}

double W_mult0_32_32_ref(double var1, double var2)
{
    double  D_var_out = var1*var2;

    return D_var_out;
}

// Modify_here  :  add new REF fucntions here.
double W_sat_ref(double var1)
{
    if (var1 > 1.0)
        var1 = 1.0;
    else if (var1 < -1.0)
        var1 = -1.0;
    return var1;
}
int  W_norm_ref(double var1)
{
    long  D_var_out = 0;

    // Special caes.
    if (var1 == 0.0)
        return 0;

    // Negate the number..
    if (var1 < 0.0)
    var1 = -(var1);

    while (var1 < 0.5)
    {
        
        var1 = var1 * 2.0;
        D_var_out++;

        if (D_var_out == 63)
            break;
    }


    return D_var_out;
}
double W_add_ref(double var1, double var2)
{
    double  D_var_out = (var1+var2);
  
    return D_var_out;
}

double W_sub_ref(double var1, double var2)
{
    double  D_var_out = (var1 - var2);

    return D_var_out;
}

double W_neg_ref(double var1)
{
    double  D_var_out = (-var1);

    return D_var_out;
}

double W_abs_ref(double var1)
{
    //double  D_var_out;
    
    if (var1  < 0.0)
        var1 = (-var1);

    return var1;
}


double W_shl_ref(double var1, short var2)
{
    double  D_var_out;
    long long shift_val =1;
    
    if (var2 <= 0)
    {
        // right shift
        if (var2 < -64)
            var2 = -64;
        var2 = -var2;
        D_var_out = var1 * 1.0 / (shift_val << var2);
    } else
    {
        if (var2 > 64)
            var2 = 64;
       
        D_var_out = var1 * (shift_val << var2);
    }

    return D_var_out;
}

double W_lshl_ref(double var1, short var2)
{
    double  D_var_out;
    long long shift_val = 1;

    if (var2 <= 0)
    {
        unsigned long long inp = (unsigned long long)var1;
        // right shift
        if (var2 < -64)
            var2 = -64;
        var2 = -var2;

  
        inp = (inp >> var2);
        D_var_out = (double)((long long)inp);
    }
    else
    {
        if (var2 > 64)
            var2 = 64;

        D_var_out = var1 * (shift_val << var2);
    }

    return D_var_out;
}

double W_shl_ref_sat(double var1, int var2)
{
    double  D_var_out;
    long long shift_val = 1;

    if (var2 <= 0)
    {
        // right shift
        if (var2 < -64)
            var2 = -64;
        var2 = -var2;
        D_var_out = var1 * 1.0 / (shift_val << var2);
    }
    else
    {
        if (var2 > 64)
            var2 = 64;

        D_var_out = var1 * (shift_val << var2);
    }

    if (D_var_out > 1.0)
        D_var_out = 1.0;
    
    if(D_var_out < -1.0)
        D_var_out = -1.0;
    
    return D_var_out;
}


double W_shr_ref(double var1, short var2)
{
    double  D_var_out;
    long long shift_val = 1;

    if (var2 < 0)
    {
        // left shift
        if (var2 < -64)
            var2 = -64;
        var2 = -var2;
        D_var_out = var1 *  (shift_val << var2);
    }
    else
    {
        // right shift
        if (var2 > 64)
            var2 = 64;

        D_var_out = var1 * 1/(shift_val << var2);
    }

    return D_var_out;
}

double W_lshr_ref(double var1, short var2)
{
    double  D_var_out;
    long long shift_val = 1;

    if (var2 < 0)
    {
        // left shift
        if (var2 < -64)
            var2 = -64;
        var2 = -var2;
        D_var_out = var1 *  (shift_val << var2);
    }
    else
    {
        // right shift
        unsigned long long inp = (unsigned long long)var1;
        if (var2 > 64)
            var2 = 64;

         inp  = (inp >> var2);
         D_var_out = (double)((long long)inp);
    }

    return D_var_out;
}

float Madd_32_16_ref(float L_var3, float L_var1, float var2)
{

    float L_var_out;
    L_var_out = L_var1* var2;
    L_var_out = L_var_out + L_var3;
    
    return L_var_out;

}


float Msub_32_16_ref(float L_var3, float L_var1, float var2)
{

    float L_var_out;
    L_var_out = L_var1* var2;
    L_var_out = L_var3 - L_var_out;
    
    return L_var_out;

}


float Madd_32_32_ref(float L_var3, float L_var1, float L_var2)
{
	float L_var_out;

    L_var_out = L_var1*L_var2;
    L_var_out = L_var_out + L_var3;
	
    return L_var_out;
}


float Msub_32_32_ref(float L_var3, float L_var1, float L_var2)
{
	float L_var_out;

    L_var_out = L_var1*L_var2;
    L_var_out = L_var3 - L_var_out;
	
    return L_var_out;
}


float Mpy_32_16_1_ref(float L_var1, float var2)
{
    float L_var_out;
    L_var_out = L_var1* var2;
    
    return L_var_out;

}


float Mpy_32_32_ref(float L_var1, float L_var2)
{
	float L_var_out;

    L_var_out = L_var1*L_var2;
	
    return L_var_out;
}

f_cmplx CL_add_ref (f_cmplx inp1, f_cmplx inp2)
{
	f_cmplx out;
	out.re = (inp1.re + inp2.re);
	out.im = (inp1.im + inp2.im);
	return out;
}

f_cmplx CL_sub_ref(f_cmplx inp1, f_cmplx inp2)
{
	f_cmplx out;
	out.re = (inp1.re - inp2.re);
	out.im = (inp1.im - inp2.im);
	return out;
}

f_cmplx CL_msu_j_ref(f_cmplx inp1, f_cmplx inp2)
{

	f_cmplx out;
	out.re = (inp1.re + inp2.im);
	out.im = (inp1.im - inp2.re);
	return out;
}

f_cmplx CL_mac_j_ref(f_cmplx inp1, f_cmplx inp2)
{

	f_cmplx out;
	out.re = (inp1.re - inp2.im);
	out.im = (inp1.im + inp2.re);
	return out;
}

f_cmplx C_add_ref (f_cmplx inp1, f_cmplx inp2)
{
	f_cmplx out;
	out.re = (inp1.re + inp2.re);
	out.im = (inp1.im + inp2.im);
	return out;
}

f_cmplx C_sub_ref(f_cmplx inp1, f_cmplx inp2)
{
	f_cmplx out;
	out.re = (inp1.re - inp2.re);
	out.im = (inp1.im - inp2.im);
	return out;
}

f_cmplx CL_multr_32x32_ref(f_cmplx input, f_cmplx coeff)
{
	f_cmplx result;
	result.re = ( ( input.re * coeff.re ) - ( input.im * coeff.im ) );
	result.im = ( ( input.re * coeff.im ) + ( input.im * coeff.re ) );    
	return result;
}

f_cmplx CL_multr_32x16_ref(f_cmplx input, f_cmplx coeff)
{
	f_cmplx result;
	result.re = ((input.re * coeff.re) - (input.im * coeff.im));
	result.im = ((input.re * coeff.im) + (input.im * coeff.re));
	return result;
}

f_cmplx C_multr_ref(f_cmplx input, f_cmplx coeff)
{
	f_cmplx result;
	result.re = ((input.re * coeff.re) - (input.im * coeff.im));
	result.im = ((input.re * coeff.im) + (input.im * coeff.re));
	return result;
}


f_cmplx C_scale_ref(f_cmplx x, float y)
{
	f_cmplx result;
	result.re = (x.re * y);
	result.im = (x.im * y);
	return result;
}

f_cmplx CL_dscale_ref(f_cmplx x, float y, float z)
{
	f_cmplx result;
	result.re = (x.re * y);
	result.im = (x.im * z);
	return result;
}


f_cmplx CL_negate_ref(f_cmplx x)
{
	f_cmplx result;
	result.re = -x.re;
	result.im = -x.im;
	return result;
}

f_cmplx CL_conjugate_ref(f_cmplx x)
{
	f_cmplx result;
	result.re = x.re;
	result.im = -x.im;			
	return result;
}


f_cmplx CL_mul_j_ref(f_cmplx input)
{
	f_cmplx temp, result;
	temp = CL_negate_ref(input);
	result.re = temp.im;
	result.im = input.re;			
	return result;
}

f_cmplx C_mac_r_ref(f_cmplx x, f_cmplx y, float c)
{
	f_cmplx result;
	result = CL_add_ref(x, C_scale_ref(y, c));	
	return result;
}

f_cmplx C_msu_r_ref(f_cmplx x, f_cmplx y, float c)
{
	f_cmplx result;
	result = CL_sub_ref(x, C_scale_ref(y, c));
	return result;
}

f_cmplx CL_shr_ref(f_cmplx inp, int shift_val)
{
	f_cmplx out;

	if (shift_val < 0)
	{

		// left shift
		if (shift_val < -32)
			shift_val = -32;
		shift_val = -shift_val;

		out.re = inp.re * ((long long)1 << shift_val);
		out.im = inp.im * ((long long)1 << shift_val);
	}
	else
	{

		// right shift
		if (shift_val >= 31)
			shift_val = 31;

		out.re = inp.re * 1.0f / ((long long)1 << shift_val);
		out.im = inp.im * 1.0f / ((long long)1 << shift_val);

	}

	if (out.re > 1.0)
	{
		out.re = 1.0;
	}
	if (out.im > 1.0)
	{
		out.im = 1.0;
	}

	if (out.re < -1.0)
	{
		out.re = -1.0;
	}
	if (out.im < -1.0)
	{
		out.im = -1.0;
	}
	return out;

}

f_cmplx CL_shl_ref(f_cmplx inp, int shift_val)
{
	f_cmplx out;

	if (shift_val < 0)
	{
		// right shift
		if (shift_val < -32)
			shift_val = -32;
		shift_val = -shift_val;

		out.re = inp.re * 1.0f / ((long long)1 << shift_val);
		out.im = inp.im * 1.0f / ((long long)1 << shift_val);

	}
	else
	{
		// left shift
		if (shift_val >= 31)
			shift_val = 31;

		out.re = inp.re * ((long long)1 << shift_val);
		out.im = inp.im * ((long long)1 << shift_val);

	}

	if (out.re > 1.0)
	{
		out.re = 1.0;
	}
	if (out.im > 1.0)
	{
		out.im = 1.0;
	}

	if (out.re < -1.0)
	{
		out.re = -1.0;
	}
	if (out.im < -1.0)
	{
		out.im = -1.0;
	}
	return out;

}

f_cmplx C_shr_ref(f_cmplx inp, short int shift_val)
{
	f_cmplx out;

	if (shift_val < 0)
	{

		// left shift
		if (shift_val < -16)
			shift_val = -16;
		shift_val = -shift_val;

		out.re = inp.re * (1 << shift_val);
		out.im = inp.im * (1 << shift_val);
	}
	else
	{

		// right shift
		if (shift_val >= 15)
			shift_val = 15;

		out.re = inp.re * 1.0f / (1 << shift_val);
		out.im = inp.im * 1.0f / (1 << shift_val);

	}

	if (out.re > 1.0)
	{
		out.re = 1.0;
	}
	if (out.im > 1.0)
	{
		out.im = 1.0;
	}

	if (out.re < -1.0)
	{
		out.re = -1.0;
	}
	if (out.im < -1.0)
	{
		out.im = -1.0;
	}
	return out;

}

f_cmplx C_shl_ref(f_cmplx inp, short int shift_val)
{
	f_cmplx out;

	if (shift_val < 0)
	{
		// right shift
		if (shift_val < -32)
			shift_val = -32;
		shift_val = -shift_val;

		out.re = inp.re * 1.0f / ((long long)1 << shift_val);
		out.im = inp.im * 1.0f / ((long long)1 << shift_val);

	}
	else
	{
		// left shift
		if (shift_val >= 31)
			shift_val = 31;

		out.re = inp.re * ((long long)1 << shift_val);
		out.im = inp.im * ((long long)1 << shift_val);

	}

	if (out.re > 1.0)
	{
		out.re = 1.0;
	}
	if (out.im > 1.0)
	{
		out.im = 1.0;
	}

	if (out.re < -1.0)
	{
		out.re = -1.0;
	}
	if (out.im < -1.0)
	{
		out.im = -1.0;
	}
	return out;

}

f_cmplx CL_round32_16_ref(f_cmplx x)
{
	
	return x;
}

/* End of file */

