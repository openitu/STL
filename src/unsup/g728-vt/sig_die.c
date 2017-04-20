#include "stdio.h"
#include "signal.h"

#ifndef SIGIOT
#define SIGIOT SIGABRT
#endif

void sig_die (register char *s, int kill) {
/* print error message, then clear buffers */
  fflush (stderr);
  fprintf (stderr, "%s\n", s);
  fflush (stderr);

  if (kill) {
    /* now get a core */
    signal (SIGIOT, SIG_DFL);
    abort ();
  } else
    exit (1);
}
