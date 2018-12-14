
-1 value plc

new-plc to plc

plc plc-dump cr

s" 192.168.0.65" plc plc-sethost abort" Set host name failed" 

plc plc-dump cr

1883 plc plc-setport abort" Set port failed"

plc plc-dump cr

plc plc-init abort" init failed."

: addtime ( plc -- ) 
    >r
    s" TIME"
    s" /test/time"
    s" IN"
    r>
    plc-add-io
;


