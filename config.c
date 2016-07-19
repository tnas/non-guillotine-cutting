#include "config.h"

const char* DIR_TEST = "./in/";
const char* ARG_FF = "-ff";
const char* ARG_BF = "-bf";
const char* ARG_HM = "-hm";
const char* ARG_MH = "-mh";
const char* ARG_RUN_TEST = "-rt";

void load_line_argument(int argc, char **argv, line_argument* argument) {
   
  if (argc < 2) {
      print_help();
      exit(1);
  }

  if (strcmp(argv[1], "-rt") == 0) {
    argument->opt_heuristic = argv[1];
  }
  else if (strcmp(argv[1], "-f") == 0) {
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

double get_time ()
{
	struct timeval tv; gettimeofday(&tv, 0);
	return (double)(tv.tv_sec * 100.0 + tv.tv_usec / 10000.0);
}

void print_help() {
    printf("Informe os seguintes parâmetros para a execução do programa:\n");
    printf("-f <nome do arquivo de entrada situado no diretório ./in>\n");
    printf("-ff <para execução da heurística construtiva first-fit>\n");
    printf("-bf <para execução da heurística construtiva best-fit>\n");
    printf("-hm <para execução da heurística de melhoramento>\n");
    printf("-mh <para execução da meta-heurística GRASP\n");
}

long int normalize_int_results(const long int* results)
{
	int exec_min_val, exec_max_val, exec;
	long int sum_val, max_val, min_val;
	
	max_val = sum_val = 0;
	min_val = INT_MAX;
	
	if (TEST_EXEC_TIMES == 1) return results[0];
	
	// Finding out max/min value
	for (exec = 0; exec < TEST_EXEC_TIMES; ++exec)
	{
		if (results[exec] > max_val)
		{
			exec_max_val = exec;
			max_val = results[exec];
		}
		else if (results[exec] < min_val)
		{
			exec_min_val = exec;
			min_val = results[exec];
		}
	}
	
	// Discarding max and min results value
	for (exec = 0; exec < TEST_EXEC_TIMES; ++exec)
	{
		if (exec != exec_min_val && exec != exec_max_val)
			sum_val += results[exec];
	}
	
	return sum_val / (TEST_EXEC_TIMES - 2);
}


double normalize_double_results(const double* results)
{
	int exec_min_val, exec_max_val, exec;
	double sum_val, max_val, min_val;
	
	max_val = sum_val = 0;
	min_val = INT_MAX;
	
	if (TEST_EXEC_TIMES == 1) return results[0];
	
	// Finding out max/min value
	for (exec = 0; exec < TEST_EXEC_TIMES; ++exec)
	{
		if (results[exec] > max_val)
		{
			exec_max_val = exec;
			max_val = results[exec];
		}
		else if (results[exec] < min_val)
		{
			exec_min_val = exec;
			min_val = results[exec];
		}
	}
	
	// Discarding max and min results value
	for (exec = 0; exec < TEST_EXEC_TIMES; ++exec)
	{
		if (exec != exec_min_val && exec != exec_max_val)
			sum_val += results[exec];
	}
	
	return sum_val / (TEST_EXEC_TIMES - 2);
}


void run_all_metaheuristic_tests()
{
	int count_instancia, exec, num_instancias, solucao_computada;
	FILE* out_file;
	FILE *file_descriptor;
	double time_initial, time_final, tempo_computado;
	double tempo_exec[TEST_EXEC_TIMES];
	long int solucoes[TEST_EXEC_TIMES];
	rectangle stock;
	unsigned num_pieces_cut;
	
	/* *******************************
	 * Definition of tests parameters
	 * *******************************
	 */
	char* instancias[] = {
		"../in/beasley_1.txt",
		"../in/beasley_2.txt",
		"../in/beasley_3.txt",
		"../in/beasley_4.txt",
		"../in/beasley_5.txt",
		"../in/beasley_6.txt",
		"../in/beasley_7.txt",
		"../in/beasley_8.txt",
		"../in/beasley_9.txt",
		"../in/beasley_10.txt",
		"../in/beasley_11.txt",
		"../in/beasley_12.txt",
		"../in/christofides.txt",
		"../in/fekete_1.txt",
		"../in/fekete_2.txt",
		"../in/fekete_3.txt",
		"../in/fekete_4.txt",
		"../in/fekete_5.txt",
		"../in/hadjiconstantinou_1.txt",
		"../in/hadjiconstantinou_2.txt",
		"../in/wang.txt",
		"../amaral/NGC_a1.TXT",
		"../amaral/NGC_a2.TXT",
		"../amaral/NGC_a3.TXT",
		"../amaral/NGC_a4.TXT",
		"../amaral/NGC_a5.TXT",
		"../amaral/NGC_b1.TXT",
		"../amaral/NGC_b2.TXT",
		"../amaral/NGC_b3.TXT",
		"../amaral/NGC_b4.TXT",
		"../amaral/NGC_b5.TXT",
		"../amaral/NGC_c1.TXT",
		"../amaral/NGC_c2.TXT",
		"../amaral/NGC_c3.TXT",
		"../amaral/NGC_c4.TXT",
		"../amaral/NGC_c5.TXT",
		"../amaral/NGC_d1.TXT",
		"../amaral/NGC_d2.TXT",
		"../amaral/NGC_d3.TXT",
		"../amaral/NGC_d4.TXT",
		"../amaral/NGC_d5.TXT",
		"../amaral/NGC_e1.TXT",
		"../amaral/NGC_e2.TXT",
		"../amaral/NGC_e3.TXT",
		"../amaral/NGC_e4.TXT",
		"../amaral/NGC_e5.TXT"
	};
	/* *****************
	 * Tests execution
	 * *****************
	 */
	
	if ((out_file = fopen("run_all_tests_output.txt", "w")) == NULL) 
		exit(1);
	
	num_instancias = sizeof(instancias)/sizeof(instancias[0]);

	for (count_instancia = 0; count_instancia < num_instancias; ++count_instancia)
	{
		fprintf(out_file, "-----------------------------------------------------------------------\n");
		fprintf(out_file, "Tests Execution - Instancia: %s\n", instancias[count_instancia]); fflush(out_file);
		printf("Processando Instancia: %s\n", instancias[count_instancia]); fflush(stdout);
			
		for (exec = 0; exec < TEST_EXEC_TIMES; ++exec)
		{
			// Carregando instancia
			if ((file_descriptor = fopen(instancias[count_instancia], "r")) == NULL) {
				printf("Erro: %s (File: %s)\n", strerror(errno), instancias[count_instancia]);
				exit(1);
			}
			
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
			// Instancia Carregada	
			
			// Executando meta heurística
			time_initial = get_time();
			solucoes[exec] = run_grasp_metaheuristic(pieces_cut, &stock, num_pieces_cut);
			time_final   = get_time();
			tempo_exec[exec]  = (time_final - time_initial)/100.0;
			
			// Liberando memoria
			for (count_piece = 0; count_piece < num_pieces_cut; 
				destroy_rectangle(&pieces_cut[count_piece++]));
		}
		
		tempo_computado   = normalize_double_results(tempo_exec);
		solucao_computada = normalize_int_results(solucoes);
		fprintf(out_file, "Time Execution: %.5f\n", tempo_computado);
		fprintf(out_file, "Solucao: %d\n", solucao_computada);
		fflush(out_file);
		
		printf(">>Time Execution: %.5f\n", tempo_computado); 
		printf(">>Solucao: %d\n", solucao_computada);
		fflush(stdout);
		
				
	}
	
	fclose(out_file);
}

