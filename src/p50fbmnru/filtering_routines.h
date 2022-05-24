#ifndef FILTERING_ROUTINES_H_
#define FILTERING_ROUTINES_H_

extern void filterFunc_IIR(const double *input, double *output, int length, const double *coeffs, int order, double *delay);
extern void filterFunc_FIR(const double *input, double *output, int length, const double *coeffs, int order, double *delay);

#endif /* !FILTERING_ROUTINES_H_ */
