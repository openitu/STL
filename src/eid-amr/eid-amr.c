/*
	eid-amr.c

	EID for AMR using G.192 error pattern of frame erasures
	This tool is based on the eid-3g tool written by Nobuhiko Naka, NTT DOCOMO (see: http://www.3gpp.org/ftp/tsg_sa/WG4_CODEC/AMR-NB_3G-Characterization/EID_Error_Patterns/)
	Modifications:  - using G.192 error pattern for frame erasures
                    - only frame erasures (no bit errors)
                    - no position parameter to skip frames (usage similar to ITU-T STL eid-xor tool)
                    - all data bits in a lost frame are forced to a value of 0 (behavior similar to ITU-T STL eid-xor tool)
					- statistics reported to stderr at the end of processing

	NOTES:
	a lost frame in DTX on/off is signalled with a frame type code 0x03, independently from the TX or RX frame type

	Contact:
		Balazs Kovesi, Stephane Ragot
		ORANGE SA
		firstname.lastname@orange.com
*/


#include <stdio.h>
#include <stdlib.h>

#define MAX_SERIAL 244
#define SERIAL_FRAMESIZE (1+MAX_SERIAL+5)

int main(int argc, char *argv[])
{

  short spch[SERIAL_FRAMESIZE];
  short epat;
  
  FILE *fsp, *fep, *fwe;

  int frame, fercnt, wrapcnt, i;

  if(argc!=4) {
    fprintf(stderr, "Usage: eid-amr [AMR bitstream (i)] [G.192 FER errpat (i)] [AMR bitstream speech with errors (o)] \n");
    exit(1);
  }

  if((fsp=fopen(argv[1], "rb"))==NULL) { perror(argv[1]); exit(1);}
  if((fep=fopen(argv[2], "rb"))==NULL) { perror(argv[2]); exit(1);}
  if((fwe=fopen(argv[3], "wb"))==NULL) { perror(argv[3]); exit(1);}
  
  frame = 0;
  fercnt = 0;
  wrapcnt = 0;

  while(fread(spch, sizeof(short), SERIAL_FRAMESIZE, fsp)==SERIAL_FRAMESIZE){
    if(fread(&epat, sizeof(short), 1, fep)!=1){
			fseek(fep, 0L, SEEK_SET);
			fprintf(stderr, "Warning: Error pattern file shorter than speech file \n--> wrap error pattern file\n");
			wrapcnt++;
			if(fread(&epat, sizeof(short), 1, fep)!=1){
				fprintf(stderr, "Error: Error pattern file read failure\n");
				exit(1);
			}
    }

	if(epat == 0x6b20) 
	{
		spch[0] = 0x03;
		fercnt++;
		for (i=0; i<MAX_SERIAL; i++)
		{
			spch[i+1] = 0; /* erase bits in bad frame (force value of 0) */
		}
	}
	else if (epat != 0x6b21)
	{ 
		perror("invalid pattern file"); 
		exit(1);
	}

    fwrite(spch, sizeof(short), SERIAL_FRAMESIZE, fwe);
    frame++;
  }

  fclose(fsp);
  fclose(fep);
  fclose(fwe);

  fprintf (stderr, "_Input bit stream file ..................: %s\n", argv[1]);
  fprintf (stderr, "_Error pattern file .....................: %s\n", argv[2]);
  fprintf (stderr, "_Output bit stream file .................: %s\n", argv[3]);

  fprintf (stderr, "# Error pattern files wrapped ...........: %ld times\n", wrapcnt);
  fprintf (stderr, "# Processed frames ..................... : %d \n", frame);
  fprintf (stderr, "# Distorted frames ..................... : %d \n", fercnt);
  fprintf (stderr, "# Frame erasure rate ....................: %f %%\n", (100.0 * fercnt) / frame);

  return(0);
}
