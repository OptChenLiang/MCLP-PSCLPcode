#include "global_functions.h"
#include "presolve.h"
#include "read.h"

//Read facility and client files if files exist and implement presolving methods
void read_file(mystr *inst)
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
      
      inst->coordinates_loaded = false;
      cout << "Client file could not be opened. " << endl;
   }
   else
      inst->coordinates_loaded = true;

   //The number of facilities/clients does not exceed the number of rows in files.
   double a;
   in_f >> a;
   if(inst->n_locations > a)
      inst->n_locations = (int)floor(a+1e-8);
   if( inst->coordinates_loaded == true)
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

   //If customer file exists,  
   if( inst->coordinates_loaded == true)
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
      if(inst->isPSCLP)
      {
         mypair->demand = 0;
         mypair->cost = inst->fixed_cost[j];
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

   cout<<"RADIUS: "<<inst->RADIUS<<endl;
	//////////////////////////
   //Initialize statistics
   inst->num_easy = 0;
   inst->num_col = 0;
   inst->num_row = 0;
   inst->presolve_time = 0.0;
   inst->presolve_IA_time = 0.0;
   inst->presolve_D_time = 0.0;
   inst->presolve_node_time = 0.0;
 
   //Implement isomorphic aggregations
   IA(inst);
	

   if(inst->isPSCLP)
   {
      if(inst->COVERING_DEMAND <= 1+1e-8)
      {
         //Calculate total valid demand of customers (covered by at least one potential facilities).
         double ratio = 0;
         for(int i = 0; i<inst->data.size(); i++)
         {
            if(inst->data[i]->locations.size() > 0)
               ratio = ratio + inst->data[i]->demand;
         }
         inst->COVERING_DEMAND = (int)ceil(inst->COVERING_DEMAND * ratio);
         cout<<"BUDGET: "<<inst->COVERING_DEMAND<<endl;
      }

   }
   else
   {
      if(inst->BUDGET < 1)
      {
         //Calculate total valid cost of facilities.
         double ratio = 0;
         for(int i = 0; i<inst->n_locations; i++)
            ratio += inst->covers[i]->demand;
         inst->BUDGET = (int)floor(inst->BUDGET * ratio);
         cout<<"BUDGET: "<<inst->BUDGET<<endl;
      }
   }
   
   inst->numchg = 0;
   inst->chgind = (int*) calloc(inst->data.size() + inst->covers.size(), sizeof(int));
   inst->sign = (char*) calloc(inst->data.size() + inst->covers.size(), sizeof(char));
   inst->n_location = (int*) calloc(inst->data.size() + inst->covers.size(), sizeof(int));
   
   int size1 = inst->n_data;
   cout<<"Row1: "<<inst->n_data<<endl;
   cout<<"presolve_IA1: "<<inst->presolve_IA_time<<endl;
   // Calculate J(i), i in I
   CalculateCovers(inst->covers, inst->data);

   inst->isfind = true;
   inst->validlocations = inst->n_locations;
   //Domination presolving
   if(inst->isD)
   {
      inst->isfind = false;
      Domination(inst);
      int n_deleted = 0;
      int j = 0;
      for ( int i = 0; i < inst->n_locations; i++ )
      {
         if(inst->covers[i]->isdeleted)
         {
            n_deleted++;
         }
         else
         {
            inst->covers[i]->pos = j;
            j++;
         }
      }
      cout<<"n_deleted_columns: "<<n_deleted<<endl;
      int lsize = 0, ncounter = 0;
      for(int i = 0; i<inst->n_data; i++)
      {
         lsize = inst->data[i]->locations.size();
         for(int j = 0; j<lsize; j++)
         {
            inst->data[i]->locations[j] = inst->covers[ inst->data[i]->locations[j] ]->pos; 
         }
      }
      j = 0;
      for ( int i = 0; i < inst->n_locations; i++ )
      {
         if(inst->covers[i]->isdeleted)
         {
            delete inst->covers[i];
         }
         else
         {
            inst->covers[j] = inst->covers[i];
            j++;
         }
      }
      inst->covers.erase(inst->covers.begin()+j, inst->covers.end());
      inst->n_locations = inst->covers.size();
      
      if(inst->isfind)
         //Reimplement isomorphic aggregation if domination presolving succeeds
         IA2(inst);
      
      cout<<"n_deleted_rows: "<<size1 - inst->n_data<<endl;
      inst->validlocations = inst->covers.size() - n_deleted;
   }
   if(inst->isSA)
   {
      //Reimplement singleton aggregations after domination presolving succeeds
      SA(inst);
   }
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
   //Statistics after presolving
   cout<<"mindemand: "<< mindemand<<" maxdemand: "<<maxdemand<<" meandemand: "<<meandemand<<endl;
   nnz += inst->validlocations;
   cout<<"DPANNZ= "<<nnz<<endl;
   cout<<"Row2: "<<inst->n_data<<endl;
}
//Free intermediate variables
/*****************************************************************/
void free_data(mystr *inst)
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

