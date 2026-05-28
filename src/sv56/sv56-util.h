/*
  ============================================================================
   File: SV56-UTIL.H                                          28.May.2026 v2.0
  ============================================================================

                      UGST/ITU-T SPEECH VOLTMETER MODULE

                        LOCAL UTILITY FUNCTIONS

       ===============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
       CODING STANDARDS".
       ===============================================================

   History:
   28.May.26  v1.0    Created: safe short-to-float conversion for sv56
                       module that always assumes 16-bit PCM input.
   28.May.26  v2.0    Rewritten: variable bit-width I/O (8, 16, 24, 32
                       bit) with native byte order. Legacy widths
                       (9-15 bit) supported via bit masking.

  ============================================================================
*/
#ifndef SV56_UTIL_defined
#define SV56_UTIL_defined 200

#include <stdint.h>

static int sv56_is_little_endian (void)
{
  unsigned int x = 1;
  return *(unsigned char *) &x;
}

static int sv56_bytes_per_sample (int bitno)
{
  if (bitno <= 8)
    return 1;
  if (bitno <= 16)
    return 2;
  if (bitno <= 24)
    return 3;
  return 4;
}

static void sv56_raw2fl (long n, const unsigned char *raw, float *y, int bitno)
{
  long k;
  int bps = sv56_bytes_per_sample (bitno);
  int le = sv56_is_little_endian ();
  double norm = (double) (1 << (bitno - 1));

  for (k = 0; k < n; k++)
    {
      const unsigned char *p = raw + k * bps;
      uint32_t uval = 0;
      int32_t sval;
      int j;

      for (j = 0; j < bps; j++)
        uval |= (uint32_t) p[le ? j : (bps - 1 - j)] << (j * 8);

      sval = (int32_t) uval;
      if (bitno < 32 && (uval & (1U << (bitno - 1))))
        sval -= (int32_t) (1U << bitno);

      y[k] = (float) ((double) sval / norm);
    }
}

static long sv56_fl2raw (long n, const float *x, unsigned char *raw, int bitno)
{
  long k;
  int bps = sv56_bytes_per_sample (bitno);
  int le = sv56_is_little_endian ();
  int pad_bits = bps * 8 - bitno;
  double scale = (double) (1U << (bitno - 1));
  long overflows = 0;

  for (k = 0; k < n; k++)
    {
      double val = (double) x[k] * scale;
      int32_t ival;
      uint32_t uval;
      int j;

      if (val > scale - 1.0)
        {
          val = scale - 1.0;
          overflows++;
        }
      else if (val < -scale)
        {
          val = -scale;
          overflows++;
        }

      ival = (int32_t) val;
      if (pad_bits > 0)
        ival &= ~((1 << pad_bits) - 1);
      uval = (uint32_t) ival;

      for (j = 0; j < bps; j++)
        raw[k * bps + (le ? j : (bps - 1 - j))] =
          (unsigned char) ((uval >> (j * 8)) & 0xFF);
    }

  return overflows;
}

#endif /* SV56_UTIL_defined */
/* ........................ End of SV56-UTIL.H .......................... */
