#include "heuristic.h"


const unsigned int GRASP_LOOP = 100;
const float GRASP_LRC_PARAM = 0.25;


/* ***************************
 * Heurística Construtiva
 * ***************************
 */
unsigned int run_constructive_heuristic(rectangle *pieces_cut, rectangle *stock, int num_pieces_cut) {
  
  register int count_pieces;
  int objective_function = 0;
  
  // Ordenação decrescente [value/(width * length)] das peças 
  rectangle_selection_sort(pieces_cut, num_pieces_cut);
  
  for (count_pieces = 0; count_pieces < num_pieces_cut; ++count_pieces) {
    if (cut_piece_stock(stock, pieces_cut[count_pieces]))
      objective_function += pieces_cut[count_pieces].value;
  }
  
  return objective_function;
}




/* ***************************
 * Heurística de Melhoramento
 * ***************************
 */
unsigned int run_improvement_heuristic(rectangle *pieces_cut, rectangle* stock, int num_pieces_cut, POLICY_GET_PIECE policy) {
  
  int index_piece = 0;
  int objective_function = 0;
  cut_rectangle* set_cut_rectangles;
  unsigned int stock_area = stock->width * stock->length;
  
  // Inicializando set_cut_rectangles
   set_cut_rectangles = malloc(stock_area * sizeof(cut_rectangle));

  // Ordenação decrescente [value/(width * length)] das peças 
  rectangle_selection_sort(pieces_cut, num_pieces_cut);
  
  //for (count_pieces = 0; count_pieces < num_pieces_cut; ++count_pieces) {
  while ((index_piece = get_piece_to_cut(pieces_cut, num_pieces_cut, policy)) != -1) {  
    
    pieces_cut[index_piece].is_evaluated = 1;
    
    build_set_cut_rectangles(*stock, set_cut_rectangles);
    
    if (cut_lower_piece_stock(stock, &pieces_cut[index_piece], set_cut_rectangles)) 
      objective_function += pieces_cut[index_piece].value;
  }
  
  free(set_cut_rectangles);
  
  return objective_function;
}


int get_piece_to_cut(rectangle *pieces_cut, int num_pieces_cut, POLICY_GET_PIECE policy) {
  
  register unsigned int count_piece, has_to_eval, index_new_piece;
  
  switch (policy) {
    
    case SEQUENTIAL: {
      for (count_piece = 0; count_piece < num_pieces_cut; ++count_piece) 
	if (!pieces_cut[count_piece].is_evaluated) return count_piece;
      break;
    }
    case RANDOMIC: {
      count_piece = has_to_eval = 0;
      
      for (count_piece = 0; count_piece < num_pieces_cut; ++count_piece) {
	if (!pieces_cut[count_piece].is_evaluated) {
	  has_to_eval = 1;
	  break;
	}
      }
      
      if (has_to_eval) {
	do {
	  index_new_piece = rand() % num_pieces_cut;
	} while (pieces_cut[index_new_piece].is_evaluated);
	
	return index_new_piece;
      }
      
      break;
    }
  }
  
  return -1;
}



/* ***************************
 * Meta-heurística GRASP
 * ***************************
 */

unsigned int run_grasp_metaheuristic(rectangle *pieces_cut, rectangle *stock, int num_pieces_cut) {
  
  unsigned int lcr_size, objective_function, local_solution, candidate_index;
  register unsigned int c_grasp_iter;
  
  srand(time(NULL));
  
  lcr_size = num_pieces_cut * GRASP_LRC_PARAM;
  rectangle candidates_list[lcr_size];
  objective_function = local_solution = candidate_index = 0;
  
  // Ordenação decrescente [value/(width * length)] das peças 
  rectangle_selection_sort(pieces_cut, num_pieces_cut);
  
  //for (c_grasp_iter = 0; c_grasp_iter < GRASP_LOOP; ++c_grasp_iter) {
    
    build_greedy_randomized_solution(candidates_list, lcr_size, pieces_cut, &candidate_index, num_pieces_cut, stock);
    
  //}
    
    return objective_function;
}


void build_greedy_randomized_solution(rectangle* candidates_list, const unsigned int size_candidates_list,
  rectangle *pieces_cut, unsigned int *next_index_piece, const int num_pieces_cut, rectangle* stock) {
  
  register unsigned int c_iter_cand;
  
  for (c_iter_cand = 0; c_iter_cand < size_candidates_list; ++c_iter_cand) {
    
    if (!candidates_list[c_iter_cand].is_cutted && *next_index_piece < num_pieces_cut) {
      candidates_list[c_iter_cand] = pieces_cut[*next_index_piece++];
    }
  }
  
  run_improvement_heuristic(candidates_list, stock, size_candidates_list, RANDOMIC);
}

void run_adaptive_search() {
}



