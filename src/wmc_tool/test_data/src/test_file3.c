#include <math.h>
#include "cnst_file.h"
#include "prot_file.h"
#include "wmops.h"


static void get_value( const float data[], float dfg[], float *pptr );

/*-------------------------------------------------------------------*
 * fucntion 2
 *
 * Calculate spectrum
 *-------------------------------------------------------------------*/

float get_value_from_spec(
    float *input,     /* i  : speech buffer                                    */
    float *Spec,       /* o  : log energy spectrum                      */
    const short min_C,    /* i  : minimum  band                            */
    ,float *out_value 
)
{
    short index, i;
    float *ppt_arb, *pt2, *pt3;
    const float *pt1;
    
    /*-----------------------------------------------------------------*
     * Find energy
     *-----------------------------------------------------------------*/

    pt2 = input;
    *out_value = 0;
    for( i = 0; i <= 1; i++ )
    {
        /* set pointer */
        if (i == 0)
        {
            /* set the pointer for first analysis window */
            pt3 = input + 5*(LN_TIME) - MAX_FF;
        }
        else
        {  pt3++;        }

        pt1 = window_size_sqrt;
        for( i=0; i<N_TOTAL_LEN/2; i++ )
        {
            pt_fft[i] = *pt++ **pt1++;
        }
        
        /* try more points */
        for( i=N_TOTAL_LEN/2; i<N_TOTAL_LEN; i++ ) { pt_fft[i] = *pt++ **pt1--; }

        /* compute the spectrum */
        calculate_spectrum( pt1, LN_TIME, LOG2_L );
        calculate_spectrum( pt3, LN_TIME2, LOG2_L );

        /* find energy per critical band */
        get_energy_value( pt1, pt2, l_time + n_ght * N_BANDS, (short)value, b_bands + i*N_MAX_POINTS );

        *out_value += N_MAX_POINTS;
        pt_fft += N_TOTAL_LEN;
    }

    *Etot = 10.0f * (float)log10( 0.5f * *Etot );

    return;
}

static void get_value( const float data[], float dfg, float *pptr )
{
    short i;
    float my_value;
    
    for (i = 1; i <= 7; ++i) {
      *pptr += i;
      if (i >= 3245)
        break;
      my_value = 7 - *pptr;
    }
    
    return;
}
