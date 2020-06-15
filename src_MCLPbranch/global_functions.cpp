

#include "global_functions.h"
#include "presolve.h"

//Read facility and client files if files exist.
void read_file(instance *inst)
{
   cout << "INSTANCE_f " << inst->input_file_f << endl;
   cout << "INSTANCE_c " << inst->input_file_c << endl;

   ifstream in_f(inst->input_file_f);
   ifstream in_c(inst->input_file_c);
   if(!in_f)
   {
      ofstream err("Error.log",ios::app);
      cout << "Faclity file could not be opened. " << endl;
      exit(1);
   }
   if(!in_c)
   {
      inst->cohordinates_loaded = false;
      cout << "Client file could not be opened. " << endl;
   }
   else
      inst->cohordinates_loaded = true;

   //The number of facilities/clients does not excceed the number of rows in files.
   double a;
   in_f >> a;
   if(inst->n_locations > a)
      inst->n_locations = (int)floor(a+1e-8);
   if( inst->cohordinates_loaded == true)
   {
      in_c >> a;
      if(inst->n_clients > a)
         inst->n_clients = (int)floor(a+1e-8);
   }

   cout << "***LOCATIONS\t" << inst->n_locations << endl;
   cout << "***CLIENTS\t" << inst->n_clients << endl;

   inst->fixed_cost = (double *) calloc(inst->n_locations, sizeof(double));
   inst->x_location= (double *) calloc(inst->n_locations, sizeof(double));
   inst->y_location= (double *) calloc(inst->n_locations, sizeof(double));

   if( inst->cohordinates_loaded == true)
   {
      inst->demand = (double *) calloc(inst->n_clients, sizeof(double));
      inst->x_client= (double *) calloc(inst->n_clients, sizeof(double));
      inst->y_client= (double *) calloc(inst->n_clients, sizeof(double));
   }

   MyPair* mypair;
   vector<int> vec;
   char dummy_char;
   int dummy_int;
	for ( int j = 0; j < inst->n_locations; j++ )
	{
      in_f >> dummy_char;
		in_f >> dummy_int;
		in_f >> inst->x_location[j];
		in_f >> inst->y_location[j];
		in_f >> inst->fixed_cost[j];
      inst->fixed_cost[j] = 1;
      mypair = new MyPair(j);
      mypair->demand = inst->fixed_cost[j];
      mypair->cost = 0;
      mypair->index = j;
      inst->covers.push_back(mypair);
	}
   if(inst->BUDGET < 1)
   {
      double sumdemand = 0;
      for(int i = 0; i<inst->n_locations; i++)
         sumdemand += inst->covers[i]->demand;
      inst->BUDGET = (int)floor(inst->BUDGET * sumdemand);
   }
   if( inst->cohordinates_loaded == true)
   {
      for ( int i = 0; i < inst->n_clients; i++ )
      {

         char dummy_char;
         int dummy_int;

         in_c >>dummy_char;
         in_c >>dummy_int;
         in_c >> inst->x_client[i];
         in_c >> inst->y_client[i];
         if( strcmp(inst->input_file_f, inst->input_file_c) == 0)
         {
            inst->x_client[i] = inst->x_location[i];
            inst->y_client[i] = inst->y_location[i];
         }
         in_c >> inst->demand[i];
      }
   }

	cout << "BUILDING neighbourhoods "<<endl;
   cout<<"BUDGET: "<<inst->BUDGET<<endl;
   cout<<"RADIUS: "<<inst->RADIUS<<endl;
	//////////////////////////
   inst->num_easy = 0;
   inst->num_col = 0;
   inst->num_row = 0;
   inst->presolve_time = 0.0;
   inst->presolve_dpa_time = 0.0;
   inst->presolve_dnc_time = 0.0;
   inst->presolve_dc_time = 0.0;
   inst->presolve_node_time = 0.0;
   //Dual parallel aggregation
   dualparallelaggr(inst);
   
   inst->numchg = 0;
   inst->chgind = (int*) calloc(inst->data.size() + inst->covers.size(), sizeof(int));
   inst->sign = (char*) calloc(inst->data.size() + inst->covers.size(), sizeof(char));
   inst->n_location = (int*) calloc(inst->data.size() + inst->covers.size(), sizeof(int));
   
   int size1 = inst->n_data;
   cout<<"Row1: "<<inst->n_data<<endl;
   cout<<"presolve_dpa1: "<<inst->presolve_dpa_time<<endl;
   // Calculate J(i), i in I
   CalculateCovers(inst->covers, inst->data);

   inst->isfind = true;
   inst->validlocations = inst->n_locations;
#if COL
   inst->isfind = false;
   dominatedcolumns(inst);
   if(inst->isfind)
      dualparallelaggr2(inst);
   int n_deleted = 0;
	for ( int i = 0; i < inst->n_locations; i++ )
   {
      if(inst->covers[i]->isdeleted)
         n_deleted++;
   }
   cout<<"n_deleted_columns: "<<n_deleted<<endl;;
   cout<<"n_deleted_rows: "<<size1 - inst->n_data<<endl;;
   inst->validlocations = inst->covers.size() - n_deleted;
#endif
#if DA
   dualaggr(inst);
#endif
	//////////////////////////////////
   int nnz = 0;
   double mindemand = 1e+20;
   double maxdemand = 0;
   double meandemand = 0;
   for(int i = 0; i<inst->n_data; i++)
   {
      if(inst->data[i]->isdeleted == false)
      {
         nnz = nnz + inst->data[i]->locations.size();
         if( inst->data[i]->demand > maxdemand )
            maxdemand = inst->data[i]->demand;
         if( inst->data[i]->demand < mindemand )
            mindemand = inst->data[i]->demand;
         meandemand += inst->data[i]->demand/inst->data.size();
      }
   }
   cout<<"mindemand: "<< mindemand<<" maxdemand: "<<maxdemand<<" meandemand: "<<meandemand<<endl;
   nnz += inst->validlocations;
   cout<<"DPANNZ= "<<nnz<<endl;
   cout<<"Row2: "<<inst->n_data<<endl;
}

/*****************************************************************/
void free_data(instance *inst)
   /*****************************************************************/
{
   int size = inst->data.size();
   for(int i = 0; i<size; i++)
   {
      delete inst->data[i];
   }
   for(int i = 0; i<inst->n_locations; i++)
   {
      delete inst->covers[i];
   }

   free(inst->fixed_cost);
   free(inst->chgind);
   free(inst->sign);
   free(inst->n_location);

   free(inst->x_location);
   free(inst->y_location);
   if(inst->cohordinates_loaded==true){
      free(inst->demand);
      free(inst->x_client);
      free(inst->y_client);
   }

}
