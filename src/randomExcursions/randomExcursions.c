#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/errorCode.h"
#include "../../include/basicDefinition.h"
#include "../../include/cephes.h"
#include "../../include/randomExcursions.h" 

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                     R A N D O M  E X C U R S I O N S  T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int RandomExcursions(const unsigned char *bitArray,
                     int bitArrayLen)
{
	int b, i, j, k, J, constraint, x;
	int cycleStart, cycleStop, *cycle = NULL, *S_k = NULL;
	int stateX[8] = { -4, -3, -2, -1, 1, 2, 3, 4 };
	int counter[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	double p_value, sum, nu[6][8];
	double pi[5][6] = { {0.0000000000, 0.00000000000, 0.00000000000, 0.00000000000, 0.00000000000, 0.0000000000},
	                    {0.5000000000, 0.25000000000, 0.12500000000, 0.06250000000, 0.03125000000, 0.0312500000},
						{0.7500000000, 0.06250000000, 0.04687500000, 0.03515625000, 0.02636718750, 0.0791015625},
						{0.8333333333, 0.02777777778, 0.02314814815, 0.01929012346, 0.01607510288, 0.0803755143},
						{0.8750000000, 0.01562500000, 0.01367187500, 0.01196289063, 0.01046752930, 0.0732727051} };

#ifdef _DEBUG
	fprintf(stdout, "\t\t\t  RANDOM EXCURSIONS TEST\n");
	fprintf(stdout, "\t\t--------------------------------------------\n");
#endif

	if ( bitArrayLen < 1000000)
	{
#ifdef _DEBUG
        fprintf(stdout, "\t\tError: The minimal input length in bits is 1000000!\n");
#endif
        return INPUT_LENGTH_TOO_SHORT;      
	}

	if ( ((S_k = (int *)calloc(bitArrayLen, sizeof(int))) == NULL) ||
		 ((cycle = (int *)calloc((bitArrayLen / 100 + 2), sizeof(int))) == NULL) )
    {
#ifdef _DEBUG
		fprintf(stdout, "Error: Insufficent Work Space Allocated.\n");
#endif
		if ( S_k != NULL )
		{
			free(S_k);
		}
		if ( cycle != NULL )
		{
			free(cycle);
		}
		return MEMORY_ALLOCATION_FAIL;
	}
	
	J = 0; 					/* DETERMINE CYCLES */
	S_k[0] = 2 * (int)bitArray[0] - 1;
	for( i = 1; i < bitArrayLen; i++ )
	{
		S_k[i] = S_k[i - 1] + 2 * bitArray[i] - 1;
		if ( S_k[i] == 0 )
		{
			J++;
			if ( J > (bitArrayLen / 100) )
			{
#ifdef _DEBUG
				fprintf(stdout, "Error: EXCEEDING THE MAX NUMBER OF CYCLES EXPECTED\n.");
#endif
				free(S_k);
				free(cycle);
				return RANDOM_EXCURSIONS_TEST_FAIL;
			}
			cycle[J] = i;
		}
	}

	if ( S_k[bitArrayLen - 1] != 0 )
	{
		J++;    /* Note: the maximal vaule of J is (bitArrayLen / 100 + 1) */
	}
	cycle[J] = bitArrayLen;
#ifdef _DEBUG
	fprintf(stdout, "\t\tCOMPUTATIONAL INFORMATION:\n");
	fprintf(stdout, "\t\t--------------------------------------------\n");
	fprintf(stdout, "\t\t(a) Number Of Cycles (J) = %d\n", J);
	fprintf(stdout, "\t\t(b) Sequence Length (n)  = %d\n", bitArrayLen);
	fprintf(stdout, "\t\t--------------------------------------------\n");
#endif

	constraint = MAX( (int)( 0.005 * pow( (double)(bitArrayLen), (double)(0.5) ) ), (int)(500) );
	if (J < constraint)
	{
#ifdef _DEBUG
		fprintf(stdout, "\t\t(c) Rejection Constraint = %d\n", constraint);
		fprintf(stdout, "\t\tJ < %d\n", constraint);
		fprintf(stdout, "\t\tWARNING:  TEST NOT APPLICABLE.  THERE ARE AN\n");
		fprintf(stdout, "\t\t\t  INSUFFICIENT NUMBER OF CYCLES.\n");
#endif
        free(S_k);
		free(cycle);
		return RANDOM_EXCURSIONS_TEST_FAIL;
	}

	cycleStart = 0;
	cycleStop = cycle[1];
	/* Note: the index of elements in cycle[] which is really used starts from 1, cycle[0] is not used. */

	for ( k = 0; k < 6; k++ )
	{
		for ( i = 0; i < 8; i++ )
		{
			nu[k][i] = (double)(0);
		}
	}

	for ( j = 1; j <= J; j++ )
	{                           /* FOR EACH CYCLE */
		for ( i = 0; i < 8; i++ )
		{
			counter[i] = 0;
		}

		for ( i = cycleStart; i < cycleStop; i++ )
		{
			if ( (S_k[i] >= 1 && S_k[i] <= 4) || (S_k[i] >= -4 && S_k[i] <= -1) )
			{
				if ( S_k[i] < 0 )
				{
					b = 4;
				}
				else
				{
					b = 3;
				}
				counter[( S_k[i] + b )]++;
			}
		}
		cycleStart = cycle[j] + 1;
		if ( j < J )
		{
			cycleStop = cycle[j + 1];
		}
		
		for ( i = 0; i < 8; i++ )
		{
			if ( (counter[i] >= 0) && (counter[i] <= 4) )
			{
				nu[( counter[i] )][i]++;
			}
			else
			{
				if ( counter[i] >= 5 )
				{
					nu[5][i]++;
				}
			}
		}
	}

	for ( i = 0; i < 8; i++ )
	{
		x = stateX[i];
		sum = (double)(0);
		for ( k = 0; k < 6; k++ )
		{
			sum += pow(( nu[k][i] - J * pi[(int)fabs((double)(x))][k] ), (double)(2.0)) / (J * pi[(int)fabs((double)(x))][k]);
		}
		p_value = cephes_igamc((double)(2.5), sum / 2.0);

		if ( isNegative(p_value) || isGreaterThanOne(p_value) )
		{
#ifdef _DEBUG
			fprintf(stdout, "WARNING:  P_VALUE IS OUT OF RANGE.\n");
#endif
            free(S_k);
			free(cycle);
			return P_VALUE_OUT_OF_RANGE;
		}

#ifdef _DEBUG
		fprintf(stdout, "%s\t\tx = %2d chi^2 = %9.6f p_value = %f\n",
				p_value < ALPHA ? "FAILURE" : "SUCCESS", x, sum, p_value);
#endif
        if ( p_value < ALPHA )
		{
			free(S_k);
			free(cycle);
			return RANDOM_EXCURSIONS_TEST_FAIL;
		}
	}

#ifdef _DEBUG
	fprintf(stdout, "\n");
#endif
	free(S_k);
	free(cycle);
	return 0;
}
