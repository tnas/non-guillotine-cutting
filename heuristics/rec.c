#include <memory.h>
#include "rec.h"



void build_rectangle(rectangle* rec) {
  
  register int count_width, count_length; 
  
  rec->content = malloc(rec->width * sizeof(unsigned int*));
  
  for (count_width = 0; count_width < rec->width; ++count_width) {
	
    rec->content[count_width] = malloc(rec->length * sizeof(unsigned int));
    
    for (count_length = 0; count_length < rec->length; ++count_length) {
      rec->content[count_width][count_length] = rec->type;
    }
  }
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