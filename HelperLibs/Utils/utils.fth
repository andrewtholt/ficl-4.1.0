
s" dynamic.fth" sfind nip 0= [if]
    load dynamic.fth
[then]

-1 value libutils
-1 value libc

\ int expandPath(char *dirList, char *fileName, char *buffer);


s" /usr/local/lib/libutils.so" dlopen abort" failed" to libutils
s" libc.so.6"                  dlopen abort" failed" to libc

1 3 s" expandPath" libutils dlsym abort" Not Found" mkfunc expand-path
0 2 s" memDump"    libutils dlsym abort" Not Found" mkfunc dump

1 1 s" usleep"     libc     dlsym abort" Not Found" mkfunc (usleep)

1024 mk-buffer drop value location
1024 mk-buffer drop value path-list

: us ( us -- )
    (usleep) drop
;

: ms ( ms -- ) 
    1000 * us
;

\ : test
\     path-list s" PATH" getenv
\ 
\     drop s" ls" drop location expand-path
\ 
\ ;
 

