#ifndef PRESOLVE__H
#define PRESOLVE__H
#include "global_variables.h"

//Calculate square of Euclidean distance of (x1,y1) and (x2,y2)
double distance(double x1,double y1,double x2, double y2);

bool CompforDNC(MyPair* a, MyPair *b);
bool CompforIndex(MyPair* a, MyPair *b);
bool CompEQ(MyPair* a, MyPair *b);

//Determine inclusive relations of  support signature in 32-bit
bool IsSubSet32(vector<MyPair*> &a, int i, int j);

//Determine inclusive relations of coverage facilities
bool IsSubSet(vector<int> &a, vector<int> &b);

//Remove the elements of Set b in Set a in nonzero cancellation 
bool RemoveSubSet(vector<int> &a, vector<int> &b);

//Calculate J(i) based on I(j)
bool CalculateCovers(vector<MyPair*> &a, vector<MyPair*> &b);

//Isomorphic aggregation and generate random data
void IA(instance *inst);

//Reimplement isomorphic aggregation if domination presolving succeeds
void IA2(instance* inst);

//Domination presolving
void Domination(instance* inst);

//Singleton aggregation
void SA(instance* inst);

//Node presolving of nonoverlap fixing
void NodePresolve(instance* inst, double bestobj);
#endif
