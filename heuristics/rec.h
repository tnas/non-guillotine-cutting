#include <stdio.h>

typedef struct {
  unsigned int type;
  float value;
  unsigned int length;
  unsigned int width;
  int** content;
} rectangle;


void build_rectangle(rectangle* rec);

void destroy_rectangle(rectangle* rec);

void print_rectangle(rectangle rec);