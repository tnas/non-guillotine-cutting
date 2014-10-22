#include <malloc.h>
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
  unsigned int is_cutted;
  unsigned int is_evaluated;
} rectangle;


void rectangle_selection_sort(rectangle* array, const int length);

void clean_active_cut_rectangles(cut_rectangle* list_cut, int num_elements);

void build_rectangle(rectangle* rec);

void destroy_rectangle(rectangle* rec);

void clean_rectangle(rectangle* rec);

void reset_status_rectangles(rectangle *rec, unsigned int size);

void print_rectangle(rectangle rec);

void print_cut_rectangle(rectangle rec);

void print_set_cut_rectangles(const cut_rectangle* set_cut_rec, int num_elements);

void build_set_cut_rectangles(rectangle stock, cut_rectangle* set_cut_rec);

int is_rectangle_area_in_list_cut_retangles(const cut_rectangle* set_cut_rec, int num_elements, const cut_rectangle cut_rec);

int is_node_empty(unsigned int line, unsigned column, const rectangle stock);

int is_inner_cut_retangle(const cut_rectangle cut_rec, const rectangle rec);

int cut_lower_piece_stock(rectangle* stock, rectangle* piece, cut_rectangle *cut_list);

int cut_piece_stock(rectangle* stock, rectangle piece);