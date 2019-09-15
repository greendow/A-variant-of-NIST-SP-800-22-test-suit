#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/errorCode.h"
#include "../../include/basicDefinition.h"
#include "../../include/cephes.h"
#include "../../include/approximateEntropy.h"  

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                A P P R O X I M A T E  E N T R O P Y   T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int ApproximateEntropy(const unsigned char *bitArray,
                       int bitArrayLen,
					   int blockLen)
{
	int i, j, index, r, powLen, blockLenUpperBound;
	double sum, ApEn[2], apen, chi_squared, p_value;
	unsigned int *counterArray;

#ifdef _DEBUG
	fprintf(stdout, "\t\t\tAPPROXIMATE ENTROPY TEST\n");
	fprintf(stdout, "\t\t--------------------------------------------\n");
#endif

	blockLenUpperBound = (int)(floor(log((double)bitArrayLen) / log((double)(2))) - 5);
	if ( (blockLen < 2) || (blockLen >=  blockLenUpperBound) )
	{
#ifdef _DEBUG
        fprintf(stdout, "\t\tNote: The blockSize = %d exceeds recommended value of %d\n",
		    blockLen, MAX(1, blockLenUpperBound));
		fprintf(stdout, "\t\tResults are inaccurate!\n");
#endif
        return INPUT_INVALID_PARAMETER;      
	}

	//--------------------------------
	for (r = 0; r < 2; r++)
	{
		powLen = (int)pow(2.0, (blockLen + r));

		if ( (counterArray = (unsigned int*)calloc(powLen, sizeof(unsigned int))) == NULL )
		{
#ifdef _DEBUG
            fprintf(stdout, "Serial Test:  Insufficient memory available.\n");
#endif
            return MEMORY_ALLOCATION_FAIL;
		}

		for ( i = 0; i < powLen; i++ )
		{
			counterArray[i] = 0;
		}

		for ( i = 0; i < bitArrayLen; i++ )
		{
			index = bitArray[i];
			for ( j = 1; j < (blockLen + r); j++ )
			{
				if ( bitArray[(i + j) % bitArrayLen] == 0)
				{
					index <<= 1;
				}
				else
				{
					index = (index << 1) | 1;
				}
			}
			counterArray[index]++;
		}

		sum = 0.0;
		for ( i = 0; i < powLen; i++ )
		{
			if ( counterArray[i] > 0 )
			{
				sum += counterArray[i] * log(counterArray[i] / (double)bitArrayLen);
			}
		}
		
		sum /= bitArrayLen;
		ApEn[r] = sum;
		free(counterArray);
	}
	//--------------------------------

	apen = ApEn[0] - ApEn[1];
	chi_squared = 2.0 * bitArrayLen * ( log(2.0) - apen );
	p_value = cephes_igamc(pow(2.0, (blockLen - 1)), chi_squared / 2.0);
#ifdef _DEBUG
	fprintf(stdout, "\t\tCOMPUTATIONAL INFORMATION:\n");
	fprintf(stdout, "\t\t--------------------------------------------\n");
	fprintf(stdout, "\t\t(a) m (block length)    = %d\n", blockLen);
	fprintf(stdout, "\t\t(b) n (sequence length) = %d\n", bitArrayLen);
	fprintf(stdout, "\t\t(c) Chi^2               = %f\n", chi_squared);
	fprintf(stdout, "\t\t(d) Phi(m)              = %f\n", ApEn[0]);
	fprintf(stdout, "\t\t(e) Phi(m+1)            = %f\n", ApEn[1]);
	fprintf(stdout, "\t\t(f) ApEn                = %f\n", apen);
	fprintf(stdout, "\t\t(g) Log(2)              = %f\n", log(2.0));
	fprintf(stdout, "\t\t--------------------------------------------\n");
#endif

#ifdef _DEBUG
	fprintf(stdout, "%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
#endif

    if ( p_value < ALPHA )
	{
		return APPROXIMATE_ENTROPY_TEST_FAIL;
	}
	else
	{
		return 0;
	}
}