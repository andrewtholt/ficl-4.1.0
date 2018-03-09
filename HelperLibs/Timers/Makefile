
CPP=g++ -std=c++11
CFLAGS=-g -fPIC
LIBS=
INCS=-I/usr/local/include
BINS=ficlTest

all:	$(BINS)

ficlTimer.o:	ficlTimer.cpp ficlTimer.h
	$(CPP) -c $(CFLAGS) $(INCS) ficlTimer.cpp -o ficlTimer.o

ficlTest:	ficlTimer.o ficlTest.cpp
	$(CPP) $(CFLAGS) $(INCS) ficlTest.cpp ficlTimer.o -o ficlTest -lnewtimers

clean:
	rm -f $(BINS)
	rm -f *.o cscope.out

