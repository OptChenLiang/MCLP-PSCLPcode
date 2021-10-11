#ifndef VARIABLE_local_HEADER
#define VARIABLE_local_HEADER


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <sstream>
#include <vector>
#include <algorithm>

//#include </home/fabio/ILOG/CPLEX_Studio_AcademicResearch127/cplex/include/ilcplex/cplex.h>
#include "../../include/cplex.h"

using namespace std;

////////////////////////////////////GENERAL///////////////////////////////////
#define VERBOSE 1000
#define EPSILON 0.0001
#define MAX_CONNECTIONS 800000000
//////////////////////////////////////////////////////////////////////////////


typedef struct {

	int algorithm; /* Solving settings */
	char *input_file; 
	int n_locations; /* Number of facilities */
	int n_clients; /* Number of clients */


	int *demand; /*Demand vector of clients */
	int *fixed_cost; /* Cost vector of facilities */
	double *singleton; /* Singleton nodes */
 
	double RADIUS; /* Coverage radius */
	double COVERING_DEMAND; /* The required coverage demand */
	double BUDGET; /* The required coverage budget */


	double timelimit; /* Time limitation */

	int seed; /* Random seed */

	int number_of_CPU; /* Number of CPUs */


	///////////////////////////////////////////////////////////////////////////////
	int *NFS;     // NFS[i]: begin of FS(i) in AFS
	int *AFS;     // arcs index ordered for FS
	int *DP;  // number of neighbours delta+

	int *NBS;     // NBS[i]: begin of BS(i) in ABS
	int *ABS;     // arcs index ordered for BS
	int *DM;  // number of neighbours delta-
	///////////////////////////////////////////////////////////////////////////////

	bool cohordinates_loaded;



	double *x_location; /*X coordinates of facilities */
   	double *y_location; /*Y coordinates of facilities */

   	double *x_client; /*X coordinates of clients */
   	double *y_client; /*Y coordinates of clients */
	
	bool *cliets_OK;

	//CPLEX related variables and environments
	CPXENVptr env_CFL,env_CFL_BEN,env_CFL_BEN_FACETS,env_CFL_BEN_2,env_DFL_BEN_2,env_CFL_BEN_AUX,env_CFL_BEN_FACETS_AUX,env_CFL_MOD,env_CFL_penalty,env_DFL,env_DFL_penalty,env_DFL_MOD,env_CFL2,env_CFL_CVAR,env_DFL_CVAR,env_CFL_CONS,env_DFL_CONS,env_CFL_GAMMA,env_DFL_GAMMA,env_CFL_EXP;
	CPXLPptr  lp_CFL,lp_CFL_BEN,lp_CFL_BEN_FACETS,lp_CFL_BEN_2,lp_DFL_BEN_2,lp_CFL_BEN_AUX,lp_CFL_BEN_FACETS_AUX,lp_CFL_MOD,lp_CFL_penalty,lp_DFL,lp_DFL_penalty,lp_DFL_MOD,lp_CFL2,lp_CFL_CVAR,lp_DFL_CVAR,lp_CFL_CONS,lp_DFL_CONS,lp_CFL_GAMMA,lp_DFL_GAMMA,lp_CFL_EXP;
	int status,ccnt,rcnt,nzcnt;
	int* rmatbeg, *rmatind,*cmatbeg, *cmatind;
	double* rmatval,*cmatval,*x,*pi,*obj, *lb, *ub,*rhs;
	char *c_type,* sense;
	char **colname;
	double objval,bestobjval;
	int lpstat,nodecount;
	///////////////////////////////////////////////////////////////////////////////


	int client_not_covered;
	int client_single_covered;

	int counter_c;
	int counter_l;

} instance;


#endif
