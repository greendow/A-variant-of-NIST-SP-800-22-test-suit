#ifndef _APPROXIMATE_ENTROPY_TEST_H_
#define _APPROXIMATE_ENTROPY_TEST_H_

#ifdef  __cplusplus
extern "C" {
#endif

int ApproximateEntropy(const unsigned char *bitArray,
                       int bitArrayLen,
					   int blockLen);

#ifdef  __cplusplus
}
#endif

#endif /*  _APPROXIMATE_ENTROPY_TEST_H_  */