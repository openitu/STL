/* test_wav_io_validate.c - Validate WAV file header parameters.
 *
 * Usage: test_wav_io_validate <file.wav> <expected_rate> <expected_channels> <expected_bits>
 * Returns 0 if WAV file matches all expected parameters, 1 otherwise.
 */
#include <stdio.h>
#include <stdlib.h>
#include "wav_io.h"

int main (int argc, char *argv[]) {
  AUDIO_FILE *af;
  long expected_rate;
  int expected_channels, expected_bits;

  if (argc != 5) {
    fprintf (stderr, "Usage: test_wav_io_validate <file.wav> <rate> <channels> <bits>\n");
    return 1;
  }

  expected_rate = atol (argv[2]);
  expected_channels = atoi (argv[3]);
  expected_bits = atoi (argv[4]);

  af = audio_open_read (argv[1], expected_rate, expected_channels, expected_bits);
  if (!af) {
    fprintf (stderr, "FAIL: Cannot open or parameters mismatch: %s\n", argv[1]);
    return 1;
  }

  if (!audio_is_wav (af)) {
    fprintf (stderr, "FAIL: %s is not a WAV file\n", argv[1]);
    audio_close (af);
    return 1;
  }

  printf ("PASS: %s (rate=%ld, ch=%d, bits=%d)\n", argv[1], audio_get_sample_rate (af), audio_get_channels (af), af->bits_per_sample);
  audio_close (af);
  return 0;
}
