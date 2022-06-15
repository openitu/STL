/*
    Implementation of BS.1770-4 as defined in Recommendation ITU-R BS.1770-4

    See LICENSE.md for terms.

    Author: erik.norvell@ericsson.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define BLOCK_SIZE                19200      /* 400 ms in 48000 Hz sample rate */
#define STEP_SIZE                 4800       /* 100 ms in 48000 Hz sample rate (75% overlap of 400 ms gating blocks) */   
#define LKFS_OFFSET               (-0.691)
#define ABSOLUTE_THRESHOLD        (-70.0)
#define RELATIVE_THRESHOLD_OFFSET (-10.0)
#define MAX_ITERATIONS            10
#define RELATIVE_DIFF             0.0001
#define MAX_CH_NUMBER             24

/*
    Channel weights for default channel ordering. Assumes channels are ordered as in 22.2 WAVE files:

    http://www.nhk.or.jp/strl/publica/bt/en/fe0045-6.pdf

    AES Pair No./Ch     No. Channel     No. Label       Name                    Weighting factor
    1/1                 1               FL              Front left              1.0
    1/2                 2               FR              Front right             1.0
    2/1                 3               FC              Front center            1.0
    2/2                 4               LFE1            LFE-1                   0.0
    3/1                 5               BL              Back left               1.41
    3/2                 6               BR              Back right              1.41
    4/1                 7               FLc             Front left center       1.0
    4/2                 8               FRc             Front right center      1.0
    5/1                 9               BC              Back center             1.0
    5/2                 10              LFE2            LFE-2                   0.0
    6/1                 11              SiL             Side left               1.41
    6/2                 12              SiR             Side right              1.41
    7/1                 13              TpFL            Top front left          1.0
    7/2                 14              TpFR            Top front right         1.0
    8/1                 15              TpFC            Top front center        1.0
    8/2                 16              TpC             Top center              1.0
    9/1                 17              TpBL            Top back left           1.0
    9/2                 18              TpBR            Top back right          1.0
    10/1                19              TpSiL           Top side left           1.0
    10/2                20              TpSiR           Top side right          1.0
    11/1                21              TpBC            Top back center         1.0
    11/2                22              BtFC            Bottom front center     1.0
    12/1                23              BtFL            Bottom front left       1.0
    12/2                24              BtFR            Bottom front right      1.0
*/
static const char default_conf_24[25] = "000L11000L11000000000000";

/*
    Channel weights for default channel ordering for files with up to 18 channels.

    http://www.microsoft.com/hwdev/tech/audio/multichaudP.asp

    No. Channel     No. Label       Name                    Weighting factor

    1.              FL              Front Left              1.0
    2.              FR              Front Right             1.0
    3.              FC              Front Center            1.0
    4.              LF              Low Frequency           0.0
    5.              BL              Back Left               1.41
    6.              BR              Back Right              1.41
    7.              FLC             Front Left of Center    1.0    
    8.              FRC             Front Right of Center   1.0
    9.              BC              Back Center             1.0
    10.             SL              Side Left               1.41
    11.             SR              Side Right              1.41    
    12.             TC              Top Center              1.0
    13.             TFL             Top Front Left          1.0
    14.             TFC             Top Front Center        1.0
    15.             TFR             Top Front Right         1.0
    16.             TBL             Top Back Left           1.0
    17.             TBC             Top Back Center         1.0
    18.             TBR             Top Back Right          1.0
*/
static const char default_conf_18[19] = "000L1100011000000";


/* R - REC - BS.1770 - 2 - 201103.pdf, Table 1, Filter coefficients for stage 1 of the pre - filter to model a spherical head */
static const double B1[3] = { 1.53512485958697, -2.69169618940638, 1.19839281085285 };
static const double A1[3] = { 1.0,              -1.69065929318241, 0.73248077421585 };

/* R - REC - BS.1770 - 2 - 201103.pdf, Table 2, Filter coefficients for the RLB weighting curve */
static const double B2[3] = { 1.0,              -2.0,              1.0              };
static const double A2[3] = { 1.0,              -1.99004745483398, 0.99007225036621 };

