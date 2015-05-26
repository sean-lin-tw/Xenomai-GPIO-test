# GPIO-test for Xenomai project 
####Various testing code(kernel module / user program) for interrupt latency measurement on embedded devices.
[Learm more about our project on Xenomai](http://wiki.csie.ncku.edu.tw/embedded/xenomai)

The code in repository is for comparing interrupt latency on embedded devices under following circumstances:

  * Xenomai version(if using dual-kernel real-time method) 
  
    2.6.4 or 3.0 
    
  * Hardware
  
    Raspberry Pi or Beaglebone Black
    
  * Latency level
  
    user-level or kernel-level
    
  * Real-time method
  
    PREEMPT_RT or Xenomai
    
  * Driver model
  
    Linux-native or Real-Time Driver Mode(RTDM)
