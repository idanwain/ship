COMP = g++-8.3.0
OBJS = simulator.o ship.o container.o port.o common.o Parser.o outputHandler.o stowage_algorithm.o lifo_algorithm.o Unsorted_Lifo_Algorithm.o WeightBalanceCalculator.o erroneous_algorithm.o
EXEC = ex1
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -lstdc++fs

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@
simulator.o: simulator.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
ship.o: ship.cpp ship.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
container.o: container.cpp container.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
port.o: port.cpp port.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
common.o: common.cpp common.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Parser.o: Parser.cpp Parser.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
outputHandler.o: outputHandler.cpp outputHandler.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
stowage_algorithm.o: stowage_algorithm.cpp stowage_algorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
lifo_algorithm.o: lifo_algorithm.cpp lifo_algorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Unsorted_Lifo_Algorithm.o: Unsorted_Lifo_Algorithm.cpp Unsorted_Lifo_Algorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
WeightBalanceCalculator.o: WeightBalanceCalculator.cpp WeightBalanceCalculator.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
erroneous_algorithm.o: erroneous_algorithm.cpp erroneous_algorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp


clean:
	rm -f $(OBJS) $(EXEC)
all: $(EXEC)