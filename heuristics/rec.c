#include "rec.h"


void rectangle_selection_sort(rectangle* array, const int length) {
  
  register int index, max_index;
  int max_temp_index;
  rectangle rec_temp, rec_max_temp;
  
  for (index = 0; index < length - 1; ++index) {
    
    rec_max_temp = *(array + index);
    max_temp_index = index;
    
    // Looking for the min element between [index, length]
    for (max_index = index + 1; max_index < length; ++max_index)      
      if (array[max_index].calculated_value > rec_max_temp.calculated_value) {
	rec_max_temp = *(array + max_index);
	max_temp_index = max_index;
      }
    // Changing the min element by the first element
    if (rec_max_temp.calculated_value > array[index].calculated_value) {
      rec_temp = *(array + index);
      *(array + index) = rec_max_temp;
      *(array + max_temp_index) = rec_temp;
    }
  }  
}


void clean_active_cut_rectangles(cut_rectangle* list_cut, int num_elements) {

  register int count;
  for (count = 0; count < num_elements; ++count) {
    list_cut[count].active = 0;
    list_cut[count].area    = 
    list_cut[count].col_fin =
    list_cut[count].col_ini = 
    list_cut[count].lin_fin =
    list_cut[count].lin_ini = -1;
  }
}


void build_rectangle(rectangle* rec) {
  
  register int count_width, count_length; 
  
  rec->content = malloc(rec->width * sizeof(unsigned int*));
  
  for (count_width = 0; count_width < rec->width; ++count_width) {
	
    rec->content[count_width] = malloc(rec->length * sizeof(unsigned int));
    
    for (count_length = 0; count_length < rec->length; ++count_length) {
      rec->content[count_width][count_length] = rec->type;
    }
  }
  
  // Setting rectangle's efficiency value
  rec->calculated_value = rec->value / (rec->width * rec->length);
  rec->is_cutted = rec->is_evaluated = 0;
}


void destroy_rectangle(rectangle* rec) {
  
  register int count_width; 
  
  for (count_width = 0; count_width < rec->width; ++count_width) {
    free(rec->content[count_width]);
  }
  
  free(rec->content);
  
}


void print_rectangle(rectangle rec) {

  register unsigned int lin, col;
  
  for (lin = 0; lin < rec.width; ++lin) {
    
    for (col = 0; col < rec.length; ++col) {
      printf("%02u ", rec.content[lin][col]);
    }
    printf("\n");
  }
}


void print_cut_rectangle(rectangle rec) {

  register unsigned int lin, col;
  
  for (lin = 0; lin < rec.width; ++lin) {
    
    for (col = 0; col < rec.length; ++col) {
      if (rec.content[lin][col]) printf("%02u ", rec.content[lin][col]);
      else printf("   ");
    }
    printf("\n");
  }
}


void print_set_cut_rectangles(const cut_rectangle* set_cut_rec, int num_elements) {
  
  register unsigned int count;
  
  for (count = 0; count < num_elements; ++count) {
    
    if (set_cut_rec[count].active) {
      printf("[lin_ini: %d, col_ini: %d, lin_fin: %d, col_fin: %d, area: %d]\n", 
	set_cut_rec[count].lin_ini, set_cut_rec[count].col_ini,
	set_cut_rec[count].lin_fin, set_cut_rec[count].col_fin, set_cut_rec[count].area);
    }
  }
}


void clean_rectangle(rectangle* rec) {
  
  register unsigned int lin, col;
  
  for (lin = 0; lin < rec->width; ++lin) {
    for (col = 0; col < rec->length; ++col) {
      rec->content[lin][col] = rec->type;
    }
  }
  
  rec->is_cutted = rec->is_evaluated = 0;
}


void reset_status_rectangles(rectangle* rec, unsigned int size) {

  register unsigned int count;
  
  for (count = 0; count < size; ++count) 
    rec[count].is_cutted = rec[count].is_evaluated = 0;
  
}



int cut_piece_stock(rectangle* stock, rectangle piece) {
  
  register int c_stock_width, c_stock_length, c_stock_local_width, c_stock_local_length;
  int fit_ok = 0;
  
  if (piece.length > stock->length || piece.width > stock->width || piece.is_cutted) 
    return 0;
  
  for (c_stock_width = 0; c_stock_width <= stock->width - piece.width; ++c_stock_width) {
    
    for (c_stock_length = 0; c_stock_length <= stock->length - piece.length; ++c_stock_length) {
      
      // Verificando se a posição do palete já não foi preenchida (ou cortada)
      if (stock->content[c_stock_width][c_stock_length] != stock->type) continue;
      
      // Verificando se o tamanho da peça na posição (c_stock_width, c_stock_length)
      // não excede o tamanho do palete
      if (c_stock_width + piece.width > stock->width ||
	  c_stock_length + piece.length > stock->length) continue;
      
      // Verificando se a área da peça cabe inteira numa área não-preenchida do palete
      fit_ok = 1; // supondo que a peça cabe
      for (c_stock_local_width = c_stock_width; (c_stock_local_width < c_stock_width + piece.width) && fit_ok; ++c_stock_local_width) {
	
	for (c_stock_local_length = c_stock_length; (c_stock_local_length < c_stock_length + piece.length) && fit_ok; ++c_stock_local_length) {
	  
	  if (stock->content[c_stock_local_width][c_stock_local_length] != stock->type) fit_ok = 0;
	}
      }
      
      // Alocando peça no palete
      if (fit_ok) {
	
	for (c_stock_local_width = c_stock_width; (c_stock_local_width < c_stock_width + piece.width) && fit_ok; ++c_stock_local_width) {
	
	  for (c_stock_local_length = c_stock_length; (c_stock_local_length < c_stock_length + piece.length) && fit_ok; ++c_stock_local_length) {
	  
	    stock->content[c_stock_local_width][c_stock_local_length] = piece.type;
	    piece.is_cutted = 1;
	  }
	}
	
	return fit_ok;
      }
    }
  }
  
  return fit_ok;
}


