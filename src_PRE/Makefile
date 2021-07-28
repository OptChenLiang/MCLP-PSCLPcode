CPLEXDIR      = /home/cl/cplex1271/cplex/
CCC = g++ -O0
CC  = gcc -O0


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
JAVA      = java  -d64 -Djava.library.path=$(CPLEXDIR)/bin/x86-64_linux -classpath $(CPLEXJARDIR):


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

C_EX = mclp_pre_branch psclp_pre_branch mclp_nopre psclp_nopre mclp_nodnc psclp_nodnc mclp_pre psclp_pre mclp_nodc psclp_nodc mclp_noda psclp_noda mclp_nodpa psclp_nodpa 
CX_EX = mclp_c_pre_branch psclp_c_pre_branch mclp_c_nopre psclp_c_nopre mclp_c_nodnc psclp_c_nodnc mclp_c_pre psclp_c_pre mclp_c_nodc psclp_c_nodc mclp_c_noda psclp_c_noda mclp_c_nodpa psclp_c_nodpa


all_c: $(C_EX) $(CX_EX)

execute_c: $(C_EX) $(CX_EX)
	 ./main
# ------------------------------------------------------------

clean :
	/bin/rm -rf *.o *~ *.class
	/bin/rm -rf $(C_EX) $(CX_EX)
	/bin/rm -rf *.mps *.ord *.sos *.lp *.sav *.net *.msg *.log *.clp

# ------------------------------------------------------------
#
# The examples
#
MCLP_PRE_BRANCH = -DBRANCH=1 -DCUT=1 -DBIN=1 -DDA=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=1
PSCLP_PRE_BRANCH = -DBRANCH=1 -DCUT=1 -DBIN=1 -DDA=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=1 -DPSCLP=1
MCLP_PRE = -DBRANCH=1 -DBIN=1 -DDA=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=1
PSCLP_PRE = -DBRANCH=1 -DBIN=1 -DDA=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=1 -DPSCLP=1
MCLP_NODC = -DBRANCH=1 -DCUT=1 -DBIN=1 -DDA=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=0 
PSCLP_NODC = -DBRANCH=1 -DCUT=1 -DBIN=1 -DDA=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=0 -DPSCLP=1
MCLP_NODNC = -DBRANCH=1 -DCUT=1 -DBIN=1 -DDA=1 -DDPA=1 -DBEN1=0 -DDC=1
PSCLP_NODNC = -DBRANCH=1 -DCUT=1 -DBIN=1 -DDA=1 -DDPA=1 -DBEN1=0 -DDC=1 -DPSCLP=1
MCLP_NOPRE = -DBRANCH=1 -DBIN=1
PSCLP_NOPRE = -DBRANCH=1 -DBIN=1 -DPSCLP=1
MCLP_NODA = -DBRANCH=1 -DCUT=1 -DBIN=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=1
PSCLP_NODA = -DBRANCH=1 -DCUT=1 -DBIN=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=1 -DPSCLP=1
MCLP_NODPA = -DBRANCH=1 -DCUT=1 -DBIN=1 -DDA=1 -DBEN1=0 -DDNC=1 -DDC=1
PSCLP_NODPA = -DBRANCH=1 -DCUT=1 -DBIN=1 -DDA=1 -DBEN1=0 -DDNC=1 -DDC=1 -DPSCLP=1

