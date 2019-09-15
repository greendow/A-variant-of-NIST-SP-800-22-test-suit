#ifndef _OVERLAPPING_TEMPLATE_MATCHING_TEST_H_
#define _OVERLAPPING_TEMPLATE_MATCHING_TEST_H_

#ifdef  __cplusplus
extern "C" {
#endif

double Pr(int u, double eta);

int OverlappingTemplateMatchings(const unsigned char *bitArray,
                                 int bitArrayLen,
								 int templateLen);

#ifdef  __cplusplus
}
#endif

#endif /*  _OVERLAPPING_TEMPLATE_MATCHING_TEST_H_  */