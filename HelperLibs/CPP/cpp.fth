s" dynamic.fth" sfind nip 0= [if]
    load dynamic.fth
[then]


s" ./libcpp.so" dlopen abort" Failed to open libcpp" value libcpp

1 0 s" newVector"      libcpp dlsym abort" Not found" mkfunc new-vector
1 1 s" vectorSize"     libcpp dlsym abort" Not found" mkfunc vector-size
1 2 s" vectorGet"      libcpp dlsym abort" Not found" mkfunc vector-get

0 2 s" vectorAppend"   libcpp dlsym abort" Not found" mkfunc vector-append
0 2 s" vectorErase"    libcpp dlsym abort" Not found" mkfunc vector-erase
0 1 s" vectorEraseAll" libcpp dlsym abort" Not found" mkfunc vector-erase-all


-1 value v
: test
    new-vector to v

    v s" Test" drop  vector-append

    v vector-size . cr

    v 0 vector-get .s

    v 0 vector-erase

    v vector-size . cr

;
