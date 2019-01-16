only forth also oop definitions

object --> sub c-plc
    c-string obj: .db-name
    c-byte   obj: .clean
    c-ptr    obj: .plc

    : init { 2:this -- }
        ." plc-init" cr
        new-plc this --> .plc --> set-ptr

    ;

    : plc@ { 2:this -- }
        this --> .plc --> get-ptr  
    ;

    : dump { 2:this -- }
        this --> plc@  plc-dump
    ;

    : set-name { addr len 2:this -- }
        addr len this --> plc@ plc-dbname!
    ;

    : setup { clean 2:this -- }
        clean this --> plc@ plc-db-setup
    ;

    : value@ { name len 2:this -- }
        name len this --> plc@ plc-value@
    ;

    : verbose { 2:this -- }
        1 this --> plc@ plc-verbose
    ;

end-class

c-plc --> new plc

plc --> methods

s" /tmp/tst.db" plc --> set-name

plc --> dump

0 plc --> setup

: set-start
    s" START" plc --> plc@ plc-value!
;

: set-stop
    s" STOP" plc --> plc@ plc-value!
;

s" STOP" plc --> value@ type cr

-1 value tst
plc --> plc@ to tst

: run
    s" START" tst plc-ld
    s" MOTOR" tst plc-or
    s" STOP" tst plc-andn
    s" MOTOR" tst plc-out
    1 tst plc-end
    s" OFF" set-start
    s" START" tst plc-ld
    s" MOTOR" tst plc-or
    s" STOP" tst plc-andn
    s" MOTOR" tst plc-out
    1 tst plc-end
;

: toggle-start
    s" ON" set-start
    run
    s" OFF" set-start
    run
;

: toggle-stop
    s" ON" set-stop
    run
    s" OFF" set-stop
    run
;

