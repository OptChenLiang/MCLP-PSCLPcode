

#include "MCLP.h"
#include "presolve.h"
#include <iomanip>
//#define solve_LP

//#define write_prob

//#define print_solution
//#define print_solution_lp
extern double presolve_time;
extern double presolve_dnc_time;


//Empty callback for a fair comparison with Benders
int CPXPUBLIC mycutcallback_FAKE(CPXCENVptr env,void *cbdata,int wherefrom,void *cbhandle,int *useraction_p)
{

	(*useraction_p)=CPX_CALLBACK_DEFAULT;

	return 0;
}

/*****************************************************************/
void build_model(instance *inst)
/*****************************************************************/
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// * setting the CPLEX environment

	//opening the environment
	inst->env=CPXopenCPLEX(&(inst->status));
	if(inst->status!=0)
	{
		printf("cannot open CPLEX environment\n");
		exit(-1);
	}

	//opening the pointer to the problem
	inst->lp=CPXcreateprob(inst->env,&(inst->status),"DFL");
	if(inst->status!=0)
	{
		printf("cannot create problem\n");
		exit(-1);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// * creating the variables *
   inst->ccnt=inst->n_locations+inst->n_data;
   inst->obj=(double*) calloc(inst->ccnt,sizeof(double));
	inst->lb=(double*) calloc(inst->ccnt,sizeof(double));
	inst->ub=(double*) calloc(inst->ccnt,sizeof(double));
	inst->c_type=(char*) calloc(inst->ccnt,sizeof(char));

	inst->colname=(char**) calloc(inst->ccnt,sizeof(char*));
	for(int i=0;i<inst->ccnt;i++)
   {
      inst->colname[i]=(char*) calloc(100,sizeof(char));
   }
	int counter = 0;
	for ( int j = 0; j < inst->n_locations; j++){

      inst->covers[j]->pos = counter;
		inst->obj[counter]=inst->covers[j]->cost;
		inst->lb[counter]=0.0;
      inst->ub[counter]=1.0;
      if(inst->covers[j]->isdeleted)
      {
         inst->ub[counter]=0.0;
         inst->covers[j]->pos = -1;
         continue;
      }
		inst->c_type[counter]='B';
		sprintf(inst->colname[counter], "y%d",j);
		counter++;

	}
   inst->validlocations = counter;
	cout << "\n*** CONTINUOUS Z VARIABLES\n";
   int size = inst->n_data;
   for ( int i = 0; i < size; i++ )
   {
      inst->obj[counter] = inst->data[i]->cost;
      inst->data[i]->pos = counter; 
      assert(inst->data[i]->isdeleted == false);
      inst->lb[counter]=0.0;
		inst->ub[counter]=1.0;
#if BIN
		inst->c_type[counter]='B';
#else
		inst->c_type[counter]='C';
#endif
		sprintf(inst->colname[counter], "z%d",i);
		counter++;
	}
	inst->status=CPXnewcols(inst->env,inst->lp,counter,inst->obj,inst->lb,inst->ub,inst->c_type,inst->colname);
   printf("locations:%d cols: %d\n", inst->validlocations, counter);
	if(inst->status!=0)
	{
		printf("error in CPXnewcols\n");
		exit(-1);
	}

	free(inst->lb);
	free(inst->ub);
	free(inst->c_type);

	for(int i=0;i<inst->ccnt;i++)
   {
      free(inst->colname[i]);
   }
	free(inst->colname);


	// * setting the objective function in the minimization form
	CPXchgobjsen(inst->env,inst->lp,CPX_MAX);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

   int nrows = 0;
   sort(inst->data.begin(), inst->data.end(), CompforDNC);
	for ( int i = 0; i < size; i++ )
	{
      assert(inst->data[i]->isdeleted == false);
		// * creating the covering  constraint *
		inst->rcnt=1;
		inst->nzcnt=inst->data[i]->locations.size()+1;
		inst->rhs=(double*) calloc(inst->rcnt,sizeof(double));
		inst->sense=(char*) calloc(inst->rcnt,sizeof(double));

		inst->rhs[0]=0.0;
		inst->sense[0]='G';
		inst->rmatbeg=(int*) calloc(inst->rcnt+1,sizeof(int));
		inst->rmatind=(int*) calloc(inst->nzcnt,sizeof(int));
		inst->rmatval=(double*) calloc(inst->nzcnt,sizeof(double));

		int counter=0;
#if DNC
      clock_t time_presolvestart = clock();
      vector<int> locations = inst->data[i]->locations;
      if(inst->data[i]->locations.size() > 2 )
      {
         for(int j = i+1; j<size; j++)
         {
            if(issubset(locations, inst->data[j]->locations))
            {
               subset(locations, inst->data[j]->locations);
               inst->rmatval[counter] = 1.0;
               inst->rmatind[counter] = inst->data[j]->pos;
               counter++;
            }
            if(locations.size() < 2)
               break;
         }
      }
      int lsize = locations.size();
      for (int k = 0; k < lsize; k++)
      {
         assert(inst->covers[locations[k]]->isdeleted == false);
         inst->rmatval[counter] = 1.0;
         inst->rmatind[counter++] = inst->covers[locations[k]]->pos;
      }
      clock_t time_presolveend=clock();
      inst->presolve_dnc_time+=(double)(time_presolveend-time_presolvestart)/(double)CLOCKS_PER_SEC;
#else
      int lsize = inst->data[i]->locations.size();
      for (int k = 0; k < lsize; k++)
      {
         assert(inst->covers[inst->data[i]->locations[k]]->isdeleted == false);
         inst->rmatval[counter] = 1.0;
         inst->rmatind[counter++] = inst->covers[inst->data[i]->locations[k]]->pos;
      }
#endif
      inst->rhs[0]=0.0;
      inst->rmatval[counter] = -1.0;
      inst->rmatind[counter++]= inst->data[i]->pos;
      inst->rmatbeg[0]=0;

		inst->status=CPXaddrows(inst->env,inst->lp,0,inst->rcnt,counter,inst->rhs,inst->sense,inst->rmatbeg,inst->rmatind,inst->rmatval,NULL,NULL);
		if(inst->status!=0)
		{
			printf("error in CPXaddrows\n");
			exit(-1);
		}

		free(inst->rmatbeg);
		free(inst->rmatval);
		free(inst->rmatind);
		free(inst->rhs);
		free(inst->sense);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
   // * creating the budget constraint *
   inst->rcnt=1;
   inst->nzcnt=inst->n_locations;
	inst->rhs=(double*) calloc(inst->rcnt,sizeof(double));
	inst->sense=(char*) calloc(inst->rcnt,sizeof(double));

	inst->rhs[0]=inst->BUDGET;
	inst->sense[0]='L';


	inst->rmatbeg=(int*) calloc(inst->rcnt,sizeof(int));
	inst->rmatind=(int*) calloc(inst->nzcnt,sizeof(int));
	inst->rmatval=(double*) calloc(inst->nzcnt,sizeof(double));

   inst->nzcnt = 0;
   counter = 0;
	for ( int i = 0; i < inst->n_locations; i++ )
   {
      if(inst->covers[i]->isdeleted)
         continue;
      inst->rmatval[inst->nzcnt]=inst->covers[i]->demand;
      inst->rmatind[inst->nzcnt]=inst->covers[i]->pos;
      inst->nzcnt++;
	}

	inst->rmatbeg[0]=0;
	inst->status=CPXaddrows(inst->env,inst->lp,0,inst->rcnt,inst->nzcnt,inst->rhs,inst->sense,inst->rmatbeg,inst->rmatind,inst->rmatval,NULL,NULL);
	if(inst->status!=0)
	{
		printf("error in CPXaddrows\n");
		exit(-1);
	}

	free(inst->rmatbeg);
	free(inst->rmatval);
	free(inst->rmatind);
	free(inst->rhs);
	free(inst->sense);


#ifdef write_prob
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// * writing the created ILP model on a file *
	inst->status=CPXwriteprob(inst->env,inst->lp,"DFL.lp",NULL);
	if(inst->status!=0) {
		printf("error in CPXwriteprob\n");
		exit(-1);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	exit(-1);
#endif
   cout<<"DNCNNZ= " << CPXgetnumnz( inst->env, inst->lp ) <<endl;;

   cout<<"presolve_dpa: "<<inst->presolve_dpa_time<<endl;
   cout<<"presolve_dnc: "<<inst->presolve_dnc_time<<endl;
   cout<<"presolve_dc: "<<inst->presolve_dc_time<<endl;
   inst->presolve_time = inst->presolve_dpa_time
      + inst->presolve_dnc_time
      + inst->presolve_dc_time;
   cout<<"Presolve Time: "<<inst->presolve_time<<endl;
   inst->n_data = inst->data.size();
}

/*****************************************************************/
void solve_model(instance *inst)
/*****************************************************************/
{
	CPXsetintparam (inst->env, CPX_PARAM_SCRIND, CPX_ON);
   
   // * Reset random seed if necessary *
   if(inst->seed != -1)
   {
      inst->status = CPXsetintparam (inst->env, CPX_PARAM_RANDOMSEED, inst->seed);
      if (inst->status)
      {
         printf ("error for CPX_PARAM_EPAGAP\n");
      }
   }

	// * Set relative tolerance *
	inst->status = CPXsetdblparam (inst->env, CPX_PARAM_EPAGAP, 0.0);
	if (inst->status)
	{
	   printf ("error for CPX_PARAM_EPAGAP\n");
	}
	
	// * Set a tolerance *
	inst->status = CPXsetdblparam (inst->env, CPX_PARAM_EPGAP, 0.0);
	if (inst->status)
   {
	   printf ("error for CPX_PARAM_EPGAP\n");
	}
	//
	//	// * Set mip tolerances integrality *
	//	inst->status = CPXsetdblparam (inst->env, CPX_PARAM_EPINT, 0.0);
	//	if (inst->status)
	//	{
	//		printf ("error for CPX_PARAM_EPINTP\n");
	//	}
	//
	//	// * Set Feasibility tolerance *
	//	inst->status = CPXsetdblparam (inst->env, CPX_PARAM_EPRHS, 1e-9);
	//	if (inst->status)
	//	{
	//		printf ("error for CPX_PARAM_EPRHS\n");
	//	}

	// * Set number of CPU*
	inst->status = CPXsetintparam (inst->env, CPX_PARAM_THREADS, inst->number_of_CPU);
	if (inst->status)
	{
		printf ("error for CPX_PARAM_EPRHS\n");
	}

	// * Set time limit *
   if(inst->presolve_time > inst->timelimit)
      inst->status = CPXsetdblparam (inst->env, CPX_PARAM_TILIM, 1);
   else
      inst->status = CPXsetdblparam (inst->env, CPX_PARAM_TILIM,inst->timelimit - inst->presolve_time);
	if (inst->status)
	{
		printf ("error for CPX_PARAM_EPRHS\n");
	}

   /*
	inst->status = CPXsetintparam (inst->env, CPXPARAM_MIP_Cuts_ZeroHalfCut, -1);
	inst->status = CPXsetintparam (inst->env, CPXPARAM_MIP_Cuts_Gomory, -1);
	inst->status = CPXsetintparam (inst->env, CPXPARAM_MIP_Cuts_LiftProj, -1);
	inst->status = CPXsetintparam (inst->env, CPXPARAM_MIP_Cuts_Covers, -1);
   */
#if 0
   // * Set node limit *
	inst->status = CPXsetintparam (inst->env, CPX_PARAM_NODELIM, 0);
	if (inst->status)
	{
		printf ("error for CPX_PARAM_EPRHS\n");
	}
#endif

	//	if(inst->option==1000)
	//	{
	//		solve_LP(inst);
	//	}
	//
	//	if(inst->option==2)
	//	{
	//
	//		//this is the only only one necessary to avoid the removal of all continuous variables
	//		CPXsetintparam(inst->env, CPX_PARAM_PREIND, CPX_OFF);
	//
	//
	////				CPXsetintparam(inst->env, CPX_PARAM_AGGIND, CPX_OFF);
	////				CPXsetintparam(inst->env, CPX_PARAM_BNDSTRENIND, CPX_OFF);
	////				CPXsetintparam(inst->env, CPX_PARAM_COEREDIND, CPX_OFF);
	////				CPXsetintparam(inst->env, CPX_PARAM_RELAXPREIND, CPX_OFF);
	////				CPXsetintparam(inst->env, CPX_PARAM_REDUCE, CPX_OFF);
	////				CPXsetintparam(inst->env, CPX_PARAM_PREPASS, CPX_OFF);
	////				CPXsetintparam(inst->env, CPX_PARAM_REPEATPRESOLVE, CPX_OFF);
	//
	//		cout << "***********\n\n AUTOMATIC BENDER'S DECOMPOSITION\n\n";
	//
	//		inst->status = CPXsetintparam (inst->env, CPXPARAM_Benders_Strategy, 3);
	//		if (inst->status)
	//		{
	//			printf ("error for CPX_PARAM_EPRHS\n");
	//		}
	//	}
   //inst->status = CPXsetintparam (inst->env, CPXPARAM_MIP_Strategy_Search, CPX_MIPSEARCH_TRADITIONAL);
   //inst->status = CPXsetintparam (inst->env, Preprocessing_Linear, CPX_OFF);
   inst->status = CPXsetintparam (inst->env, CPXPARAM_Preprocessing_Linear, 0);
   inst->status = CPXsetintparam (inst->env, CPXPARAM_MIP_Strategy_CallbackReducedLP, CPX_OFF);
   //Preprocessing_Linear and MIP_Strategy_CallbackReducedLP
   //inst->status = CPXsetintparam (inst->env, CPXPARAM_Preprocessing_Presolve, CPX_OFF);
	
   //OPEN FAKE CALLBACK FOR A FAIR COMPARISON WITH BENDERS (SAME BRANCHING STRATEGY!!!!)
	//cout << "OPEN FAKE CALLBACK FOR FAIR COMPARISONS\n";
	inst->status = CPXsetlazyconstraintcallbackfunc(inst->env,mycutcallback_FAKE,inst);
	if (inst->status)
	{
		printf ("error for CPXsetlazyconstraintcallbackfunc\n");
	}
#if BRANCH
	if (inst->status)
	{
		printf ("error for CPX_PARAM_EPRHS\n");
	}
   inst->status = CPXsetusercutcallbackfunc (inst->env, mycutcallback, inst);
	if (inst->status)
	{
		printf ("error for CPXsetusercutcallbackfunc\n");
	}
#endif
//#if OPEN
//   inst->status = CPXsetintparam (inst->env, CPXPARAM_Preprocessing_Presolve, CPX_ON);
//#endif

	///////////////////////////////////////////////////////////////////////////////////
	// * solving the MIP model
	clock_t time_start=clock();

	cout << "\nCPXmipopt:\n";
	int cur_numcols=CPXgetnumcols(inst->env,inst->lp);
   assert( cur_numcols == inst->validlocations + inst->data.size());
   inst->x=(double*) calloc(cur_numcols,sizeof(double));

   inst->rcnt = 1;
   inst->lb=(double*) calloc(cur_numcols,sizeof(double));
   inst->ub=(double*) calloc(cur_numcols,sizeof(double));
   inst->rhs=(double*) calloc(inst->rcnt,sizeof(double));
   inst->sense=(char*) calloc(inst->rcnt,sizeof(double));

   inst->rhs[0]=0.0;
   inst->sense[0]='G';
   inst->rmatbeg=(int*) calloc(inst->rcnt,sizeof(int));
   inst->rmatind=(int*) calloc(cur_numcols,sizeof(int));
   inst->rmatval=(double*) calloc(cur_numcols,sizeof(double));

	int cur_numrows=CPXgetnumrows(inst->env,inst->lp);
#if 0
   int* indices=(int*) calloc(inst->n_locations+inst->data.size(),sizeof(int));
   int* priority=(int*) calloc(inst->n_locations+inst->data.size(),sizeof(int));

   for(int ii=0;ii<cur_numcols;ii++)
   {
      indices[ii]=ii;
      priority[ii]=0;
      if(ii>=inst->validlocations)
         priority[ii]=1000;
   }
   inst->status = CPXcopyorder(inst->env, inst->lp, cur_numcols, indices, priority, NULL);
   free(indices);
   free(priority);
	if(inst->status!=0)
	{
		printf("error in CPXcopyorder\n");
		exit(-1);
	}
#endif
	inst->status=CPXmipopt(inst->env,inst->lp);
	if(inst->status!=0)
	{
		printf("error in CPXmipopt\n");
		exit(-1);
	}

	clock_t time_end=clock();
	double solution_time=(double)(time_end-time_start)/(double)CLOCKS_PER_SEC;
	///////////////////////////////////////////////////////////////////////////////////

	bool sol_found=true;

	// * getting the solution

   int size = 0;
   size = inst->n_data;
   inst->nodeindex = 10000;
   inst->status=CPXgetmipx(inst->env,inst->lp,inst->x,0,cur_numcols-1);
   
   if(inst->status!=0)
   {
      sol_found=false;
      printf("error in CPXgetmipx\n");
	}

	inst->status=CPXgetmipobjval(inst->env,inst->lp,&(inst->objval));
	if(inst->status!=0)
	{
		printf("error in CPXgetmipobjval\n");
	}

	printf("\n\nMIP solution value ->\t\%f",inst->objval);

	int open_facilities=-1;
	int satisfied_clients=-1;

	if(sol_found){

		open_facilities=0;
		satisfied_clients=0;

		for ( int j = 0; j < inst->n_locations; j++){

         if(inst->covers[j]->pos >=0)
         {
            if( (int)(inst->x[inst->covers[j]->pos]+0.5) ==1){
               {
                  open_facilities++;
               }
            }
         }
		}

		for ( int i = 0; i < size; i++ ){
#if PRESOLVE
         if(inst->data[i]->locations.size() == 0)
            continue;
         else if(inst->data[i]->locations.size() == 1)
         {
            if(inst->x[inst->covers[inst->data[i]->locations[0]]->pos] > 0.5)
               satisfied_clients += inst->data[i]->clients.size();
         }
         else if( inst->x[inst->data[i]->pos] > 0.5)
         {
            satisfied_clients += inst->data[i]->clients.size();
         }
#else
			if( (int)(inst->x[inst->validlocations + i]+0.5)==1){
				satisfied_clients++;
			}
#endif
      }
   }

	inst->bestobjval=-1;
	inst->status=CPXgetbestobjval(inst->env,inst->lp,&(inst->bestobjval));
	if(inst->status!=0)
	{
		printf("error in CPXgetbestobjval\n");
	}

	inst->lpstat=CPXgetstat(inst->env,inst->lp);
	inst->nodecount = CPXgetnodecnt(inst->env, inst->lp);
   int itcnt = CPXgetmipitcnt( inst->env, inst->lp );

   cout<<"\nitcnt: "<<itcnt<<endl;

   inst->status = CPXgetintparam( inst->env, CPX_PARAM_RANDOMSEED, &inst->seed);
   cout<<"\nseed: "<< inst->seed<<endl;

	cout << "\n\nlpstat\t" << inst->lpstat << endl;


	cout << "\n***open_facilities\t" << open_facilities << " " << inst->BUDGET<<endl;
	cout << "***satisfied_clients\t" << satisfied_clients << endl;

   //Statistics output
	cout << "\n\nSTAT:\tobjval\t" << setw(16) << inst->objval << "\tbestobjval\t" << inst->bestobjval << "\tlpstat\t" << inst->lpstat << "\topen_facilities\t" << open_facilities << "\tsatisfied_clients\t" << satisfied_clients <<"\tnodecount\t"<<inst->nodecount<<"\tpresolve_time\t"<<inst->presolve_dpa_time <<"\tsolve_time\t"<< solution_time<<"\ttotal_time\t"<<inst->presolve_time+solution_time<<"\ttotal_time_minus_presolve_time\t "<<inst->presolve_time+solution_time<< endl << endl;

   cout<<"presolve_node_time "<<inst->presolve_node_time<<endl;
   
   cout<<"num_easy "<<inst->num_easy<<endl;
   cout<<"num_col "<<inst->num_col<<endl;
   cout<<"num_row "<<inst->num_row<<endl;

	free(inst->x);
	free(inst->lb);
	free(inst->ub);
	free(inst->rhs);
	free(inst->sense);
	free(inst->rmatbeg);
	free(inst->rmatind);
	free(inst->rmatval);
	free(inst->obj);
}


/*****************************************************************/
void clean_model(instance *inst)
/*****************************************************************/
{
	inst->status=CPXfreeprob(inst->env,&(inst->lp));
	if(inst->status!=0) {printf("error in CPXfreeprob\n");exit(-1);}

	inst->status=CPXcloseCPLEX(&(inst->env));
	if(inst->status!=0) {printf("error in CPXcloseCPLEX\n");exit(-1);}
}
         
// * Node presolving callback *
static int CPXPUBLIC
mycutcallback (CPXCENVptr env,
      void       *cbdata,
      int        wherefrom,
      void       *cbhandle,
      int        *useraction_p)
{
   clock_t time_presolvestart = clock();
   int status = 0;
   instance* inst = (instance*) cbhandle;
   *useraction_p = CPX_CALLBACK_DEFAULT;

   int depth = 0;
   CPXgetcallbacknodeinfo(env, cbdata, wherefrom, 0, CPX_CALLBACK_INFO_NODE_DEPTH, &depth);
   long nodeindex = 10;
   CPXgetcallbacknodeinfo(env, cbdata, wherefrom, 0, CPX_CALLBACK_INFO_NODE_SEQNUM_LONG, &nodeindex);
   if(inst->nodeindex == nodeindex)
      return 0;
   else
      inst->nodeindex = nodeindex;
   /*
   int isvarbranch = 0;
   int nodebranch = 1;
   int* a = &nodebranch;
   CPXgetcallbacknodeinfo(env, cbdata, wherefrom, 0, CPX_CALLBACK_INFO_NODE_TYPE, &isvarbranch);
   if(isvarbranch == '0' )
   {
      CPXgetcallbacknodeinfo(env, cbdata, wherefrom, 0, CPX_CALLBACK_INFO_NODE_VAR, a);
      cout<<"nodebranch: "<<nodebranch<<endl;
   }
   */

   int lpstat;
   double objval = 0;
   int curncols = CPXgetnumcols(env, inst->lp);
   int curnrows = CPXgetnumrows(env, inst->lp);
   if( CPXgetcallbacknodex( env, cbdata, wherefrom, inst->x, 0, curncols-1 ) )
      cout<<"getnodex error!"<<endl;
   if( CPXgetcallbacknodelb( env, cbdata, wherefrom, inst->lb, 0, curncols-1 ) )
      cout<<"getnodelb error!"<<endl;
   if( CPXgetcallbacknodeub( env, cbdata, wherefrom, inst->ub, 0, curncols-1 ) )
      cout<<"getnodeub error!"<<endl;
#if CUT
   inst->numchg = 0;
   double bestobj = 0;
   CPXgetcallbackinfo(env, cbdata, wherefrom, CPX_CALLBACK_INFO_BEST_INTEGER, &bestobj );
   nodepresolve(inst, bestobj);
   for(int i = 0; i<inst->numchg; i++)
   {
      inst->rmatind[0] = inst->chgind[i];
      assert(inst->rmatind[0]>=0 && inst->rmatind[0]<curncols);
      if(inst->sign[i] == 'L')
      {
//         if(inst->x[inst->chgind[i]] >= 1e-8)
         {
            inst->rmatval[0] = 1;
            inst->status = CPXcutcallbackaddlocal (env, cbdata, wherefrom, 1, 0, 'L', inst->rmatind, inst->rmatval);
         }
      }
      if(inst->sign[i] == 'G')
      {
//         if(inst->x[inst->chgind[i]] <= 1-1e-8)
         {
            inst->rmatval[0] = 1;
            inst->status = CPXcutcallbackaddlocal (env, cbdata, wherefrom, 1, 1, 'G', inst->rmatind, inst->rmatval);
         }
      }
      
      if(inst->status != 0)
         cout<< "CPXcutcallbackaddlocal error!"<<endl;
   }
#endif
   clock_t time_presolveend=clock();
   inst->presolve_node_time+=(double)(time_presolveend-time_presolvestart)/(double)CLOCKS_PER_SEC;
   return 0;
}

