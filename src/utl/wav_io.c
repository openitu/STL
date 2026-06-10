/* wav_io.c - WAV file I/O support for STL tools
 *
 * Canonical WAV (RIFF) reader/writer for 8/16/24/32-bit PCM and 32-bit IEEE float.
 * See wav_io.h for API documentation.
 */
#include "wav_io.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* WAV format constants */
#define WAV_RIFF_TAG      0x46464952  /* "RIFF" */
#define WAV_WAVE_TAG      0x45564157  /* "WAVE" */
#define WAV_FMT_TAG       0x20746D66  /* "fmt " */
#define WAV_DATA_TAG      0x61746164  /* "data" */
#define WAV_PCM_FORMAT    1
#define WAV_FLOAT_FORMAT  3
#define WAV_HEADER_SIZE   44

/* --- Helper: read little-endian integers from file --- */

static int read_u16 (FILE * fp, unsigned short *val) {
  unsigned char b[2];
  if (fread (b, 1, 2, fp) != 2)
    return 0;
  *val = (unsigned short) (b[0] | (b[1] << 8));
  return 1;
}

static int read_u32 (FILE * fp, unsigned long *val) {
  unsigned char b[4];
  if (fread (b, 1, 4, fp) != 4)
    return 0;
  *val = (unsigned long) b[0] | ((unsigned long) b[1] << 8) | ((unsigned long) b[2] << 16) | ((unsigned long) b[3] << 24);
  return 1;
}

/* --- Helper: write little-endian integers to file --- */

static void write_u16 (FILE * fp, unsigned short val) {
  unsigned char b[2];
  b[0] = (unsigned char) (val & 0xFF);
  b[1] = (unsigned char) ((val >> 8) & 0xFF);
  fwrite (b, 1, 2, fp);
}

static void write_u32 (FILE * fp, unsigned long val) {
  unsigned char b[4];
  b[0] = (unsigned char) (val & 0xFF);
  b[1] = (unsigned char) ((val >> 8) & 0xFF);
  b[2] = (unsigned char) ((val >> 16) & 0xFF);
  b[3] = (unsigned char) ((val >> 24) & 0xFF);
  fwrite (b, 1, 4, fp);
}

/* --- Helper: case-insensitive extension check --- */

static int has_wav_extension (const char *filename) {
  const char *dot;
  dot = strrchr (filename, '.');
  if (!dot)
    return 0;
  if ((dot[1] == 'w' || dot[1] == 'W') && (dot[2] == 'a' || dot[2] == 'A') && (dot[3] == 'v' || dot[3] == 'V') && dot[4] == '\0')
    return 1;
  return 0;
}

/* --- Public API --- */

