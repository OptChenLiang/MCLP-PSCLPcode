CCC = g++ -O0

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -std=c++11
COPT  = -m64 -fPIC -fno-strict-aliasing -std=c++11

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXLIBDIR   = ../lib/

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)
CLNDIRS   = -L$(CPLEXLIBDIR)
CCLNFLAGS = -lconcert -lilocplex -lcplex -lm -lpthread -ldl
CLNFLAGS  = -lcplex -lm -lpthread -ldl

all:
	make all_c

CONCERTINCDIR = ../include
CPLEXINCDIR   = ../include

EXINC         = ../include
EXSRCC        = ./

CFLAGS  = $(COPT) -I$(CPLEXINCDIR) -O2


#------------------------------------------------------------
#  make all      : to compile the examples. 
#  make execute  : to compile and execute the examples.
#------------------------------------------------------------
dirMCLP=LocationCovering/src_MCLP
dirPSCLP=LocationCovering/src_PSCLP

C_EX = MCLPBD PSCLPBD 

all_c: $(C_EX)

execute_c: $(C_EX) 
	 ./main
# ------------------------------------------------------------

clean :
	/bin/rm -rf $(dirPSCLP)/*.o $(dirMCLP)/*.o *.o *~ *.class
	/bin/rm -rf bin/*
	/bin/rm -rf *.mps *.ord *.sos *.lp *.SAv *.net *.msg *.log *.clp

# ------------------------------------------------------------
#
MCLPBD: $(dirMCLP)/Main.cpp $(dirMCLP)/global_functions.o $(dirMCLP)/MCLP_BEN.o $(dirMCLP)/presolve.o $(dirMCLP)/read.o $(dirMCLP)/MCLP.o
	mkdir -p bin
	$(CCC) $(CFLAGS) $(CLNDIRS) -o bin/$@ $^ $(CLNFLAGS)

PSCLPBD: $(dirPSCLP)/Main.cpp $(dirPSCLP)/global_functions.o $(dirPSCLP)/PSCLP_BEN.o $(dirPSCLP)/presolve.o $(dirPSCLP)/read.o $(dirPSCLP)/PSCLP.o
	mkdir -p bin
	$(CCC) $(CFLAGS) $(CLNDIRS) -o bin/$@ $^ $(CLNFLAGS)

dep_file = ./$@

%.o : %.cpp %.h
	$(CCC) $(CFLAGS) -o $(dep_file) -c $<
