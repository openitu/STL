/* wav_io.h - WAV file I/O support for STL tools
 *
 * Provides transparent reading/writing of WAV and raw PCM files.
 * Input format is auto-detected (RIFF header check).
 * Output format is determined by filename extension (.wav = WAV, else raw).
 * Supports 8/16/24/32-bit PCM and 32-bit IEEE float WAV.
 */
#ifndef WAV_IO_H
#define WAV_IO_H

#include <stdio.h>

typedef struct {
  FILE *fp;
  int is_wav;                   /* 1 if WAV format, 0 if raw PCM */
  int channels;                 /* Number of channels (from WAV header, or 1 for raw) */
  long sample_rate;             /* Sample rate in Hz (from WAV header, or 0 for raw) */
  int bits_per_sample;          /* Bits per sample (from WAV header, or 16 for raw) */
  int audio_format;             /* WAV format tag: 1=PCM, 3=IEEE float (0 for raw) */
  long data_offset;             /* Byte offset to start of PCM data */
  long data_size;               /* Bytes of PCM data written (for WAV write) */
  int warned_multichan;         /* Flag: already warned about multi-channel extraction */
  int write_mode;               /* 1 if opened for writing, 0 for reading */
} AUDIO_FILE;

/* Open file for reading. Auto-detects WAV vs raw PCM.
 * expected_rate: if > 0, error if WAV sample rate doesn't match
 * expected_channels: if > 0, error if WAV channel count doesn't match
 * expected_bits: if > 0, error if WAV bits_per_sample doesn't match
 * Returns NULL on error (prints message to stderr). */
AUDIO_FILE *audio_open_read (const char *filename, long expected_rate, int expected_channels, int expected_bits);

/* Open file for writing. Writes WAV if filename ends in .wav, raw otherwise.
 * sample_rate/channels/bits_per_sample used for WAV header.
 * For raw output, these are ignored.
 * Returns NULL on error. */
AUDIO_FILE *audio_open_write (const char *filename, long sample_rate, int channels, int bits_per_sample);

/* Read up to nsamples into buffer, using the appropriate sample size
 * based on the file's bits_per_sample (8/16/24/32-bit PCM or 32-bit float).
 * Buffer must be large enough for the native sample size.
 * For multi-channel WAV, extracts channel 0 (warns once on stderr).
 * Returns number of samples actually read. */
long audio_read (AUDIO_FILE * af, void *buffer, long nsamples);

/* Write nsamples from buffer, using the appropriate sample size
 * based on the file's bits_per_sample.
 * Returns number of samples actually written. */
long audio_write (AUDIO_FILE * af, void *buffer, long nsamples);

/* Close file. For WAV output, updates header with final data size. */
void audio_close (AUDIO_FILE * af);

/* Returns 1 if file was detected/opened as WAV, 0 if raw */
int audio_is_wav (AUDIO_FILE * af);

/* Returns sample rate from WAV header (0 if raw) */
long audio_get_sample_rate (AUDIO_FILE * af);

/* Returns channel count from WAV header (0 if raw) */
int audio_get_channels (AUDIO_FILE * af);

/* Seek to a byte offset relative to the start of PCM data.
 * For WAV files, accounts for the header (data_offset).
 * For raw files, seeks from byte 0.
 * Returns 0 on success, -1 on failure. */
int audio_seek (AUDIO_FILE * af, long offset);

/* Returns total size of PCM data in bytes.
 * For WAV, returns the data chunk size from the header.
 * For raw, returns the file size. */
long audio_get_data_size (AUDIO_FILE * af);

#endif /* WAV_IO_H */