void usage()
{
    fprintf( stdout, "bs1770demo.exe [options] <input file> [<output file>]\n" );
    fprintf( stdout, "\n" );
    fprintf( stdout, "<input file>      Input file,  16 bit PCM, 48 kHz\n" );
    fprintf( stdout, "[<output file>]   Output file, 16 bit PCM, 48 kHz (Optional)\n" );
    fprintf( stdout, "\n" );
    fprintf( stdout, "Options:\n" );
    fprintf( stdout, "-nchan N          Number of channels [1..24] (Default: 1)\n" );
    fprintf( stdout, "-lev L            Target level LKFS (Default: -26)\n" );
    fprintf( stdout, "-conf xxxx        Configuration string:\n") ;
    fprintf( stdout, "                      '1' ldspk pos within |elev| < 30 deg, 60 deg <= |azim| <= 120 deg\n" );
    fprintf( stdout, "                      'L' LFE channel (weight zero)\n" );
    fprintf( stdout, "                      '0' otherwise\n" );
    fprintf( stdout, "                      (Default conf nchan <= 18: %s)\n", default_conf_18 );
    fprintf( stdout, "                      (Default conf nchan  > 18: %s)\n", default_conf_24 );

    exit(-1);
}


void scale(
    const double *input,  /* i: Input signal       */
    const double fac,     /* i: Scaling factor     */
          double *output, /* i: Output signal      */
    const long length     /* i: Length of signal   */
)
{
    long i;
    for( i = 0; i < length; i++ )
    {
        output[i] = input[i] * fac;
    }

    return;
}

/*-------------------------------------------------
 * Sum of squares of input signal
 *-------------------------------------------------*/
double sumsq(             /* o: Sum of squared signal */
    const double *input,  /* i: Input signal          */
    const long length     /* i: Length of signal      */
)
{
    long i;
    double result;
    result = 0;
    for( i = 0; i < length; i++ )
    {
        result = result + input[i] * input[i];
    }

    return result;
}

/*------------------------------------------------- 
 * 2nd order iir filter
 * y(n) = b[0] * x(n) + b[1] * x(n-1) + b[2] * x(n-2)
                      - a[1] * y(n-1) - a[2] * y(n-2)
    
   a[0] assumed to be 1.0

 *-------------------------------------------------*/
void iir2(
    const double *input,  /* i  : Input signal         */
          double *output, /* o  : Output signal        */
    const long length,    /* i  : Length of signal     */
    const double *B,      /* i  : B coefficients       */
    const double *A,      /* i  : A coefficients       */
          double *Bmem,   /* i/o: B memory (3 samples) */
          double *Amem    /* i/o: A memory (3 samples) */
    )
{
    long i;

    for (i = 0; i < length; i++ )
    {
        Bmem[2] = Bmem[1];
        Bmem[1] = Bmem[0];
        Bmem[0] = input[i];

        Amem[2] = Amem[1];
        Amem[1] = Amem[0];
        Amem[0] = B[0] * Bmem[0] + B[1] * Bmem[1] + B[2] * Bmem[2]
                                 - A[1] * Amem[1] - A[2] * Amem[2];
        output[i] = Amem[0];
    }

    return;
}

void deinterleave_short2double(
    short *input_short,  /* i: Input short signal     */
    double *input,       /* i: Input signal in double */
    const long length,   /* i: Length of signal       */
    const long nchan     /* i: Number of channels     */
)
{
    long i,j,k;
    long block_length;

    block_length = length / nchan;
    j = 0;
    k = 0;

    for ( i = 0; i < length; i++ )
    {
        input[block_length*j + k] = ((double)(input_short[i])) / 32768.0;
        j++;
        if (j == nchan )
        {
            j = 0;
            k++;
        }
    }

    return;
}

long interleave_double2short( /* returns number of clipped samples */
    double *input,            /* i: Input signal in double         */
    short *input_short,       /* i: Input signal in short          */
    const long length,        /* i: Length of signal               */
    const long nchan          /* i: Number of channels             */
)
{
    long i, j, k, indx;
    long block_length;
    long clip;
    double input_16;

    block_length = length / nchan;
    clip = 0;
    indx = 0;
    j = 0;
    k = 0;
    for( i = 0; i < length; i++ )
    {
        input_16 = input[indx] * 32768.0;

        if( input_16 > 32767.0 )
        {
            clip++;
            input_short[i] = 32767;
        }
        else
        {
            if( input_16 < -32768.0 )
            {
                clip++;
                input_short[i] = -32768;
            }
            else
            {
                if ( input_16 > 0.0 )
                {
                    input_short[i] = (short)(input_16 + 0.5);
                }
                else 
                {
                    input_short[i] = (short)(input_16 - 0.5);
                }
            }
        }
        j++;
        if( j == nchan )
        {
            j = 0;
            k++;
        }
        indx = block_length * j + k;
    }

    return clip;
}

