# gain_chk — gain amplification verification

## Copyright

This software is protected by copyright law and by international treaties. The source code, and all of its derivations, is provided by VoiceAge Corporation under the "ITU-T Software Tools' General Public License". Please, read the license file or refer to ITU-T Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND AUDIO CODING STANDARDS". Any use of this software is permitted provided that this notice is not removed and that neither the authors nor VoiceAge Corporation are deemed to have made any representations as to the suitability of this software for any purpose nor are held responsible for any defects of this software. THERE IS NO WARRANTY FOR THIS SOFTWARE.

(C) 2026 copyright VoiceAge Corporation. All Rights Reserved.

## Description

`gain_chk` compares reference and processed 16-bit PCM streams in 20 ms frames,
performs short-time spectral analysis, and reports per-band energy ratios (dB
and percent). Optional VAD flags split statistics into active and inactive
frames. Optional threshold checks (`-a`, `-s`) return a numeric exit status for
scripting.

## Files

```
gain_chk.c ........................ main program
test_data/ ........................ regression PCM and CMake driver for CTest
Gain_Chk_Release_Notes.txt ....... revision notes (VoiceAge V3.0 history)
```

## Build

`gain_chk` is not built on its own: it is a target of the **top-level STL** CMake
project (see the repository root `README.md` for toolchain prerequisites).

From the **STL repository root** (the directory that contains the top-level
`CMakeLists.txt`):

1. **Configure** the build (example: out-of-tree build directory `build`):

   ```shell
   cmake -S . -B build
   ```

   You can use another build path or generator (for example `cmake -G Ninja -S . -B build`).

2. **Build only** the `gain_chk` executable:

   ```shell
   cmake --build build --target gain_chk
   ```

3. The binary is written to **`build/bin/gain_chk`** (the project sets
   `CMAKE_RUNTIME_OUTPUT_DIRECTORY` to `${CMAKE_BINARY_DIR}/bin`).

To build **all** STL targets, omit `--target gain_chk` and run
`cmake --build build` (or your chosen build directory).

## Tests (CTest)

CMake registers **five** tests in `src/gain_chk/CMakeLists.txt`:

| CTest name      | Reference (`-i`)   | Processed (`-o`)   | Golden stderr                         |
|-----------------|--------------------|--------------------|---------------------------------------|
| `gain_chk_self` | `input48c.pcm`     | `input48c.pcm`     | `test_data/gain_chk_stderr_self.ref`  |
| `gain_chk_a`    | `input48c.pcm`     | `syn.a.pcm`        | `test_data/gain_chk_stderr_a.ref`     |
| `gain_chk_b`    | `input48c.pcm`     | `syn.b.pcm`        | `test_data/gain_chk_stderr_b.ref`     |
| `gain_chk_c`    | `input48c.pcm`     | `syn.c.pcm`        | `test_data/gain_chk_stderr_c.ref`     |
| `gain_chk_d`    | `input48c.pcm`     | `syn.d.pcm`        | `test_data/gain_chk_stderr_d.ref`     |

Each test runs `gain_chk` at **48 kHz** (`-r 48000`), captures **stderr** to a scratch
file under the build tree, and compares it **byte-for-byte** to the matching `*.ref`
in `src/gain_chk/test_data/` (two-decimal band summary line plus the expected
end-of-file warning when the PCM length is an exact multiple of the frame size).

### Running the tests

All commands below assume your **current working directory is the STL repository
root** (the folder that contains the top-level `CMakeLists.txt`). The examples use
an out-of-tree build directory named **`build`**; use the same name everywhere, or
replace it with your own path.

**Full chain in one line** (configure, build `gain_chk`, run only the five
`gain_chk_*` tests):

```shell
cmake -S . -B build && cmake --build build --target gain_chk && ctest --test-dir build -R '^gain_chk_' --output-on-failure
```

**If the build tree is already configured**, rebuild the tool and run the same tests:

```shell
cmake --build build --target gain_chk && ctest --test-dir build -R '^gain_chk_' --output-on-failure
```

**Same steps split** (equivalent to the one-liner):

1. `cmake -S . -B build`
2. `cmake --build build --target gain_chk`
3. `ctest --test-dir build -R '^gain_chk_' --output-on-failure`

**If you prefer to run `ctest` from inside the build directory**:

```shell
cd build
ctest -R '^gain_chk_' --output-on-failure
```

**Verbose** output (shows the exact `cmake -P …/gain_chk_ctest.cmake` command line
for each test):

```shell
ctest --test-dir build -R '^gain_chk_' -V
```

`CTestTestfile.cmake` (and related CTest files) under **`build/`** are **generated
by CMake** from `add_test`; they are not edited by hand (same idea as
`src/truncate/`).

### Updating the golden `*.ref` files

If you change `gain_chk`, the floating-point path, or the PCM files under
`test_data/`, the stderr text may change. In that case, regenerate the corresponding
`gain_chk_stderr_*.ref` (for example by running `gain_chk` once per case, redirecting
stderr to a file, and replacing the ref) and commit the updated refs after you have
verified the new output is correct.

## Usage (summary)

```
gain_chk -i original_f -o processed_f -t results_f -r Fs [-v VAD_f] [-a val] [-s val]
```

- `-i` reference binary (16-bit samples, frame length implied by `Fs`)
- `-o` processed binary (same layout as reference)
- `-t` results text file (append mode)
- `-r` sampling frequency in Hz: 8000, 16000, 32000, or 48000
- `-v` optional VAD file (one 16-bit flag per frame)
- `-a` optional active-frame maximum amplification threshold (dB), requires build with `THRESHOLD_CHECK`
- `-s` optional inactive-frame maximum attenuation threshold (dB); requires `-v`

Frequency bands analysed are configured in the `bands[]` table in `gain_chk.c`
(terminate the list with `0.0f`; values in (0,1] are interpreted as fractions of
`Fs/2`).

## Formatting

C sources in this tree follow `FORMATTING.md` at the repository root (GNU
`indent` options; spaces instead of tab characters).
