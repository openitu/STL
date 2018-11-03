# Annex A - List of software tools available

(This annex forms an integral part of this Recommendation)

This annex contains a list with a short description of the software tools available in the ITU‑T Software Tool Library. This is referred to in the associated documentation as the Software Tool Library release 2009, or STL2009. All the routines in the modules are written in C.

## a) Example programs available

Associated header file: `ugstdemo.h`

The following programs are examples of the use of the modules:

|||
|-------|-----------------------------------------------------------|
| g711demo.c | on the use of the ITU‑T G.711 module. |
| g726demo.c | on the use of the ITU‑T G.726 module. |
| g727demo.c | on the use of the ITU‑T G.727 module |
| g722demo.c | on the use of the ITU‑T G.722 module. |
| g728enc.c | on the use of the ITU‑T G.728 floating-point encoder. |
| g728dec.c | on the use of the ITU‑T G.728 floating-point decoder. |
| g728fpenc.c | on the use of the ITU‑T G.728 fixed-point encoder. |
| g728fpdec.c | on the use of the ITU‑T G.728 fixed-point decoder. |
| rpedemo.c | on the use of the full-rate GSM 06.10 speech codec module. |
| sv56demo.c | on the use of the speech voltmeter module, and also the gain/loss routine. |
| eiddemo.c | on the use of the error insertion device for bit error insertion and frame erasure. |
| eid-ev.c | on the use of the error insertion device for bit error insertion for layered bitstreams, which can be used to apply errors to individual layers in layered bitstreams such as ITU‑T G.718 or ITU‑T G.729.1. |
| gen-patt.c | on the use of generating bit error pattern files for error insertion in ITU‑T G.192-compliant serial bitstreams encoded files. |
| gen_rate_profile.c | on the use of fast switching rate profile generation tool. |
| firdemo.c | on the use of the FIR (finite impulse response) high-quality low-pass and band-pass filters and of the FIR-IRS filters, associated with the rate change module. |
| pcmdemo.c | on the use of the ITU‑T G.712 (standard PCM) IIR (infinite impulse response) filters, associated with the rate change module. |
| filter.c | on the use of both the IIR and the FIR filters available in the rate change module. |
| mnrudemo.c | on the use of the narrow-band and wideband modulated noise reference unity (ITU‑T P.81) module. |
| spdemo.c | on the use of the serialization and parallelization routines of the utility module. |
| g711iplc.c | on the use of Appendix I/G.711 Packet Loss Concealment module. |
| reverb.c | on the use of the reverberation module. |
| truncate.c | on the use of the bitstream truncation module. |
| freqresp.c | on the use of the frequency response computation tool. |
| stereoop.c | on the use of stereo file operations. |

NOTE - The module for the Basic Operators does not have a demo program but it is supplemented by two tools: one to evaluate program ROM complexity for fixed-point code (`basop_cnt.c`), and another to evaluate complexity (including program ROM) of floating-point implementations (`flc_example.c`). Both reside in the Basic Operators module.

## b) Rate change module with FIR (finite impulse response) routines

Name: `firflt.c`