double gated_loudness(                  /* o: gated loudness                 */
    const double *gating_block_energy,  /* i: gating_block_energy            */
    const double fac,                   /* i: Scaling factor                 */
    const long n_gating_blocks,         /* i: Number of gating blocks        */
    const double threshold              /* i: LKFS threshold                 */
)
{
    long i;
    long count;
    double energy = 0.0;
    count = 0;
    for( i = 0; i < n_gating_blocks; i++ )
    {
        if( (LKFS_OFFSET + 10 * log10( gating_block_energy[i] * fac * fac )) > threshold )
        {
            energy += gating_block_energy[i] * fac * fac;
            count++;
        }
    }

    return LKFS_OFFSET + 10 * log10( energy / count );
}

double gated_loudness_adaptive(         /* o: gated loudness, using adaptive threshold  */
    const double *gating_block_energy,  /* i: gating_block_energy                       */
    const double fac,                   /* i: Scaling factor                            */
    const long n_gating_blocks          /* i: Number of gating blocks                   */
)
{
    double relative_threshold;
    double gated_loudness_final;

    /* Find scaling factor */
    relative_threshold = gated_loudness( gating_block_energy, fac, n_gating_blocks, ABSOLUTE_THRESHOLD ) + RELATIVE_THRESHOLD_OFFSET;
    if( ABSOLUTE_THRESHOLD > relative_threshold )
    {
        relative_threshold = ABSOLUTE_THRESHOLD;
    }
    gated_loudness_final = gated_loudness( gating_block_energy, fac, n_gating_blocks, relative_threshold );
    return gated_loudness_final;
}


double find_scaling_factor(            /* o: scaling factor                 */
    const double *gating_block_energy, /* i: gating_block_energy            */
    const long n_gating_blocks,        /* i: Number of gating blocks        */
    const double lev,                  /* i: Target level                   */
          double *lev_input,           /* o: Input level                    */
          double *lev_obtained         /* o: Obtained level                 */
)
{
    long itr;
    double last_fac;
    double fac;
    double gated_loudness_final;

    last_fac = 100.0; /* Dummy init to trigger first iteration */
    fac = 1.0;
    itr = 0;
    while( (fabs( 1.0 - fac / last_fac ) > RELATIVE_DIFF) && (itr < MAX_ITERATIONS) )
    {
        /* Find scaling factor */
        gated_loudness_final = gated_loudness_adaptive( gating_block_energy, fac, n_gating_blocks );
        last_fac = fac;
        fac *= pow( 10.0, (lev - gated_loudness_final) / 20.0 );
        if (itr == 0 )
        {
            *lev_input = gated_loudness_final;
        }
        itr++;
    }

    *lev_obtained = gated_loudness_final;
    return fac;
}

