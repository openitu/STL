#include <stdio.h>
#include <stdlib.h>
#include "sv56-util.h"

#define BLK_LEN 256

int main (int argc, char *argv[])
{
  FILE *Fi, *Fo;
  unsigned char in_buf[BLK_LEN * 2];
  unsigned char out_buf[BLK_LEN * 4];
  float Buf[BLK_LEN];
  long l;
  int out_bitno, out_bps;

  if (argc < 4) {
    fprintf (stderr, "Usage: %s <out_bitno> <in_16bit> <out_file>\n", argv[0]);
    return 1;
  }

  out_bitno = atoi (argv[1]);
  out_bps = sv56_bytes_per_sample (out_bitno);

  Fi = fopen (argv[2], "rb");
  if (!Fi) { fprintf (stderr, "Cannot open %s\n", argv[2]); return 1; }
  Fo = fopen (argv[3], "wb");
  if (!Fo) { fprintf (stderr, "Cannot open %s\n", argv[3]); fclose (Fi); return 1; }

  while ((l = fread (in_buf, 2, BLK_LEN, Fi)) > 0) {
    sv56_raw2fl (l, in_buf, Buf, 16);
    sv56_fl2raw (l, Buf, out_buf, out_bitno);
    fwrite (out_buf, out_bps, l, Fo);
  }

  fclose (Fi);
  fclose (Fo);
  return 0;
}
