
s" utils.fth" sfind nip 0= [if]
    load utils.fth
[then]

-1 value sid 
-1 value out-buffer
-1 value in-buffer
0 value init-run
-1. 2value outString

255 constant /buffer

: on
    -1
;

: off 
    0
;

: cmd>
;

: init
    init-run 0= if
        s" localhost" 10001 socket-connect abort" connect" to sid 
    
        /buffer allocate abort" allocate" to in-buffer
        /buffer allocate abort" allocate" to out-buffer
    
        in-buffer  /buffer erase
        out-buffer /buffer erase

        in-buffer 6 sid socket-recv drop

        -1 to init-run
    then
;

: mk-command ( addr n ) 
    in-buffer  /buffer erase
    out-buffer /buffer erase

    out-buffer swap move
    out-buffer dup strlen  add-cr
;

: send-command ( address len --- )
    sid socket-send abort" socket-send" drop
    in-buffer 255 sid socket-recv 

    in-buffer swap dump
;

: command ( cnd len -- )
    mk-command
    send-command
;

: db-print
    s" ^print" mk-command .s
    send-command
    in-buffer 64 dump
;

: ?connected
    s" ^get connected" command
    in-buffer count evaluate
;

: db-connect
    s" ^connect" command
;

: $load { addr n | offset -- }
    s" ^load " dup to offset 
    out-buffer swap move

    addr out-buffer offset + n move
    out-buffer dup strlen  add-cr

    2dup type
    send-command
;

: get-name ( | name len )
    s" ^get-col name" command
    in-buffer count
;

: get-state
    s" ^get-col state" command
    in-buffer count
;

: update ( addr len --- )

    in-buffer  /buffer erase
    out-buffer /buffer erase

    s" select * from io_point where name='" out-buffer swap move
\    out-buffer strlen  addr n  strcat

\    out-buffer swap move   \ len
\    out-buffer swap name len strcat
    
;

: main
    init

    in-buffer 10 dump

    s" sqliteCmds.txt" $load
;

