#ifndef _DISCRETE_FOURIER_TRANSFORM_ALGORITHM_H_
#define _DISCRETE_FOURIER_TRANSFORM_ALGORITHM_H_

#ifdef  __cplusplus
extern "C" {
#endif

void __ogg_fdrffti(int n, double *wsave, int *ifac);
void __ogg_fdrfftf(int n, double *r, double *wsave, int *ifac);

#ifdef  __cplusplus
}
#endif

#endif /*  _DISCRETE_FOURIER_TRANSFORM_ALGORITHM_H_  */