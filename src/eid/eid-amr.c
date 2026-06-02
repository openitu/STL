/*
  eid-amr.c

  EID for AMR using G.192 error pattern of frame erasures.
  This tool is based on the eid-3g tool written by Nobuhiko Naka, NTT DOCOMO
  (see: http://www.3gpp.org/ftp/tsg_sa/WG4_CODEC/AMR-NB_3G-Characterization/EID_Error_Patterns/)

  Modifications:
  - using G.192 error pattern for frame erasures
  - only frame erasures (no bit errors)
  - no position parameter to skip frames (usage similar to ITU-T STL eid-xor tool)
  - all data bits in a lost frame are forced to a value of 0
  - statistics reported to stderr at the end of processing
  - uses softbit.c read_g192() for error pattern reading

  NOTES:
  A lost frame in DTX on/off is signalled with a frame type code 0x03,
  independently from the TX or RX frame type.

  Contact:
  Balazs Kovesi, Stephane Ragot
  ORANGE SA
  firstname.lastname@orange.com

  ===============================================================
  COPYRIGHT NOTE: This source code, and all of its derivations,
  is subject to the "ITU-T General Public License". Please have
  it read in the distribution disk, or in the ITU-T Recommendation
  G.191 on "SOFTWARE TOOLS FOR SPEECH AND AUDIO CODING STANDARDS".
  See LICENSE.md in the top-level directory for terms.
  ===============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ugstdemo.h"
#include "softbit.h"

#define MAX_SERIAL 244
#define SERIAL_FRAMESIZE (1 + MAX_SERIAL + 5)

int main(int argc, char *argv[]) {
  short spch[SERIAL_FRAMESIZE];
  short epat;

  FILE *fsp, *fep, *fwe;

  long frame, fercnt, wrapcnt;
  int i;

  if (argc != 4) {
    fprintf(stderr, "eid-amr: EID for 3GPP AMR codec using G.192 frame erasure patterns\n\n");
    fprintf(stderr, "Usage: eid-amr <AMR bitstream (i)> <G.192 FER pattern (i)> <AMR bitstream with errors (o)>\n");
    exit(1);
  }

  if ((fsp = fopen(argv[1], RB)) == NULL) { perror(argv[1]); exit(1); }
  if ((fep = fopen(argv[2], RB)) == NULL) { perror(argv[2]); exit(1); }
  if ((fwe = fopen(argv[3], WB)) == NULL) { perror(argv[3]); exit(1); }

  frame = 0;
  fercnt = 0;
  wrapcnt = 0;

  while (fread(spch, sizeof(short), SERIAL_FRAMESIZE, fsp) == SERIAL_FRAMESIZE) {
    /* Read one erasure flag from G.192 error pattern */
    if (read_g192(&epat, 1, fep) != 1) {
      fseek(fep, 0L, SEEK_SET);
      fprintf(stderr, "Warning: Error pattern file shorter than speech file\n--> wrap error pattern file\n");
      wrapcnt++;
      if (read_g192(&epat, 1, fep) != 1) {
        fprintf(stderr, "Error: Error pattern file read failure\n");
        exit(1);
      }
    }

    if (epat == G192_FER) {
      spch[0] = 0x03; /* NO_DATA frame type */
      fercnt++;
      for (i = 0; i < MAX_SERIAL; i++) {
        spch[i + 1] = 0; /* erase bits in bad frame (force value of 0) */
      }
    } else if (epat != G192_SYNC) {
      fprintf(stderr, "Error: invalid pattern value 0x%04X at frame %ld\n", (unsigned short)epat, frame);
      exit(1);
    }

    fwrite(spch, sizeof(short), SERIAL_FRAMESIZE, fwe);
    frame++;
  }

  fclose(fsp);
  fclose(fep);
  fclose(fwe);

  fprintf(stderr, "_Input bit stream file ..................: %s\n", argv[1]);
  fprintf(stderr, "_Error pattern file .....................: %s\n", argv[2]);
  fprintf(stderr, "_Output bit stream file .................: %s\n", argv[3]);
  fprintf(stderr, "# Error pattern files wrapped ...........: %ld times\n", wrapcnt);
  fprintf(stderr, "# Processed frames ..................... : %ld\n", frame);
  fprintf(stderr, "# Distorted frames ..................... : %ld\n", fercnt);
  if (frame > 0)
    fprintf(stderr, "# Frame erasure rate ....................: %f %%\n", (100.0 * fercnt) / frame);

  return 0;
}
