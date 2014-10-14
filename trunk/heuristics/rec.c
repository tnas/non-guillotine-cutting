#include <memory.h>
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
      printf("%u ", rec.content[lin][col]);
    }
    printf("\n");
  }
  
}