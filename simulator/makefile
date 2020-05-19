COMP = g++-9.3.0
INTERFACEDIR = ../interfaces
COMMONDIR = ../common
OBJS = main.o $(COMMONDIR)/Ship.o $(COMMONDIR)/Container.o $(COMMONDIR)/Port.o $(COMMONDIR)/Common.o $(COMMONDIR)/Parser.o $(COMMONDIR)/WeightBalanceCalculator.o SimulatorObj.o AlgorithmFactoryRegistrar.o AlgorithmRegistration.o  Travel.o SimulatorValidation.o
EXEC = simulator
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -lstdc++fs -ldl -export-dynamic

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@

main.o: main.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
$(COMMONDIR)/Ship.o: $(COMMONDIR)/Ship.cpp $(COMMONDIR)/Ship.h
	$(COMP) $(CPP_COMP_FLAG) -o $@ -c $*.cpp
$(COMMONDIR)/Container.o: $(COMMONDIR)/Container.cpp $(COMMONDIR)/Container.h
	$(COMP) $(CPP_COMP_FLAG) -o $@ -c $*.cpp
$(COMMONDIR)/Port.o: $(COMMONDIR)/Port.cpp $(COMMONDIR)/Port.h
	$(COMP) $(CPP_COMP_FLAG) -o $@ -c $*.cpp
$(COMMONDIR)/Common.o: $(COMMONDIR)/Common.cpp $(COMMONDIR)/Common.h
	$(COMP) $(CPP_COMP_FLAG) -o $@ -c $*.cpp
$(COMMONDIR)/Parser.o: $(COMMONDIR)/Parser.cpp $(COMMONDIR)/Parser.h
	$(COMP) $(CPP_COMP_FLAG) -o $@ -c $*.cpp
$(COMMONDIR)/WeightBalanceCalculator.o: $(COMMONDIR)/WeightBalanceCalculator.cpp
	$(COMP) $(CPP_COMP_FLAG) -o $@ -c $*.cpp
SimulatorObj.o: SimulatorObj.cpp SimulatorObj.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
AlgorithmFactoryRegistrar.o: AlgorithmFactoryRegistrar.cpp AlgorithmFactoryRegistrar.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
AlgorithmRegistration.o: AlgorithmRegistration.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Travel.o: Travel.cpp Travel.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
SimulatorValidation.o: SimulatorValidation.cpp SimulatorValidation.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

clean:
	rm -f $(OBJS) $(EXEC)