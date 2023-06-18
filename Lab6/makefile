CC=mpiCC
CCRUN=mpirun
RUNFLAGS=-np 2
FLAGS=-std=c++20
BOOST_LIBS=-lboost_mpi -lboost_serialization

BINARY=bin
CODEDIRS=.
CPPFILES=$(foreach D,$(CODEDIRS),$(wildcard $(D)/*.cpp))

all:$(BINARY)

$(BINARY):$(CPPFILES)
	$(CC) $(FLAGS) $(CPPFILES) $(BOOST_LIBS) -o $@

run:
	$(CCRUN) $(RUNFLAGS) $(BINARY)

clean:
	rm -rf $(BINARY) $(OBJECTS)