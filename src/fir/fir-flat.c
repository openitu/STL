/*                                                         v2.2 16.MAY.97 18H00
  =============================================================================

                          U    U   GGG    SSSS  TTTTT
                          U    U  G       S       T
                          U    U  G  GG   SSSS    T
                          U    U  G   G       S   T
                           UUU     GG     SSS     T

                   ========================================
                    ITU-T - USER'S GROUP ON SOFTWARE TOOLS
                   ========================================

       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
       CODING STANDARDS".
       =============================================================

MODULE:         FIRFLT, HIGH QUALITY FIR UP/DOWN-SAMPLING FILTER
                Sub-unit: Flat-weighting filters, LP and BP

ORIGINAL BY:
 	Chris Tate <c.n.tate@bnr.co.uk>
	John Barnes <G.J.P.Barnes@bnr.co.uk>
	Rudolf Hofmann <hf@pkinbg.uucp>
        Simao Ferraz de Campos Neto <simao@ctd.comsat.com>

DESCRIPTION:
        This file contains procedures for flat-weighting low-pass and
	high-pass filters, with rate factors of 2 and 3 for both up-
	and down-sampling.

FUNCTIONS:
  Global (have prototype in firflt.h)
         = hq_down_2_to_1_init() :  initialize down-sampling filter 2:1
         = hq_down_3_to_1_init() :  initialize down-sampling filter 3:1
         = hq_up_1_to_2_init()   :  initialize   up-sampling filter 1:2
         = hq_up_1_to_3_init()   :  initialize   up-sampling filter 1:3
         = shq_down_2_to_1_init() :  initialize down-sampling filter 2:1
         = shq_down_3_to_1_init() :  initialize down-sampling filter 3:1
         = shq_up_1_to_2_init()   :  initialize   up-sampling filter 1:2
         = shq_up_1_to_3_init()   :  initialize   up-sampling filter 1:3
         = linear_phase_pb_2_to_1_init()
         = linear_phase_pb_1_to_2_init()
         = linear_phase_pb_1_to_1_init()

  Local (should be used only here -- prototypes only in this file)
         = fill_lp_2_to_1(...)   : filling filter coefficients into
                                   array for flat low pass, factor 2:1
         = fill_lp_3_to_1(...)   : idem, for flat low pass, factor 3:1
         = fill_flat_band_pass(...): idem, for linear-phase band-pass



HISTORY:
    16.Dec.91 v0.1 First beta-version <hf@pkinbg.uucp>
    28.Feb.92 v1.0 Release of 1st version to UGST <hf@pkinbg.uucp>
    20.Apr.94 v2.0 Added new filtering routines: modified IRS at 16kHz and
                   48kHz, Delta-SM, Linear-phase band-pass.
                   <simao@cpqd.ansp.br>
    30.Sep.94 v2.1 Updated to accomodate changes in the name of the name and
                   slitting of module in several files, for ease of expansion.
    ** THIS FILE WAS SPLIT FROM THE ORIGINAL .C FILE **
    16.May.97 v2.2 Added function for HQ flat bandpass keeping the sampling
                   rate (1:1 factor)
    05.April.2012 v2.3 - Added filters: SHQ2 and SHQ3 (France Telecom)
  =============================================================================
*/


/*
 * ......... INCLUDES .........
 */
#include <stdio.h>
#ifndef VMS
#include <stdlib.h>             /* General utility definitions */
#endif

#include "firflt.h"             /* Global definitions for FIR-FIR filter */


/*
 * ......... Local function prototypes .........
 */

void fill_lp_2_to_1 ARGS ((float *h0[], long *lenh0));
void fill_lp_3_to_1 ARGS ((float **h0, long *lenh0));
void fill_slp_2_to_1 ARGS ((float *h0[], long *lenh0));
void fill_slp_3_to_1 ARGS ((float **h0, long *lenh0));
void fill_flat_band_pass ARGS ((float **h0, long *lenh0));


/*
 * ..... Private function prototypes defined in other sub-unit .....
 */
extern SCD_FIR *fir_initialization ARGS ((long lenh0, float h0[], double gain, long idwnup, int hswitch));


/*
 * ...................... BEGIN OF FUNCTIONS .........................
 */


/*
  ============================================================================

        SCD_FIR *hq_down_2_to_1_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for high quality FIR downsampling filter
        by factor of 2.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
SCD_FIR *hq_down_2_to_1_init () {
  float *h0;                    /* pointer to array with FIR coeff. */
  long lenh0;                   /* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_lp_2_to_1 (&h0, &lenh0);

  return fir_initialization (   /* Returns: pointer to SCD_FIR-struct */
                              lenh0,    /* In: number of FIR-coefficients */
                              h0,       /* In: pointer to array with FIR-cof. */
                              1.0,      /* In: gain factor for FIR-coeffic. */
                              2l,       /* In: Down-sampling factor */
                              'D'       /* In: switch to down-sampling kernel */
    );
}

/* ..................... End of hq_down_2_to_1_init() ..................... */



/*
  ============================================================================

        SCD_FIR *hq_up_1_to_2_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for high quality upsampling FIR filter
        by factor of 2.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
SCD_FIR *hq_up_1_to_2_init () {
  float *h0;                    /* pointer to array with FIR coeff. */
  long lenh0;                   /* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_lp_2_to_1 (&h0, &lenh0);

  return fir_initialization (   /* Returns: pointer to SCD_FIR-struct */
                              lenh0,    /* In: number of FIR-coefficients */
                              h0,       /* In: pointer to array with FIR-cof. */
                              2.0,      /* In: gain factor for FIR-coeffic. */
                              2l,       /* In: Up-sampling factor */
                              'U'       /* In: Switch to upsampling procedure */
    );
}

/* ..................... End of hq_up_1_to_2_init() ..................... */



/*
  ============================================================================

        SCD_FIR *hq_down_3_to_1_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for high quality FIR downsampling filter
        by factor of 3.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
SCD_FIR *hq_down_3_to_1_init () {
  float *h0;                    /* pointer to array with FIR coeff. */
  long lenh0;                   /* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_lp_3_to_1 (&h0, &lenh0);

  return fir_initialization (   /* Returns: pointer to SCD_FIR-struct */
                              lenh0,    /* In: number of FIR-coefficients */
                              h0,       /* In: pointer to array with FIR-cof. */
                              1.0,      /* In: gain factor for FIR-coeffic. */
                              3l,       /* In: Down-sampling factor */
                              'D'       /* In: switch to down-sampling proc. */
    );
}

/* .................... End of hq_down_3_to_1_init() ..................... */



/*
  ============================================================================

        SCD_FIR *hq_up_1_to_3_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for high quality FIR upsampling filter
        by factor of 3.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
SCD_FIR *hq_up_1_to_3_init () {
  float *h0;                    /* pointer to array with FIR coeff. */
  long lenh0;                   /* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_lp_3_to_1 (&h0, &lenh0);

  return fir_initialization (   /* Returns: pointer to SCD_FIR-struct */
                              lenh0,    /* In: number of FIR-coefficients */
                              h0,       /* In: pointer to array with FIR-cof. */
                              3.0,      /* In: gain factor for FIR-coeffic. */
                              3l,       /* In: Up-sampling factor */
                              'U'       /* In: switch to upsampling procedure */
    );
}

/* ...................... End of hq_up_1_to_3_init() ...................... */


/*
  ============================================================================

        void fill_lp_2_to_1 (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize pointer to array with of FIR coefficients of a low
        pass filter for up/down sampling filter with factor 2:1 or 1:2
        and 24-bit representation.

        Parameters:
        ~~~~~~~~~~~
        h0:    (Out) pointer to array with FIR coefficients
        lenh0: (Out) pointer to number of coefficients


        Return value:
        ~~~~~~~~~~~~~
        None.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
#define f24 (float)0x00800000
#define lenh02 118

void fill_lp_2_to_1 (float *h0[], long *lenh0) {
  static float h02[lenh02] = {
    1584. / f24, 805. / f24, -4192. / f24, -8985. / f24,
    -5987. / f24, 2583. / f24, 4657. / f24, -3035. / f24,
    -7004. / f24, 1542. / f24, 8969. / f24, 567. / f24,
    -10924. / f24, -3757. / f24, 12320. / f24, 7951. / f24,
    -12793. / f24, -13048. / f24, 11923. / f24, 18793. / f24,
    -9331. / f24, -24802. / f24, 4694. / f24, 30570. / f24,
    2233. / f24, -35439. / f24, -11526. / f24, 38680. / f24,
    23114. / f24, -39474. / f24, -36701. / f24, 36999. / f24,
    51797. / f24, -30419. / f24, -67658. / f24, 18962. / f24,
    83318. / f24, -1927. / f24, -97566. / f24, -21284. / f24,
    108971. / f24, 51215. / f24, -115837. / f24, -88430. / f24,
    116130. / f24, 133716. / f24, -107253. / f24, -188497. / f24,
    85497. / f24, 255795. / f24, -44643. / f24, -342699. / f24,
    -28185. / f24, 468096. / f24, 167799. / f24, -696809. / f24,
    -519818. / f24, 1446093. / f24, 3562497. / f24, 3562497. / f24,
    1446093. / f24, -519818. / f24, -696809. / f24, 167799. / f24,
    468096. / f24, -28185. / f24, -342699. / f24, -44643. / f24,
    255795. / f24, 85497. / f24, -188497. / f24, -107253. / f24,
    133716. / f24, 116130. / f24, -88430. / f24, -115837. / f24,
    51215. / f24, 108971. / f24, -21284. / f24, -97566. / f24,
    -1927. / f24, 83318. / f24, 18962. / f24, -67658. / f24,
    -30419. / f24, 51797. / f24, 36999. / f24, -36701. / f24,
    -39474. / f24, 23114. / f24, 38680. / f24, -11526. / f24,
    -35439. / f24, 2233. / f24, 30570. / f24, 4694. / f24,
    -24802. / f24, -9331. / f24, 18793. / f24, 11923. / f24,
    -13048. / f24, -12793. / f24, 7951. / f24, 12320. / f24,
    -3757. / f24, -10924. / f24, 567. / f24, 8969. / f24,
    1542. / f24, -7004. / f24, -3035. / f24, 4657. / f24,
    2583. / f24, -5987. / f24, -8985. / f24, -4192. / f24,
    805. / f24, 1584. / f24
  };


  *lenh0 = lenh02;              /* store 'number of coefficients' */
  *h0 = h02;                    /* store pointer to h02[]-array */
}

#undef f24
#undef lenh02
/* ....................... End of fill_lp_2_to_1() ....................... */



