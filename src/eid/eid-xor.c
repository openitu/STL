/*                                                          02.Feb.2010 v1.2
   =========================================================================

   eid-xor.c
   ~~~~~~~~~~

   Program Description:
   ~~~~~~~~~~~~~~~~~~~~

   This example program performs a "logical" exclusive-or operation of
   a file (representing an encoded speech bitstream) with another file
   (representing a bit error pattern.

   The file containing an encoded speech bitstream can be in a compact
   binary format, in the G.192 serial bitstream format (which uses
   16-bit softbits), or in the byte-oriented G.192 format.

   The file containing the error pattern will be in one of three
   possible formats: G.192 16-bit softbit format (without synchronism
   header for bit errors), byte-oriented version of the G.192 format,
   and compact, hard-bit binary (bit) mode. These are described in the
   following.

   The headerless G.192 serial bitstream format is as described in
   G.192, with the exceptions listed below. The main feature is that
   the softbits and frame erasure indicators are right-aligned at
   16-bit word boundaries (unsigned short):
   '0'=0x007F and '1'=0x0081, and good/bad frame = 0x6B21/0x6B20

   In the byte-oriented softbit serial bitstream, only the lower byte
   of the softbits defined in G.192 are used. Hence:
   '0'=0x7F and '1'=0x81, and good/bad frame = 0x21/0x20

   In the compact (bit) mode, only hard bits are saved. Each byte will
   have information about eight bits or frames. The LBbs will refer to
   bits or frames that occur first in time. Here, '1' means that a bit
   is in error or that a frame should be erased, and a '0', otherwise.

   Conventions:
   ~~~~~~~~~~~~

   Bitstreams can be disturbed in two ways: by bit errors, or by frame
   erasures. The STL EID supports three basic modes: random/bit errors
   (labeled BER), simple frame erasure (labeled FER), and Bellcore
   model burst frame erasure (labeled BFER). Here are some conventions
   that apply to the particular formats for each of these three EID
   operating modes.

   BER: bitstream generated by this program are composed of bits 1/0,
        *without* synchronism headers or any other frame delimitation
        (i.e., only bits affecting the payload are present). Frame
        boundaries are defined by the user's application only. The
        following applies:
        G.192 mode: file will contain either 0x007F (no disturbance) or
	            0x0081 (bit error)
        Byte mode:  file will contain either 0x7F (no disturbance) or
	            0x81 (bit error)
        Compact mode: each bit in the file will indicate whether a
	            disturbance occurred (bit 1) or not (bit 0).
		    Lower order bits apply to bits occurring first
	            in time.

   FER/BFER: bitstream generate by this program is composed only by
        the indication of whether a frame should be erased or not. No
        payload is present. The following applies:
        G.192 mode: file will contain either 0x6B21 (no disturbance) or
	            0x6B20 (frame erasure)
        Byte mode:  file will contain either 0x21 (no disturbance) or
	            0x20 (frame erasure)
        Compact mode: each bit in the file will indicate whether a
	            frame erasure occurred (bit 1) or not (bit 0).
		    Lower order bits apply to bits occurring first
	            in time.

   Usage:
   ~~~~~
   eid-xor [Options] in_bs err_pat_bs out_bs
   Where:
   in_bs ...... input encoded speech bitstream file
   err_pat .... error pattern bitstream file
   out_bs ..... disturbed encoded speech bitstream file

   Options:
   -frame # ... Set the frame size to #. Necessary for headerless G.192
                bitstreams or for compact binary files.
   -bs mode ... Mode for bitstream (g192, byte, or bit)
   -ep mode ... Mode for error pattern (g192, byte, or bit)
   -ber ....... Error pattern is a bit error pattern (needed for bit format)
   -fer ....... Error pattern is a frame erasure pattern (for bit format)
   -vbr ....... Enables variable bit rate operation
   -q ......... Quiet operation
   -? ......... Displays this message
   -help ...... Displays a complete help message

   Original Author:
   ~~~~~~~~~~~~~~~~
   Simao Ferraz de Campos Neto
   Comsat Laboratories                  Tel:    +1-301-428-4516
   22300 Comsat Drive                   Fax:    +1-301-428-9287
   Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

   History:
   ~~~~~~~~
   15.Aug.97 v.1.0 Created based on eid-xor.c <simao.campos@comsat.com>
   09.Jun.05 v.1.1 Bug correction during EP file reading. <Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com>
   02.Feb.10 v.1.2 Modified maximum string length for filenames to avoid
                   buffer overruns (y.hiwasaki)

   ========================================================================= */