AUDIO_FILE *audio_open_read (const char *filename, long expected_rate, int expected_channels, int expected_bits) {
  AUDIO_FILE *af;
  unsigned long riff_tag, file_size, wave_tag;
  unsigned long chunk_id, chunk_size;
  unsigned short audio_format, num_channels, bits_per_sample;
  unsigned long sample_rate;
  int fmt_found = 0;

  af = (AUDIO_FILE *) calloc (1, sizeof (AUDIO_FILE));
  if (!af)
    return NULL;

  af->fp = fopen (filename, "rb");
  if (!af->fp) {
    fprintf (stderr, "ERROR: Cannot open input file '%s'\n", filename);
    free (af);
    return NULL;
  }

  af->write_mode = 0;

  /* Check for RIFF header */
  if (!read_u32 (af->fp, &riff_tag) || riff_tag != WAV_RIFF_TAG) {
    /* Not a WAV file — treat as raw PCM */
    fseek (af->fp, 0, SEEK_SET);
    af->is_wav = 0;
    af->channels = 1;
    af->sample_rate = 0;
    af->bits_per_sample = 16;
    af->data_offset = 0;
    return af;
  }

  /* Parse RIFF/WAVE header */
  if (!read_u32 (af->fp, &file_size) || !read_u32 (af->fp, &wave_tag) || wave_tag != WAV_WAVE_TAG) {
    fprintf (stderr, "ERROR: '%s' has RIFF header but is not a WAVE file\n", filename);
    fclose (af->fp);
    free (af);
    return NULL;
  }

  /* Find fmt and data chunks */
  while (read_u32 (af->fp, &chunk_id) && read_u32 (af->fp, &chunk_size)) {
    if (chunk_id == WAV_FMT_TAG) {
      if (!read_u16 (af->fp, &audio_format) || !read_u16 (af->fp, &num_channels) || !read_u32 (af->fp, &sample_rate)) {
        fprintf (stderr, "ERROR: Cannot read fmt chunk in '%s'\n", filename);
        fclose (af->fp);
        free (af);
        return NULL;
      }
      /* Skip byte rate and block align */
      fseek (af->fp, 4 + 2, SEEK_CUR);
      if (!read_u16 (af->fp, &bits_per_sample)) {
        fprintf (stderr, "ERROR: Cannot read bits_per_sample in '%s'\n", filename);
        fclose (af->fp);
        free (af);
        return NULL;
      }
      /* Skip any extra fmt bytes */
      if (chunk_size > 16)
        fseek (af->fp, chunk_size - 16, SEEK_CUR);
      fmt_found = 1;
    } else if (chunk_id == WAV_DATA_TAG) {
      if (!fmt_found) {
        fprintf (stderr, "ERROR: data chunk before fmt chunk in '%s'\n", filename);
        fclose (af->fp);
        free (af);
        return NULL;
      }
      af->data_offset = ftell (af->fp);
      af->data_size = (long) chunk_size;
      break;
    } else {
      /* Skip unknown chunk */
      fseek (af->fp, chunk_size, SEEK_CUR);
    }
  }

  if (!fmt_found || af->data_offset == 0) {
    fprintf (stderr, "ERROR: Missing fmt or data chunk in '%s'\n", filename);
    fclose (af->fp);
    free (af);
    return NULL;
  }

  /* Validate format */
  if (audio_format != WAV_PCM_FORMAT && audio_format != WAV_FLOAT_FORMAT) {
    fprintf (stderr, "ERROR: '%s' has unsupported WAV format (format tag=%d). Only PCM and IEEE float are supported.\n", filename, audio_format);
    fclose (af->fp);
    free (af);
    return NULL;
  }

  if (audio_format == WAV_FLOAT_FORMAT && bits_per_sample != 32) {
    fprintf (stderr, "ERROR: '%s' is float WAV with %d bits (only 32-bit float supported).\n", filename, bits_per_sample);
    fclose (af->fp);
    free (af);
    return NULL;
  }

  if (audio_format == WAV_PCM_FORMAT && bits_per_sample != 8 && bits_per_sample != 16 && bits_per_sample != 24 && bits_per_sample != 32) {
    fprintf (stderr, "ERROR: '%s' has unsupported bit depth (%d). Supported: 8, 16, 24, 32.\n", filename, bits_per_sample);
    fclose (af->fp);
    free (af);
    return NULL;
  }

  /* Check expected parameters */
  if (expected_bits > 0 && (int) bits_per_sample != expected_bits) {
    fprintf (stderr, "ERROR: WAV bit depth (%d) does not match expected (%d) in '%s'\n", bits_per_sample, expected_bits, filename);
    fclose (af->fp);
    free (af);
    return NULL;
  }

  if (expected_rate > 0 && (long) sample_rate != expected_rate) {
    fprintf (stderr, "ERROR: WAV sample rate (%lu Hz) does not match expected rate (%ld Hz) in '%s'\n", sample_rate, expected_rate, filename);
    fclose (af->fp);
    free (af);
    return NULL;
  }

  if (expected_channels > 0 && (int) num_channels != expected_channels) {
    fprintf (stderr, "ERROR: WAV channel count (%d) does not match expected channels (%d) in '%s'\n", num_channels, expected_channels, filename);
    fclose (af->fp);
    free (af);
    return NULL;
  }

  af->is_wav = 1;
  af->channels = (int) num_channels;
  af->sample_rate = (long) sample_rate;
  af->bits_per_sample = (int) bits_per_sample;
  af->audio_format = (int) audio_format;

  return af;
}


AUDIO_FILE *audio_open_write (const char *filename, long sample_rate, int channels, int bits_per_sample) {
  AUDIO_FILE *af;

  af = (AUDIO_FILE *) calloc (1, sizeof (AUDIO_FILE));
  if (!af)
    return NULL;

  af->fp = fopen (filename, "wb");
  if (!af->fp) {
    fprintf (stderr, "ERROR: Cannot open output file '%s'\n", filename);
    free (af);
    return NULL;
  }

  af->write_mode = 1;
  af->sample_rate = sample_rate;
  af->channels = channels;
  af->bits_per_sample = bits_per_sample > 0 ? bits_per_sample : 16;

  if (has_wav_extension (filename)) {
    af->is_wav = 1;
    /* Write placeholder WAV header (44 bytes) — updated on close */
    write_u32 (af->fp, WAV_RIFF_TAG);
    write_u32 (af->fp, 0);      /* file size - 8 (placeholder) */
    write_u32 (af->fp, WAV_WAVE_TAG);
    /* fmt chunk */
    write_u32 (af->fp, WAV_FMT_TAG);
    write_u32 (af->fp, 16);     /* fmt chunk size */
    write_u16 (af->fp, WAV_PCM_FORMAT);
    write_u16 (af->fp, (unsigned short) af->channels);
    write_u32 (af->fp, (unsigned long) af->sample_rate);
    write_u32 (af->fp, (unsigned long) (af->sample_rate * af->channels * af->bits_per_sample / 8));     /* byte rate */
    write_u16 (af->fp, (unsigned short) (af->channels * af->bits_per_sample / 8));      /* block align */
    write_u16 (af->fp, (unsigned short) af->bits_per_sample);
    /* data chunk header */
    write_u32 (af->fp, WAV_DATA_TAG);
    write_u32 (af->fp, 0);      /* data size (placeholder) */
    af->data_offset = ftell (af->fp);
    af->data_size = 0;
  } else {
    af->is_wav = 0;
    af->data_size = 0;
  }

  return af;
}


long audio_read (AUDIO_FILE * af, void *buffer, long nsamples) {
  int bps, ch, sample_bytes, frame_bytes;
  long frames_read, i;

  if (!af || !af->fp || !buffer || nsamples <= 0)
    return 0;

  bps = af->bits_per_sample;
  ch = af->is_wav ? af->channels : 1;
  sample_bytes = (bps == 24) ? 3 : (bps / 8);
  frame_bytes = sample_bytes * ch;

  /* Raw files are native-endian; direct fread is safe for 8/16-bit mono */
  if (!af->is_wav && ch <= 1 && bps <= 2) {
    return (long) fread (buffer, (size_t) sample_bytes, (size_t) nsamples, af->fp);
  }

  /* WAV (little-endian) and multi-channel: extract with byte-order handling */
  {
    unsigned char *raw_buf;

    if (ch > 1 && !af->warned_multichan) {
      fprintf (stderr, "WARNING: Multi-channel WAV (%d channels). Extracting channel 1 only.\n", ch);
      af->warned_multichan = 1;
    }

    raw_buf = (unsigned char *) malloc ((size_t) (nsamples * frame_bytes));
    if (!raw_buf)
      return 0;

    frames_read = (long) fread (raw_buf, (size_t) frame_bytes, (size_t) nsamples, af->fp);

    if (bps == 8) {
      unsigned char *out = (unsigned char *) buffer;
      for (i = 0; i < frames_read; i++)
        out[i] = raw_buf[i * frame_bytes];
    } else if (bps == 16) {
      short *out = (short *) buffer;
      for (i = 0; i < frames_read; i++) {
        unsigned char *p = raw_buf + i * frame_bytes;
        out[i] = (short) (p[0] | (p[1] << 8));
      }
    } else if (bps == 24) {
      long *out = (long *) buffer;
      for (i = 0; i < frames_read; i++) {
        unsigned char *p = raw_buf + i * frame_bytes;
        out[i] = (long) p[0] | ((long) p[1] << 8) | ((long) (signed char) p[2] << 16);
      }
    } else if (bps == 32) {
      long *out = (long *) buffer;
      if (af->audio_format == WAV_FLOAT_FORMAT) {
        float *fout = (float *) buffer;
        for (i = 0; i < frames_read; i++) {
          unsigned char *p = raw_buf + i * frame_bytes;
          union { unsigned char b[4]; float f; } u;
          u.b[0] = p[0]; u.b[1] = p[1]; u.b[2] = p[2]; u.b[3] = p[3];
          fout[i] = u.f;
        }
      } else {
        for (i = 0; i < frames_read; i++) {
          unsigned char *p = raw_buf + i * frame_bytes;
          out[i] = (long) p[0] | ((long) p[1] << 8) | ((long) p[2] << 16) | ((long) (signed char) p[3] << 24);
        }
      }
    }

    free (raw_buf);
    return frames_read;
  }
}


