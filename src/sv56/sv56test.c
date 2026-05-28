/*                                                              v2.0 28.May.26
  ============================================================================

  SV56TEST.C
  ~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Minimal test program for the ITU-T P.56 speech voltmeter.
  Reads a PCM input file with configurable bit width (8, 16, 24, 32),
  measures the active speech level, and prints a one-line result.

  This program is intended for automated testing with different
  bitno values against input data of the corresponding format.

  Usage:
  ~~~~~~
  $ sv56test <bitno> <input_file> <output_file> [sample_rate]
  where:
  bitno        is the bit depth (8, 16, 24, or 32);
  input_file   is the input file (2's complement, native byte order);
  output_file  is the output text file for the measurement result;
  sample_rate  is the sampling rate in Hz (default: 16000).

  Output:
  ~~~~~~~
  A single line to the output file (and stdout):
    ActLev[dB]: <value>  RMSLev[dB]: <value>  Activity[%]: <value>

  Original author
  ~~~~~~~~~~~~~~~
  Jan Reimes

  Log of changes
  ~~~~~~~~~~~~~~
  28.May.26  v1.0  Creation.
  28.May.26  v2.0  Variable bit-width I/O (8, 16, 24, 32 bit).

  ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sv-p56.h"
#include "sv56-util.h"

#define BLK_LEN 256

int main (int argc, char *argv[])
{
  SVP56_state state;
  FILE *Fi, *Fo;
  unsigned char raw_buf[BLK_LEN * 4];
  float Buf[BLK_LEN];
  long l;
  int bitno, bps;
  double sf = 16000.0;
  double ActiveLeveldB;

  if (argc < 4) {
    fprintf (stderr, "Usage: %s <bitno> <input_file> <output_file> [sample_rate]\n", argv[0]);
    return 1;
  }

  bitno = atoi (argv[1]);
  if (bitno < 8 || bitno > SVP56_MAX_NO_BITS) {
    fprintf (stderr, "Error: bitno must be between 8 and %d\n", SVP56_MAX_NO_BITS);
    return 1;
  }

  bps = sv56_bytes_per_sample (bitno);

  if (argc >= 5)
    sf = atof (argv[4]);

  Fi = fopen (argv[2], "rb");
  if (Fi == NULL) {
    fprintf (stderr, "Error: cannot open %s\n", argv[2]);
    return 1;
  }

  Fo = fopen (argv[3], "w");
  if (Fo == NULL) {
    fprintf (stderr, "Error: cannot open %s for writing\n", argv[3]);
    fclose (Fi);
    return 1;
  }

  init_speech_voltmeter (&state, sf, bitno);

  while ((l = fread (raw_buf, bps, BLK_LEN, Fi)) > 0) {
    sv56_raw2fl (l, raw_buf, Buf, bitno);
    ActiveLeveldB = speech_voltmeter (Buf, l, &state);
  }

  fclose (Fi);

  fprintf (Fo, "ActLev[dB]: %10.3f  RMSLev[dB]: %10.3f  Activity[%%]: %8.3f\n",
           ActiveLeveldB, SVP56_get_rms_dB (state),
           SVP56_get_activity (state));
  fclose (Fo);

  printf ("ActLev[dB]: %10.3f  RMSLev[dB]: %10.3f  Activity[%%]: %8.3f\n",
          ActiveLeveldB, SVP56_get_rms_dB (state),
          SVP56_get_activity (state));

  return 0;
}

/* ....................... End of SV56TEST.C ....................... */
