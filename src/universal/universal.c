#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/errorCode.h"
#include "../../include/basicDefinition.h"
#include "../../include/cephes.h"
#include "../../include/universal.h" 

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                         U N I V E R S A L  T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int Universal(const unsigned char *bitArray, int bitArrayLen)
{
	int i, j, p, blockLen, Q, K;
	double arg, sqrt2, sigma, phi, sum, p_value, c;
	long *T, decRep;
	double expected_value[17] = { 0, 0, 0, 0, 0, 0, 5.2177052, 6.1962507, 7.1836656,
				8.1764248, 9.1723243, 10.170032, 11.168765,
				12.168070, 13.167693, 14.167488, 15.167379 };
	double variance[17] = { 0, 0, 0, 0, 0, 0, 2.954, 3.125, 3.238, 3.311, 3.356, 3.384,
				3.401, 3.410, 3.416, 3.419, 3.421 };
	
	/* * * * * * * * * ** * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * THE FOLLOWING REDEFINES blockLen, SHOULD THE CONDITION:                    *
	 *     bitArrayLen >= ( 1010 * pow((double)(2.0), blockLen) * blockLen )                *
	 * NOT BE MET, FOR THE BLOCK LENGTH blockLen.                                 *
	 * * * * * * * * * * ** * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	blockLen = 5;
	if ( bitArrayLen >= 387840 )
	{
		blockLen = 6;
		if ( bitArrayLen >= 904960 )
		{
			blockLen = 7;
			if ( bitArrayLen >= 2068480 )
			{
				blockLen = 8;
				if ( bitArrayLen >= 4654080 )
				{
					blockLen = 9;
					if ( bitArrayLen >= 10342400 )
					{
						blockLen = 10;
						if ( bitArrayLen >= 22753280 )
						{
							blockLen = 11;
							if ( bitArrayLen >= 49643520 )
							{
								blockLen = 12;
								if ( bitArrayLen >= 107560960 )
								{
									blockLen = 13;
									if ( bitArrayLen >= 231669760 )
									{
										blockLen = 14;
										if ( bitArrayLen >= 496435200 )
										{
											blockLen = 15;
											if ( bitArrayLen >= 1059061760 )
											{
												blockLen = 16;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		return INPUT_LENGTH_TOO_SHORT;
	}
	
	
	Q = 10 * (int)pow((double)(2.0), (double)(blockLen));
	K = (int)(floor(((double)(bitArrayLen) / blockLen)) - (double)Q);	 		    /* BLOCKS TO TEST */
	
	p = (int)pow((double)(2.0), (double)(blockLen));
	if ( (T = (long *)calloc(p, sizeof(long))) == NULL )
	{
		return MEMORY_ALLOCATION_FAIL;
	}

	/* COMPUTE THE EXPECTED:  Formula 16, in Marsaglia's Paper */
	c = 0.7 - 0.8 / (double)blockLen + (4 + 32 / (double)blockLen) * pow((double)(K), -3.0 / (double)blockLen) / 15;
	sigma = c * sqrt(variance[blockLen] / (double)K);
	sqrt2 = 1.4142135623731;
	sum = 0.0;
	for ( i = 0; i < p; i++ )
	{
		T[i] = 0;
	}

	for ( i = 1; i <= Q; i++ )
	{		/* INITIALIZE TABLE */
		decRep = 0;
		for ( j = 0; j < blockLen; j++ )
		{
			decRep += bitArray[(i - 1) * blockLen + j] * (long)pow((double)(2.0), (double)(blockLen - 1 - j));
		}
		T[decRep] = i;
	}

	for ( i = (Q + 1); i <= (Q + K); i++ )
	{ 	/* PROCESS BLOCKS */
		decRep = 0;
		for ( j = 0; j < blockLen; j++ )
		{
			decRep += bitArray[(i - 1) * blockLen + j] * (long)pow((double)(2.0), (double)(blockLen - 1 - j));
		}
		sum += log((double)(i - T[decRep])) / log(2.0);
		T[decRep] = i;
	}
	phi = (double)( sum / (double)K );

#ifdef _DEBUG
	fprintf(stdout, "\t\t\tUNIVERSAL STATISTICAL TEST\n");
	fprintf(stdout, "\t\t--------------------------------------------\n");
	fprintf(stdout, "\t\tCOMPUTATIONAL INFORMATION:\n");
	fprintf(stdout, "\t\t--------------------------------------------\n");
	fprintf(stdout, "\t\t(a) L         = %d\n", blockLen);
	fprintf(stdout, "\t\t(b) Q         = %d\n", Q);
	fprintf(stdout, "\t\t(c) K         = %d\n", K);
	fprintf(stdout, "\t\t(d) sum       = %f\n", sum);
	fprintf(stdout, "\t\t(e) sigma     = %f\n", sigma);
	fprintf(stdout, "\t\t(f) variance  = %f\n", variance[blockLen]);
	fprintf(stdout, "\t\t(g) exp_value = %f\n", expected_value[blockLen]);
	fprintf(stdout, "\t\t(h) phi       = %f\n", phi);
	fprintf(stdout, "\t\t(i) WARNING:  %d bits were discarded.\n", bitArrayLen - (Q + K) * blockLen);
	fprintf(stdout, "\t\t-----------------------------------------\n");
#endif

	arg = fabs(phi - expected_value[blockLen]) / (sqrt2 * sigma);
	p_value = cephes_erfc(arg);
	if ( isNegative(p_value) || isGreaterThanOne(p_value) )
	{
#ifdef _DEBUG
		fprintf(stdout, "\t\tWARNING:  P_VALUE IS OUT OF RANGE\n");
#endif
        free(T);
		return P_VALUE_OUT_OF_RANGE;
	}
#ifdef _DEBUG
	fprintf(stdout, "%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
#endif

	free(T);
	if ( p_value < ALPHA )
	{
		return UNIVERSAL_STATISTICAL_TEST_FAIL;
	}
	else
	{
		return 0;
	}
}