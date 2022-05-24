#define MNRU_DEFINED 200

/* Smart function prototypes: for [ag]cc, VaxC, and [tb]cc */
#if !defined(ARGS)
#if (defined(__STDC__) || defined(VMS) || defined(MSDOS))
#define ARGS(s) s
#else
#define ARGS(s) ()
#endif
#endif

#define RANDOM_state new_RANDOM_state
#define random_MNRU new_random_MNRU

// --------------------------------- BG_MNRU --------------------------------

/* Definitions for the MNRU algorithm */
#define MOD_NOISE    1
#define NOISE_ONLY   0
#define SIGNAL_ONLY -1

#define MNRU_START     1
#define MNRU_CONTINUE  0
#define MNRU_STOP     -1

#define RANDOM_RUN 0
#define RANDOM_RESET 1

#define S1    -8.0      /* s1 = my - 4 * sigma (=-8.0 for gaussian noise) */
#define S2     8.0      /* s2 = my + 4 * sigma (= 8.0 for gaussian noise) */
#define DIF    16.0     /* s2 - s1                                        */
#define MO     8.0      /* mo = 2 * (sigma)^2  (= 8.0 for gaussian noise) */
#define BIT15  32767.0
#define TABLE_SIZE 8192 /* 2^13 */
#define ITER_NO 8
#define FACTOR 8  /* = 65536(max.no returned by ran16_32c) div.by TABLE_SIZE */

#define DNULL (double *)0

#define CONST         69069
#define INIT          314159265L
#define BIT32         4294967296.0
#define BIT24			 16777216.0
#define BIT8    		 256.0

#define ALPHA 0.985
#define DNULL (double *)0

//These are the old noise gains for MNRU_process().
//These values expect the output lowpass to be enabled
/* NOISE_GAIN = 0.3795 for best match with the average SNR */
/*              0.3787 for best best match with the total SNR */
/*              0.3793 for a "balanced" middle-way between both SNRs */
//#define NOISE_GAIN 0.3793

//These are the new noise gains for MNRU_process_new(), which filters
//the noise in accordance with P.50
/*NOISE_GAIN = 3.1010 for best match with the average RMS power
               2.9580 for best match with the total RMS power
					3.0287 for a middle-way between both values        */
#define NOISE_GAIN 3.0287
#define NO_OUT_FILTER

#include <math.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "filtering_routines.h"

#include "filtering_coeffs.h"

/* Definition of type for random_MNRU state variables */
typedef struct
{
  short   idum;
  short   inext, inextp;
  long    ma[56];	/* this is a special value; shall not be changed [1],[2] */
} ori_RANDOM_state;

/* Definition of type for random_MNRU state variables */
typedef struct
{
  float *gauss;

} new_RANDOM_state;

/* Definitions for the MNRU state variable */
#define MNRU_STAGE_OUT_FLT 2         /* number of 2nd-order stages in filter */

/* Definition of type for MNRU state variables */
typedef struct
{
  /* State variables related to the rnadom number generation */
  long            seed, clip;
  double          signal_gain, noise_gain;
  double         *vet, last_xk, last_yk, last_y20k_lp;
  RANDOM_state    rnd_state;	/* for random_MNRU() */
  char            rnd_mode;

  /* State variables related to the output band-pass filtering */
  double A[MNRU_STAGE_OUT_FLT][3];    /* numerator coefficients */
  double B[MNRU_STAGE_OUT_FLT][2];    /* denominator coefficients */
  double DLY[MNRU_STAGE_OUT_FLT][2];  /* delay storage elements (z-shifts) */
} MNRU_state;

/* Prototype for MNRU and random function(s) */
//#ifdef NO_PROMOTION
//    double *MNRU_process ARGS((char operation, MNRU_state *s, float *input, float *output, long n, long seed, char mode, double Q));
//#else
//    double *MNRU_process ARGS((short operation, MNRU_state *s, double *input, double *output, long n, long seed, short mode, double Q));
//#endif
//
//float random_MNRU ARGS((char *mode, RANDOM_state *r, long seed));


