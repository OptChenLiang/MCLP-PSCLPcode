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

CONCERTINCDIR = include
CPLEXINCDIR   = include

EXINC         = include
EXSRCC        = ./

CFLAGS  = $(COPT) -I$(CPLEXINCDIR) -O2


#------------------------------------------------------------
#  make all      : to compile the examples. 
#  make execute  : to compile and execute the examples.
#------------------------------------------------------------

C_EX = MCLPBD PSCLPBD 

all_c: $(C_EX)

execute_c: $(C_EX) 
	 ./main
# ------------------------------------------------------------

clean :
	/bin/rm -rf src_PSCLP/*.o src_MCLP/*.o *.o *~ *.class
	/bin/rm -rf bin/*
	/bin/rm -rf *.mps *.ord *.sos *.lp *.SAv *.net *.msg *.log *.clp

# ------------------------------------------------------------
#
MCLPBD: src_MCLP/Main.cpp src_MCLP/global_functions.o src_MCLP/MCLP_BEN.o src_MCLP/presolve.o src_MCLP/read.o src_MCLP/MCLP.o
	mkdir -p bin
	$(CCC) $(CFLAGS) $(CLNDIRS) -o bin/$@ $^ $(CLNFLAGS)

PSCLPBD: src_PSCLP/Main.cpp src_PSCLP/global_functions.o src_PSCLP/PSCLP_BEN.o src_PSCLP/presolve.o src_PSCLP/read.o src_PSCLP/PSCLP.o
	mkdir -p bin
	$(CCC) $(CFLAGS) $(CLNDIRS) -o bin/$@ $^ $(CLNFLAGS)

dep_file = ./$@

%.o : %.cpp %.h
	$(CCC) $(CFLAGS) -o $(dep_file) -c $<
