COMP = g++-8.3.0
INTERFACEDIR = ../interfaces
COMMONDIR = ../common
SHARED_OBJS = _313263204_a.so _313263204_b.so
OBJS = _313263204_a.o _313263204_b.o $(COMMONDIR)/Ship.o $(COMMONDIR)/Container.o $(COMMONDIR)/Port.o $(COMMONDIR)/Common.o $(COMMONDIR)/Parser.o $(COMMONDIR)/WeightBalanceCalculator.o
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -fPIC
CPP_LINK_FLAG = -lstdc++fs -shared

all: $(SHARED_OBJS)

_313263204_a.so: _313263204_a.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^
_313263204_b.so: _313263204_b.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

_313263204_a.o: _313263204_a.cpp _313263204_a.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
_313263204_b.o: _313263204_b.cpp _313263204_b.h
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

clean:
	rm -f $(OBJS) $(SHARED_OBJS)