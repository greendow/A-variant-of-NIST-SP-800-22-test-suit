#ifndef _NON_OVERLAPPING_TEMPLATE_MATCHING_TEST_H_
#define _NON_OVERLAPPING_TEMPLATE_MATCHING_TEST_H_

#define MAX_NUM_OF_TEMPLATES    148    /* APERIODIC TEMPLATES: 148=>temp_length=9 */

#ifdef  __cplusplus
extern "C" {
#endif

int NonOverlappingTemplateMatchings(const unsigned char *bitArray,
                                    int bitArrayLen,
									int templateLen);

#ifdef  __cplusplus
}
#endif

#endif /*  _NON_OVERLAPPING_TEMPLATE_MATCHING_TEST_H_  */