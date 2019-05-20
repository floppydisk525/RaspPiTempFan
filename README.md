# RaspPiTempFan
Raspberry Pi Temperature control of a wall fan to control temperature of garage

## OVERVIEW
This project uses a raspberry Pi to check the temperature inside a garage and ambient outside temperature and turn on a wall fan that will blow hot air out of the garage and suck in cool air to cool the garage.  

By having a temperature sensor outside, the program running on the raspberry pi will decide whether venting the garage makes sense.  IE, if it's hotter outside than inside, then it doesn't make sense to vent the garage.  


## PARTS
### RaspberryPi or Similar (TBD)
[RaspberryPi](https://www.raspberrypi.org/)

![alt text][RaspPi]

### 3.3Vdc Input 120/10A Output Relay Solid State Relay
[Icstation 3V 1 Channel Relay Power Switch Module with Optocoupler](https://www.amazon.com/gp/product/B01M0E6SQM)

![alt text][3_3vdc]

[DataSheet](http://www.circuitbasics.com/wp-content/uploads/2016/03/DS18B20-Datasheet.pdf)

[How much current??](https://www.amazon.com/ask/questions/Tx1Y6ACZYHHM2PY/ref=ask_dp_dpmw_al_hza)
The gpio pin is connected to the input of an optical coupler on the relay board. Mine drew 0.15mA from the GPIO pin and 78mA of power from the 3.3v power rail in the on state. 

[How to setup?](https://www.amazon.com/ask/questions/Tx2I5KJHUHOQISZ/ref=ask_dp_dpmw_al_hza)
My working configuration is:  
gnd = gnd pin from pi + external 3.3vdc -  
Signal = output pin from pi  
VCC = external 3.3vdc +  
Jumpers = on  

### Temperature Sensor DS18B20
[Aideepen 5pcs DS18B20 Waterproof Temperature Sensor Digital Thermal Stainless Steel Tube Probe 1 Meter](https://www.amazon.com/gp/product/B01LY53CED)
![alt text][DS18B20]

Output lead:  
Red- VCC  
Yellow- DATA  
Black-GND  

### HiLetgo DHT11 Temperature and Humidity Sensor Module (Not Implemented)
[HiLetgo 5pcs DHT11 Temperature and Humidity Sensor Module](https://www.amazon.com/gp/product/B01DKC2GQ0)  Unfortunately, this sensor has not been added to the c-code and is slated for a future release.  The impetus to include this is to monitor humidity in the garage.  The real need is to monitor temperature, which is being done with the DS18B20, so this is an add-on slated for future implementation.  See the [RefLinks.md](https://github.com/floppydisk525/RaspPiTempFan/blob/progcomb/cprog/Ref/RefLinks.md) for DHT11 info.  
![alt text][DHT11]

### 10 Inch Shutter Exhaust Fan
[Iliving 10 Inch Variable Speed Shutter Exhaust Fan, Wall-Mounted](https://www.amazon.com/gp/product/B01G8I7HVC)

![alt text][fan]

### KB Electronics Variable Speed AC Motor Control
[KB Electronics 8811012 Solid State Variable Speed AC Electric Motor Control, 5.0 Max amp, 115V, # K177-1005](https://www.amazon.com/gp/product/B000F9DAL2)
![alt text][speedctrl]

[3_3vdc]: /Images/Icstation%203V%201%20Channel%20Relay%20Power.PNG "3.3Vdc Input 120/10A Output Relay Solid State Relay"
[DS18B20]: /Images/Aideepen%205pcs%20DS18B20.PNG "Aideepen 5pcs DS18B20 Waterproof Temperature Sensor"
[DHT11]: /Images/HiLetgo%205pcs%20DHT11%20Temperature.PNG "HiLetgo 5pcs DHT11 Temperature and Humidity Sensor Module"
[RaspPi]: /Images/rasppi.jpg "Raspberry Pi Image"
[speedctrl]: /Images/kbcontrol.png "KB Speed Control"
[fan]: /Images/10inchfan.png "Shutter Fan 10"
[fritz1]: /Fritz/RaspPiTempFan_1_sensor_bb.png "DS18B20 1 sensor setup GPIO 26"

## RASPBERRY PI SETUP
### Raspberry Pi
Prerequisite is to have a running Raspberry Pi with Stretch or stretch-lite installed.  I used a Raspberry Pi 3 Model B v1.2.  

uname -a
Linux autopi 4.14.34-v7+ #1110 SMP Mon Apr 16 15:18:51 BST 2018 armv7l GNU/Linux

### SETUP SSH
Not covered in detail, but you might want to setup SSH and a remote terminal to talk 'headless' to the raspberry pi.  I do this so that I don't need a separate 'workstation' with a monitor, keyboard, and mouse connected directly to the raspberry pi.  This way, I can setup the raspberry pi, plug it in and connect to it remotely from my latptop where I usually do most of my computer work. 

The internet has many examples - google how to setup SSH.
If using windows, install PUTTY.  Also, WinSCP is a useful Windows install that allows you to transfer files to and from your remote raspberry pi or any linux OS computer for that matter. 

Lastly, to find your RaspPi on your network, a nice tool is [Angry IP Scanner](https://angryip.org/) that allows you to scan your network (ie 192.168.0.0 to 192.168.0.255) to find any computers connected to the network.  

### SETUP STATIC IP ADDRESS
I never used to setup static IP Addresses on linux boxes, but it makes sense so that you don't have to run Angry IP Scanner to find the IP address or login in directly to the computer to find the IP Address.  The latter is problematic because because it means I need to take the RaspPi to a monitor, keyboard, and mouse and boot it.  Too much futzing around...

Here are 2 resources for setting up static IP Addresses:  
http://www.circuitbasics.com/how-to-set-up-a-static-ip-on-the-raspberry-pi/  
https://www.modmypi.com/blog/how-to-give-your-raspberry-pi-a-static-ip-address-update

#### PRO TIP - Create a file backup before modifying important files
Example below referenced from [here](http://ubuntuhandbook.org/index.php/2014/01/boot-into-text-console-ubuntu-linux-14-04/).  
sudo cp -n /etc/dhcpcd.conf /etc/dhcpcd.conf.orig

If for some reason you want to revert to original settings, just run the command below in terminal:  
sudo mv /etc/dhcpcd.conf.orig /etc/dhcpcd.conf

#### Static IP Address Setup Tips/Notes
The [example at circuit basics](http://www.circuitbasics.com/how-to-set-up-a-static-ip-on-the-raspberry-pi/) shows how to find your  default gateway IP with:  
route -ne

And, the IP addresses of your domain name with:  
cat /etc/resolv.conf

Then, modify your dhcpcd.conf  (make backup as shown above)  
sudo nano /etc/dhcpcd.conf

Then, reboot:  
sudo reboot

Use Putty to ssh into the new static IP address.  I made sure to setup my wifi (wlan0) ip address to a static address so I could work on my rasppi anywhere in the house where it was plugged in and could reach the wifi.  

## TEMPERATURE TEST AND SETUP FOR DS18B20
Started off by following this [tutorial](https://circuitdigest.com/microcontroller-projects/raspberry-pi-ds18b20-temperature-sensor-interfacing).  I checked the data sheet for the DS18B20 (References Folder) and it can accept 3.3vdc or 5vdc input.    

### Wire sensor to the Rasperry Pi by adapting from Blinkio  
![alt text][fritz1]  

### Setup of the one-wire interface
There are a few tutorials easily found on the web to interface a Raspberry Pi to the DS18B20 temperature sensor.  But, usually they simply tell you how to connect, but may or (usually) may not tell you what you're doing behind the scenes.  A little googling found a few resources, of which have been cut and paste into text files and saved in the References Folder:  
https://www.kernel.org/doc/Documentation/w1/w1.generic  
https://jumpnowtek.com/rpi/Using-DS18B20-1-wire-Temp-Sensors-with-the-Raspberry-Pi.html  

Using this [example](https://pinout.xyz/pinout/1_wire ), here are the following steps:  
#### Let's backup config.txt
sudo cp -n /boot/config.txt /boot/config.txt.orig  

If for some reason you want to revert to original settings, just run the command below in terminal:  
sudo mv /boot/config.txt.orig /boot/config.txt  

#### Update  /boot/config.txt:  
Update the config.txt file by opening the file:  
sudo nano /boot/config.txt  

##### Add the following line:  
dtoverlay=w1-gpio,gpiopin=26  
The gpiopin changes the dtoverlay from default gpio 4 to gpio 26.  I'm using gpio 26 because gpio 4, gpio 5, and gpio 6 are used from blinkio.  If you want to use the default gpio pin 4, do not use the gpiopin=26 and only add the following line:  
dtoverlay=w1-gpio  

#### Reboot  
sudo reboot  

#### Check for Connected Devices
cd /sys/bus/w1/devices  
ls  
Response: 28-01143c62f7aa  28-01143fb265aa  28-01143fc657aa  w1_bus_master1  

#### Get the Sensor ID
Each sensor has a unique ID beginning with 28-00000xxxxxxx.  Check the temperature by issuing the following commands:  
cd 28-00000xxxxxxx  
ls  
cat w1_slave  

#### The Output looks like...  
(Display output here...)

## LET'S CODE
In order to expedite code, I borrowed the base code from https://github.com/floppydisk525/blinkio.  I already had a breadboard setup for an input, output LED, and heart beat timer.  Seems logical to use those items as a precursor for this project.  Ultimatley, I would like to have an input button to act as an override from the Raspberry Pi control and turn it on manually.  

I renamed the code to tempio.c (from blinkio.c) and placed it in the cprog folder of this project.  Also, I updated the header text to indicate the change in the tempio.c file.  

Note that GPIO is setup the following way:  
GPIO 4 - pushbutton input  
GPIO 5 - Output LED triggered by Pushbutton  
GPIO 6 - HeartBeat  

Let's start by customizing the new tempio.c program for temperature setup.  Before we do that, we need to be able to compile and run the program.  

### Compile and Run c program tempio.c  
Compile the c program tempio.c:  
gcc -o tempio -l rt tempio.c -l bcm2835  

Run the c program:  
sudo ./tempio  

### Program Additions for Temperature  
We need to add a few methods for reading the temperature and are utilizing code from [Brad's c-program](http://bradsrpi.blogspot.com/2013/12/c-program-to-read-temperature-from-1.html), and saved that program for reference in the file Temp1sensor.c in the cprog/Ref folder.  

The additions include:  
Global Variables for Reading Temperature  
DS18Setup () Method to setup 1-wire devices  
DS18ReadCheckTime () Method that checks the temperature in a pre-defined interval in seconds
DS18ReadTemp () Read the 1-wire DS18B20 device.  

The main program calls the DS18ReadCheckTime () method and when the desired number of seconds elapse, the method calls DS18ReadTemp() method to actually read the one-wire device.  
### RELEASES
Release v0.01 tests the addition of the read temperature code and the second timer method, but doesn't actually read the temperature.  
Release v0.02 adds the reading of temperature for qty 1 1-wire device. 

### FUTURE RELEASE STRATEGY  
Release v0.1 to adds multiple 1-wire devices.  
Release v0.1.1 to add storing values in sqllite DB.  
Release v0.2 to add simple/basic fan control based on outside and garage temperature
Release v0.x pthread to remove lagginess from push button (This is nice to have for this project).  See [RefLinks.md](https://github.com/floppydisk525/RaspPiTempFan/blob/progcomb/cprog/Ref/RefLinks.md) file for web links to example.  
Release v0.x DHT11 sensor connectivity.  Currently not implemented.  
Release v0.x PID control.  

## RESOURCES  
### Links to more resources (because you always need more)  
[RefLinks.md](https://github.com/floppydisk525/RaspPiTempFan/blob/progcomb/cprog/Ref/RefLinks.md)  

### Links to Reference DS18B20 Sensor Projects   
https://circuitdigest.com/microcontroller-projects/raspberry-pi-ds18b20-temperature-sensor-interfacing  
http://bradsrpi.blogspot.com/2013/12/c-program-to-read-temperature-from-1.html  
https://www.raspberrypi.org/forums/viewtopic.php?t=70709  
http://www.circuitbasics.com/raspberry-pi-ds18b20-temperature-sensor-tutorial/  
https://www.raspberrypi-spy.co.uk/2013/03/raspberry-pi-1-wire-digital-thermometer-sensor/  
https://pinout.xyz/pinout/1_wire  

### Links to C code examples  
http://bradsrpi.blogspot.com/2013/12/c-program-to-read-temperature-from-1.html  
http://bradsrpi.blogspot.com/2014/06/c-program-to-read-multiple-ds18b20-1.html  

### Links to Git Markdown to help w/ this README.md page
https://guides.github.com/features/mastering-markdown/  
https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet#blockquotes  

### Links to Git committing for us git dummies
https://stackoverflow.com/questions/9329244/how-to-commit-and-push-all-changes-including-deletes    
Good Git tutorial I stumbled across:  
https://medium.freecodecamp.org/the-essential-git-handbook-a1cf77ed11b5   

### PID example  
https://nicisdigital.wordpress.com/2011/06/27/proportional-integral-derivative-pid-controller/   

