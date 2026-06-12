       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
       CODING STANDARDS".
       =============================================================

# UGST P.56 Speech Voltmeter

Measures and equalizes active speech levels according to ITU-T Recommendation
P.56. Includes `sv56demo` (measure + equalize) and `actlev` (measure only).

# WAV file support

`sv56demo` transparently supports WAV files (8/16/24/32-bit PCM and 32-bit
IEEE float). The format is auto-detected from the RIFF header; raw PCM files
continue to work as before (assumed 16-bit, native byte order).

When a WAV file is detected:
- Bit depth and sample rate are read from the header
- The `-bits` flag overrides the A/D resolution used by the P.56 algorithm
  (must be ≤ file bit depth), simulating a lower-resolution ADC/DAC
- Output format matches the input (WAV output if filename ends in `.wav`)

## Benefits of higher bit depths

The P.56 speech voltmeter's ability to detect and measure speech depends on
the signal being above the quantization noise floor. Higher bit depths
provide more dynamic range:

| Bit depth | Dynamic range | Noise floor |
|-----------|--------------|-------------|
| 16-bit    | ~96 dB       | -96 dBov   |
| 24-bit    | ~144 dB      | -144 dBov  |
| 32-bit    | ~192 dB      | -192 dBov  |

### Test results: very quiet speech (-91 dBov)

| Resolution | Active level | Activity | Result |
|------------|-------------|----------|--------|
| 32-bit     | -91.029 dBov | 90.2%   | Speech correctly detected and measured |
| 16-bit ADC | —           | 0%       | **Signal below noise floor — undetectable** |

With 32-bit resolution, the speech voltmeter correctly measures a signal at
-91 dBov with 90% activity. With a 16-bit ADC simulation (`-bits 16`), the
same signal falls below the quantization threshold and the algorithm reports
zero activity — it cannot distinguish speech from silence.

For signals at normal levels (-26 dBov), all bit depths produce identical
measurements, confirming full backward compatibility.

# Source files

```
sv-p56.c ........ P.56 speech voltmeter module
sv-p56.h ........ prototypes and definitions for the SV module
sv56demo.c ...... demo program: measure active level and equalize
actlevel.c ...... demo program: measure level/min/max for multiple files
```

# Dependencies (from ../utl)

```
ugst-utl.c ...... float/short conversion and gain/loss functions
ugst-utl.h ...... prototypes for UGST utilities
ugstdemo.h ...... macros for UGST demo programs
wav_io.c ........ WAV file I/O (auto-detect, read, write)
wav_io.h ........ WAV I/O prototypes
```

# Test data

The `test_data/` directory contains:
- `voice.src` — raw 16-bit PCM test signal
- `voice.nrm`, `voice.ltl` — reference outputs for regression tests
- `speech_*.wav` — WAV test files at various bit depths and levels
- `*.ref`, `*.log.ref` — reference outputs for binary and text comparison
