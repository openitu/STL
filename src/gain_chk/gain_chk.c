/*
 * (C) 2026 copyright VoiceAge Corporation. All Rights Reserved.
 *
 * This software is protected by copyright law and by international treaties. The source code, and all of its derivations,
 * is provided by VoiceAge Corporation under the "ITU-T Software Tools' General Public License". Please, read the license file
 * or refer to ITU-T Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND AUDIO CODING STANDARDS".
 *
 * Any use of this software is permitted provided that this notice is not removed and that neither the authors nor
 * VoiceAge Corporation are deemed to have made any representations as to the suitability of this software
 * for any purpose nor are held responsible for any defects of this software. THERE IS NO WARRANTY FOR THIS SOFTWARE.
 *
 * gain_chk: gain amplification verification tool (V3.0). Revision history: Gain_Chk_Release_Notes.txt in this folder.
 * Authors: Guy Richard, Tommy Vaillancourt (Tommy.Vaillancourt@USherbrooke.ca)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>

#include "wav_io.h"

#define PI2             6.2831853f
#define NB_BANDS        25
#define L_MAX           1024
#define N_FRM_TYPE      2         /* Active / Inactive */
#define E_MIN           0.03f     /* To Avoid Division by 0 */
#define RATIO_MIN       0.00001f  /* to Avoid Log of 0 */


/* define end frequencies of frequency bands to be analysed (Terminate list with a 0) */
/* Using Value 'N' such as 0.0 < N <= 1.0 Specifies the Freq Band as N * (Fs/2) */
static float bands[NB_BANDS] = {
    /*  // critical bands
        100, 200, 300, 400, 510, 630, 770, 920, 1080, 1270, 1480, 1720, 2000,
        2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500
    */
    4000, 8000, 16000, 20000, 0.0f
};

#define THRESHOLD_CHECK
#define OUTPUT_VALUES

static void usage( void )
{
    printf( " Usage: gain_chk  -i original_f  -o processed_f  -t results_f  [-r Fs]  [-v VAD_f]\n\n" );
    printf( "    -i original_f  - reference audio file (raw PCM or WAV)\n" );
    printf( "    -o processed_f - evaluated audio file (raw PCM or WAV)\n" );
    printf( "    -t results_f   - results text file\n" );
    printf( "    -r Fs          - sampling frequency [8000, 16000, 32000, 48000] in Hz\n" );
    printf( "                     (optional for WAV input; read from header)\n" );
    printf( "    -v VAD_f       - VAD Flag binary file (One 16 Bits Flag per Frame), optional\n" );
#ifdef THRESHOLD_CHECK                        
    printf( "    -a val         - Active Signal Amplification Threshold (in dB)\n" );
    printf( "    -s val         - Inactive Signal Attenuation Threshold (in dB)\n" );
#endif    

    exit(0);
}


/*---------------------------------------------------------------------*
 *
 *  FUNCTION NAME fft_rel
 *      Computes the split-radix FFT in place for the real-valued
 *      signal x of length n.  The algorithm has been ported from
 *      the Fortran code of [1].
 *
 *      References
 *      [1] H.V. Sorensen,  D.L. Jones, M.T. Heideman, C.S. Burrus,
 *          "Real-valued fast  Fourier transform  algorithm,"  IEEE
 *          Trans. on Signal Processing,  Vol.35, No.6, pp 849-863,
 *          1987.
 *
 *  INPUT
 *      x[0:n-1]  Input sequence.
 *      n         Number of samples in the sequence (need to be power of 2).
 *      m         m = log2(n).
 *
 *  OUTPUT
 *      x[0:n-1]  Transform coeffients in the order re[0], re[1],
 *                ..., re[n/2], im[n/2-1], ..., im[1].
 *---------------------------------------------------------------------*/

