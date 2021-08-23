#include "presolve.h"
#include <cstdlib>
#define MAXFACILITY 2500
#define MAXCLIENT (MAXFACILITY*MAXFACILITY) 
/*
double distance(double x1,double y1,double x2, double y2)
{
	double distancex = (x2 - x1)*(x2 - x1);
	double distancey = (y2 - y1)*(y2 - y1);
	return  distancex + distancey;
}*/

bool Comp(MyPair* a, MyPair *b)
{
   int size1 = a->locations.size();
   int size2 = b->locations.size();
   for( int i = 0; i < size1 && i< size2; i++)
   {
      if( a->locations[i] < b->locations[i] )
         return true;
      else if( a->locations[i] > b->locations[i] )
         return false;
   }
   if(size1 < size2 )
      return true;
   else 
      return false;
   return false;
}

bool CompforDNC(MyPair* a, MyPair *b)
{
   int size1 = a->locations.size();
   int size2 = b->locations.size();
   if(size1 > size2)
      return true;
   else if (size1 < size2)
      return false;
   for( int i = 0; i < size1 && i< size2; i++)
   {
      if( a->locations[i] < b->locations[i] )
         return true;
      else if( a->locations[i] > b->locations[i] )
         return false;
   }
   return false;
}

bool CompforIndex(MyPair* a, MyPair *b)
{
   return a->index < b->index;
}

bool CompEQ(MyPair* a, MyPair *b)
{
   int size1 = a->locations.size();
   int size2 = b->locations.size();
   if(size1 != size2)
      return false;
   for( int i = 0; i < size1 && i< size2; i++)
   {
      if( a->locations[i] < b->locations[i] )
         return false;
      else if( a->locations[i] > b->locations[i] )
         return false;
   }
   return true;
}
// Set bound in node presolving
bool setbound(mystr* inst, int pos, char sign, char method)
{
   assert(pos >= 0 && pos <= inst->validlocations + inst->data.size());
   if(sign == 'L')
   {
      if( inst->ub[pos] > 1e-6)
      {
         inst->ub[pos] = 0;
         inst->chgind[inst->numchg] = pos;
         inst->sign[inst->numchg] = 'L';
         inst->numchg++;
         switch (method)
         {
            case 'e':
               inst->num_easy++;
               break;
            case 'c':
               inst->num_col++;
               break;
            case 'r':
               inst->num_row++;
               break;
            default:
               assert(0);
               break;
         }
      }
      else
         return false;
   }
   else if(sign == 'G')
   {
      if( inst->lb[pos] <= 1-1e-8 )
      {
         inst->lb[pos] = 1;
         inst->chgind[inst->numchg] = pos;
         inst->sign[inst->numchg] = 'G';
         inst->numchg++;
         //todo
         switch (method)
         {
            case 'e':
               inst->num_easy++;
               break;
            case 'c':
               inst->num_col++;
               break;
            case 'r':
               inst->num_row++;
               break;
            default:
               assert(0);
               break;
         }
      }
      else 
         return false;
   }
   else
      assert(0);
   return true;
}

bool CalculateCovers(vector<MyPair*> &a, vector<MyPair*> &b)
{
   int size = a.size();
   int bsize = b.size();
   for(int i = 0; i<size; i++)
   {
      a[i]->locations.clear();
   }
   for(int i = 0; i<bsize; i++)
   {      
      //inst->data[i]->Print();
      size = b[i]->locations.size();
      for(int k = 0; k<size; k++)
      {
         a[b[i]->locations[k]]->locations.push_back(i);
      }
   }
   return true;
}

bool IsSubSet(vector<int> &a, vector<int> &b)
{
   int size1 = a.size();
   int size2 = b.size();
   if(size1 < size2)
      return false;
   int i = 0, j = 0; 
   bool isSubset = true;
   while(i < size1 && j< size2)
   {
      if( a[i] == b[j] )
      {
         i++;
         j++;
      }
      else if( a[i] < b[j] )
      {
         i++;
      }
      else if( a[i] > b[j] )
      {
         isSubset = false;
         break;
      }
   }
   if(isSubset == false || j < size2)
   {
      return false;
   }
   return true;
}

bool RemoveSubSet(vector<int> &a, vector<int> &b)
{
   int size1 = a.size();
   int size2 = b.size();
   if(size1 < size2)
      return false;
   int i = 0, j = 0; 
   int n_locations = 0;
   while(i < size1 && j< size2)
   {
      if( a[i] == b[j] )
      {
         i++;
         j++;
      }
      else if( a[i] < b[j] )
      {
         a[n_locations] = a[i]; 
         n_locations++;
         i++;
      }
      else if( a[i] > b[j] )
      {
         assert(0);
      }
   }
   for(; i<size1; i++)
   {
      a[n_locations] = a[i]; 
      n_locations++;
   }
   a.erase(a.begin() + n_locations, a.end());
   return true;
}