/*
  ============================================================================

        void fill_lp_3_to_1 (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize pointer to array with FIR coefficients for up/down
        sampling low-pass filter with factor 3:1 or 1:3 and 24 bit
        representation.

        Parameters:
        ~~~~~~~~~~~
        h0:    (Out) pointer to array with FIR coefficients
        lenh0: (Out) pointer to number of coefficients


        Return value:
        ~~~~~~~~~~~~~
        None.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
#define f24 (float)0x00800000
#define lenh03 168

void fill_lp_3_to_1 (float **h0, long *lenh0) {
  static float h03[lenh03] = {
    877. / f24, 3745. / f24, 6479. / f24, 8447. / f24, 7307. / f24,
    3099. / f24, -2223. / f24, -5302. / f24, -3991. / f24, 766. / f24,
    5168. / f24, 5362. / f24, 731. / f24, -5140. / f24, -7094. / f24,
    -2830. / f24, 4611. / f24, 8861. / f24, 5584. / f24, -3260. / f24,
    -10326. / f24, -8887. / f24, 888. / f24, 11145. / f24, 12532. / f24,
    2617. / f24, -10961. / f24, -16207. / f24, -7257. / f24, 9442. / f24,
    19522. / f24, 12931. / f24, -6288. / f24, -22007. / f24, -19398. / f24,
    1280. / f24, 23148. / f24, 26290. / f24, 5704. / f24, -22403. / f24,
    -33102. / f24, -14655. / f24, 19237. / f24, 39196. / f24, 25404. / f24,
    -13162. / f24, -43824. / f24, -37610. / f24, 3766. / f24, 46146. / f24,
    50752. / f24, 9264. / f24, -45243. / f24, -64134. / f24, -26137. / f24,
    40124. / f24, 76873. / f24, 46957. / f24, -29705. / f24, -87899. / f24,
    -71773. / f24, 12729. / f24, 95920. / f24, 100661. / f24, 12412. / f24,
    -99329. / f24, -133927. / f24, -48113. / f24, 95967. / f24, 172563. / f24,
    98654. / f24, -82409. / f24, -219347. / f24, -173208. / f24, 51783. / f24,
    282060. / f24, 295863. / f24, 14257. / f24, -387590. / f24, -556360. / f24,
    -195882. / f24, 696028. / f24, 1767624. / f24, 2494432. / f24, 2494432. / f24,
    1767624. / f24, 696028. / f24, -195882. / f24, -556360. / f24, -387590. / f24,
    14257. / f24, 295863. / f24, 282060. / f24, 51783. / f24, -173208. / f24,
    -219347. / f24, -82409. / f24, 98654. / f24, 172563. / f24, 95967. / f24,
    -48113. / f24, -133927. / f24, -99329. / f24, 12412. / f24, 100661. / f24,
    95920. / f24, 12729. / f24, -71773. / f24, -87899. / f24, -29705. / f24,
    46957. / f24, 76873. / f24, 40124. / f24, -26137. / f24, -64134. / f24,
    -45243. / f24, 9264. / f24, 50752. / f24, 46146. / f24, 3766. / f24,
    -37610. / f24, -43824. / f24, -13162. / f24, 25404. / f24, 39196. / f24,
    19237. / f24, -14655. / f24, -33102. / f24, -22403. / f24, 5704. / f24,
    26290. / f24, 23148. / f24, 1280. / f24, -19398. / f24, -22007. / f24,
    -6288. / f24, 12931. / f24, 19522. / f24, 9442. / f24, -7257. / f24,
    -16207. / f24, -10961. / f24, 2617. / f24, 12532. / f24, 11145. / f24,
    888. / f24, -8887. / f24, -10326. / f24, -3260. / f24, 5584. / f24,
    8861. / f24, 4611. / f24, -2830. / f24, -7094. / f24, -5140. / f24,
    731. / f24, 5362. / f24, 5168. / f24, 766. / f24, -3991. / f24,
    -5302. / f24, -2223. / f24, 3099. / f24, 7307. / f24, 8447. / f24,
    6479. / f24, 3745. / f24, 877. / f24
  };


  *lenh0 = lenh03;              /* store 'number of coefficients' */
  *h0 = h03;                    /* store pointer to h02[]-array */
}

#undef lenh03
#undef f24
/* ........................ End of fill_lp_3_to_1() ........................ */


/* ************************************************************************ */
/* ****** The functions that follow were added after the ITU-T STL92 ****** */
/* ************************************************************************ */

/*
  ============================================================================

        void fill_flat_band_pass (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~
        These are the filter coefficients describing the
        impulse response of the "flat" filter used in the
        "Talker Dependency Experiment" of the Selection Tests of the
        ITU-TSS 8kbit/s speech coder. A summary of the filter
        characteristics is as follows:

        - linear phase
        - 168 taps
        - band-pass filter with cut-off of very low frequency (a la
          G.712) for the telephony range [NOTE: can be used for other
          sampling frequencies, but the lower cut-off will change.
          Initially designed for 16kHz sampling rate!]
        - coefficients quantised and scaled so +/-1 gives +/-32768
          (ie a factor of 32768)
        - |Pass-band ripple| < 0.2dB (over approx. 143Hz -> 3407Hz)
        - 3dB points at approx. 98Hz and 3462Hz
        - stopband attenuation > 50dB (over approx. 0Hz -> 8Hz)
          and > 50dB (over approx. 3600Hz -> 8000Hz)


        Parameters:
        ~~~~~~~~~~~
        h0:    (Out) pointer to array with FIR coefficients
        lenh0: (Out) pointer to number of coefficients

        Return value:
        ~~~~~~~~~~~~~
        None.

        Author:
        ~~~~~~~
        Filter coefficients:
          John Barnes <G.J.P.Barnes@bnr.co.uk>
          Chris Tate <c.n.tate@bnr.co.uk>

        Routine:
          Simao Ferraz de Campos Neto
          DDS/Pr11                      Tel: +55-192-39-1396
          CPqD/Telebras                 Fax: +55-192-53-4754
          13088-061 Campinas SP Brazil  E-mail: <simao@cpqd.ansp.br>

        History:
        ~~~~~~~~
        10.Apr.94 v1.0 Release of 1st version <simao@cpqd.ansp.br>

 ============================================================================
*/
#define f16 32768.
#define FLAT_BAND_PASS_LEN 168

void fill_flat_band_pass (float **h0, long *lenh0) {
  static float flat_coef[FLAT_BAND_PASS_LEN] = {
    -43. / f16, -168. / f16, -156. / f16, -90. / f16, -1. / f16,
    -7. / f16, -78. / f16, -107. / f16, -55. / f16, -13. / f16,
    -55. / f16, -120. / f16, -108. / f16, -39. / f16, -31. / f16,
    -108. / f16, -154. / f16, -97. / f16, -29. / f16, -70. / f16,
    -168. / f16, -170. / f16, -72. / f16, -36. / f16, -134. / f16,
    -221. / f16, -159. / f16, -45. / f16, -74. / f16, -215. / f16,
    -251. / f16, -120. / f16, -34. / f16, -148. / f16, -296. / f16,
    -242. / f16, -67. / f16, -61. / f16, -255. / f16, -349. / f16,
    -186. / f16, -24. / f16, -140. / f16, -372. / f16, -349. / f16,
    -97. / f16, -24. / f16, -274. / f16, -465. / f16, -278. / f16,
    -1. / f16, -96. / f16, -443. / f16, -491. / f16, -139. / f16,
    54. / f16, -257. / f16, -605. / f16, -414. / f16, 43. / f16,
    17. / f16, -503. / f16, -699. / f16, -207. / f16, 214. / f16,
    -164. / f16, -803. / f16, -652. / f16, 137. / f16, 298. / f16,
    -547. / f16, -1107. / f16, -360. / f16, 636. / f16, 175. / f16,
    -1265. / f16, -1354. / f16, 468. / f16, 1485. / f16, -570. / f16,
    -3275. / f16, -1493. / f16, 5831. / f16, 12895. / f16, 12895. / f16,
    5831. / f16, -1493. / f16, -3275. / f16, -570. / f16, 1485. / f16,
    468. / f16, -1354. / f16, -1265. / f16, 175. / f16, 636. / f16,
    -360. / f16, -1107. / f16, -547. / f16, 298. / f16, 137. / f16,
    -652. / f16, -803. / f16, -164. / f16, 214. / f16, -207. / f16,
    -699. / f16, -503. / f16, 17. / f16, 43. / f16, -414. / f16,
    -605. / f16, -257. / f16, 54. / f16, -139. / f16, -491. / f16,
    -443. / f16, -96. / f16, -1. / f16, -278. / f16, -465. / f16,
    -274. / f16, -24. / f16, -97. / f16, -349. / f16, -372. / f16,
    -140. / f16, -24. / f16, -186. / f16, -349. / f16, -255. / f16,
    -61. / f16, -67. / f16, -242. / f16, -296. / f16, -148. / f16,
    -34. / f16, -120. / f16, -251. / f16, -215. / f16, -74. / f16,
    -45. / f16, -159. / f16, -221. / f16, -134. / f16, -36. / f16,
    -72. / f16, -170. / f16, -168. / f16, -70. / f16, -29. / f16,
    -97. / f16, -154. / f16, -108. / f16, -31. / f16, -39. / f16,
    -108. / f16, -120. / f16, -55. / f16, -13. / f16, -55. / f16,
    -107. / f16, -78. / f16, -7. / f16, -1. / f16, -90. / f16,
    -156. / f16, -168. / f16, -43. / f16
  };

  *lenh0 = FLAT_BAND_PASS_LEN;  /* store 'number of coefficients' */
  *h0 = flat_coef;              /* store pointer to []-array */
}

#undef f16
#undef FLAT_BAND_PASS_LEN
/* .................... End of fill_flat_band_pass() .................... */


/*
  ============================================================================

        SCD_FIR *linear_phase_pb_2_to_1_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~
        Initialization routine for the linear-phase, FIR implementation of
        the band-pass filter characteristic for data sampled at 16 kHz and
        down-sampling of 2:1. The mask resembles that of ITU-T Rec.G.712.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <simao@cpqd.ansp.br>
        ~~~~~~~

        History:
        ~~~~~~~~
        20.Apr.94 v1.0 Release of 1st version <simao>

 ============================================================================
*/
SCD_FIR *linear_phase_pb_2_to_1_init () {
  float *h0;                    /* pointer to array with FIR coeff. */
  long lenh0;                   /* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_flat_band_pass (&h0, &lenh0);

  return fir_initialization (   /* Returns: pointer to SCD_FIR-struct */
                              lenh0,    /* In: number of FIR-coefficients */
                              h0,       /* In: pointer to array with FIR-cof. */
                              1.0,      /* In: gain factor for FIR-coeffic. */
                              2l,       /* In: Down-sampling factor */
                              'D'       /* In: switch to down-sampling proc. */
    );                          /* (works here as simple FIR-fil. */
}

/* ............... End of linear_phase_pb_2_to_1_init() ............... */


/*
  ============================================================================

        SCD_FIR *linear_phase_pb_1_to_2_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~
        Initialization routine for the linear-phase, FIR implementation of
        the band-pass filter characteristic for data sampled at 16 kHz and
        down-sampling of 2:1. The mask resembles that of ITU-T Rec.G.712.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <simao@cpqd.ansp.br>
        ~~~~~~~

        History:
        ~~~~~~~~
        20.Apr.94 v1.0 Release of 1st version <simao>

 ============================================================================
*/
SCD_FIR *linear_phase_pb_1_to_2_init () {
  float *h0;                    /* pointer to array with FIR coeff. */
  long lenh0;                   /* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_flat_band_pass (&h0, &lenh0);

  return fir_initialization (   /* Returns: pointer to SCD_FIR-struct */
                              lenh0,    /* In: number of FIR-coefficients */
                              h0,       /* In: pointer to array with FIR-cof. */
                              2.0,      /* In: gain factor for FIR-coeffic. */
                              2l,       /* In: Down-sampling factor */
                              'U'       /* In: switch to down-sampling proc. */
    );                          /* (works here as simple FIR-fil. */
}

/* ................ End of linear_phase_pb_1_to_2_init() ................ */

/* ************************************************************************ */
/* ****** The functions that follow were added after the ITU-T STL96 ****** */
/* ************************************************************************ */



/*
  ============================================================================

        SCD_FIR *linear_phase_pb_1_to_1_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~
        Initialization routine for the linear-phase, FIR implementation of
        the band-pass filter characteristic for data sampled at 16 kHz and
        no downsampling (factor is 1:1). The output signal, although at the
	16 kHz sampling rate, will have a 4 kHz *effective* bandwidth.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <simao.campos@comsat.com>
        ~~~~~~~

        History:
        ~~~~~~~~
        16.May.97 v1.0 Release of 1st version <simao>
        06.Oct.99 v2.0 Fixed wrong gain in function (was 2.0) <simao>

 ============================================================================
*/
SCD_FIR *linear_phase_pb_1_to_1_init () {
  float *h0;                    /* pointer to array with FIR coeff. */
  long lenh0;                   /* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_flat_band_pass (&h0, &lenh0);

  return fir_initialization (   /* Returns: pointer to SCD_FIR-struct */
                              lenh0,    /* In: number of FIR-coefficients */
                              h0,       /* In: pointer to array with FIR-cof. */
                              1.0,      /* In: gain factor for FIR-coeffic. */
                              1l,       /* In: Down-sampling factor */
                              'D'       /* In: switch to down-sampling proc. */
    );                          /* (works here as simple FIR-fil.) */
}