float ran_vax( void);
unsigned long ran16_32c( void);

double *MNRU_process(char operation, MNRU_state *s, double *input, double *output, long n, long seed, char mode, double Q);

double *MNRU_process_new(char operation, MNRU_state *s, double* input, double* output, long n, long seed, char mode, double Q);

float random_MNRU(char *mode, RANDOM_state * r, long seed);

void show_use(void)
{   printf("\n");
	 printf("\n      USE: p50fbmnru <inputfile> <outputfile> <Q/dB> <Mode>\n\n");
	 printf("\n           The noise is P.50-filtered. Requires 48kHz sampling rate.\n");
	 printf("\n           Mode M:   Modulated Noise");
	 printf("\n                N:   Noise only");
	 printf("\n                S:   Signal only\n");
	 exit(1);
}

int main(int argc, char *argv[])
{	MNRU_state			state;
	//short     				In, Out, B_Len;
	FILE					*In, *Out;
	short					B_Len, BuffLen;
	long					lFileLen = 0;
	int					i, B_Max;
	float					Q;
	short             operation, MNRU_mode;

	static short 			Buf[512];
	static double			In_Buf[512], Out_Buf[512];

	//Do inits to prevent crashing when option 'S' is selected
	state.rnd_state.gauss = NULL;
	state.vet = NULL;

	if( argc != 5 )	     show_use();

	 /* +++++++++++++++++  open input & output files  ++++++++++++++++++ */

	 //_fmode	=	O_BINARY;
	 //if( (In = open( argv[1], O_RDONLY) ) == -1 )
	 In = fopen( argv[1], "rb");
	 if( In == NULL )
	 {   printf(" can't open input file: %s \n", argv[1]);
		  exit(1);
	 }

	 Out = fopen( argv[2], "wb");
	 if( Out == NULL )
	 {   printf(" can't open output file: %s \n", argv[2]);
		  exit(1);
	 }

	 Q 	= 	(float)atof( argv[3]);

     char mrnumode = argv[4][0];
	 if( mrnumode == 'M' || mrnumode == 'm' ) MNRU_mode = MOD_NOISE;
	 if( mrnumode == 'N' || mrnumode == 'n' ) MNRU_mode = NOISE_ONLY;
	 if( mrnumode == 'S' || mrnumode == 's' ) MNRU_mode = SIGNAL_ONLY;

	 B_Len	=	512; BuffLen = B_Len / sizeof(short);
	 //B_Max	=	(short)	(filelength( In) / B_Len);
	 if(  fseek( In, 0, SEEK_END) == -1)  return( -1);
    if( (lFileLen =  ftell( In)) == -1)  return( -1);
    if(  fseek( In, 0, SEEK_SET) == -1)  return( -1);

	 B_Max	=	lFileLen / B_Len;

	 /* +++++++++++++++++++++++++  initialize  +++++++++++++++++++++++++ */
	 printf(" Input file ............ %s ", argv[1]);
	 printf("\n Output file ........... %s ", argv[2]);
	 printf("\n Q ..................... %g dB", Q);
	 if( MNRU_mode == MOD_NOISE) printf("\n Mode .................. Mod-Noise");
	 if( MNRU_mode == NOISE_ONLY) printf("\n Mode .................. Noise only");
	 if( MNRU_mode == SIGNAL_ONLY) printf("\n Mode .................. Signal only");
	 printf("\n blocks ................ %5d", B_Max);

// ==> Auswerteschleife
	//lseek( In, 0, SEEK_SET);
	if(  fseek( In, 0, SEEK_SET) == -1)  return( -1);

	operation	=	MNRU_START;

	do
	{	//B_Len	=	read( In, Buf, B_Len);
		BuffLen  = (short)fread(Buf, sizeof(short), BuffLen, In);
		if( BuffLen <= 0)
			break;

		for( i=0; i<BuffLen; i++)
		{	In_Buf[i]	=	(double) Buf[i];
			Out_Buf[i]	=	0;
		}

		MNRU_process_new( operation, &state, In_Buf, Out_Buf, (long) BuffLen
							, (long) 314159265, (char)MNRU_mode, Q);

		for( i=0; i<BuffLen; i++)
		{	//Out_Buf[i]	=	Out_Buf[i];
			if( Out_Buf[i]>0)		Buf[i]	=	(short) ( Out_Buf[i] +0.5);
			else					Buf[i]	=	(short) ( Out_Buf[i] -0.5);
		}

		//write( Out, Buf, B_Len);
		fwrite(Buf, sizeof(short), BuffLen, Out);

		if( operation==MNRU_START)	operation	=	MNRU_CONTINUE;

		printf("\b\b\b\b\b%5d", B_Max--);

	} while( B_Max >= 0);

	operation	=	MNRU_STOP;

	MNRU_process_new( operation, &state, In_Buf, Out_Buf, (long) 0
							, (long) 0, (char)0, 0);

	printf("\n\n ==> FERTIG ! \n\n");

	//close( In), close( Out);
	fclose(In); fclose(Out);

	return 0;
}

/*
  =============================================================================

	new_random_MNRU (char *mode, RANDOM_state *r, long seed)
        ~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Random number generator based on a gaussian random
	number table accessed by a uniform random number generator.

	The gaussian random sample table is generated at start-up time
        by a Monte-Carlo algorithm using a linear congruential generator
        (LCG). During run-time the algorithm accesses randomly this table
	using indeces generated by another LCG.

	To (re)initialize the sequence, use mode=RANDOM_RESET (the routine
	will change mode to RANDOM_RUN).

	Functions used:
	~~~~~~~~~~~~~~~
        ran_vax():    generate uniformly distributed samples in the range
                      0..1. Return a float number.
	ran16_32C():  generate uniformly distributed samples in the range
	              0..65535 (2^16-1)

        Prototype: MNRU.H
        ~~~~~~~~~~

        History:
        ~~~~~~~~
        01.Jul.95  1.0	Created based on the random number generator
                        implemented by Aachen University and used by
                        the ITU-T 8kbit/s speech codec host laboratory
                        (in hardware). <simao@ctd.comsat.com>

=============================================================================
*/
float           new_random_MNRU(char *mode, RANDOM_state *r, long seed)
{

  long    i;
  double  z1;            /* white random number   -8...8          */
                         /* weighted with a gaussian distribution */
  double  z2;            /* white random number    0...1          */
  double  phi;           /* gauss curve                           */

  long index;

  /* *** RUN INITIALIZATION SEQUENCE *** */
  if (*mode == RANDOM_RESET)	/* then reset sequence */
  {
    /* Toogle mode from reset to run */
    *mode = RANDOM_RUN;

    /* Allocate memory for gaussian table */
    r->gauss = (float *)calloc(TABLE_SIZE, sizeof(float));

    /* Generate gaussian random number table */
    for(i=0L; i<TABLE_SIZE; i++)
    {
      /* Interact until find gaussian sample */
      do
      {
			z1 = S1 + DIF*(double)ran_vax();
			phi= exp( -(z1)*(z1)/MO);
			z2 = (double)ran_vax();
      } while(z2 > phi);

      /* Save gaussian-distributed sample in table */
      r->gauss[i] = (float)z1;
    }
  }

  /* ***  REAL GENERATOR (after initialization) ***/
  for (z1=0, i=0;i<ITER_NO;i++)
  {
    index = ran16_32c()/FACTOR;
    z1 += r->gauss[index];
  }
  z1 /= 2; /* provisional */

  /* Return gaussian sample */
  return ((float)z1);
}
/*  .................... End of new_random_MNRU() ....................... */


