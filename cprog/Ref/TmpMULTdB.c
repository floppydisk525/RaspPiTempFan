//Assuming that the code above is saved in a file name w1mdb.c, compile it using the following command:
//gcc -Wall -lsqlite3 -o w1mdb w1mdb.c

//file taken from:
//  http://bradsrpi.blogspot.com/search?q=1-wire+sensor+sqlite3

/*
C Program to Read Multiple DS18B20 1-Wire Temperature Sensors & Save Data to a Sqlite Database on a Raspberry Pi
The C program below reads the temperature from multiple DS18B20 1-Wire devices and saves the data to a Sqlite3 database.  This example has a couple additional features: It uses a linked list rather than arrays to keep track of the attached sensors and it uses signal() with an event handler to detect when the user presses ctrl-C to end the program.  The handler allows for the current round of readings to complete and closes the database safely.

To use 1-Wire devices with the Raspberry Pi, you will need to load a couple kernel modules by issuing the following commands before running the program:


modprobe w1-gpio
modprobe w1-therm

Connections

The DS18B20s are hooked up in non-parasitic power mode.  Looking at the flat side of the head of the sensor, the left pin is connected to ground, the right pin is connected to 3V3, and the center pin is attached to the center pin of the next sensor.  There is a 4.7k Ohm pull-up resistor on the connection to the Raspberry Pi's (rev. B) GPIO4 pin.

Sqlite3 Database Table

The code below assumes a Sqlite3 database in a file called ds18b20_temp_data.db in the same directory as the program.  Here is the schema for the table:

CREATE TABLE ds18b20_temp_data (
   date_time    date    not null,
   dev_id       varchar(16) not null,
   temp_c       number(7,3) not null,
   constraint pk_ds18b20_temp_data primary key(date_time, dev_id)
);
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <signal.h>

// Flag used by handler - changed to 0 when user presses Ctrl-C
// Loop that reads & records temperatures keeps running when
// keepRunning = 1
int8_t volatile keepRunning = 1;

// Pointer to Sqlite3 DB - used to access DB when open
sqlite3 *db = NULL;
// Path to DB file - same dir as this program's executable
char *dbPath = "ds18b20_temp_data.db";
// DB Statement handle - used to run SQL statements
sqlite3_stmt *stmt = NULL;

// struct to hold ds18b20 data for linked list
// 1-Wire driver stores info in file for device as text
struct ds18b20 {
 char devPath[128];
 char devID[16];
 char tempData[6];
 struct ds18b20 *next;
};

// Find connected 1-wire devices. 1-wire driver creates entries for each device
// in /sys/bus/w1/devices on the Raspberry Pi. Create linked list.
int8_t findDevices(struct ds18b20 *d) {
  DIR *dir;
        struct dirent *dirent;
  struct ds18b20 *newDev;
        char path[] = "/sys/bus/w1/devices";
        int8_t i = 0;
        dir = opendir(path);
        if (dir != NULL)
        {
                while ((dirent = readdir(dir))) {
                        // 1-wire devices are links beginning with 28-
                        if (dirent->d_type == DT_LNK &&
                                        strstr(dirent->d_name, "28-") != NULL) {
        newDev = malloc( sizeof(struct ds18b20) );
                                strcpy(newDev->devID, dirent->d_name);
                                // Assemble path to OneWire device
                                sprintf(newDev->devPath, "%s/%s/w1_slave", path, newDev->devID);
                                i++;
        newDev->next = 0;
        d->next = newDev;
        d = d->next;
                        }
    }
    (void) closedir(dir);
        }
        else {
                perror ("Couldn't open the w1 devices directory");
                return 1;
        }
  return i;
}

// Write data to DB (DB already opened in main())
int8_t recordTemp(char *devID, double tempC) {
 char *sql = "INSERT INTO ds18b20_temp_data(date_time, dev_id, temp_c) VALUES(datetime('now'), ?, ?)";
 sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
     sqlite3_bind_text(stmt, 1, devID, strlen(devID), 0);
     sqlite3_bind_double(stmt, 2, tempC);
     sqlite3_step(stmt);  // Run SQL INSERT
     sqlite3_reset(stmt); // Clear statement handle for next use
 return 0;
}

// Cycle through linked list of devices & take readings.
// Print out results & store readings in DB.
int8_t readTemp(struct ds18b20 *d) {
  while(d->next != NULL){
    d = d->next;
    int fd = open(d->devPath, O_RDONLY);
    if(fd == -1) {
            perror ("Couldn't open the w1 device.");
                  return 1;
          }
  // 1-wire driver stores data in file as long block of text
                // Store file contents in buf & look for t= that marks start of temp.
    char buf[256];
    ssize_t numRead;
          while((numRead = read(fd, buf, 256)) > 0) {
                 strncpy(d->tempData, strstr(buf, "t=") + 2, 5);
                 double tempC = strtod(d->tempData, NULL);
   // Driver stores temperature in units of .001 degree C
   tempC /= 1000;
                 printf("Device: %s  - ", d->devID);
                 printf("Temp: %.3f C  ", tempC);
                  printf("%.3f F\n\n", tempC * 9 / 5 + 32);
   recordTemp(d->devID, tempC);
          }
          close(fd);
  }
  return 0;
}

// Called when user presses Ctrl-C
void intHandler() {
    printf("\nStopping...\n");
    keepRunning = 0;
}

int main (void) {
 // Intercept Ctrl-C (SIGINT) in order to finish writing data & close DB
 signal(SIGINT, intHandler);
 struct ds18b20 *rootNode;
 struct ds18b20 *devNode;
        int rc = sqlite3_open(dbPath, &db);
 // If rc is not 0, there was an error
        if(rc){
                fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
                exit(0);
        }
 // Handler sets keepRunning to 0 when user presses Ctrl-C
 // When Ctrl-C is pressed, complete current cycle of readings,
 // close DB, & exit.
 while(keepRunning) {
    rootNode = malloc( sizeof(struct ds18b20) );
    devNode = rootNode;
    int8_t devCnt = findDevices(devNode);
    printf("\nFound %d devices\n\n", devCnt);
    readTemp(rootNode);
    // Free linked list memory
    while(rootNode) {
      // Start with current value of root node
      devNode = rootNode;
      // Save address of next devNode to rootNode before 
                        // deleting current devNode
      rootNode = devNode->next;
      // Free current devNode.
      free(devNode);
    }
    // Now free rootNode
    free(rootNode);
  }
 sqlite3_close(db);
  return 0;
}


