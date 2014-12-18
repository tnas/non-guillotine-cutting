#include "heuristic.h"


const unsigned int GRASP_LOOP = 1000;
const float GRASP_LRC_PARAM = 0.50;


/* *********************************
 * Heurística Construtiva First-Fit
 * *********************************
 */
unsigned int run_first_fit_constructive_heuristic(rectangle *pieces_cut, rectangle *stock, int num_pieces_cut) {
  
  register int count_pieces;
  int objective_function = 0;
  
  for (count_pieces = 0; count_pieces < num_pieces_cut; ++count_pieces) {
    if (cut_piece_stock(stock, pieces_cut[count_pieces]))
      objective_function += pieces_cut[count_pieces].value;
  }
  
  return objective_function;
}




/* ********************************
 * Heurística Construtiva Best-Fit
 * ********************************
 */
unsigned int run_best_fit_constructive_heuristic(rectangle *pieces_cut, rectangle* stock, int num_pieces_cut) {
  
  int index_piece = 0;
  int objective_function = 0;
  cut_rectangle* set_cut_rectangles;
  unsigned int stock_area = stock->width * stock->length;
  
  // Inicializando set_cut_rectangles
   set_cut_rectangles = malloc(stock_area * sizeof(cut_rectangle));

  for (index_piece = 0; index_piece < num_pieces_cut; ++index_piece) {
    
    pieces_cut[index_piece].is_evaluated = 1;
    
    build_set_cut_rectangles(*stock, set_cut_rectangles);
    
    if (cut_lower_piece_stock(stock, &pieces_cut[index_piece], set_cut_rectangles)) {
      objective_function += pieces_cut[index_piece].value;
    }
  }
  
  free(set_cut_rectangles);
  
  return objective_function;
}



/* *********************************
 * Heurística de Melhoramento 
 * *********************************
 */


unsigned int run_improvement_heuristic(rectangle *pieces_cut, rectangle *stock, int num_pieces_cut) {
  
  unsigned int track_pos, solution;
  register unsigned int count_pieces;
  
  solution = track_pos = 0;
  int track_solution[num_pieces_cut];

  // Ordenação decrescente [value/(width * length)] das peças 
  rectangle_selection_sort(pieces_cut, num_pieces_cut);
  
  // Obtendo solução inicial
  for (count_pieces = 0; count_pieces < num_pieces_cut; ++count_pieces) {
    if (cut_piece_stock(stock, pieces_cut[count_pieces])) {
      solution += pieces_cut[count_pieces].value;
      track_solution[track_pos++] = count_pieces;
    }
  }
  
  for (; track_pos < num_pieces_cut; track_solution[track_pos++] = -1);
   
  // Executando melhoramento via análise da vizinhança
  solution = run_adaptive_search(*stock, pieces_cut, num_pieces_cut, track_solution, solution);
 
  clean_rectangle(stock);
  reset_status_rectangles(pieces_cut, num_pieces_cut);
  
  solution = 0;
  for (count_pieces = 0; count_pieces < num_pieces_cut; ++count_pieces) {
    if (track_solution[count_pieces] != -1 && cut_piece_stock(stock, pieces_cut[track_solution[count_pieces]])) {
      solution += pieces_cut[track_solution[count_pieces]].value;
    }
  }
  
  return solution;
  
}

