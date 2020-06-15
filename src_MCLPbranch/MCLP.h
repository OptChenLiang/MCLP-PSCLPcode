#ifndef DFL_HEADER
#define DFL_HEADER


#include "global_variables.h"
#include "global_functions.h"

using namespace std;

/*****************************************************************/
void build_model(instance *inst);
/*****************************************************************/

/*****************************************************************/
void solve_model(instance *inst);
/*****************************************************************/

/*****************************************************************/
void clean_model(instance *inst);
/*****************************************************************/
static int CPXPUBLIC
mycutcallback (CPXCENVptr env,
      void       *cbdata,
      int        wherefrom,
      void       *cbhandle,
      int        *useraction_p);
#endif
