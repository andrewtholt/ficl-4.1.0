
libmqttcallback.so:	callbacks.o
	gcc -g -shared -Wl,-soname,libmqttcallback.so -o libmqttcallback.so callbacks.o -lc


callbacks.o:	callbacks.c
	gcc -fPIC -Wall -g -c -o callbacks.o callbacks.c
