//tempio.c
//
//---------------------------------------------------------
//Code from TmpMULTsensor.c has been added to this program.
//---------------------------------------------------------
//
//This program started with the blinkio program and 
//adds code for a BS18B20 temp sensor.  Also, it changes
//the pushbotton from a momentary to a latch.
//
//The repository for the blinkio.c program is here
//https://github.com/floppydisk525/blinkio
//The blinkio program (was) is for bcm2835 library
//Blinks a pin on and off every 1 sec
//receives pushbutton input to momentary output.  
//
//
//
//NOTE to try:  Need to compile the BS18B20 program
//without the older c files.  
//
//After installing bcm2835, you can build and run this program
//with something like:
//gcc -Wall tempio.c -o tempio -l bcm2835
//gcc -Wall -std=gnu99 tempio.c -o tempio -l bcm2835
//sudo ./tempio
//
//Author: MJ copied from bcm2835 examples

#include <stdio.h>    //include for all c progs
#include <bcm2835.h>  //include for raspPi gpio
#include <time.h>     //include for timing

//next include for reading DS18b20
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

// blinks on rpi plug pi pin 11 (gpio 17) FROM BCM EXAMPLE
//#define PIN RPI_GPIO_P1_11
// blinks on rpi plug pi  pin 7 (gpio  4)
#define IN_GPIO4 RPI_V2_GPIO_P1_07
#define OUT_GPIO5 RPI_V2_GPIO_P1_29
#define OUT_GPIO6 RPI_V2_GPIO_P1_31

//define variables
int hb_state_1s = 0;    //heartBeat IO
int CheckTime = 0;      //boolean (int) to allow time check.
struct timespec gettime_now;

//define global variables for DS18b20 and file system reading
DIR *dir;
struct dirent *dirent;
char dev[16];      // Dev ID
char devPath[128]; // Path to device
char buf[256];     // Data from device
char tmpData[6];   // Temp C * 1000 reported by device 
char path[] = "/sys/bus/w1/devices"; 
ssize_t numRead;
 
//define timing variables to read ds18b20
int DS18TimerStartVal = 0; 		//time that counter starts
int DS18TimerStartValPlusHalfSec = 0;	//time counter starts plus half second
int DS18GetTimeStartFlag = 0;	//Indicates counter is counting and 'have' start time
int DS18ReadTimeInterval = 2;  //read temp every 2 sec
int DS18HalfSecFlag = 0;       //half second time ctr flag
int DS18SecCnt	= 0;            //second counter

//multiple timing variables
int devCnt = 0;

//-----------------------------------------------------
//------------------ HEARTBEAT TIME -------------------
//-----------------------------------------------------
//The gettime_now.tv_nsec counts in nanoseconds from 0 to 1 second (1 billion cts)
//  The way this works is that when the nano seconds get above 10000, the
//  CheckTime variable is set to 1 and the next if statement will then check
//  if the nanocounter has started back at 1.  Once it has, then it will
//  reset the CheckTime variable to 0 (don't check) and toggle the GPIO output.
void HeartBeat()
{
	clock_gettime(CLOCK_REALTIME, &gettime_now);
	//printf("gettime_now: %ld\n",gettime_now.tv_nsec);

   if(gettime_now.tv_nsec > 500000000)
	   CheckTime = 1;

   if (CheckTime == 1 && gettime_now.tv_nsec <500000000)
   {
		CheckTime = 0;                           //set checktime false
		hb_state_1s ^= 1;                        //toggle the pin state
		bcm2835_gpio_write(OUT_GPIO5, hb_state_1s);
   }
}

//-----------------------------------------------------
//-------- DELAY FOR MicroSec WITHOUT SLEEPING --------
//---------------- SEND ANY TIME AMT  -----------------
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
			dm_time_difference += 1000000000;		//(Rolls over every 1 second)
		if (dm_time_difference > (delay_us * 1000))		//Delay for # nS
			break;
	}
}

//-----------------------------------------------------------
//----------------  DS18b20 TEMP SENSOR READ  ---------------
//-----------------------------------------------------------

void DS18Setup ()
{
	int i = 0;

	// 1st pass counts devices
	dir = opendir (path);
	if (dir != NULL)
	{
		while ((dirent = readdir (dir))) {
			 // 1-wire devices are links beginning with 28-
			if (dirent->d_type == DT_LNK && strstr(dirent->d_name, "28-") != NULL) {
				i++;
			}
		}
		(void) closedir (dir);
	}
	else
	{
		perror ("Couldn't open the w1 devices directory");
		//return 1;
	}
	devCnt = i;
	i = 0;

	// 2nd pass allocates space for data based on device count
	char dev[devCnt][16];
	char devPath[devCnt][128];
	dir = opendir (path);
	if (dir != NULL){
		while ((dirent = readdir (dir))) {
			// 1-wire devices are links beginning with 28-
			if (dirent->d_type == DT_LNK && strstr(dirent->d_name, "28-") != NULL) { 
				strcpy(dev[i], dirent->d_name);
			   // Assemble path to OneWire device
				sprintf(devPath[i], "%s/%s/w1_slave", path, dev[i]);
				i++;
			}
		}
		(void) closedir (dir);
	}
	else
	{
		perror ("Couldn't open the w1 devices directory");
		//return 1;
	}
	i = 0;
}

