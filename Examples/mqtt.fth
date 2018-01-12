
load dynamic.fth

0 value init-run
-1 value libmosquitto
-1 value buffer
255 constant /buffer

: init
    init-run 0= if
        /buffer allocate abort" Allocate failed" to buffer
        buffer /buffer erase

        s" /usr/lib/x86_64-linux-gnu/libmosquitto.so.1" dlopen abort" What." to libmosquitto


        -1 to init-run
    then
;

init

( result-cnt arg-cnt func-ptr -- )

\ 1 1 s" test1" libmine dlsym abort" Not found" mkfunc test1

1 3 s" mosquitto_new"      libmosquitto dlsym abort" Not found" mkfunc mqtt-new 
1 0 s" mosquitto_lib_init" libmosquitto dlsym abort" Not found" mkfunc mqtt-init \ Returns 0


: test
    mqtt-init drop

    c" FICL" 1 buffer mqtt-new

;