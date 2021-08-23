## Introduction
The README contains code and complementary material to the paper

L. Chen, S.-J. Chen, W.-K. Chen, Y.-H. Dai, and J. Juan, (2021). 
Efficient presolving methods for solving maximal covering and partial set covering location problems

To solve the maximal covering location problem (MCLP) and partial set covering location problem (PSCLP), the branch-and-cut 
algorithm is used in the paper combined with five proposed presolving methods, i.e., 
i)   singleton aggregation (SA), 
ii)  isomorphic aggregation (IA),
iii) nonzero cancellation (NC),
iv)  domination (Dom), and
v)   nonoverlap fixing (NF), 
for both the MCLP and PSCLP.

The directory "PRECPX/" includes all source files and header files for the code; the directory "data/" contains the related
data sets in the paper; the directory "statistics/" gives more detailed numerical material as a complement. 
Note that, to compile and run the code, the CPLEX libraries and header files are required, which given in directories 
"lib/" and "include/".

## Executable files
We have written the "Makefile" in the main directory, and one just needs to run "make" to compile and generate the executable
file "IMPSolving" used in the paper in a new created directory "bin/". 

In addition, in order to analyze the presolving reductions more efficiently (see Tables 2, 3, 5, 6, 8, and 9 in the paper),
we also generate another executable file "OnlyPresolve_IMPSolving", where only the presolving parts are implemented (without
the solving procedures).

## Data sets
Directory "data/" contains all data files used in our paper, including eight social networks files "MSG.txt", "GNU.txt", "HEP.txt",
"ENRON.txt", "FACEBOOK.txt", "DEEZER.txt", "TWITTER.txt", "EPINIONS.txt". The download links of these networkss are

MSG: "http://snap.stanford.edu/data/CollegeMsg.txt.gz";
GNU: "http://snap.stanford.edu/data/p2p-Gnutella04.txt.gz";
FACEBOOK: "https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/weic-graphdata.zip";
ENRON: "http://snap.stanford.edu/data/email-Enron.txt.gz";
FACEBOOK: "http://snap.stanford.edu/data/gemsec_facebook_dataset.tar.gz";
DEEZER: "http://snap.stanford.edu/data/gemsec_deezer_dataset.tar.gz";
TWITTER: "http://snap.stanford.edu/data/twitter_combined.txt.gz";
EPINIONS: "http://snap.stanford.edu/data/soc-sign-epinions.txt.gz".

Notice that GNU, HEP, FACEBOOK, and DEEZER  are undirected networks while the others are directed networks. Here, by removing extra
information, each of the above eight text files in "data/" only consists of two columns, where the first column entries represent 
the indices of source nodes of arcs, and the second column enries represent the corresponding indices of the target nodes. 

We have written a GNU awk file "TransFormat.awk" to first transform the network files. In the transformed file, each repeated arcs
appears only once associated with the number of the corresponding parallel arcs, and if the considered network is undirected, 
the transformed file contains two opposite directed arcs for each undirected edge. Below we present two example for this.

(i) Considering the directed network file "MSG.txt", by running
  
  awk -f TransFormat.awk -vIsDirected=1 MSG.txt > MSG.dat,
  
we can get the transformed network file "MSG.dat", where the first line in the file gives the number of nodes and arcs of the network
(any parallel arc only counts once), and each of the remaining lines shows the index of source node, the index of target node and the 
number of associated parallel arc, respectively. 

(ii) For the undirected network file "FACEBOOK.txt" by running

  awk -f TransFormat.awk -vIsDirected=0 FACEBOOK.txt > FACEBOOK.dat,

we can get the transformed network file "FACEBOOK.dat", where the first line in the file still gives the number of nodes and
arcs in the network (any parallel arc only counts once), and the other lines show the indices of source nodes, the indices of target 
nodes and the parallel number of the corresponding arcs, respectively. The difference from the above is that, for each edge in 
file "FACEBOOK.txt", both the edge and its opposite edge are contained in the data file "FACEBOOK.dat".

Note that, in the transformed file, we renumbered the nodes indices so that the nodes indices range from 1 to |V|. To transform
and generate all the test data sets in the paper, please simply run the script "./TransFormat.sh" in the directory "data/".


MCLP_None 
MCLP_All 
MCLP_NoSA 
MCLP_NoIA 
MCLP_NoNC 
MCLP_NoDom 
MCLP_NoNF 

PSCLP_None 
PSCLP_All 
PSCLP_NoSA 
PSCLP_NoIA
PSCLP_NoNC 
PSCLP_NoDom
PSCLP_NoNF 

## How to run the solver
For a given executable file [EXE], the solver can be run in the main directory as

  ./bin/[EXE] data/[NF] [PM] ([P]) [SS] [TL] [K] [NS] [MRS] [RS],

where [EXE] is executable file (IMPSolving or OnlyPresolve_IMPSolving) and the arguments are: 
[NF] transformed network data file; 
[PM] propagation model (1 for the ICM, 2 for the LTM); 
[P] activation probability (only used as an argument when [PM] = 1, i.e., under propagation model ICM. Otherwise, we omit this parameter);
[SS] Bender decomposition with sepecfic presolving methods (1  with only the SNA, 2 with the SNA and SCNA, 3 with the SNA, SCNA and INA);
[TL] time limitation in seconds; 
[K] number of initial active nodes;
[NS] number of sampling scenarios; 
[MRS] parameter "MaxReacSize" (see the paper for more details); 
[RS] the random seed of sampling.

For instance, considering the network MSG under the ICM with the activation probability being 0.1 for each arc, if [TL]=14400s,
[K]=20, [NS]=100, [MRS]=8, and [RS]=10, then to solve the corresponding IMP with presolving SNA and SCNA, one can run 

  ./bin/IMPSolving data/MSG.dat 1 0.1 2 14400 20 100 8 10.

Similarly, considering the network HEP under the LTM with the activation weight being n_ij/deg(G,j) for each arc (i,j)
(described in our paper), if [TL]=100s, [K]=5, [NS]=500, [MRS]=4, and [RS]=20, then to solve the corresponding IMP with
presolving SNA, SCNA and INA, one can run 

  ./bin/IMPSolving data/HEP.dat 2 3 100 5 500 4 20.
  

