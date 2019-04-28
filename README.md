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

### HiLetgo DHT11 Temperature and Humidity Sensor Module (Optional)
[HiLetgo 5pcs DHT11 Temperature and Humidity Sensor Module](https://www.amazon.com/gp/product/B01DKC2GQ0)
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

## TEMP TEST AND SETUP FOR DS18B20
Started off by following this [tutorial](https://circuitdigest.com/microcontroller-projects/raspberry-pi-ds18b20-temperature-sensor-interfacing).  

In order to expedite code, I borrowed the base code from https://github.com/floppydisk525/blinkio.  I already had a breadboard setup for an input, output LED, and heart beat timer.  Seems logical to use those items as a precursor for this project.  Ultimatley, I would like to have an input button to act as an override from the Raspberry Pi control and turn it on manually, if say the wife decides that's what needs to be done:-).  

I renamed the code to tempio.c (from blinkio.c) and placed it in the cprog folder of this project.  Also, I updated the header text to indicate the change in the tempio.c file.  

Note that GPIO is setup the following way:  
GPIO 4 - pushbutton input  
GPIO 5 - Output LED triggered by Pushbutton  
GPIO 6 - HeartBeat  

Let's start by customizing the new tempio.c program for temperature setup.  


## RESOURCES
### Links to Reference Sensor Projects  
https://circuitdigest.com/microcontroller-projects/raspberry-pi-ds18b20-temperature-sensor-interfacing
http://bradsrpi.blogspot.com/2013/12/c-program-to-read-temperature-from-1.html
https://www.raspberrypi.org/forums/viewtopic.php?t=70709
http://www.circuitbasics.com/raspberry-pi-ds18b20-temperature-sensor-tutorial/

### Links to Git Markdown to help w/ this README.md page
https://guides.github.com/features/mastering-markdown/
https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet#blockquotes

### Links to Git committing for us git dummies
https://stackoverflow.com/questions/9329244/how-to-commit-and-push-all-changes-including-deletes

