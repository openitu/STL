/* test_wav_io.c - Test program for wav_io library
 *
 * Creates test WAV files, reads them back, and verifies correctness.
 * Returns 0 on success, 1 on failure.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wav_io.h"

#define NSAMPLES 100
#define TEST_RATE 8000

static int test_write_and_read_wav (void) {
  AUDIO_FILE *af;
  short buf_out[NSAMPLES], buf_in[NSAMPLES];
  long n;
  int i;

  /* Generate test samples */
  for (i = 0; i < NSAMPLES; i++)
    buf_out[i] = (short) (i * 100 - 5000);

  /* Write WAV */
  af = audio_open_write ("test_data/test_out.wav", TEST_RATE, 1, 16);
  if (!af) {
    fprintf (stderr, "FAIL: Cannot open test_out.wav for writing\n");
    return 1;
  }
  if (!audio_is_wav (af)) {
    fprintf (stderr, "FAIL: test_out.wav not detected as WAV for writing\n");
    audio_close (af);
    return 1;
  }
  n = audio_write (af, buf_out, NSAMPLES);
  if (n != NSAMPLES) {
    fprintf (stderr, "FAIL: Wrote %ld samples, expected %d\n", n, NSAMPLES);
    audio_close (af);
    return 1;
  }
  audio_close (af);

  /* Read back */
  af = audio_open_read ("test_data/test_out.wav", TEST_RATE, 1, 16);
  if (!af) {
    fprintf (stderr, "FAIL: Cannot open test_out.wav for reading\n");
    return 1;
  }
  if (!audio_is_wav (af)) {
    fprintf (stderr, "FAIL: test_out.wav not detected as WAV\n");
    audio_close (af);
    return 1;
  }
  if (audio_get_sample_rate (af) != TEST_RATE) {
    fprintf (stderr, "FAIL: Sample rate %ld, expected %d\n", audio_get_sample_rate (af), TEST_RATE);
    audio_close (af);
    return 1;
  }
  if (audio_get_channels (af) != 1) {
    fprintf (stderr, "FAIL: Channels %d, expected 1\n", audio_get_channels (af));
    audio_close (af);
    return 1;
  }
  n = audio_read (af, buf_in, NSAMPLES);
  if (n != NSAMPLES) {
    fprintf (stderr, "FAIL: Read %ld samples, expected %d\n", n, NSAMPLES);
    audio_close (af);
    return 1;
  }
  audio_close (af);

  /* Verify samples */
  if (memcmp (buf_out, buf_in, NSAMPLES * sizeof (short)) != 0) {
    fprintf (stderr, "FAIL: Sample data mismatch\n");
    return 1;
  }

  printf ("PASS: write_and_read_wav\n");
  return 0;
}

static int test_raw_file (void) {
  AUDIO_FILE *af;
  short buf_out[NSAMPLES], buf_in[NSAMPLES];
  long n;
  int i;

  /* Write raw file directly */
  {
    FILE *fp = fopen ("test_data/test_raw.pcm", "wb");
    if (!fp) {
      fprintf (stderr, "FAIL: Cannot create test_raw.pcm\n");
      return 1;
    }
    for (i = 0; i < NSAMPLES; i++)
      buf_out[i] = (short) (i * 50);
    fwrite (buf_out, sizeof (short), NSAMPLES, fp);
    fclose (fp);
  }

  /* Read via audio_open_read */
  af = audio_open_read ("test_data/test_raw.pcm", 0, 0, 16);
  if (!af) {
    fprintf (stderr, "FAIL: Cannot open test_raw.pcm\n");
    return 1;
  }
  if (audio_is_wav (af)) {
    fprintf (stderr, "FAIL: Raw file detected as WAV\n");
    audio_close (af);
    return 1;
  }
  n = audio_read (af, buf_in, NSAMPLES);
  if (n != NSAMPLES) {
    fprintf (stderr, "FAIL: Read %ld samples from raw, expected %d\n", n, NSAMPLES);
    audio_close (af);
    return 1;
  }
  audio_close (af);

  if (memcmp (buf_out, buf_in, NSAMPLES * sizeof (short)) != 0) {
    fprintf (stderr, "FAIL: Raw sample data mismatch\n");
    return 1;
  }

  printf ("PASS: raw_file\n");
  return 0;
}

