
load dynamic.fth

0 value init-run
-1 value libmosquitto
-1 value libhelper
-1 value client
1883 constant port

-1 value buffer

struct
     2 chars  field msg-flag
    64 chars field topic
    32 chars field payload
endstruct msg    


: init
    init-run 0= if
        s" /usr/lib/x86_64-linux-gnu/libmosquitto.so.1" dlopen abort" What." to libmosquitto
        s" /usr/local/lib/libmqttcallback.so" dlopen abort" What."  to libhelper


        -1 to init-run
    then
;

init
1 0 s" sizeOfMessage" libhelper dlsym abort" Not found" mkfunc /buffer

1 3 s" mosquitto_new"      libmosquitto dlsym abort" Not found" mkfunc mqtt-new 
1 4 s" mosquitto_connect"  libmosquitto dlsym abort" Not found" mkfunc mqtt-client 
1 0 s" mosquitto_lib_init" libmosquitto dlsym abort" Not found" mkfunc mqtt-init \ Returns 0
0 2 s" mosquitto_message_callback_set" libmosquitto dlsym abort" Not found" mkfunc mqtt-msg-callback-set 
1 4 s" mosquitto_subscribe"  libmosquitto dlsym abort" Not found" mkfunc mqtt-sub 
1 3 s" mosquitto_loop"       libmosquitto dlsym abort" Not found" mkfunc mqtt-loop 
1 7 s" mosquitto_publish"    libmosquitto dlsym abort" Not found" mkfunc mqtt-pub 

/buffer allocate abort" Allocate failed" to buffer
buffer /buffer erase

s" messageCallback" libhelper dlsym abort" Not found" constant msg-callback

: test
    mqtt-init drop

    s" FICL" drop 1 buffer mqtt-new ?dup 0= abort" new failed." to client
    client s" 192.168.0.65" drop port 10 mqtt-client abort" client failed."
    client msg-callback mqtt-msg-callback-set

    client 0 s" /home/office/proliant/power" drop 0 mqtt-sub

    client 500 1 mqtt-loop 
;

: test-pub
    client 0 s" /home/outside/BackFloodlight/cmnd/power" drop
    s" ON" swap 0 1 mqtt-pub
;


: .msg
    buffer msg-flag w@ . cr
    buffer topic   64 type cr
    buffer payload 32 type cr
;


: run
    begin 
        client 500 1 mqtt-loop
        0=
    while
        buffer msg-flag w@ if
            .msg
            0 buffer msg-flag w!
        then
    repeat
;