/*
unsigned int run_new_adaptive_search(rectangle stock, rectangle *pieces_cut, int num_pieces_cut, int *track_solution, unsigned int local_function) {
  
  register unsigned int counter, ext_counter, num_solution, index_item_solution;
  unsigned int size_track_solution, extended_size_track_solution, pivot_index, neighbor_index, neighbor_function;
  int **neighborhood;
  int mem_solution;
  
  // Dimensão da solução
  size_track_solution = 0;
  for (counter = 0; counter < num_pieces_cut; ++counter)
    if (track_solution[counter] != -1) size_track_solution++;
  
  // Parte estendida da solução - 30/10/2014
  extended_size_track_solution = num_pieces_cut - size_track_solution;
  int extended_track_solution[extended_size_track_solution];
  
  // Alocando vizinhança da solução  
  neighborhood = malloc((size_track_solution - 1) * sizeof(int*));
  for (counter = 0; counter < size_track_solution - 1; ++counter)
    neighborhood[counter] = malloc(size_track_solution * sizeof(int));
  
  // Selecionando pivô para gerar vizinhança de solução
  pivot_index = rand() % size_track_solution;
  
  // Gerando vizinhança e verificando melhor solução local
  for (num_solution = 0; num_solution < size_track_solution - 1; ++num_solution) {
    
    for (index_item_solution = 0; index_item_solution < size_track_solution; ++index_item_solution) {
	neighborhood[num_solution][index_item_solution] = track_solution[index_item_solution];
    }
    
    // Executando permuta com o pivot_index
    neighbor_index = num_solution < pivot_index ? num_solution : num_solution + 1;
    mem_solution = neighborhood[num_solution][neighbor_index];
    neighborhood[num_solution][neighbor_index] = track_solution[pivot_index];
    neighborhood[num_solution][pivot_index] = mem_solution;
    
    neighbor_function = evaluate_solution(stock, pieces_cut, num_pieces_cut, neighborhood[num_solution], size_track_solution);
    
    // Verificando se alguma peça pode ser alocada com a nova solução encontrada - 30/10/2014
    ext_counter = 0;
    for (counter = 0; counter < num_pieces_cut; ++counter) {
      if (!pieces_cut[counter].is_cutted && cut_piece_stock(&stock, pieces_cut[counter])) {
	neighbor_function += pieces_cut[counter].value;
	extended_track_solution[ext_counter++] = pieces_cut[counter].type;
      }
    }
    
    if (neighbor_function > local_function) {
      
      local_function = neighbor_function;
      
      for (counter = 0; counter < size_track_solution; ++counter)
	track_solution[counter] = neighborhood[num_solution][counter];
      
      for (; counter < size_track_solution + ext_counter; ++counter) 
	track_solution[counter] = extended_track_solution[counter];
      
      for (; counter < num_pieces_cut; ++counter) 
	track_solution[counter] = - 1;
    }
  }
  
  // Destruindo vizinhança da solução
  for (counter = 0; counter < size_track_solution - 1; ++counter) free(neighborhood[counter]);
  free(neighborhood);
  
  return local_function;
}
*/


/* ***************************
 * Meta-heurística GRASP
 * ***************************
 */

unsigned int run_grasp_metaheuristic(rectangle *pieces_cut, rectangle *stock, int num_pieces_cut) {
  
  unsigned int lcr_size, objective_function, local_solution;
  register unsigned int c_grasp_iter, c_item_solution;
  
  srand(time(NULL));
  
  // Ordenação decrescente [value/(width * length)] das peças 
  rectangle_selection_sort(pieces_cut, num_pieces_cut);
  
  lcr_size = num_pieces_cut * GRASP_LRC_PARAM;
  objective_function = local_solution = 0;
  int track_solution[num_pieces_cut], solution[num_pieces_cut];
  
  for (c_grasp_iter = 0; c_grasp_iter < GRASP_LOOP; ++c_grasp_iter) {
    
    local_solution = build_greedy_randomized_solution(pieces_cut, num_pieces_cut, *stock, lcr_size, track_solution);
    
    clean_rectangle(stock);
    reset_status_rectangles(pieces_cut, num_pieces_cut);
    
    local_solution = run_adaptive_search(*stock, pieces_cut, num_pieces_cut, track_solution, local_solution);
    
    if (local_solution > objective_function) { 
      
      objective_function = local_solution;
      
      for (c_item_solution = 0; c_item_solution < num_pieces_cut; ++c_item_solution) 
	solution[c_item_solution] = track_solution[c_item_solution];
    }
  }
  
  evaluate_solution(*stock, pieces_cut, num_pieces_cut, solution, num_pieces_cut);
  
  return objective_function;
}




unsigned int build_greedy_randomized_solution(rectangle *pieces_cut, int num_pieces_cut, rectangle stock, int lcr_size, int *track_solution) {
  
  int index_candidate;
  register unsigned int c_iter_cand;
  cut_rectangle *set_cut_rectangles;
  unsigned int objective_function;
  
  // Inicializando set_cut_rectangles
   set_cut_rectangles = malloc(stock.length * stock.width * sizeof(cut_rectangle));
  
  // Reset no track_solution e pieces_cut
  for (c_iter_cand = 0; c_iter_cand < num_pieces_cut; track_solution[c_iter_cand++] = -1) ;
  reset_status_rectangles(pieces_cut, num_pieces_cut);
  clean_rectangle(&stock);
  
  objective_function = c_iter_cand = 0;
  
  while (lcr_size < num_pieces_cut) {
    
    if ((index_candidate = get_random_piece_to_cut(pieces_cut, lcr_size)) != -1) {
      
	pieces_cut[index_candidate].is_evaluated = 1;
	
	build_set_cut_rectangles(stock, set_cut_rectangles);
	
	if (cut_lower_piece_stock(&stock, &pieces_cut[index_candidate], set_cut_rectangles)) {
	  track_solution[c_iter_cand++] = index_candidate;
	  objective_function += pieces_cut[index_candidate].value;
	}
	
	lcr_size++;
    }
  } 
  
  free(set_cut_rectangles);
  
  return objective_function;
}


