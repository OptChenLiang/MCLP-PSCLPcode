CCC = g++ -O0

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -std=c++11
COPT  = -m64 -fPIC -fno-strict-aliasing -std=c++11

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXLIBDIR   = lib/

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)
CLNDIRS   = -L$(CPLEXLIBDIR)
CCLNFLAGS = -lconcert -lilocplex -lcplex -lm -lpthread -ldl
CLNFLAGS  = -lcplex -lm -lpthread -ldl

all:
	make all_c

execute: all
	make execute_c

CONCERTINCDIR = include
CPLEXINCDIR   = include

EXINC         = include
EXSRCC        = ./

CFLAGS  = $(COPT) -I$(CPLEXINCDIR) -O2


#------------------------------------------------------------
#  make all      : to compile the examples. 
#  make execute  : to compile and execute the examples.
#------------------------------------------------------------

C_EX = MCLP_All PSCLP_All MCLP_None PSCLP_None MCLP_NoNC PSCLP_NoNC MCLP_NoNF PSCLP_NoNF MCLP_NoDom PSCLP_NoDom MCLP_NoSA PSCLP_NoSA MCLP_NoIA PSCLP_NoIA 

all_c: $(C_EX)

execute_c: $(C_EX) 
	 ./main
# ------------------------------------------------------------

clean :
	/bin/rm -rf *.o *~ *.class
	/bin/rm -rf $(C_EX) $(CX_EX)
	/bin/rm -rf *.mps *.ord *.sos *.lp *.SAv *.net *.msg *.log *.clp

# ------------------------------------------------------------
#
# The examples
#
MCLP_PRE_BRANCH = -DBRANCH=1 -DCUT=1 -DBIN=1 -DSA=1 -DIA=1 -DBEN1=0 -DNC=1 -DDom=1
PSCLP_PRE_BRANCH = -DBRANCH=1 -DCUT=1 -DBIN=1 -DSA=1 -DIA=1 -DBEN1=0 -DNC=1 -DDom=1 -DPSCLP=1
MCLP_PRE = -DBRANCH=1 -DBIN=1 -DSA=1 -DIA=1 -DBEN1=0 -DNC=1 -DDom=1
PSCLP_PRE = -DBRANCH=1 -DBIN=1 -DSA=1 -DIA=1 -DBEN1=0 -DNC=1 -DDom=1 -DPSCLP=1
MCLP_NODom = -DBRANCH=1 -DCUT=1 -DBIN=1 -DSA=1 -DIA=1 -DBEN1=0 -DNC=1 -DDom=0 
PSCLP_NODom = -DBRANCH=1 -DCUT=1 -DBIN=1 -DSA=1 -DIA=1 -DBEN1=0 -DNC=1 -DDom=0 -DPSCLP=1
MCLP_NONC = -DBRANCH=1 -DCUT=1 -DBIN=1 -DSA=1 -DIA=1 -DBEN1=0 -DDom=1
PSCLP_NONC = -DBRANCH=1 -DCUT=1 -DBIN=1 -DSA=1 -DIA=1 -DBEN1=0 -DDom=1 -DPSCLP=1
MCLP_NOPRE = -DBRANCH=1 -DBIN=1
PSCLP_NOPRE = -DBRANCH=1 -DBIN=1 -DPSCLP=1
MCLP_NOSA = -DBRANCH=1 -DCUT=1 -DBIN=1 -DIA=1 -DBEN1=0 -DNC=1 -DDom=1
PSCLP_NOSA = -DBRANCH=1 -DCUT=1 -DBIN=1 -DIA=1 -DBEN1=0 -DNC=1 -DDom=1 -DPSCLP=1
MCLP_NOIA = -DBRANCH=1 -DCUT=1 -DBIN=1 -DSA=1 -DBEN1=0 -DNC=1 -DDom=1
PSCLP_NOIA = -DBRANCH=1 -DCUT=1 -DBIN=1 -DSA=1 -DBEN1=0 -DNC=1 -DDom=1 -DPSCLP=1

MCLP_None: main.cpp global_functions.o model.o presolve.o
	echo "$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NOPRE) -o $@ $^ $(CLNFLAGS)"
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NOPRE) -o $@ $^ $(CLNFLAGS)

PSCLP_None: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_NOPRE) -o $@ $^ $(CLNFLAGS)

MCLP_All: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_PRE_BRANCH) -o $@ $^ $(CLNFLAGS)

PSCLP_All: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_PRE_BRANCH) -o $@ $^ $(CLNFLAGS)

MCLP_NoNF: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_PRE) -o $@ $^ $(CLNFLAGS)

PSCLP_NoNF: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_PRE) -o $@ $^ $(CLNFLAGS)

MCLP_NoNC: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NONC) -o $@ $^ $(CLNFLAGS)

PSCLP_NoNC: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_NONC) -o $@ $^ $(CLNFLAGS)

MCLP_NoDom: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NODom) -o $@ $^ $(CLNFLAGS)

PSCLP_NoDom: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_NODom) -o $@ $^ $(CLNFLAGS)

MCLP_NoSA: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NOSA) -o $@ $^ $(CLNFLAGS)

PSCLP_NoSA: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_NOSA) -o $@ $^ $(CLNFLAGS)

MCLP_NoIA: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NOIA) -o $@ $^ $(CLNFLAGS)

PSCLP_NoIA: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_NOIA) -o $@ $^ $(CLNFLAGS)
	
dep_file = .$@.d

main.o: main.cpp
	$(CCC) $(CFLAGS) -Wp,-MD,$(dep_file) -c $<

%.o : %.cpp %.h
	$(CCC) $(CFLAGS) -Wp,-MD,$(dep_file) -c $<
