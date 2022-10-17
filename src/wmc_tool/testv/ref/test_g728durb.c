            #include "g728.h"
/*AddedByWMC_Tool*/#include "wmc_auto.h"
/*AddedByWMC_Tool*/#define g728_vrdotp_ func_(g728_vrdotp,3)

            /*
             * (c) Copyright 1990-2007 AT&T Intellectual Property.
             * All rights reserved.
             *
             * NOTICE:
             * This software source code ("Software"), and all of its derivations,
             * is subject to the "ITU-T General Public License". It can be viewed
             * in the distribution disk, or in the ITU-T Recommendation G.191 on
             * "SOFTWARE TOOLS FOR SPEECH AND AUDIO CODING STANDARDS".
             *
             * The availability of this "Software" does not provide any license by
             * implication, estoppel, or otherwise under any patent rights owned or
             * controlled by AT&T Inc. or others covering any use of the Software
             * herein.
             */

            /*
             *  durbin's recursion - compute filter coefficients from autocorrelation coeff.
             *    routine is structured so the recursion can be started in one call and
             *    continued in the next call. Returns 1 if successful, 0 otherwise.
             */
            int g728_durbin (Float * r,     /* autocorrelation coefficients */
                             int i,         /* order of filter already done */
                             int order,     /* order of filter */
                             Float * a,     /* filter coefficients of this frame */
                             Float * rc0,   /* first reflection coefficient */
                             Float * re) { func_start_   /* residual energy */
              Float t, rc;
              int j, k;

              if_ (i == 0)
$("*M[]")       *re = r[0];
$("Gt")       for_ (; i < order &&__ *re > (Float) 0.; i++) {
$("GtM[]+[]")   t = r[i + 1] + g728_vrdotp_ (a, &r[1], i);
$("[]MM-/")     a[i] = rc = -t / *re;       /* reflection coeff. */
                if_ (i == 0)
$("*M")           *rc0 = rc;                /* save rc[0] for postfilter */
$("+=*")        *re += rc * t;
$("M-")         for_ (j = 0, k = i - 1; j <= k; j++, k--) {
$("DM[]+*[]")     t = a[j] + rc * a[k];
$("[]+=*[]")      a[k] += rc * a[j];
$("[]M")          a[j] = t;
                }
              }
$("*Gt")      return_ *re > (Float) 0. ?_ 1 :_ 0;      /* test if ill-conditioned */
            }
/*AddedByWMC_Tool*/ int PROM_Size_Func( test_g728durb ) { return 37; }
