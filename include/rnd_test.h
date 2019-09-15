/**************************************************
* File name: rnd_test.h
* Author: HAN Wei
* Author's blog: http://blog.csdn.net/henter/
* License: MIT License (https://mit-license.org/)
           Copyright (c) 2019 HAN Wei
**************************************************/

#ifndef _RANDOMNESS_TEST_H_
#define _RANDOMNESS_TEST_H_

#ifdef  __cplusplus
extern "C" {
#endif

/************************************************
 * Name: convertToBitArray
 * Function: convert an octet string to an array,
 *   and the value of each element in the array 
 *   is a bit-value of the octet string (in big-endian)
 * Parameters:
 *   input[in]      the octec string
 *   inputLen[in]   the length of the octet string,
 *                  count in bytes
 *   bitArray[out]  the buffer to store bit-value,
 *                  its length must be greater than 
 *                  or equal to (8 * input_len)
 ************************************************/
void convertToBitArray(const unsigned char *input,
                       int inputLen,
                       unsigned char *bitArray);

/************************************************
 * Name: randomnessTest
 * Function: perform randomness test on a binary 
 *     byte stream
 * Parameters:
 *   input[in]        the octec string
 *   input_len[in]    the length of the octet string,
 *                    count in bytes
 * Return value:
 *  0:                test passes
    any other value:  an error occurs
 ************************************************/
int randomnessTest(unsigned char *input, int inputLen);

#ifdef  __cplusplus
}
#endif

#endif /*  _RANDOMNESS_TEST_H_  */