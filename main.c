#include "config.h"
#include "heuristics/heuristic.h"

int main(int argc, char **argv) {

    line_argument argument;
    load_line_argument(argc, argv, &argument);
    
    char* path_file = malloc(strlen(DIR_TEST) + strlen(argument.file_name));
    strcat(path_file, DIR_TEST);
    strcat(path_file, argument.file_name);

    FILE *file_descriptor;

    if ((file_descriptor = fopen(path_file, "r")) == NULL) {
        printf("Erro: %s (File: %s)\n", strerror(errno), path_file);
        exit(1);
    }
    free(path_file);
    
    rectangle stock;
    unsigned num_pieces_cut;
    
    fscanf(file_descriptor, "%u %u %u", &stock.length, &stock.width, &num_pieces_cut);
    stock.type = 0;
    build_rectangle(&stock);
    print_rectangle(stock);
    
    rectangle pieces_cut[num_pieces_cut];
    int count_piece = 0;
    
    
    while (fscanf(file_descriptor, "%u %u %u %f", 
	&(pieces_cut[count_piece].type), &(pieces_cut[count_piece].length), 
	&(pieces_cut[count_piece].width), &(pieces_cut[count_piece].value)
    ) != EOF) build_rectangle(&pieces_cut[count_piece++]);
    
    fclose(file_descriptor);
    
    if (!strcmp(argument.opt_heuristic, ARG_HC)) {
      run_constructive_heuristic(pieces_cut, stock, num_pieces_cut);
    }
    
    for (count_piece = 0; count_piece < num_pieces_cut; 
	 destroy_rectangle(&pieces_cut[count_piece++]));

    return 0;
}





