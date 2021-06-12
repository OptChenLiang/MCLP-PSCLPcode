#ifndef DFL_BEN_2_HEADER
#define DFL_BEN_2_HEADER


#include "global_variables.h"
#include "global_functions.h"
#include "struct.h"

using namespace std;


/*****************************************************************/
void build_model_DFL_BEN_2(instance *inst);
/*****************************************************************/

/*****************************************************************/
void solve_model_DFL_BEN_2(instance *inst, mystr* inst1);
/*****************************************************************/

/*****************************************************************/
void clean_model_DFL_BEN_2(instance *inst);
/*****************************************************************/

/*****************************************************************/
void solve_continuous_relaxation_DFL(instance *inst);
/*****************************************************************/


#endif
