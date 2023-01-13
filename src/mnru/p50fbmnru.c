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

void show_use(void)
{
     printf("P.50 Fullband MNRU - %s\n", VERSION_STL);
     printf("\n  P.50 Fullband MNRU shapes the gaussian noise with an average speech power spectrum as ITU-T Rec. P.50.");

     printf("\n  Requires 48kHz sampling rate.\n");

	 printf("\n  Usage: p50fbmnru <inputfile> <outputfile> <Q/dB> <Mode> [dcFilterOn]\n");

	 printf("\n      Mode M:   Modulated Noise");
	 printf("\n           N:   Noise only");
	 printf("\n           S:   Signal only\n");
	 printf("\n      Options:");
     printf("\n         dcFilterOn  1 for enable DC Filter (for compatibility with P.50 MNRU prior 2023)");
     printf("\n                     0 to disable DC Filter (default, and recommended)\n\n");
	 exit(1);
}

int main(int argc, char *argv[])
{
    MNRU_state      state;
    FILE            *In, *Out;
    char            applyDcRemoval ;
    short           B_Len, BuffLen;
    long            lFileLen = 0;
    int             i, B_Max;
    float           Q;
    short           operation, MNRU_mode;
	static short 	Buf[512];
	static double	In_Buf[512], Out_Buf[512];

	//Do inits to prevent crashing when option 'S' is selected
	state.rnd_state.gauss = NULL;
	state.vet = NULL;

    if ( (argc > 6) || (argc < 5)) {
        show_use();
    }
    else if (argc  == 6) {
        applyDcRemoval = (char) atoi(argv[5]);
    }
    else {
        applyDcRemoval = 0;
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

    if (applyDcRemoval == 0) {
        printf("DC-removal filter disabled\n");
    }
    else {
        printf("DC-removal filter enabled\n");
    }

	 /* +++++++++++++++++++++++++  initialize  +++++++++++++++++++++++++ */
	 printf(" Input file ............ %s ", argv[1]);
	 printf("\n Output file ........... %s ", argv[2]);
	 printf("\n Q ..................... %g dB", Q);
	 if( MNRU_mode == MOD_NOISE) printf("\n Mode .................. Mod-Noise");
	 if( MNRU_mode == NOISE_ONLY) printf("\n Mode .................. Noise only");
	 if( MNRU_mode == SIGNAL_ONLY) printf("\n Mode .................. Signal only");
	 printf("\n blocks ................ %5d", B_Max);

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

		P50_MNRU_process( operation, &state, In_Buf, Out_Buf, (long) BuffLen, (long) 314159265, (char) MNRU_mode, Q,
		    applyDcRemoval);

		for( i=0; i<BuffLen; i++)
		{
			if( Out_Buf[i]>0)		Buf[i]	=	(short) ( Out_Buf[i] +0.5);
			else					Buf[i]	=	(short) ( Out_Buf[i] -0.5);
		}

		fwrite(Buf, sizeof(short), BuffLen, Out);

		if( operation==MNRU_START)	operation	=	MNRU_CONTINUE;

		printf("\b\b\b\b\b%5d", B_Max--);

	} while( B_Max >= 0);

	operation	=	MNRU_STOP;

	P50_MNRU_process( operation, &state, In_Buf, Out_Buf, (long) 0, (long) 0, (char)0, 0, 0);

    printf("\n Done\n");
	fclose(In); fclose(Out);

	return 0;
}
