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