long audio_write (AUDIO_FILE * af, void *buffer, long nsamples) {
  int bps, sample_bytes;
  long n, i;

  if (!af || !af->fp || !buffer || nsamples <= 0)
    return 0;

  bps = af->bits_per_sample;
  sample_bytes = (bps == 24) ? 3 : (bps / 8);

  if (bps != 24) {
    /* For raw or 8-bit: direct fwrite (native endian or single byte) */
    if (!af->is_wav || bps == 8) {
      n = (long) fwrite (buffer, (size_t) sample_bytes, (size_t) nsamples, af->fp);
      af->data_size += n * sample_bytes;
      return n;
    }
    /* WAV 16-bit: write little-endian */
    if (bps == 16) {
      short *in = (short *) buffer;
      for (i = 0; i < nsamples; i++) {
        unsigned char b[2];
        b[0] = (unsigned char) (in[i] & 0xFF);
        b[1] = (unsigned char) ((in[i] >> 8) & 0xFF);
        if (fwrite (b, 1, 2, af->fp) != 2) break;
      }
      af->data_size += i * 2;
      return i;
    }
    /* WAV 32-bit: write little-endian */
    {
      int *in = (int *) buffer;
      for (i = 0; i < nsamples; i++) {
        unsigned char b[4];
        unsigned int val = (unsigned int) in[i];
        b[0] = (unsigned char) (val & 0xFF);
        b[1] = (unsigned char) ((val >> 8) & 0xFF);
        b[2] = (unsigned char) ((val >> 16) & 0xFF);
        b[3] = (unsigned char) ((val >> 24) & 0xFF);
        if (fwrite (b, 1, 4, af->fp) != 4) break;
      }
      af->data_size += i * 4;
      return i;
    }
  }

  /* 24-bit: pack from long */
  {
    long *in = (long *) buffer;
    for (i = 0; i < nsamples; i++) {
      unsigned char b[3];
      long val = in[i];
      b[0] = (unsigned char) (val & 0xFF);
      b[1] = (unsigned char) ((val >> 8) & 0xFF);
      b[2] = (unsigned char) ((val >> 16) & 0xFF);
      if (fwrite (b, 1, 3, af->fp) != 3)
        break;
    }
    af->data_size += i * 3;
    return i;
  }
}


void audio_close (AUDIO_FILE * af) {
  if (!af)
    return;

  if (af->fp) {
    if (af->write_mode && af->is_wav) {
      /* Update WAV header with final sizes */
      unsigned long file_size = (unsigned long) af->data_size + WAV_HEADER_SIZE - 8;
      fseek (af->fp, 4, SEEK_SET);
      write_u32 (af->fp, file_size);
      fseek (af->fp, WAV_HEADER_SIZE - 4, SEEK_SET);
      write_u32 (af->fp, (unsigned long) af->data_size);
    }
    fclose (af->fp);
  }

  free (af);
}


int audio_is_wav (AUDIO_FILE * af) {
  return af ? af->is_wav : 0;
}


long audio_get_sample_rate (AUDIO_FILE * af) {
  return af ? af->sample_rate : 0;
}


int audio_get_channels (AUDIO_FILE * af) {
  return af ? af->channels : 0;
}


int audio_seek (AUDIO_FILE * af, long offset) {
  if (!af || !af->fp)
    return -1;
  return fseek (af->fp, af->data_offset + offset, SEEK_SET) < 0 ? -1 : 0;
}


long audio_get_data_size (AUDIO_FILE * af) {
  if (!af || !af->fp)
    return 0;
  if (af->is_wav)
    return af->data_size;
  else {
    long cur = ftell (af->fp);
    fseek (af->fp, 0, SEEK_END);
    long size = ftell (af->fp);
    fseek (af->fp, cur, SEEK_SET);
    return size;
  }
}
