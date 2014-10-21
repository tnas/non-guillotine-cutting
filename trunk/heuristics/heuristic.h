#include <time.h>
#include <stdlib.h>

#include "rec.h"

extern const unsigned int GRASP_LOOP;
extern const float GRASP_LRC_PARAM;

typedef enum { SEQUENTIAL, RANDOMIC } POLICY_GET_PIECE;


void run_constructive_heuristic(rectangle* pieces_cut, rectangle stock, int num_pieces_cut);

unsigned int run_improvement_heuristic(rectangle *pieces_cut, rectangle *stock, int num_pieces_cut, POLICY_GET_PIECE policy);

void run_grasp_metaheuristic(rectangle *pieces_cut, rectangle stock, int num_pieces_cut);
