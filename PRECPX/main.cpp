#include "global_functions.h"
#include "global_variables.h"
#include "model.h"

int main(int argc, char** argv) {
   
	instance inst;
// Presolving settings  
#if BRANCH
   inst.isBranch = true;
#else
   inst.isBranch = false;
#endif
#if CUT
   inst.isCut = true;
#else
   inst.isCut = false;
#endif
#if BIN
   inst.isBin = true;
#else
   inst.isBin = false;
#endif
#if IA
   inst.isIA = true;
#else
   inst.isIA = false;
#endif
#if SA
   inst.isSA = true;
#else
   inst.isSA = false;
#endif
#if Dom
   inst.isDom = true;
#else
   inst.isDom = false;
#endif
#if NC
   inst.isNC = true;
#else
   inst.isNC = false;
#endif
#if PSCLP
   inst.isPSCLP = true;
#else
   inst.isPSCLP = false;
#endif

   
	inst.input_file_f = (char *) calloc(1000, sizeof(char));
	inst.input_file_c = (char *) calloc(1000, sizeof(char));
	////////////////////////////////////////////////////////////////////////////////////////
	if (argc >= 8)
	{
      //Input locations file, clients file and other solving parameters
      strcpy(inst.input_file_f, argv[1]);
      strcpy(inst.input_file_c, argv[2]);
      inst.n_locations=atoi(argv[3]);
      inst.n_clients=atoll(argv[4]);
      inst.timelimit=atof(argv[5]);
      inst.RADIUS=atof(argv[6]);
      cout << "***RADIUS " << inst.RADIUS << endl;
      if(inst.isPSCLP)
      {
         inst.COVERING_DEMAND=atof(argv[7]);
         cout << "***BUDGET " << inst.COVERING_DEMAND << endl;
      }
      else
      {
         inst.BUDGET=atof(argv[7]);
         cout << "***BUDGET " << inst.BUDGET << endl;
      }
      inst.seed = -1;
      if (argc >= 9)
         inst.seed=atof(argv[8]); /* Random seed (optional) */
	}
	else
	{
		//Error input
		cout << "ERROR NUMBER STANDARD PARAMETERS" << endl;
		cout << "Param1:\t facility filename\n";
      cout << "Param2:\t client filename(can be NULL)\n";
      cout << "Param3:\t n_facilities\n";
      cout << "Param4:\t n_clients\n";
		cout << "Param5:\t time limit\n";
		cout << "Param6:\t RADIUS\n";
      if(inst.isPSCLP)
         cout << "Param7:\t DEMAND(<=1 percentage, >1 real number)\n";
      else
         cout << "Param7:\t BUDGET(<=1 percentage, >1 real number)\n";
		cout << "Param8:\t seed(optional)\n";
		exit(-1);
	}
	/////////////////////////////////////////////////////////////////////////////////////////
   // Read file and presolve
	clock_t time_start=clock();
	read_file(&inst);
	clock_t time_end=clock();
	double inst_generation_time=(double)(time_end-time_start)/(double)CLOCKS_PER_SEC;
	cout << "READING TIME: " << inst_generation_time << endl;

	inst.number_of_CPU=1;
	cout << "***NUMBER OF CPUs " << inst.number_of_CPU << endl;
	
   inst.coordinates_loaded = false;
   //Build MIP model
   build_model(&inst);
   //Solve MIP model
   solve_model(&inst);
   //Clean MIP model
   clean_model(&inst);

   //Free memory of input data files
   free(inst.input_file_f);
   free(inst.input_file_c);

   free_data(&inst);

   printf("DONE!\n");

   return 1;
}