/* ................ End of linear_phase_pb_1_to_1_init() ................ */


/*****************************************************************************/
/*****************************************************************************/

/*
  ============================================================================

        SCD_FIR *shq_down_2_to_1_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for super high quality FIR downsampling filter
        by factor of 2.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <balazs.kovesi@orange.com>
        ~~~~~~~

        History:
        ~~~~~~~~
		 05.April.2012 v1.0 - Added filters: SHQ2 and SHQ3 (France Telecom)

 ============================================================================
*/
SCD_FIR *shq_down_2_to_1_init () {
  float *h0;                    /* pointer to array with FIR coeff. */
  long lenh0;                   /* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_slp_2_to_1 (&h0, &lenh0);

  return fir_initialization (   /* Returns: pointer to SCD_FIR-struct */
                              lenh0,    /* In: number of FIR-coefficients */
                              h0,       /* In: pointer to array with FIR-cof. */
                              1.0,      /* In: gain factor for FIR-coeffic. */
                              2l,       /* In: Down-sampling factor */
                              'D'       /* In: switch to down-sampling kernel */
    );
}

/* ..................... End of shq_down_2_to_1_init() ..................... */



/*
  ============================================================================

        SCD_FIR *shq_up_1_to_2_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for super high quality upsampling FIR filter
        by factor of 2.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <balazs.kovesi@orange.com>
        ~~~~~~~

        History:
        ~~~~~~~~
		 05.April.2012 v1.0 - Added filters: SHQ2 and SHQ3 (France Telecom)

 ============================================================================
*/
SCD_FIR *shq_up_1_to_2_init () {
  float *h0;                    /* pointer to array with FIR coeff. */
  long lenh0;                   /* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_slp_2_to_1 (&h0, &lenh0);

  return fir_initialization (   /* Returns: pointer to SCD_FIR-struct */
                              lenh0,    /* In: number of FIR-coefficients */
                              h0,       /* In: pointer to array with FIR-cof. */
                              2.0,      /* In: gain factor for FIR-coeffic. */
                              2l,       /* In: Up-sampling factor */
                              'U'       /* In: Switch to upsampling procedure */
    );
}

/* ..................... End of shq_up_1_to_2_init() ..................... */



/*
  ============================================================================

        SCD_FIR *shq_down_3_to_1_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for super high quality FIR downsampling filter
        by factor of 3.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <balazs.kovesi@orange.com>
        ~~~~~~~

        History:
        ~~~~~~~~
		 05.April.2012 v1.0 - Added filters: SHQ2 and SHQ3 (France Telecom)

 ============================================================================
*/
SCD_FIR *shq_down_3_to_1_init () {
  float *h0;                    /* pointer to array with FIR coeff. */
  long lenh0;                   /* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_slp_3_to_1 (&h0, &lenh0);

  return fir_initialization (   /* Returns: pointer to SCD_FIR-struct */
                              lenh0,    /* In: number of FIR-coefficients */
                              h0,       /* In: pointer to array with FIR-cof. */
                              1.0,      /* In: gain factor for FIR-coeffic. */
                              3l,       /* In: Down-sampling factor */
                              'D'       /* In: switch to down-sampling proc. */
    );
}

/* .................... End of shq_down_3_to_1_init() ..................... */



/*
  ============================================================================

        SCD_FIR *shq_up_1_to_3_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for super high quality FIR upsampling filter
        by factor of 3.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <balazs.kovesi@orange.com>
        ~~~~~~~

        History:
        ~~~~~~~~
		 05.April.2012 v1.0 - Added filters: SHQ2 and SHQ3 (France Telecom)

 ============================================================================
*/
SCD_FIR *shq_up_1_to_3_init () {
  float *h0;                    /* pointer to array with FIR coeff. */
  long lenh0;                   /* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_slp_3_to_1 (&h0, &lenh0);

  return fir_initialization (   /* Returns: pointer to SCD_FIR-struct */
                              lenh0,    /* In: number of FIR-coefficients */
                              h0,       /* In: pointer to array with FIR-cof. */
                              3.0,      /* In: gain factor for FIR-coeffic. */
                              3l,       /* In: Up-sampling factor */
                              'U'       /* In: switch to upsampling procedure */
    );
}

/* ...................... End of shq_up_1_to_3_init() ...................... */


/*
  ============================================================================

        void fill_slp_2_to_1 (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize pointer to array with of FIR coefficients of a low
        pass filter for up/down sampling filter with factor 2:1 or 1:2
        and float representation.
		v1.0 filtdemo firpm 16000,3935,4000,0.3, 80, 728
		v1.1 filtdemo firpm 16000,3935,4000,0.3, 100, 872
		h2 = filtdemo('getfilt')
		for i = 1:length(h2) fprintf('(float)%20.12e, ', h2(i));
		if rem(i,5) == 0 fprintf('\n');	end end

        Parameters:
        ~~~~~~~~~~~
        h0:    (Out) pointer to array with FIR coefficients
        lenh0: (Out) pointer to number of coefficients


        Return value:
        ~~~~~~~~~~~~~
        None.

        Author: <balazs.kovesi@orange.com>
        ~~~~~~~

        History:
        ~~~~~~~~
        08.DEC.2011 v1.0 Release of 1st version <balazs.kovesi@orange.com>
        15.FEB.2012 v1.1 Release of 2nd version with more stopband attenuation <balazs.kovesi@orange.com>

 ============================================================================
*/
#define lensh02 873

