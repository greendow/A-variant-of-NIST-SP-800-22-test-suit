#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../../include/errorCode.h"
#include "../../include/basicDefinition.h"
#include "../../include/cephes.h"
#include "../../include/frequency.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                          F R E Q U E N C Y  T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int Frequency(const unsigned char *bitArray, int bitArrayLen)
{
	int i;
	double sum, s_obs, f, p_value, sqrt2 = 1.41421356237309504880;

#ifdef _DEBUG
	fprintf(stdout, "\t\t\t      FREQUENCY TEST\n");
	fprintf(stdout, "\t\t---------------------------------------------\n");
#endif

	if ( bitArrayLen < 100 )
	{
#ifdef _DEBUG
		fprintf(stdout, "\t\t   n=%d is too short\n", bitArrayLen);
#endif
		return INPUT_LENGTH_TOO_SHORT;
	}
	
	sum = 0.0;
	for ( i = 0; i < bitArrayLen; i++ )
	{
		sum += 2 * (int)bitArray[i] - 1;
	}
	s_obs = fabs(sum) / sqrt((double)(bitArrayLen));
	f = s_obs / sqrt2;
	p_value = cephes_erfc(f);

#ifdef _DEBUG
	fprintf(stdout, "\t\tCOMPUTATIONAL INFORMATION:\n");
	fprintf(stdout, "\t\t---------------------------------------------\n");
	fprintf(stdout, "\t\t(a) The nth partial sum = %d\n", (int)sum);
	fprintf(stdout, "\t\t(b) S_n/n               = %f\n", sum / bitArrayLen);
	fprintf(stdout, "\t\t---------------------------------------------\n");
	fprintf(stdout, "%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
#endif

	if ( p_value < ALPHA )
	{
		return FREQUENCY_TEST_FAIL;
	}
	else
	{
		return 0;
	}
}
