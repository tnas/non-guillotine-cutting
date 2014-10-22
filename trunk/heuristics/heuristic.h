#include <time.h>
#include <stdlib.h>

#include "rec.h"

extern const unsigned int GRASP_LOOP;
extern const float GRASP_LRC_PARAM;


unsigned int run_constructive_heuristic(rectangle* pieces_cut, rectangle *stock, int num_pieces_cut);

unsigned int run_improvement_heuristic(rectangle *pieces_cut, rectangle *stock, int num_pieces_cut);

unsigned int run_grasp_metaheuristic(rectangle *pieces_cut, rectangle *stock, int num_pieces_cut);

unsigned int build_greedy_randomized_solution(rectangle *pieces_cut, int num_pieces_cut, rectangle stock, int lcr_size, int *track_solution);

int get_random_piece_to_cut(const rectangle *pieces_cut, int lenght_array_pieces);

unsigned int run_adaptive_search(rectangle stock, rectangle *pieces_cut, int num_pieces_cut, int *track_solution, unsigned int local_function);

unsigned int evaluate_solution(rectangle stock, rectangle *pieces_cut, int num_pieces_cut, int *solution, unsigned int size_solution);