/*
  ===========================================================================
  float ran_vax(void);
  ~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Function that simulates the VAX Fortran function RAN(x), that returns
  a number uniformly distributed between 0.0 and 1.0. This implementation
  is based on Aachen University's randm() function of the narrow-band
  MNRU table-generation program montrand.c by CA (6.3.90).

  Parameters: none.
  ~~~~~~~~~~~

  Return value:
  ~~~~~~~~~~~~~
  An float number uniformly distributed in the range 0.0 and 1.0.

  Author:
  ~~~~~~~
  Simao Ferraz de Campos Neto
  Comsat Laboratories                  Tel:    +1-301-428-4516
  22300 Comsat Drive                   Fax:    +1-301-428-9287
  Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

  History:
  ~~~~~~~~
  01.Jul.95  v1.00  Created, adapted from montrand.c

  ===========================================================================
*/

float ran_vax()
{
  static unsigned long  seed, buffer;
  static float          ran;
  static short          firsttime=0;

  if(firsttime == 0)
  {
  	firsttime = 1;
  	seed      = INIT;
  }

  seed    = seed * CONST + 1;         /* includes the mod 2**32 operation  */
  buffer  = seed & 0xFFFFFF00;        /* mask the first 24 bit             */
  ran     = (float)buffer / BIT32;    /* and divide by 2**32 to get random */

  return(ran);
}
/*  ......................... End of ran_vax() ............................ */


/*
  ===========================================================================
  unsigned long ran16_32c(void);
  ~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Function that simulates the DSP32C function RAN24(), modified to return
  a number between 0 and 2^16-1. This is based on Aachen University's
  randm() of the narrow-band MNRU program mnrusim.c by PB (08.04.1991).

  Parameters: none.
  ~~~~~~~~~~~

  Return value:
  ~~~~~~~~~~~~~
  An unsigned long number in the range 0 and 2^16-1.

  Author:
  ~~~~~~~
  Simao Ferraz de Campos Neto
  Comsat Laboratories                  Tel:    +1-301-428-4516
  22300 Comsat Drive                   Fax:    +1-301-428-9287
  Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

  History:
  ~~~~~~~~
  01.Jul.95  v1.00  Created, adapted from mnrusim.c

  ===========================================================================
*/
unsigned long ran16_32c()
{
  static float   seed = 12345.0;
  double         buffer1, buffer2;
  long           seedl;
  unsigned long  result;

  buffer1 = ((253.0 * seed) + 1.0);
  buffer2 = (buffer1/BIT24) ;
  seedl   = ((long)buffer2) & 0x00FFFFFFL;
  seed    = buffer1 = buffer1 - (float)seedl * BIT24;
  result  = buffer1 / BIT8;

  return result;
}
/*  .................... End of ran16_32c() ....................... */

/* *********************** STL92_RNG ****************************** */



/*
  ==========================================================================

        double *MNRU_process (char operation, MNRU_state *s,
        ~~~~~~~~~~~~~~~~~~~~  float *input, float *output,
                              long n, long seed, char mode, double Q)

        Description:
        ~~~~~~~~~~~~

        Module for addition of modulated noise to a vector of `n' samples,
        according to CCITT Recommendation P.81 (Blue Book), for the
        narrow-band model. Depending on the `mode', it:

        - add modulated noise to the `input' buffer at a SNR level of
          `Q' dB, saving to `output' buffer (mode==MOD_NOISE);

        - put into `output' only the noise, without adding to the original
          signal (mode==NOISE_ONLY);

        - copy to `output' the `input' samples (mode==SIGNAL_ONLY);

        There is the need of state variables, which are declared in MNRU.H.
        These are reset calling the function with the argument `operation'
        set as MNRU_START. In the last call of the function, call it with
        operation=MNRU_STOP, to release the memory allocated for the
        processing. Normal operation is followed when operation is set as
        MNRU_CONTINUE.

        Valid inputs are:
        operation:    MNRU_START, MNRU_CONTINUE, MNRU_STOP (see description
                      above; defined in MNRU.H);
        s:	      a pointer to a structure defined as MNRU_state, as in
        	      MNRU.H;
        input:        pointer to input float-data vector; must represent
                      8 kHz speech samples.
        output:	      pointer to output float-data vector; will represent
                      8 kHz speech samples.
        n:	      long with the number of samples (float) in input;
        seed:	      initial value for random number generator;
        mode:	      operation mode: MOD_NOISE, SIGNAL_ONLY, NOISE_ONLY
        	      (see description above; defined in MNRU.H);
        Q:	      double defining the desired value for the signal-to-
                      modulated-noise for the output data.

        ==================================================================
        NOTE! New values of `seed', `mode' and `Q' are considered only
              when operation==MNRU_START, because they are considered as
              INITIAL state values.
        ==================================================================

        For more details on the algorithm, see the documentation related.


        Return Value:
        ~~~~~~~~~~~~~
        Returns a (double *)NULL if uninitialized or if initialization
        failed; returns a (double *) to the 20 kHz data vector if reset was
        OK and/or is in "run" (MNRU_CONTINUE) operation.


        History:
        ~~~~~~~~
        05.Feb.1992     1.10 Release of the modular version.
                             <tdsimao@venus.cpqd.ansp.br>
        05.Feb.1992     2.00 Updated according to the new P.81:
                             - no up/downsampling
			     - input signal DC removal filter
			     - output low-pass filter (instead of band-pass)
			     <simao@ctd.comsat.com>

  ==========================================================================
*/
/* original RPELTP: #define ALPHA 0.999 */