/* ..... Generic include files ..... */
#include "ugstdemo.h"           /* general UGST definitions */
#include <stdio.h>              /* Standard I/O Definitions */
#include <math.h>
#include <stdlib.h>
#include <string.h>             /* memset */
#include <ctype.h>              /* toupper */

/* ..... OS-specific include files ..... */
#if defined (unix) && !defined(MSDOS)
/*                 ^^^^^^^^^^^^^^^^^^ This strange construction is necessary
                                      for DJGPP, because "unix" is defined,
				      even it being MSDOS! */
#if defined(__ALPHA)
#include <unistd.h>             /* for SEEK_... definitions used by fseek() */
#else
#include <sys/unistd.h>         /* for SEEK_... definitions used by fseek() */
#endif
#endif




/* ..... Module definition files ..... */
#include "softbit.h"            /* Soft bit definitions and prototypes */

/* ..... Definitions used by the program ..... */

/* Generic definitions */
#define EID_BUFFER_LENGTH 256
#define OUT_RECORD_LENGTH 512


/* Local function prototypes */
short eid_xor ARGS ((int a, int b));
long insert_errors ARGS ((short *a, short *b, short *c, long n));
void display_usage ARGS ((int level));


/* ************************* AUXILIARY FUNCTIONS ************************* */

/*
  Perform XOR operation on two samples
*/
short eid_xor (int a, int b) {
  return (a ^ b ? G192_ONE : G192_ZERO);
}

/* ......................... End of eid_xor() ......................... */


/*
  Insert errors by XOR-ing the input data arrays and saving the
  disturbed data in a third array.
*/
long insert_errors (short *a, short *b, short *c, long n)  {
  long i;
  long register disturbed;
  short bit;

  for (disturbed = i = 0; i < n; i++) {
    bit = eid_xor (a[i], b[i]);
    if (bit != a[i])
      disturbed++;
    c[i] = bit;
  }
  return (disturbed);
}

/* ....................... End of insert_errors() ....................... */


/*
   --------------------------------------------------------------------------
   display_usage(int level);

   Shows program usage.

   History:
   ~~~~~~~~
   11/Aug/1997  v1.0 Created <simao>
   --------------------------------------------------------------------------
 */
