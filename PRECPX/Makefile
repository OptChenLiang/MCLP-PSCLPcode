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


CONCERTINCDIR = ../include
CPLEXINCDIR   = ../include

EXINC         = ../include
EXSRCC        = ./

CFLAGS  = $(COPT) -I$(CPLEXINCDIR) -O2

C_EX = MCLPCPX PSCLPCPX

all_c: $(C_EX)

clean :
	/bin/rm -rf *.o src/*.o *.o.d src/*.o.d *~ *.class
	/bin/rm -rf bin/*
	/bin/rm -rf *.mps *.ord *.sos *.lp *.SAv *.net *.msg *.log *.clp

MCLPCPX: src/main.cpp src/global_functions.o src/model.o src/presolve.o
	mkdir -p bin
	$(CCC) $(CFLAGS) $(CLNDIRS) -o bin/$@ $^ $(CLNFLAGS)

PSCLPCPX: src/main.cpp src/global_functions.o src/model.o src/presolve.o
	mkdir -p bin
	$(CCC) $(CFLAGS) $(CLNDIRS) -DPSCLP=1 -o bin/$@ $^ $(CLNFLAGS)

dep_file = ./$@

#$(CCC) $(CFLAGS) -Wp,-MD,$(dep_file) -c $<

%.o : %.cpp %.h
	$(CCC) $(CFLAGS) -o $(dep_file) -c $<
