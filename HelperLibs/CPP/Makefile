CFLAGS=-g
CPP=g++

all:	tst

tst:	tst.c cpp.o
	$(CC) $(CFLAGS) tst.c cpp.o -o tst -lstdc++

cpp.o:	cpp.cpp cpp.h
	$(CPP) -c $(CFLAGS) cpp.cpp  -o cpp.o

clean:
	rm -f cscope.out *.o