#define P(x) printf x
void display_usage (int level) {
  P (("eid-xor.c - Version 1.2 of 02/Feb/2010 \n\n"));

  if (level) {
    P (("Program Description:\n"));
    P (("\n"));
    P (("This example program performs a \"logical\" exclusive-or operation of\n"));
    P (("a file (representing an encoded speech bitstream) with another file\n"));
    P (("(representing a bit error pattern.\n"));
    P (("\n"));
    P (("The file containing an encoded speech bitstream can be in a compact\n"));
    P (("binary format, in the G.192 serial bitstream format (which uses\n"));
    P (("16-bit softbits), or in the byte-oriented G.192 format.\n"));
    P (("\n"));
    P (("The file containing the error pattern will be in one of three\n"));
    P (("possible formats: G.192 16-bit softbit format (without synchronism\n"));
    P (("header for bit errors), byte-oriented version of the G.192 format,\n"));
    P (("and compact, hard-bit binary (bit) mode. These are described in the\n"));
    P (("following.\n"));
    P (("\n"));
    P (("The headerless G.192 serial bitstream format is as described in\n"));
    P (("G.192, with the exceptions listed below. The main feature is that\n"));
    P (("the softbits and frame erasure indicators are right-aligned at\n"));
    P (("16-bit word boundaries (unsigned short): \n"));
    P (("'0'=0x007F and '1'=0x0081, and good/bad frame = 0x6B21/0x6B20\n"));
    P (("\n"));

    P (("In the byte-oriented softbit serial bitstream, only the lower byte\n"));
    P (("of the softbits defined in G.192 are used. Hence:\n"));
    P (("'0'=0x7F and '1'=0x81, and good/bad frame = 0x21/0x20\n"));
    P (("\n"));
    P (("In the compact (bit) mode, only hard bits are saved. Each byte will\n"));
    P (("have information about eight bits or frames. The LBbs will refer to\n"));
    P (("bits or frames that occur first in time. Here, '1' means that a bit\n"));
    P (("is in error or that a frame should be erased, and a '0', otherwise.\n"));
    P (("\n"));
    P (("Conventions:\n"));
    P (("~~~~~~~~~~~~\n"));
    P (("\n"));
    P (("Bitstreams can be disturbed in two ways: by bit errors, or by frame\n"));
    P (("erasures. The STL EID supports three basic modes: random/bit errors\n"));
    P (("(labeled BER), simple frame erasure (labeled FER), and Bellcore\n"));
    P (("model burst frame erasure (labeled BFER). Here are some conventions\n"));
    P (("that apply to the particular formats for each of these three EID\n"));
    P (("operating modes.\n"));
    P (("\n"));

    P (("BER: bitstream generated by this program are composed of bits 1/0,\n"));
    P (("     *without* synchronism headers or any other frame delimitation\n"));
    P (("     (i.e., only bits affecting the payload are present). Frame\n"));
    P (("     boundaries are defined by the user's application only. The\n"));
    P (("     following applies:\n"));
    P (("     G.192 mode: file will contain either 0x007F (no disturbance) or\n"));
    P (("	            0x0081 (bit error)\n"));
    P (("     Byte mode:  file will contain either 0x7F (no disturbance) or\n"));
    P (("	            0x81 (bit error)\n"));
    P (("     Compact mode: each bit in the file will indicate whether a\n"));
    P (("	            disturbance occurred (bit 1) or not (bit 0).\n"));
    P (("		    Lower order bits apply to bits occurring first\n"));
    P (("	            in time.\n"));
    P (("\n"));
    P (("FER/BFER: bitstream generate by this program is composed only by\n"));
    P (("     the indication of whether a frame should be erased or not. No\n"));
    P (("     payload is present. The following applies:\n"));
    P (("     G.192 mode: file will contain either 0x6B21 (no disturbance) or\n"));
    P (("	            0x6B20 (frame erasure)\n"));
    P (("     Byte mode:  file will contain either 0x21 (no disturbance) or\n"));
    P (("	            0x20 (frame erasure)\n"));
    P (("     Compact mode: each bit in the file will indicate whether a frame\n"));
    P (("	            erasure occurred (bit 1) or not (bit 0). Lower order\n"));
    P (("		    bits apply to bits occurring first in time.\n"));
    P (("\n"));
  } else {
    P (("Program to insert bit errors and frame erasures in bitstream \n"));
    P (("files using a previously generated error pattern. Three formats \n"));
    P (("are acceptable: g192, byte, and (compact) bit.\n\n"));
  }

  P (("Usage:\n"));
  P (("eid-xor [Options] in_bs err_pat_bs out_bs\n"));
  P (("Where:\n"));
  P ((" in_bs ...... input encoded speech bitstream file\n"));
  P ((" err_pat .... error pattern bitstream file\n"));
  P ((" out_bs ..... disturbed encoded speech bitstream file    \n"));
  P (("\n"));
  P (("Options:\n"));
  P ((" -frame # ... Set the frame size to # (for headerless G.192\n"));
  P (("              bitstreams or for compact binary files).\n"));
  P ((" -bs mode ... Mode for bitstream (g192, byte, or bit)\n"));
  P ((" -ep mode ... Mode for error pattern (g192, byte, or bit)\n"));
  P ((" -ber ....... Error pattern is a bit error pattern (needed for bit format)\n"));
  P ((" -fer ....... Error pattern is a frame erasure pattern (for bit format)\n"));
  P ((" -vbr ....... Enables variable bit rate operation (different frame sizes)\n"));
  P ((" -q ......... Quiet operation\n"));
  P ((" -? ......... Displays this message\n"));
  P ((" -help ...... Displays a complete help message\n"));

  /* Quit program */
  exit (-128);
}

#undef P
/* .................... End of display_usage() ........................... */


