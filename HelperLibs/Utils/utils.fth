
load dynamic.fth

-1 value libutils

\ int expandPath(char *dirList, char *fileName, char *buffer);


s" /usr/local/lib/libutils.so" dlopen abort" failed" to libutils

1 3 s" expandPath" libutils dlsym abort" Not Found" mkfunc expand-path


1024 mk-buffer drop value location
1024 mk-buffer drop value path-list


\ : test
\     path-list s" PATH" getenv
\ 
\     drop s" ls" drop location expand-path
\ 
\ ;
 

