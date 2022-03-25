/*
    Implementation of ESDRU as defined in ITU-T Recommendation P.811

    Matches output of MATLAB version esdru.m, if the random number generator and cos 
    function is synchronized.

    See LICENSE.md for terms.

    Author: erik.norvell@ericsson.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define LOCAL_PI       3.14159265358979323846

void usage()
{
    fprintf( stdout, "esdru.exe [options] <alpha> <input file> <output file>\n" );
    fprintf( stdout, "\n" );
    fprintf( stdout, "<alpha>           Alpha value [0.0 ... 1.0]\n" );
    fprintf( stdout, "<input file>      Input file, 16 bit Stereo PCM\n" );
    fprintf( stdout, "<output file>     Output file, 16 bit Stereo PCM\n" );
    fprintf( stdout, "\n" );
    fprintf( stdout, "Options:\n" );
    fprintf( stdout, "-sf FS            Sampling frequency FS Hz (Default: 48000 Hz)\n" );
    fprintf( stdout, "-e_step S         Max step S during high energy [0.0 ... 1.0] (Default: 0.5)\n" );
    fprintf( stdout, "-seed I           Set random seed I [unsigned int] (Default: 1)\n" );

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
 * Compute energy of left + right
 * e( n ) = left( n ).^2 + right( n ).^2
 *-------------------------------------------------*/
void energy(
    const double *input,  /* i: Input signal       */
    double *e,            /* i: Output signal      */
    const long length     /* i: Length of signal   */
)
{
    long i;
    for( i = 0; i < length; i++ )
    {
        e[i] = input[2 * i] * input[2 * i] + input[2 * i + 1] * input[2 * i + 1];
    }

    return;
}

/*------------------------------------------------- 
 * First order one - pole iir filter of the form
 * y( n ) = fac * x( n ) + (1 - fac) * y( n - 1 )
 *-------------------------------------------------*/
void ar1(
    const double fac,     /* i: filter coefficient */
    const double *input,  /* i: Input signal       */
          double *output, /* i: Output signal      */
    const long length,    /* i: Length of signal   */
    const long dir        /* i: Direction (1, -1)  */
)
{
    long i;
    double mem;

    mem = 0.0;

    if( dir == 1 )
    { 
        for (i = 0; i < length; i++ )
        {
            output[i] = fac * input[i] + (1.0 - fac) * mem;
            mem = output[i];
        }
    }
    else
    {
        if ( dir == -1 )
        {
            for( i = length-1; i >= 0; i-- )
            {
                output[i] = fac * input[i] + (1.0 - fac) * mem;
                mem = output[i];
            }
        }
    }


    return;
}

void convert_short2double(
    short *input_short,  /* i: Input short signal     */
    double *input,       /* i: Input signal in double */
    const long length    /* i: Length of signal       */
)
{
    long i;
    for ( i = 0; i < length; i++ )
    {
        input[i] = (double)(input_short[i]);
    }

    return;
}

long convert_double2short( /* returns number of clipped samples */
    double *input,         /* i: Input signal in double         */
    short *input_short,    /* i: Input signal in short          */
    const long length      /* i: Length of signal               */
)
{
    long i;
    long clip;
    clip = 0;
    for( i = 0; i < length; i++ )
    {
        if( input[i] > 32767.0f )
        {
            clip++;
            input_short[i] = 32767;
        }
        else
        {
            if( input[i] < -32768.0f )
            {
                clip++;
                input_short[i] = -32768;
            }
            else
            {
                if ( input[i] > 0.0 ) 
                {
                    input_short[i] = (short)(input[i] + 0.5f);
                }
                else 
                {
                    input_short[i] = (short)(input[i] - 0.5f);
                }
            }
        }
    }

    return clip;
}

void g_mod_nrg(
    const double *input, /* i: Stereo input signal                 */
    const long length,   /* i: Length of input signal in samples   */
    const long step,     /* i: Length of transition in samples     */
    const double e_step, /* i: Energy step in high energy segments */
          double *m      /* o: Modulation curve                    */
)
{
    long i, j, M;
    double *e,*es,*el;
    double m_prev, m_delta, m_new;
    double xf_win;
    
    e = malloc( sizeof( double )*length );
    es = malloc( sizeof( double )*length );
    el = malloc( sizeof( double )*length );

    M = length / step + ((length % step) > 0); /* M = ceil(length/step) */

    energy( input, e, length );

    ar1( 0.001, e, es, length, -1 );
    ar1( 0.001, es, es, length, 1 );
    ar1( 0.0001, es, el, length, -1 );
    ar1( 0.0001, el, el, length, 1 );
    scale( el, 0.77813, el, length );

    i = 0;
    m_prev = 1.0;
    while( i < length )    
    {
        if( (rand() / ((double)RAND_MAX)) < 0.2 )
        { 
            if( es[i] < el[i] )
            {
                m_delta = 1.0;
            }
            else
            {
                m_delta = e_step;
            }
            m_new = rand() / ((double)RAND_MAX) * m_delta + m_prev * (1.0 - m_delta);
        }
        else
        {
            m_new = m_prev;
        }

        for(j = 0; j < step && i < length; i++,j++ )
        {
            xf_win = 0.5 * (1.0 - cos( LOCAL_PI * j / step ));
            m[i] = m_new * xf_win + m_prev * (1.0 - xf_win);
        }
        m_prev = m_new;
    }

    free( e );
    free( es );
    free( el );

    return;
}

