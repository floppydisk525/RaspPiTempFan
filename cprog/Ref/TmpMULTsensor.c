//To compile and run the code, use the following commands:
//gcc -Wall -o w1m w1m.c
//./w1m

// This program reads multiple sensors and prints them to cmd line

// Program copied from
//    http://bradsrpi.blogspot.com/2014/01/c-program-to-read-temperature-from.html


#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main (void) {
	DIR *dir;
	struct dirent *dirent;
	char buf[256];     // Data from device
	char tmpData[5];   // Temp C * 1000 reported by device 
	const char path[] = "/sys/bus/w1/devices"; 
	ssize_t numRead;
	int i = 0;
	int devCnt = 0;

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
		return 1;
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
		return 1;
	}
	i = 0;

	// Read temp continuously
	// Opening the device's file triggers new reading
	while(1) 
	{
		int fd = open(devPath[i], O_RDONLY);
		if(fd == -1)
		{
			perror ("Couldn't open the w1 device.");
			return 1;
		}
		while((numRead = read(fd, buf, 256)) > 0) 
		{
			strncpy(tmpData, strstr(buf, "t=") + 2, 5);
			float tempC = strtof(tmpData, NULL);
			printf("Device: %s - ", dev[i]);
			printf("Temp: %.3f C  ", tempC / 1000);
			printf("%.3f F\n", (tempC / 1000) * 9 / 5 + 32);
		}
		close(fd);
		i++;
		if(i == devCnt) {
			i = 0;
			printf("%s\n", ""); // Blank line after each cycle
		}
	}
    return 0;
}