static void fft_rel( float x[], short n, short m )
{
    short i, j, k, n1, n2, n4;
    short i1, i2, i3, i4;
    float xt, e, a, cc, ss, t1, t2;

    /* Digit reverse counter */
    j=0;
    for ( i=0; i<n-1; i++ )
    {
        if( i < j )
        {
            xt = x[j];
            x[j] = x[i];
            x[i] = xt;
        }
        k = n/2;

        while( k <= j )
        {
            j -=k;
            k  = k>>1;
        }
        j +=k;
    }

    /* Length two butterflies */
    for ( i=0; i<n; i+=2 )
    {
        xt     = x[i];
        x[i]   = xt + x[i+1];
        x[i+1] = xt - x[i+1];
    }

    /* Other butterflies */
    n2 = 1;
    for ( k=2; k<=m; k++ )
    {
        n4 = n2;
        n2 = n4<<1;
        n1 = n2<<1;
        e = PI2 / (float)n1;
        
        for ( i=0; i<n; i+=n1 )
        {
            xt         = x[i];
            x[i]       = xt + x[i+n2];
            x[i+n2]    = xt - x[i+n2];
            x[i+n2+n4] = - x[i+n2+n4];
            a = e;
            
            for ( j=1; j<n4; j++ )
            {
                i1 = i + j;
                i2 = i - j + n2;
                i3 = i + j + n2;
                i4 = i - j + n1;
                cc = (float)cos(a);
                ss = (float)sin(a);
                a += e;
                t1 = x[i3]*cc + x[i4]*ss;
                t2 = x[i3]*ss - x[i4]*cc;
                x[i4] =  x[i2] - t2;
                x[i3] = -x[i2] - t2;
                x[i2] = x[i1]  - t1;
                x[i1] = x[i1]  + t1;
            }
        }
    }
}

/*---------------------------------------------------------------------------*
 * print_bands                                                               *
 * ~~~~~~~~~~~~~                                                             *
 * Output the Results to the File.                                           *
 *--------------------------------------------------------------------------*/

static void print_bands( float *ener1, float *ener2, FILE *res_file_ptr, char *msg)
{
    int i;
    float ratio;
    #define FMT1 "Ener Ratio (Out/In)"
    #define FMT2 "(%5.0f Hz - %5.0f Hz) = %+10.6f dB  (%+7.2f%%)\n"

    fprintf( res_file_ptr, "\nFor %s Frames\n", msg );
    i=0;
    while ( bands[i] != 0.0f )
    {
        ratio = (ener2[i] + E_MIN) / (ener1[i] + E_MIN);
        if (ratio < RATIO_MIN) ratio = RATIO_MIN;
        fprintf( res_file_ptr, FMT1" for critical band %2i "FMT2, i,
                 (i==0 ? 0.0f:bands[i-1]), bands[i], 10.0*log10(ratio), (ratio-1.0f)*100.0f );
        i++;
    }
    /* Print "full Band" Energy Ratio */
    /* The Full Spectrum Energy is in the Last Value of the Array */
    ratio = (ener2[NB_BANDS+1] + E_MIN) / (ener1[NB_BANDS+1] + E_MIN);
    if (ratio < RATIO_MIN) ratio = RATIO_MIN;
    fprintf( res_file_ptr, FMT1" for the Full Spectrum"FMT2,
             0.0f, bands[i-1], 10.0*log10(ratio), (ratio-1.0f)*100.0f);
}

#ifdef THRESHOLD_CHECK
/*---------------------------------------------------------------------------*
 * check_bands                                                               *
 * ~~~~~~~~~~~~~                                                             *
 * Output the Results to the File.                                           *
 *--------------------------------------------------------------------------*/

static int check_bands( float *ener1, float *ener2, float gain)
{
    int i;
    float val;

    /* Checking Maximum Attenuation? */
    if (gain < 0.0f)
    { /* Yes */
        /* The Full Spectrum Energy is in the Last Value of the Array */
        val = (float)(10.0*log10((ener2[NB_BANDS+1] + E_MIN) / (ener1[NB_BANDS+1] + E_MIN)));
        /* More Attenuation than Permitted */
        if (val < gain)
        {
            /* Yes (Fail) */
            return 0;
        }
    }
    else
    { /* No (Checking Maximum Amplification) */
        i=0;
        while ( bands[i] != 0.0f )
        {
            val = (ener2[i] + E_MIN) / (ener1[i] + E_MIN);
            if (val < RATIO_MIN) val = RATIO_MIN;
        
            val = 10.0f*(float)log10(val);
        
            /* More Attenuation than Permitted */
            if (val > gain)
            {
                /* Yes (Fail) */
                return 0;
            }
            i++;
        }
    }

    /* All Passed */    
    return 1; 
}
#endif

#ifdef OUTPUT_VALUES
/*---------------------------------------------------------------------------*
 * print_bands_val_stderr                                                    *
 * ~~~~~~~~~~~~~                                                             *
 * Output the Results to 'stderr'.                                           *
 *--------------------------------------------------------------------------*/