double *MNRU_process(char operation, MNRU_state *s, double* input, double* output, long n, long seed, char mode, double Q)
{
  long            count, i;
  double          noise, tmp;
  register double inp_smp, out_tmp, out_flt;


  /*
   *    ..... RESET PORTION .....
   */

  /* Check if is START of operation: reset state and allocate memory buffer */
  if (operation == MNRU_START)
  {

    /* Reset clip counter */
    s->clip = 0;

    /* Allocate memory for sample's buffer */
   //if ((s->vet = (double *) calloc(n, sizeof(double))) == DNULL)
      //return ((double *) DNULL);
	 if ((s->vet = (double *) calloc(n, sizeof(double))) == NULL)
      return (NULL);

    /* Seed for random number generation */
    s->seed = seed;

    /* Gain for signal path */
    if (mode == MOD_NOISE)
      s->signal_gain = 1.000;
    else if (mode == SIGNAL_ONLY)
      s->signal_gain = 1.000;
    else			/* (mode == NOISE_ONLY) */
      s->signal_gain = 0.000;

    /* Gain for noise path */
    if (mode == MOD_NOISE || mode == NOISE_ONLY)
      s->noise_gain = NOISE_GAIN * pow(10.0, (-0.05 * Q));
    else			/* (mode == SIGNAL_ONLY) */
      s->noise_gain = 0;

    /* Flag for random sequence initialization */
    s->rnd_mode = RANDOM_RESET;

    /* Initialization of the output low-pass filter */
    /* Cleanup memory */
    memset(s->DLY, '\0', sizeof(s->DLY));

#ifdef NBMNRU_MASK_ONLY
    /* Load numerator coefficients */
    s->A[0][0]= 0.758717518025; s->A[0][1]= 1.50771485802; s->A[0][2]= 0.758717518025;
    s->A[1][0]= 0.758717518025; s->A[1][1]= 1.46756552150; s->A[1][2]= 0.758717518025;

    /* Load denominator coefficients */
    s->B[0][0]= 1.16833932919; s->B[0][1]= 0.400250061172;
    s->B[1][0]= 1.66492368687; s->B[1][1]= 0.850653444434;
#else
    /* Load numerator coefficients */
    s->A[0][0]= 0.775841885724; s->A[0][1]= 1.54552788762; s->A[0][2]= 0.775841885724;
    s->A[1][0]= 0.775841885724; s->A[1][1]= 1.51915539326; s->A[1][2]= 0.775841885724;

    /* Load denominator coefficients */
    s->B[0][0]= 1.23307153957; s->B[0][1]= 0.430807372835;
    s->B[1][0]= 1.71128410940; s->B[1][1]= 0.859087959597;
#endif

    /* Initialization of the input DC-removal filter */
    s->last_xk = s->last_yk = 0;
  }

  /*
   *    ..... REAL MNRU WORK .....
   */

  /* Initialize memory and upsample by a factor of 5 */
  memset(s->vet, '\0', n * sizeof(double));

  for (count = 0; count < n; count++)
  {
    /* Copy sample to local variable */
    inp_smp = *input++;

#ifndef NO_DC_REMOVAL
    /* Remove DC from input sample: H(z)= (1-Z-1)/(1-a.Z-1) */
    tmp = inp_smp - s->last_xk;
    tmp += ALPHA * s->last_yk;

    /* Update for next time */
    s->last_xk = inp_smp;
    s->last_yk = tmp;

    /* Overwrite DC-removed version of the input signal */
    inp_smp = tmp;
#endif

    /* Random number generation */
    if (mode == SIGNAL_ONLY)
      noise = 0;
    else
    {
      noise = (double) random_MNRU(&s->rnd_mode, &s->rnd_state, s->seed);
      noise *= s->noise_gain * inp_smp;	/* noise modulated by input sample */
      if (noise>1.00 || noise <-1.00) s->clip++; /* clip counter */
    }

    /* Addition of signal and modulated noise */
    out_tmp  = noise + inp_smp * s->signal_gain;

#ifdef NO_OUT_FILTER
    out_flt = out_tmp;
#else
    /* Filter output sample by each stage of the low-pass IIR filter */
    for (i=0; i<MNRU_STAGE_OUT_FLT; i++)
    {
        out_flt = out_tmp * s->A[i][0] + s->DLY[i][1];
        s->DLY[i][1] = out_tmp * s->A[i][1] - out_flt * s->B[i][0] +
	               s->DLY[i][0];
        s->DLY[i][0] = out_tmp * s->A[i][2] - out_flt * s->B[i][1];

        out_tmp = out_flt;  /* output becomes input for next stage */
    }
#endif

    /* Copy noise-modulated speech sample to output vector */
    *output++  = out_flt;
  }

  /* Check if is end of operation THEN release memory buffer */
  if (operation == MNRU_STOP)
  {
	 if (s->rnd_state.gauss != NULL)	free(s->rnd_state.gauss);
	 if (s->vet != NULL)					free(s->vet);
    //s->vet = (double *) DNULL;
	 s->rnd_state.gauss = NULL;
	 s->vet = NULL;
  }

  /* Return address of vet: if NULL, nothing is allocated */
  return ((double *) s->vet);
}

/*  .................... End of MNRU_process() ....................... */




/*
  ==========================================================================

        double *MNRU_process_new (char operation, MNRU_state *s,
        ~~~~~~~~~~~~~~~~~~~~~~~~  float *input, float *output,
                                  long n, long seed, char mode, double Q)


!		  New version shapes the noise according to the P.50 spectrum density
!		  before adding it to the signal.

        Description:
        ~~~~~~~~~~~~

        Module for addition of modulated noise to a vector of `n' samples,
        according to CCITT Recommendation P.81 (Blue Book), for the
        narrow-band model. Depending on the `mode', it:

        - add modulated noise to the `input' buffer at a SNR level of
          `Q' dB, saving to `output' buffer (mode==MOD_NOISE);

        - put into `output' only the noise, without adding to the original
          signal (mode==NOISE_ONLY);

        - copy to `output' the `input' samples (mode==SIGNAL_ONLY);

        There is the need of state variables, which are declared in MNRU.H.
        These are reset calling the function with the argument `operation'
        set as MNRU_START. In the last call of the function, call it with
        operation=MNRU_STOP, to release the memory allocated for the
        processing. Normal operation is followed when operation is set as
        MNRU_CONTINUE.

        Valid inputs are:
        operation:    MNRU_START, MNRU_CONTINUE, MNRU_STOP (see description
                      above; defined in MNRU.H);
        s:	      a pointer to a structure defined as MNRU_state, as in
        	      MNRU.H;
        input:        pointer to input float-data vector; must represent
                      8 kHz speech samples.
        output:	      pointer to output float-data vector; will represent
                      8 kHz speech samples.
        n:	      long with the number of samples (float) in input;
        seed:	      initial value for random number generator;
        mode:	      operation mode: MOD_NOISE, SIGNAL_ONLY, NOISE_ONLY
        	      (see description above; defined in MNRU.H);
        Q:	      double defining the desired value for the signal-to-
                      modulated-noise for the output data.

        ==================================================================
        NOTE! New values of `seed', `mode' and `Q' are considered only
              when operation==MNRU_START, because they are considered as
              INITIAL state values.
        ==================================================================

        For more details on the algorithm, see the documentation related.


        Return Value:
        ~~~~~~~~~~~~~
        Returns a (double *)NULL if uninitialized or if initialization
        failed; returns a (double *) to the 20 kHz data vector if reset was
        OK and/or is in "run" (MNRU_CONTINUE) operation.


        History:
        ~~~~~~~~
        05.Feb.1992     1.10 Release of the modular version.
                             <tdsimao@venus.cpqd.ansp.br>
        05.Feb.1992     2.00 Updated according to the new P.81:
                             - no up/downsampling
			     - input signal DC removal filter
			     - output low-pass filter (instead of band-pass)
			     <simao@ctd.comsat.com>

		  08.Mar.2007		Now shapes the noise according to P.50 before adding
								it to the signal. This requires 48kHz-sampled signals

  ==========================================================================
*/
/* original RPELTP: #define ALPHA 0.999 */

double *MNRU_process_new(char operation, MNRU_state *s, double* input, double* output, long n, long seed, char mode, double Q)
{
  long            count;
  double          tmp;

  //Variables needed for filtering functions
  static double					*delayLine_FIR, *delayLine_IIR;
  static double					*filteredNoiseTemp;


  /*
   *    ..... RESET PORTION .....
   */

  /* Check if is START of operation: reset state and allocate memory buffer */
  if (operation == MNRU_START)
  {

    /* Reset clip counter */
    s->clip = 0;

    /* Allocate memory for sample's buffer */
   //if ((s->vet = (double *) calloc(n, sizeof(double))) == DNULL)
      //return ((double *) DNULL);
	 if ((s->vet = (double *) calloc(n, sizeof(double))) == NULL)
      return (NULL);
	 if ((filteredNoiseTemp = (double *) calloc(n, sizeof(double))) == NULL)
      return (NULL);

    /* Seed for random number generation */
    s->seed = seed;

    /* Gain for signal path */
    if (mode == MOD_NOISE)
      s->signal_gain = 1.000;
    else if (mode == SIGNAL_ONLY)
      s->signal_gain = 1.000;
    else			/* (mode == NOISE_ONLY) */
      s->signal_gain = 0.000;

    /* Gain for noise path */
    if (mode == MOD_NOISE || mode == NOISE_ONLY)
      s->noise_gain = NOISE_GAIN * pow(10.0, (-0.05 * Q));
    else			/* (mode == SIGNAL_ONLY) */
      s->noise_gain = 0;

    /* Flag for random sequence initialization */
    s->rnd_mode = RANDOM_RESET;

    /* Initialization of the output low-pass filter */
    /* Cleanup memory */
    memset(s->DLY, '\0', sizeof(s->DLY));

#ifdef NBMNRU_MASK_ONLY
    /* Load numerator coefficients */
    s->A[0][0]= 0.758717518025; s->A[0][1]= 1.50771485802; s->A[0][2]= 0.758717518025;
    s->A[1][0]= 0.758717518025; s->A[1][1]= 1.46756552150; s->A[1][2]= 0.758717518025;

    /* Load denominator coefficients */
    s->B[0][0]= 1.16833932919; s->B[0][1]= 0.400250061172;
    s->B[1][0]= 1.66492368687; s->B[1][1]= 0.850653444434;
#else
    /* Load numerator coefficients */
    s->A[0][0]= 0.775841885724; s->A[0][1]= 1.54552788762; s->A[0][2]= 0.775841885724;
    s->A[1][0]= 0.775841885724; s->A[1][1]= 1.51915539326; s->A[1][2]= 0.775841885724;

    /* Load denominator coefficients */
    s->B[0][0]= 1.23307153957; s->B[0][1]= 0.430807372835;
    s->B[1][0]= 1.71128410940; s->B[1][1]= 0.859087959597;
#endif

    /* Initialization of the input DC-removal filter */
    s->last_xk = s->last_yk = 0;


	 //Init filter delay lines and state variables
	 if ((delayLine_FIR = (double *)calloc(iFIRcoeffsLen, sizeof(double))) == NULL)
		 return NULL;
	 if ((delayLine_IIR = (double *)calloc(iIIRorder,     sizeof(double))) == NULL)
		 return NULL;
  }

  /*
   *    ..... REAL MNRU WORK .....
   */

  if (operation != MNRU_STOP)
  {
	  //skip everything if mode == SIGNAL_ONLY
	  if (mode == SIGNAL_ONLY)
	  {
		  for (count = 0; count < n; count++)
			  output[count] = input[count];
		  return s->vet;
	  }


	  /* Initialize memory and upsample by a factor of 5 */
	  memset(s->vet, '\0', n * sizeof(double)); //originally unused, now used for storing values from RNG
	  memset(filteredNoiseTemp, 0, n * sizeof(double));

	  //Fill noise array
	  for (count = 0; count < n; count++)
	  {
		 /* Random number generation */
		 if (mode == SIGNAL_ONLY)
			//noise = 0;
			s->vet[count] = 0;
		 else
		 {
			//noise = (double) random_MNRU(&s->rnd_mode, &s->rnd_state, s->seed);
			s->vet[count] = (double) random_MNRU(&s->rnd_mode, &s->rnd_state, s->seed);
		 }
	  }

	  for (count = 0; count < n; count++)
		  s->vet[count] *= s->noise_gain;

	  //Filter the noise according to P.50
	  //Two cascaded filters for P.50 filter:
	  //An IIR highpass filter, followed by a FIR lowpass filter.

	  //First, filter the data in s->vet using an IIR filter, and store the result in filteredNoiseTemp
	  filterFunc_IIR(s->vet, filteredNoiseTemp, n, dIIRcoeffs, iIIRorder, delayLine_IIR);

	  //Second, filter the data in filteredNoiseTemp using an FIR filter and store the result in s->vet
	  filterFunc_FIR(filteredNoiseTemp, s->vet, n, dFIRcoeffs, iFIRcoeffsLen, delayLine_FIR);


#ifndef NO_DC_REMOVAL
	  for (count = 0; count < n; count++)
	  {
		 /* Remove DC from input sample: H(z)= (1-Z-1)/(1-a.Z-1) */
		 tmp = input[count] - s->last_xk;
		 tmp += ALPHA * s->last_yk;

		 /* Update for next time */
		 s->last_xk = input[count];
		 s->last_yk = tmp;

		 /* Overwrite DC-removed version of the input signal */
		 input[count] = tmp;
	  }
#endif

	 //Add the modulated noise to the signal
	 for (count = 0; count < n; count++)
		 output[count] = input[count] * (s->signal_gain + s->vet[count]);

  }
  else //operation == MNRU_STOP
  {
	 if (s->rnd_state.gauss != NULL)	free(s->rnd_state.gauss);
	 if (s->vet != NULL)				free(s->vet);
    //s->vet = (double *) DNULL;
	 s->rnd_state.gauss = NULL;
	 s->vet = NULL;

	 if (filteredNoiseTemp)	free(filteredNoiseTemp);
	 filteredNoiseTemp = NULL;

	 //Release filter delay lines and state variables
	 if (delayLine_FIR)	free(delayLine_FIR);
	 if (delayLine_IIR)	free(delayLine_IIR);
	 delayLine_FIR = delayLine_IIR = NULL;
  }

  /* Return address of vet: if NULL, nothing is allocated */
  return ((double *) s->vet);
}

/*  .................... End of MNRU_process() ....................... */
