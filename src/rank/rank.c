#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/errorCode.h"
#include "../../include/basicDefinition.h"
#include "../../include/cephes.h"
#include "../../include/matrix.h"
#include "../../include/rank.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                              R A N K  T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int Rank(const unsigned char *bitArray, int bitArrayLen)
{
	const int rowsCount = 32, columnCount = 32;
	int N, k;
    int r, i;
	double product;
	double p_value, chi_squared, arg1, p_32, p_31, p_30, R, F_32, F_31, F_30;
	unsigned char **matrix;

#ifdef _DEBUG
	fprintf(stdout, "\t\t\t\tRANK TEST\n");
	fprintf(stdout, "\t\t---------------------------------------------\n");
#endif

	if ( !(matrix = create_matrix(rowsCount, columnCount)) )
	{
		return MEMORY_ALLOCATION_FAIL;
	}

	if ( bitArrayLen < (38 * rowsCount * columnCount) )
	{
#ifdef _DEBUG
	   fprintf(stdout, "\t\tError: Insuffucient # Of Bits To Define a (%d x %d) Matrix\n", rowsCount, columnCount);
#endif
		delete_matrix(rowsCount, matrix);
		return INPUT_LENGTH_TOO_SHORT;
	}

	N = bitArrayLen / (rowsCount * columnCount);

	/* COMPUTE PROBABILITIES */
	r = 32;
	product = 1;
	for ( i = 0; i <= (r - 1); i++ )
	{
		product *= ((1.e0 - pow((double)(2.0), (i - 32))) * (1.e0 - pow((double)(2.0), (i - 32)))) / (1.e0 - pow((double)(2.0), (i - r)));
	}
	p_32 = pow((double)(2.0), r * (32 + 32 - r) - 32 * 32) * product;

	r = 31;
	product = 1;
	for ( i = 0; i <= (r - 1); i++ )
	{
		product *= ((1.e0 - pow((double)(2.0), (i-32))) * (1.e0 - pow((double)(2.0), (i-32)))) / (1.e0 - pow((double)(2.0), (i - r)));
	}
	p_31 = pow((double)(2.0), r * (32 + 32 - r) - 32 * 32) * product;
	p_30 = 1 - (p_32 + p_31);

	F_32 = 0;
	F_31 = 0;
	for ( k = 0; k < N; k++ )
	{
		/* FOR EACH 32x32 MATRIX */
		def_matrix(bitArray, rowsCount, columnCount, matrix, k);
		R = computeRank(rowsCount, columnCount, matrix);
		if ( R == 32 )
		{
			F_32++;		/* DETERMINE FREQUENCIES */
		}
		if ( R == 31 )
		{
			F_31++;
		}
	}
	F_30 = (double)N - (F_32 + F_31);
	chi_squared = pow(F_32 - N * p_32, (double)(2.0)) / (double)(N * p_32) +
				      pow(F_31 - N * p_31, (double)(2.0)) / (double)(N * p_31) +
				      pow(F_30 - N * p_30, (double)(2.0)) / (double)(N * p_30);
		
	arg1 = -chi_squared / 2.e0;
#ifdef _DEBUG
	fprintf(stdout, "\t\tCOMPUTATIONAL INFORMATION:\n");
	fprintf(stdout, "\t\t---------------------------------------------\n");
	fprintf(stdout, "\t\t(a) Probability P_%d = %f\n", 32, p_32);
	fprintf(stdout, "\t\t(b)             P_%d = %f\n", 31, p_31);
	fprintf(stdout, "\t\t(c)             P_%d = %f\n", 30, p_30);
	fprintf(stdout, "\t\t(d) Frequency   F_%d = %d\n", 32, (int)F_32);
	fprintf(stdout, "\t\t(e)             F_%d = %d\n", 31, (int)F_31);
	fprintf(stdout, "\t\t(f)             F_%d = %d\n", 30, (int)F_30);
	fprintf(stdout, "\t\t(g) # of matrices    = %d\n", N);
	fprintf(stdout, "\t\t(h) Chi^2            = %f\n", chi_squared);
	fprintf(stdout, "\t\t(i) NOTE: %d BITS WERE DISCARDED.\n", bitArrayLen % (rowsCount * columnCount));
	fprintf(stdout, "\t\t---------------------------------------------\n");
#endif
	p_value = exp(arg1);
	if ( isNegative(p_value) || isGreaterThanOne(p_value) )
	{
#ifdef _DEBUG
		fprintf(stdout, "WARNING:  P_VALUE IS OUT OF RANGE.\n");
#endif
        delete_matrix(rowsCount, matrix);
		return P_VALUE_OUT_OF_RANGE;
	}

	/* DEALLOCATE MATRIX  */
	delete_matrix(rowsCount, matrix);

#ifdef _DEBUG
	fprintf(stdout, "%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
#endif
	if ( p_value < ALPHA )
	{
		return MATRIX_RANK_TEST_FAIL;
	}
	else
	{
		return 0;
	}
}
