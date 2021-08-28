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

C_EX = MCLP PSCLP

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
MCLP: main.cpp global_functions.o model.o presolve.o
	echo "$(CCC) $(CFLAGS) $(CLNDIRS) -o $@ $^ $(CLNFLAGS)"
	$(CCC) $(CFLAGS) $(CLNDIRS) -o $@ $^ $(CLNFLAGS)

PSCLP: main.cpp global_functions.o model.o presolve.o
	$(CCC) $(CFLAGS) $(CLNDIRS) -DPSCLP=1 -o $@ $^ $(CLNFLAGS)

dep_file = .$@.d

main.o: main.cpp
	$(CCC) $(CFLAGS) -Wp,-MD,$(dep_file) -c $<

%.o : %.cpp %.h
	$(CCC) $(CFLAGS) -Wp,-MD,$(dep_file) -c $<