/*****************************************************************/
void READ_NEW_FILE1(instance *inst, mystr* inst1)
/*****************************************************************/
{

	//cout << "INSTANCE\t" << inst->input_file << endl;


	//ifstream in(inst->input_file);
	inst->n_locations = inst1->covers.size();
	inst->n_clients = inst1->data.size();

	//cout << "\n\n***LOCATIONS\t" << inst->n_locations << endl;
	//cout << "***CLIENTS\t" << inst->n_clients << endl;

	inst->fixed_cost = (int *) calloc(inst->n_locations, sizeof(int));
	inst->singleton = (double *) calloc(inst->n_locations, sizeof(double));
	inst->demand = (int *) calloc(inst->n_clients, sizeof(int));

	//inst->x_location= (double *) calloc(inst->n_locations, sizeof(double));
	//inst->y_location= (double *) calloc(inst->n_locations, sizeof(double));

	//inst->x_client= (double *) calloc(inst->n_clients, sizeof(double));
	//inst->y_client= (double *) calloc(inst->n_clients, sizeof(double));

	inst->cohordinates_loaded=true;


   double sumcost = 0;
	for ( int j = 0; j < inst->n_locations; j++ )
	{
		//char dummy_char;
		//int dummy_int;

		//in >>dummy_char;
		//in >>dummy_int;

//		cout << dummy_char << "\t" <<dummy_int << endl;
//		cin.get();

//		in >> inst->x_location[j];
//		in >> inst->y_location[j];
		inst->singleton[j] = inst1->covers[j]->cost;
		inst->fixed_cost[j] = 1;
      sumcost += inst->fixed_cost[j];
	}
   if(inst1->BUDGET < 1)
      inst->BUDGET = inst1->BUDGET*sumcost;
   else
      inst->BUDGET = inst1->BUDGET;
      

	for ( int i = 0; i < inst->n_clients; i++ )
	{

		//char dummy_char;
		//int dummy_int;

		//in >>dummy_char;
		//in >>dummy_int;

//		cout << dummy_char << "\t" <<dummy_int << endl;
//		cin.get();

//		in >> inst->x_client[i];
//		in >> inst->y_client[i];
//		in >> inst->demand[i];
		inst->demand[i] = inst1->data[i]->cost;
	}


#ifdef	print_point
	for ( int j = 0; j < inst->n_locations; j++ )
	{
		cout << "Location\t" <<  j << "\t(x,y)\t" << inst->x_location[j] << "\t" << inst->y_location[j] << endl;
	}

	for ( int i = 0; i < inst->n_clients; i++ )
	{

		cout << "Client\t" << i << "\t(x,y)\t" << inst->x_client[i]  << "\t" << inst->y_client[i] << endl;
	}
#endif



#ifdef	print_distances
	cout << "DISTANCES\n";
	for ( int j = 0; j < inst->n_locations; j++ )
	{
		cout << "location\t" << j << endl;
		for ( int i = 0; i < inst->n_clients; i++ )
		{
			cout << "distance to client\t" << i << "\t" << distance_manatthan(inst->x_location[j],inst->y_location[j],inst->x_client[i],inst->y_client[i]) << "\t" << distance(inst->x_location[j],inst->y_location[j],inst->x_client[i],inst->y_client[i])<< endl;

		}
	}
	cin.get();
#endif



	cout << "\n\nBUILDING neighbourhoods\n";
	vector < vector < int > > dummy;
#if 1
	for ( int i = 0; i < inst1->n_data; i++ ){

		vector < int > local_dummy;

      local_dummy = inst1->data[i]->locations;

		dummy.push_back(local_dummy);
	}
#else
	for ( int i = 0; i < inst->n_clients; i++ ){

		vector < int > local_dummy;

		for ( int j = 0; j < inst->n_locations; j++)
		{

			if(distance(inst->x_client[i],inst->y_client[i],inst->x_location[j],inst->y_location[j]) < inst->RADIUS)
			{
				local_dummy.push_back(j);
			}
		}
		dummy.push_back(local_dummy);
	}
#endif
	///////////////////////////////////
	build_data_structure(inst,dummy);
	///////////////////////////////////

}

