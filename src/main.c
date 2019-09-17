/**************************************************
* File name: main.c
* Author: HAN Wei
* Author's blog: http://blog.csdn.net/henter/
* License: MIT License (https://mit-license.org/)
           Copyright (c) 2019 HAN Wei
**************************************************/

#include <stdio.h>
#include "../include/errorCode.h"
#include "../include/c_file_operation.h"
#include "../include/rnd_test.h"

int main(int argc, char *argv[])
{
    long long llfileLen;
    int fileLen, errorCode;
    char *octetStream;

    if ( argc != 2)
    {
        fprintf(stdout, "Error: Argument count is invalid!\n");
        return INVALID_PARAMETER_COUNT;
    }

    fprintf(stdout, "Randomness sample data file: %s\n\n", argv[1]);
    if ( errorCode = GetFileLength(argv[1], &llfileLen) )
    {
        fprintf(stdout, "Get file length failed!\n");
        fprintf(stdout, "Error code: 0x%x\n", errorCode);
        return errorCode;
    }
    fileLen = (int)(llfileLen);

    if ( errorCode = ReadFileIntoMemoryBuffer(argv[1], &octetStream) )
    {
        fprintf(stdout, "Read file failed!\n");
        fprintf(stdout, "Error code: 0x%x\n", errorCode);
        return errorCode;
    }

    if ( errorCode = randomnessTest((unsigned char *)(octetStream), fileLen) )
    {
        FreeFileInMemoryBuffer(octetStream);
        return errorCode;
    }

    FreeFileInMemoryBuffer(octetStream);
    return 0;
}
