#ifndef PRESOLVE__H
#define PRESOLVE__H
#include "struct.h"

//Calculate the distance between (x1,y1) and (x2,y2)
double distance(double x1,double y1,double x2, double y2);

bool Comp(MyPair* a, MyPair *b);
bool CompforDNC(MyPair* a, MyPair *b);
bool CompforIndex(MyPair* a, MyPair *b);
bool CompEQ(MyPair* a, MyPair *b);

//Set a contains Set b ?
bool IsSubSet(vector<int> &a, vector<int> &b);

//Remove the elements of Set a in Set b?
bool RemoveSubSet(vector<int> &a, vector<int> &b);

//Calculate J(i) based on I(j)
bool CalculateCovers(vector<MyPair*> &a, vector<MyPair*> &b);

//Isomorphic aggregation (and generate random data)
void IA(mystr *inst);

//Reimplement isomorphic aggregation when domination presolving succeeds
void IA2(mystr* inst);

//Domination presolving
void Domination(mystr* inst);

//Singleton aggregation
void SA(mystr* inst);

//Node presolving
void NodePresolve(mystr* inst, double bestobj);
#endif
