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
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <cstddef>
#include <functional>
#include "include/cplex/cplex.h"

using namespace std;

class MyPair
{
public:
   vector<int> locations;
   vector<long long> clients;
   double demand;
   double cost;
   int index;
   vector<int> fronts;
   vector<int> same;
   vector<int> nexts;
   bool isdeleted;
   int pos;
   MyPair(int ind)
   {
      demand = 0; 
      index = ind; 
      isdeleted = false; 
   }
   ~MyPair(){};
   void Print()
   {
      int size = locations.size(); 
      cout<<"index: "<<index<<" ";
      cout<<"deleted: "<<isdeleted<<" ";
      cout<<"locations: ";
      for(int j = 0; j<size; j++)
      {
         cout<<locations[j]<<" ";
      }
      size = clients.size(); 
      if(size > 0)
         cout<<"\t clients: ";
      for(int j = 0; j<size; j++)
      {
         cout<<clients[j]<<" ";
      }
      size = fronts.size(); 
      if(size > 0)
         cout<<"\t fronts: ";
      for(int j = 0; j<size; j++)
      {
         cout<<fronts[j]<<" ";
      }
      size = nexts.size(); 
      if(size > 0)
         cout<<"\t nexts: ";
      for(int j = 0; j<size; j++)
      {
         cout<<nexts[j]<<" ";
      }
      cout<<endl;
   }
};

struct MyArray
{ 
   int* a;
   int len;
};

struct MyArray_hasher
{ 
   size_t operator ()(MyArray const& r) const
   {
      int size = r.len;
      size_t t = 0;
      for(int i = 0; i<size; i++)
         t = t + hash<int>()(r.a[i]);
      return t;
   }
};

struct MyArray_equal
{
   bool operator()(MyArray const& lhs, MyArray const& rhs) const
   {
      int size1 = lhs.len;
      int size2 = rhs.len;
      if(size1 != size2)
         return false;
      for(int i = 0; i<size1; i++)
      {
         if(lhs.a[i] != rhs.a[i])
            return false;
      }
      return true;
   }
};

struct MyVector_hasher
{ 
   size_t operator ()(vector<int> const& r) const
   {
      int size = r.size();
      size_t t = 0;
      for(int i = 0; i<size; i++)
         t = t + hash<int>()(r[i]);
      return t;
   }
};

struct MyVector_equal
{
   bool operator()(vector<int> const& lhs, vector<int> const& rhs) const
   {
      int size1 = lhs.size();
      int size2 = rhs.size();
      if(size1 != size2)
         return false;
      for(int i = 0; i<size1; i++)
      {
         if(lhs[i] != rhs[i])
            return false;
      }
      return true;
   }
};

typedef struct {
////////////////////
   vector<MyPair*> data;
   vector<MyPair*> covers;
   vector<int> singlecover;

   int n_data;

	int algorithm;
	char *input_file_f;
	char *input_file_c;
	int n_locations;
	long long n_clients;

	double *demand;
	double *fixed_cost;

   bool findsolution;
	double RADIUS;
   double sumvalid;
	double COVERING_DEMAND;
	double BUDGET;
   
   bool isfind;
   int validlocations;

	double timelimit;

	int seed;

	int number_of_CPU;
   
   int* n_location;

   int numchg;
   int* chgind;
   char* sign;

   long nodeindex;

#if 0
	///////////////////////////////////////////////////////////////////////////////
	int *NFS;     // NFS[i]: begin of FS(i) in AFS
	int *AFS;     // arcs index ordered for FS
	int *DP;  // number of neighbours delta+

	int *NBS;     // NBS[i]: begin of BS(i) in ABS
	int *ABS;     // arcs index ordered for BS
	int *DM;  // number of neighbours delta-
	///////////////////////////////////////////////////////////////////////////////
#endif

	bool cohordinates_loaded;
	double *x_location;
	double *y_location;

	double *x_client;
	double *y_client;

	/////////////////////////////////////CPLEX/////////////////////////////////////
	CPXENVptr env;
	CPXLPptr lp;
	int status,ccnt,rcnt,nzcnt;
	int* rmatbeg, *rmatind,*cmatbeg, *cmatind;
	double* rmatval,*cmatval,*x,*pi,*obj, *lb, *ub,*rhs;
	char *c_type,* sense;
	char **colname;
	double objval,bestobjval;
	int lpstat,nodecount;
	///////////////////////////////////////////////////////////////////////////////

	int counter_c;
	int counter_l;

   double presolve_time;
   double presolve_dpa_time;
   double presolve_dnc_time;
   double presolve_dc_time;
   double presolve_node_time;

   long num_easy;
   long num_col;
   long num_row;
} instance;


#endif
