/*---------------------------------------------------------------------------*
 * Delay-and-error profile to FER pattern conversion tool, V1.2             *
 * ------------------------------------------                               *
 * (C) 2012 Fraunhofer IIS.                                                 *
 *                                                                          *
 * ===============================================================          *
 * COPYRIGHT NOTE: This source code, and all of its derivations,            *
 * is subject to the "ITU-T General Public License". Please have            *
 * it read in the distribution disk, or in the ITU-T Recommendation         *
 * G.191 on "SOFTWARE TOOLS FOR SPEECH AND AUDIO CODING STANDARDS".         *
 * See LICENSE.md in the top-level directory for terms.                      *
 * ===============================================================          *
 *                                                                          *
 * Fraunhofer IIS makes no representation nor warranty in regard to         *
 * the accuracy, completeness or sufficiency of The Software, nor           *
 * shall Fraunhofer IIS be held liable for any damages whatsoever           *
 * relating to use of said Software.                                        *
 *---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ugstdemo.h"
#include "softbit.h"

static void usage() {
  fprintf(stdout, "\nConvert a delay and error profile to an error pattern\n");
  fprintf(stdout, "\nValid commandline switches:\n");
  fprintf(stdout, "-i <inputfile>\n");
  fprintf(stdout, "-o <outputfile>\n");
  fprintf(stdout, "-L <length in frames>\n");
  fprintf(stdout, "-s <shift/offset in frames>\n");
  fprintf(stdout, "-f <frames_per_packet> [1;2]\n");
  fprintf(stdout, "-l <late_loss_rate in percent>\n");
  fprintf(stdout, "-b use byte-oriented G.192 format (0x21 okay, 0x20 lost)\n");
  fprintf(stdout, "-w use word-oriented G.192 format (0x6b21 okay, 0x6b20 lost)\n");
  fprintf(stdout, "-c use LF for text format to have one entry per line - was default in V1.0\n");
  fprintf(stdout, "-d <constant JBM delay in milliseconds>\n");
  fprintf(stdout, " either -l or -d parameter must be supplied (not both!) and a valid inputfile\n");
}

/* Write one frame erasure flag to output */
static void write_flag(short flag, int useG192, int useG192WordOriented, int useLF, FILE *outfile) {
  if (useG192) {
    if (useG192WordOriented)
      save_g192(&flag, 1, outfile);
    else
      save_byte(&flag, 1, outfile);
  } else {
    /* Text mode: 0 = good, 1 = erased */
    if (useLF)
      fprintf(outfile, "%c\n", flag == G192_FER ? '1' : '0');
    else
      fprintf(outfile, "%c", flag == G192_FER ? '1' : '0');
  }
}

