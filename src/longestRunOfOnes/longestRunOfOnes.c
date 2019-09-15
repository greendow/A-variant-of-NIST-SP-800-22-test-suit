#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../../include/errorCode.h"
#include "../../include/basicDefinition.h"
#include "../../include/cephes.h"
#include "../../include/longestRunOfOnes.h" 

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                      L O N G E S T  R U N S  T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int LongestRunOfOnes(const unsigned char *bitArray, int bitArrayLen)
{
	double p_value, chi2, pi[7];
	int run, v_n_obs, blockCount, i, j, K, blockLen, V[7];
	unsigned int nu[7] = { 0, 0, 0, 0, 0, 0, 0 };

#ifdef _DEBUG
	fprintf(stdout, "\t\t\t  LONGEST RUNS OF ONES TEST\n");
	fprintf(stdout, "\t\t---------------------------------------------\n");
#endif

	if ( bitArrayLen < 128 )
	{
#ifdef _DEBUG
		fprintf(stdout, "\t\t   n = %d is too short\n", bitArrayLen);
#endif
		return INPUT_LENGTH_TOO_SHORT;
	}

	if ( bitArrayLen < 6272 )
	{
		K = 3;
		blockLen = 8;
		V[0] = 1;
		V[1] = 2;
		V[2] = 3;
		V[3] = 4;
		pi[0] = 0.2148;
		pi[1] = 0.3672;
		pi[2] = 0.2305;
		pi[3] = 0.1875;
	}
	else
	{
	    if ( bitArrayLen < 750000 )
		{
		    K = 5;
		    blockLen = 128;
		    V[0] = 4;
			V[1] = 5;
			V[2] = 6;
			V[3] = 7;
			V[4] = 8;
			V[5] = 9;
		    pi[0] = 0.1174;
		    pi[1] = 0.2430;
		    pi[2] = 0.2493;
		    pi[3] = 0.1752;
		    pi[4] = 0.1027;
		    pi[5] = 0.1124;
		}
		else
		{
			K = 6;
			blockLen = 10000;
			V[0] = 10;
			V[1] = 11;
			V[2] = 12;
			V[3] = 13;
			V[4] = 14;
			V[5] = 15;
			V[6] = 16;
			pi[0] = 0.0882;
			pi[1] = 0.2092;
			pi[2] = 0.2483;
			pi[3] = 0.1933;
			pi[4] = 0.1208;
			pi[5] = 0.0675;
			pi[6] = 0.0727;
		}
	}
	
	blockCount = bitArrayLen / blockLen;
	for ( i = 0; i < blockCount; i++ )
	{
		v_n_obs = 0;
		run = 0;
		for ( j = 0; j < blockLen; j++ )
		{
			if ( bitArray[i * blockLen + j] == 1 )
			{
				run++;
				if ( run > v_n_obs )
				{
					v_n_obs = run;
				}
			}
			else
			{
				run = 0;
			}
		}

        if ( v_n_obs <= V[0] )
		{
			nu[0]++;
		}
		else
		{
			if ( v_n_obs >= V[K])
			{
				nu[K]++;
			}
			else
			{
				for ( j = 1; j < K; j++ )
				{
					if ( v_n_obs == V[j] )
					{
						nu[j]++;
					}
				}
			}
		}
	}

	chi2 = 0.0;
	for ( i = 0; i <= K; i++ )
	{
		chi2 += ((nu[i] - blockCount * pi[i]) * (nu[i] - blockCount * pi[i])) / (blockCount * pi[i]);
	}

	p_value = cephes_igamc((double)(K / 2.0), chi2 / 2.0);
#ifdef _DEBUG
	fprintf(stdout, "\t\tCOMPUTATIONAL INFORMATION:\n");
	fprintf(stdout, "\t\t---------------------------------------------\n");
	fprintf(stdout, "\t\t(a) N (# of substrings)  = %d\n", blockCount);
	fprintf(stdout, "\t\t(b) M (Substring Length) = %d\n", blockLen);
	fprintf(stdout, "\t\t(c) Chi^2                = %f\n", chi2);
	fprintf(stdout, "\t\t---------------------------------------------\n");
	fprintf(stdout, "\t\t      F R E Q U E N C Y\n");
	fprintf(stdout, "\t\t---------------------------------------------\n");
#endif
	if ( K == 3 )
	{
#ifdef _DEBUG
		fprintf(stdout, "\t\t  <=1     2     3    >=4   P-value  Assignment");
		fprintf(stdout, "\n\t\t %3d %3d %3d  %3d ", nu[0], nu[1], nu[2], nu[3]);
#endif
	}
	else
	{
		if ( K == 5 )
		{
#ifdef _DEBUG
			fprintf(stdout, "\t\t<=4  5  6  7  8  >=9 P-value  Assignment");
			fprintf(stdout, "\n\t\t %3d %3d %3d %3d %3d  %3d ", nu[0], nu[1], nu[2],
				nu[3], nu[4], nu[5]);
#endif
		}
		else
		{
#ifdef _DEBUG
			fprintf(stdout,"\t\t<=10  11  12  13  14  15 >=16 P-value  Assignment");
			fprintf(stdout,"\n\t\t %3d %3d %3d %3d %3d %3d  %3d ", nu[0], nu[1], nu[2],
				nu[3], nu[4], nu[5], nu[6]);
#endif
		}
	}

	if ( isNegative(p_value) || isGreaterThanOne(p_value) )
	{
#ifdef _DEBUG
        fprintf(stdout, "\n\t\tp_value = %f\n", p_value);
		fprintf(stdout, "WARNING:  P_VALUE IS OUT OF RANGE.\n");
#endif
        return P_VALUE_OUT_OF_RANGE;
	}

#ifdef _DEBUG
	fprintf(stdout, "\n%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
#endif

	if ( p_value < ALPHA )
	{
		return LONGEST_RUN_OF_ONES_TEST_FAIL;
	}
	else
	{
		return 0;
	}
}
