import RPi.GPIO as GPIO  
GPIO.setmode(GPIO.BCM)  

PIN = 23
POUT = 22

# GPIO 23 set up as input. It is pulled up to stop false signals  
GPIO.setup(PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)  
GPIO.setup(POUT, GPIO.OUT, initial=0)

print "[Interrupt latency test for user level]"  
print "PIN: " + str(PIN) + "  POUT:" + str(POUT)
   
try:  
    GPIO.wait_for_edge(23, GPIO.BOTH)  
    GPIO.output(POUT, GPIO.input(PIN))
except KeyboardInterrupt:  
    GPIO.cleanup()       # clean up GPIO on CTRL+C exit  
GPIO.cleanup()           # clean up GPIO on normal exit
