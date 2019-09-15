#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../../include/errorCode.h"
#include "../../include/basicDefinition.h"
#include "../../include/cephes.h"
#include "../../include/blockFrequency.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                    B L O C K  F R E Q U E N C Y  T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int BlockFrequency(const unsigned char *bitArray,
                   int bitArrayLen,
				   int blockLen)
{
	int	i, j, blockCount, blockSum;
	double p_value, sum, pi, v, chi_squared;

#ifdef _DEBUG
	fprintf(stdout, "\t\t\tBLOCK FREQUENCY TEST\n");
	fprintf(stdout, "\t\t---------------------------------------------\n");
#endif

	if ( (blockLen < 2) || ( blockLen > bitArrayLen) )
    {
#ifdef _DEBUG
        fprintf(stdout, "Block frequency block length = %d\n", blockLen);
        fprintf(stdout, "The parameter value is invalid!\n");
#endif
        return INPUT_INVALID_PARAMETER;
    }
	
	blockCount = bitArrayLen / blockLen;
	sum = 0.0;
	
	for ( i = 0; i < blockCount; i++ )
	{
		blockSum = 0;
		for ( j = 0; j < blockLen; j++ )
		{
			blockSum += bitArray[j + i * blockLen];
		}
		pi = (double)blockSum / (double)blockLen;
		v = pi - 0.5;
		sum += (v * v);
	}
	chi_squared = 4.0 * blockLen * sum;
	p_value = cephes_igamc(blockCount / 2.0, chi_squared / 2.0);

#ifdef _DEBUG
	fprintf(stdout, "\t\tCOMPUTATIONAL INFORMATION:\n");
	fprintf(stdout, "\t\t---------------------------------------------\n");
	fprintf(stdout, "\t\t(a) Chi^2           = %f\n", chi_squared);
	fprintf(stdout, "\t\t(b) # of substrings = %d\n", blockCount);
	fprintf(stdout, "\t\t(c) block length    = %d\n", blockLen);
	fprintf(stdout, "\t\t(d) Note: %d bits were discarded.\n", bitArrayLen % blockLen);
	fprintf(stdout, "\t\t---------------------------------------------\n");
	fprintf(stdout, "%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
#endif

	if ( p_value < ALPHA )
	{
		return BLOCK_FREQUENCY_TEST_FAIL;
	}
	else
	{
		return 0;
	}
}
