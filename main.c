#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "rectangle/rec.h"

const char* DIR_TEST = "../in/";

void print_help();

int main(int argc, char **argv) {

    //rectangle rectangle_list[] = NULL;

    if (argc != 4) {
        print_help();
        exit(1);
    }

    char *file_name = NULL;
    char *opt_heuristic = NULL;

    if (strcmp(argv[1], "-f") == 0) {
        file_name = argv[2];
        opt_heuristic = argv[3];
    }
    else if (strcmp(argv[2], "-f") == 0) {
        file_name = argv[3];
        opt_heuristic = argv[1];
    }
    else {
        print_help();
        exit(1);
    }
    
    char* path_file = malloc(strlen(DIR_TEST) + strlen(file_name));
    strcat(path_file, DIR_TEST);
    strcat(path_file, file_name);

    FILE *file_descriptor;

    if ((file_descriptor = fopen(path_file, "r")) == NULL) {
        printf("Erro: %s (File: %s)\n", strerror(errno), path_file);
        exit(1);
    }
    
    rectangle stock;
    unsigned num_pieces_cut;
    
    fscanf(file_descriptor, "%u %u %u", &stock.length, &stock.width, &num_pieces_cut);
    stock.type = 0;
    print_rectangle(stock);
    
    
    fclose(file_descriptor);
    free(path_file);

    return 0;
}

void print_help() {
    printf("Informe os seguintes parâmetros para a execução do programa:\n");
    printf("-f <nome do arquivo de entrada situado no diretório ./in>\n");
    printf("-hc <para execução da heurística construtiva>\n");
    printf("-hm <para execução da heurística de melhoramento>\n");
    printf("-mh <para execução da meta-heurística\n");
}



