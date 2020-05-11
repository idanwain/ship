COMP = g++-8.3.0
SHARED_OBJS = _313263204_a.so _313263204_b.so
OBJS = _313263204_a.o _313263204_b.o
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
clean:
	rm -f $(OBJS) $(SHARED_OBJS)