#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/errorCode.h"
#include "../../include/basicDefinition.h"
#include "../../include/cephes.h"
#include "../../include/dfft.h"
#include "../../include/discreteFourierTransform.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
         D I S C R E T E  F O U R I E R  T R A N S F O R M  T E S T 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int DiscreteFourierTransform(const unsigned char *bitArray, int bitArrayLen)
{
	double	p_value, upperBound, percentile, N_l, N_o, d, *X = NULL, *wsave = NULL, *m = NULL;
	int i, count, ifac[15];

#ifdef _DEBUG
	fprintf(stdout, "\t\t\t\tFFT TEST\n");
	fprintf(stdout, "\t\t---------------------------------------------\n");
#endif

	if ( bitArrayLen < 1000 )
	{
#ifdef _DEBUG
		fprintf(stdout, "\t\t   n=%d is too short\n", bitArrayLen);
#endif
		return INPUT_LENGTH_TOO_SHORT;
	}

	if ( ((X = (double *)calloc(bitArrayLen, sizeof(double))) == NULL) ||
		 ((wsave = (double *)calloc((2 * bitArrayLen), sizeof(double))) == NULL) ||
		 ((m = (double *)calloc((bitArrayLen / 2 + 1), sizeof(double))) == NULL) )
	{
#ifdef _DEBUG
	    fprintf(stdout,"\t\tUnable to allocate working arrays for the DFT.\n");
#endif
        if( X != NULL )
		{
			free(X);
		}
		if( wsave != NULL )
		{
			free(wsave);
		}
		if( m != NULL )
		{
			free(m);
		}
		return MEMORY_ALLOCATION_FAIL;
	}

	for ( i = 0; i < bitArrayLen; i++ )
	{
		X[i] = 2 * (int)bitArray[i] - 1;
	}
	
	__ogg_fdrffti(bitArrayLen, wsave, ifac);    /* INITIALIZE WORK ARRAYS */
	__ogg_fdrfftf(bitArrayLen, X, wsave, ifac);    /* APPLY FORWARD FFT */
	
	m[0] = sqrt(X[0] * X[0]);    /* COMPUTE MAGNITUDE */
	
	for ( i = 0; i < (bitArrayLen / 2); i++ )
	{
		m[i + 1] = sqrt(pow(X[2 * i + 1], (double)(2.0)) + pow(X[2 * i + 2], (double)(2.0)));
	}
	count = 0;    /* CONFIDENCE INTERVAL */
	upperBound = sqrt(2.995732274 * (double)(bitArrayLen));
	for ( i = 0; i < (bitArrayLen / 2); i++ )
	{
		if ( m[i] < upperBound )
		{
			count++;
		}
	}
	percentile = (double)count / (bitArrayLen / 2) * 100;
	N_l = (double) count;    /* number of peaks less than h = sqrt((double)(3.0) * n) */
	N_o = (double) 0.95 * bitArrayLen / 2.0;
	d = (N_l - N_o) / sqrt((double)(bitArrayLen) / 4.0 * 0.95 * 0.05);
	p_value = cephes_erfc(fabs(d) / 1.4142135624);
#ifdef _DEBUG
	fprintf(stdout, "\t\tCOMPUTATIONAL INFORMATION:\n");
	fprintf(stdout, "\t\t-------------------------------------------\n");
	fprintf(stdout, "\t\t(a) Percentile = %f\n", percentile);
	fprintf(stdout, "\t\t(b) N_l        = %f\n", N_l);
	fprintf(stdout, "\t\t(c) N_o        = %f\n", N_o);
	fprintf(stdout, "\t\t(d) d          = %f\n", d);
	fprintf(stdout, "\t\t-------------------------------------------\n");
	fprintf(stdout, "%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
#endif
	free(X);
	free(wsave);
	free(m);

	if ( p_value < ALPHA )
	{
		return DISCRETE_FOURIER_TRANSFORM_TEST_FAIL;
	}
	else
	{
		return 0;
	}
}