void fill_slp_2_to_1 (h0, lenh0)
     float *h0[];
     long *lenh0;
{
  static float sh02[lensh02] = {
    (float) 3.796696928466e-006, (float) - 1.916895913880e-004, (float) - 9.559790653313e-004, (float) - 2.309090436241e-003, (float) - 3.355260288418e-003,
    (float) - 2.821616068751e-003, (float) - 6.494578851510e-004, (float) 1.368071261832e-003, (float) 1.370278722621e-003, (float) - 2.336521287917e-004,
    (float) - 1.185316896754e-003, (float) - 3.557893702940e-004, (float) 8.234667012568e-004, (float) 6.081914903075e-004, (float) - 4.869084931865e-004,
    (float) - 6.784989588347e-004, (float) 2.211378711751e-004, (float) 6.567967063227e-004, (float) - 2.508428842065e-005, (float) - 5.918859501058e-004,
    (float) - 1.133457948942e-004, (float) 5.103528659400e-004, (float) 2.077730139398e-004, (float) - 4.257030079639e-004, (float) - 2.693320767774e-004,
    (float) 3.450469073084e-004, (float) 3.071631118731e-004, (float) - 2.713518058883e-004, (float) - 3.278882894771e-004, (float) 2.057925864008e-004,
    (float) 3.366006455231e-004, (float) - 1.482361183421e-004, (float) - 3.367394950886e-004, (float) 9.845446547453e-005, (float) 3.312977192436e-004,
    (float) - 5.533287789728e-005, (float) - 3.217977487285e-004, (float) 1.843967368528e-005, (float) 3.099640470313e-004, (float) 1.328843895284e-005,
    (float) - 2.965903264243e-004, (float) - 4.040411770010e-005, (float) 2.826060999738e-004, (float) 6.386636089117e-005, (float) - 2.682059180462e-004,
    (float) - 8.400473971490e-005, (float) 2.539623487208e-004, (float) 1.015795101283e-004, (float) - 2.398340044811e-004, (float) - 1.167662237676e-004,
    (float) 2.262277295494e-004, (float) 1.302389953023e-004, (float) - 2.128963846610e-004, (float) - 1.420059883336e-004, (float) 2.001325818624e-004,
    (float) 1.525152442899e-004, (float) - 1.878370907468e-004, (float) - 1.619282978772e-004, (float) 1.760669168660e-004, (float) 1.705747652949e-004,
    (float) - 1.645342831667e-004, (float) - 1.782437567680e-004, (float) 1.536329356026e-004, (float) 1.854489350961e-004, (float) - 1.431090602159e-004,
    (float) - 1.923098357663e-004, (float) 1.327632434198e-004, (float) 1.988019142637e-004, (float) - 1.224823030873e-004, (float) - 2.047505646951e-004,
    (float) 1.125436526441e-004, (float) 2.105136263884e-004, (float) - 1.026783876418e-004, (float) - 2.159988725649e-004, (float) 9.291206030779e-005,
    (float) 2.212630220095e-004, (float) - 8.322092258564e-005, (float) - 2.263811595801e-004, (float) 7.350850871907e-005, (float) 2.313695981464e-004,
    (float) - 6.362969041322e-005, (float) - 2.360269018314e-004, (float) 5.380295174011e-005, (float) 2.405543319009e-004, (float) - 4.394022835345e-005,
    (float) - 2.450580921435e-004, (float) 3.380378919976e-005, (float) 2.493326534241e-004, (float) - 2.346096839263e-005, (float) - 2.533379001119e-004,
    (float) 1.298908745154e-005, (float) 2.571630808553e-004, (float) - 2.297001211178e-006, (float) - 2.607451266109e-004, (float) - 8.605011575155e-006,
    (float) 2.640373512578e-004, (float) 1.959506007225e-005, (float) - 2.672713250910e-004, (float) - 3.098103063675e-005, (float) 2.701901534524e-004,
    (float) 4.267470287693e-005, (float) - 2.727139254030e-004, (float) - 5.452878257789e-005, (float) 2.749705769146e-004, (float) 6.658908505887e-005,
    (float) - 2.770661665078e-004, (float) - 7.913713870114e-005, (float) 2.786327455942e-004, (float) 9.185585608604e-005, (float) - 2.798865730517e-004,
    (float) - 1.049053516406e-004, (float) 2.807049278836e-004, (float) 1.182425329275e-004, (float) - 2.810498626639e-004, (float) - 1.318263284371e-004,
    (float) 2.808967923003e-004, (float) 1.455506215115e-004, (float) - 2.804597862803e-004, (float) - 1.597707317081e-004, (float) 2.793085842202e-004,
    (float) 1.741553960777e-004, (float) - 2.775743726755e-004, (float) - 1.887189126391e-004, (float) 2.752088365441e-004, (float) 2.033292028967e-004,
    (float) - 2.723750410219e-004, (float) - 2.181185345008e-004, (float) 2.690195363322e-004, (float) 2.331377491223e-004, (float) - 2.650434716994e-004,
    (float) - 2.483814212302e-004, (float) 2.602688344952e-004, (float) 2.635383838792e-004, (float) - 2.550401641814e-004, (float) - 2.789293863887e-004,
    (float) 2.491244096402e-004, (float) 2.945239709175e-004, (float) - 2.422677063636e-004, (float) - 3.097933373634e-004, (float) 2.353216838587e-004,
    (float) 3.261618119620e-004, (float) - 2.259436476779e-004, (float) - 3.401042900591e-004, (float) 2.190501787592e-004, (float) 3.582196439592e-004,
    (float) - 2.060273208877e-004, (float) - 3.696711810736e-004, (float) 1.999494168871e-004, (float) 3.895781288432e-004, (float) - 1.842093035737e-004,
    (float) - 4.009205689012e-004, (float) 1.747765776478e-004, (float) 4.170995964833e-004, (float) - 1.621552954359e-004, (float) - 4.335350916263e-004,
    (float) 1.456088038973e-004, (float) 4.447247309663e-004, (float) - 1.338587279643e-004, (float) - 4.603990618508e-004, (float) 1.185710423385e-004,
    (float) 4.756987576397e-004, (float) - 1.000048491894e-004, (float) - 4.866041612047e-004, (float) 8.476411455038e-005, (float) 4.998780903868e-004,
    (float) - 6.827228464703e-005, (float) - 5.146667747733e-004, (float) 4.753115459198e-005, (float) 5.255987039514e-004, (float) - 2.779134779343e-005,
    (float) - 5.355054669473e-004, (float) 9.674319498129e-006, (float) 5.478734582255e-004, (float) 1.129727308307e-005, (float) - 5.595560306151e-004,
    (float) - 3.493017868056e-005, (float) 5.677956913097e-004, (float) 5.713859403233e-005, (float) - 5.757813638276e-004, (float) - 7.867758251969e-005,
    (float) 5.852799390936e-004, (float) 1.025052366029e-004, (float) - 5.941431998669e-004, (float) - 1.287437605610e-004, (float) 6.000426995209e-004,
    (float) 1.543883746225e-004, (float) - 6.047947232235e-004, (float) - 1.791348851279e-004, (float) 6.103250533849e-004, (float) 2.050193734239e-004,
    (float) - 6.160453602904e-004, (float) - 2.334612077119e-004, (float) 6.194421365491e-004, (float) 2.625902273169e-004, (float) - 6.205808968069e-004,
    (float) - 2.909797933304e-004, (float) 6.209921559119e-004, (float) 3.189781263679e-004, (float) - 6.218075972611e-004, (float) - 3.484125142872e-004,
    (float) 6.217754987501e-004, (float) 3.794107287795e-004, (float) - 6.197373802874e-004, (float) - 4.110126983210e-004, (float) 6.154740944566e-004,
    (float) 4.419629007982e-004, (float) - 6.102659557898e-004, (float) - 4.726777081461e-004, (float) 6.046314459814e-004, (float) 5.039254713546e-004,
    (float) - 5.984097187292e-004, (float) - 5.364779620831e-004, (float) 5.904522258905e-004, (float) 5.697973331218e-004, (float) - 5.802595330793e-004,
    (float) - 6.029679388270e-004, (float) 5.681353065596e-004, (float) 6.352880253925e-004, (float) - 5.552572270111e-004, (float) - 6.675564463085e-004,
    (float) 5.416686835400e-004, (float) 7.004526959840e-004, (float) - 5.267382801903e-004, (float) - 7.340039635307e-004, (float) 5.098171451209e-004,
    (float) 7.676007675934e-004, (float) - 4.909399565337e-004, (float) - 8.009185568264e-004, (float) 4.701439201458e-004, (float) 8.334617833183e-004,
    (float) - 4.480343741175e-004, (float) - 8.653751202061e-004, (float) 4.250690490151e-004, (float) 8.973420149762e-004, (float) - 4.009277182108e-004,
    (float) - 9.297436930304e-004, (float) 3.747939959452e-004, (float) 9.620204713426e-004, (float) - 3.465711833398e-004, (float) - 9.937591861314e-004,
    (float) 3.163873335071e-004, (float) 1.024733751383e-003, (float) - 2.843503685027e-004, (float) - 1.054638636894e-003, (float) 2.509109323082e-004,
    (float) 1.083612570606e-003, (float) - 2.163900938188e-004, (float) - 1.112203502198e-003, (float) 1.803999502271e-004, (float) 1.140444841288e-003,
    (float) - 1.425881460745e-004, (float) - 1.168069638194e-003, (float) 1.029125055130e-004, (float) 1.195016515742e-003, (float) - 6.105502150698e-005,
    (float) - 1.220730499698e-003, (float) 1.722309885781e-005, (float) 1.244802222599e-003, (float) 2.793170702027e-005, (float) - 1.267597215030e-003,
    (float) - 7.433354174569e-005, (float) 1.289443015758e-003, (float) 1.223071384268e-004, (float) - 1.310173684327e-003, (float) - 1.717961954652e-004,
    (float) 1.329974954064e-003, (float) 2.232129919651e-004, (float) - 1.348538305787e-003, (float) - 2.767754823491e-004, (float) 1.365216750693e-003,
    (float) 3.319638947909e-004, (float) - 1.380095678916e-003, (float) - 3.886579246534e-004, (float) 1.393229786168e-003, (float) 4.469245863820e-004,
    (float) - 1.404378077863e-003, (float) - 5.064357635350e-004, (float) 1.413756425541e-003, (float) 5.672145160932e-004, (float) - 1.421549050340e-003,
    (float) - 6.296742832444e-004, (float) 1.427153322508e-003, (float) 6.930389693885e-004, (float) - 1.431723738610e-003, (float) - 7.590842956586e-004,
    (float) 1.433006680474e-003, (float) 8.254983244591e-004, (float) - 1.433298843968e-003, (float) - 8.949540559698e-004, (float) 1.429349304332e-003,
    (float) 9.640000205246e-004, (float) - 1.424329953401e-003, (float) - 1.035464848937e-003, (float) 1.415601499601e-003, (float) 1.106948972842e-003,
    (float) - 1.405087088396e-003, (float) - 1.179673831986e-003, (float) 1.392478926372e-003, (float) 1.254375291373e-003, (float) - 1.376198638858e-003,
    (float) - 1.329115340603e-003, (float) 1.358185632312e-003, (float) 1.405674275449e-003, (float) - 1.337154931483e-003, (float) - 1.483771621272e-003,
    (float) 1.312269387543e-003, (float) 1.561900282048e-003, (float) - 1.285301279727e-003, (float) - 1.642079777660e-003, (float) 1.254114183544e-003,
    (float) 1.722681043912e-003, (float) - 1.219211242008e-003, (float) - 1.803063205736e-003, (float) 1.181949655991e-003, (float) 1.884828365040e-003,
    (float) - 1.140994785147e-003, (float) - 1.967442657059e-003, (float) 1.095925463900e-003, (float) 2.049903843463e-003, (float) - 1.047877974227e-003,
    (float) - 2.133252204410e-003, (float) 9.962070742267e-004, (float) 2.217664374578e-003, (float) - 9.399028586473e-004, (float) - 2.301879563126e-003,
    (float) 8.799533412324e-004, (float) 2.386557312794e-003, (float) - 8.160716710263e-004, (float) - 2.472094691981e-003, (float) 7.471803656806e-004,
    (float) 2.557416285858e-003, (float) - 6.736494366209e-004, (float) - 2.642112224434e-003, (float) 5.963947377435e-004, (float) 2.727379304929e-003,
    (float) - 5.143438896094e-004, (float) - 2.812899297523e-003, (float) 4.268830572252e-004, (float) 2.897715073078e-003, (float) - 3.346488919796e-004,
    (float) - 2.981956373303e-003, (float) 2.380521797797e-004, (float) 3.066623414599e-003, (float) - 1.359382829811e-004, (float) - 3.151255446637e-003,
    (float) 2.782985135369e-005, (float) 3.235110828048e-003, (float) 8.584892395569e-005, (float) - 3.318355500563e-003, (float) - 2.049786891772e-004,
    (float) 3.401760376538e-003, (float) 3.307328425452e-004, (float) - 3.484791457407e-003, (float) - 4.636621183957e-004, (float) 3.566606134171e-003,
    (float) 6.034051597652e-004, (float) - 3.647201821596e-003, (float) - 7.497497805930e-004, (float) 3.727291920777e-003, (float) 9.036595637004e-004,
    (float) - 3.806649107069e-003, (float) - 1.065867612984e-003, (float) 3.884567294966e-003, (float) 1.236277315086e-003, (float) - 3.961005564525e-003,
    (float) - 1.414791150723e-003, (float) 4.036756667297e-003, (float) 1.602515835838e-003, (float) - 4.111869542510e-003, (float) - 1.800743383460e-003,
    (float) 4.185540927469e-003, (float) 2.009872291898e-003, (float) - 4.257408085939e-003, (float) - 2.230058942337e-003, (float) 4.327941872004e-003,
    (float) 2.462366047089e-003, (float) - 4.397272684020e-003, (float) - 2.708200770755e-003, (float) 4.464960149734e-003, (float) 2.968539548004e-003,
    (float) - 4.530883832943e-003, (float) - 3.244542924923e-003, (float) 4.595095710114e-003, (float) 3.537784469302e-003, (float) - 4.657460095921e-003,
    (float) - 3.849899735547e-003, (float) 4.718000809430e-003, (float) 4.182843960043e-003, (float) - 4.776999990045e-003, (float) - 4.539528511166e-003,
    (float) 4.833943437272e-003, (float) 4.922646164542e-003, (float) - 4.888454593861e-003, (float) - 5.334885946192e-003, (float) 4.941143760176e-003,
    (float) 5.780469043985e-003, (float) - 4.992461365274e-003, (float) - 6.265199509171e-003, (float) 5.041554413526e-003, (float) 6.794738285542e-003,
    (float) - 5.088173036641e-003, (float) - 7.376044395058e-003, (float) 5.132524592209e-003, (float) 8.018375971792e-003, (float) - 5.174387044018e-003,
    (float) - 8.733007006427e-003, (float) 5.213750365616e-003, (float) 9.534285432471e-003, (float) - 5.251195927114e-003, (float) - 1.044195072235e-002,
    (float) 5.286143606555e-003, (float) 1.148100562390e-002, (float) - 5.318182436083e-003, (float) - 1.268466390300e-002, (float) 5.347878808814e-003,
    (float) 1.410004942171e-002, (float) - 5.375128932984e-003, (float) - 1.579376157069e-002, (float) 5.399698942850e-003, (float) 1.786325203520e-002,
    (float) - 5.422169472156e-003, (float) - 2.045920855579e-002, (float) 5.442242611442e-003, (float) 2.382438645728e-002, (float) - 5.459104973796e-003,
    (float) - 2.837649392789e-002, (float) 5.474214913211e-003, (float) 3.490845950781e-002, (float) - 5.486081437467e-003, (float) - 4.511486713033e-002,
    (float) 5.495807726126e-003, (float) 6.340609543466e-002, (float) - 5.502587343827e-003, (float) - 1.059496325621e-001, (float) 5.506719537453e-003,
    (float) 3.182586432339e-001, (float) 4.944918979528e-001, (float) 3.182586432339e-001, (float) 5.506719537453e-003, (float) - 1.059496325621e-001,
    (float) - 5.502587343827e-003, (float) 6.340609543466e-002, (float) 5.495807726126e-003, (float) - 4.511486713033e-002, (float) - 5.486081437467e-003,
    (float) 3.490845950781e-002, (float) 5.474214913211e-003, (float) - 2.837649392789e-002, (float) - 5.459104973796e-003, (float) 2.382438645728e-002,
    (float) 5.442242611442e-003, (float) - 2.045920855579e-002, (float) - 5.422169472156e-003, (float) 1.786325203520e-002, (float) 5.399698942850e-003,
    (float) - 1.579376157069e-002, (float) - 5.375128932984e-003, (float) 1.410004942171e-002, (float) 5.347878808814e-003, (float) - 1.268466390300e-002,
    (float) - 5.318182436083e-003, (float) 1.148100562390e-002, (float) 5.286143606555e-003, (float) - 1.044195072235e-002, (float) - 5.251195927114e-003,
    (float) 9.534285432471e-003, (float) 5.213750365616e-003, (float) - 8.733007006427e-003, (float) - 5.174387044018e-003, (float) 8.018375971792e-003,
    (float) 5.132524592209e-003, (float) - 7.376044395058e-003, (float) - 5.088173036641e-003, (float) 6.794738285542e-003, (float) 5.041554413526e-003,
    (float) - 6.265199509171e-003, (float) - 4.992461365274e-003, (float) 5.780469043985e-003, (float) 4.941143760176e-003, (float) - 5.334885946192e-003,
    (float) - 4.888454593861e-003, (float) 4.922646164542e-003, (float) 4.833943437272e-003, (float) - 4.539528511166e-003, (float) - 4.776999990045e-003,
    (float) 4.182843960043e-003, (float) 4.718000809430e-003, (float) - 3.849899735547e-003, (float) - 4.657460095921e-003, (float) 3.537784469302e-003,
    (float) 4.595095710114e-003, (float) - 3.244542924923e-003, (float) - 4.530883832943e-003, (float) 2.968539548004e-003, (float) 4.464960149734e-003,
    (float) - 2.708200770755e-003, (float) - 4.397272684020e-003, (float) 2.462366047089e-003, (float) 4.327941872004e-003, (float) - 2.230058942337e-003,
    (float) - 4.257408085939e-003, (float) 2.009872291898e-003, (float) 4.185540927469e-003, (float) - 1.800743383460e-003, (float) - 4.111869542510e-003,
    (float) 1.602515835838e-003, (float) 4.036756667297e-003, (float) - 1.414791150723e-003, (float) - 3.961005564525e-003, (float) 1.236277315086e-003,
    (float) 3.884567294966e-003, (float) - 1.065867612984e-003, (float) - 3.806649107069e-003, (float) 9.036595637004e-004, (float) 3.727291920777e-003,
    (float) - 7.497497805930e-004, (float) - 3.647201821596e-003, (float) 6.034051597652e-004, (float) 3.566606134171e-003, (float) - 4.636621183957e-004,
    (float) - 3.484791457407e-003, (float) 3.307328425452e-004, (float) 3.401760376538e-003, (float) - 2.049786891772e-004, (float) - 3.318355500563e-003,
    (float) 8.584892395569e-005, (float) 3.235110828048e-003, (float) 2.782985135369e-005, (float) - 3.151255446637e-003, (float) - 1.359382829811e-004,
    (float) 3.066623414599e-003, (float) 2.380521797797e-004, (float) - 2.981956373303e-003, (float) - 3.346488919796e-004, (float) 2.897715073078e-003,
    (float) 4.268830572252e-004, (float) - 2.812899297523e-003, (float) - 5.143438896094e-004, (float) 2.727379304929e-003, (float) 5.963947377435e-004,
    (float) - 2.642112224434e-003, (float) - 6.736494366209e-004, (float) 2.557416285858e-003, (float) 7.471803656806e-004, (float) - 2.472094691981e-003,
    (float) - 8.160716710263e-004, (float) 2.386557312794e-003, (float) 8.799533412324e-004, (float) - 2.301879563126e-003, (float) - 9.399028586473e-004,
    (float) 2.217664374578e-003, (float) 9.962070742267e-004, (float) - 2.133252204410e-003, (float) - 1.047877974227e-003, (float) 2.049903843463e-003,
    (float) 1.095925463900e-003, (float) - 1.967442657059e-003, (float) - 1.140994785147e-003, (float) 1.884828365040e-003, (float) 1.181949655991e-003,
    (float) - 1.803063205736e-003, (float) - 1.219211242008e-003, (float) 1.722681043912e-003, (float) 1.254114183544e-003, (float) - 1.642079777660e-003,
    (float) - 1.285301279727e-003, (float) 1.561900282048e-003, (float) 1.312269387543e-003, (float) - 1.483771621272e-003, (float) - 1.337154931483e-003,
    (float) 1.405674275449e-003, (float) 1.358185632312e-003, (float) - 1.329115340603e-003, (float) - 1.376198638858e-003, (float) 1.254375291373e-003,
    (float) 1.392478926372e-003, (float) - 1.179673831986e-003, (float) - 1.405087088396e-003, (float) 1.106948972842e-003, (float) 1.415601499601e-003,
    (float) - 1.035464848937e-003, (float) - 1.424329953401e-003, (float) 9.640000205246e-004, (float) 1.429349304332e-003, (float) - 8.949540559698e-004,
    (float) - 1.433298843968e-003, (float) 8.254983244591e-004, (float) 1.433006680474e-003, (float) - 7.590842956586e-004, (float) - 1.431723738610e-003,
    (float) 6.930389693885e-004, (float) 1.427153322508e-003, (float) - 6.296742832444e-004, (float) - 1.421549050340e-003, (float) 5.672145160932e-004,
    (float) 1.413756425541e-003, (float) - 5.064357635350e-004, (float) - 1.404378077863e-003, (float) 4.469245863820e-004, (float) 1.393229786168e-003,
    (float) - 3.886579246534e-004, (float) - 1.380095678916e-003, (float) 3.319638947909e-004, (float) 1.365216750693e-003, (float) - 2.767754823491e-004,
    (float) - 1.348538305787e-003, (float) 2.232129919651e-004, (float) 1.329974954064e-003, (float) - 1.717961954652e-004, (float) - 1.310173684327e-003,
    (float) 1.223071384268e-004, (float) 1.289443015758e-003, (float) - 7.433354174569e-005, (float) - 1.267597215030e-003, (float) 2.793170702027e-005,
    (float) 1.244802222599e-003, (float) 1.722309885781e-005, (float) - 1.220730499698e-003, (float) - 6.105502150698e-005, (float) 1.195016515742e-003,
    (float) 1.029125055130e-004, (float) - 1.168069638194e-003, (float) - 1.425881460745e-004, (float) 1.140444841288e-003, (float) 1.803999502271e-004,
    (float) - 1.112203502198e-003, (float) - 2.163900938188e-004, (float) 1.083612570606e-003, (float) 2.509109323082e-004, (float) - 1.054638636894e-003,
    (float) - 2.843503685027e-004, (float) 1.024733751383e-003, (float) 3.163873335071e-004, (float) - 9.937591861314e-004, (float) - 3.465711833398e-004,
    (float) 9.620204713426e-004, (float) 3.747939959452e-004, (float) - 9.297436930304e-004, (float) - 4.009277182108e-004, (float) 8.973420149762e-004,
    (float) 4.250690490151e-004, (float) - 8.653751202061e-004, (float) - 4.480343741175e-004, (float) 8.334617833183e-004, (float) 4.701439201458e-004,
    (float) - 8.009185568264e-004, (float) - 4.909399565337e-004, (float) 7.676007675934e-004, (float) 5.098171451209e-004, (float) - 7.340039635307e-004,
    (float) - 5.267382801903e-004, (float) 7.004526959840e-004, (float) 5.416686835400e-004, (float) - 6.675564463085e-004, (float) - 5.552572270111e-004,
    (float) 6.352880253925e-004, (float) 5.681353065596e-004, (float) - 6.029679388270e-004, (float) - 5.802595330793e-004, (float) 5.697973331218e-004,
    (float) 5.904522258905e-004, (float) - 5.364779620831e-004, (float) - 5.984097187292e-004, (float) 5.039254713546e-004, (float) 6.046314459814e-004,
    (float) - 4.726777081461e-004, (float) - 6.102659557898e-004, (float) 4.419629007982e-004, (float) 6.154740944566e-004, (float) - 4.110126983210e-004,
    (float) - 6.197373802874e-004, (float) 3.794107287795e-004, (float) 6.217754987501e-004, (float) - 3.484125142872e-004, (float) - 6.218075972611e-004,
    (float) 3.189781263679e-004, (float) 6.209921559119e-004, (float) - 2.909797933304e-004, (float) - 6.205808968069e-004, (float) 2.625902273169e-004,
    (float) 6.194421365491e-004, (float) - 2.334612077119e-004, (float) - 6.160453602904e-004, (float) 2.050193734239e-004, (float) 6.103250533849e-004,
    (float) - 1.791348851279e-004, (float) - 6.047947232235e-004, (float) 1.543883746225e-004, (float) 6.000426995209e-004, (float) - 1.287437605610e-004,
    (float) - 5.941431998669e-004, (float) 1.025052366029e-004, (float) 5.852799390936e-004, (float) - 7.867758251969e-005, (float) - 5.757813638276e-004,
    (float) 5.713859403233e-005, (float) 5.677956913097e-004, (float) - 3.493017868056e-005, (float) - 5.595560306151e-004, (float) 1.129727308307e-005,
    (float) 5.478734582255e-004, (float) 9.674319498129e-006, (float) - 5.355054669473e-004, (float) - 2.779134779343e-005, (float) 5.255987039514e-004,
    (float) 4.753115459198e-005, (float) - 5.146667747733e-004, (float) - 6.827228464703e-005, (float) 4.998780903868e-004, (float) 8.476411455038e-005,
    (float) - 4.866041612047e-004, (float) - 1.000048491894e-004, (float) 4.756987576397e-004, (float) 1.185710423385e-004, (float) - 4.603990618508e-004,
    (float) - 1.338587279643e-004, (float) 4.447247309663e-004, (float) 1.456088038973e-004, (float) - 4.335350916263e-004, (float) - 1.621552954359e-004,
    (float) 4.170995964833e-004, (float) 1.747765776478e-004, (float) - 4.009205689012e-004, (float) - 1.842093035737e-004, (float) 3.895781288432e-004,
    (float) 1.999494168871e-004, (float) - 3.696711810736e-004, (float) - 2.060273208877e-004, (float) 3.582196439592e-004, (float) 2.190501787592e-004,
    (float) - 3.401042900591e-004, (float) - 2.259436476779e-004, (float) 3.261618119620e-004, (float) 2.353216838587e-004, (float) - 3.097933373634e-004,
    (float) - 2.422677063636e-004, (float) 2.945239709175e-004, (float) 2.491244096402e-004, (float) - 2.789293863887e-004, (float) - 2.550401641814e-004,
    (float) 2.635383838792e-004, (float) 2.602688344952e-004, (float) - 2.483814212302e-004, (float) - 2.650434716994e-004, (float) 2.331377491223e-004,
    (float) 2.690195363322e-004, (float) - 2.181185345008e-004, (float) - 2.723750410219e-004, (float) 2.033292028967e-004, (float) 2.752088365441e-004,
    (float) - 1.887189126391e-004, (float) - 2.775743726755e-004, (float) 1.741553960777e-004, (float) 2.793085842202e-004, (float) - 1.597707317081e-004,
    (float) - 2.804597862803e-004, (float) 1.455506215115e-004, (float) 2.808967923003e-004, (float) - 1.318263284371e-004, (float) - 2.810498626639e-004,
    (float) 1.182425329275e-004, (float) 2.807049278836e-004, (float) - 1.049053516406e-004, (float) - 2.798865730517e-004, (float) 9.185585608604e-005,
    (float) 2.786327455942e-004, (float) - 7.913713870114e-005, (float) - 2.770661665078e-004, (float) 6.658908505887e-005, (float) 2.749705769146e-004,
    (float) - 5.452878257789e-005, (float) - 2.727139254030e-004, (float) 4.267470287693e-005, (float) 2.701901534524e-004, (float) - 3.098103063675e-005,
    (float) - 2.672713250910e-004, (float) 1.959506007225e-005, (float) 2.640373512578e-004, (float) - 8.605011575155e-006, (float) - 2.607451266109e-004,
    (float) - 2.297001211178e-006, (float) 2.571630808553e-004, (float) 1.298908745154e-005, (float) - 2.533379001119e-004, (float) - 2.346096839263e-005,
    (float) 2.493326534241e-004, (float) 3.380378919976e-005, (float) - 2.450580921435e-004, (float) - 4.394022835345e-005, (float) 2.405543319009e-004,
    (float) 5.380295174011e-005, (float) - 2.360269018314e-004, (float) - 6.362969041322e-005, (float) 2.313695981464e-004, (float) 7.350850871907e-005,
    (float) - 2.263811595801e-004, (float) - 8.322092258564e-005, (float) 2.212630220095e-004, (float) 9.291206030779e-005, (float) - 2.159988725649e-004,
    (float) - 1.026783876418e-004, (float) 2.105136263884e-004, (float) 1.125436526441e-004, (float) - 2.047505646951e-004, (float) - 1.224823030873e-004,
    (float) 1.988019142637e-004, (float) 1.327632434198e-004, (float) - 1.923098357663e-004, (float) - 1.431090602159e-004, (float) 1.854489350961e-004,
    (float) 1.536329356026e-004, (float) - 1.782437567680e-004, (float) - 1.645342831667e-004, (float) 1.705747652949e-004, (float) 1.760669168660e-004,
    (float) - 1.619282978772e-004, (float) - 1.878370907468e-004, (float) 1.525152442899e-004, (float) 2.001325818624e-004, (float) - 1.420059883336e-004,
    (float) - 2.128963846610e-004, (float) 1.302389953023e-004, (float) 2.262277295494e-004, (float) - 1.167662237676e-004, (float) - 2.398340044811e-004,
    (float) 1.015795101283e-004, (float) 2.539623487208e-004, (float) - 8.400473971490e-005, (float) - 2.682059180462e-004, (float) 6.386636089117e-005,
    (float) 2.826060999738e-004, (float) - 4.040411770010e-005, (float) - 2.965903264243e-004, (float) 1.328843895284e-005, (float) 3.099640470313e-004,
    (float) 1.843967368528e-005, (float) - 3.217977487285e-004, (float) - 5.533287789728e-005, (float) 3.312977192436e-004, (float) 9.845446547453e-005,
    (float) - 3.367394950886e-004, (float) - 1.482361183421e-004, (float) 3.366006455231e-004, (float) 2.057925864008e-004, (float) - 3.278882894771e-004,
    (float) - 2.713518058883e-004, (float) 3.071631118731e-004, (float) 3.450469073084e-004, (float) - 2.693320767774e-004, (float) - 4.257030079639e-004,
    (float) 2.077730139398e-004, (float) 5.103528659400e-004, (float) - 1.133457948942e-004, (float) - 5.918859501058e-004, (float) - 2.508428842065e-005,
    (float) 6.567967063227e-004, (float) 2.211378711751e-004, (float) - 6.784989588347e-004, (float) - 4.869084931865e-004, (float) 6.081914903075e-004,
    (float) 8.234667012568e-004, (float) - 3.557893702940e-004, (float) - 1.185316896754e-003, (float) - 2.336521287917e-004, (float) 1.370278722621e-003,
    (float) 1.368071261832e-003, (float) - 6.494578851510e-004, (float) - 2.821616068751e-003, (float) - 3.355260288418e-003, (float) - 2.309090436241e-003,
    (float) - 9.559790653313e-004, (float) - 1.916895913880e-004, (float) 3.796696928466e-006
  };

  *lenh0 = lensh02;             /* store 'number of coefficients' */
  *h0 = sh02;                   /* store pointer to h02[]-array */
}

