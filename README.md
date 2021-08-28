## Introduction
The README contains code and complementary material to the paper

L. Chen, S.-J. Chen, W.-K. Chen, Y.-H. Dai, and J. Juan, (2021). 
Efficient presolving methods for solving maximal covering and partial set covering location problems

To solve the maximal covering location problem (MCLP) and partial set covering location problem (PSCLP), the branch-and-cut 
algorithm is used in the paper combined with five proposed presolving methods, i.e., 

1. singleton aggregation (`SIN_AGG` ), 
2. isomorphic aggregation (`ISO_AGG`),
3. nonzero cancellation (`NON_CAN`),
4. domination (`DOM`), and
5. nonoverlap fixing (`NON_FIX`), 

for both the MCLP and PSCLP.

The directory `PRECPX/` includes all source files and header files for solving the MCLP and PSCLP using CPLEX (`CPX`);
the directory `PREBD` includes all source files and header files for solving the MCLP and PSCLP using the Benders Decomposition (`BD`);
the directory `TESTDATA/` contains the related data sets in the paper; . 
Note that, to compile and run the code, the CPLEX libraries and header files are required, which given in directories  `lib/` and `include/`.

## Executable files
We have written the `Makefile` in the main directory, and one just needs to run 

```
make
```

 in directory `PRECPX/` or `PREBD/` to compile and generate the executable files in a new created directory `bin/` in the corresponding directory. 

- `PRECPX/bin/MCLPCPX`   to solve the MCLP using CPLEX
- `PRECPX/bin/PSCLPCPX`  to solve the PSCLP using CPLEX
- `PREBD/bin/MCLPBD`     to solve the MCLP using Benders Decomposition
- `PREBD/bin/PSCLPBD`    to solve the PSCLP using Benders Decomposition

As for whether to use presolving methods, see the parameters of executable files in "How to run the solver".

## Data sets
In directory `TESTDATA/`, we provide some facility information.
Directory `f_data_100/` contains coordinates of 100 facilities and directory `f_data_2500/` contains coordinates of 2500 facilities.

In each file, 

- column 1 "F" means facility; 

- column 2 is the index of facility; 

- columns 3 and 4 is the coordinates (x,y);

- column 5 is the demand if this facility is regarded as a customer

   (the costs of all facilities are 1 in the paper).

## How to run the solver
For a given executable file [EXE], the solver can be run in the main directory as

  ```
  ./bin/[EXE] [PS] [FF] [FC] [NF] [NC] [TL] [R] [P] ([RS])
  ```

where `[EXE]` is executable file (`MCLPCPX`, `PSCLPCPX`, `MCLPBD`or `PSCLPBD`) and the arguments are: 
`[PS]` for `MCLPCPX` or `PSCLPCPX` 

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
`[FC]` customers file (can be NULL and then generate customers randomly when running the code);
`[NF]` number of facilities;
`[NC]` number of customers
`[TL]` time limitation in seconds;
`[R]`  covering radius;
`[P] ` budget in the MCLP or covering demand in the PSCLP,
     <= 1 means percentage;
`[RS]` `CPXPARAM_RandomSeed` for `CPX` (optional).

one can execute scripts e.g. `./Subs-MCLP-T1.sh` to see the command in the paper.

### Example

1. For instance in testset T1, considering the MCLP with 100 facilities and 100000 customers for the facility dataset `../TESTDATA/f_data_100/n100_f10_100_s2.dat` with covering radius 5.75 and budget 10, if one test it by using CPLEX with our presolving methods within 10000 seconds, one can run 

 ```
 ./bin/MCLPCPX 1 ../TESTDATA/f_data_100/n100_f10_100_s2.dat NULL 100 100000 10000 5.75 0.1
 ```

2. For instance in testset T2, considering the MCLP with 1700 facilities and 1700 customers (same coordinates) for the facility and customer dataset `../TESTDATA/f_data_2500/n2500s2.dat` with covering radius 4and budget 20, if one test it by using CPLEX with our presolving methods except the domination within 10000 seconds, one can run 

 ```
./bin/MCLPCPX 5 ../TESTDATA/f_data_2500/n2500s2.dat ../TESTDATA/f_data_2500/n2500s2.dat 1700 1700 10000 4 20
 ```

3. For instance in testset T3, considering the PSCLP with 1500 facilities and 10000000 customers for the facility dataset `../TESTDATA/n2500s5.dat` with covering radius 1.0328 and covering demand 70% of total demand, if one test it by using Benders Decomposition with our presolving methods within 10000 seconds, one can run 

  ```
  ./bin/BD 1 ../TESTDATA/f_data_2500/n2500s5.dat NULL 1500 10000000 10000 1.0328 0.7
  ```

