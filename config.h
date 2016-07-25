#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <limits.h>
#include <math.h>

#include "heuristics/heuristic.h"
#define TEST_EXEC_TIMES 5

extern const char* DIR_TEST;
extern const char* ARG_FF;
extern const char* ARG_BF;
extern const char* ARG_HM;
extern const char* ARG_MH;
extern const char* ARG_RUN_TEST;

typedef struct {
  char *file_name;
  char *opt_heuristic;
} line_argument;

typedef struct 
{
	double average_value;
	double standard_deviation;
} result;

double get_time ();
void load_line_argument(int argc, char **argv, line_argument* argument);
void print_help();
void run_all_metaheuristic_tests();