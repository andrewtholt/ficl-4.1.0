
s" utils.fth" sfind nip 0= [if]
    load utils.fth
[then]

-1 value sid 
-1 value out-buffer
-1 value in-buffer
-1. 2value outString


255 constant /buffer

: main
    s" localhost" 10001 socket-connect abort" connect" to sid 
    
    /buffer allocate abort" allocate" to in-buffer
    /buffer allocate abort" allocate" to out-buffer
    
    in-buffer  /buffer erase
    out-buffer /buffer erase

    in-buffer 6 sid socket-recv .s

    in-buffer 10 dump

    s" ^print" out-buffer swap move
    out-buffer 6 add-cr
    sid socket-send abort" socket-send"

    out-buffer 10 dump

    in-buffer 255 sid socket-recv .s
    in-buffer 10 dump

;

