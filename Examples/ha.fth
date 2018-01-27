
load haLoadClass.fth

: setup
\    mqtt-init drop

    s" FICL" drop 1 mqtt-buffer mqtt-new ?dup 0= abort" new failed." to client
    client s" 192.168.0.65" drop port 10 mqtt-client abort" client failed."
    client msg-callback mqtt-msg-callback-set

    client mqtt-loop-start abort" Loop start, failed."

;

setup

c-powerload --> new lamp
s" /home/outside/BackFloodlight/cmnd/power" lamp --> setTopic

0 lamp --> setOutput
lamp --> display