int cut_lower_piece_stock(rectangle* stock, rectangle *piece, cut_rectangle *cut_list) {
  
  register int c_stock_width, c_stock_length, c_stock_local_width, c_stock_local_length, c_cut_set;
  int fit_ok, min_cut_area;
  
  if (piece->length > stock->length || piece->width > stock->width) return 0;

  for (c_cut_set = 0, fit_ok = 0, min_cut_area = stock->width * stock->length; 
    c_cut_set < (stock->width * stock->length) && cut_list[c_cut_set].active; 
    ++c_cut_set) {
    
    if (is_inner_cut_retangle(cut_list[c_cut_set], *piece) && cut_list[c_cut_set].area <= min_cut_area) {
      fit_ok = 1;
      c_stock_width = cut_list[c_cut_set].lin_ini;
      c_stock_length = cut_list[c_cut_set].col_ini;
      min_cut_area = cut_list[c_cut_set].area;
    }
  }

  // Alocando peça no palete
  if (fit_ok) {
    
    for (c_stock_local_width = c_stock_width; c_stock_local_width < c_stock_width + piece->width; ++c_stock_local_width) {
    
      for (c_stock_local_length = c_stock_length; c_stock_local_length < c_stock_length + piece->length; ++c_stock_local_length) {
      
	stock->content[c_stock_local_width][c_stock_local_length] = piece->type;
	piece->is_cutted = 1;
      }
    }
  }
  
  return fit_ok;
}



void build_set_cut_rectangles(rectangle stock, cut_rectangle* set_cut_rec) {
  
  register int c_stock_width, c_stock_length, c_stock_local_width, c_stock_local_length, c_fix_stock_local_length;
  int c_cut_rec = 0;
  cut_rectangle cut_rec_candidate;
  int stock_area = stock.width * stock.length;
  
  clean_active_cut_rectangles(set_cut_rec, stock_area);
    
  for (c_stock_width = 0; c_stock_width < stock.width; ++c_stock_width) {
  
    for (c_stock_length = 0; c_stock_length < stock.length; ++c_stock_length) {
      
	if (is_node_empty(c_stock_width, c_stock_length, stock)) {
	  
	  // Inicializando cut_rectangle candidate
	  cut_rec_candidate.lin_ini = c_stock_width;
	  cut_rec_candidate.col_ini = c_stock_length;
	  
	  c_fix_stock_local_length = stock.length;
	  
	  for (c_stock_local_width = c_stock_width; c_stock_local_width < stock.width; ++c_stock_local_width) {
	    
	    // Verificando se o primeiro node do cut_rectangle está vazio
	    if (!is_node_empty(c_stock_local_width, c_stock_length, stock)) break;
	    
	    for (c_stock_local_length = c_stock_length; c_stock_local_length < c_fix_stock_local_length; ++c_stock_local_length) {
	      
	      if (!is_node_empty(c_stock_local_width, c_stock_local_length, stock)) {
		c_fix_stock_local_length = c_stock_local_length;
	      }
	    }
	    
	    cut_rec_candidate.col_fin = c_fix_stock_local_length - 1;
	    cut_rec_candidate.lin_fin = c_stock_local_width;
	  }
	  
	  // Calculando a area do cut_rec_candidate
	  cut_rec_candidate.area = (cut_rec_candidate.lin_fin - cut_rec_candidate.lin_ini + 1) *
	    (cut_rec_candidate.col_fin - cut_rec_candidate.col_ini + 1);
	    
	  // Verificando se o cut_rec_candidate não está completamente contido em algum 
	  // cut_rectangle previamente encontrado
	  if (!is_rectangle_area_in_list_cut_retangles(set_cut_rec, stock_area, cut_rec_candidate)) {	    
	    set_cut_rec[c_cut_rec].active = 1;
	    set_cut_rec[c_cut_rec].lin_ini = cut_rec_candidate.lin_ini;
	    set_cut_rec[c_cut_rec].lin_fin = cut_rec_candidate.lin_fin;
	    set_cut_rec[c_cut_rec].col_ini = cut_rec_candidate.col_ini;
	    set_cut_rec[c_cut_rec].col_fin = cut_rec_candidate.col_fin;
	    set_cut_rec[c_cut_rec].area    = cut_rec_candidate.area; 
	    ++c_cut_rec;
	  }
	}
    }
  }
}


int is_node_empty(unsigned int line, unsigned column, const rectangle stock) {
  return stock.content[line][column] == stock.type; 
}


int is_inner_cut_retangle(const cut_rectangle cut_rec, const rectangle rec) {
  return cut_rec.lin_ini + rec.width - 1 <= cut_rec.lin_fin &&
         cut_rec.col_ini + rec.length - 1 <= cut_rec.col_fin;
}
  
int is_rectangle_area_in_list_cut_retangles(const cut_rectangle* set_cut_rec, int num_elements, const cut_rectangle cut_rec) {
  
  register unsigned int count;
  
  for (count = 0; count < num_elements; ++count) {
    
    if (set_cut_rec[count].active) {
      if (cut_rec.lin_ini >= set_cut_rec[count].lin_ini && cut_rec.lin_fin <= set_cut_rec[count].lin_fin &&
	  cut_rec.col_ini >= set_cut_rec[count].col_ini && cut_rec.col_fin <= set_cut_rec[count].col_fin) return 1;
      }
  }
  
  return 0;
}