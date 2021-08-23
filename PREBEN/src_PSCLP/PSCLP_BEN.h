#ifndef CFL_BEN_2_HEADER
#define CFL_BEN_2_HEADER


#include "global_variables.h"
#include "global_functions.h"
#include "struct.h"

using namespace std;


/*****************************************************************/
void comb_solve_model_CFL_BEN_2_AUX(instance *inst);
/*****************************************************************/

/*****************************************************************/
void load_N_value_2(instance *inst,bool rounding);
/*****************************************************************/

//Build Benders master problem
void build_model_CFL_BEN_2(instance *inst);
/*****************************************************************/

//Solve PSCLP using Benders decomposition
void solve_model_CFL_BEN_2(instance *inst, mystr* inst1);
/*****************************************************************/

//Close CPLEX environment
void clean_model_CFL_BEN_2(instance *inst);
/*****************************************************************/

/*****************************************************************/
void solve_continuous_relaxation_CFL(instance *inst);
/*****************************************************************/


#endif
