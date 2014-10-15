#include <stdio.h>

typedef struct {
  unsigned int type;
  float value;
  unsigned int length;
  unsigned int width;
  float calculated_value;
  int** content;
} rectangle;


void rectangle_selection_sort(rectangle* array, const int length);

void build_rectangle(rectangle* rec);

void destroy_rectangle(rectangle* rec);

void print_rectangle(rectangle rec);

void print_cut_rectangle(rectangle rec);