/* ************************************************************************* */
/* ************************** MAIN_PROGRAM ********************************* */
/* ************************************************************************* */
int main (int argc, char *argv[]) {
  /* Command line parameters */
  char ep_type = BER;           /* Type of error pattern: FER or BER */
  char bs_format = g192;        /* Generic Speech bitstream format */
  char obs_format = g192;       /* Output Speech bitstream format */
  char ep_format = g192;        /* Error pattern format */
  char ibs_file[MAX_STRLEN];    /* Input bitstream file */
  char obs_file[MAX_STRLEN];    /* Output bitstream file */
  char ep_file[MAX_STRLEN];     /* Error pattern file */
  long fr_len = 0;              /* Frame length in bits */
  long bs_len, ep_len;          /* BS and EP lengths, with headers */
  long ep_true_len;             /* number of words read in EP file */
  long ori_bs_len, ori_fr_len;  /* Frame/BS legth memory */
  long blk = EID_BUFFER_LENGTH; /* Standard frame length */
  long start_frame = 1;         /* Start inserting error from 1st one */
  char sync_header = 1;         /* Flag for input BS */
  long wraps = 0;               /* Count how many times wraps the EP file */

  /* File I/O parameter */
  FILE *Fibs;                   /* Pointer to input encoded bitstream file */
  FILE *Fobs;                   /* Pointer to input encoded bitstream file */
  FILE *Fep;                    /* Pointer to error pattern file */
#ifdef DEBUG
  FILE *F;
#endif

  /* Data arrays */
  short *bs;                    /* Encoded speech bitstream */
  short *payload;               /* Point to payload in bitstream */
  short *ep;                    /* Error pattern buffer */
  short *erased_frame;          /* A totally erased frame */

  /* Aux. variables */
  double disturbed = 0;         /* # of distorted bits/frames */
  double processed = 0;         /* # of processed bits/frames */
  char vbr = 0;                 /* Flag for variable bit rate mode */
  long ibs_sample_len;          /* Size (bytes) of samples in the BS */
  char tmp_type;
  long i, k;
  long items;                   /* Number of output elements */
#if defined(VMS)
  char mrs[15] = "mrs=512";
#endif
  char quiet = 0;

  /* Pointer to a function */
  long (*read_data) () = read_g192;     /* To read input bitstream */
  long (*read_patt) () = read_g192;     /* To read error pattern */
  long (*save_data) () = save_g192;     /* To save output bitstream */

  /* ......... GET PARAMETERS ......... */

  /* Check options */
  if (argc < 2)
    display_usage (0);
  else {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp (argv[1], "-start") == 0) {
        /* Define starting sample/frame for error insertion */
        start_frame = atol (argv[2]);

        /* Move arg{c,v} over the option to the next argument */
        argc -= 2;
        argv += 2;
      } else if (strcmp (argv[1], "-frame") == 0) {
        /* Define input & output encoded speech bitstream format */
        fr_len = atoi (argv[2]);

        /* Move arg{c,v} over the option to the next argument */
        argc -= 2;
        argv += 2;
      } else if (strcmp (argv[1], "-bs") == 0) {
        /* Define input & output encoded speech bitstream format */
        for (i = 0; i < nil; i++) {
          if (strstr (argv[2], format_str (i)))
            break;
        }
        if (i == nil) {
          HARAKIRI ("Invalid BS format type. Aborted\n", 5);
        } else
          bs_format = i;

        /* Move arg{c,v} over the option to the next argument */
        argc -= 2;
        argv += 2;
      } else if (strcmp (argv[1], "-ep") == 0) {
        /* Define error pattern format */
        for (i = 0; i < nil; i++) {
          if (strstr (argv[2], format_str (i)))
            break;
        }
        if (i == nil) {
          HARAKIRI ("Invalid error pattern format type. Aborted\n", 5);
        } else
          ep_format = i;

        /* Move arg{c,v} over the option to the next argument */
        argc -= 2;
        argv += 2;
      } else if (strcmp (argv[1], "-ber") == 0 || strcmp (argv[1], "-BER") == 0) {
        /* Error pattern type: BER */
        ep_type = BER;

        /* Move arg{c,v} over the option to the next argument */
        argc--;
        argv++;
      } else if (strcmp (argv[1], "-fer") == 0 || strcmp (argv[1], "-FER") == 0 || strcmp (argv[1], "-bfer") == 0 || strcmp (argv[1], "-BFER") == 0) {
        /* Error pattern type: FER */
        ep_type = FER;

        /* Move arg{c,v} over the option to the next argument */
        argc--;
        argv++;
      } else if (strcmp (argv[1], "-vbr") == 0) {
        /* Enables variable bit rate operation */
        vbr = 1;

        /* Move arg{c,v} over the option to the next argument */
        argc--;
        argv++;
      } else if (strcmp (argv[1], "-q") == 0) {
        /* Set quiet mode */
        quiet = 1;

        /* Move arg{c,v} over the option to the next argument */
        argc--;
        argv++;
      } else if (strcmp (argv[1], "-?") == 0) {
        display_usage (0);
      } else if (strstr (argv[1], "-help")) {
        display_usage (1);
      } else {
        fprintf (stderr, "ERROR! Invalid option \"%s\" in command line\n\n", argv[1]);
        display_usage (0);
      }
  }

  /* Get command line parameters */
  GET_PAR_S (1, "_Input bit stream file ..................: ", ibs_file);
  GET_PAR_S (2, "_Error pattern file .....................: ", ep_file);
  GET_PAR_S (3, "_Output bit stream file .................: ", obs_file);



  /* Starting frame is from 0 to number_of_frames-1 */
  start_frame--;

  /* Open files */
  if ((Fibs = fopen (ibs_file, RB)) == NULL)
    HARAKIRI ("Could not open input bitstream file\n", 1);
  if ((Fep = fopen (ep_file, RB)) == NULL)
    HARAKIRI ("Could not open error pattern file\n", 1);
  if ((Fobs = fopen (obs_file, WB)) == NULL)
    HARAKIRI ("Could not create output file\n", 1);
