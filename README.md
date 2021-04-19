# Automatique chicken coop door

## Build of material

|Image|Description and link|Quantity|Price
|---|---|---|---|
|![Arduino Nano](res/img/arduinoNano.png)|[Arduino Nano](https://www.aliexpress.com/item/4000430870768.html)|1|6€|
|![4 Relay 12V](res/img/relay.png)|[4 Relay Modules 12V](https://www.aliexpress.com/item/32962362750.html)|1|3€|
|![12V Power supply](res/img/powerSupply.png)|[12V 2A Power Supply](https://www.aliexpress.com/item/4001159381316.html)|1|3€|
|![Motor and reductor](res/img/motor.png)|[12V Motor + Reductor 40rpm](https://www.aliexpress.com/item/32888516547.html)|1|9€|
|![Photosensitive](res/img/photosensitive.png)|[Photosensitive 5V](https://www.aliexpress.com/item/1215447668.html)|1|1€40|
|![Speed Sensor](res/img/speedSensor.png)|[Speed Measure sensor 5V](https://www.aliexpress.com/item/1005001653555599.html)|1|2€70|
|![Disque encoder](res/img/encoder.png)|[Disque encoder](https://www.aliexpress.com/item/1005001954155530.html)|1|2€60|


#
## Conso
### 12V Power supply
Arduino = 5mA <br>
Relay = 5mA max per relay <br>
Motor = 40mA pick <br>
**Total: 65mA**
#
## Schematics
![Electronic Schematic](res/img/circuit.svg)

## Program
Just flash the .ino progam into your arduino Nano using ArduinoIDE

## Adjust the motor
Adjust the xxxx in:
```C
#define DOOR_MOVE_TIME xxxx
```
xxxx is the time to open or close the door in minisecondes