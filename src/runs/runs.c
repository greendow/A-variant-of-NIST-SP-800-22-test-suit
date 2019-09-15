#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/errorCode.h"
#include "../../include/basicDefinition.h"
#include "../../include/cephes.h"
#include "../../include/runs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                              R U N S  T E S T 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int Runs(const unsigned char *bitArray, int bitArrayLen)
{
	int S, k;
	double pi, V, erfc_arg, p_value;

#ifdef _DEBUG
	fprintf(stdout, "\t\t\t\tRUNS TEST\n");
	fprintf(stdout, "\t\t------------------------------------------\n");
#endif

	S = 0;
	for ( k = 0; k < bitArrayLen; k++ )
	{
		if ( bitArray[k] )
		{
			S++;
		}
	}
	pi = (double)S / (double)bitArrayLen;

	if ( fabs(pi - (double)(0.5)) >= ((double)(2.0) / sqrt((double)bitArrayLen)) )
	{
#ifdef _DEBUG
		fprintf(stdout, "\t\tPI ESTIMATOR CRITERIA NOT MET! PI = %f\n", pi);
#endif
		return RUNS_TEST_FAIL;
	}
	else
	{
		V = 1;
		for ( k = 1; k < bitArrayLen; k++ )
		{
			if ( bitArray[k] != bitArray[k-1] )
			{
				V++;
			}
		}
	
		erfc_arg = fabs(V - 2.0 * bitArrayLen * pi * (1.0 - pi)) / (2.0 * pi * (1.0 - pi) * sqrt(2.0 * (double)(bitArrayLen)));
		p_value = cephes_erfc(erfc_arg);
#ifdef _DEBUG
		fprintf(stdout, "\t\tCOMPUTATIONAL INFORMATION:\n");
		fprintf(stdout, "\t\t------------------------------------------\n");
		fprintf(stdout, "\t\t(a) Pi                        = %f\n", pi);
		fprintf(stdout, "\t\t(b) V_n_obs (Total # of runs) = %d\n", (int)V);
		fprintf(stdout, "\t\t(c) V_n_obs - 2 n pi (1-pi)\n");
		fprintf(stdout, "\t\t    -----------------------   = %f\n", erfc_arg);
		fprintf(stdout, "\t\t      2 sqrt(2n) pi (1-pi)\n");
		fprintf(stdout, "\t\t------------------------------------------\n");
#endif
		if ( isNegative(p_value) || isGreaterThanOne(p_value) )
		{
#ifdef _DEBUG
			fprintf(stdout, "\n\t\tp_value = %f\n", p_value);
			fprintf(stdout, "WARNING:  P_VALUE IS OUT OF RANGE.\n");
#endif
			return P_VALUE_OUT_OF_RANGE;
		}
#ifdef _DEBUG
		fprintf(stdout, "%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
#endif
	}

	if ( p_value < ALPHA )
	{
		return RUNS_TEST_FAIL;
	}
	else
	{
		return 0;
	}
}
