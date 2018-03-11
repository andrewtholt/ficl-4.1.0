
CPP=g++ -std=c++11
CFLAGS=-g -fPIC
LIBS=
INCS=-I/usr/local/include
BINS=ficlTest

MAJOR=1
MINOR=0

all:	$(BINS) libficltimer.so.$(MAJOR).$(MINOR)

ficlTimer.o:	ficlTimer.cpp ficlTimer.h
	$(CPP) -c $(CFLAGS) $(INCS) ficlTimer.cpp -o ficlTimer.o

ficlTest:	ficlTimer.o ficlTest.cpp
	$(CPP) $(CFLAGS) $(INCS) ficlTest.cpp ficlTimer.o -o ficlTest -lnewtimers

libficltimer.so.$(MAJOR).$(MINOR):	ficlTimer.o
		gcc -g -shared -Wl,-soname,libficltimer.so.$(MAJOR).$(MINOR) -o libutils.so ficlTimer.o -lc -lstdc++

clean:
	rm -f $(BINS)
	rm -f *.o cscope.out lib*