MCLP_C_PRE_BRANCH = -DBRANCH=1 -DCUT=1 -DBIN=0 -DDA=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=1
PSCLP_C_PRE_BRANCH = -DBRANCH=1 -DCUT=1 -DBIN=0 -DDA=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=1 -DPSCLP=1
MCLP_C_PRE = -DBRANCH=1 -DBIN=0 -DDA=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=1
PSCLP_C_PRE = -DBRANCH=1 -DBIN=0 -DDA=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=1 -DPSCLP=1
MCLP_C_NODC = -DBRANCH=1 -DCUT=1 -DBIN=0 -DDA=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=0 
PSCLP_C_NODC = -DBRANCH=1 -DCUT=1 -DBIN=0 -DDA=1 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=0 -DPSCLP=1
MCLP_C_NODNC = -DBRANCH=1 -DCUT=1 -DBIN=0 -DDA=1 -DDPA=1 -DBEN1=0 -DDC=1
PSCLP_C_NODNC = -DBRANCH=1 -DCUT=1 -DBIN=0 -DDA=1 -DDPA=1 -DBEN1=0 -DDC=1 -DPSCLP=1
MCLP_C_NOPRE = -DBRANCH=1 -DBIN=0
PSCLP_C_NOPRE = -DBRANCH=1 -DBIN=0 -DPSCLP=1
MCLP_C_NODA = -DBRANCH=1 -DCUT=1 -DBIN=0 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=1
PSCLP_C_NODA = -DBRANCH=1 -DCUT=1 -DBIN=0 -DDPA=1 -DBEN1=0 -DDNC=1 -DDC=1 -DPSCLP=1
MCLP_C_NODPA = -DBRANCH=1 -DCUT=1 -DBIN=0 -DDA=1 -DBEN1=0 -DDNC=1 -DDC=1
PSCLP_C_NODPA = -DBRANCH=1 -DCUT=1 -DBIN=0 -DDA=1 -DBEN1=0 -DDNC=1 -DDC=1 -DPSCLP=1

mclp_nopre: main.cpp global_functions.o model.o presolve.o
	echo "$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NOPRE) -o $@ $^ $(CLNFLAGS)"
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NOPRE) -o $@ $^ $(CLNFLAGS)

psclp_nopre: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_NOPRE) -o $@ $^ $(CLNFLAGS)

mclp_pre_branch: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_PRE_BRANCH) -o $@ $^ $(CLNFLAGS)

psclp_pre_branch: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_PRE_BRANCH) -o $@ $^ $(CLNFLAGS)

mclp_pre: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_PRE) -o $@ $^ $(CLNFLAGS)

psclp_pre: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_PRE) -o $@ $^ $(CLNFLAGS)

mclp_nodnc: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NODNC) -o $@ $^ $(CLNFLAGS)

psclp_nodnc: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_NODNC) -o $@ $^ $(CLNFLAGS)

mclp_nodc: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NODC) -o $@ $^ $(CLNFLAGS)

psclp_nodc: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_NODC) -o $@ $^ $(CLNFLAGS)

mclp_noda: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NODA) -o $@ $^ $(CLNFLAGS)

psclp_noda: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_NODA) -o $@ $^ $(CLNFLAGS)

mclp_nodpa: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NODPA) -o $@ $^ $(CLNFLAGS)

psclp_nodpa: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_NODPA) -o $@ $^ $(CLNFLAGS)
	

mclp_c_nopre: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_NOPRE) -o $@ $^ $(CLNFLAGS)

psclp_c_nopre: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_NOPRE) -o $@ $^ $(CLNFLAGS)

mclp_c_pre_branch: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_C_PRE_BRANCH) -o $@ $^ $(CLNFLAGS)

psclp_c_pre_branch: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_C_PRE_BRANCH) -o $@ $^ $(CLNFLAGS)

mclp_c_pre: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_C_PRE) -o $@ $^ $(CLNFLAGS)

psclp_c_pre: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_C_PRE) -o $@ $^ $(CLNFLAGS)

mclp_c_nodnc: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_C_NODNC) -o $@ $^ $(CLNFLAGS)

psclp_c_nodnc: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_C_NODNC) -o $@ $^ $(CLNFLAGS)

mclp_c_nodc: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_C_NODC) -o $@ $^ $(CLNFLAGS)

psclp_c_nodc: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_C_NODC) -o $@ $^ $(CLNFLAGS)

mclp_c_noda: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_C_NODA) -o $@ $^ $(CLNFLAGS)

psclp_c_noda: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_C_NODA) -o $@ $^ $(CLNFLAGS)

mclp_c_nodpa: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(MCLP_C_NODPA) -o $@ $^ $(CLNFLAGS)

psclp_c_nodpa: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) $(PSCLP_C_NODPA) -o $@ $^ $(CLNFLAGS)
	
dep_file = .$@.d

main.o: main.cpp
	$(CCC) $(CFLAGS) -Wp,-MD,$(dep_file) -c $<

%.o : %.cpp %.h
	$(CCC) $(CFLAGS) -Wp,-MD,$(dep_file) -c $<

# Local Variables:
# mode: makefile
# End:
