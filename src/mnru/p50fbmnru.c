#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ... Include MNRU module ... */
#include "mnru.h"

/* ... Include of utilities ... */
#include "ugst-utl.h"

#define RANDOM_state new_RANDOM_state
#define random_MNRU new_random_MNRU

/* Defines minimum and maximum value for a short int. */
#define SHRT_MIN -32768
#define SHRT_MAX +32767

void show_use(void)
{
     printf("P.50 Fullband MNRU - %s\n", VERSION_STL);
     printf("\n  P.50 Fullband MNRU shapes the gaussian noise with an average speech power spectrum as ITU-T Rec. P.50.");

     printf("\n  Requires 48kHz sampling rate.\n");

	 printf("\n  Usage: p50fbmnru <inputfile> <outputfile> <Q/dB> <Mode> [dcFilter] [--overflow]\n");

	 printf("\n      Mode M:   Modulated Noise");
	 printf("\n           N:   Noise only");
	 printf("\n           S:   Signal only\n");
	 printf("\n      Options:");
     printf("\n         dcFilter  0 to disable DC Filter (default)");
     printf("\n                   1 for legacy DC removal filter (115 Hz -3 dB cutoff, same as P.50 MNNU prior 2023)");
     printf("\n                   2 for legacy DC removal filter (60 Hz -3 dB cutoff)");
     printf("\n                   3 for legacy DC removal filter (30 Hz -3 dB cutoff)");
     printf("\n                   4 for legacy DC removal filter (15 Hz -3 dB cutoff)");
     printf("\n ");
     printf("\n         --overflow  int16 overflow (legacy, same as P.50 MNNU prior 2023)");
     printf("\n                     if undefined, int16 are clamped (default)");
     printf("\n\n");
}

int main(int argc, char *argv[])
{
    MNRU_state      state;
    FILE            *In, *Out;
    char            dcFilterMode ;
    short           B_Len, BuffLen;
    long            lFileLen = 0;
    int             i, B_Max;
    float           Q;
    short           operation, MNRU_mode;
	static short 	Buf[512];
	static double	In_Buf[512], Out_Buf[512];

	// Initial seed for random number generator, ran16_32c( float *seed )
	float fseed  = 12345.0;

	long    lOverflowCnt = 0;
	char    overflowEnabled = 0;

	//Do inits to prevent crashing when option 'S' is selected
	state.rnd_state.gauss = NULL;
	state.vet = NULL;

    if ( (argc > 7) || (argc < 5)) {
        show_use();
        exit(1);
    }
    else if (argc  > 5) {
        for (i = 5; i < argc; i++) {
            if ( strcmp(argv[i], "--overflow") == 0) {
                overflowEnabled = 1;
            }
            else if (strlen(argv[i]) == 1) {
                dcFilterMode = (char) atoi(argv[i]);
            }
            else {
                show_use();
                printf("\nUnrecognized parameter: %s\n", argv[i]);
                exit(1);
            }
        }
    }
    else {
        dcFilterMode = 0;
    }

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

    if (dcFilterMode == 0) {
        printf("\nDC-removal filter mode:  disabled\n");
    }
    else {
        printf("\nDC-removal filter mode:  %d\n", dcFilterMode);
    }

	if(  fseek( In, 0, SEEK_SET) == -1)  return( -1);

	operation	=	MNRU_START;

	do
	{
		BuffLen  = (short) fread(Buf, sizeof(short), BuffLen, In);
		if( BuffLen <= 0)
			break;

		for( i=0; i<BuffLen; i++)
		{	In_Buf[i]	=	(double) Buf[i];
			Out_Buf[i]	=	0;
		}

		P50_MNRU_process( operation, &state, In_Buf, Out_Buf, (long) BuffLen,  (char) MNRU_mode, Q, dcFilterMode, &fseed);

		for( i=0; i<BuffLen; i++)
		{

			if ( Out_Buf[i]>0) {
                /* Check for potential int16 overflow */
                if (Out_Buf[i]  + 0.5 > SHRT_MAX) {
                    lOverflowCnt++;
                    if (overflowEnabled == 0) {
                        Buf[i] = SHRT_MAX;
                    }
                    else {
                        Buf[i]	= (short) ( Out_Buf[i] + 0.5);
                    }
                }
                else {
                    Buf[i]	= (short) ( Out_Buf[i] + 0.5);
                }
            }
			else
			{
                if (Out_Buf[i]  - 0.5 < SHRT_MIN) {
                    lOverflowCnt++;
                    if (overflowEnabled == 0) {
                        Buf[i] = SHRT_MIN;
                    }
                    else {
                        Buf[i] = (short) ( Out_Buf[i] - 0.5);
                    }
                }
                else {
                    Buf[i] = (short) ( Out_Buf[i] - 0.5);
                }
		    }
		}

		fwrite(Buf, sizeof(short), BuffLen, Out);

		if( operation==MNRU_START)	operation	=	MNRU_CONTINUE;

		printf("\b\b\b\b\b%5d", B_Max--);

	} while( B_Max >= 0);

	operation	=	MNRU_STOP;

	P50_MNRU_process( operation, &state, In_Buf, Out_Buf, (float) 0, (char)0, 0, 0, NULL);

    if (lOverflowCnt > 0) {
        if (overflowEnabled == 0) {
            printf("\n\n!!!! CLIPPING WARNING !!!! %d samples were CLAMPED\n", lOverflowCnt);
        }
        else {
            printf("\n\n!!!! CLIPPING WARNING !!!! OVERFLOW for %d samples\n", lOverflowCnt);
        }
    }

    printf("\n Done\n");
	fclose(In); fclose(Out);

	return 0;
}
