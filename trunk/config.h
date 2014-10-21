#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "heuristics/heuristic.h"

extern const char* DIR_TEST;
extern const char* ARG_HC;
extern const char* ARG_HM;
extern const char* ARG_MH;

typedef struct {
  char *file_name;
  char *opt_heuristic;
} line_argument;


void load_line_argument(int argc, char **argv, line_argument* argument);

void print_help();