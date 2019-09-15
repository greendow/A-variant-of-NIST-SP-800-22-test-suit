#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_

#define FREQUENCY_TEST_FAIL                            0x00001001
#define BLOCK_FREQUENCY_TEST_FAIL                      0x00001002
#define RUNS_TEST_FAIL                                 0x00001003
#define LONGEST_RUN_OF_ONES_TEST_FAIL                  0x00001004
#define MATRIX_RANK_TEST_FAIL                          0x00001005
#define DISCRETE_FOURIER_TRANSFORM_TEST_FAIL           0x00001006
#define NON_OVERLAPPING_TEMPLATE_MATCHING_TEST_FAIL    0x00001007
#define OVERLAPPING_TEMPLATE_MATCHING_TEST_FAIL        0x10001008
#define UNIVERSAL_STATISTICAL_TEST_FAIL                0x10001009
#define LINEAR_COMPLEXITY_TEST_FAIL                    0x1000100A
#define SERIAL_TEST_FAIL                               0x1000100B
#define APPROXIMATE_ENTROPY_TEST_FAIL                  0x1000100C
#define CUMULATIVE_SUMS_TEST_FAIL                      0x1000100D
#define RANDOM_EXCURSIONS_TEST_FAIL                    0x1000100E
#define RANDOM_EXCURSIONS_VARIANT_TEST_FAIL            0x1000100F

#define INPUT_LENGTH_TOO_SHORT                         0x00002001
#define MEMORY_ALLOCATION_FAIL                         0x00002002
#define P_VALUE_OUT_OF_RANGE                           0x00002003
#define INPUT_INVALID_PARAMETER                        0x00002004
#define INVALID_PARAMETER_COUNT                        0x00002005
#define PARSE_JSON_FAIL                                0x00002006
#define INVALID_JSON                                   0x00002007


#endif /*  _ERROR_CODE_H_  */
