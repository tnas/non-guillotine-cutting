#include <time.h>
#include <stdlib.h>

#include "rec.h"

extern const unsigned int GRASP_LOOP;
extern const float GRASP_LRC_PARAM;

typedef enum { SEQUENTIAL, RANDOMIC } POLICY_GET_PIECE;


unsigned int run_constructive_heuristic(rectangle* pieces_cut, rectangle *stock, int num_pieces_cut);

unsigned int run_improvement_heuristic(rectangle *pieces_cut, rectangle *stock, int num_pieces_cut, POLICY_GET_PIECE policy);

unsigned int run_grasp_metaheuristic(rectangle *pieces_cut, rectangle *stock, int num_pieces_cut);

int get_piece_to_cut(rectangle *pieces_cut, int num_pieces_cut, POLICY_GET_PIECE policy);

void build_greedy_randomized_solution(rectangle* candidates_list, const unsigned int size_candidates_list,
  rectangle *pieces_cut, unsigned int *next_index_piece, const int num_pieces_cut, rectangle* stock);
