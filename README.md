# RaspPiTempFan
Raspberry Pi Temperature control of a wall fan to control temperature of garage

OVERVIEW
This project uses a raspberry Pi to check the temperature inside a garage and ambient outside temperature and turn on a wall fan that will blow hot air out of the garage and suck in cool air to cool the garage.  

By having a temperature sensor outside, the program running on the raspberry pi will decide whether venting the garage makes sense.  IE, if it's hotter outside than inside, then it doesn't make sense to vent the garage.  


PARTS
Icstation 3V 1 Channel Relay Power Switch Module with Optocoupler High Level Trigger
https://www.amazon.com/gp/product/B01M0E6SQM

https://www.amazon.com/ask/questions/Tx1Y6ACZYHHM2PY/ref=ask_dp_dpmw_al_hza
The gpio pin is connected to the input of an optical coupler on the relay board. Mine drew 0.15mA from the GPIO pin and 78mA of power from the 3.3v power rail in the on state. 

https://www.amazon.com/ask/questions/Tx2I5KJHUHOQISZ/ref=ask_dp_dpmw_al_hza
My working configuration is: 
gnd = gnd pin from pi + external 3.3vdc - 
Signal = output pin from pi 
VCC = external 3.3vdc + 
Jumpers = on

Aideepen 5pcs DS18B20 Waterproof Temperature Sensor Digital Thermal Stainless Steel Tube Probe 1 Meter
https://www.amazon.com/gp/product/B01LY53CED

Output lead: 
Red- VCC
Yellow- DATA
Black-GND

HiLetgo 5pcs DHT11 Temperature and Humidity Sensor Module
https://www.amazon.com/gp/product/B01DKC2GQ0