void DominatedColumns(mystr* inst)
{
   clock_t time_presolvestart=clock();
   int asize = inst->covers.size();
   sort(inst->covers.begin(), inst->covers.end(), CompforDNC);
   for(int i = 0; i<asize; i++)
   {
      if(inst->covers[i]->isdeleted == true)
         continue;
      for(int j = i+1; j<asize; j++)
      {
         if(inst->covers[j]->isdeleted == true)
            continue;
         if(IsSubSet(inst->covers[i]->locations, inst->covers[j]->locations))
         {
            inst->covers[j]->isdeleted = true;
         }
      }
   }
   sort(inst->covers.begin(), inst->covers.end(), CompforIndex);
   //Remove the deleted facilities from the set I(j).
   int lsize = 0, ncounter = 0;
   for(int i = 0; i<inst->n_data; i++)
   {
      lsize = inst->data[i]->locations.size();
      ncounter = 0;
      for(int j = 0; j<lsize; j++)
      {
         if(inst->covers[inst->data[i]->locations[j]]->isdeleted == false)
         {
            inst->data[i]->locations[ncounter] = inst->data[i]->locations[j]; 
            ncounter++;
         }
      }
      inst->data[i]->locations.erase(inst->data[i]->locations.begin()+ncounter, inst->data[i]->locations.end());
   }
   clock_t time_presolveend=clock();
   inst->presolve_dc_time+=(double)(time_presolveend-time_presolvestart)/(double)CLOCKS_PER_SEC;
}

void DualParallelAggr2(mystr* inst)
{
   inst->isfind = false;
   clock_t time_presolvestart;
   clock_t time_presolveend;
   time_presolvestart=clock();
   unordered_map<vector<int>, MyPair*, MyVector_hasher, MyVector_equal> mymap;
   unordered_map<vector<int>, MyPair*, MyVector_hasher, MyVector_equal>::iterator it;
	for ( int i = 0; i < inst->n_data; i++ )
   {
      it = mymap.find(inst->data[i]->locations);
      if(it == mymap.end())
      {
         mymap.insert({ inst->data[i]->locations, inst->data[i]});
      }
      else
      {
         (*it).second->cost += inst->data[i]->cost;
         (*it).second->demand += inst->data[i]->demand;
         for(int j = 0; j<inst->data[i]->clients.size(); j++)
            (*it).second->clients.push_back(inst->data[i]->clients[j]);
         delete inst->data[i];
         inst->data[i] = NULL;
         inst->isfind = true;
      }
   }
   time_presolveend = clock();
   inst->presolve_dpa_time += (double)(time_presolveend-time_presolvestart)/(double)CLOCKS_PER_SEC;
   int ncounter = 0;
   for(int i = 0; i<inst->n_data; i++)
   {
      if(inst->data[i] != NULL)
      {
         inst->data[ncounter] = inst->data[i];
         ncounter++;
      }
   }
   inst->data.erase(inst->data.begin()+ncounter, inst->data.end());
   inst->n_data = inst->data.size();
   CalculateCovers(inst->covers, inst->data);
   return;
}

