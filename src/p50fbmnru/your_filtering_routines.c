#include <assert.h>
#include "your_filtering_routines.h"

void your_filterFunc_IIR(const double *input, double *output, int length, const double *coeffs, int order, double *delay)
{
    /* NOTE: Limitation of the implementation below */
    assert(order == 2);

    while (length-- > 0) {
        double in, out;

        in = *input++;

        out = in * coeffs[0] + delay[1];
        delay[1] = in * coeffs[1] - out * coeffs[4] + delay[0];
        delay[0] = in * coeffs[2] - out * coeffs[5];

        *output++ = out;
    }
}

void your_filterFunc_FIR(const double *input, double *output, int length, const double *coeffs, int order, double *delay)
{
    while (length-- > 0) {
        double in, out;
        int i;

        in = *input++;

        /* Numerator */
        for (i=order-2; i>=0; --i)
            delay[i+1] = delay[i];
        delay[0] = in;
        out = 0;
        for (i=0; i<order; ++i) {
            out += coeffs[i] * delay[i];
        }

        *output++ = out;
    }
}
