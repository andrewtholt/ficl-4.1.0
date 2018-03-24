CFLAGS=-g -fPIC
CPP=g++

all:	tst

tst:	tst.c cpp.o
	$(CC) $(CFLAGS) tst.c cpp.o -o tst -lstdc++

cpp.o:	cpp.cpp cpp.h
	$(CPP) -c $(CFLAGS) cpp.cpp  -o cpp.o

libcpp.so:	cpp.o
	$(CC) -g -shared -Wl,-soname,libcpp.so -o libcpp.so cpp.o -lc -lstdc++

clean:
	rm -f cscope.out *.o
