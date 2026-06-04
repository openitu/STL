/*---------------------------------------------------------------------------*
 * Randomization tool, V1.1                                                  *
 * ------------------------------------------                                *
 * (C) 2012 Fraunhofer IIS. All rights reserved.                             *
 *                                                                           *
 *      ===============================================================      *
 *      COPYRIGHT NOTE: This source code, and all of its derivations,        *
 *      is subject to the "ITU-T General Public License". Please have        *
 *      it read in the distribution disk, or in the ITU-T Recommendation     *
 *      G.191 on "SOFTWARE TOOLS FOR SPEECH AND AUDIO CODING STANDARDS".     *
 *      See LICENSE.md in the top-level directory for terms.                 *
 *      ===============================================================      *
 *                                                                           *
 * The LCS generator is copied from ITU-T G.191, file STL/eid.c.             *
 *                                                                           *
 * Fraunhofer IIS makes no representation nor warranty in regard to          *
 * the accuracy, completeness or sufficiency of The Software, nor            *
 * shall Fraunhofer IIS be held liable for any damages whatsoever            *
 * relating to use of said Software.                                         *
 *---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define MAX_ITEMS 1000
#define VERBOSE 0
#define DEFAULT_SEED 3141592653U

typedef enum {
  SUBSET = 0,
  FROM_RANGE = 1
} MODE;

/*
============================================================================
       double EID_random (unsigned long *seed); inherited from STL/eid.c,
       ~~~~~~~~~~~~~~~~~

       Description:
       ~~~~~~~~~~~~
       Returns a new random number, generated a linear congruential
       sequence (LCS) generator. See: Knuth, D.E. 1981: "Seminumerical
       Algorithms" vol.2 of The Art of Computer Programming; Reading,
       Mass.; Addison-Wesley.

       Parameters:
       ~~~~~~~~~~~
       seed: ... long seed.

       Return value:
       ~~~~~~~~~~~~~
       Returns a random number as double in the range 0..1.

       Author: <hf@pkinbg.uucp>
       ~~~~~~~
       History:
       ~~~~~~~~
       28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>
       20.Apr.92 v2.0 Change of polinomial generator to LCG
                      <hf@pkinbg.uucp>
       26.Jan.98 v3.0 Corrected bug for 64-bit operating systems (where
                      longs have 64, not 32 bits). Implemented by
                      <simao.campos@comsat.com>, after bug reported
                      by <claude.lamblin@cnet.francetelecom.fr>
       12.Aug.12 v4.0 Changed unsigned long to unsigned int to have identical
                      behavior on at least Windows, Linux, OSX
============================================================================ */

static double EID_random(seed)
unsigned int *seed;
{
  /* Size in bits (=size in bytes * 8) for int variables*/
  static double bits_in_int = sizeof(unsigned int) * 8;
  /* Update RNG */
  *seed = ((unsigned int)69069L * (*seed) +1L);
  /* Return random number as a double */
#ifdef WAS
  return(pow((double)2.0, (double)-32.0) * (double)(*seed));
#else
  return(pow((double)2.0, -bits_in_int) * (double)(*seed));
#endif
}

static void usage()
{
  fprintf( stdout,"usage: random [OPTIONS] [ITEM_LIST]\n" );
  fprintf( stdout,"  OPTIONS:\n" );
  fprintf( stdout,"   -s SEED         any number in between 0 and %u: as a seed for the RNG, default: %u\n", UINT_MAX, DEFAULT_SEED );
  fprintf( stdout,"   -d PRERUNS      number of dummy pre-runs to alter the seed, default: 0\n" );
  fprintf( stdout,"   -r START STOP   range mode, values define lowest and highest allowed value, not set by default\n" );
  fprintf( stdout,"   -n NUM_ITEMS    select more than one item from the provided item list, default: 1\n" );
  exit(-1);
}

int main( int argc, char** argv )
{
  unsigned int seed = DEFAULT_SEED;
  MODE mode = SUBSET;
  unsigned int num_items = 1;
  unsigned int num_dummyruns = 0;
  unsigned int items_in_list = 0;
  unsigned int selected_item = 0;
  unsigned int verbose = 0;
  unsigned int i = 0, j = 0;
  unsigned int range_start = 0, range_stop = 1;
  double rand_f = 0.0f;
  char * item_list [MAX_ITEMS] = {0};

  for(i=1; i<argc; ++i) {
    /* help */
    if(!strcmp(argv[i],"-h")) {
      usage();
    }
    /* verbose output */
    if(!strcmp(argv[i],"-v")) {
      verbose = 1;
    }
    /* seed */
    if(!strcmp(argv[i],"-s")) {
      if(++i >= argc) usage();
      seed = atoi(argv[i]);
      continue;
    }
    /* num items to select */
    if(!strcmp(argv[i],"-n")) {
      if(++i >= argc) usage();
      num_items = atoi(argv[i]);
      continue;
    }
    /* num dummy runs before the item selection is started - similar to  different seed */
    if(!strcmp(argv[i],"-d")) {
      if(++i >= argc) usage();
      num_dummyruns = atoi(argv[i]);
      continue;
    }
    /* select item within given range instead of a subset */
    if(!strcmp(argv[i],"-r")) {
      mode = FROM_RANGE;
      if(++i >= argc) usage();
      range_start = atoi(argv[i]);
      if(++i >= argc) usage();
      range_stop = atoi(argv[i]);
      continue;
    }
    break;
  }

  if(mode == SUBSET) {
    /* list items are the last */
    if(i == (argc)) usage();
    items_in_list = argc - i;
    if(items_in_list > MAX_ITEMS)
    {
      fprintf(stderr, "only up to %u items are supported by this tool\n", MAX_ITEMS);
      exit(-2);
    }
  }

  for(i=0; i<num_dummyruns; i++) {
    EID_random(&seed);
  }

  if(mode == FROM_RANGE)
  {
    for(j=0; j<num_items; j++) {
      rand_f = ((range_stop - range_start + 1)*EID_random(&seed));
      printf("%i ", range_start + (int)rand_f);
    }
    printf("\n");
  }
  else if(mode == SUBSET)
  {

    if(num_items > items_in_list)
    {
      fprintf(stderr, "requested %u items, but only %u in list\n", num_items, items_in_list);
      exit (-1);
    }

    for(i=0; i<items_in_list; i++) {
      item_list[i] = argv[argc - items_in_list + i];
    }

    for(j=0; j<num_items; j++) {
      rand_f = ((items_in_list)*EID_random(&seed));

      selected_item = (int)rand_f;
      printf("%s ",item_list[selected_item]);

      if(verbose)
        printf("\t", item_list[i]);

      for(i=0; i<items_in_list;i++) {
        if(i<selected_item) {
          if(verbose)
            printf(" %s", item_list[i]);
        }
        else if(i==selected_item) {
          if(verbose)
            printf(" -");
          if(i<items_in_list-1) {
            item_list[i] = item_list[i+1];
            if(verbose)
              printf(" %s", item_list[i]);
          }
        }
        else if(i>selected_item && i<items_in_list-1) {
          item_list[i] = item_list[i+1];
          if(verbose)
            printf(" %s", item_list[i]);
        }
      }
      if(verbose)
        printf("\n");
      items_in_list--;
    }
    if(!verbose)
      printf("\n");
  }

  return 0;
}
