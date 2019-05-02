//
//Copied this C program from 
//http://bradsrpi.blogspot.com/2013/12/c-program-to-read-temperature-from-1.html
//This program reads qty 1 sensor DS18B20.
//
//To COMPILES this program 
//Assuming the code is saved in a file called Tmp1sensor.c, run the following tom compile - 
//
//gcc -Wall -std=gnu99 Tmp1sensor.c -o Tmp1sensor
//
//Run the code by typing ./Tmp1sensor  
//Press ctrl-c to end the program.
//
//
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
     
int main (void) {
 DIR *dir;
 struct dirent *dirent;
 char dev[16];      // Dev ID
 char devPath[128]; // Path to device
 char buf[256];     // Data from device
 char tmpData[6];   // Temp C * 1000 reported by device 
 char path[] = "/sys/bus/w1/devices"; 
 ssize_t numRead;
 
 dir = opendir (path);
 if (dir != NULL)
 {
  while ((dirent = readdir (dir)))
   // 1-wire devices are links beginning with 28-
   if (dirent->d_type == DT_LNK && 
     strstr(dirent->d_name, "28-") != NULL) { 
    strcpy(dev, dirent->d_name);
    printf("\nDevice: %s\n", dev);
   }
        (void) closedir (dir);
        }
 else
 {
  perror ("Couldn't open the w1 devices directory");
  return 1;
 }

        // Assemble path to OneWire device
 sprintf(devPath, "%s/%s/w1_slave", path, dev);
 // Read temp continuously
 // Opening the device's file triggers new reading
 while(1) {
  int fd = open(devPath, O_RDONLY);
  if(fd == -1)
  {
   perror ("Couldn't open the w1 device.");
   return 1;   
  }
  while((numRead = read(fd, buf, 256)) > 0) 
  {
   strncpy(tmpData, strstr(buf, "t=") + 2, 5); 
   float tempC = strtof(tmpData, NULL);
   printf("Device: %s  - ", dev); 
   printf("Temp: %.3f C  ", tempC / 1000);
   printf("%.3f F\n\n", (tempC / 1000) * 9 / 5 + 32);
  }
  close(fd);
 } 
        /* return 0; --never called due to loop */
}