#include <memory.h>
#include <stdio.h>

typedef struct {
  int lin_ini;
  int col_ini;
  int lin_fin;
  int col_fin;
  int area;
  unsigned int active;
} cut_rectangle;


typedef struct {
  unsigned int type;
  float value;
  unsigned int length;
  unsigned int width;
  float calculated_value;
  int** content;
} rectangle;


void rectangle_selection_sort(rectangle* array, const int length);

void clean_active_cut_rectangles(cut_rectangle* list_cut, int num_elements);

void destroy_rectangle(rectangle* rec);

void print_rectangle(rectangle rec);

void print_cut_rectangle(rectangle rec);