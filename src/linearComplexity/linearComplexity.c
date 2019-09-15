#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/errorCode.h"
#include "../../include/basicDefinition.h"
#include "../../include/cephes.h"
#include "../../include/linearComplexity.h" 

int LinearComplexity(const unsigned char *bitArray,
                     int bitArrayLen,
					 int blockLen)
{
	const int K = 6;
	int i, ii, j, d, blockCount, L, m, N_, parity, sign;
	double p_value, T_, mean, nu[7], chi2;
	double pi[7] = { 0.010417, 0.03125, 0.12500, 0.50000, 0.25000, 0.06250, 0.020833 };
	unsigned char *T = NULL, *P = NULL, *B_ = NULL, *C = NULL;

#ifdef _DEBUG
	fprintf(stdout, "\t\t\t  LINEAR COMPLEXITY TEST\n");
	fprintf(stdout, "\t\t---------------------------------------------\n");
#endif

	if ( bitArrayLen < 1000000 )
	{
#ifdef _DEBUG
		fprintf(stdout, "\t\t   n = %d is too short\n", bitArrayLen);
#endif
		return INPUT_LENGTH_TOO_SHORT;
	}

	if ( (blockLen < 500) || ( blockLen > 5000) )
    {
#ifdef _DEBUG
        fprintf(stdout, "\t\tLinear complexity block length = %d\n", blockLen);
        fprintf(stdout, "\t\tThe parameter value is invalid!\n");
#endif
        return INPUT_INVALID_PARAMETER;
    }
	
	blockCount = (int)floor((double)(bitArrayLen) / blockLen);
	if ( ((B_ = (unsigned char *) calloc(blockLen, sizeof(unsigned char))) == NULL) ||
		 ((C  = (unsigned char *) calloc(blockLen, sizeof(unsigned char))) == NULL) ||
		 ((P  = (unsigned char *) calloc(blockLen, sizeof(unsigned char))) == NULL) ||
		 ((T  = (unsigned char *) calloc(blockLen, sizeof(unsigned char))) == NULL) )
	{
#ifdef _DEBUG
		fprintf(stdout, "Insufficient Memory for Work Space.\n");
#endif
		if ( B_ != NULL )
		{
			free(B_);
		}
		if ( C != NULL )
		{
			free(C);
		}
		if ( P != NULL )
		{
			free(P);
		}
		if ( T != NULL )
		{
			free(T);
		}
		return MEMORY_ALLOCATION_FAIL;
	}

#ifdef _DEBUG
	fprintf(stdout, "\t\t\tM (substring length)     = %d\n", blockLen);
	fprintf(stdout, "\t\t\tN (number of substrings) = %d\n", blockCount);
	fprintf(stdout, "\t\t-----------------------------------------------------\n");
	fprintf(stdout, "\t\t        F R E Q U E N C Y                            \n");
	fprintf(stdout, "\t\t-----------------------------------------------------\n");
	fprintf(stdout, "\t\t  C0   C1   C2   C3   C4   C5   C6    CHI2    P-value\n");
	fprintf(stdout, "\t\t-----------------------------------------------------\n");
	fprintf(stdout, "\t\t\tNote: %d bits were discarded!\n", bitArrayLen % blockLen);
#endif

	for ( i = 0; i < (K + 1); i++ )
	{
		nu[i] = 0.00;
	}
	for ( ii = 0; ii < blockCount; ii++ )
	{
		for ( i = 0; i < blockLen; i++ )
		{
			B_[i] = 0;
			C[i] = 0;
			T[i] = 0;
			P[i] = 0;
		}
		L = 0;
		m = -1;
		d = 0;
		C[0] = 1;
		B_[0] = 1;
		
		/* DETERMINE LINEAR COMPLEXITY */
		N_ = 0;
		while ( N_ < blockLen )
		{
			d = (int)bitArray[ii * blockLen + N_];
			for ( i = 1; i <= L; i++ )
			{
				d += C[i] * bitArray[ii * blockLen + N_ - i];
			}
			d = d % 2;
			if ( d == 1 ) {
				for ( i = 0; i < blockLen; i++ )
				{
					T[i] = C[i];
					P[i] = 0;
				}
				for ( j = 0; j < blockLen; j++ )
				{
					if ( B_[j] == 1 )
					{
						P[j+N_-m] = 1;
					}
				}
				for ( i = 0; i < blockLen; i++ )
				{
					C[i] = (C[i] + P[i]) % 2;
				}
				if ( L <= N_/2 )
				{
					L = N_ + 1 - L;
					m = N_;
					for ( i = 0; i < blockLen; i++ )
					{
						B_[i] = T[i];
					}
				}
			}
			N_++;
		}

		if ( (parity = (blockLen + 1) % 2) == 0 )
		{ 
			sign = 1;
		}
		else
		{
			sign = -1;
		}
		mean = blockLen / 2.0 + (9.0 + sign) / 36.0 - 1.0 / pow(2.0, blockLen) * (blockLen / 3.0 + 2.0 / 9.0);
		if ( (parity = blockLen % 2) == 0 )
		{
			sign = 1;
		}
		else
		{ 
			sign = -1;
		}
		T_ = sign * (L - mean) + 2.0 / 9.0;
		
		if ( T_ <= -2.5 )
		{
			nu[0]++;
		}
		else
		{
			if ( T_ <= -1.5 )
			{
				nu[1]++;
			}
			else
			{
				if ( T_ <= -0.5 )
				{
					nu[2]++;
				}
				else
				{
					if ( T_ <= 0.5 )
					{
						nu[3]++;
					}
					else
					{
						if ( T_ <= 1.5 )
						{
							nu[4]++;
						}
						else
						{
							if ( T_ <= 2.5 )
							{
								nu[5]++;
							}
							else
							{
								nu[6]++;
							}
						}
					}
				}
			}
		}
	}

#ifdef _DEBUG
    fprintf(stdout, "\t\t");
#endif
	chi2 = 0.00;
	for ( i = 0; i < (K + 1); i++ )
	{
#ifdef _DEBUG
		fprintf(stdout, "%4d ", (int)nu[i]);
#endif
	}
	for ( i = 0; i < (K + 1); i++ )
	{
		chi2 += pow(nu[i] - blockCount * pi[i], 2.0) / (blockCount * pi[i]);
	}
	p_value = cephes_igamc(K / 2.0, chi2 / 2.0);

#ifdef _DEBUG
	fprintf(stdout, "%9.6f%9.6f\n", chi2, p_value);
	fprintf(stdout, "%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
#endif
	free(B_);
	free(P);
	free(C);
	free(T);

	if ( p_value < ALPHA )
	{
		return LINEAR_COMPLEXITY_TEST_FAIL;
	}
	else
	{
		return 0;
	}	
}