void apply_spatial_dist(
          double *input, /*i/o: Input signal / Output signal with spatial distortion */
    const long length,   /*  i: Length of input signal in samples                    */
    const double *m,     /*  i: Modulating function                                  */
    const double alpha   /*  i: Modulation strength [0.0 ... 1.0]                    */
)
{
    double g1,g2,left,right;
    long i;

    for (i = 0; i < length; i++ )
    {
        g1 = 1.0f - m[i] * (1.0f - alpha);
        g2 = 1.0f + alpha - g1;
        left  = input[2 * i];
        right = input[2 * i + 1];
        /* y = [(x(:,1)*alpha + x(:,2)*(1-alpha)).*g1 (x(:,1)*(1-alpha) + x(:,2)*alpha).*g2 ]; */
        input[2 * i] = (left * alpha + right * (1.0f - alpha)) * g1;
        input[2 * i + 1] = (left * (1.0f - alpha) + right * alpha) * g2;
    }

    return;
}

int main(int argc, char **argv )
{
    FILE* f_input;
    FILE* f_output;
    char *input_filename;
    char *output_filename;
    double *input;
    short *input_short;
    unsigned int seed;
    double *m;
    double alpha;
    double e_step;
    long step;
    long length;
    long fs;
    long clip;
    long i;

    fs = 48000;   /* Default sampling frequency */
    e_step = 0.5; /* Default allowed modulation step during high energy segments */
    seed = 1;     /* Default seed for srand (same as implicitly run) */
    i = 1;

    /* Command line parsing */
    if (argc == 1)
    {
        usage();
    }
    
    /* Process options */
    while( argv[i][0] == '-' )
    {
        if( strcmp( argv[i], "-sf" ) == 0)
        {
            if(sscanf( argv[i+1], "%ld", &fs ) != 1)
            {
                fprintf(stderr, "Invalid sampling frequency %s, exiting..\n", argv[i + 1] );
                usage();
            }
            i += 2;
        }
        else if( strcmp( argv[i], "-e_step" ) == 0 )
        {
            if( sscanf( argv[i + 1], "%lf", &e_step ) != 1 )
            {
                fprintf( stderr, "Invalid e_step %s, exiting..\n", argv[i + 1] );
                usage();
            }
            else
            {
                if( e_step < 0.0 || e_step > 1.0 )
                {
                    fprintf( stderr, "Invalid e_step value %s, exiting..\n\n", argv[i + 1] );
                    usage();
                }
            }
            i += 2;
        }
        else if( strcmp( argv[i], "-seed" ) == 0 )
        {
            if( sscanf( argv[i + 1], "%u", &seed ) != 1 )
            {
                fprintf( stderr, "Invalid seed %s, exiting..\n", argv[i + 1] );
                usage();
            }
            i += 2;
        }
        else
        {
            fprintf( stderr, "Invalid option %s, exiting..\n", argv[i] );
            usage();
        }
    }

    /* Process mandatory arguments */
    if ((argc-i) != 3 )
    {
        fprintf( stderr, "Not enough input arguments, exiting..\n" );
        usage();
    }

    if( (sscanf( argv[i], "%lf", &alpha )) != 1 )
    {
        fprintf( stderr, "Invalid alpha value %s, exiting..\n\n", argv[i] );
        usage();
    }
    else
    {
        if( alpha < 0.0 || alpha > 1.0 )
        {
            fprintf( stderr, "Invalid alpha value %s, exiting..\n\n", argv[i] );
            usage();
        }
        i++;
    }
    input_filename = argv[i++];
    output_filename = argv[i];
    if( (f_input = fopen( input_filename, "rb" )) == NULL )
    {
        fprintf( stderr, "Could not open input file %s, exiting..\n\n", input_filename );
        usage();
    }
    if( (f_output = fopen( output_filename, "wb" )) == NULL )
    {
        fprintf( stderr, "Could not open output file %s, exiting..\n\n", output_filename );
        usage();
    }

    /* Print settings */
    fprintf( stdout, "Input file:       %s\n", input_filename );
    fprintf( stdout, "Output file:      %s\n", output_filename );
    fprintf( stdout, "alpha:            %f\n", alpha );
    fprintf( stdout, "e_step:           %f\n", e_step );
    fprintf( stdout, "seed:             %u\n", seed );
    fprintf( stdout, "Sampling rate:    %ld\n", fs );

    /* Set random seed */
    srand( seed );

    /* Load input file */
    fseek( f_input, 0L, SEEK_END );
    length = ftell( f_input ) / 4; /* 2 bytes per sample, 2 channels */
    rewind( f_input );
    input = malloc(sizeof(double) * length * 2);
    input_short = malloc( sizeof( short ) * length * 2 );
    m = malloc( sizeof( double ) * length );
    fread( input_short, sizeof(short), length * 2, f_input);
    convert_short2double( input_short, input, length * 2);

    step = (long) (1.5 * fs / 50.0);
    g_mod_nrg( input, length, step, e_step, m );

    apply_spatial_dist( input, length, m, alpha );

    clip = convert_double2short(input, input_short, length * 2);

    fwrite( input_short, sizeof( short ), length * 2, f_output );

    fprintf( stdout, "--> Done processing %ld samples\n", length );
    if (clip > 0)
    {
        fprintf( stderr, "*** Warning: %ld samples clipped\n", clip );
    }

    fclose( f_input );
    fclose( f_output );
    free( input );
    free( input_short );
    free( m );

}