Associated header file: `firflt.h`

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| delta_sm_16khz_init | initialize 16 kHz 1:1 ÄSM weighting filter. |
| hq_down_2_to_1_init | initialize 2:1 low-pass down-sampling filter. |
| hq_down_3_to_1_init | initialize 3:1 low-pass down-sampling filter. |
| hq_up_1_to_2_init   | initialize 1:2 low-pass up-sampling filter. |
| hq_up_1_to_3_init   | initialize 1:3 low-pass up-sampling filter. |
| irs_8khz_init       | initialize 8-kHz P.48 IRS weighting filter. |
| irs_16khz_init      | initialize 16-kHz P.48 IRS weighting filter. |
| linear_phase_pb_2_to_1_init | initialize 2:1 bandpass down-sampling filter. |
| linear_phase_pb_1_to_2_init | initialize 1:2 bandpass up-sampling filter. |
| linear_phase_pb_1_to_1_init | initialize 1:1 bandpass filter. |
| mod_irs_16khz_init | initialize 16-kHz send-side modified IRS weighting filter. |
| mod_irs_48khz_init | initialize 48-kHz send-side modified IRS weighting filter. |
| psophometric_8khz_init | initialize 1:1 ITU‑T O.41 psophometric weighting filter. |
| p341_16khz_init | initialize 1:1 ITU‑T P.341 send-part weighting filter for data sampled at 16 kHz. |
| rx_mod_irs_16khz_init | initialize 16-kHz modified IRS receive-side weighting filter. |
| rx_mod_irs_8khz_init | initialize 8-kHz modified IRS receive-side weighting filter. |
| tia_irs_8khz_init | initialize 8-kHz IRS weighting filter using the TIA coefficients. |
| ht_irs_16khz_init | initialize 16-kHz IRS weighting filter with a half-tilt inclination within the ITU‑T P.48 mask. |
| msin_16khz_init | initialize mobile station weighting filter. |
| bp5k_16khz_init | initialize 50-Hz to 5-kHz-bandpass filter (16 kHz sampling). |
| bp100_5k_16khz_init | initialize a 100-Hz to 5-kHz-bandpass filter (16-kHz sampling). |
| bp14k_32khz_init | initialize a 50-Hz to 14-kHz-bandpass filter (32-kHz sampling). |
| bp20k_48khz_init | initialize a 20-Hz to 20-kHz-bandpass filter (48-kHz sampling). |
| LP1p5_48kHz_init | initialize a low-pass filter with a cut-off frequency of 1.5 kHz (48-kHz sampling). |
| LP35_48kHz_init | initialize a low-pass filter with a cut-off frequency of 3.5 kHz (48-kHz sampling). |
| LP7_48kHz_init | initialize a low-pass filter with a cut-off frequency of 7 kHz (48‑kHz sampling). |
| LP10_48kHz_init | initialize a low-pass filter with a cut-off frequency of 10 kHz (48-kHz sampling). |
| LP12_48kHz_init | initialize a low-pass filter with a cut-off frequency of 12 kHz at (48-kHz sampling). |
| LP14_48kHz_init | initialize a low-pass filter with a cut-off frequency of 14 kHz at 48-kHz sampling). |
| LP20_48kHz_init | initialize a low-pass filter with a cut-off frequency of 20 kHz (48-kHz sampling). |
| hq_kernel | FIR filtering function. |
| hq_reset | clear state variables. |
| hq_free | deallocate FIR-filter memory. |

## c) Rate change module with IIR routines

Name: `iirflt.c`

Associated header file: `iirflt.h`

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| stdpcm_kernel | parallel-form IIR kernel filtering routine. |
| stdpcm_16khz_init | initialization of a parallel-form IIR standard PCM‑filter for input and output data at 16 kHz. |
| stdpcm_1_to_2_init | as "stdpcm_16khz_init( )", but needs input with sampling frequency of 8 kHz and returns data at 16 kHz. |
| stdpcm_2_to_1_init | as "stdpcm_16khz_init( )", but needs input with sampling frequency of 16 kHz and returns data at 8 kHz. |
| stdpcm_reset | clear state variables (needed only if another signal should be processed with the same filter) for a parallel-form structure. |
| stdpcm_free | deallocate filter memory for a parallel-form state variable structure. |
| cascade_iir_kernel | cascade-form IIR filtering routine. |
| iir_G712_8khz init | initialization of a cascade-form IIR standard PCM filter for data sampled at 8 kHz. |
| iir_irs_8khz_init | initialization of a cascade-form IIR P.48 IRS filter for data sampled at 8 kHz. |
| iir_casc_1p_3_to_1_init | initialization of a cascade-form IIR low-pass filter for asynchronization filtering of data and downsampling by a factor of 3:1. |
| iir_casc_1p_1_to_3_init | initialization of a cascade-form IIR low-pass filter for asynchronization filtering of data and upsampling by a factor of 3:1. |
| cascade_iir_reset | clear state variables (needed only if another signal should be processed with the same filter) for a cascade-form structure. |
| cascade_iir_free | deallocate filter memory for a cascade-form state variable structure. |
| direct_iir_kernel | direct-form IIR filtering routine. |
| iir_dir_dc_removal_init | Initialize a direct-form IIR filter structure for a 1:1 DC removal filtering. |
| direct_reset | clear state variables (needed only if another signal should be processed with the same filter) for a direct-form structure. |
| direct_iir_free | deallocate filter memory for a direct-form state variable structure. |

## d) Error insertion module

Name: `eid.c`

Associated header file: `eid.h`

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| open_eid | initializes the error pattern generator (for single-bit errors, burst bit-errors, or single frame erasures). |
| open_burst_eid | initializes the burst frame erasure pattern generator. |
| reset_burst_eid | reinitializes the burst frame erasure pattern generator. |
| BER_generator | generates a bit error sequence with properties defined by "open_eid". |
| FER_generator_random | generates a random frame erasure sequence with properties, defined by "open_eid". |
| FER_generator_burst | generates a burst frame erasure sequence with properties, defined by "open_burst_eid". |
| BER_insertion | modifies the input data bits according to the error pattern, stored in a buffer. |
| FER_module | frame erasure module. |
| close_eid | frees memory allocated to the EID state variable buffer. |

## e) G.711 module

Name: `g711.c`

