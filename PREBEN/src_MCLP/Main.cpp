/*
 *		Main.cpp
 *		Created on: 01/12/2017
 *		Author: Fabio Furini
 */




#include "global_functions.h"
#include "global_variables.h"
#include "struct.h"
#include "read.h"
#include "MCLP.h"
#include "MCLP_BEN.h"

int main(int argc, char** argv) {

	instance inst;
	mystr inst1;
//Presolving settings
#if NOIA 
   inst1.isIA = false;
#else
   inst1.isIA = true;
#endif
#if NOD
   inst1.isD = false;
#else
   inst1.isD = true;
#endif
   inst1.isPSCLP = false;
	inst.algorithm = 2;
#if NOSA
   inst1.isSA = false;
#else
   inst1.isSA = true;
#endif
   //Input facilities file and clients file
   inst1.input_file_f = (char *) calloc(1000, sizeof(char)); 
   inst1.input_file_c = (char *) calloc(1000, sizeof(char)); 

	////////////////////////////////////////////////////////////////////////////////////////
	if (argc >= 8)
	{
		/*Param1*/strcpy(inst1.input_file_f, argv[1]);
		/*Param2*/strcpy(inst1.input_file_c, argv[2]);
      /*Param3*/inst1.n_locations=atoi(argv[3]);             
      /*Param4*/inst1.n_clients=atoll(argv[4]);
		/*Param5*/inst.timelimit=atof(argv[5]);
		/*Param6*/inst1.RADIUS=atof(argv[6]);
      cout << "***RADIUS " << inst1.RADIUS << endl;
      if( inst1.isPSCLP )
      {
         inst1.COVERING_DEMAND=atof(argv[7]);
         cout << "***BUDGET " << inst.COVERING_DEMAND << endl;
      }
      else
      {
         inst1.BUDGET=atof(argv[7]);
         cout << "***BUDGET " << inst1.BUDGET << endl;
      }
      inst1.seed = -1;
      if (argc >= 9)
         inst1.seed=atof(argv[8]);

	}
	else
	{
		cout << "ERROR NUMBER STANDARD PARAMETERS" << endl;
		cout << "Param1:\t instance name\n";
		cout << "Param2:\t algorithm\n";
		cout << "Param3:\t time limit\n";
		cout << "Param4:\t RADIUS\n";
		cout << "Param5:\t BUDGET\n";

		exit(-1);
	}
	////////////////////////////////////////////////////////////////////////////////////////


   inst1.coordinates_loaded = false;
	inst.cohordinates_loaded=false;

	cout << "***RADIUS\t" << inst1.RADIUS << endl;
	cout << "***BUDGET\t" << inst1.BUDGET << endl;


	/////////////////////////////////////////////////////////////////////////////////////////
	clock_t time_start=clock();
	//Read facility and client files if files exist and implement presolving methods
	read_file(&inst1);
	//Build data structures based on readed data
	READ_NEW_FILE1(&inst, &inst1);
   //Presolving time statistics
   cout<<"presolve_IA: "<<inst1.presolve_IA_time<<endl;
   cout<<"presolve_D: "<<inst1.presolve_D_time<<endl;
   inst1.presolve_time = inst1.presolve_IA_time
      + inst1.presolve_D_time;
   cout<<"Presolve Time: "<<inst1.presolve_time<<endl;

	clock_t time_end=clock();
	double inst_generation_time=(double)(time_end-time_start)/(double)CLOCKS_PER_SEC;

	cout << "\n\nREADING TIME:\t" << inst_generation_time << endl;
	/////////////////////////////////////////////////////////////////////////////////////////


	inst.number_of_CPU=1;
	cout << "\n***NUMBER OF CPUs\t" << inst.number_of_CPU << endl;


	if(inst.algorithm==1)
	{
		cout << "\n\n----------->>>>>>COMPACT MODEL\n";

		///////////////////////////
		build_model_DFL(&inst);
		solve_model_DFL(&inst);
		clean_model_DFL(&inst);
		///////////////////////////

	}



	if(inst.algorithm==2)
	{
		cout << "\n\n----------->>>>>>BRANCH-AND-BENDERS-CUT\n";

		//this bender an advanced  implementation of the bender (it generates cuts using combinatorial algorithms) *** in addition there is the possibility of activating an  initial cut loop

		///////////////////////////
		build_model_DFL_BEN_2(&inst);
		solve_model_DFL_BEN_2(&inst, &inst1);
		clean_model_DFL_BEN_2(&inst);
		///////////////////////////

	}


	free(inst1.input_file_f);
	free(inst1.input_file_c);
   free_data(&inst1);

	free_data(&inst);

	printf("\nDONE!\n");

	return 1;
}