void DualParallelAggr(mystr *inst)
{
   if(inst->coordinates_loaded == false)
      cout<< "Randomly generate!"<<endl;
   unordered_map<MyArray, MyPair*, MyArray_hasher, MyArray_equal> mymap;
   unordered_map<MyArray, MyPair*, MyArray_hasher, MyArray_equal>::iterator it;
   clock_t time_presolvestart;
   clock_t time_presolveend;
   MyPair* pair;
   int local_dummy_a[MAXFACILITY];
   int **totalarray;
   if(inst->isDpa)
      totalarray = new int*[inst->n_locations*inst->n_locations];
   else
      totalarray = new int*[inst->n_clients];
   int n_data = 0;
   double Distance = 0.0;
   double x,y;
   int len = 0;
   double rand_max = 1.0*RAND_MAX;
   MyArray local_dummy;
   local_dummy.a = local_dummy_a;
   local_dummy.len = len;
   time_presolvestart=clock();
   double rsquare = inst->RADIUS;
	for ( long long i = 0; i < inst->n_clients; i++ ){
      len = 0;
      if(inst->coordinates_loaded == false)
      {
         //Random generate points of clients
         x = rand()/rand_max*30;
         y = rand()/rand_max*30;
         for ( int j = 0; j < inst->n_locations; j++)
         {
            Distance = distance(x,y,inst->x_location[j],inst->y_location[j]);
            if(Distance < rsquare)
               local_dummy_a[len++] = j;
         }
      }
      else
      {
         for ( int j = 0; j < inst->n_locations; j++)
         {
            Distance = distance(inst->x_client[i],inst->y_client[i],inst->x_location[j],inst->y_location[j]);
            if( strcmp(inst->input_file_f, inst->input_file_c) == 0 && i == j)
               Distance = 0;
            if(Distance < rsquare)
            {
               local_dummy_a[len++] = j;
            }
         }
      }
      if(inst->isDpa)
      {
         local_dummy.a = local_dummy_a;
         local_dummy.len = len;
         it = mymap.find(local_dummy);
         if(it == mymap.end())
         {
            pair = new MyPair(n_data);
            totalarray[n_data] = new int[local_dummy.len];
            for(int ll = 0; ll<local_dummy.len; ll++)
            {
               pair->locations.push_back(local_dummy.a[ll]);
               totalarray[n_data][ll] = local_dummy.a[ll];
            }
            local_dummy.a = totalarray[n_data];
            if(inst->isPSCLP)
            {
               pair->cost = 0;
               if(inst->coordinates_loaded==true)
                  pair->demand = inst->demand[i];
               else
                  pair->demand = (rand() % 100)+ 1;
            }
            else
            {
               pair->demand = 0;
               if(inst->coordinates_loaded == true)
                  pair->cost = inst->demand[i];
               else
                  pair->cost += (rand() % 100)+ 1;
            }
            inst->data.push_back(pair);
            mymap.insert({ local_dummy, pair});
            n_data++;
         }
         else
         {
            if(inst->isPSCLP)
            {
               it->second->cost = 0;
               if(inst->coordinates_loaded==true)
                  it->second->demand = inst->demand[i];
               else
                  it->second->demand += (rand() % 100)+ 1;
            }
            else
            {
               it->second->demand = 0;
               if(inst->coordinates_loaded == true)
                  it->second->cost += inst->demand[i];
               else
                  it->second->cost += (rand() % 100)+ 1;
            }
         }
      }
      else
      {
         local_dummy.a = local_dummy_a;
         local_dummy.len = len;
         pair = new MyPair(n_data);
         totalarray[n_data] = new int[local_dummy.len];
         for(int ll = 0; ll<local_dummy.len; ll++)
         {
            pair->locations.push_back(local_dummy.a[ll]);
            totalarray[n_data][ll] = local_dummy.a[ll];
         }
         local_dummy.a = totalarray[n_data];
         if(inst->isPSCLP)
         {
            pair->cost = 0;
            if(inst->coordinates_loaded==true)
               pair->demand = inst->demand[i];
            else
               pair->demand = (rand() % 100)+ 1;
         }
         else
         {
            pair->demand = 0;
            if(inst->coordinates_loaded==true)
               pair->cost = inst->demand[i];
            else
               pair->cost = (rand() % 100)+ 1;
         }
         inst->data.push_back(pair);
         n_data++;
      }
   }
   time_presolveend=clock();
   inst->presolve_dpa_time+=(double)(time_presolveend-time_presolvestart)/(double)CLOCKS_PER_SEC;
   cout<< inst->presolve_dpa_time <<endl;
   for(int i = 0; i<n_data; i++)
   {
      delete[] totalarray[i];
   }
   delete[] totalarray;
   inst->n_data = inst->data.size();
}

void DualAggr(mystr* inst)
{
   int csize = inst->data.size();
   inst->n_data = 0;
   for(int i = 0; i<csize; i++)
   {
      if(inst->data[i]->locations.size() == 0)
      {
         delete inst->data[i];
      }
      else if(inst->data[i]->locations.size() == 1)
      {
         inst->covers[inst->data[i]->locations[0]]->cost += inst->data[i]->cost;
         inst->covers[inst->data[i]->locations[0]]->demand += inst->data[i]->demand;
         inst->singlecover.push_back(inst->data[i]->locations[0]);
         delete inst->data[i];
      }
      else
      {
         inst->data[inst->n_data] = inst->data[i];
         inst->n_data++;
      }
   }
   inst->data.erase(inst->data.begin()+inst->n_data, inst->data.end()); 
}

void NodePresolveInit(mystr* inst)
{
   int i = 0, j = 0;
   int posi = 0;
   int posj = 0;
   int lsize = 0;
   int datasize = inst->data.size();
   for(i = 0; i<datasize; i++)
   {
      posi = inst->data[i]->pos;
      assert(posi >= 0);
      if(inst->ub[posi] <= 1e-6)
      {
         inst->data[i]->isdeleted = true;
         lsize = inst->data[i]->locations.size();
         for(j = 0; j<lsize; j++)
         {
            posj = inst->covers[inst->data[i]->locations[j]]->pos;
            assert(posj >= 0);
            setbound(inst, posj, 'L', 'e');
         }
      }
      else 
         inst->data[i]->isdeleted = false;
   }
}

void NodePresolve(mystr* inst, double bestobj)
{
   NodePresolveInit(inst);
}