#undef lensh02
/* ....................... End of fill_slp_2_to_1() ....................... */



/*
  ============================================================================

        void fill_slp_3_to_1 (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize pointer to array with FIR coefficients for up/down
        sampling low-pass filter with factor 3:1 or 1:3 and 24 bit
        representation.
		v1.0 filtdemo firpm 16000,2602,2667,0.3, 80, 728
		v1.1 filtdemo firpm 16000,2602,2667,0.3, 100, 872
		h3 = filtdemo('getfilt')
		for i = 1:length(h3) fprintf('(float)%20.12e, ', h3(i));
		if rem(i,5) == 0 fprintf('\n');	end end

        Parameters:
        ~~~~~~~~~~~
        h0:    (Out) pointer to array with FIR coefficients
        lenh0: (Out) pointer to number of coefficients


        Return value:
        ~~~~~~~~~~~~~
        None.

        Author: <balazs.kovesi@orange.com>
        ~~~~~~~

        History:
        ~~~~~~~~
		 05.April.2012 v1.0 - Added filters: SHQ2 and SHQ3 (France Telecom)

 ============================================================================
*/
#define lensh03 873

void fill_slp_3_to_1 (h0, lenh0)
     float **h0;
     long *lenh0;
{
  static float sh03[lensh03] = {
    (float) - 5.702667454304e-005, (float) - 2.290248756652e-004, (float) - 5.695388694927e-004, (float) - 1.047262848994e-003, (float) - 1.506278259322e-003,
    (float) - 1.697084386485e-003, (float) - 1.398843870300e-003, (float) - 5.776254455710e-004, (float) 5.267279465880e-004, (float) 1.474227829087e-003,
    (float) 1.850941005949e-003, (float) 1.521886181517e-003, (float) 7.374635156026e-004, (float) - 2.831586756965e-006, (float) - 2.610758773407e-004,
    (float) 5.790488592536e-005, (float) 6.558699193392e-004, (float) 1.065027553853e-003, (float) 9.887411557505e-004, (float) 5.083645939775e-004,
    (float) 2.342940220659e-006, (float) - 1.567062494057e-004, (float) 1.110403908595e-004, (float) 5.457214173255e-004, (float) 7.717764862227e-004,
    (float) 6.055726812478e-004, (float) 1.949515317766e-004, (float) - 1.184196188439e-004, (float) - 9.160203082544e-005, (float) 2.209374944482e-004,
    (float) 5.265018221270e-004, (float) 5.531560214976e-004, (float) 2.815751711742e-004, (float) - 4.853153913830e-005, (float) - 1.563367357580e-004,
    (float) 3.366074541317e-005, (float) 3.328093694602e-004, (float) 4.636714131641e-004, (float) 3.086568259652e-004, (float) 9.521084805123e-006,
    (float) - 1.672524549008e-004, (float) - 7.321835254365e-005, (float) 1.915905388598e-004, (float) 3.756857462901e-004, (float) 3.081437126124e-004,
    (float) 5.226907920748e-005, (float) - 1.575946050063e-004, (float) - 1.344789464591e-004, (float) 9.034674803378e-005, (float) 3.003518594011e-004,
    (float) 2.971198893452e-004, (float) 8.384203623239e-005, (float) - 1.407963804041e-004, (float) - 1.703890216943e-004, (float) 1.652684652722e-005,
    (float) 2.386510548738e-004, (float) 2.839782736591e-004, (float) 1.090673723768e-004, (float) - 1.216308514221e-004, (float) - 1.923295141096e-004,
    (float) - 3.985072213105e-005, (float) 1.875991204246e-004, (float) 2.717660216481e-004, (float) 1.314707046626e-004, (float) - 1.008243557814e-004,
    (float) - 2.054434756406e-004, (float) - 8.467292384865e-005, (float) 1.442573681930e-004, (float) 2.610066066766e-004, (float) 1.524510476976e-004,
    (float) - 7.902847673495e-005, (float) - 2.130526218114e-004, (float) - 1.219890713673e-004, (float) 1.063399253112e-004, (float) 2.519859951800e-004,
    (float) 1.734491436341e-004, (float) - 5.587231315091e-005, (float) - 2.169349988011e-004, (float) - 1.549061289756e-004, (float) 7.120763788262e-005,
    (float) 2.437177281471e-004, (float) 1.948555676425e-004, (float) - 3.086033846812e-005, (float) - 2.175745882557e-004, (float) - 1.849236252476e-004,
    (float) 3.722566423543e-005, (float) 2.353340603942e-004, (float) 2.167081052392e-004, (float) - 3.703170533307e-006, (float) - 2.152202989036e-004,
    (float) - 2.130638454690e-004, (float) 3.078805881752e-006, (float) 2.259877675272e-004, (float) 2.390210432952e-004, (float) 2.617581267358e-005,
    (float) - 2.094639911149e-004, (float) - 2.396408397869e-004, (float) - 3.220609123686e-005, (float) 2.146367670825e-004, (float) 2.613020076192e-004,
    (float) 5.899602321765e-005, (float) - 1.997806524280e-004, (float) - 2.644528721997e-004, (float) - 6.908347167316e-005, (float) 2.003994342832e-004,
    (float) 2.828118853525e-004, (float) 9.456005724760e-005, (float) - 1.859305896169e-004, (float) - 2.873053305036e-004, (float) - 1.078135783906e-004,
    (float) 1.825933898221e-004, (float) 3.028816854225e-004, (float) 1.326435299606e-004, (float) - 1.676779512332e-004, (float) - 3.079331791370e-004,
    (float) - 1.485750250569e-004, (float) 1.605453339105e-004, (float) 3.207874253071e-004, (float) 1.729981027969e-004, (float) - 1.446923844597e-004,
    (float) - 3.258518962902e-004, (float) - 1.912839973272e-004, (float) 1.337849210997e-004, (float) 3.359204137548e-004, (float) 2.154852410306e-004,
    (float) - 1.164355791130e-004, (float) - 3.402946219591e-004, (float) - 2.356749110871e-004, (float) 1.017519030470e-004, (float) 3.473219066124e-004,
    (float) 2.595861094810e-004, (float) - 8.247985162440e-005, (float) - 3.501778624332e-004, (float) - 2.808530765872e-004, (float) 6.448731739501e-005,
    (float) 3.542546122559e-004, (float) 3.044839629528e-004, (float) - 4.306342775237e-005, (float) - 3.551282513302e-004, (float) - 3.263921987448e-004,
    (float) 2.192426783394e-005, (float) 3.561971331453e-004, (float) 3.497859191992e-004, (float) 2.055980934252e-006, (float) - 3.544148970394e-004,
    (float) - 3.717977628683e-004, (float) - 2.629987685449e-005, (float) 3.520514596533e-004, (float) 3.944857642507e-004, (float) 5.277571215679e-005,
    (float) - 3.471915002737e-004, (float) - 4.160648347652e-004, (float) - 7.993813455368e-005, (float) 3.411114019359e-004, (float) 4.376339448427e-004,
    (float) 1.088493550823e-004, (float) - 3.327174320834e-004, (float) - 4.581277303213e-004, (float) - 1.385955596429e-004, (float) 3.227066885945e-004,
    (float) 4.780809063323e-004, (float) 1.696775520342e-004, (float) - 3.105144597273e-004, (float) - 4.968659484029e-004, (float) - 2.014702976084e-004,
    (float) 2.967304590010e-004, (float) 5.150937489368e-004, (float) 2.347087797104e-004, (float) - 2.804528555952e-004, (float) - 5.317678587328e-004,
    (float) - 2.684301130237e-004, (float) 2.625408226728e-004, (float) 5.476982715351e-004, (float) 3.035278106007e-004, (float) - 2.419332992486e-004,
    (float) - 5.617064251919e-004, (float) - 3.388485533212e-004, (float) 2.196467033878e-004, (float) 5.746786785084e-004, (float) 3.752457273392e-004,
    (float) - 1.947946881500e-004, (float) - 5.857581310721e-004, (float) - 4.120508580248e-004, (float) 1.677356672876e-004, (float) 5.950203471973e-004,
    (float) 4.492196431000e-004, (float) - 1.384637472272e-004, (float) - 6.023600081843e-004, (float) - 4.866311063082e-004, (float) 1.070101277961e-004,
    (float) 6.077115455789e-004, (float) 5.242229671950e-004, (float) - 7.332517897366e-005, (float) - 6.108956903682e-004, (float) - 5.617572406918e-004,
    (float) 3.765710388723e-005, (float) 6.122535665132e-004, (float) 5.998690210736e-004, (float) 1.103337252265e-006, (float) - 6.101918060802e-004,
    (float) - 6.366652752751e-004, (float) - 4.101866700778e-005, (float) 6.065187804431e-004, (float) 6.737711020146e-004, (float) 8.349599417419e-005,
    (float) - 6.001874205543e-004, (float) - 7.107611600986e-004, (float) - 1.289670732718e-004, (float) 5.898895822538e-004, (float) 7.457161842468e-004,
    (float) 1.753449481877e-004, (float) - 5.774443295022e-004, (float) - 7.798678193657e-004, (float) - 2.230334725061e-004, (float) 5.635271134980e-004,
    (float) 8.152306244447e-004, (float) 2.755046534225e-004, (float) - 5.434273506344e-004, (float) - 8.464802274973e-004, (float) - 3.277092661666e-004,
    (float) 5.210307991128e-004, (float) 8.756596447340e-004, (float) 3.794273420016e-004, (float) - 4.990318212014e-004, (float) - 9.079503131393e-004,
    (float) - 4.380386208554e-004, (float) 4.685715007029e-004, (float) 9.341398358924e-004, (float) 4.952457363886e-004, (float) - 4.360095410718e-004,
    (float) - 9.581583004572e-004, (float) - 5.526178358650e-004, (float) 4.017025817632e-004, (float) 9.817267986707e-004, (float) 6.126724675261e-004,
    (float) - 3.629093431450e-004, (float) - 1.002439646862e-003, (float) - 6.737587650718e-004, (float) 3.204253623397e-004, (float) 1.020517658997e-003,
    (float) 7.360180736312e-004, (float) - 2.737981347557e-004, (float) - 1.035166912472e-003, (float) - 7.986608366722e-004, (float) 2.233015838909e-004,
    (float) 1.045721090576e-003, (float) 8.598542706791e-004, (float) - 1.720078687206e-004, (float) - 1.056474335818e-003, (float) - 9.249608577418e-004,
    (float) 1.139016227189e-004, (float) 1.061470672775e-003, (float) 9.889032323226e-004, (float) - 5.260312145850e-005, (float) - 1.062802973797e-003,
    (float) - 1.052661166844e-003, (float) - 1.234469409022e-005, (float) 1.060136819634e-003, (float) 1.115982607845e-003, (float) 8.099003769379e-005,
    (float) - 1.052868934904e-003, (float) - 1.177547710041e-003, (float) - 1.512053491539e-004, (float) 1.044016157338e-003, (float) 1.241286624384e-003,
    (float) 2.276412600050e-004, (float) - 1.028714494774e-003, (float) - 1.302806041666e-003, (float) - 3.068949503392e-004, (float) 1.009322937005e-003,
    (float) 1.363843344369e-003, (float) 3.906609064444e-004, (float) - 9.838656513075e-004, (float) - 1.422262246448e-003, (float) - 4.770635768666e-004,
    (float) 9.535551337159e-004, (float) 1.478448816939e-003, (float) 5.657155736003e-004, (float) - 9.194440566142e-004, (float) - 1.534039735215e-003,
    (float) - 6.587006775073e-004, (float) 8.793107473467e-004, (float) 1.587111750398e-003, (float) 7.547147875959e-004, (float) - 8.338828124821e-004,
    (float) - 1.638222602213e-003, (float) - 8.546083977396e-004, (float) 7.818895385424e-004, (float) 1.686035622285e-003, (float) 9.575865466165e-004,
    (float) - 7.231383262379e-004, (float) - 1.729347857177e-003, (float) - 1.061753152135e-003, (float) 6.598691919979e-004, (float) 1.770597985361e-003,
    (float) 1.169781516343e-003, (float) - 5.891500710358e-004, (float) - 1.806699639550e-003, (float) - 1.278568313904e-003, (float) 5.141179931545e-004,
    (float) 1.841027853974e-003, (float) 1.391905141504e-003, (float) - 4.306947853300e-004, (float) - 1.869713791607e-003, (float) - 1.506708961470e-003,
    (float) 3.409327833389e-004, (float) 1.894038136926e-003, (float) 1.624014474020e-003, (float) - 2.437048054541e-004, (float) - 1.912908177017e-003,
    (float) - 1.743223810231e-003, (float) 1.387216181787e-004, (float) 1.925117887436e-003, (float) 1.862486593022e-003, (float) - 2.821070804092e-005,
    (float) - 1.933229951667e-003, (float) - 1.984800448536e-003, (float) - 9.123378372710e-005, (float) 1.933710705029e-003, (float) 2.106846839834e-003,
    (float) 2.165780283200e-004, (float) - 1.929629718754e-003, (float) - 2.232160937187e-003, (float) - 3.519049528335e-004, (float) 1.916818837101e-003,
    (float) 2.357203940722e-003, (float) 4.947204738569e-004, (float) - 1.896926099071e-003, (float) - 2.483378267598e-003, (float) - 6.466372123975e-004,
    (float) 1.868195138012e-003, (float) 2.609246811353e-003, (float) 8.069238284096e-004, (float) - 1.830682441595e-003, (float) - 2.734527768949e-003,
    (float) - 9.751897771009e-004, (float) 1.785017781192e-003, (float) 2.860388753350e-003, (float) 1.153172877730e-003, (float) - 1.729312203396e-003,
    (float) - 2.985339528727e-003, (float) - 1.339916394519e-003, (float) 1.664473634983e-003, (float) 3.110932565488e-003, (float) 1.537838751758e-003,
    (float) - 1.587754627653e-003, (float) - 3.234954638367e-003, (float) - 1.745673580609e-003, (float) 1.499886644666e-003, (float) 3.358421754204e-003,
    (float) 1.965130821805e-003, (float) - 1.398872765578e-003, (float) - 3.479845703961e-003, (float) - 2.195572687404e-003, (float) 1.285003495634e-003,
    (float) 3.600016865417e-003, (float) 2.438562967617e-003, (float) - 1.156618036116e-003, (float) - 3.718180542243e-003, (float) - 2.694534774140e-003,
    (float) 1.012925180360e-003, (float) 3.834373750933e-003, (float) 2.964679101911e-003, (float) - 8.525485951972e-004, (float) - 3.948403524289e-003,
    (float) - 3.250343132486e-003, (float) 6.737384489508e-004, (float) 4.059688224041e-003, (float) 3.552470544664e-003, (float) - 4.754275550708e-004,
    (float) - 4.168928142770e-003, (float) - 3.873956808281e-003, (float) 2.542076569209e-004, (float) 4.274495623229e-003, (float) 4.215541860516e-003,
    (float) - 8.876791492217e-006, (float) - 4.377372661019e-003, (float) - 4.581049335760e-003, (float) - 2.649760888835e-004, (float) 4.475649547624e-003,
    (float) 4.971772999526e-003, (float) 5.691152906036e-004, (float) - 4.570711772495e-003, (float) - 5.392866001693e-003, (float) - 9.088730314323e-004,
    (float) 4.661637388475e-003, (float) 5.848570522367e-003, (float) 1.289087294300e-003, (float) - 4.748854210334e-003, (float) - 6.345622677870e-003,
    (float) - 1.717082156368e-003, (float) 4.831896281817e-003, (float) 6.891804999608e-003, (float) 2.201630775936e-003, (float) - 4.910578903278e-003,
    (float) - 7.497415910832e-003, (float) - 2.754230074063e-003, (float) 4.984610468332e-003, (float) 8.175812718542e-003, (float) 3.389552293110e-003,
    (float) - 5.054700764131e-003, (float) - 8.946696508829e-003, (float) - 4.129951171808e-003, (float) 5.118515051062e-003, (float) 9.833011394039e-003,
    (float) 5.001471509665e-003, (float) - 5.178437003297e-003, (float) - 1.087335325218e-002, (float) - 6.047856746429e-003, (float) 5.230953032476e-003,
    (float) 1.211655268747e-002, (float) 7.325438345332e-003, (float) - 5.280041797037e-003, (float) - 1.364671806495e-002, (float) - 8.931122228028e-003,
    (float) 5.322573977944e-003, (float) 1.558985548986e-002, (float) 1.101542440923e-002, (float) - 5.359868891871e-003, (float) - 1.816627590250e-002,
    (float) - 1.384581531568e-002, (float) 5.391484192615e-003, (float) 2.178322539469e-002, (float) 1.793508230734e-002, (float) - 5.418411065727e-003,
    (float) - 2.729479625998e-002, (float) - 2.441474613854e-002, (float) 5.438809365830e-003, (float) 3.683129238215e-002, (float) 3.635025705283e-002,
    (float) - 5.453919890093e-003, (float) - 5.764317282647e-002, (float) - 6.601087176309e-002, (float) 5.463088425909e-003, (float) 1.404772330964e-001,
    (float) 2.728880604165e-001, (float) 3.278675322959e-001, (float) 2.728880604165e-001, (float) 1.404772330964e-001, (float) 5.463088425909e-003,
    (float) - 6.601087176309e-002, (float) - 5.764317282647e-002, (float) - 5.453919890093e-003, (float) 3.635025705283e-002, (float) 3.683129238215e-002,
    (float) 5.438809365830e-003, (float) - 2.441474613854e-002, (float) - 2.729479625998e-002, (float) - 5.418411065727e-003, (float) 1.793508230734e-002,
    (float) 2.178322539469e-002, (float) 5.391484192615e-003, (float) - 1.384581531568e-002, (float) - 1.816627590250e-002, (float) - 5.359868891871e-003,
    (float) 1.101542440923e-002, (float) 1.558985548986e-002, (float) 5.322573977944e-003, (float) - 8.931122228028e-003, (float) - 1.364671806495e-002,
    (float) - 5.280041797037e-003, (float) 7.325438345332e-003, (float) 1.211655268747e-002, (float) 5.230953032476e-003, (float) - 6.047856746429e-003,
    (float) - 1.087335325218e-002, (float) - 5.178437003297e-003, (float) 5.001471509665e-003, (float) 9.833011394039e-003, (float) 5.118515051062e-003,
    (float) - 4.129951171808e-003, (float) - 8.946696508829e-003, (float) - 5.054700764131e-003, (float) 3.389552293110e-003, (float) 8.175812718542e-003,
    (float) 4.984610468332e-003, (float) - 2.754230074063e-003, (float) - 7.497415910832e-003, (float) - 4.910578903278e-003, (float) 2.201630775936e-003,
    (float) 6.891804999608e-003, (float) 4.831896281817e-003, (float) - 1.717082156368e-003, (float) - 6.345622677870e-003, (float) - 4.748854210334e-003,
    (float) 1.289087294300e-003, (float) 5.848570522367e-003, (float) 4.661637388475e-003, (float) - 9.088730314323e-004, (float) - 5.392866001693e-003,
    (float) - 4.570711772495e-003, (float) 5.691152906036e-004, (float) 4.971772999526e-003, (float) 4.475649547624e-003, (float) - 2.649760888835e-004,
    (float) - 4.581049335760e-003, (float) - 4.377372661019e-003, (float) - 8.876791492217e-006, (float) 4.215541860516e-003, (float) 4.274495623229e-003,
    (float) 2.542076569209e-004, (float) - 3.873956808281e-003, (float) - 4.168928142770e-003, (float) - 4.754275550708e-004, (float) 3.552470544664e-003,
    (float) 4.059688224041e-003, (float) 6.737384489508e-004, (float) - 3.250343132486e-003, (float) - 3.948403524289e-003, (float) - 8.525485951972e-004,
    (float) 2.964679101911e-003, (float) 3.834373750933e-003, (float) 1.012925180360e-003, (float) - 2.694534774140e-003, (float) - 3.718180542243e-003,
    (float) - 1.156618036116e-003, (float) 2.438562967617e-003, (float) 3.600016865417e-003, (float) 1.285003495634e-003, (float) - 2.195572687404e-003,
    (float) - 3.479845703961e-003, (float) - 1.398872765578e-003, (float) 1.965130821805e-003, (float) 3.358421754204e-003, (float) 1.499886644666e-003,
    (float) - 1.745673580609e-003, (float) - 3.234954638367e-003, (float) - 1.587754627653e-003, (float) 1.537838751758e-003, (float) 3.110932565488e-003,
    (float) 1.664473634983e-003, (float) - 1.339916394519e-003, (float) - 2.985339528727e-003, (float) - 1.729312203396e-003, (float) 1.153172877730e-003,
    (float) 2.860388753350e-003, (float) 1.785017781192e-003, (float) - 9.751897771009e-004, (float) - 2.734527768949e-003, (float) - 1.830682441595e-003,
    (float) 8.069238284096e-004, (float) 2.609246811353e-003, (float) 1.868195138012e-003, (float) - 6.466372123975e-004, (float) - 2.483378267598e-003,
    (float) - 1.896926099071e-003, (float) 4.947204738569e-004, (float) 2.357203940722e-003, (float) 1.916818837101e-003, (float) - 3.519049528335e-004,
    (float) - 2.232160937187e-003, (float) - 1.929629718754e-003, (float) 2.165780283200e-004, (float) 2.106846839834e-003, (float) 1.933710705029e-003,
    (float) - 9.123378372710e-005, (float) - 1.984800448536e-003, (float) - 1.933229951667e-003, (float) - 2.821070804092e-005, (float) 1.862486593022e-003,
    (float) 1.925117887436e-003, (float) 1.387216181787e-004, (float) - 1.743223810231e-003, (float) - 1.912908177017e-003, (float) - 2.437048054541e-004,
    (float) 1.624014474020e-003, (float) 1.894038136926e-003, (float) 3.409327833389e-004, (float) - 1.506708961470e-003, (float) - 1.869713791607e-003,
    (float) - 4.306947853300e-004, (float) 1.391905141504e-003, (float) 1.841027853974e-003, (float) 5.141179931545e-004, (float) - 1.278568313904e-003,
    (float) - 1.806699639550e-003, (float) - 5.891500710358e-004, (float) 1.169781516343e-003, (float) 1.770597985361e-003, (float) 6.598691919979e-004,
    (float) - 1.061753152135e-003, (float) - 1.729347857177e-003, (float) - 7.231383262379e-004, (float) 9.575865466165e-004, (float) 1.686035622285e-003,
    (float) 7.818895385424e-004, (float) - 8.546083977396e-004, (float) - 1.638222602213e-003, (float) - 8.338828124821e-004, (float) 7.547147875959e-004,
    (float) 1.587111750398e-003, (float) 8.793107473467e-004, (float) - 6.587006775073e-004, (float) - 1.534039735215e-003, (float) - 9.194440566142e-004,
    (float) 5.657155736003e-004, (float) 1.478448816939e-003, (float) 9.535551337159e-004, (float) - 4.770635768666e-004, (float) - 1.422262246448e-003,
    (float) - 9.838656513075e-004, (float) 3.906609064444e-004, (float) 1.363843344369e-003, (float) 1.009322937005e-003, (float) - 3.068949503392e-004,
    (float) - 1.302806041666e-003, (float) - 1.028714494774e-003, (float) 2.276412600050e-004, (float) 1.241286624384e-003, (float) 1.044016157338e-003,
    (float) - 1.512053491539e-004, (float) - 1.177547710041e-003, (float) - 1.052868934904e-003, (float) 8.099003769379e-005, (float) 1.115982607845e-003,
    (float) 1.060136819634e-003, (float) - 1.234469409022e-005, (float) - 1.052661166844e-003, (float) - 1.062802973797e-003, (float) - 5.260312145850e-005,
    (float) 9.889032323226e-004, (float) 1.061470672775e-003, (float) 1.139016227189e-004, (float) - 9.249608577418e-004, (float) - 1.056474335818e-003,
    (float) - 1.720078687206e-004, (float) 8.598542706791e-004, (float) 1.045721090576e-003, (float) 2.233015838909e-004, (float) - 7.986608366722e-004,
    (float) - 1.035166912472e-003, (float) - 2.737981347557e-004, (float) 7.360180736312e-004, (float) 1.020517658997e-003, (float) 3.204253623397e-004,
    (float) - 6.737587650718e-004, (float) - 1.002439646862e-003, (float) - 3.629093431450e-004, (float) 6.126724675261e-004, (float) 9.817267986707e-004,
    (float) 4.017025817632e-004, (float) - 5.526178358650e-004, (float) - 9.581583004572e-004, (float) - 4.360095410718e-004, (float) 4.952457363886e-004,
    (float) 9.341398358924e-004, (float) 4.685715007029e-004, (float) - 4.380386208554e-004, (float) - 9.079503131393e-004, (float) - 4.990318212014e-004,
    (float) 3.794273420016e-004, (float) 8.756596447340e-004, (float) 5.210307991128e-004, (float) - 3.277092661666e-004, (float) - 8.464802274973e-004,
    (float) - 5.434273506344e-004, (float) 2.755046534225e-004, (float) 8.152306244447e-004, (float) 5.635271134980e-004, (float) - 2.230334725061e-004,
    (float) - 7.798678193657e-004, (float) - 5.774443295022e-004, (float) 1.753449481877e-004, (float) 7.457161842468e-004, (float) 5.898895822538e-004,
    (float) - 1.289670732718e-004, (float) - 7.107611600986e-004, (float) - 6.001874205543e-004, (float) 8.349599417419e-005, (float) 6.737711020146e-004,
    (float) 6.065187804431e-004, (float) - 4.101866700778e-005, (float) - 6.366652752751e-004, (float) - 6.101918060802e-004, (float) 1.103337252265e-006,
    (float) 5.998690210736e-004, (float) 6.122535665132e-004, (float) 3.765710388723e-005, (float) - 5.617572406918e-004, (float) - 6.108956903682e-004,
    (float) - 7.332517897366e-005, (float) 5.242229671950e-004, (float) 6.077115455789e-004, (float) 1.070101277961e-004, (float) - 4.866311063082e-004,
    (float) - 6.023600081843e-004, (float) - 1.384637472272e-004, (float) 4.492196431000e-004, (float) 5.950203471973e-004, (float) 1.677356672876e-004,
    (float) - 4.120508580248e-004, (float) - 5.857581310721e-004, (float) - 1.947946881500e-004, (float) 3.752457273392e-004, (float) 5.746786785084e-004,
    (float) 2.196467033878e-004, (float) - 3.388485533212e-004, (float) - 5.617064251919e-004, (float) - 2.419332992486e-004, (float) 3.035278106007e-004,
    (float) 5.476982715351e-004, (float) 2.625408226728e-004, (float) - 2.684301130237e-004, (float) - 5.317678587328e-004, (float) - 2.804528555952e-004,
    (float) 2.347087797104e-004, (float) 5.150937489368e-004, (float) 2.967304590010e-004, (float) - 2.014702976084e-004, (float) - 4.968659484029e-004,
    (float) - 3.105144597273e-004, (float) 1.696775520342e-004, (float) 4.780809063323e-004, (float) 3.227066885945e-004, (float) - 1.385955596429e-004,
    (float) - 4.581277303213e-004, (float) - 3.327174320834e-004, (float) 1.088493550823e-004, (float) 4.376339448427e-004, (float) 3.411114019359e-004,
    (float) - 7.993813455368e-005, (float) - 4.160648347652e-004, (float) - 3.471915002737e-004, (float) 5.277571215679e-005, (float) 3.944857642507e-004,
    (float) 3.520514596533e-004, (float) - 2.629987685449e-005, (float) - 3.717977628683e-004, (float) - 3.544148970394e-004, (float) 2.055980934252e-006,
    (float) 3.497859191992e-004, (float) 3.561971331453e-004, (float) 2.192426783394e-005, (float) - 3.263921987448e-004, (float) - 3.551282513302e-004,
    (float) - 4.306342775237e-005, (float) 3.044839629528e-004, (float) 3.542546122559e-004, (float) 6.448731739501e-005, (float) - 2.808530765872e-004,
    (float) - 3.501778624332e-004, (float) - 8.247985162440e-005, (float) 2.595861094810e-004, (float) 3.473219066124e-004, (float) 1.017519030470e-004,
    (float) - 2.356749110871e-004, (float) - 3.402946219591e-004, (float) - 1.164355791130e-004, (float) 2.154852410306e-004, (float) 3.359204137548e-004,
    (float) 1.337849210997e-004, (float) - 1.912839973272e-004, (float) - 3.258518962902e-004, (float) - 1.446923844597e-004, (float) 1.729981027969e-004,
    (float) 3.207874253071e-004, (float) 1.605453339105e-004, (float) - 1.485750250569e-004, (float) - 3.079331791370e-004, (float) - 1.676779512332e-004,
    (float) 1.326435299606e-004, (float) 3.028816854225e-004, (float) 1.825933898221e-004, (float) - 1.078135783906e-004, (float) - 2.873053305036e-004,
    (float) - 1.859305896169e-004, (float) 9.456005724760e-005, (float) 2.828118853525e-004, (float) 2.003994342832e-004, (float) - 6.908347167316e-005,
    (float) - 2.644528721997e-004, (float) - 1.997806524280e-004, (float) 5.899602321765e-005, (float) 2.613020076192e-004, (float) 2.146367670825e-004,
    (float) - 3.220609123686e-005, (float) - 2.396408397869e-004, (float) - 2.094639911149e-004, (float) 2.617581267358e-005, (float) 2.390210432952e-004,
    (float) 2.259877675272e-004, (float) 3.078805881752e-006, (float) - 2.130638454690e-004, (float) - 2.152202989036e-004, (float) - 3.703170533307e-006,
    (float) 2.167081052392e-004, (float) 2.353340603942e-004, (float) 3.722566423543e-005, (float) - 1.849236252476e-004, (float) - 2.175745882557e-004,
    (float) - 3.086033846812e-005, (float) 1.948555676425e-004, (float) 2.437177281471e-004, (float) 7.120763788262e-005, (float) - 1.549061289756e-004,
    (float) - 2.169349988011e-004, (float) - 5.587231315091e-005, (float) 1.734491436341e-004, (float) 2.519859951800e-004, (float) 1.063399253112e-004,
    (float) - 1.219890713673e-004, (float) - 2.130526218114e-004, (float) - 7.902847673495e-005, (float) 1.524510476976e-004, (float) 2.610066066766e-004,
    (float) 1.442573681930e-004, (float) - 8.467292384865e-005, (float) - 2.054434756406e-004, (float) - 1.008243557814e-004, (float) 1.314707046626e-004,
    (float) 2.717660216481e-004, (float) 1.875991204246e-004, (float) - 3.985072213105e-005, (float) - 1.923295141096e-004, (float) - 1.216308514221e-004,
    (float) 1.090673723768e-004, (float) 2.839782736591e-004, (float) 2.386510548738e-004, (float) 1.652684652722e-005, (float) - 1.703890216943e-004,
    (float) - 1.407963804041e-004, (float) 8.384203623239e-005, (float) 2.971198893452e-004, (float) 3.003518594011e-004, (float) 9.034674803378e-005,
    (float) - 1.344789464591e-004, (float) - 1.575946050063e-004, (float) 5.226907920748e-005, (float) 3.081437126124e-004, (float) 3.756857462901e-004,
    (float) 1.915905388598e-004, (float) - 7.321835254365e-005, (float) - 1.672524549008e-004, (float) 9.521084805123e-006, (float) 3.086568259652e-004,
    (float) 4.636714131641e-004, (float) 3.328093694602e-004, (float) 3.366074541317e-005, (float) - 1.563367357580e-004, (float) - 4.853153913830e-005,
    (float) 2.815751711742e-004, (float) 5.531560214976e-004, (float) 5.265018221270e-004, (float) 2.209374944482e-004, (float) - 9.160203082544e-005,
    (float) - 1.184196188439e-004, (float) 1.949515317766e-004, (float) 6.055726812478e-004, (float) 7.717764862227e-004, (float) 5.457214173255e-004,
    (float) 1.110403908595e-004, (float) - 1.567062494057e-004, (float) 2.342940220659e-006, (float) 5.083645939775e-004, (float) 9.887411557505e-004,
    (float) 1.065027553853e-003, (float) 6.558699193392e-004, (float) 5.790488592536e-005, (float) - 2.610758773407e-004, (float) - 2.831586756965e-006,
    (float) 7.374635156026e-004, (float) 1.521886181517e-003, (float) 1.850941005949e-003, (float) 1.474227829087e-003, (float) 5.267279465880e-004,
    (float) - 5.776254455710e-004, (float) - 1.398843870300e-003, (float) - 1.697084386485e-003, (float) - 1.506278259322e-003, (float) - 1.047262848994e-003,
    (float) - 5.695388694927e-004, (float) - 2.290248756652e-004, (float) - 5.702667454304e-005
  };

  *lenh0 = lensh03;             /* store 'number of coefficients' */
  *h0 = sh03;                   /* store pointer to h02[]-array */
}

#undef lensh03
/* ........................ End of fill_lp_3_to_1() ........................ */



/* ************************* END OF FIR-FLAT.C ************************** */
