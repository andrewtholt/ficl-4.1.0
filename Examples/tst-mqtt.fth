
load dynamic.fth
load mqtt.fth

: setup
\    mqtt-init drop

    s" FICL" drop 1 mqtt-buffer mqtt-new ?dup 0= abort" new failed." to client
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
    mqtt-buffer msg-flag drop w@ . cr
    mqtt-buffer topic   type cr
    mqtt-buffer payload type cr
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
        mqtt-buffer msg-flag drop w@ if
            .msg
            mqtt-buffer payload drop dup strlen 
            mqtt-buffer topic drop dup strlen evaluate

            proliant_power . cr
            logic drop
            0 mqtt-buffer msg-flag drop w!
        then
    repeat
;


