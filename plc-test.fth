
-1 value plc

new-plc to plc

plc plc-dump cr

s" 192.168.0.65" plc plc-sethost abort" Set host name failed" 

plc plc-dump cr

1883 plc plc-setport abort" Set port failed"

plc plc-dump cr

s" FORTH" plc plc-init abort" init failed."

1 plc plc-verbose

: add-start ( plc -- ) 
    >r
    s" START"
    s" /test/start"
    s" IN"
    r>
    plc-add-io
;

: add-stop
    >r
    s" STOP"
    s" /test/stop"
    s" IN"
    r>
    plc-add-io
;

: add-light
    >r
    s" LIGHT"
    s" /home/outside/BackFloodlight/cmnd/power"
    s" OUT"
    r>
    plc-add-io
;

: logic 
    s" START" plc plc-ld
    s" LIGHT" plc plc-or
    s" STOP"  plc plc-andn
    s" LIGHT" plc plc-out

    1000 plc plc-end
;

    

plc add-start
plc add-stop
plc add-light

: go

    begin
        logic
        plc plc-mutex sem-wait
    0<> until
;

\ s" plc" daemon
plc plc-run

go

