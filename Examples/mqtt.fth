
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
1 3 s" mosquitto_loop"       libmosquitto dlsym abort" Not found" mkfunc mqtt-loop 
1 4 s" mosquitto_subscribe"  libmosquitto dlsym abort" Not found" mkfunc (mqtt-sub)
1 7 s" mosquitto_publish"    libmosquitto dlsym abort" Not found" mkfunc (mqtt-pub)

/buffer allocate abort" Allocate failed" to buffer
buffer /buffer erase

s" messageCallback" libhelper dlsym abort" Not found" constant msg-callback

: mqtt-sub { topic tlen -- rc }
    client 0 topic 0 (mqtt-sub)
;

: mqtt-pub { topic tlen payload plen -- rc }
   client 0 topic plen payload 0 1 (mqtt-pub)
;

: setup
    mqtt-init drop

    s" FICL" drop 1 buffer mqtt-new ?dup 0= abort" new failed." to client
    client s" 192.168.0.65" drop port 10 mqtt-client abort" client failed."
    client msg-callback mqtt-msg-callback-set

    s" /home/office/proliant/power" mqtt-sub drop
    s" /home/environment/day" mqtt-sub drop

\    client 500 1 mqtt-loop 
;

: test-pub
    s" /home/outside/BackFloodlight/cmnd/power" s" ON" mqtt-pub
;

: .msg
    buffer msg-flag w@ . cr
    buffer topic   64 type cr
    buffer payload 32 type cr
;

0 value proliant_power
0 value daylight

: switch-backlight ( on|off -- )
    s" /home/outside/BackFloodlight/cmnd/power" 
    rot 0<> if
        s" ON"
    else 
        s" OFF"
    then
    mqtt-pub
;

: /home/office/proliant/power
    2dup s" ON"  compare 0= if -1 to proliant_power then
    2dup s" OFF" compare 0= if  0 to proliant_power then

    2drop
;

: /home/environment/day
    2dup s" TRUE"  compare 0= if -1 to daylight then
    2dup s" FALSE" compare 0= if  0 to daylight then

    2drop
;

: logic
    daylight invert switch-backlight
;

: run
    setup
    begin 
        client 500 1 mqtt-loop
        0=
    while
        buffer msg-flag w@ if
            .msg
            buffer payload dup strlen
            buffer topic dup strlen evaluate

            proliant_power . cr
            logic
            0 buffer msg-flag w!
        then
    repeat
;


