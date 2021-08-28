#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

#include "struct.h"

//Read facility and client files if files exist and implement presolving methods
void read_file(mystr *inst);
/*****************************************************************/

//Free intermediate variables
void free_data(mystr *inst);
/*****************************************************************/

//Build data structures based on read data
void READ_NEW_FILE1(instance *inst, mystr* inst1);
/*****************************************************************/
#endif