#ifdef DEBUG
  F = fopen ("ep.g192", WB);    /* File to save the EP in G.192 format */
#endif


  /* *** CHECK CONSISTENCY *** */

  /* Do preliminary inspection in the INPUT BITSTREAM FILE to check its format (byte, bit, g192) */
  i = check_eid_format (Fibs, ibs_file, &tmp_type);

  /* Check whether the specified BS format matches with the one in the file */
  if (i != bs_format) {
    /* The input bitstream format is not the same as specified */
    fprintf (stderr, "*** Switching bitstream format from %s to %s ***\n", format_str ((int) bs_format), format_str (i));
    bs_format = i;
  }

  /* Check whether the BS has a sync header */
  if (tmp_type == FER) {
    /* The input BS may have a G.192 synchronism header - verify */
    if (bs_format == g192) {
      short tmp[2];

      /* Get presumed first G.192 sync header */
      fread (tmp, sizeof (short), 2, Fibs);
      /* tmp[1] should have the frame length */
      i = tmp[1];
      /* advance file to the (presumed) next G.192 sync header */
      fseek (Fibs, (long) (tmp[1]) * sizeof (short), SEEK_CUR);
      /* get (presumed) next G.192 sync header */
      fread (tmp, sizeof (short), 2, Fibs);
      /* Verify */
      /* if (((tmp[0] & 0xFFF0) == 0x6B20) && (i == tmp[1])) */
      if ((tmp[0] & 0xFFF0) == 0x6B20) {
        fr_len = i;
        sync_header = 1;
        if (i != tmp[1])
          vbr = 1;
      } else
        sync_header = 0;
    } else if (bs_format == byte) {
      char tmp[2];

      /* Get presumed first byte-wise G.192 sync header */
      fread (tmp, sizeof (char), 2, Fibs);
      /* tmp[1] should have the frame length */
      i = tmp[1];
      /* advance file to the (presumed) next byte-wise G.192 sync header */
      fseek (Fibs, (long) tmp[1], SEEK_CUR);
      /* get (presumed) next G.192 sync header */
      fread (tmp, sizeof (char), 2, Fibs);
      /* Verify */
      /* if (((tmp[0] & 0xF0) == 0x20) && (i == tmp[1])) */
      if ((tmp[0] & 0xF0) == 0x20) {
        fr_len = i;
        sync_header = 1;
        if (i != tmp[1])
          vbr = 1;
      } else
        sync_header = 0;
    } else
      sync_header = 0;

    /* Rewind file */
    fseek (Fibs, 0l, SEEK_SET);
  }

  /* If input BS is headerless, any frame size will do; using default */
  if (fr_len == 0)
    fr_len = blk;


  /* Do preliminary inspection in the ERROR PATTERN FILE to check its format (byte, bit, g192) */
  i = check_eid_format (Fep, ep_file, &tmp_type);

  /* Check whether the specified EP format matches with the one in the file */
  if (i != ep_format) {
    /* The error pattern format is not the same as specified */
    fprintf (stderr, "*** Switching error pattern format from %s to %s ***\n", format_str ((int) ep_format), format_str (i));
    ep_format = i;
  }

  /* Check whether the specified EP type matches with the one in the file */
  if (tmp_type != ep_type) {
    /* The error pattern type is not the same as specified */

    if (ep_format == compact) {
      fprintf (stderr, "*** Cannot infer error pattern type. Using %s ***\n", type_str ((int) ep_type));
    } else {
      fprintf (stderr, "*** Switching error pattern type from %s to %s ***\n", type_str ((int) ep_type), type_str ((int) tmp_type));
      ep_type = tmp_type;
    }
  }

  /* VBR operation is not possible with compact or headerless bitstreams */
  if (vbr && (bs_format == compact || !sync_header)) {
    vbr = 0;
    if (bs_format == compact)
      fprintf (stderr, "VBR operation disabled for compact bitstreams!\n");
    else
      fprintf (stderr, "VBR operation disabled for headerless bitstreams!\n");
  }

  /* One final check: the FER operation is not possible when the output bitstream format is compact, since it is not possible to convey the information that a frame was erased without a header *and* without softbits. The STL conveys frame erasure information based on either of these two mechanisms. If this anomalous situation is found, the output bitstream is redefined as having the g192 format */
  if (bs_format == compact && ep_type == FER)
    obs_format = g192;
  else
    obs_format = bs_format;

  /* Also, reset sync if BS is compact */
  if (bs_format == compact && sync_header) {
    fprintf (stderr, "*** Disabling SYNC header for compact bitstream ***\n");
    sync_header = 0;
  }

  /* *** FINAL INITIALIZATIONS *** */

  /* Use the proper data I/O functions */
  read_data = bs_format == byte ? read_byte : (bs_format == g192 ? read_g192 : read_bit_ber);
  read_patt = ep_format == byte ? read_byte : (ep_format == g192 ? read_g192 : (ep_type == BER ? read_bit_ber : read_bit_fer));
  save_data = obs_format == byte ? save_byte : (obs_format == g192 ? save_g192 : save_bit);

  /* Define BS sample size, in bytes */
  ibs_sample_len = bs_format == byte ? 1 : (bs_format == g192 ? 2 : 0);

  /* Inspect the bitstream file for variable frame sizes (i.e. variable bit rate operation of the codec), if the option vbr is set (NOT the default). NOTE: VBR operation is not possible for compact bitstreams! */
  if (vbr) {
    /* Two local variables */
    short offset = 0;           /* Where is next frame leng.val. in the BS */
    long max_fr_len = fr_len;   /* Maximum frame length found in file */

    /* Scan file for largest frame size */
    while (!feof (Fibs)) {
      /* Move to position where next frame length value is expected */
      fseek (Fibs, (long) (ibs_sample_len + offset), SEEK_CUR);

      /* get (presumed) next G.192 sync header */
      if ((items = read_data (&offset, 1l, Fibs)) != 1)
        break;

      /* We have a different frame length here! */
      if (offset > max_fr_len)
        max_fr_len = offset;

      /* Convert offset number read to no.of bytes */
      offset *= ibs_sample_len;
    }

    /* Rewind file */
    fseek (Fibs, 0l, SEEK_SET);

    /* Set the frame length to the maximum possible value */
    fr_len = max_fr_len;
  }

  /* Define how many samples are read for each frame */
  /* Bitstream may have sync headers, which are 2 samples-long */
  bs_len = sync_header ? fr_len + 2 : fr_len;
  ep_len = fr_len;

  /* Save frame lengths in memory */
  ori_bs_len = bs_len;
  ori_fr_len = fr_len;

  /* Allocate memory for data buffers */
  if ((bs = (short *) calloc (bs_len, sizeof (short))) == NULL)
    HARAKIRI ("Can't allocate memory for bitstream. Aborted.\n", 6);
  if ((ep = (short *) calloc (ep_len, sizeof (short))) == NULL)
    HARAKIRI ("Can't allocate memory for error pattern. Aborted.\n", 6);

  /* Initializes to the start of the payload in input bitstream */
  payload = sync_header ? bs + 2 : bs;

  /* Prepare a totally-erased frame */
  /* ... allocate memory */
  if ((erased_frame = (short *) calloc (bs_len, sizeof (short))) == NULL)
    HARAKIRI ("Can't allocate memory for erased frame. Aborted.\n", 6);

  /* ... set the frame samples to zero (total uncertainty) */
  memset (erased_frame, 0, bs_len * sizeof (short));

  /* ... put sync header, if one should be used */
  if (sync_header) {
    erased_frame[0] = G192_FER; /* Erased frame indication */
    erased_frame[1] = fr_len;   /* The default one; may change in VBR mode */
  }


  /* *** START ACTUAL WORK *** */

  switch (ep_type) {
  case FER:
    k = 0;
    while (1) {
      /* Read one frame from BS: two steps for VBR mode, one otherwise */
      if (vbr) {
        /* Get sync header to see how many samples are in this frame */
        if ((items = read_data (bs, 2l, Fibs)) != 2)
          break;
        fr_len = bs[1];
        bs_len = sync_header ? fr_len + 2 : fr_len;

        /* ... and read payload, if not an empty frame */
        if (fr_len != 0)
          items += read_data (payload, fr_len, Fibs);
      } else
        /* Read one whole frame from bitstream */
        items = read_data (bs, bs_len, Fibs);

      /* Stop when reaches end-of-file */
      if (items == 0)
        break;

      /* Aborts on error */
      if (items < 0)
        KILL (ibs_file, 7);

      /* Check if read all expected samples; if not, take a special action */
      if (items < bs_len) {
        if (sync_header) {
          /* If the bitstream has sync header, this situation should not occur, since the length of the input bitstream file should be a multiple of the frame size! The file is either invalid otr corrupt. Execution is aborted at this point */
          fprintf (stderr, "%s\n%s\n%s\n%s\n", "*** File size for this bitstream file is not multiple  ***", "*** of the given frame length. Check that the correct  ***", "*** frame size was used (is this a variable-frame size ***", "*** file?) and that the bitstream is not corrupted.***");
          exit (9);
        } else if (feof (Fibs)) {
          /* EOF reached. Since the input bitstream is headerless, this maybe a corrupt file, or the user simply specified the wrong frame size. Warn the user and continue */
          fprintf (stderr, "%s\n%s\n%s\n%s\n", "*** File size for this HEADERLESS bitstream is not ***", "*** multiple of the given frame length. Check that ***", "*** the correct frame size was selected & that the ***", "*** bitstream file is not corrupted.***");
          bs_len = fr_len = items;
        } else                  /* An unknown error happened! */
          KILL (ibs_file, 7);
      }
      /* Read a number of erasure flags from file */
      while (k == 0) {
        /* No EP flags in buffer; read a number of them */
        ep_true_len = k = read_patt (ep, ep_len, Fep);

        /* No flags read - either error or EOF */
        /* Go back to beginning of EP & fill up EP buffer */
        if (k <= 0) {
          if (k < 0)
            KILL (ep_file, 7);  /* Error: abort */
          fseek (Fep, 0l, SEEK_SET);    /* EOF: Rewind */
          wraps++;              /* Count how many times wrapped EP */
        }
      }

      /* Update frame counters */
      processed++;

      /* Save original or erased frame, as appropriate */
      if (ep[ep_true_len - k] == G192_FER) {
        /* If in VBR mode, update frame length value */
        if (vbr)
          erased_frame[1] = fr_len;
        /* Write erased frame */
        items = save_data (erased_frame, bs_len, Fobs);
        disturbed++;
      } else
        /* Write undisturbed frame */
        items = save_data (bs, bs_len, Fobs);

      /* Abort on error */
      if (items < bs_len)
        KILL (obs_file, 7);

      /* Decrement counter of number of flags in EP buffer */
      k--;
    }
    break;

  case BER:
    while (1) {
      /* Read one frame from BS: two steps for VBR mode, one otherwise */
      if (vbr) {
        /* Get sync header to see how many samples are in this frame */
        if ((items = read_data (bs, 2l, Fibs)) != 2)
          break;
        fr_len = bs[1];
        bs_len = sync_header ? fr_len + 2 : fr_len;

        /* ... and read payload, if not an empty frame */
        if (fr_len != 0)
          items += read_data (payload, fr_len, Fibs);
      } else
        /* Read one whole frame from bitstream */
        items = read_data (bs, bs_len, Fibs);

      /* Stop when reaches end-of-file */
      if (items == 0)
        break;

      /* Aborts on error */
      if (items < 0)
        KILL (ibs_file, 7);

      /* Check if read all expected samples; if not, probably hit EOF */
      if (items < bs_len) {
        if (sync_header) {
          /* This situation should not occur in a headed BS - Abort */
          fprintf (stderr, "%s\n%s\n%s\n%s\n", "*** File size for this bitstream file is not multiple  ***", "*** of the given frame length. Check that the correct  ***", "*** frame size was used (is this a variable-frame size ***", "*** file?) and that the bitstream is not corrupted.***");
          exit (9);
        } else if (feof (Fibs)) {
          /* Headerless BS is corrupted or wrong frame size was used */
          /* This is not important for BER, so the msg is not printed */
#ifdef DEBUG
          fprintf (stderr, "%s\n%s\n%s\n%s\n", "*** File size for this HEADERLESS bitstream is not ***", "*** multiple of the given frame length. Check that ***", "*** the correct frame size was selected & that the ***", "*** bitstream file is not corrupted.***");
#endif
          bs_len = fr_len = items;
        } else                  /* An unknown error happened! */
          KILL (ibs_file, 7);
      }

      /* Read one error pattern frame from file */
      items = read_patt (ep, ep_len, Fep);

      /* Treat case when EP finishes before BS: */
      /* Go back to beginning of EP & fill up EP buffer */
      if (items < ep_len) {
        if (items < 0)
          KILL (ep_file, 7);
        k = ep_len - items;     /* Number of missing EP samples */
        fseek (Fep, 0l, SEEK_SET);      /* Rewind */
        items = read_patt (&ep[items], k, Fep); /* Fill-up EP buffer */

        /* Count how many times wrapped the EP file */
        wraps++;
      }
#ifdef DEBUG
      /* Save to a temp file - debugging */
      fwrite (ep, sizeof (short), ep_len, F);
#endif

      /* Convolve errors */
      items = insert_errors (payload, ep, payload, fr_len);

      /* Update BER counters */
      disturbed += items;
      processed += fr_len;

      /* Save disturbed bitstream to file */
      items = save_data (bs, bs_len, Fobs);

      /* Abort on error */
      if (items < bs_len)
        KILL (obs_file, 7);
    }
    break;
  }


  /* *** PRINT SUMMARY OF OPTIONS & RESULTS ON SCREEN *** */

  /* Restore frame lengths from memory (just in case they were changed) */
  bs_len = ori_bs_len;
  fr_len = ori_fr_len;

  /* Print summary */
  fprintf (stderr, "# Bitstream format %s...... : %s\n", sync_header ? "(G.192 header) " : "(headerless) ..", format_str ((int) bs_format));
  if (bs_format != obs_format)
    fprintf (stderr, "# Out bitstream format %s.. : %s\n", sync_header ? "(G.192 header) " : "(headerless) ..", format_str ((int) obs_format));

  fprintf (stderr, "# Pattern format %s....... : %s\n", ep_type == FER ? "(frame erasure) " : "(bit error) ....", format_str ((int) ep_format));
  fprintf (stderr, "# Error pattern files wrapped ...........: %ld times\n", wraps);
  fprintf (stderr, "# Frame size ............................: %ld\n", fr_len);
  fprintf (stderr, "# Processed %s..................... : %.0f \n", ep_type == BER ? "bits .." : "frames ", processed);
  fprintf (stderr, "# Distorted %s..................... : %.0f \n", ep_type == BER ? "bits .." : "frames ", disturbed);
  fprintf (stderr, "# %s.....................: %f %%\n", ep_type == BER ? "Bit error rate ..." : "Frame erasure rate", 100.0 * disturbed / processed);


  /* *** FINALIZATIONS *** */

  /* Free memory allocated */
  free (erased_frame);
  free (ep);
  free (bs);

  /* Close the output file and quit *** */
  fclose (Fibs);
  fclose (Fep);
  fclose (Fobs);
#ifdef DEBUG
  fclose (F);
#endif

#ifndef VMS                     /* return value to OS if not VMS */
  return 0;
#endif
}
