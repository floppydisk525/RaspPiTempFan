/*
thrheart.c program by MJohnsen
Program to test heartbeat on a different thread.  Initial program to run heartbeat
and print out heartbeat to the cmd line.  Will be the basis to start adding 'other' 
features.  

programs threading programs to adding
1. Passing data, ie location and count of temp sensors 
	NOTE:  include dummy data in QEMU (how to include PB)
1.5 read dummy temp data
2. writing to a db with heart beat and temp sensors doing they're thing

adding sqlite3 database commands
sudo apt-get install sqlite3
sudo apt-get install sqlitebrowser  (not sure what this is - large dwnload...)

other Q's
how to do dependency injection.  so that you can run program from 
    qemu emulation vs. on real hrdwr RaspPI

to compile
gcc -Wall thrheart2.c -o thrheart2 -lpthread
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
//#include <string.h>

// Flag used by handler - changed to 0 when user presses Ctrl-C
// Loop that reads & records temperatures keeps running when
// keepRunning = 1
int8_t volatile keepRunning = 1;

void *HeartBeat(void *data)
{
    //int my_data = (int)data;     //used to pass thread id
	struct timespec gettime_now;
	int CheckTime = 0;
	int hrtcnt = 0;

	while (keepRunning)
	{
		clock_gettime(CLOCK_REALTIME, &gettime_now);
		//printf("gettime_now: %ld\n",gettime_now.tv_nsec);

	   if(gettime_now.tv_nsec > 500000000)
		   CheckTime = 1;

	   if (CheckTime == 1 && gettime_now.tv_nsec <500000000)
	   {
			CheckTime = 0;                           //set checktime false
			//hb_state_1s ^= 1;                        //toggle the pin state
			//bcm2835_gpio_write(OUT_GPIO5, hb_state_1s);
			hrtcnt++;
			printf("\nHEARTBEAT %d\n", hrtcnt);
	   }
	   usleep(10000);		//delay 10ms to let processor do other stuff
	}
   
    //printf("\n Hello from new thread - got %d !\n", my_data);
    pthread_exit(NULL);
}

// Called when user presses Ctrl-C
void intHandler() {
    printf("\nCTRL-C Pressed...  Stopping...\n");
    keepRunning = 0;
}

int main()
{
     // Intercept Ctrl-C (SIGINT) in order to finish writing data & close DB
	signal(SIGINT, intHandler);
	int rc;
    pthread_t thread_id;

    //int t = 11;

//    rc = pthread_create(&thread_id, NULL, HeartBeat, (void*)t);
	rc = pthread_create(&thread_id, NULL, HeartBeat, NULL);
	if(rc)
    {
      printf("\n ERROR: return code from pthread_create is %d \n", rc);
      return(1);
    }
    printf("\n Created new thread (%u)... \n", thread_id);
	
	int i = 0;
	while (keepRunning){
		printf("Main Timing %d\n", i);
		usleep(100000);		//delay 100ms 
		i++;
		if(i == 10)
			i=0;
	}
	
	pthread_join(thread_id, NULL);    //waits for the HeartBeat thread (to stop?)
	
	printf("\nProgram Exiting, threads stopped.\n");
    pthread_exit(NULL);	
	return (0);
}