int get_random_piece_to_cut(const rectangle *pieces_cut, int lenght_array_pieces) {
  
  register unsigned int count_piece, has_to_eval, index_new_piece;
  
  count_piece = has_to_eval = 0;
  
  for (count_piece = 0; count_piece < lenght_array_pieces; ++count_piece) {
    if (!pieces_cut[count_piece].is_evaluated) {
      has_to_eval = 1;
      break;
    }
  }
  
  if (has_to_eval) {
    do {
      index_new_piece = rand() % lenght_array_pieces;
    } while (pieces_cut[index_new_piece].is_evaluated);
    
    return index_new_piece;
  }
      
  return -1;
}



unsigned int run_adaptive_search(rectangle stock, rectangle *pieces_cut, int num_pieces_cut, int *track_solution, unsigned int local_function) {
  
  register unsigned int counter, num_solution, index_item_solution;
  unsigned int size_track_solution, pivot_index, neighbor_index, neighbor_function;
  int **neighborhood;
  int mem_solution;
  
  // Dimensão da solução
  size_track_solution = 0;
  for (counter = 0; counter < num_pieces_cut; ++counter)
    if (track_solution[counter] != -1) size_track_solution++; 
  
  // Alocando vizinhança da solução  
  neighborhood = malloc((size_track_solution - 1) * sizeof(int*));
  for (counter = 0; counter < size_track_solution - 1; ++counter)
    neighborhood[counter] = malloc(size_track_solution * sizeof(int));
  
  // Selecionando pivô para gerar vizinhança de solução
  pivot_index = rand() % size_track_solution;
  
  // Gerando vizinhança e verificando melhor solução local
  for (num_solution = 0; num_solution < size_track_solution - 1; ++num_solution) {
    
    for (index_item_solution = 0; index_item_solution < size_track_solution; ++index_item_solution) {
	neighborhood[num_solution][index_item_solution] = track_solution[index_item_solution];
    }
    
    // Executando permuta com o pivot_index
    neighbor_index = num_solution < pivot_index ? num_solution : num_solution + 1;
    mem_solution = neighborhood[num_solution][neighbor_index];
    neighborhood[num_solution][neighbor_index] = track_solution[pivot_index];
    neighborhood[num_solution][pivot_index] = mem_solution;
    
    neighbor_function = evaluate_solution(stock, pieces_cut, num_pieces_cut, neighborhood[num_solution], size_track_solution);
    
    if (neighbor_function > local_function) {
      
      local_function = neighbor_function;
      
      for (counter = 0; counter < size_track_solution; ++counter)
	track_solution[counter] = neighborhood[num_solution][counter];
      
      for (; counter < num_pieces_cut; ++counter) 
	track_solution[counter] = - 1;
    }
  }
  
  // Destruindo vizinhança da solução
  for (counter = 0; counter < size_track_solution - 1; ++counter) free(neighborhood[counter]);
  free(neighborhood);
  
  return local_function;
}


unsigned int evaluate_solution(rectangle stock, rectangle *pieces_cut, int num_pieces_cut, int *solution, unsigned int size_solution) {
  
  register unsigned int c_index_sol;
  cut_rectangle *set_cut_rectangles;
  unsigned int objective_function;
  
  // Inicializando set_cut_rectangles
   set_cut_rectangles = malloc(stock.length * stock.width * sizeof(cut_rectangle));
  
  // Reset pieces_cut
  reset_status_rectangles(pieces_cut, num_pieces_cut);
  
  // Reset stock
  clean_rectangle(&stock);
  
  objective_function = 0;
  
  for (c_index_sol = 0; c_index_sol < size_solution; ++c_index_sol) {
    
    if (solution[c_index_sol] < 0) continue;
    
    pieces_cut[solution[c_index_sol]].is_evaluated = 1;
    
    build_set_cut_rectangles(stock, set_cut_rectangles);
    
    if (cut_lower_piece_stock(&stock, &pieces_cut[solution[c_index_sol]], set_cut_rectangles)) {
      objective_function += pieces_cut[solution[c_index_sol]].value;
    }
  }
  
  free(set_cut_rectangles);
  
  return objective_function;
}

