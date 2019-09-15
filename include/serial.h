#ifndef _SERIAL_TEST_H_
#define _SERIAL_TEST_H_

#ifdef  __cplusplus
extern "C" {
#endif

int psi2(const unsigned char *bitArray,
         int bitArrayLen,
		 int blockLen,
		 double *sum);

int Serial(const unsigned char *bitArray,
           int bitArrayLen,
		   int blockLen);

#ifdef  __cplusplus
}
#endif

#endif /*  _SERIAL_TEST_H_  */