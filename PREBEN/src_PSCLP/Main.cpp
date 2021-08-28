/*
 *		Main.cpp
 *		Created on: 01/12/2017
 *		Author: Fabio Furini
 */




#include "global_functions.h"
#include "global_variables.h"
#include "struct.h"
#include "read.h"
#include "PSCLP.h"
#include "PSCLP_BEN.h"

int main(int argc, char** argv) {

	instance inst;
	mystr inst1;
//Presolving settings
   inst1.isPSCLP = true;
	inst.algorithm = 2;
   
   //Input facilities file and clients file
   inst1.input_file_f = (char *) calloc(1000, sizeof(char)); 
   inst1.input_file_c = (char *) calloc(1000, sizeof(char)); 

	////////////////////////////////////////////////////////////////////////////////////////
   int exec = 0;
	if (argc >= 9)
	{
      /*Param1*/exec = atoi(argv[1]);
		/*Param2*/strcpy(inst1.input_file_f, argv[2]);
		/*Param3*/strcpy(inst1.input_file_c, argv[3]);
      /*Param4*/inst1.n_locations=atoi(argv[4]);             
      /*Param5*/inst1.n_clients=atoll(argv[5]);
		/*Param6*/inst.timelimit=atof(argv[6]);
		/*Param7*/inst1.RADIUS=atof(argv[7]);
      cout << "***RADIUS " << inst1.RADIUS << endl;
      if( inst1.isPSCLP )
      {
         inst1.COVERING_DEMAND=atof(argv[8]);
         cout << "***BUDGET " << inst1.COVERING_DEMAND << endl;
      }
      else
      {
         inst1.BUDGET=atof(argv[8]);
         cout << "***BUDGET " << inst1.BUDGET << endl;
      }
      inst1.seed = -1;
      if (argc >= 10)
         inst1.seed=atof(argv[9]);

	}
	else
	{
		//Error input
		cout << "ERROR NUMBER STANDARD PARAMETERS" << endl;
		cout << "Param1:\t instance name\n";
		cout << "Param2:\t algorithm\n";
		cout << "Param3:\t time limit\n";
		cout << "Param4:\t RADIUS\n";
		cout << "Param5:\t BUDGET\n";

		exit(-1);
	}
   
   inst1.isIA = true;
   inst1.isSA = true;
   inst1.isD = true;
   switch (exec)
   {
      case 0: //NO_PRE
         inst1.isIA = false;
         inst1.isSA = false;
         inst1.isD = false;
         break;
      case 1: //ALL
         inst1.isIA = true;
         inst1.isSA = true;
         inst1.isD = true;
         break;
   }
	////////////////////////////////////////////////////////////////////////////////////////


   inst1.coordinates_loaded = false;
	inst.cohordinates_loaded=false;

	cout << "***RADIUS\t" << inst1.RADIUS << endl;
	cout << "***BUDGET\t" << inst1.COVERING_DEMAND << endl;


	/////////////////////////////////////////////////////////////////////////////////////////
	clock_t time_start=clock();
	//Read facility and client files if files exist and implement presolving methods
	read_file(&inst1);
	//Build data structures based on read data
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
		build_model_CFL(&inst);
		solve_model_CFL(&inst);
		clean_model_CFL(&inst);
		///////////////////////////

	}



	if(inst.algorithm==2)
	{
		cout << "\n\n----------->>>>>>BRANCH-AND-BENDERS-CUT\n";

		//this bender an advanced  implementation of the bender (it generates cuts using combinatorial algorithms) *** in addition there is the possibility of activating an  initial cut loop

		///////////////////////////
		build_model_CFL_BEN_2(&inst);
		solve_model_CFL_BEN_2(&inst, &inst1);
		clean_model_CFL_BEN_2(&inst);
		///////////////////////////

	}

	//Free memory
	free(inst1.input_file_f);
	free(inst1.input_file_c);
   free_data(&inst1);

	free_data(&inst);

	printf("\nDONE!\n");

	return 1;
}

