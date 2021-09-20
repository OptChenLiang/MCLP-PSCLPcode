## Introduction
The README contains code and complementary material to the paper

L. Chen, S.-J. Chen, W.-K. Chen, Y.-H. Dai, and J. Juan, (2021). 
Efficient presolving methods for solving maximal covering and partial set covering location problems

To solve the maximal covering location problem (MCLP) and partial set covering location problem (PSCLP), the branch-and-cut 
algorithm is used in the paper combined with five proposed presolving methods, i.e., 

1. singleton aggregation (`SIN_AGG`), 
2. isomorphic aggregation (`ISO_AGG`),
3. nonzero cancellation (`NON_CAN`),
4. domination (`DOM`), and
5. nonoverlap fixing (`NON_FIX`), 


The directory `PRECPX/` includes all source files and header files for solving the MCLP and PSCLP using CPLEX (`CPX`);
the directory `PREBD/` includes all source files and header files for solving the MCLP and PSCLP using the Benders decomposition (`BD`);
the directory `TESTDATA/` contains the related data sets in the paper; . 
Note that, to compile and run the code, the CPLEX libraries and header files are required, which given in directories  `lib/` and `include/`.

## Executable files
We have written the `Makefile` in the main directory, and one just needs to run 

```
make
```

 in directory `PRECPX/` or `PREBD/` to compile and generate the executable files in a new created directory `bin/` in the corresponding directory. 

- `PRECPX/bin/MCLPCPX`   to solve the MCLP using CPX
- `PRECPX/bin/PSCLPCPX`  to solve the PSCLP using CPX
- `PREBD/bin/MCLPBD`     to solve the MCLP using BD
- `PREBD/bin/PSCLPBD`    to solve the PSCLP using BD

See the parameters of executable files in "How to run the solver" to get information of implementing presolving.

## Data sets
In directory `TESTDATA/`, we provide some facility information.
Directory `f_data_100/` contains coordinates of 100 facilities and directory `f_data_2500/` contains coordinates of 2500 facilities, which are all randomly generated.

In each file, 

- column 1 "F" means "facility"; 

- column 2 gives the index of facility; 

- columns 3 and 4 give coordinates (x,y) of facility;

- column 5 gives the demand of customer when considering test set T2 where the coordinates of facilities and customers are chosen to be identical

   (the costs of all facilities are 1 in the paper).

## How to run the solver
For a given executable file [EXE] (`MCLPCPX`, `PSCLPCPX`, `MCLPBD`, or `PSCLPBD`), the solver can be run in the corresponding main directory as

  ```
  ./bin/[EXE] [PS] [FF] [FC] [NF] [NC] [TL] [R] [P] ([RS])
  ```

where the arguments are listed in the following:

`[PS]` presolving settings for `MCLPCPX` or `PSCLPCPX`; 
- 0: `CPX`; 
- 1: `PRE+CPX`;
- 2: `No SIN_AGG`;
- 3: `No ISO_AGG`;
- 4: `No NON_CAN`;
- 5: `No DOM`;
- 6: `No NON_FIX`;

  for `PREBD`
- 0: `BD`;
- 1: `PRE+BD`;

`[FF]` facilities file; 
`[FC]` customers file (as the numbers of customers are extremely large in some testsets in the paper, the parameter can be NULL and then generated randomly when running the code);
`[NF]` number of facilities;
`[NC]` number of customers;
`[TL]` time limitation in seconds;
`[R]`  covering radius;
`[P] ` budget parameter for the MCLP or covering demand parameter in the PSCLP
     (a value less than 1 means a percentage);
`[RS]` `CPXPARAM_RandomSeed` for `CPX` (optional).

one can execute scripts e.g. `./Subs-MCLP-T1.sh` to see all the command in the paper.


### Example

1. For instance in testset T1, consider the MCLP with 100 facilities and 100000 customers. The dataset is given by `../TESTDATA/f_data_100/n100_f10_100_s2.dat` with covering radius 5.75 and budget 10. For using CPLEX with our presolving methods within 10000 seconds, one can run 

 ```
 ./bin/MCLPCPX 1 ../TESTDATA/f_data_100/n100_f10_100_s2.dat NULL 100 100000 10000 5.75 0.1
 ```

2. For instance in testset T2, consider the MCLP with 1700 facilities and 1700 customers (same coordinates). The dataset is given by `../TESTDATA/f_data_2500/n2500s2.dat` with covering radius 4 and budget 20. For using CPLEX with our presolving methods except the domination within 10000 seconds, one can run 

#SC@LC: why the name of dateset is n2500s2.dat in this example?

 ```
./bin/MCLPCPX 5 ../TESTDATA/f_data_2500/n2500s2.dat ../TESTDATA/f_data_2500/n2500s2.dat 1700 1700 10000 4 20
 ```

3. For instance in testset T3, consider the PSCLP with 1500 facilities and 10000000 customers. The dataset is given by `../TESTDATA/n2500s5.dat` with covering radius 1.0328 and covering demand 70% of total demand. For using Benders Decomposition with our presolving methods within 10000 seconds, one can run 

  ```
  ./bin/BD 1 ../TESTDATA/f_data_2500/n2500s5.dat NULL 1500 10000000 10000 1.0328 0.7
  ```

