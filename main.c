#include "config.h"

int main(int argc, char **argv) {

    line_argument argument;
    struct timeval start_time, end_time;
    struct rusage ru;
    double time_initial, time_final;
    unsigned int objective_function;
    
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
    
    rectangle pieces_cut[num_pieces_cut];
    int count_piece = 0;
    
    
    while (fscanf(file_descriptor, "%u %u %u %f", 
	&(pieces_cut[count_piece].type), &(pieces_cut[count_piece].length), 
	&(pieces_cut[count_piece].width), &(pieces_cut[count_piece].value)
    ) != EOF) build_rectangle(&pieces_cut[count_piece++]);
    
    fclose(file_descriptor);
    
    if (!strcmp(argument.opt_heuristic, ARG_FF)) {
      
      getrusage(RUSAGE_SELF, &ru);
      start_time = ru.ru_utime;
      objective_function = run_first_fit_constructive_heuristic(pieces_cut, &stock, num_pieces_cut);
      int i = 0; while (i < 10000000) ++i;
      getrusage(RUSAGE_SELF, &ru);
      end_time = ru.ru_utime;
      
      printf("Solução ótima gerada pela heurística construtiva first-fit:\n");
      print_cut_rectangle(stock);
      printf("Valor maximizado: %d\n", objective_function);
      
      time_initial = start_time.tv_sec * 1000000 + (start_time.tv_usec);
      time_final = end_time.tv_sec * 1000000 + (end_time.tv_usec);
      printf("Tempo de execucao: %.2f milisegundos\n", time_final - time_initial);
    }
    else if (!strcmp(argument.opt_heuristic, ARG_BF)) {
      
      getrusage(RUSAGE_SELF, &ru);
      start_time = ru.ru_utime;
      objective_function = run_best_fit_constructive_heuristic(pieces_cut, &stock, num_pieces_cut);
      int i = 0; while (i < 10000000) ++i;
      getrusage(RUSAGE_SELF, &ru);
      end_time = ru.ru_utime;
      
      printf("Solução ótima gerada pela heurística construtiva best-fit:\n");
      print_cut_rectangle(stock);
      printf("Valor maximizado: %d\n", objective_function);
      
      time_initial = start_time.tv_sec * 1000000 + (start_time.tv_usec);
      time_final = end_time.tv_sec * 1000000 + (end_time.tv_usec);
      printf("Tempo de execução: %.2f milisegundos\n", time_final - time_initial);
    }
    else if (!strcmp(argument.opt_heuristic, ARG_HM)) {
      
      getrusage(RUSAGE_SELF, &ru);
      start_time = ru.ru_utime;
      objective_function = run_improvement_heuristic(pieces_cut, &stock, num_pieces_cut);
      int i = 0; while (i < 10000000) ++i;
      getrusage(RUSAGE_SELF, &ru);
      end_time = ru.ru_utime;
      
      printf("Solução ótima gerada pela heurística de melhoramento:\n");
      print_cut_rectangle(stock);
      printf("Valor maximizado: %d\n", objective_function);
      
      time_initial = start_time.tv_sec * 1000000 + (start_time.tv_usec);
      time_final = end_time.tv_sec * 1000000 + (end_time.tv_usec);
      printf("Tempo de execução: %.2f milisegundos\n", time_final - time_initial);
    }
    else if (!strcmp(argument.opt_heuristic, ARG_MH)) {
      
      time_initial = get_time();
      objective_function = run_grasp_metaheuristic(pieces_cut, &stock, num_pieces_cut);
      time_final = get_time();
      
      printf("Solucao otima gerada pela meta-heuristica GRASP:\n");
      print_cut_rectangle(stock);
      printf("Valor maximizado: %d\n", objective_function);
      printf("Tempo de execucao: %.5f segundos\n", (time_final - time_initial)/100.0);
    }
    
    for (count_piece = 0; count_piece < num_pieces_cut; 
	 destroy_rectangle(&pieces_cut[count_piece++]));

    return 0;
}





