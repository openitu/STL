/* ..... OS-specific include files ..... */
#ifdef VMS
#include <stat.h>
#else
#include <sys/stat.h>
#endif

/* Buffer size definitions */
#define EID_BUFFER_LENGTH 256
#define OUT_RECORD_LENGTH 512

/* Local type definitions */
typedef struct {
  long burst_len;               /* Maximum burst leng.that will be individually observed */
  long *hist;                   /* EP histogram array */
  long count;                   /* Count number of occurences */
  long in_event;                /* Flags to indicate if in a burst event */
  long unexpected;              /* Number of unexpected conversions */
  char first_time;              /* Flag for general initializations */
  short *ep;                    /* Buffer for hard-bit version of error pattern */
  long processed, disturbed;    /* No. of processed/disturbed bits */
  float event_distance;         /* Sum of error/erasure event distances */
  float event_distance_sq;      /* Squared sum of error/erasure event distances */
  long min_distance;            /* Minimum distance between events */
  long max_distance;            /* Maximum distance between events */
  long event_started;           /* Starting point of error/erasure event */
  long last_event;              /* Position of last error/erasure event */
  long event_no;                /* Number of error/erasure events */
} ep_histogram_state;

/* Local function prototypes */
int init_ep_histogram ARGS ((ep_histogram_state * state, long burst_len));
long compute_ep_histogram ARGS ((short *pattern, long items, int ep_type, ep_histogram_state * state, int reset));
void free_ep_histogram ARGS ((ep_histogram_state * state));

/*
  ---------------------------------------------------------------------------
  int init_ep_histogram (ep_histogram_state *state, long burst_len);
  ~~~~~~~~~~~~~~~~~~~~~

  Function to set initial values in the EP histogram state variable
  and allocate memory for EP histogram buffer.

  Parameters:
  ~~~~~~~~~~~
  state ..... pointer to state-variable for error pattern histogram
  burst_len . longest burst length to look for

  Returned value:
  ~~~~~~~~~~~~~~~
  0: OK
  -1: cound't allocate histogram array

  Original author: <simao.campos@comsat.com>
  ~~~~~~~~~~~~~~~~

  History:
  ~~~~~~~~
  19.Nov.97  v.1.0  Created.

  ---------------------------------------------------------------------------
*/
int init_ep_histogram (ep_histogram_state * state, long burst_len) {
  long i;

  /* Reset some variables */
  state->in_event = 0;
  state->first_time = 1;
  state->ep = 0;
  state->burst_len = burst_len;
  state->min_distance = 2147483647;
  state->max_distance = 0;
  state->event_distance = 0.0;
  state->event_distance_sq = 0.0;
  state->event_started = 0;
  state->event_no = 0;
  state->unexpected = 0;
  state->processed = 0;
  state->disturbed = 0;
  state->count = 0;
  state->last_event = 0;

  /* Allocate memory for histograms */
  state->hist = (long *) calloc (burst_len + 2, sizeof (long));
  if (state->hist == NULL)
    return (-1);

  /* Reset values in histogram */
  for (i = 0; i <= burst_len + 1; i++)
    state->hist[i] = 0;

  /* Return OK */
  return (0);
}

/* ..................... End of init_ep_histogram() ..................... */


/*
  ---------------------------------------------------------------------------
  void free_ep_histogram (ep_histogram_state *state);
  ~~~~~~~~~~~~~~~~~~~~~~

  Deallocate memory for EP histogram buffer.

  Parameters:
  ~~~~~~~~~~~
  state ..... pointer to state-variable for error pattern histogram

  Returned value: void
  ~~~~~~~~~~~~~~~

  Original author: <simao.campos@comsat.com>
  ~~~~~~~~~~~~~~~~

  History:
  ~~~~~~~~
  19.Nov.97  v.1.0  Created.

  ---------------------------------------------------------------------------
*/
void free_ep_histogram (ep_histogram_state * state) {
  free (state->hist);
}

/* ..................... End of free_ep_histogram() ..................... */


