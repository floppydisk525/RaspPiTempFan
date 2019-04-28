//blinkio.c
//
//Example program for bcm2835 library
//Blinks a pin on and off every 1 sec
//
//After installing bcm2835, you can build this
//with something like:
//gcc -o blinkio blink.c -l bcm2835
//sudo ./blinkio
//
//Author: MJ copied from bcm2835 examples

#include <stdio.h>    //include for all c progs
#include <bcm2835.h>  //include for raspPi gpio
#include <time.h>     //include for timing

// blinks on rpi plug pi pin 11 (gpio 17) FROM BCM EXAMPLE
//#define PIN RPI_GPIO_P1_11
// blinks on rpi plug pi  pin 7 (gpio  4)
#define IN_GPIO4 RPI_V2_GPIO_P1_07
#define OUT_GPIO5 RPI_V2_GPIO_P1_29
#define OUT_GPIO6 RPI_V2_GPIO_P1_31

//define variable
int hb_state_1s = 0;    //heartBeat IO
int CheckTime = 0;      //boolean (int) to allow time check.
struct timespec gettime_now;

//--------------------------
//----- HEARTBEAT TIME -----
//--------------------------
//The gettime_now.tv_nsec counts in nanoseconds from 0 to 1 second (1 billion cts)
//  The way this works is that when the nano seconds get above 10000, the
//  CheckTime variable is set to 1 and the next if statement will then check
//  if the nanocounter has started back at 1.  Once it has, then it will
//  reset the CheckTime variable to 0 (don't check) and toggle the GPIO output.
void HeartBeat()
{
   clock_gettime(CLOCK_REALTIME, &gettime_now);
//   printf("gettime_now: %ld\n",gettime_now.tv_nsec);

   if(gettime_now.tv_nsec > 500000000)
     CheckTime = 1;

   if (CheckTime == 1 && gettime_now.tv_nsec <500000000)
   {
     CheckTime = 0;                           //set checktime false
     hb_state_1s ^= 1;                        //toggle the pin state
     bcm2835_gpio_write(OUT_GPIO5, hb_state_1s);
   }
}

//*****************************************************
//******** DELAY FOR MicroSec WITHOUT SLEEPING ********
//**************** SEND ANY TIME AMT  *****************
//Using delayMicroseconds lets the linux scheduler decide to jump to another process.  Using this function avoids letting the
//scheduler know we are pausing and provides much faster operation if you are needing to use lots of delays.
void DelayMicrosecondsNoSleep (int delay_us)
{
	long int dm_start_time;
	long int dm_time_difference;
	struct timespec dm_gettime_now;

	clock_gettime(CLOCK_REALTIME, &dm_gettime_now);
	dm_start_time = dm_gettime_now.tv_nsec;		//Get nS value
	while (1)
	{
		clock_gettime(CLOCK_REALTIME, &dm_gettime_now);
		dm_time_difference = dm_gettime_now.tv_nsec - dm_start_time;
		if (dm_time_difference < 0)
			dm_time_difference += 1000000000;				//(Rolls over every 1 second)
		if (dm_time_difference > (delay_us * 1000))		//Delay for # nS
			break;
	}
}

//-----------------------------------------------------------
//------------------------  MAIN  ---------------------------
//-----------------------------------------------------------

int main(int argc, char **argv)
{
    // If you call this, it will not actually access the GPIO
    // Use for testing
//    bcm2835_set_debug(1);
    if (!bcm2835_init())
        return 1;

    // Set RPI pin P1-15 to be an input
    bcm2835_gpio_fsel(IN_GPIO4, BCM2835_GPIO_FSEL_INPT);
    //  with a pullup
    // bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_UP);
    bcm2835_gpio_fsel(OUT_GPIO5, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(OUT_GPIO6, BCM2835_GPIO_FSEL_OUTP);

    //-----------------------------------------------------------
    //-------------------  infinite while loop ------------------
    //-----------------------------------------------------------
    while (1)
    {
        // Read some data
        uint8_t value = bcm2835_gpio_lev(IN_GPIO4);
        //printf("read from gpio 4: %d\n", value);
        bcm2835_gpio_write(OUT_GPIO6, value);

        HeartBeat();       //call heartbeat function

        // wait a bit (which is better below?
        delay(10);      //time in ms
//        DelayMicrosecondsNoSleep(10);     //time in msecs
    }

   //-----------------------------------------------------------
   //------------------  END OF INFITIE LOOP  ------------------

    bcm2835_close();
    return 0;
}
