        #include <math.h>
        #include "cnst_file.h"
        #include "prot_file.h"
        #include "wmops.h"
/*AddedByWMC_Tool*/#include "wmc_auto.h"
/*AddedByWMC_Tool*/#define calculate_spectrum_ func_(calculate_spectrum,3)
/*AddedByWMC_Tool*/#define   get_energy_value_ func_(get_energy_value,5)


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
        { func_start_
            short index, i;
            float *ppt_arb, *pt2, *pt3;
            const float *pt1;

            /*-----------------------------------------------------------------*
             * Find energy
             *-----------------------------------------------------------------*/

$("M")      pt2 = input;
$("M")      *out_value = 0;
            for_( i = 0; i <= 1; i++ )
            {
                /* set pointer */
                if_ (i == 0)
                {
                    /* set the pointer for first analysis window */
$("M+*-")           pt3 = input + 5*(LN_TIME) - MAX_FF;
                }
                else
                {  pt3++;        }

$("M")          pt1 = window_size_sqrt;
$("*")          for_( i=0; i<N_TOTAL_LEN/2; i++ )
$("*")          {
$("[]M*")           pt_fft[i] = *pt++ **pt1++;
                }

                /* try more points */
$("*")          for_( i=N_TOTAL_LEN/2; i<N_TOTAL_LEN; i++ ) { $("[]M*")pt_fft[i] = *pt++ **pt1--; }

                /* compute the spectrum */
                calculate_spectrum_( pt1, LN_TIME, LOG2_L );
                calculate_spectrum_( pt3, LN_TIME2, LOG2_L );

                /* find energy per critical band */
$("+*+*")       get_energy_value_( pt1, pt2, l_time + n_ght * N_BANDS, (short)value, b_bands + i*N_MAX_POINTS );

$("+=")         *out_value += N_MAX_POINTS;
$("+=")         pt_fft += N_TOTAL_LEN;
            }

$("M**")    *Etot = 10.0f * (float)log10_( 0.5f * *Etot );

            return_;
        }

        static void get_value( const float data[], float dfg, float *pptr )
        { func_start_
            short i;
            float my_value;

            for_ (i = 1; i <= 7; ++i) {
$("+=")       *pptr += i;
$("Ge")       if_ (i >= 3245)
                break_;
$("M-")       my_value = 7 - *pptr;
            }

            return_;
        }
/*AddedByWMC_Tool*/ int PROM_Size_Func( test_file3 ) { return 43; }