long parse_conf(      /*  o: 0:success, -1:fail   */
    const char *str,  /*  i: Configuration string */
    const long nchan, /*  i: Number of channels   */ 
    double *G         /*  o: Number of channels   */
)
{
    long i;
    for(i = 0; i < nchan; i++ )
    {
        if( str[i] == '0' )
        {
            G[i] = 1.0;
        }
        else if( str[i] == '1' )
        {
            G[i] = 1.41;
        }
        else if( str[i] == 'L' || str[i] == 'l' )
        {
            G[i] = 0;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

int main(int argc, char **argv )
{
    FILE* f_input;
    FILE* f_output;
    char *input_filename;
    char *output_filename;
    double *input;
    double *p_input;
    short *input_short;
    double *Bmem1, *Amem1, *Bmem2, *Amem2;
    double *gating_block_energy; /* Buffer for energy values of gating block j */
    double *e_tmp;   /* Circular buffer for computing energy of each 100 ms sub-block */
    long nchan; 
    long length_total;
    long length;
    long n_gating_blocks;
    long clip;
    long n,i,j;
    const char *conf;
    double lev_input;
    double lev_target;
    double lev_obtained;
    double fac;
    double G[MAX_CH_NUMBER];
    short zero_input_flag;

    lev_target = -26;  /* Default target level       */
    i = 1;
    conf = NULL;
    nchan = -1;
    zero_input_flag = 1;

    /* Command line parsing */
    if( argc == 1 )
    {
        usage();
    }
    
    /* Process options */
    while( argv[i][0] == '-' )
    {
        if( strcmp( argv[i], "-nchan" ) == 0 )
        {
            if( sscanf( argv[i + 1], "%ld", &nchan ) != 1 )
            {
                fprintf( stderr, "*** Invalid number of channels %s, exiting..\n", argv[i + 1] );
                usage();
            }
            if ( nchan > MAX_CH_NUMBER )
            {
                fprintf( stderr, "*** Number of channels %ld exceeds maximum number of %d, exiting..\n", nchan, MAX_CH_NUMBER );
                usage();
            }
            i += 2;
        }
        else if( strcmp( argv[i], "-lev" ) == 0 )
        {
            if( sscanf( argv[i + 1], "%lf", &lev_target ) != 1 )
            {
                fprintf( stderr, "*** Invalid target level %s, exiting..\n", argv[i + 1] );
                usage();
            }
            i += 2;
        }
        else if( strcmp( argv[i], "-conf" ) == 0 )
        {
            conf = argv[i + 1];
            i += 2;
        }
        else
        {
            fprintf( stderr, "*** Invalid option %s, exiting..\n", argv[i] );
            usage();
        }
    }

    /* Process mandatory arguments */
    if ((argc-i) < 1 )
    {
        fprintf( stderr, "*** Not enough input arguments, exiting..\n" );
        usage();
    }

    input_filename = argv[i++];
    if( (f_input = fopen( input_filename, "rb" )) == NULL )
    {
        fprintf( stderr, "*** Could not open input file %s, exiting..\n\n", input_filename );
        usage();
    }
    if( (argc - i) < 1 )
    {
        f_output = NULL;
    }
    else
    {
        output_filename = argv[i];
        if( (f_output = fopen( output_filename, "wb" )) == NULL )
        {
            fprintf( stderr, "*** Could not open output file %s, exiting..\n\n", output_filename );
            usage();
        }
    }

    /* Setup configuration */
    if( conf == NULL )
    {
        if (nchan == -1 )
        {
            nchan = 1;
        }
        if( nchan <= 18 )
        {
            conf = default_conf_18;
        }
        else
        {
            conf = default_conf_24;
        }
        parse_conf(conf, nchan, G);
    }
    else
    {
        if( (nchan != -1L) && (strlen(conf) != nchan) )
        {
            fprintf( stderr, "*** Number of channels %ld does not match configuration %s, exiting..\n\n", nchan, conf );
            usage();
        }
        else
        {
            nchan = (long) strlen( conf );
            if( parse_conf( conf, nchan, G ) != 0 )
            {
                fprintf( stderr, "*** Invalid configuration %s, exiting..\n\n", conf );
                usage();
            }
        }
    }


    /* Print settings */
    fprintf( stdout, "Input file:       %s\n", input_filename );
    if (f_output != NULL )
    {
        fprintf( stdout, "Output file:      %s\n", output_filename );
    }
    fprintf( stdout, "nchan:            %ld\n", nchan );

    /* Find length of input file */
    fseek( f_input, 0L, SEEK_END );
    length_total = ftell( f_input ) / (2*nchan); /* 2 bytes per sample (16 bits), nchan channels */
    if( (ftell( f_input ) % (2 * nchan)) != 0 )
    {
        fprintf( stderr, "*** Number of samples not divisible into number of channels, exiting..\n" );
        exit( -1 );
    }
    if( length_total < BLOCK_SIZE )
    {
        fprintf( stderr, "*** Input file must be longer than 400 ms to use bs1770demo, exiting..\n" );
        exit( -1 );
    }
    rewind( f_input );
    n_gating_blocks = 4 * (length_total - BLOCK_SIZE) / (BLOCK_SIZE);

    /* Allocate input buffers */
    input = malloc( sizeof( double ) * STEP_SIZE * nchan );
    input_short = malloc( sizeof( short ) * STEP_SIZE * nchan ); 
    /* Allocate filter buffers */
    Bmem1 = calloc( sizeof( double ), 3 * nchan );
    Amem1 = calloc( sizeof( double ), 3 * nchan );
    Bmem2 = calloc( sizeof( double ), 3 * nchan );
    Amem2 = calloc( sizeof( double ), 3 * nchan );
    /* Allocate energy array to allow two passes */
    gating_block_energy = malloc( sizeof( double ) * n_gating_blocks);
    e_tmp = malloc( sizeof( double ) * 4 );

    /* Obtain filtering and compute energy of gating blocks */
    for( n = 0, j = -3; j < n_gating_blocks; n++, j++ )
    {
        /* Read next sub-block */
        fread( input_short, sizeof( short ), STEP_SIZE * nchan, f_input );

        deinterleave_short2double( input_short, input, STEP_SIZE * nchan, nchan );

        /* Filter sub-block and store energy in circular buffer e_tmp */
        p_input = input;
        e_tmp[(n % 4)] = 0;
        for(i = 0; i < nchan; i++ )
        {
            iir2( p_input, p_input, STEP_SIZE, B1, A1, Bmem1 + 3 * i, Amem1 + 3 * i );
            iir2( p_input, p_input, STEP_SIZE, B2, A2, Bmem2 + 3 * i, Amem2 + 3 * i );
            e_tmp[(n % 4)] += G[i] * sumsq( p_input, STEP_SIZE );
            p_input += STEP_SIZE;
        }

        /* Compute energies of block j from 4 current sub-blocks in circular buffer, excluding incomplete blocks */
        if( j >= 0 )
        {
            gating_block_energy[j] = ( e_tmp[0] + e_tmp[1] + e_tmp[2] + e_tmp[3] ) / ((double)BLOCK_SIZE);
            zero_input_flag &= (gating_block_energy[j] == 0.0); /* Keep track of all-zero input */
        }
    }

    if( !zero_input_flag )
    { 

        if( f_output != NULL )
        { 
            /* Output file is specified -- find the scaling factor to reach the target level and apply scaling */

            /* Find scaling factor */
            /* Since a rescaling affects the relative gating threshold the factor is found through an iterative function */
            fac = find_scaling_factor( gating_block_energy, n_gating_blocks, lev_target, &lev_input, &lev_obtained );

            /* Apply scaling */
            rewind( f_input ); 
            length_total = 0;
            clip = 0;
            while( (length = (long)fread( input_short, sizeof( short ), STEP_SIZE * nchan, f_input ) ) )
            {
                deinterleave_short2double( input_short, input, STEP_SIZE * nchan, nchan );
                scale(input, fac, input, STEP_SIZE * nchan );
                clip += interleave_double2short( input, input_short, STEP_SIZE * nchan, nchan );
                length_total += length / nchan;
                fwrite( input_short, sizeof( short ), length, f_output );
            }

            fprintf( stdout, "Input level:      %.6f\n", lev_input );
            fprintf( stdout, "Target level:     %.6f\n", lev_target );
            fprintf( stdout, "Obtained level:   %.6f\n", lev_obtained );
            fprintf( stdout, "Scaling factor:   %.6f\n", fac );
            fprintf( stdout, "\n--> Done processing %ld samples\n", length_total );
            if( clip > 0 )
            {
                fprintf( stderr, "*** Warning: %ld samples clipped\n", clip );
            }

            fclose( f_output );
        }
        else
        {
            /* No output file is specified -- find the input level */
            lev_input = gated_loudness_adaptive( gating_block_energy, 1.0, n_gating_blocks );
            fprintf( stdout, "Input level:      %.6f\n", lev_input );
            fprintf( stdout, "\n--> Done processing %ld samples\n", length_total );
        }
    }
    else
    {
        fprintf( stderr, "*** Warning: All non-LFE channels are zero\n" );
        if( f_output != NULL )
        {
            fprintf( stderr, "*** Scaling of zero input not possible, exiting ..\n" );
            exit( -1 );
        }
        else
        {
            fprintf( stdout, "Input level:      -Inf\n" );
            fprintf( stdout, "\n--> Done processing %ld samples\n", length_total );
        }
    }

    /* Close files */
    fclose( f_input );

    /* Deallocate memory */
    free( input );
    free( input_short );
    free( Bmem1 );
    free( Amem1 );
    free( Bmem2 );
    free( Amem2 );
    free( gating_block_energy );
    free( e_tmp );

}