int main(int argc, char **argv) {
  int i;
  char *infilename = NULL;
  char *outfilename = NULL;
  float late_loss_rate = 0.0f;
  unsigned int constant_delay_ms = 0;
  int shift = 0;
  int length = 7500;
  unsigned int framesPerPacket = 1; /* no aggregation by default */
  int useG192 = 0;
  int useG192WordOriented = 0;
  int useLF = 0;
  int retval = 0;

  char line[64] = {0};
  unsigned int delayCount[2001] = {0}; /* [-1;1999] ms */
  unsigned int line_cnt = 0;

  FILE *infile = NULL;
  FILE *outfile = NULL;

  while (argc > 1 && argv[1][0] == '-') {
    if (strcmp(argv[1], "-i") == 0) {
      if (argc < 3) { usage(); return -1; }
      infilename = argv[2];
      argc -= 2; argv += 2;
    } else if (strcmp(argv[1], "-o") == 0) {
      if (argc < 3) { usage(); return -1; }
      outfilename = argv[2];
      argc -= 2; argv += 2;
    } else if (strcmp(argv[1], "-l") == 0) {
      if (argc < 3) { usage(); return -1; }
      late_loss_rate = (float)atof(argv[2]);
      argc -= 2; argv += 2;
    } else if (strcmp(argv[1], "-d") == 0) {
      if (argc < 3) { usage(); return -1; }
      constant_delay_ms = atoi(argv[2]);
      argc -= 2; argv += 2;
    } else if (strcmp(argv[1], "-s") == 0) {
      if (argc < 3) { usage(); return -1; }
      shift = atoi(argv[2]);
      argc -= 2; argv += 2;
    } else if (strcmp(argv[1], "-L") == 0) {
      if (argc < 3) { usage(); return -1; }
      length = atoi(argv[2]);
      argc -= 2; argv += 2;
    } else if (strcmp(argv[1], "-f") == 0) {
      if (argc < 3) { usage(); return -1; }
      framesPerPacket = atoi(argv[2]);
      argc -= 2; argv += 2;
    } else if (strcmp(argv[1], "-b") == 0) {
      useG192 = 1;
      argc--; argv++;
    } else if (strcmp(argv[1], "-w") == 0) {
      useG192 = 1;
      useG192WordOriented = 1;
      argc--; argv++;
    } else if (strcmp(argv[1], "-c") == 0) {
      useLF = 1;
      argc--; argv++;
    } else if (strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "--help") == 0) {
      usage();
      return 0;
    } else {
      fprintf(stderr, "ERROR! Invalid option \"%s\" in command line\n\n", argv[1]);
      usage();
      return -1;
    }
  }

  if ((infilename == NULL) || ((late_loss_rate == 0) && (constant_delay_ms == 0)) ||
      framesPerPacket == 0U || framesPerPacket > 2U) {
    usage();
    return -1;
  }

  infile = fopen(infilename, "r");
  if (!infile) {
    fprintf(stderr, "unable to open %s\n", infilename);
    return -2;
  }

  if (outfilename) {
    outfile = fopen(outfilename, useG192 ? WB : "w");
    if (!outfile) {
      fprintf(stderr, "unable to open %s\n", outfilename);
      retval = -2;
      goto cleanup;
    }
  }

  /* read offset to Nirvana */
  for (i = 0; i < shift; i++) {
    if (!fgets(line, 63, infile)) {
      fprintf(stderr, "shift out of range\n");
      retval = -3;
      goto cleanup;
    }
  }

  /* create delay histogram */
  for (i = 0; i < length; i++) {
    int delay_ms;
    if (!fgets(line, 63, infile)) {
      rewind(infile);
      if (!fgets(line, 63, infile)) {
        fprintf(stderr, "unable to rewind and read file\n");
        retval = -4;
        goto cleanup;
      }
    }
    line_cnt++;
    delay_ms = atoi(line);
    if ((delay_ms < -1) || (delay_ms > 1999)) {
      fprintf(stderr, "value in line %u out of range - aborting\n", line_cnt);
      retval = -5;
      goto cleanup;
    }
    if (delay_ms == -1)
      delayCount[2000]++;
    else
      delayCount[delay_ms]++;
  }

  if (late_loss_rate) {
    unsigned int late_loss_cnt = 0;
    for (i = 1999; i >= 0; i--) {
      float current_late_loss_rate;
      late_loss_cnt += delayCount[i];
      current_late_loss_rate = ((float)late_loss_cnt * 100.0f) / (float)line_cnt;
      if (current_late_loss_rate > late_loss_rate) {
        constant_delay_ms = i;
        fprintf(stdout, "selected %ims to stay below %f late loss\n", i, late_loss_rate);
        break;
      }
    }
  }

  /**** emulate constant delay JBM with given delay ****/
  fseek(infile, 0, SEEK_SET);

  /* read offset to nirvana, again */
  for (i = 0; i < shift; i++) {
    if (!fgets(line, 63, infile)) {
      fprintf(stderr, "shift out of range\n");
      retval = -3;
      goto cleanup;
    }
  }

  if (constant_delay_ms) {
    unsigned int late_loss_cnt = 0;
    unsigned int network_loss_cnt = 0;
    unsigned int iFramePerPacket;
    for (i = 0; i < length; i++) {
      int delay_ms;
      short flag;
      if (!fgets(line, 63, infile)) {
        rewind(infile);
        if (!fgets(line, 63, infile)) {
          fprintf(stderr, "unable to rewind and read file\n");
          retval = -4;
          goto cleanup;
        }
      }
      delay_ms = atoi(line);

      if (delay_ms == -1) {
        network_loss_cnt++;
        flag = G192_FER;
      } else if ((unsigned int)delay_ms > constant_delay_ms) {
        late_loss_cnt++;
        flag = G192_FER;
      } else {
        flag = G192_SYNC;
      }

      if (outfile) {
        for (iFramePerPacket = 0; iFramePerPacket != framesPerPacket; ++iFramePerPacket)
          write_flag(flag, useG192, useG192WordOriented, useLF, outfile);
      }
    }

    fprintf(stdout, "#processed delay and error values: %u\n", line_cnt);
    fprintf(stdout, "network loss rate: %.3f%%\n", ((float)network_loss_cnt * 100.0f) / (float)line_cnt);
    fprintf(stdout, "late loss rate: %.3f%%\n", ((float)late_loss_cnt * 100.0f) / (float)line_cnt);
    fprintf(stdout, "distorted frames: %u\n", framesPerPacket * (network_loss_cnt + late_loss_cnt));
  }

cleanup:
  fclose(infile);
  if (outfile)
    fclose(outfile);

  return retval;
}
