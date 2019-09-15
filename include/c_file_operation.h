/**************************************************
* File name: c_file_operation.h
* Author: HAN Wei
* Author's blog: http://blog.csdn.net/henter/
* Description: declare some file operation functions,
    support both Windows and Linux
* License: MIT License (https://mit-license.org/)
           Copyright (c) 2019 HAN Wei
**************************************************/
 
 
#ifndef HEADER_C_FILE_OPERATION_H
  #define HEADER_C_FILE_OPERATION_H
 
#define FUNCTION_EXECUTE_SUCCEED      (int)(0)
#define INPUT_INVALID_NULL_PARAMETER  (int)(1)
#define FILE_OPERATION_FAIL           (int)(2)
#define FILE_LENGTH_OUT_OF_SCOPE      (int)(3)
 
#define MAX_FILE_BUFFER_LENGTH  (long long)(268435456)  /* 268435456 = 256M */
 
#define LITTTE_FILE_BUFFER_LENGTH (long long)(1048576)   /* 1048576 = 1M */
#define MEDIUM_FILE_BUFFER_LEGNTH (long long)(8388608)   /* 8388608 = 8M */
#define LARGE_FILE_BUFFER_LENGTH  (long long)(67108864)  /* 67108864 = 64M */
 
#ifdef  __cplusplus
  extern "C" {
#endif
 
/**************************************************
* Name: GetFileLength
* Function: get file length
* Parameters:
    file_name[in]  file name, path can be included
    file_len[out]  file length, size in bytes
* Return value:
    0:                function executes successfully
    any other value:  an error occurs
* Notes:
1. Large file is supported. That is, file length can be arbitrary.
2. UTF-16 file_name string is NOT supported.
**************************************************/
int GetFileLength(const char *file_name, long long *file_len);
 
/**************************************************
* Name: ReadFileIntoMemoryBuffer
* Function: read file into memory buffer
* Parameters:
    file_name[in]  file name, path can be included
    pp[out]        a pointer address, that pointer will point 
	               to the memory newly allocated
* Return value:
    0:                function executes successfully
    any other value:  an error occurs
* Notes:
1. The max file length is 256 MB. The upper bound is set 
   deliberately to avoid occupying too much memory.
2. UTF-16 file_name string is NOT supported.
3. This function allocate memory buffer internally. It read 
   a file into the buffer. The buffer length is equal to the 
   file length.
4. FreeFileInMemoryBuffer() MUST be invoked to free memory 
   allocated in this function.
**************************************************/
int ReadFileIntoMemoryBuffer(const char *file_name, char **pp);
 
/**************************************************
* Name: FreeFileInMemoryBuffer
* Function: free memory buffer allocated in ReadFileIntoMemoryBuffer()
* Parameters:
    p[in]  a pointer that points to the allocated memory by 
	       function ReadFileIntoMemoryBuffer()
* Return value:
    0:                function executes successfully
    any other value:  an error occurs
**************************************************/
int FreeFileInMemoryBuffer(char *p);
 
/**************************************************
* Name: CompareFileInBinaryFormat
* Function: Compare two files
* Parameters:
    file1_name[in]  file name
	file2_name[in]  file name
	result[out]     0  -- two files are identical
	                -1 -- two files are different
* Return value:
    0:                function executes successfully
    any other value:  an error occurs
* Notes:
1. Large file is supported. That is, file length can be arbitrary.
2. Memory buffer of various length is automatically chosen internally 
   according to the file length in order to limit memory usage and 
   improve effiency.
3. A special case: If the length of two files are both 0 byte, the two files are 
   considered identical.
**************************************************/
int CompareFileInBinaryFormat(char *file1_name, char *file2_name, int *result);
 
#ifdef  __cplusplus
  }
#endif
 
#endif  /* end of HEADER_C_FILE_OPERATION_H */
