#include <sys/mman.h>
#include <native/task.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <signal.h>

#define TASK_PRIO  99 /* Highest RT priority */
#define TASK_MODE  0  /* No flags */
#define TASK_STKSZ 0  /* Stack size (use default one) */
RT_TASK task_desc;

void cleanup (void)
{
	rt_task_delete(&task_desc);
}

// General purpose error message
// A real system would probably have a better error handling method...
static void panic(char *message)
{
        fprintf(stderr,"Fatal error: %s\n", message);
        exit(1);
}


#define MAX_LOGENTRIES 200000
static unsigned int logindex;
static struct timespec timestamps[MAX_LOGENTRIES];
static void logtimestamp(void)
{
        clock_gettime(CLOCK_MONOTONIC, &timestamps[logindex++]);
        if(logindex > MAX_LOGENTRIES){
                logindex = 0;
        }
}

static void dumptimestamps(int unused)
{
        FILE *fp = fopen("timestamps3.txt","w");
        int i;
        for(i=0; i < logindex; i++){
                if(timestamps[i].tv_sec > 0){
                        fprintf(fp,"%d.%09d\n", (int) timestamps[i].tv_sec,
                                (int) timestamps[i].tv_nsec);
                }
        }
        fclose(fp);
        exit(0);
}


// Initialize a GPIO pin in Linux using the sysfs interface
FILE *init_gpio(int gpioport)
{
        // Export the pin to the GPIO directory
        FILE *fp = fopen("/sys/class/gpio/export","w");
        fprintf(fp,"%d",gpioport);
        fclose(fp);

        // Set the pin as an output
        char filename[256];
        sprintf(filename,"/sys/class/gpio/gpio%d/direction",gpioport);
        fp = fopen(filename,"w");
        if(!fp){
                panic("Could not open gpio file");
        }
        fprintf(fp,"out");
        fclose(fp);

        // Open the value file and return a pointer to it.
        sprintf(filename,"/sys/class/gpio/gpio%d/value",gpioport);
        fp = fopen(filename,"w");
        if(!fp){
                panic("Could not open gpio file");
        }
        return fp;
}

// Given a FP in the stepper struct, set the I/O pin
// to the specified value. Uses the sysfs GPIO interface.
void setiopin(FILE *fp, int val)
{
        fprintf(fp,"%d\n",val);
        fflush(fp);
}

// Adds "delay" nanoseconds to timespecs and sleeps until that time
static void sleep_until(struct timespec *ts, int delay)
{
        
        ts->tv_nsec += delay;
        if(ts->tv_nsec >= 1000*1000*1000){
                ts->tv_nsec -= 1000*1000*1000;
                ts->tv_sec++;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, ts,  NULL);
}

// Demo program for running two steppers at the same time connected to
// the Raspberry PI platform.

void task_body (void *cookie)
{
	struct timespec ts;
        unsigned int delay = 1000*1000; // Note: Delay in ns
        FILE *pin0 = init_gpio(18);
        FILE *pin1 = init_gpio(23);
        FILE *pin2 = init_gpio(24);
        FILE *pin3 = init_gpio(25);

        signal(SIGINT, dumptimestamps);
        clock_gettime(CLOCK_MONOTONIC, &ts);

        // Lock memory to ensure no swapping is done.
        if(mlockall(MCL_FUTURE|MCL_CURRENT)){
                fprintf(stderr,"WARNING: Failed to lock memory\n");
        }

        // Set our thread to real time priority
 //       struct sched_param sp;
 //       sp.sched_priority = 30;
 //       if(pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp)){
//                fprintf(stderr,"WARNING: Failed to set stepper thread"
 //                       "to real-time priority\n");
 //       }

        while(1){
		                
		sleep_until(&ts,delay); logtimestamp(); setiopin(pin0,1);
                sleep_until(&ts,delay); logtimestamp(); setiopin(pin3,0);
                sleep_until(&ts,delay); logtimestamp(); setiopin(pin1,1);
                sleep_until(&ts,delay); logtimestamp(); setiopin(pin0,0);
                sleep_until(&ts,delay); logtimestamp(); setiopin(pin2,1);
                sleep_until(&ts,delay); logtimestamp(); setiopin(pin1,0);
                sleep_until(&ts,delay); logtimestamp(); setiopin(pin3,1);
                sleep_until(&ts,delay); logtimestamp(); setiopin(pin2,0);
        }
}

int main (int argc, char *argv[])
{
	int err;
	mlockall(MCL_CURRENT|MCL_FUTURE);
	/* ... */
	err = rt_task_create(&task_desc, "MyTaskName", TASK_STKSZ, TASK_PRIO, TASK_MODE);
	while(1)
	{	
		if (!err)
			rt_task_start(&task_desc,&task_body,NULL);
	}	
	/* ... */
	printf("in while loop\n");
}
