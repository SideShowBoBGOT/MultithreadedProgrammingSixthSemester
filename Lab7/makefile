CC=mpiCC
CCRUN=mpirun
RUNFLAGS=-np 4
FLAGS=-std=c++20 -B /usr/include/boost/
BOOST_LIBS=-lboost_system -lboost_thread -lboost_mpi -lboost_serialization -lboost_log -lboost_log_setup -lpthread

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