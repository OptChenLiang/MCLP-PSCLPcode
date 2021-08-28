## Introduction
The README contains code and complementary material to the paper

L. Chen, S.-J. Chen, W.-K. Chen, Y.-H. Dai, and J. Juan, (2021). 
Efficient presolving methods for solving maximal covering and partial set covering location problems

To solve the maximal covering location problem (MCLP) and partial set covering location problem (PSCLP), the branch-and-cut 
algorithm is used in the paper combined with five proposed presolving methods, i.e., 
i)   singleton aggregation (SIN_AGG), 
ii)  isomorphic aggregation (ISO_AGG),
iii) nonzero cancellation (NON_CAN),
iv)  domination (DOM), and
v)   nonoverlap fixing (NON_FIX), 
for both the MCLP and PSCLP.

The directory "PRECPX/" includes all source files and header files for solving the MCLP and PSCLP using CPLEX (CPX);
the directory "PREBD/" includes all source files and header files for solving the MCLP and PSCLP using the Benders Decomposition (BD);
the directory "TESTDATA/" contains the related data sets in the paper; . 
Note that, to compile and run the code, the CPLEX libraries and header files are required, which given in directories 
"lib/" and "include/".

## Executable files
We have written the "Makefile" in the main directory, and one just needs to run "make" in directory "PRECPX/" or "PREBD/"
to compile and generate the executable files in a new created directory "bin/" in the corresponding directory. 
   PRECPX/bin/MCLPCPX   to solve the MCLP using CPX 
   PRECPX/bin/PSCLPCPX  to solve the PSCLP using CPX
   PREBD/bin/MCLPBD     to solve the MCLP using BD
   PREBD/bin/PSCLPBD    to solve the PSCLP using BD
As for whether to use presolving methods, see the parameters of executable files in "How to run the solver".

## Data sets
G
For all testsets, 
For testset T1
For testsets T2 and T3, the  
Directory "data/" contains all data files used in our paper, including eight social networks files "MSG.txt", "GNU.txt", "HEP.txt",
"ENRON.txt", "FACEBOOK.txt", "DEEZER.txt", "TWITTER.txt", "EPINIONS.txt". The download links of these networkss are

## How to run the solver
For a given executable file [EXE], the solver can be run in the main directory as

  ./bin/[EXE] [PS] [FF] [FC] [NF] [NC] [TL] [R] [P] ([RS])

where [EXE] is executable file (IMPSolving or OnlyPresolve_IMPSolving) and the arguments are: 
[PS] PRECPX 0 for CPX; 
            1 for PRE+CPX;
            2 for No SIN_AGG;
            3 for No ISO_AGG;
            4 for No NON_CAN;
            5 for No DOM;
            6 for No NON_FIX;
     PREBD  0 for BD;
            1 for PRE+BD;
[FF] facilities file; 
[FC] customers file (can be NULL and then generate customers randomly when running the code);
[NF] number of facilities;
[NC] number of customers
[TL] time limitation in seconds;
[R]  covering radius;
[P]  budget in the MCLP or covering demand in the PSCLP,
     <= 1 means percentage;
[RS] CPXPARAM_RandomSeed for CPX (optional).

For instance, considering the MCLP with PRE+CPX, if [TL]=10000s, [NF]=100, [NC]=100000, [R]=5.75, and [P]=0.1, 
one can run 

  ./bin/MCLPCPX 0 1 ../TESTDATA/f_data/n100_f10_100_s2.dat NULL 100 100000 10000 5.75 0.1

For instance, considering the PSCLP with PRE+BD, if [TL]=10000s, [NF]=1500, [NC]=10000000, [R]=1.0328, and [P]=0.7, 
one can run 

  ./bin/BD 1 1 ../TESTDATA/n2500s5.dat NULL 1500 10000000 10000 1.0328 0.7
