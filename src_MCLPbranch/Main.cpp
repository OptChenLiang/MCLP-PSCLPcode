#include "global_functions.h"
#include "global_variables.h"
#include "MCLP.h"

int main(int argc, char** argv) {
   
	instance inst;

	inst.input_file_f = (char *) calloc(1000, sizeof(char));
	inst.input_file_c = (char *) calloc(1000, sizeof(char));
	////////////////////////////////////////////////////////////////////////////////////////
	if (argc >= 8)
	{
      strcpy(inst.input_file_f, argv[1]);
      strcpy(inst.input_file_c, argv[2]);
      inst.n_locations=atoi(argv[3]);
      inst.n_clients=atoll(argv[4]);
      inst.timelimit=atof(argv[5]);
      inst.RADIUS=atof(argv[6]);
      inst.BUDGET=atof(argv[7]);
      inst.seed = -1;
      if (argc >= 9)
         inst.seed=atof(argv[8]);
	}
	else
	{
		cout << "ERROR NUMBER STANDARD PARAMETERS" << endl;
		cout << "Param1:\t facility filename\n";
      cout << "Param2:\t client filename(can be NULL)\n";
      cout << "Param3:\t n_facilities\n";
      cout << "Param4:\t n_clients\n";
		cout << "Param5:\t time limit\n";
		cout << "Param6:\t RADIUS\n";
		cout << "Param7:\t BUDGET(<=1 percentage, >1 real number)\n";
		cout << "Param8:\t seed(optional)\n";
		exit(-1);
	}
	////////////////////////////////////////////////////////////////////////////////////////

	cout << "***RADIUS " << inst.RADIUS << endl;
	cout << "***BUDGET " << inst.BUDGET << endl;

	/////////////////////////////////////////////////////////////////////////////////////////
   // Read file and presolve
	clock_t time_start=clock();
	read_file(&inst);
	clock_t time_end=clock();
	double inst_generation_time=(double)(time_end-time_start)/(double)CLOCKS_PER_SEC;
	cout << "READING TIME: " << inst_generation_time << endl;

	inst.number_of_CPU=1;
	cout << "***NUMBER OF CPUs " << inst.number_of_CPU << endl;
	
   inst.cohordinates_loaded = false;
   build_model(&inst);
   solve_model(&inst);
   clean_model(&inst);

   free(inst.input_file_f);
   free(inst.input_file_c);

   free_data(&inst);

   printf("DONE!\n");

   return 1;
}



