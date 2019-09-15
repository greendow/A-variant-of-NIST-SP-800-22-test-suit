/**************************************************
* File name: rnd_test.c
* Author: HAN Wei
* Author's blog: http://blog.csdn.net/henter/
* License: MIT License (https://mit-license.org/)
           Copyright (c) 2019 HAN Wei
**************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../include/errorCode.h"
#include "../include/cJSON.h"
#include "../include/c_file_operation.h"
#include "../include/basicDefinition.h"
#include "../include/rnd_test.h"
#include "../include/frequency.h"
#include "../include/blockFrequency.h"
#include "../include/runs.h"
#include "../include/longestRunOfOnes.h"
#include "../include/rank.h"
#include "../include/discreteFourierTransform.h"
#include "../include/nonOverlappingTemplateMatchings.h"
#include "../include/overlappingTemplateMatchings.h"
#include "../include/universal.h"
#include "../include/linearComplexity.h"
#include "../include/serial.h"
#include "../include/approximateEntropy.h"
#include "../include/cumulativeSums.h"
#include "../include/randomExcursions.h"
#include "../include/randomExcursionsVariant.h"

void convertToBitArray(const unsigned char *input,
                       int inputLen,
                       unsigned char *bitArray)
{
    int i;
    const unsigned char *p;
	unsigned char *q;

    p = input;
    q = bitArray;
    for ( i = 0; i < inputLen; i++ )
    {
        *q = (*p & 0x80) >> 7;
        q++;
        *q = (*p & 0x40) >> 6;
        q++;
        *q = (*p & 0x20) >> 5;
        q++;
        *q = (*p & 0x10) >> 4;
        q++;
        *q = (*p & 0x8) >> 3;
        q++;
        *q = (*p & 0x4) >> 2;
        q++;
        *q = (*p & 0x2) >> 1;
        q++;
        *q = *p & 0x1;
        q++;

        p++;
    }
}

int randomnessTest(unsigned char *input, int inputLen)
{
    const char configFile[] = {"config.json"};
    long long lconfigFileLen;
    int configFileLen;
    char *configFileContent;
    cJSON *configJson;
    const char *errorDescription;
    const cJSON *blockFrequencyBlockLenJson = NULL, *nonOverlappingTemplateLenJson = NULL,
                *OverlappingTemplateLenJson = NULL, *linearComplexityBlockLenJson = NULL,
                *serialBlockLenJson = NULL, *approximateEntropyBlockLenJson = NULL;


    int errorCode = 0, testResult[15], bitStreamLen, passItemCount;
    unsigned char *bitStream;
    int blockFrequencyBlockLen, nonOverlappingTemplateLen, OverlappingTemplateLen,
            linearComplexityBlockLen, serialBlockLen, approximateEntropyBlockLen;
	double passRate;
    enum testItem
    {
        frequencyFlag,
        blockFrequencyFlag,
        runsFlag,
        longestRunOfOnesFlag,
        matrixRankFlag,
        discreteFourierTransformFlag,
        nonOverlappingTemplateMatchingsFlag,
        overlappingTemplateMatchingsFlag,
        universalFlag,
        linearComplexityFlag,
        serialFlag,
        approximateEntropyFlag,
        cumulativeSumsFlag,
        randomExcursionsFlag,
        randomExcursionsVariantFlag
    } rndTestItem;

    if ( errorCode = GetFileLength(configFile, &lconfigFileLen) )
    {
#ifdef _DEBUG
        fprintf(stdout, "Get config file length failed!\n");
#endif
        return errorCode;
    }
    configFileLen = (int)lconfigFileLen;

    if ( errorCode = ReadFileIntoMemoryBuffer(configFile, &configFileContent) )
    {
#ifdef _DEBUG
        fprintf(stdout, "Read config file failed!\n");
#endif
        return errorCode;
    }

    configJson = cJSON_Parse(configFileContent);
    if ( !(configJson) )
    {
        if ( errorDescription = cJSON_GetErrorPtr() )
        {
#ifdef _DEBUG
            fprintf(stdout, "Error before: %s\n", errorDescription);
#endif
        }
        cJSON_Delete(configJson);
        FreeFileInMemoryBuffer(configFileContent);
        return PARSE_JSON_FAIL;
    }

    FreeFileInMemoryBuffer(configFileContent);
    blockFrequencyBlockLenJson = cJSON_GetObjectItemCaseSensitive(configJson, "blockFrequency_BlockLen");
    nonOverlappingTemplateLenJson = cJSON_GetObjectItemCaseSensitive(configJson, "nonOverlapping_TemplateLen");
    OverlappingTemplateLenJson = cJSON_GetObjectItemCaseSensitive(configJson, "Overlapping_TemplateLen");
    linearComplexityBlockLenJson = cJSON_GetObjectItemCaseSensitive(configJson, "linearComplexity_BlockLen");
    serialBlockLenJson = cJSON_GetObjectItemCaseSensitive(configJson, "serial_BlockLen");
    approximateEntropyBlockLenJson = cJSON_GetObjectItemCaseSensitive(configJson, "approximateEntropy_BlockLen");

    if ( !(cJSON_IsNumber(blockFrequencyBlockLenJson)) || !(cJSON_IsNumber(nonOverlappingTemplateLenJson))
        || !(cJSON_IsNumber(OverlappingTemplateLenJson)) || !(cJSON_IsNumber(linearComplexityBlockLenJson))
        || !(cJSON_IsNumber(serialBlockLenJson)) || !(cJSON_IsNumber(approximateEntropyBlockLenJson)) )
    {
        cJSON_Delete(configJson);
        return INVALID_JSON;
    }

    blockFrequencyBlockLen = (int)(blockFrequencyBlockLenJson->valuedouble);
    nonOverlappingTemplateLen = (int)(nonOverlappingTemplateLenJson->valuedouble);
    OverlappingTemplateLen = (int)(OverlappingTemplateLenJson->valuedouble);
    linearComplexityBlockLen = (int)(linearComplexityBlockLenJson->valuedouble);
    serialBlockLen = (int)(serialBlockLenJson->valuedouble);
    approximateEntropyBlockLen = (int)(approximateEntropyBlockLenJson->valuedouble);
    cJSON_Delete(configJson);
#ifdef _DEBUG
	fprintf(stdout, "\n\t\t\tParameters config\n");
	fprintf(stdout, "\t\t--------------------------------------------\n");
    fprintf(stdout, "\t\t  blockFrequencyBlockLen = %d\n", blockFrequencyBlockLen);
    fprintf(stdout, "\t\t  nonOverlappingTemplateLen = %d\n", nonOverlappingTemplateLen);
    fprintf(stdout, "\t\t  OverlappingTemplateLen = %d\n", OverlappingTemplateLen);
    fprintf(stdout, "\t\t  linearComplexityBlockLen = %d\n", linearComplexityBlockLen);
    fprintf(stdout, "\t\t  serialBlockLen = %d\n", serialBlockLen);
    fprintf(stdout, "\t\t  approximateEntropyBlockLen = %d\n", approximateEntropyBlockLen);
	fprintf(stdout, "\n");
#endif

    bitStreamLen = inputLen * 8;
    if ( !(bitStream = (unsigned char *)malloc(bitStreamLen)) )
    {
#ifdef _DEBUG
        fprintf(stdout, "Memory allocation failed!\n");
#endif
        return MEMORY_ALLOCATION_FAIL;
    }
    convertToBitArray(input, inputLen, bitStream);

    rndTestItem = frequencyFlag;
    if ( testResult[rndTestItem] = Frequency(bitStream, bitStreamLen) )
    {
        errorCode = testResult[rndTestItem];
#ifdef _DEBUG
		fprintf(stdout, "Frequency test failed!\n");
		fprintf(stdout, "Test program exits because all subsequent tests depend on the passing of this test!\n");
#endif
		free(bitStream);
		return errorCode;
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = BlockFrequency(bitStream,
                                                  bitStreamLen,
                                                  blockFrequencyBlockLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = Runs(bitStream, bitStreamLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = LongestRunOfOnes(bitStream, bitStreamLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = Rank(bitStream, bitStreamLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = DiscreteFourierTransform(bitStream, bitStreamLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = NonOverlappingTemplateMatchings(bitStream,
                                                                   bitStreamLen,
                                                                   nonOverlappingTemplateLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = OverlappingTemplateMatchings(bitStream,
                                                                bitStreamLen,
                                                                OverlappingTemplateLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = Universal(bitStream, bitStreamLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = LinearComplexity(bitStream,
                                                    bitStreamLen,
                                                    linearComplexityBlockLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = Serial(bitStream,
                                          bitStreamLen,
                                          serialBlockLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = ApproximateEntropy(bitStream,
                                                      bitStreamLen,
                                                      approximateEntropyBlockLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = CumulativeSums(bitStream, bitStreamLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = RandomExcursions(bitStream, bitStreamLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }
    rndTestItem++;
#ifdef _DEBUG
    fprintf(stdout, "\n");
#endif

    if ( testResult[rndTestItem] = RandomExcursionsVariant(bitStream, bitStreamLen) )
    {
        if ( !(errorCode) )
        {
            errorCode = testResult[rndTestItem];
        }
    }

#ifdef _DEBUG
    rndTestItem = frequencyFlag;
    fprintf(stdout, "\n\n\t\t\tRandomness Test Result:\n");
    fprintf(stdout, "\t-------------------------------------------\n");
    fprintf(stdout, "\t 1. Frequency test:                          %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t 2. Block Frequency test:                    %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t 3. Runs test:                               %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t 4. Longest Run Of Ones test:                %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t 5. Matrix Rank test:                        %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t 6. Discrete Fourier Transform test:         %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t 7. Non-Overlapping Template Matchings test: %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t 8. Overlapping Template Matchings test:     %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t 9. Universal test:                          %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t10. Linear Complexity test:                  %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t11. Serial test:                             %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t12. Approximate Entropy test:                %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t13. Cumulative Sums test:                    %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t14. Random Excursions test:                  %s\n", testResult[rndTestItem] ? "Failure": "Success");
    rndTestItem++;
    fprintf(stdout, "\t15. randomExcursionsVariant test:            %s\n", testResult[rndTestItem] ? "Failure": "Success");
#endif

    passItemCount = 0;
    for ( rndTestItem = frequencyFlag; rndTestItem <= randomExcursionsVariantFlag; rndTestItem++ )
    {
        if ( !(testResult[rndTestItem]) )
        {
            passItemCount++;
        }
    }
    passRate = (double)(passItemCount) / (sizeof(testResult) / sizeof(int));
#ifdef _DEBUG
	fprintf(stdout, "\t---------------------------------------------\n");
	fprintf(stdout, "\t\t\tSummarization:\n");
	fprintf(stdout, "\t\tPass rate = %.4f%%\n", (passRate * 100));
#endif

    free(bitStream);
    return errorCode;
}