/*
  ---------------------------------------------------------------------------
  long compute_ep_histogram (short *pattern, long items, int ep_type,
  ~~~~~~~~~~~~~~~~~~~~~~~~~  long *ep_hist, long burst_len,
			     long *processed, long *disturbed, int reset);

  Function that computes the short term histogram for an input error
  pattern. This function performs complete initialization at the first
  time it is called, and at any later time if parameter reset is
  non-zero. The function performs accumulative calculations since stat
  or since the last reset. If the number of softbits in the pattern is
  zero, the function assumes that this is the last step of a series of
  measurements and flushes any necessary updates, followed by a
  partial reset of some of the static variables.

  Parameters:
  ~~~~~~~~~~~
  pattern ..... headerless G.192 bit error/frame erasure pattern array.
  items ....... Number of softbits/erasure flags in pattern. When 'items'
                is null, the function assumes that this is the last
                computation of a series (like when the EOF is found),
                and the update of the counters is followed by a
                partial reset of the state variables.
  ep_type ..... Error pattern type (BER or FER).
  state ....... State variable for the error pattern histogram.
                This function will allocate memory for the temporary
                error-pattern buffer, and reset a number of variable
                in the first time the function is invoked. In the
                state variable, hist[0] will contain the number of
                undisturbed bits, and hist[burst_len] will contain the
                number of bursts longer than 'burst_len' bits/frames.
  reset ....... Flag to force the initialization of the static variables
                as well as the associated counters. A reset is always
                performed the first time that the function is invoked.

  Returned value:
  ~~~~~~~~~~~~~~~
  Returns the number of samples in the pattern which had an unexpected
  value. Unexpected values would be values other than 0x6b2? (?=0..F)
  for FER patterns, or 0x007F/0x0081 for BER patterns.

  Original author: <simao.campos@comsat.com>
  ~~~~~~~~~~~~~~~~

  History:
  ~~~~~~~~
  10.Oct.97  v1.0  Created.
  19.Nov.97  v1.1  Changed to use a state variable rather than local
                   static variables. Necessary for processing multiple
                   EP at the same time. <simao>

  ---------------------------------------------------------------------------
*/
long compute_ep_histogram (short *pattern, long items, int ep_type, ep_histogram_state * s, int reset) {
  long i;
  long this_event;
  float delta;

  /* Initial reset */
  if (reset || s->first_time || items == 0) {
    /* Free temporary storage if previously allocated */
    if (s->ep)
      free (s->ep);

    /* Allocate memory for temporary buffer */
    if (s->ep == NULL && items != 0)
      if ((s->ep = (short *) calloc (items, sizeof (short))) == NULL)
        HARAKIRI ("Can't allocate memory for counter. Aborted.\n", 6);

    /* Flush possible error counts from last block, update counters */
    if (s->in_event) {
      s->hist[s->count <= s->burst_len ? s->count : s->burst_len + 1]++;
      s->disturbed += s->count;
      s->count = 0;
      s->in_event = 0;

      s->event_no++;
      s->last_event = s->event_started;
    }

    /* Reset error event counter array */
    if (reset || s->first_time) {
      for (i = 0; i <= s->burst_len + 1; i++)
        s->hist[i] = 0;
      s->processed = s->disturbed = 0;
    }

    /* Set variables to a known state */
    s->count = s->in_event = s->first_time = 0;
  }

  /* Stop if no items to be processed */
  if (items == 0)
    return (0);

  /* Update counter */
  s->processed += items;

  /* Convert EP to hardbit notation */
  s->unexpected += soft2hard (pattern, s->ep, items, ep_type);

  /* Search for errors/erasures */
  for (i = 0; i < items; i++) {
    if (s->ep[i]) {
      s->count++;
      if (!s->in_event) {
        s->event_started = s->processed - items + i;
        delta = s->event_started - s->last_event;
        if (delta > s->max_distance)
          s->max_distance = delta;
        if (delta < s->min_distance)
          s->min_distance = delta;
        s->event_distance += delta;
        s->event_distance_sq += delta * delta;
#ifdef DEBUG
        fprintf (stderr, "Event at bit/frame: %ld\n", s->event_started);
#endif
      }
      s->in_event = 1;
    } else {
      /* No longer in an error/erasure event. Flush the info to the EP counter array, update disturb.counter and reset variables/flags */
      if (s->in_event) {
        this_event = (s->count <= s->burst_len) ? s->count : s->burst_len + 1;
        s->hist[this_event]++;
        s->disturbed += s->count;
        s->count = 0;
        s->in_event = 0;

        s->event_no++;
        s->last_event = s->event_started;
      }
    }
  }

  /* Set number of undisturbed bits/frames */
  s->hist[0] = s->processed - s->disturbed;

  /* Return number of unexpected values */
  return (s->unexpected);
}

/* ...................... End of compute_ep_histogram() ................... */


/*
  file .... file name
  format .. error pattern format (g192, byte, compact
  start ... first item to process (bit or frame, depending on error
            pattern type)
  limit ... max number of items to process
*/
long get_max_items (char *file, char format, long start, long limit) {
  struct stat fileinfo;
  long bytes, max_items;

  stat (file, &fileinfo);
  switch (format) {
  case g192:
    bytes = fileinfo.st_size - start * 2;
    break;
  case byte:
    bytes = fileinfo.st_size - start;
    break;
  case compact:
    bytes = fileinfo.st_size - start / 8;
    break;
  }


  switch (format) {
  case g192:
    max_items = bytes / 2;
    break;
  case byte:
    max_items = bytes;
    break;
  case compact:
    max_items = bytes * 8;
    break;
  }
  if (limit > 0 && max_items > limit)
    max_items = limit;

  return (max_items);
}

/* ......................... End of get_max_items() ....................... */