Associated header file: `g711.h`

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| alaw_compress | compands 1 vector of linear PCM samples to A-law; uses 13 Most Significant Bits (MSBs) from input and 8 Least Significant Bits (LSBs) on output. |
| alaw_expand | expands 1 vector of A-law samples to linear PCM; uses 8 LSBs from input and 13 MSBs on output. |
| ulaw_compress | compands 1 vector of linear PCM samples to µ-law; uses 14 MSBs from input and 8 LSBs on output. |
| ulaw_expand | expands 1 vector of µ-law samples to linear PCM; uses 8 LSBs from input and 14 MSBs on output. |

## f) Appendix I/G.711 Packet Loss Concealment module

Name: `lowcfe.c`

Associated header file: `lowcfe.h`

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| g711plc_construct | LowcFE Constructor. |
| g711plc_dofe | generate the synthetic signal. |
| g711plc_addtohistory | a good frame was received and decoded, add the frame to history buffer. |

## g) G.726 module

Name: `g726.c`

Associated header file: `g726.h`

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| G726_encode | ITU‑T G.726 encoder at 40, 32, 24 and 16 kbit/s. |
| G726_decode | ITU‑T G.726 decoder at 40, 32, 24 and 16 kbit/s. |

## h) Modulated noise reference unit module

Name: `mnru.c`

Associated header file: `mnru.h`

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| MNRU_process | module for addition of modulated noise to a vector of samples, according to Rec. ITU-T P.810, for both the narrow-band and the wideband models. |

## i) Speech voltmeter module

Name: `sv-p56.c`

Associated header file: `sv-p56.h`

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| init_speech_voltmeter | initializes a speech voltmeter state variable. |
| speech_voltmeter | measurement of the active speech level of data in a buffer according to Rec. ITU-T P.56. |

## j) Module with UGST utilities

Name: `ugst-utl.c`

Associated header file: `ugst-utl.h`

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| scale | gain/loss insertion algorithm. |
| sh2fl_16bit | conversion of two&#39;s complement, 16-bit integer to floating point. |
| sh2fl_15bit | conversion of two&#39;s complement, 15-bit integer to floating point. |
| sh2fl_14bit | conversion of two&#39;s complement, 14-bit integer to floating point. |
| sh2fl_13bit | conversion of two&#39;s complement, 13-bit integer to floating point. |
| sh2fl_12bit | conversion of two&#39;s complement, 12-bit integer to floating point. |
| sh2fl | generic function for conversion from integer to floating point. |
| sh2fl_alt | alternate (faster) implementation of sh2fl, with compulsory range conversion. |
| fl2sh_16bit | conversion of floating point data to two&#39;s complement, 16‑bit integer. |
| fl2sh_15bit | conversion of floating point data to two&#39;s complement, 15‑bit integer. |
| fl2sh_14bit | conversion of floating point data to two&#39;s complement, 14‑bit integer. |
| fl2sh_13bit | conversion of floating point data to two&#39;s complement, 13‑bit integer. |
| fl2sh_12bit | conversion of floating point data to two&#39;s complement, 12‑bit integer. |
| fl2sh | generic function for conversion from floating point to integer. |
| serialize_left_justified | serialization for left-justified data. |
| serialize_right_justified | serialization for right-justified data. |
| parallelize_left_justified | parallelization for left-justified data. |
| parallelize_right_justified | parallelization for right-justified data. |

## k) G.722 module

Name: `g722.c`

Associated header file: `g722.h`

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| G722_encode | ITU‑T G.722 wideband speech encoder at 64 kbit/s. |
| G722_decode | ITU‑T G.722 wideband speech decoder at 64, 56 and 48 kbit/s. |
| g722_reset_encoder | initialization of the ITU‑T G.722 encoder state variable. |
| g722_reset_decoder | initialization of the ITU‑T G.722 decoder state variable. |

## l) RPE-LTP module

Name: `rpeltp.c`

Associated header file: `rpeltp.h`

Functions included:

|||
|---------------|----------------------------------------------------------------------|
| rpeltp_encode | GSM 06.10 full-rate RPE-LTP speech encoder at 13 kbit/s. |
| rpeltp_decode | GSM 06.10 full-rate RPE-LTP speech decoder at 13 kbit/s. |
| rpeltp_init   | initialize memory for the RPE-LTP state variables. |
| rpeltp_delete | release memory previously allocated for the RPE-LTP state variables. |

## m) G.727 module

Name: `g727.c`

Associated header file: `g727.h`

Functions included:

|||
|-------------|-----------------------------------------------------------|
| G727_encode | ITU‑T G.727 encoder at 40, 32, 24 and 16 kbit/s. |
| G727_decode | ITU‑T G.727 decoder at 40, 32, 24 and 16 kbit/s. |

## n) Basic Operators

Name: `basop32.c`, `enh1632.c`, `enh40.c`

Associated header file: `stl.h`

Variable definitions:

