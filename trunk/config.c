#include "config.h"

const char* DIR_TEST = "../in/";
const char* ARG_FF = "-ff";
const char* ARG_BF = "-bf";
const char* ARG_HM = "-hm";
const char* ARG_MH = "-mh";

void load_line_argument(int argc, char **argv, line_argument* argument) {
   
  if (argc != 4) {
      print_help();
      exit(1);
  }

  if (strcmp(argv[1], "-f") == 0) {
    argument->file_name = argv[2];
    argument->opt_heuristic = argv[3];
  }
  else if (strcmp(argv[2], "-f") == 0) {
    argument->file_name = argv[3];
    argument->opt_heuristic = argv[1];
  }
  else {
    print_help();
    exit(1);
  }
}

void print_help() {
    printf("Informe os seguintes parâmetros para a execução do programa:\n");
    printf("-f <nome do arquivo de entrada situado no diretório ./in>\n");
    printf("-hc <para execução da heurística construtiva>\n");
    printf("-hm <para execução da heurística de melhoramento>\n");
    printf("-mh <para execução da meta-heurística\n");
}