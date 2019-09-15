/**************************************************
* File name: c_file_operation.c
* Author: HAN Wei
* Author's blog: http://blog.csdn.net/henter/
* Description: implement some file operation functions,
    support both Windows and Linux
* License: MIT License (https://mit-license.org/)
           Copyright (c) 2019 HAN Wei
**************************************************/
 
#include "../include/c_file_operation.h"
#include "../include/errorCode.h"
#include <sys/types.h>  
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int GetFileLength(const char *file_name, long long *file_length)
{
#if defined(_WIN32) || defined(_WIN64)
	struct __stat64 buf;
#else
	struct stat buf;
#endif
 
	if ( (!(file_name)) || (!(file_length)) )
	{
		return INPUT_INVALID_NULL_PARAMETER;
	}
 
#if defined(_WIN32) || defined(_WIN64)
	if ( _stat64(file_name, &buf) )
#else
	if ( stat64(file_name, &buf) )
#endif
	{
		return FILE_OPERATION_FAIL;
	}
 
	*file_length = buf.st_size;
	return FUNCTION_EXECUTE_SUCCEED;
}
 
int ReadFileIntoMemoryBuffer(const char *file_name, char **pp)
{
	int error_code;
	long long file_length;
	int file_len;
	char *pointer;
	FILE *fp;
 
	if ( (!(file_name)) || (!(pp)) )
	{
		return INPUT_INVALID_NULL_PARAMETER;
	}
 
	if ( error_code = GetFileLength(file_name, &file_length) )
	{
		return error_code;
	}
 
	if ( file_length > MAX_FILE_BUFFER_LENGTH )
	{
		return FILE_LENGTH_OUT_OF_SCOPE;
	}
 
	file_len = (int)(file_length);
 
	if ( !( fp = fopen(file_name, "rb") ) )
		return FILE_OPERATION_FAIL;
 
	if ( !(pointer = (char *)malloc(file_len)) )
	{
		fclose(fp);
		return MEMORY_ALLOCATION_FAIL;
	}
	fread(pointer, file_len, 1, fp);
	fclose(fp);
	*pp = pointer;
	return FUNCTION_EXECUTE_SUCCEED;
}
 
int FreeFileInMemoryBuffer(char *p)
{
	if ( !(p) )
	{
		return INPUT_INVALID_NULL_PARAMETER;
	}
 
	free(p);
	return FUNCTION_EXECUTE_SUCCEED;
}
 
int CompareFileInBinaryFormat(char *file1_name, char *file2_name, int *result)
{
	int error_code;
	long long file1_len, file2_len, remain_byte_count;
	char *file1_buf, *file2_buf;
	int buf_len, read_byte_count;
	FILE *fp1, *fp2;
 
	if ( (!(file1_name)) || (!(file2_name)) || (!(result)) )
	{
		return INPUT_INVALID_NULL_PARAMETER;
	}
 
	if ( !(strcmp(file1_name, file2_name)) )
	{
		*result = 0;
		return FUNCTION_EXECUTE_SUCCEED;
	}
 
	if ( error_code = GetFileLength(file1_name, &file1_len) )
	{
		return error_code;
	}
 
	if ( error_code = GetFileLength(file2_name, &file2_len) )
	{
		return error_code;
	}
 
	if ( file1_len != file2_len )
	{
		*result = -1;
		return FUNCTION_EXECUTE_SUCCEED;
	}
 
	if ( !(file1_len))
	{
		*result = 0;
		return FUNCTION_EXECUTE_SUCCEED;
	}
 
	if ( file1_len <= LITTTE_FILE_BUFFER_LENGTH )
	{
		buf_len = (int)(LITTTE_FILE_BUFFER_LENGTH);
	}
	else
	{
		if ( file1_len <= MEDIUM_FILE_BUFFER_LEGNTH )
		{
			buf_len = (int)(MEDIUM_FILE_BUFFER_LEGNTH);
		}
		else
		{
			buf_len = (int)(LARGE_FILE_BUFFER_LENGTH);
		}
	}
 
	if ( !(file1_buf = (char *)malloc(buf_len)) )
	{
		return MEMORY_ALLOCATION_FAIL;
	}
	if ( !(file2_buf = (char *)malloc(buf_len)) )
	{
		free(file1_buf);
		return MEMORY_ALLOCATION_FAIL;
	}
 
	if ( !(fp1 = fopen(file1_name, "rb")) )
	{
		free(file2_buf);
		free(file1_buf);
		return FILE_OPERATION_FAIL;
	}
	if ( !(fp2 = fopen(file2_name, "rb")) )
	{
		fclose(fp1);
		free(file2_buf);
		free(file1_buf);
		return FILE_OPERATION_FAIL;
	}
 
	remain_byte_count = file1_len;
	while (remain_byte_count > 0)
	{
		if ( remain_byte_count < (long long)(buf_len) )
		{
			read_byte_count = (int)(remain_byte_count);
		}
		else
		{
			read_byte_count = buf_len;
		}
		fread(file1_buf, read_byte_count, 1, fp1);
		fread(file2_buf, read_byte_count, 1, fp2);
		if ( memcmp(file1_buf, file2_buf, read_byte_count) )
		{
			*result = -1;
			fclose(fp2);
			fclose(fp1);
			free(file2_buf);
			free(file1_buf);
			return FUNCTION_EXECUTE_SUCCEED;
		}
		remain_byte_count = remain_byte_count - (long long)(read_byte_count);
	}
 
	*result = 0;
	fclose(fp2);
	fclose(fp1);
	free(file2_buf);
	free(file1_buf);
	return FUNCTION_EXECUTE_SUCCEED;
}
