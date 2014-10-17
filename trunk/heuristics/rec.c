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


/*void init_list_cut_rectangle(cut_rectangle** list_cut) {
  build_rectangle(rec);
  rec->list_cut_rectangle = malloc(rec->width * rec->length * sizeof(cut_rectangle));
}*/

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
  
  // Setting rectangle's calculated value
  rec->calculated_value = rec->value / (rec->width * rec->length);
}


void destroy_rectangle(rectangle* rec) {
  
  register int count_width, count_length; 
  
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