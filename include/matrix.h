#ifndef _MATRIX_RANK_ALGORITHM_H_
#define _MATRIX_RANK_ALGORITHM_H_

#ifdef  __cplusplus
extern "C" {
#endif

unsigned char** create_matrix(int M, int Q);
int computeRank(int M, int Q, unsigned char **matrix);
void perform_elementary_row_operations(int flag, int i, int M, int Q, unsigned char **A);
int find_unit_element_and_swap(int flag, int i, int M, int Q, unsigned char **A);
int swap_rows(int i, int index, int Q, unsigned char **A);
int determine_rank(int m, int M, int Q, unsigned char **A);
void def_matrix(const unsigned char *bitArray, int M, int Q, unsigned char **m, int k);
void delete_matrix(int M, unsigned char **matrix);

#ifdef  __cplusplus
}
#endif

#endif /*  _MATRIX_RANK_ALGORITHM_H_  */