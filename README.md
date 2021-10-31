## Introduction
This site contains code and complementary material to the paper

L. Chen, S.-J. Chen, W.-K. Chen, Y.-H. Dai, T. Quan, and J. Chen (2021). 
Efficient presolving methods for solving maximal covering and partial set covering location problems

To enhance of capability of solving the maximal covering location problem (MCLP) and 
partial set covering location problem (PSCLP), the paper proposes five presolving methods including

1. singleton aggregation (`SIN_AGG`), 
2. isomorphic aggregation (`ISO_AGG`),
3. nonzero cancellation (`NNZ_CAN`),
4. domination (`DOM`), and
5. nonoverlap fixing (`NON_FIX`).

Directory `PRECPX/` includes all source files and header files for solving the MCLP and PSCLP
using CPLEX (CPX) with the proposed presolving methods;

Directory `PREBD/` contains Makefile and the patch for the Benders decomposition (BD) [Cordeau et. al.，2019];

Directory `TESTDATA/` contains the related data sets in the paper.

## How to complile PRECPX

In order to compile the excutable files, CPLEX libraries and header files are required. 
Therefore you use the command:
```
cd PRECPX
make CPLEXINCDIR=<cplex/include/ilcplex/>  CPLEXLIBDIR=<cplex/lib/x86-64_linux/static_pic/> 
- "<cplex/include/ilcplex/>": directory of CPLEX headers
- "<cplex/lib/x86-64_linux/static_pic/>": directory of CPLEX libraries

Then the following executable files will be generated.

- `bin/MCLPCPX`   (solve the MCLP using CPX)
- `bin/PSCLPCPX`  (solve the PSCLP using CPX)

See the parameters of executable files in "How to run the solver" to get information.

## How to complile PREBD
In order to build PREBD based on BD, please download their code in directory `PREBD` by 
```
cd PREBD
git clone https://github.com/fabiofurini/LocationCovering
```
Excute `patch -p0 < PREBD.patch`, and then, simlar to compling PRECPX, you compile using the command
```
make CPLEXINCDIR=<cplex/include/ilcplex/>  CPLEXLIBDIR=<cplex/lib/x86-64_linux/static_pic/> 
```

The following executable files will be generated.

- `bin/MCLPBD`     (solve the MCLP using BD)
- `bin/PSCLPBD`    (solve the PSCLP using BD)

See the parameters of executable files in "How to run the solver" to get information of implementing presolving.

## Information for the data sets
In directory `TESTDATA/`, we provide some facility information. In each file, 

- row 1 gives the number of points; 

- column 1 gives the index of points; 

- columns 2 and 3 give coordinates (x,y) of points;

- column 4 gives the budget of the facility; (ignored when this file is used as customer file)

- column 5 gives the demand of the customer; (ignored when this file is used as facility file)

## How to run the solver
For a given executable file [EXE] (`MCLPCPX`, `PSCLPCPX`, `MCLPBD`, or `PSCLPBD`), the solver can be run
in the corresponding main directory as

  ```
  ./bin/[EXE] [PS] [IF] [IC] [TL] [R] [IP] [P] ([RS])
  ```

where the arguments are listed in the following:

`[PS]` presolving settings for `MCLPCPX` or `PSCLPCPX`; 
- 0: `CPX`;             (CPLEX solver without our proposed presolving methods) 
- 1: `PRE+CPX`;         (CPX with our five proposed presolving methods)
- 2: `No SIN_AGG`;      (PRE+CPX with the singleton aggregation disabled)
- 3: `No ISO_AGG`;      (PRE+CPX with the isomorphic aggregation disabled)
- 4: `No NON_CAN`;      (PRE+CPX with the nonzero cancellation disabled)
- 5: `No DOM`;          (PRE+CPX with the domination disabled)
- 6: `No NON_FIX`;      (PRE+CPX with the nonoverlap fixing disabled)

  for `MCLPBD` or `PSCLPBD`
- 0: `BD`;              (Benders decomposition [Cordeau et. al.，2019])
- 1: `PRE+BD`;          (BD with the singleton and isomorphic aggregations and domination)

`[FF]` facility file; 
`[FC]` customer file (as the numbers of customers are extremely large in some testsets in the paper, 
      the parameter can be the number of customers and then the customer data will be generated randomly);
`[TL]` time limitation in seconds;
`[R]`  covering radius;
`[IP]` is the parameter [P] percentage of total budget for the MCLP or total covering demand for the PSCLP?;
`[P] ` budget for the MCLP or covering demand for the PSCLP (real value if [IP]=0; percentage if [IP]=1);
`[RS]` `CPXPARAM_RandomSeed` for `CPX` (optional).

The solver can be run as, e.g. 
 ```
 ./bin/MCLPCPX 1 ../TESTDATA/n100s2.dat 100000 10000 5.75 1 0.1
 ```