static int test_raw_to_wav_output (void) {
  AUDIO_FILE *af;
  short buf[NSAMPLES];
  int i;

  for (i = 0; i < NSAMPLES; i++)
    buf[i] = (short) (i * 10);

  /* Write to .wav extension — should produce WAV */
  af = audio_open_write ("test_data/test_ext.wav", 16000, 1, 16);
  if (!af || !audio_is_wav (af)) {
    fprintf (stderr, "FAIL: .wav extension not producing WAV output\n");
    if (af)
      audio_close (af);
    return 1;
  }
  audio_write (af, buf, NSAMPLES);
  audio_close (af);

  /* Write to .pcm extension — should produce raw */
  af = audio_open_write ("test_data/test_ext.pcm", 16000, 1, 16);
  if (!af || audio_is_wav (af)) {
    fprintf (stderr, "FAIL: .pcm extension producing WAV output\n");
    if (af)
      audio_close (af);
    return 1;
  }
  audio_write (af, buf, NSAMPLES);
  audio_close (af);

  printf ("PASS: raw_to_wav_output\n");
  return 0;
}

static int test_rate_mismatch (void) {
  AUDIO_FILE *af;
  short buf[NSAMPLES];
  int i;

  /* Create a valid 8000 Hz WAV */
  for (i = 0; i < NSAMPLES; i++)
    buf[i] = (short) i;
  af = audio_open_write ("test_data/test_8k.wav", 8000, 1, 16);
  if (!af)
    return 1;
  audio_write (af, buf, NSAMPLES);
  audio_close (af);

  /* Try to open expecting 16000 Hz — should fail */
  af = audio_open_read ("test_data/test_8k.wav", 16000, 0, 16);
  if (af != NULL) {
    fprintf (stderr, "FAIL: Rate mismatch not detected\n");
    audio_close (af);
    return 1;
  }

  printf ("PASS: rate_mismatch\n");
  return 0;
}

static int test_stereo_extraction (void) {
  AUDIO_FILE *af;
  short stereo_buf[NSAMPLES * 2], mono_buf[NSAMPLES];
  long n;
  int i;

  /* Create stereo WAV: ch0 = i, ch1 = -i */
  for (i = 0; i < NSAMPLES; i++) {
    stereo_buf[i * 2] = (short) i;
    stereo_buf[i * 2 + 1] = (short) (-i);
  }
  af = audio_open_write ("test_data/test_stereo.wav", 8000, 2, 16);
  if (!af)
    return 1;
  /* Write raw interleaved data directly for stereo */
  fwrite (stereo_buf, sizeof (short), NSAMPLES * 2, af->fp);
  af->data_size += NSAMPLES * 2 * sizeof (short);
  audio_close (af);

  /* Read back — should extract channel 0 */
  af = audio_open_read ("test_data/test_stereo.wav", 0, 0, 16);
  if (!af) {
    fprintf (stderr, "FAIL: Cannot open stereo WAV\n");
    return 1;
  }
  if (audio_get_channels (af) != 2) {
    fprintf (stderr, "FAIL: Expected 2 channels, got %d\n", audio_get_channels (af));
    audio_close (af);
    return 1;
  }
  n = audio_read (af, mono_buf, NSAMPLES);
  if (n != NSAMPLES) {
    fprintf (stderr, "FAIL: Read %ld samples, expected %d\n", n, NSAMPLES);
    audio_close (af);
    return 1;
  }
  audio_close (af);

  /* Verify channel 0 extracted */
  for (i = 0; i < NSAMPLES; i++) {
    if (mono_buf[i] != (short) i) {
      fprintf (stderr, "FAIL: Stereo extraction mismatch at sample %d: got %d, expected %d\n", i, mono_buf[i], i);
      return 1;
    }
  }

  printf ("PASS: stereo_extraction\n");
  return 0;
}

int main (void) {
  int failures = 0;

  failures += test_write_and_read_wav ();
  failures += test_raw_file ();
  failures += test_raw_to_wav_output ();
  failures += test_rate_mismatch ();
  failures += test_stereo_extraction ();

  if (failures == 0)
    printf ("\nAll wav_io tests PASSED\n");
  else
    printf ("\n%d wav_io test(s) FAILED\n", failures);

  /* Clean up test files */
  remove ("test_data/test_out.wav");
  remove ("test_data/test_raw.pcm");
  remove ("test_data/test_ext.wav");
  remove ("test_data/test_ext.pcm");
  remove ("test_data/test_8k.wav");
  remove ("test_data/test_stereo.wav");

  return failures ? 1 : 0;
}