• v1, v2: 16-bit variables

• L_v1, L_v2, L_v3: 32-bit variables

• L40_v1, L40_v2, L40_v3: 40-bit variables

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| add(v1, v2) | Performs the addition (v1+v2) with overflow control and saturation; the 16-bit result is set at +32767 when overflow occurs or at –32768 when underflow occurs. |
| sub(v1, v2) | Performs the subtraction (v1–v2) with overflow control and saturation; the 16-bit result is set at +32767 when overflow occurs or at –32768 when underflow occurs. |
| abs_s(v1) | Absolute value of v1. If v1 is –32768, returns 32767. |
| shl(v1, v2) | Arithmetically shifts the 16-bit input v1 left by v2 positions. Zero fills the v2 LSB of the result. If v2 is negative, arithmetically shifts v1 right by –v2 with sign extension. Saturates the result in case of underflows or overflows. |
| shr(v1, v2) | Arithmetically shifts the 16-bit input v1 right v2 positions with sign extension. If v2 is negative, arithmetically shifts v1 left by –v2 and zero fills the –v2 LSB of the result:<br/>`shr(v1, v2) = shl(v1, –v2)`<br/> Saturates the result in case of underflows or overflows. |
| negate(v1) | Negates v1 with saturation, saturate in the case when input is –32768:<br/>`negate(v1) = sub(0, v1)` |`
| s_max(v1, v2) | Compares two 16-bit variables v1 and v2 and returns the maximum value. |
| s_min(v1, v2) | Compares two 16-bit variables v1 and v2 and returns the minimum value. |
| norm_s(v1) | Produces the number of left shifts needed to normalize the 16-bit variable v1 for positive values on the interval with minimum of 16384 and maximum 32767, and for negative values on the interval with minimum of –32768 and maximum of –16384; in order to normalize the result, the following operation must be done:<br/>`norm_v1 = shl(v1, norm_s(v1))` |
| L_add(L_v1, L_v2) | 32-bit addition of the two 32-bit variables (L_v1+L_v2) with overflow control and saturation; the result is set at +2147483647 when overflow occurs or at –2147483648 when underflow occurs. |
| L_sub(L_v1, L_v2) | 32-bit subtraction of the two 32-bit variables (L_v1–L_v2) with overflow control and saturation; the result is set at +2147483647 when overflow occurs or at –2147483648 when underflow occurs. |
| L_abs(L_v1) | Absolute value of L_v1, with L_abs(–2147483648) = 2147483647. |
| L_shl(L_v1, v2) | Arithmetically shifts the 32-bit input L_v1 left v2 positions. Zero fills the v2 LSB of the result. If v2 is negative, arithmetically shifts L_v1 right by –v2 with sign extension. Saturates the result in case of underflows or overflows. |
| L_shr(L_v1, v2) | Arithmetically shifts the 32-bit input L_v1 right v2 positions with sign extension. If v2 is negative, arithmetically shifts L_v1 left by –v2 and zero fills the –v2 LSB of the result. Saturates the result in case of underflows or overflows. |
| L_negate(L_v1) | Negates the 32-bit L_v1 with saturation, saturate in the case where input is –2147483648. |
| L_max(L_v1, L_v2) | Compares two 32-bit variables L_v1 and L_v2 and returns the maximum value. |
| L_min(L_v1, L_v2) | Compares two 32-bit variables L_v1 and L_v2 and returns the minimum value. |
| norm_l(L_v1) | Produces the number of left shifts needed to normalize the 32-bit variable L_v1 for positive values on the interval with minimum of 1073741824 and maximum 2147483647, and for negative values on the interval with minimum of –2147483648 and maximum of –1073741824; in order to normalize the result, the following operation must be done:<br/>`L_norm_v1 = L_shl( L_v1, norm_l( L_v1))` |
| L_mult(v1, v2) | L_mult implements the 32-bit result of the multiplication of v1 times v2 with one shift left, i.e.,<br/>`L_mult(v1, v2) = L_shl( (v1 \* v2), 1)` <br/>Note that L_mult(–32768,–32768) = 2147483647. |
| L_mult0(v1, v2) | L_mult0 implements the 32-bit result of the multiplication of v1 times v2 without left shift, i.e.,<br/>`L_mult(v1, v2) = (v1 \* v2)`|
| mult(v1, v2) | Performs the multiplication of v1 by v2 and gives a 16-bit result which is scaled, i.e.,<br/>`mult(v1, v2) = extract_l( L_shr( (v1 times v2), 15))`<br/>Note that mult(–32768,–32768) = 32767. |
| mult_r(v1, v2) | Same as mult() but with rounding, i.e.,<br/>`mult_r( v1, v2) = extract_l( L_shr( ((v1 \* v2) + 16384), 15))` <br/>and<br/>`mult_r(–32768, –32768) = 32767.` |
| L_mac(L_v3, v1, v2) | Multiplies v1 by v2 and shifts the result left by 1. Adds the 32-bit result to L_v3 with saturation, returns a 32-bit result:<br/>`L_mac( L_v3, v1, v2) = L_add(L_v3, L_mult( v1, v2))`|
| L_mac0(L_v3, v1, v2) | Multiplies v1 by v2 without left shift. Adds the 32-bit result to L_v3 with saturation, returning a 32-bit result:<br/>`L_mac(L_v3, v1, v2) = L_add(vL_v3, L_mult0(vv1, v2))`|
| L_macNs(L_v3, v1, v2) | Multiplies v1 by v2 and shifts the result left by 1. Adds the 32-bit result to L_v3 without saturation, returns a 32-bit result. Generates carry and overflow values:<br/> `L_macNs( L_v3, v1, v2) = L_add_c( L_v3, L_mult(v1, v2))` |
| mac_r(L_v3, v1, v2) | Multiplies v1 by v2 and shifts the result left by 1. Adds the 32-bit result to L_v3 with saturation. Rounds the 16 least significant bits of the result into the 16 most significant bits with saturation and shifts the result right by 16. Returns a 16‑bit result.<br/> `mac_r( L_v3, v1, v2) = round( L_mac( L_v3, v1, v2)) = extract_h( L_add( L_add( L_v3, L_mult( v1, v2)), 32768))` |
| L_msu(L_v3, v1, v2) | Multiplies v1 by v2 and shifts the result left by 1. Subtracts the 32-bit result from L_v3 with saturation, returns a 32-bit result:<br/>`L_msu(L_v3, v1, v2) = L_sub(L_v3, L_mult(v1, v2))`  |
| L_msu0(L_v3, v1, v2) | Multiplies v1 by v2 without left shift. Subtracts the 32-bit result from L_v3 with saturation, returning a 32-bit result:<br/>`L_msu(L_v3, v1, v2) = L_sub(L_v3, L_mult0( v1, v2))` |
| L_msuNs(L_v3, v1, v2) | Multiplies v1 by v2 and shifts the result left by 1. Subtracts the 32-bit result from L_v3 without saturation, returns a 32‑bit result. Generates carry and overflow values:<br/>`L_msuNs( L_v3, v1, v2) = L_sub_c(L_v3, L_mult( v1, v2))` |
| msu_r(L_v3, v1, v2) | Multiplies v1 by v2 and shifts the result left by 1. Subtracts the 32-bit result from L_v3 with saturation. Rounds the 16 least significant bits of the result into the 16 bits with saturation and shifts the result right by 16. Returns a 16-bit result.<br/>`msu_r( L_v3, v1, v2) = round( L_msu( L_v3, v1, v2)) = extract_h( L_add( L_sub( L_v3, L_mult( v1, v2)), 32768))` |
| s_and(v1, v2) | Performs a bit wise AND between the two 16-bit variables v1 and v2. |
| s_or(v1, v2) | Performs a bit wise OR between the two 16-bit variables v1 and v2. |
| s_xor(v1, v2) | Performs a bit wise XOR between the two 16-bit variables v1 and v2. |
| lshl(v1, v2) | Logically shifts left the 16-bit variable v1 by v2 positions:<br/>if v2 is negative, v1 is shifted to the least significant bits by (–v2) positions with insertion of 0 at the most significant bit.<br/>if v2 is positive, v1 is shifted to the most significant bits by (v2) positions without saturation control. |
| lshr(v1, v2) | Logically shifts right the 16-bit variable v1 by v2 positions:<br/>if v2 is positive, v1 is shifted to the least significant bits by (v2) positions with insertion of 0 at the most significant bit.<br/>if v2 is negative, v1 is shifted to the most significant bits by (–v2) positions without saturation control. |
| L_and(L_v1, L_v2) | Performs a bit wise AND between the two 32-bit variables L_v1 and L_v2. |
| L_or(L_v1, L_v2) | Performs a bit wise OR between the two 32-bit variables L_v1 and L_v2. |
| L_xor(L_v1, L_v2) | Performs a bit wise XOR between the two 32-bit variables L_v1 and L_v2. |
| L_lshl(L_v1, v2) | Logically shifts left the 32-bit variable L_v1 by v2 positions:<br/>if v2 is negative, L_v1 is shifted to the least significant bits by (–v2) positions with insertion of 0 at the most significant bit.<br/>if v2 is positive, L_v1 is shifted to the most significant bits by (v2) positions without saturation control. |
| L_lshr(L_v1, v2) | Logically shifts right the 32-bit variable L_v1 by v2 positions:<br/>if v2 is positive, L_v1 is shifted to the least significant bits by (v2) positions with insertion of 0 at the most significant bit.<br/>if v2 is negative, L_v1 is shifted to the most significant bits by (–v2) positions without saturation control. |
| extract_h(L_v1) | Returns the 16 most significant bits of L_v1. |
| extract_l(L_v1) | Returns the 16 least significant bits of L_v1. |
| round(L_v1) | Rounds the lower 16 bits of the 32-bit input number into the most significant 16 bits with saturation. Shifts the resulting bits right by 16 and returns the 16-bit number:<br/>`round(L_v1) = extract_h( L_add( L_v1, 32768))` |
| L_deposit_h(v1) | Deposits the 16-bit v1 into the 16-bit most significant bits of the 32‑bit output. The 16 least significant bits of the output are zeroed. |
| L_deposit_l(v1) | Deposits the 16-bit v1 into the 16-bit least significant bits of the 32‑bit output. The 16 most significant bits of the output are sign extended. |
| L_add_c(L_v1, L_v2) | Performs the 32-bit addition with carry. No saturation. Generates carry and overflow values. The carry and overflow values are binary variables which can be tested and assigned values. |
| L_sub_c(L_v1, L_v2) | Performs the 32-bit subtraction with carry (borrow). Generates carry (borrow) and overflow values. No saturation. The carry and overflow values are binary variables which can be tested and assigned values. |
| shr_r(v1, v2) | Same as shr( v1, v2) but with rounding. Saturates the result in case of underflows or overflows.<br/>if v2 is strictly greater than zero, then `if (sub( shl ( shr( v1,v2), 1), shr( v1, sub(v2, 1))) == 0)`<br/>`then shr_r(v1, v2) = shr(v1, v2)`<br/>`else shr_r(v1, v2) = add(shr(v1, v2), 1)` <br/>On the other hand, if v2 is lower than or equal to zero, then<br/>`shr_r(v1, v2) = shr(v1, v2)` |
| shl_r(v1, v2) | Same as shl( v1, v2) but with rounding. Saturates the result in case of underflows or overflows:<br/>`shl_r( v1, v2) = shr_r( v1, –v2)`<br/>In the previous version of the STL-basic operators, this operator is called shift_r(v1, v2); both names can be used. |
| L_shr_r(L_v1, v2) | Same as L_shr( v1, v2) but with rounding. Saturates the result in case of underflows or overflows:<br/> if v2 is strictly greater than zero, then<br/> `if( L_sub( L_shl( L_shr( L_v1, v2), 1), L_shr( L_v1, sub( v2, 1)))) == 0`<br/>`then L_shr_r( L_v1, v2) = L_shr( L_v1, v2)`<br/> `else L_shr_r( L_v1, v2) = L_add( L_shr( L_v1, v2), 1)`<br/>On the other hand, if v2 is less than or equal to zero, then<br/>`L_shr_r( L_v1, v2) = L_shr( L_v1, v2)` |
| L_shl_r(L_v1, v2) | Same as L_shl( L_v1, v2) but with rounding. Saturates the result in case of underflows or overflows.<br/>`L_shift_r( L_v1, v2) = L_shr_r( L_v1, –v2)`<br/>In the previous version of the STL-basic operators, this operator is called L_shift_r(L_v1, v2); both names can be used. |
| i_mult(v1, v2) | Multiplies two 16-bit variables v1 and v2 returning a 16-bit value with overflow control. |
| rotl(v1, v2, \*v3) | Rotates the 16-bit variable v1 by 1 bit to the most significant bits. Bit 0 of v2 is copied to the least significant bit of the result before it is returned. The most significant bit of v1 is copied to the bit 0 of v3 variable. |
| rotr(v1, v2, \*v3) | Rotates the 16-bit variable v1 by 1 bit to the least significant bits. Bit 0 of v2 is copied to the most significant bit of the result before it is returned. The least significant bit of v1 is copied to the bit 0 of v3 variable. |
| L_rotl(L_v1, v2, \*v3) | Rotates the 32-bit variable L_v1 by 1 bit to the most significant bits. Bit 0 of v2 is copied to the least significant bit of the result before it is returned. The most significant bit of L_v1 is copied to the bit 0 of v3 variable. |
| L_rotr(L_v1, v2, \*v3) | Rotates the 32-bit variable L_v1 by 1 bit to the least significant bits. Bit 0 of v2 is copied to the most significant bit of the result before it is returned. The least significant bit of L_v1 is copied to the bit 0 of v3 variable. |
| L_sat(L_v1) | Long (32-bit) L_v1 is set to 2147483647 if an overflow occurred, or –2147483648 if an underflow occurred, on the most recent L_add_c(), L_sub_c(), L_macNs() or L_msuNs() operations. The carry and overflow values are binary variables which can be tested and assigned values. |
| L_mls(L_v1, v2) | Performs a multiplication of a 32-bit variable L_v1 by a 16‑bit variable v2, returning a 32-bit value. |
| div_s(v1, v2) | Produces a result which is the fractional integer division of v1 by v2. Values in v1 and v2 must be positive and v2 must be greater than or equal to v1. The result is positive (leading bit equal to 0) and truncated to 16 bits. If v1 equals v2, then div( v1, v2) = 32767. |
| div_l(L_v1, v2) | Produces a result which is the fractional integer division of a positive 32-bit variable L_v1 by a positive 16-bit variable v2. The result is positive (leading bit equal to 0) and truncated to 16 bits. |
| L40_add(L40_v1, L40_v2) | Adds the two 40-bit variables L40_v1 and L40_v2 without saturation control on 40 bits. Any detected overflow on 40 bits will exit execution. |
| L40_sub(L40_v1, L40_v2) | Subtracts the two 40-bit variables L40_v2 from L40_v1 without saturation control on 40 bits. Any detected overflow on 40 bits will exit execution. |
| L40_abs(L40_v1) | Returns the absolute value of the 40-bit variable L40_v1 without saturation control on 40 bits. Any detected overflow on 40 bits will exit execution. |
| L40_shl(L40_v1, v2) | Arithmetically shifts left the 40-bit variable L40_v1 by v2 positions:<br/>if v2 is negative, L40_v1 is shifted to the least significant bits by (–v2) positions with extension of the sign bit.<br/>if v2 is positive, L40_v1 is shifted to the most significant bits by (v2) positions without saturation control on 40 bits. Any detected overflow on 40 bits will exit execution. |
| L40_shr(L40_v1, v2) | Arithmetically shifts right the 40-bit variable L40_v1 by v2 positions:<br/>if v2 is positive, L40_v1 is shifted to the least significant bits by (v2) positions with extension of the sign bit.<br/>if v2 is negative, L40_v1 is shifted to the most significant bits by (–v2) positions without saturation control on 40 bits. Any detected overflow on 40 bits will exit execution. |
| L40_negate(L40_v1) | Negates the 40-bit variable L40_v1 without saturation control on 40 bits. Any detected overflow on 40 bits will exit execution. |
| L40_max(L40_v1, L40_v2) | Compares two 40-bit variables L40_v1 and L40_v2 and returns the maximum value. |
| L40_min(L40_v1, L40_v2) | Compares two 40-bit variables L40_v1 and L40_v2 and returns the minimum value. |
| norm_L40(L40_v1) | Produces the number of left shifts needed to normalize the 40-bit variable L40_v1 for positive values on the interval with minimum of 1073741824 and maximum 2147483647, and for negative values on the interval with minimum of –2147483648 and maximum of –1073741824; in order to normalize the result, the following operation must be done:<br/> `L40_norm_v1 = L40_shl( L40_v1, norm_L40( L40_v1))` |
| L40_mult(v1, v2) | Multiplies the 2 signed 16-bit variables v1 and v2 without saturation control on 40 bits. Any detected overflow on 40 bits will exit execution.<br/>The operation is performed in fractional mode:<br/>v1 and v2 are supposed to be in 1Q15 format.<br/>The result is produced in 9Q31 format. |
| L40_mac(L40_v1, v2, v3) | Equivalent to:<br/>`L40_add( L40_v1, L40_mult( v2, v3))` |
| L40_msu(L40_v1, v2, v3) | Equivalent to:<br/>`L40_sub( L40_v1, L40_mult( v2, v3))` |
| L40_lshl(L40_v1, v2) | Logically shifts left the 40-bit variable L40_v1 by v2 positions:<br/>if v2 is negative, L40_v1 is shifted to the least significant bits by (–v2) positions with insertion of 0 at the most significant bit.<br/>if v2 is positive, L40_v1 is shifted to the most significant bits by (v2) positions without saturation control. |
| L40_lshr(L40_v1, v2) | Logically shifts right the 40-bit variable L40_v1 by v2 positions:<br/>if v2 is positive, L40_v1 is shifted to the least significant bits by (v2) positions with insertion of 0 at the most significant bit.<br/>if v2 is negative, L40_v1 is shifted to the most significant bits by (–v2) positions without saturation control. |
| Extract40_H(L40_v1) | Returns the bits [31..16] of L40_v1. |
| Extract40_L(L40_v1) | Returns the bits [15..00] of L40_v1. |
| round40(L40_v1) | Equivalent to:<br/>`extract_h( L_saturate40( L40_round( L40_v1)))` |
| L_Extract40(L40_v1) | Returns the bits [31..00] of L40_v1. |
| L_saturate40(L40_v1) | If L40_v1 is greater than 2147483647,returns 2147483647.<br/>If L40_v1 is lower than –2147483648,returns –2147483648.<br/>If not,equivalent to: `L_Extract40( L40_v1)` |
| L40_deposit_h(v1) | Deposits the 16-bit variable v1 in the bits [31..16] of the return value: the return value bits [15..0] are set to 0 and the bits [39..32] sign extend v1 sign bit. |
| L40_deposit_l(v1) | Deposits the 16-bit variable v1 in the bits [15..0] of the return value: the return value bits [39..16] sign extend v1 sign bit. |
| L40_deposit32(L_v1) | Deposits the 32-bit variable L_v1 in the bits [31..0] of the return value: the return value bits [39..32] sign extend L_v1 sign bit. |
| L40_round(L40_v1) | Performs a rounding to the infinite on the 40-bit variable L40_v1.32768 is added to L40_v1 without saturation control on 40 bits. Any detected overflow on 40 bits will exit execution.The end-result 16 LSB are cleared to 0. |
| mac_r40(L40_v1, v2, v3) | Equivalent to:<br/>`round40( L40_mac( L40_v1, v2, v3))` |
| msu_r40(L40_v1, v2, v3) | Equivalent to:<br/>`round40( L40_msu( L40_v1, v2, v3))` |
| Mpy_32_16_ss(L_v1, v2, \*L_v3_h, \*v3_l) | Multiplies the 2 signed values L_v1 (32-bit) and v2 (16-bit) with saturation control on 48 bits.<br/>The operation is performed in fractional mode:<br/>When L_v1 is in 1Q31 format, and v2 is in 1Q15 format, the result is produced in 1Q47 format: L_v3_h bears the 32 most significant bits while v3_l bears the 16 least significant bits. |
| L40_shr_r(L40_v1, v2) | Arithmetically shifts the 40-bit variable L40_v1 by v2 positions to the least significant bits and rounds the result.<br/>It is equivalent to L40_shr( L40_v1, v2) except that if v2 is positive and the last shifted out bit is 1, then the shifted result is incremented by 1 without saturation control on 40 bits.<br/>Any detected overflow on 40 bits will exit execution. |
| L40_shl_r(L40_v1, v2) | Arithmetically shifts the 40-bit variable L40_v1 by v2 positions to the most significant bits and rounds the result.<br/>It is equivalent to L40_shl( L40_var1, v2) except if v2 is negative.In this case, it does the same as L40_shr_r( L40_v1, (–v2)). |
| L40_set(L40_v1) | Assigns a 40-bit constant to the returned 40-bit variable. |
| Mpy_32_32_ss(L_v1, L_v2, \*L_v3_h, \*L_v3_l) | Multiplies the 2 signed 32-bit values L_v1 and L_v2 with saturation control on 64 bits.<br/>The operation is performed in fractional mode:<br/>When L_v1 and L_v2 are in 1Q31 format, the result is produced in 1Q63 format: L_v3_h bears the 32 most significant bits while L_v3_l bears the 32 least significant bits. |

The Basic Operators module is supplemented by two tools: one to evaluate program ROM complexity for fixed-point code, and another to evaluate complexity (including program ROM) of floating-point implementations.

### n.1) Program ROM estimation tool for fixed-point C code

Name: `basop_cnt.c`

Associated header file: None.

Usage: `basop cnt input.c [result_file_name.txt]`

The basop_cnt tool estimates the program ROM of applications written using the ITU-T Basic Operator libraries. It counts the number of calls to basic operators in the input C source file, and also the number of calls to user‑defined functions. The sum of these two numbers gives an estimation of the required PROM.

### n.2) Complexity evaluation tool for floating-point C code

Name: `flc.c`

Associated header file: `flc.h`

Functions included:

|||
|-----------|--------------------------------------------------------------|
| FLC_init | Initialize the floating-point counters. |
| FLC_sub_start | Marks the start of a subroutine/subsection. |
| FLC_sub_end | Marks the end of a subroutine/subsection. |
| FLC_end | Computes and prints complexity, i.e., floating-point counter results. |
| FLC_frame_update | Marks the end of a frame processing to keep track of the per-frame maxima. |

## o) Reverberation module

Name: `reverb-lib.c`

Associated header file: `reverb-lib.h`

Functions included:

|||
| --- | --- |
| conv | Convolution routine. |
| shift | Shift elements of a vector for the block-based convolution. |

## p) Bit stream truncation module

Name: `trunc-lib.c`

Associated header file: `trunc-lib.h`

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| trunc | Frame truncation routine. |

## q) Frequency response calculation module

Name: `fft.c`

Associated header file: `fft.h`

Functions included:

|||
|---------------------|-----------------------------------------------------------|
| rdft | Discrete Fourier Transform for real signals. |
| genHanning | Hanning window generation routine. |
| powSpect | Power spectrum computation routine. |
