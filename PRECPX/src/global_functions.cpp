#include "global_functions.h"
#include "presolve.h"

//Read facility and client files if files exist.
void read_file(instance *inst)
{
   cout << "Facility information: " << inst->input_file_f << endl; /* Facility file */
   cout << "Customer information: " << inst->input_file_c << endl; /* Client file */

   ifstream in_f(inst->input_file_f);
   if(!in_f)
   {
      ofstream err("Error.log",ios::app);
      cout << "Faclity file could not be opened. " << endl;
      exit(1);
   }
   ifstream in_c(inst->input_file_c);
   if(atoi(inst->input_file_c) > 0)
   {
      inst->n_clients = atoi(inst->input_file_c); 
      inst->coordinates_loaded = false;
      cout << "Customer file could not be opened. " << endl;
   }
   else
      inst->coordinates_loaded = true;

   //The number of facilities/clients does not exceed the number of rows in files.
   in_f >> inst->n_locations;
   if( inst->coordinates_loaded == true)
   {
      in_c >> inst->n_clients;
   }
   cout << "Original model: "<< endl;
   cout << "Number of facilities: " << inst->n_locations << endl;
   cout << "Numer of customers: " << inst->n_clients << endl;

   inst->fixed_cost = (double *) calloc(inst->n_locations, sizeof(double));
   inst->x_location= (double *) calloc(inst->n_locations, sizeof(double));
   inst->y_location= (double *) calloc(inst->n_locations, sizeof(double));

   //If customer file exists,  
   if( inst->coordinates_loaded == true)
   {
      inst->demand = (double *) calloc(inst->n_clients, sizeof(double));
      inst->x_client= (double *) calloc(inst->n_clients, sizeof(double));
      inst->y_client= (double *) calloc(inst->n_clients, sizeof(double));
   }

   //Generate client pointer
   MyPair* mypair; 
   vector<int> vec;
   int dummy_int;
   double a;
	for ( int j = 0; j < inst->n_locations; j++ )
	{
		in_f >> dummy_int;
		in_f >> inst->x_location[j];
		in_f >> inst->y_location[j];
		in_f >> inst->fixed_cost[j];
		in_f >> a;
      mypair = new MyPair(j);
      if(inst->isPSCLP)
      {
         mypair->demand = 0;
         mypair->cost = inst->fixed_cost[j];
         //mypair->cost = rand()%10 + 90;//inst->fixed_cost[j];
      }
      else
      {
         mypair->demand = inst->fixed_cost[j];
         mypair->cost = 0;
      }
      mypair->index = j;
      inst->covers.push_back(mypair);
	}
   if( inst->coordinates_loaded == true)
   {
      for ( int i = 0; i < inst->n_clients; i++ )
      {
         in_c >> dummy_int;
         in_c >> inst->x_client[i];
         in_c >> inst->y_client[i];
         if( strcmp(inst->input_file_f, inst->input_file_c) == 0)
         {
            inst->x_client[i] = inst->x_location[i];
            inst->y_client[i] = inst->y_location[i];
         }
		   in_c >> a;
         in_c >> inst->demand[i];
      }
   }

   //cout<<"Radius: "<<inst->RADIUS<<endl; /* Coverage radius */
	//////////////////////////
   //Initialize statistics
   inst->nfix = 0;
   inst->presolve_time = 0.0; 
   inst->presolve_IA_time = 0.0;
   inst->presolve_NC_time = 0.0;
   inst->presolve_D_time = 0.0;
   inst->presolve_node_time = 0.0;
   //If the customer file does not exits, generate randomly the coordinates of locations of customers.
   IA(inst); /* Generate coordinates of locations and do isomorphic aggregation */
   //PSCLP model
   if(inst->isPSCLP)
   {
      if(inst->COVERING_DEMAND <= 1+1e-8)
      {
         //Calculate total valid demand of customers (covered by at least one potential facilities).
         double TotalDemand = 0;
         for(int i = 0; i<inst->data.size(); i++)
         {
            if(inst->data[i]->locations.size() > 0)
               TotalDemand += inst->data[i]->demand;
         }
         inst->COVERING_DEMAND = ceil(inst->COVERING_DEMAND * TotalDemand);
         //cout<<"Demand: "<<inst->COVERING_DEMAND<<endl; /* The required coverage demand */
      }

   }
   //MCLP model
   else
   {
      if(inst->BUDGET < 1)
      {
         //Calculate total valid cost of facilities.
         double TotalCost = 0;
         for(int i = 0; i<inst->n_locations; i++)
            TotalCost += inst->covers[i]->demand;
         inst->BUDGET = floor(inst->BUDGET * TotalCost);
         //cout<<"Budget: "<<inst->BUDGET<<endl; /* The required coverage budget */
      }
   }
   
   inst->numchg = 0;
   inst->chgind = (int*) calloc(inst->data.size() + inst->covers.size(), sizeof(int));
   inst->sign = (char*) calloc(inst->data.size() + inst->covers.size(), sizeof(char));
   inst->n_location = (int*) calloc(inst->data.size() + inst->covers.size(), sizeof(int));
   
   int size1 = inst->n_data;
   //cout<<"Row1: "<<inst->n_data<<endl; 
   //cout<<"presolve_ISO_AGG: "<<inst->presolve_IA_time<<endl;
   // Calculate J(i), i in I
   CalculateCovers(inst->covers, inst->data);

   inst->isfind = true;
   inst->validlocations = inst->n_locations;
   //Do domination presolving
   if(inst->isDom)
   {
      inst->isfind = false;
      Domination(inst);
      int n_deleted = 0;
      for ( int i = 0; i < inst->n_locations; i++ )
      {
         if(inst->covers[i]->isdeleted)
            n_deleted++;
      }
      //cout<<"Number of deleted columns: "<<n_deleted<<endl;
      
      if(inst->isfind)
         //Reimplement isomorphic aggregation if domination presolving succeeds
         IA2(inst);
      
      //cout<<"n_deleted_rows: "<<size1 - inst->n_data<<endl;
      inst->validlocations = inst->covers.size() - n_deleted;
   }
   if(inst->isSA)
   {
      //Reimplement singleton aggregation after domination presolving
      SA(inst);
   }
   //////////////////////////////////
   //Calculate statistics after presolving
   int nnz = 0;
   for(int i = 0; i<inst->n_data; i++)
   {
      if(inst->data[i]->isdeleted == false)
      {
         nnz = nnz + inst->data[i]->locations.size();
      }
   }
   
   if(inst->isPSCLP)
      nnz += inst->validlocations;
   else
      nnz += inst->n_data;
   cout<<"NNZ before nonzero cancellation: "<<nnz<<endl; /* Number of nonzeros in model after presolving */
}

//Free intermediate variables
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
   if(inst->coordinates_loaded==true){
      free(inst->demand);
      free(inst->x_client);
      free(inst->y_client);
   }

}
