#ifndef YOUR_FILTERING_ROUTINES_H_
#define YOUR_FILTERING_ROUTINES_H_

extern void your_filterFunc_IIR(const double *input, double *output, int length, const double *coeffs, int order, double *delay);
extern void your_filterFunc_FIR(const double *input, double *output, int length, const double *coeffs, int order, double *delay);

#endif /* !YOUR_FILTERING_ROUTINES_H_ */