void DS18ReadTemp()
{
	int j = 0;		//counter for reading sensors
	
	while(j != devCnt)
	{
		int fd = open(devPath[j], O_RDONLY);
		if(fd == -1)
		{
			perror ("Couldn't open the w1 device.");
			//return 1;
		}
		while((numRead = read(fd, buf, 256)) > 0) 
		{
			strncpy(tmpData, strstr(buf, "t=") + 2, 5);
			float tempC = strtof(tmpData, NULL);
			printf("Device: %s - ", dev[j]);
			printf("Temp: %.3f C  ", tempC / 1000);
			printf("%.3f F\n", (tempC / 1000) * 9 / 5 + 32);
		}
		close(fd);
		j++;
	}
	j = 0;
	printf("%s\n", ""); // Blank line after each cycle
}

void DS18ReadCheckTime ()
{
	/*This subroutine works by setting DS18TimerStartVal equal to gettime_now.tv_nsec as the base for counting. It then sets a flag DS18GetTimeStartFlag to say that we ahve the base value.  As well as adds a half second to the base value to look at a range of 1/2 second later.  The half second could be made a lot smaller, but keep it large because we don't want to miss a second count (there's probably WAY easier ways to do this.)
	
	Anyway,DS18HalfSecFlag indicates that 1/2 second has gone by and then sets 
	flag to 1, indicating that the next time the counter passes the start value to
	increment a second counter DS18SecCnt.
	
	When DS18SecCnt second counter equals DS18ReadTimeInterval (set in global variables), then we'll read the temparature.  currently set for 1 second 
	intervals, but the timer could be modified to read in smaller increments as desired.  
	*/
	
	//------------------------ read current time -----------------
	//time_t t = time(NULL);
	//struct tm tm = *localtime(&t);

	//printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	//--------------------------------------------------------------
	
	clock_gettime(CLOCK_REALTIME, &gettime_now);   //get current time of counter
	
	if(DS18GetTimeStartFlag == 0)		//set variables with starttime(s), set flag have time
	{
		DS18GetTimeStartFlag =1;
		DS18TimerStartVal = gettime_now.tv_nsec;
		DS18TimerStartValPlusHalfSec = DS18TimerStartVal + 500000000;
		if(DS18TimerStartValPlusHalfSec > 1000000000)
			DS18TimerStartValPlusHalfSec = DS18TimerStartValPlusHalfSec - 1000000000;		
	}

	if(DS18HalfSecFlag == 0)
	{
		if(DS18TimerStartVal <= 500000000)
		{
			if(gettime_now.tv_nsec > DS18TimerStartValPlusHalfSec || gettime_now.tv_nsec <= DS18TimerStartVal)
			{
				DS18HalfSecFlag = 1;
			}
		}
		else	//timerstart time greater than 500000000
		{
			if(gettime_now.tv_nsec > DS18TimerStartValPlusHalfSec && gettime_now.tv_nsec < DS18TimerStartVal)
			{
				DS18HalfSecFlag = 1;
			}
		}
	}
	
	if(DS18HalfSecFlag == 1)
	{
		if(DS18TimerStartVal <= 500000000)
		{
			if(gettime_now.tv_nsec <= DS18TimerStartValPlusHalfSec && gettime_now.tv_nsec > DS18TimerStartVal)
			{
				DS18HalfSecFlag = 0;
				DS18SecCnt++;	//increment 1 sec counter
			}
		}
		else	//timerstart time greater than 500000000
		{
			if(gettime_now.tv_nsec <= DS18TimerStartValPlusHalfSec || gettime_now.tv_nsec > DS18TimerStartVal)
			{
				DS18HalfSecFlag = 0;
				DS18SecCnt++;   //increment 1 sec counter
			}
		}	
	}
	
	if(DS18SecCnt == DS18ReadTimeInterval)
	{
		DS18ReadTemp ();     //read value of temp HERE
		DS18SecCnt = 0;		 //set second counter back to 0.
				
		//following lines print to command line to show reading temp
		//DELETE when ready.
				time_t t = time(NULL);
		struct tm tm = *localtime(&t);		
		printf("READ TEMPERATURE (time): %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		//----------------  delete to here -----------------
	}
}

//-----------------------------------------------------------
//------------------------  MAIN  ---------------------------
//-----------------------------------------------------------

int main(int argc, char **argv)
{
    // If you call this, it will not actually access the GPIO
    // Use for testing
	//bcm2835_set_debug(1);
    if (!bcm2835_init())
        return 1;

    // Set RPI pin P1-15 to be an input
    bcm2835_gpio_fsel(IN_GPIO4, BCM2835_GPIO_FSEL_INPT);
    //  with a pullup
    // bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_UP);
    bcm2835_gpio_fsel(OUT_GPIO5, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(OUT_GPIO6, BCM2835_GPIO_FSEL_OUTP);

	DS18Setup ();	//setup DS18 info

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

		DS18ReadCheckTime (); //call read temperature

        // wait a bit (which is better below?)
        delay(10);      //time in ms
//        DelayMicrosecondsNoSleep(10);     //time in msecs
    }

   //-----------------------------------------------------------
   //------------------  END OF INFITIE LOOP  ------------------

    bcm2835_close();
    return 0;
}
