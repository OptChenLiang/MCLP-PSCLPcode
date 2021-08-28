#ifndef DFL_BEN_2_HEADER
#define DFL_BEN_2_HEADER


#include "global_variables.h"
#include "global_functions.h"
#include "struct.h"

using namespace std;


//Build Benders master problem
void build_model_DFL_BEN_2(instance *inst);
/*****************************************************************/

//Solve MCLP using Benders decomposition
void solve_model_DFL_BEN_2(instance *inst, mystr* inst1);
/*****************************************************************/

//Close CPLEX environment
void clean_model_DFL_BEN_2(instance *inst);
/*****************************************************************/

/*****************************************************************/
void solve_continuous_relaxation_DFL(instance *inst);
/*****************************************************************/


#endif
