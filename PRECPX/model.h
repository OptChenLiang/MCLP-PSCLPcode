#ifndef DFL_HEADER
#define DFL_HEADER


#include "global_variables.h"
#include "global_functions.h"

using namespace std;

//Build MCLP or PSCLP model
void build_model(instance *inst);
/*****************************************************************/

//Solve PSCLP or MCLP model using CPLEX
void solve_model(instance *inst);
/*****************************************************************/

//Close CPLEX environments
void clean_model(instance *inst);
/*****************************************************************/

//Node presolving callback of nonoverlap fixing
static int CPXPUBLIC
mycutcallback (CPXCENVptr env,
      void       *cbdata,
      int        wherefrom,
      void       *cbhandle,
      int        *useraction_p);
#endif
