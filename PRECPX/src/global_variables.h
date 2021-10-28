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
#include "../../include/cplex.h"

using namespace std;

//Covering relationship and aggregation information
class MyPair
{
   public:
      vector<int> locations; /* Vector of facilities covering the client */
      vector<long long> clients; /* Vector of covered clients */
      unsigned int sign;
      double demand; /* Client demand */
      double cost; /* Facility cost */
      int index; 
      bool isdeleted; /* Is the facility/client deleted by presolving */
      int pos;
      //Initialize
      MyPair(int ind)
      {
         demand = 0; 
         index = ind; 
         isdeleted = false; 
      }
      ~MyPair(){};
      //Output callback
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
         cout<<endl;
      }
};

//Hash key storing coverage facilities of a client in isomorphic aggregation
struct MyArray
{ 
   int* a;
   int len;
};

//Hash function
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

//Decide identify whether two clients have isomorphic coverage relations
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

//Hash function when reimplementing isomorphic aggregation if domination presolving succeeds
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

//Decide identify whether two clients have isomorphic coverage relations when reimplementing isomorphic aggregation if domination presolving succeeds
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

//MCLP instance
typedef struct {
   ////////////////////
   vector<MyPair*> data;  /* Rows(Clients) information */
   vector<MyPair*> covers;  /* Columns(Facilities) informaion */
   vector<int> singlecover;  

   int n_data; /* Number of clients after presolving */

   int algorithm; /* Solving settings */
   char *input_file_f; /* Facility file */
   char *input_file_c; /* Client file */
   int n_locations; /* Number of facilities */
   long long n_clients; /* Number of clients */
   bool is_percentage; /* Is percentage */

   double *demand; /*Demand vector of clients */
   double *fixed_cost; /* Cost vector of facilities */

   bool findsolution; 
   double RADIUS; /* Coverage radius */
   double sumvalid;
   double COVERING_DEMAND; /* The required coverage demand */
   double BUDGET; /* The required coverage budget */

   bool isfind; /* Does domination presolving succeed */
   int validlocations; /* Number of facilities after domination presolving */

   double timelimit;  /* Time limitation */

   int seed;  /* Random seed */

   int number_of_CPU; /* Number of CPUs */

   int* n_location; /*Vector of numbers of coverage facilities */

   int numchg;
   int* chgind;
   char* sign;

   long nodeindex;
   vector<bool> nodes;

   bool coordinates_loaded;
   double *x_location; /*X coordinates of facilities */
   double *y_location; /*Y coordinates of facilities */

   double *x_client; /*X coordinates of clients */
   double *y_client; /*Y coordinates of clients */

   //CPLEX related variables and environments
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

   double presolve_time; /* Total presolving time */
   double presolve_IA_time; /* Presolving time of isomorphic aggregation */
   double presolve_NC_time; /* Presolving time of nonzero cancellation */
   double presolve_D_time; /* Presolving time of domination presolving */
   double presolve_node_time; /* Prsolving time of nonoverlap fixing presolving */

   long nfix;

   bool isBranch;
   bool isCut;
   bool isBin; /*Set client variables binary or not */
   bool isIA; /* Do isomorphic aggregation or not */
   bool isSA; /* Do singleton aggregation or not */
   bool isDom; /* Do domination presolving or not */
   bool isNC; /* Do nonzero cancellation or not */
   bool isPSCLP; /* Is solving model a PSCLP (or a MCLP) */
} instance;


#endif
