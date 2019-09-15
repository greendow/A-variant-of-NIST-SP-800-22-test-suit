#ifndef _BLOCK_FREQUENCY_TEST_H_
#define _BLOCK_FREQUENCY_TEST_H_

#ifdef  __cplusplus
extern "C" {
#endif

int BlockFrequency(const unsigned char *bitArray,
                   int bitArrayLen,
				   int blockLen);

#ifdef  __cplusplus
}
#endif

#endif /*  _BLOCK_FREQUENCY_TEST_H_  */