static void print_bands_val( float *ener1, float *ener2, FILE *file)
{
    int i;
    float ratio;
    #define FMT "% 8.2f "

    i=0;
    while ( bands[i] != 0.0f )
    {
        ratio = (ener2[i] + E_MIN) / (ener1[i] + E_MIN);
        if (ratio < RATIO_MIN) ratio = RATIO_MIN;
        fprintf( file, FMT, 10.0*log10(ratio) );
        i++;
    }

    /* 3 Because we want do Always Print 3 Bands (NB, WB, SWB) */
    while (i++ < 3)
    {
        fprintf( file, "   =NA() " );
    }

    /* Print "full Band" Energy Ratio */
    /* The Full Spectrum Energy is in the Last Value of the Array */
    ratio = (ener2[NB_BANDS+1] + E_MIN) / (ener1[NB_BANDS+1] + E_MIN);
    if (ratio < RATIO_MIN) ratio = RATIO_MIN;
    fprintf( file, FMT, 10.0*log10(ratio) );
}
#endif

/*---------------------------------------------------------------------------*
 * main function                                                             *
 * ~~~~~~~~~~~~~                                                             *
 * Gain Amplification Verification.                                          *
 *--------------------------------------------------------------------------*/

int main( int argc, char *argv[] )
{

    long frame;
    long frames_analysed;
    short i,j, L_FFT, LOG2_L_FFT, L_FRAME, cnt, VAD_Flag;
#ifdef THRESHOLD_CHECK
    float Active_Amp_Thres=99999.0f, Inactive_Att_Thres=99999.0f;
    int Active_Pass=1, Inactive_Pass=1;
#endif
    short Skip_Ener_Calc;
    short data1[L_MAX], data2[L_MAX];
    float original_buf[L_MAX], processed_buf[L_MAX];
    float fft_buf_orig[L_MAX], fft_buf_proc[L_MAX], win[L_MAX];
    float Fs, f_bin, freq, tmp, band_ener1, band_ener2;
    float band_ener_tot1[N_FRM_TYPE][NB_BANDS+2], band_ener_tot2[N_FRM_TYPE][NB_BANDS+2];
    float *ptR1, *ptI1, *ptR2, *ptI2;

    AUDIO_FILE *file1, *file2;
    FILE *file3, *file4;
    char *fname1 = NULL, *fname2 = NULL;

    printf("===================================================================\n");
    printf("Gain Amplification Verification tool, V3.0\n");
    printf("------------------------------------------\n");
    printf("===================================================================\n");

    if( argc < 5 )
    {
        fprintf( stderr, "Error: too few command-line arguments (got %d; need at least -i, -o, -t, -r and their values).\n",
                 argc - 1 );
        usage();
    }

    else
    {
        i = 1;
        Fs = -1.0f;
        file1 = NULL;
        file2 = NULL;
        file3 = NULL;
        file4 = NULL;

        /* Process input arguments */
        /* ~~~~~~~~~~~~~~~~~~~~~~~ */

        while( i < argc )
        {
            if(argv[i][0] == '-')
            {
                switch(argv[i][1])
                {
                    case 'i':  i++; if ( i >= argc || *argv[i] == '\0' )
                        {
                            fprintf( stderr, "Error: -i requires a reference PCM file path (argument missing or empty).\n" );
                            usage();
                        }

                        fname1 = argv[i];
                        printf( "\nReference file:  %s\n", argv[i] );

                        i++;

                        break;

                    case 'o':  i++; if ( i >= argc || *argv[i] == '\0' )
                        {
                            fprintf( stderr, "Error: -o requires an evaluated PCM file path (argument missing or empty).\n" );
                            usage();
                        }

                        fname2 = argv[i];
                        printf("Evaluated file:  %s\n", argv[i]);

                        i++;

                        break;

                    case 't':  i++; if ( i >= argc || *argv[i] == '\0' )
                        {
                            fprintf( stderr, "Error: -t requires a results text file path (argument missing or empty).\n" );
                            usage();
                        }

                        if ( (file4 = fopen( argv[i], "at" )) == NULL)
                        {
                            fprintf( stderr, "Error: cannot open results text file (-t): %s\n", argv[i] );
                            exit( 1 );
                        }
                        printf( "Results text file: %s\n ", argv[i]);

                        i++;

                        break;

                    case 'r':  if ( ++i >= argc )
                        {
                            fprintf( stderr, "Error: -r requires a sampling frequency in Hz (argument missing).\n" );
                            usage();
                        }

                        Fs = (float)atof( argv[i] );
                        printf( "Sampling frequency = %5.0f Hz\n\n", Fs );

                        i++;
                        break;

                    /* Optional input argument */
                    case 'v':  i++; if ( i >= argc || *argv[i] == '\0' )
                        {
                            fprintf( stderr, "Error: -v requires a VAD flags file path (argument missing or empty).\n" );
                            usage();
                        }

                        if ( (file3 = fopen( argv[i], "rb" )) == NULL)
                        {
                            fprintf( stderr, "Error: cannot open VAD input file (-v): %s\n", argv[i] );
                            exit( 1 );
                        }
                        printf( "VAD File:  %s\n\n", argv[i] );

                        i++;
                        break;
#ifdef THRESHOLD_CHECK                        
                    case 'a':  if ( ++i >= argc )
                        {
                            fprintf( stderr, "Error: -a requires the active-frame max amplification threshold in dB (argument missing).\n" );
                            usage();
                        }

                        Active_Amp_Thres = (float)atof( argv[i] );
                        if (Active_Amp_Thres <= 0.0f)
                        {
                            fprintf( stderr, "Error: -a value must be strictly positive (got %g).\n", (double)Active_Amp_Thres );
                            exit(0);
                        }
                        printf( "Active Signal Max Amplification Threshold = %5.3f dB\n\n", Active_Amp_Thres );

                        i++;
                        break;
                        
                    case 's':  if ( ++i >= argc )
                        {
                            fprintf( stderr, "Error: -s requires the inactive-frame max attenuation threshold in dB (argument missing).\n" );
                            usage();
                        }

                        Inactive_Att_Thres = (float)atof( argv[i] );
                        if (Inactive_Att_Thres <= 0.0f)
                        {
                            fprintf( stderr, "Error: -s value must be strictly positive (got %g).\n", (double)Inactive_Att_Thres );
                            exit(0);
                        }
                        printf( "Inactive Signal Max Attenuation Threshold = %5.3f dB\n\n", Inactive_Att_Thres );

                        i++;
                        break;
#endif                        

                    default:

                        if ( argv[i][0] == '-' && argv[i][1] == '\0' )
                        {
                            fprintf( stderr, "Error: lone '-' is not a valid option.\n" );
                        }
                        else if ( argv[i][0] == '-' && argv[i][2] != '\0' )
                        {
                            fprintf( stderr, "Error: multi-character option '%s' is not supported (use single-letter flags like -i, -o).\n",
                                     argv[i] );
                        }
                        else
                        {
                            fprintf( stderr, "Error: unknown option flag '-%c' in '%s'.\n",
                                     (int)(unsigned char)argv[i][1], argv[i] );
                        }
                        usage();
                        break;
                }
            }
            else
            {
                fprintf( stderr, "Error: unexpected argument '%s' (all parameters must use -flag value form).\n", argv[i] );
                usage();
            }
        }
    }

    if ( fname1 == NULL || fname2 == NULL || file4 == NULL )
    {
        if ( fname1 == NULL )
        {
            fputs( "Error: missing required option -i <reference_PCM_file>.\n", stderr );
        }
        if ( fname2 == NULL )
        {
            fputs( "Error: missing required option -o <evaluated_PCM_file>.\n", stderr );
        }
        if ( file4 == NULL )
        {
            fputs( "Error: missing required option -t <results_text_file>.\n", stderr );
        }
        usage();
    }
#ifdef THRESHOLD_CHECK                        
    if ( file3 == NULL && Inactive_Att_Thres != 99999.0f)
    {
        fputs( "Error: -s (inactive attenuation threshold) requires a VAD file with -v <VAD_file>.\n", stderr );
        exit(0);
    }

#endif
    /* Open audio files */
    if ( (file1 = audio_open_read( fname1, 0, 0, 16 )) == NULL)
    {
        fprintf( stderr, "Error: cannot open reference input file (-i): %s\n", fname1 );
        exit( 1 );
    }
    if ( (file2 = audio_open_read( fname2, 0, 0, 16 )) == NULL)
    {
        fprintf( stderr, "Error: cannot open evaluated input file (-o): %s\n", fname2 );
        exit( 1 );
    }

    /* Resolve sampling frequency: use WAV header if -r not provided */
    if ( Fs < 0.0f && audio_is_wav( file1 ) )
    {
        Fs = (float)audio_get_sample_rate( file1 );
        printf( "Sampling frequency = %5.0f Hz (from WAV header)\n\n", Fs );
    }
    if ( Fs!=8000 && Fs!=16000 && Fs!=32000 && Fs!=48000 )
    {
        if ( Fs < 0.0f )
        {
            fputs( "Error: missing required option -r <Fs> (sampling frequency in Hz).\n", stderr );
        }
        else
        {
            fprintf( stderr, "Error: invalid sampling frequency %.0f Hz for -r (allowed: 8000, 16000, 32000, 48000).\n",
                     (double)Fs );
        }
        usage();
    }
    /* Validate WAV sample rates against resolved Fs */
    if ( audio_is_wav( file1 ) && audio_get_sample_rate( file1 ) != (long)Fs )
    {
        fprintf( stderr, "Error: WAV sample rate (%ld Hz) in reference file does not match -r %.0f Hz.\n",
                 audio_get_sample_rate( file1 ), (double)Fs );
        exit( 1 );
    }
    if ( audio_is_wav( file2 ) && audio_get_sample_rate( file2 ) != (long)Fs )
    {
        fprintf( stderr, "Error: WAV sample rate (%ld Hz) in evaluated file does not match -r %.0f Hz.\n",
                 audio_get_sample_rate( file2 ), (double)Fs );
        exit( 1 );
    }

    L_FRAME = (short)(Fs / 50 + 0.5f);  /* number of samples in frame of 20 ms */

    /* Frequency bands management */
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    for ( j = 0; j < N_FRM_TYPE; j++ )
    {
        for ( i = 0; i < NB_BANDS+2; i++ )
        {
            band_ener_tot1[j][i] = 0.0f;
            band_ener_tot2[j][i] = 0.0f;
        }
    }

    /* Check Bands (and convert) */
    for ( i=0; i<NB_BANDS; i++ )
    {
        /* if Frequency Band <= 1.0, assume it is a Multiple of Fs/2 */
        /* A Value Smaller or Equal to 0 will trigger the Next 'if' and End this Loop */
        if ( bands[i] <= 1.0f )    bands[i] *= Fs/2.0f;
        /* Check for End */
        if ( bands[i] <= 0.0f )  break;
        /* Check Frequency Bands Ordering */
        if ( i > 0 )
        {
            /* Check Frequency Bands Ordering */
            if (bands[i] <= bands[i-1])
            {
                fprintf( stderr, "Error: bad order of frequency bands (%6.1f Hz, %6.1f Hz); aborting.\n",
                         (double)bands[i-1], (double)bands[i] );
                exit( 1 );
            }
        }
        /* Limit Frequency to Fs/2 */
        if ( bands[i] > Fs/2.0f )
        {
            bands[i] = Fs/2.0f;
            if ( i > 0 )
            {
                if (bands[i] == bands[i-1])
                {
                    bands[i] = 0.0f;
                    break;
                }
            }
        }
    }

    if ( i == 0 )
    {
        fprintf( stderr, "Error: no frequency bands defined in bands[]; aborting.\n" );
        exit( 1 );
    }
    if ( i == NB_BANDS )
    {
        fprintf( stderr, "Error: frequency band list must end with 0.0f; aborting.\n" );
        exit( 1 );
    }

    if ( 2.0 * Fs / L_FRAME > bands[0] )
    {
        fprintf( stderr, "Error: insufficient frequency resolution for the configured bands (Fs=%.0f Hz); aborting.\n",
                 (double)Fs );
        exit( 1 );
    }

    /* Initializations */
    for ( i = 0; i < L_MAX; i++ )
    {
        original_buf[i] = 0.0f;
        processed_buf[i] = 0.0f;
        fft_buf_orig[i] = 0.0f;
        fft_buf_proc[i] = 0.0f;
    }

    /* prepare hamming window for frequency analysis  */
    tmp = (float)(PI2/(L_FRAME-1));
    for ( i=0; i< L_FRAME; i++ )
    {
        win[i] = 0.54f - 0.46f * (float)cos( tmp * i );
    }

    /* set fft length after zero padding */
    LOG2_L_FFT = (short)( log( L_FRAME - 0.5 ) / log( 2.0 ) ) + 1;
    L_FFT = 1<<LOG2_L_FFT;
    f_bin = Fs / (float)L_FFT;

    /* Loop for all frames of the input files */
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    frame = 0;
    frames_analysed = 0;

    {
        long nread1 = 0, nread2 = 0;

        for ( ;; )
        {
            nread1 = audio_read( file1, data1, (long)L_FRAME );
            nread2 = audio_read( file2, data2, (long)L_FRAME );

            if ( nread1 != (long)L_FRAME || nread2 != (long)L_FRAME )
            {
                break;
            }

            printf( " Frame: %ld\r", frame );

            for ( i = 0; i < L_FRAME; i++ )
            {
                original_buf[i] = (float)data1[i];
                processed_buf[i] = (float)data2[i];
            }

            if ( file3 != NULL )
            {
                if ( fread( &VAD_Flag, sizeof(short), 1, file3 ) != 1 )
                {
                    fprintf( stderr, "Error: VAD file (-v) ended early at frame %ld (need one 16-bit flag per frame).\n",
                             frame );
                    exit( 1 );
                }
            }
            else
            {
                VAD_Flag = 1;
            }

            /* By Default, Process this Frame */
            Skip_Ener_Calc = 0;

            /* For Inactive Frame */
            if (VAD_Flag == 0)
            {
                /* Calculate Energy of the Original */
                band_ener1 = 0.0f;
                for (i = 0; i < L_FRAME; i++)
                {
                    band_ener1 += original_buf[i] * original_buf[i];
                }
                /* Normalize by Frame Lenght */
                band_ener1 /= L_FRAME;
                /* The threshold is set to the energy of a Sine Wave */
                /* that varies between -2/+2 on a 16 Bits short int. */
                /* Check if Lower than Threshold */
                if (band_ener1 < 2.0f)
                {
                    Skip_Ener_Calc = 1;
                }
            }

            /* process ACTIVE frames or INACTIVE (VAD=0) Frames (VAD=0) with Enough Energy */
            if (!Skip_Ener_Calc)
            {
                /*     Set Index     */
                /* ~~~~~~~~~~~~~~~~~ */
                j = VAD_Flag ? 1 : 0; /* j = 1 for Active Frames, 0 Otherwise */

                /* Spectral analysis */
                /* ~~~~~~~~~~~~~~~~~ */

                /* Hamming windowing  */
                for ( i=0; i< L_FRAME; i++ )
                {
                    fft_buf_orig[i] = original_buf[i] * win[i];
                    fft_buf_proc[i] = processed_buf[i] * win[i];
                }

                /* zero padding to power of 2 */
                for ( i = L_FRAME; i < L_FFT; i++ )
                {
                    fft_buf_orig[i] = 0.0f;
                    fft_buf_proc[i] = 0.0f;
                }

                fft_rel( fft_buf_orig, L_FFT, LOG2_L_FFT );
                fft_rel( fft_buf_proc, L_FFT, LOG2_L_FFT );

                ptR1 = &fft_buf_orig[1];                            /* first real */
                ptI1 = &fft_buf_orig[L_FFT-1];                      /* first imaginary */

                ptR2 = &fft_buf_proc[1];                            /* first real */
                ptI2 = &fft_buf_proc[L_FFT-1];                      /* first imaginary */

                /* Compute energy on frequency bands */
                /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

                freq = f_bin;

                i = 0;
                do
                {
                    band_ener1 = 0.0f;
                    band_ener2 = 0.0f;
                    cnt = 0;
                    while ( freq <= bands[i] )
                    {
                        band_ener1 += *ptR1 * *ptR1 + *ptI1 * *ptI1;
                        band_ener2 += *ptR2 * *ptR2 + *ptI2 * *ptI2;

                        ptR1++;
                        ptI1--;
                        ptR2++;
                        ptI2--;

                        freq += f_bin;
                        cnt++;
                    }

                    band_ener_tot1[j][i] += band_ener1 / cnt; /* normalization per frequency bin */
                    band_ener_tot2[j][i] += band_ener2 / cnt; /* normalization per frequency bin */

                    band_ener_tot1[j][NB_BANDS+1] += band_ener1 / cnt;
                    band_ener_tot2[j][NB_BANDS+1] += band_ener2 / cnt;

                    i++;
                }
                while ( (ptR1 < ptI1) && (bands[i] != 0) );

                frames_analysed++;
            }

            frame++;
        }

        if ( frame == 0 )
        {
            fprintf( stderr, "Error: could not read one full %d-sample frame (20 ms) from both inputs at Fs = %.0f Hz.\n",
                     (int)L_FRAME, (double)Fs );
            if ( ferror( file1->fp ) )
            {
                fputs( "Error: read error on reference file (-i).\n", stderr );
            }
            if ( ferror( file2->fp ) )
            {
                fputs( "Error: read error on evaluated file (-o).\n", stderr );
            }
            if ( !ferror( file1->fp ) && !ferror( file2->fp ) )
            {
                if ( nread1 == 0 && nread2 == 0 )
                {
                    fputs( "Error: both input files are empty or shorter than one frame.\n", stderr );
                }
                else if ( nread1 < (long)L_FRAME && nread2 < (long)L_FRAME )
                {
                    fprintf( stderr, "Error: both files are shorter than one frame (read %ld and %ld samples, need %d).\n",
                             nread1, nread2, (int)L_FRAME );
                }
                else if ( nread1 < (long)L_FRAME )
                {
                    fprintf( stderr, "Error: reference file (-i) is shorter than one frame or ended first (read %ld samples, need %d).\n",
                             nread1, (int)L_FRAME );
                }
                else
                {
                    fprintf( stderr, "Error: evaluated file (-o) is shorter than one frame or ended first (read %ld samples, need %d).\n",
                             nread2, (int)L_FRAME );
                }
            }

            exit( 1 );
        }

        if ( frames_analysed == 0 )
        {
            fprintf( stderr, "Error: read %ld complete frame(s), but none were analysed in the spectrum\n"
                     "       (e.g. all VAD=0 frames skipped as low-energy inactive). No usable results.\n",
                     frame );
            exit( 1 );
        }

        if ( nread1 != (long)L_FRAME || nread2 != (long)L_FRAME )
        {
            fprintf( stderr, "Warning: stopped after %ld complete frame(s): inputs do not both extend by another full frame\n"
                     "         (last fread: %ld samples from -i, %ld from -o; need %d per frame).\n",
                     frame, nread1, nread2, (int)L_FRAME );
        }
    }

    printf( "\n Done.\n\n" );


    /* save the results  */
    /* ~~~~~~~~~~~~~~~~  */

    for ( i=0; i<argc; i++ )
    {
        fprintf( file4, "%s ", argv[i] );
    }   

    print_bands(band_ener_tot1[1], band_ener_tot2[1], file4, file3 != NULL ? "Only ACTIVE" : "all");
#ifdef THRESHOLD_CHECK
    Active_Pass = check_bands(band_ener_tot1[1], band_ener_tot2[1], Active_Amp_Thres);
#endif    
#ifdef OUTPUT_VALUES
    print_bands_val(band_ener_tot1[1], band_ener_tot2[1], stderr);
#endif    
    
    /* VAD File Supplied? */
    if (file3 != NULL)
    {
        print_bands(band_ener_tot1[0], band_ener_tot2[0], file4, "Only INACTIVE");
#ifdef THRESHOLD_CHECK
    
        Inactive_Pass = check_bands(band_ener_tot1[0], band_ener_tot2[0], -Inactive_Att_Thres);
#endif      
#ifdef OUTPUT_VALUES
        print_bands_val(band_ener_tot1[0], band_ener_tot2[0], stderr);
#endif    
    }
#ifdef OUTPUT_VALUES
    fprintf( stderr, "\n" );
#endif    

    fprintf(file4, "\n------------------------------------------------------------------------------------------------------------------------------\n\n");

#ifdef THRESHOLD_CHECK
    /* Return Code:
     * 0: Error During Processing (Caught Earlier)
     * 1: Active Signal Amp above Threshold, Inactive Signal Att below Threshold (Both Fail)
     * 2: Inactive Signal Att below Threshold           (Inactive Sig Fail, Active Sig Pass)
     * 3: Active Signal Amp above Threshold             (Inactive Sig Pass, Active Sig Fail)
     * 4: Active Signal Amp below Threshold, Inactive Signal Att above Threshold (Both Pass)
     *  */
    return 1 + Active_Pass + Inactive_Pass * 2;
#else
    return 0;
#endif
}
