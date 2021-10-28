#include "global_functions.h"
#include "global_variables.h"
#include "model.h"

int main(int argc, char** argv) {
   
	instance inst;
#ifdef PSCLP
   inst.isPSCLP = true;
#else
   inst.isPSCLP = false;
#endif
   
	inst.input_file_f = (char *) calloc(1000, sizeof(char));
	inst.input_file_c = (char *) calloc(1000, sizeof(char));
	////////////////////////////////////////////////////////////////////////////////////////
   int exec = 0;
	if (argc >= 8)
	{
      exec = atoi(argv[1]);
      //Input locations file, clients file and other solving parameters
      strcpy(inst.input_file_f, argv[2]);
      strcpy(inst.input_file_c, argv[3]);
      inst.timelimit=atof(argv[4]);
      inst.RADIUS=atof(argv[5]);
      inst.is_percentage=atoi(argv[6]);
      cout << "Radius: " << inst.RADIUS << endl;
      if(inst.isPSCLP)
      {
         inst.COVERING_DEMAND=atof(argv[7]);
         if(inst.is_percentage == 1)
            cout << "Demand(%): " << inst.COVERING_DEMAND << endl;
         else
            cout << "Demand: " << inst.COVERING_DEMAND << endl;
      }
      else
      {
         inst.BUDGET=atof(argv[7]);
         if(inst.is_percentage == 1)
            cout << "Budget(%): " << inst.BUDGET << endl;
         else
            cout << "Budget: " << inst.BUDGET << endl;
      }
      inst.seed = -1;
      if (argc >= 9)
         inst.seed=atof(argv[8]); /* Random seed (optional) */
	}
	else
	{
		//Error input
		cout << "ERROR NUMBER STANDARD PARAMETERS" << endl;
      cout << "Param1:\t settings\n";
		cout << "Param2:\t file of facilities\n";
      cout << "Param3:\t customers file (can be a number and then generate customers randomly when running the code)\n";
		cout << "Param4:\t time limitation in seconds\n";
		cout << "Param5:\t covering radius\n";
		cout << "Param6:\t is percentage of Param7 (total budget for the MCLP or total covering demand for the PSCLP)?\n";
		cout << "Param7:\t budget for the MCLP or covering demand for the PSCLP\n";
		cout << "Param8:\t CPXPARAM_RandomSeed for CPLEX\n";
		exit(-1);
	}
   inst.isBranch = true;
   inst.isCut = true;
   inst.isIA = true;
   inst.isSA = true;
   inst.isDom = true;
   inst.isNC = true;
   switch (exec)
   {
      case 0: //NO_PRE
         inst.isCut = false;
         inst.isIA = false;
         inst.isSA = false;
         inst.isDom = false;
         inst.isNC = false;
         break;
      case 1: //ALL
         break;
      case 2: //NO_SIN_AGG
         inst.isSA = false;
         break;
      case 3: //NO_ISO_AGG
         inst.isIA = false;
         break;
      case 4: //NO_NON_CAN
         inst.isNC = false;
         break;
      case 5: //NO_DOM
         inst.isDom = false;
         break;
      case 6: //NO_NON_FIX
         inst.isCut = false;
         break; 
   }
	/////////////////////////////////////////////////////////////////////////////////////////
   // Read file and presolve
	clock_t time_start=clock();
	read_file(&inst);
	clock_t time_end=clock();
	double inst_generation_time=(double)(time_end-time_start)/(double)CLOCKS_PER_SEC;
	cout << "Reading time: " << inst_generation_time << endl;

	inst.number_of_CPU=1;
	cout << "Number of CPUs: " << inst.number_of_CPU << endl;
	
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


