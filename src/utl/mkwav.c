/* mkwav.c - Create a WAV file from raw PCM data or silence.
 *
 * Usage: mkwav <output.wav> <sample_rate> <channels> <bits> [input.pcm]
 *        If input.pcm is omitted, generates 100 samples of silence.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wav_io.h"

int main (int argc, char *argv[]) {
  AUDIO_FILE *af;
  long rate;
  int channels, bits;
  short buf[256];
  long n;

  if (argc < 5) {
    fprintf (stderr, "Usage: mkwav <output.wav> <sample_rate> <channels> <bits> [input.pcm]\n");
    return 1;
  }

  rate = atol (argv[2]);
  channels = atoi (argv[3]);
  bits = atoi (argv[4]);

  af = audio_open_write (argv[1], rate, channels, bits);
  if (!af)
    return 1;

  if (argc > 5) {
    FILE *fp = fopen (argv[5], "rb");
    if (!fp) {
      fprintf (stderr, "Cannot open input: %s\n", argv[5]);
      audio_close (af);
      return 1;
    }
    while ((n = (long) fread (buf, sizeof (short), 256, fp)) > 0)
      audio_write (af, buf, n);
    fclose (fp);
  } else {
    memset (buf, 0, sizeof (buf));
    audio_write (af, buf, 100);
  }

  audio_close (af);
  return 0;
}
