#include <EEPROM.h>

#define LUX_SENSOR A7
#define CODER A6
#define INTER A1
#define DOOR_IN1 4   // IN3 on H Bridge board
#define DOOR_IN2 3   // IN4 on H Bridge board
#define RADIO 8
#define LOCKER 11

#define OPEN HIGH
#define CLOSE LOW
#define ON LOW
#define OFF HIGH

typedef enum eDoorState {
  eOpenning = 0,
  eOpened,
  eClosing,
  eClosed,
  eUnknown,
}DoorState;

typedef enum eDoorRequest {
  eOpen = 0,
  eClose,
  eStop,
  eNo,
}DoorRequest;

// The time of arduino sleeping in ms
#define ACTIVE 20
#define STANDBY 100 //20000 // 3min = 36000 or 15min = 900000
#define WAITING_TIME_BEFORE_CLOSE 10 // in minutes
#define WAITING_TIME_LOCKER 1000 // 1 sec before do something else to be sure the magnet is free
#define TURN_NBR 45    // Number of turn to open or close the door

unsigned long time;
unsigned long sleepTime;
unsigned int turn; // contain the current nomber of turn of the door motor
bool turnCounted;  // true if this turn is already counted
DoorState doorState;
int eepromDoorStateAddr = 0;

void prln(String str)
{
  Serial.println(str);
}

void pr(String str)
{
  Serial.print(str);
}

void eepromUpdateDoorState() {
  EEPROM.write(eepromDoorStateAddr, doorState);
}

DoorState eepromGetDoorState() {
  return (DoorState)EEPROM.read(eepromDoorStateAddr);
}

void manageDoor(DoorRequest requestedState) {
  switch (requestedState) {
  case eOpen:
    Serial.println("Openning Door");
    digitalWrite(DOOR_IN1, HIGH);
    digitalWrite(DOOR_IN2, LOW);
    doorState = eOpenning;
    break;
  case eClose:
    Serial.println("Closing Door");
    digitalWrite(DOOR_IN1, LOW);
    digitalWrite(DOOR_IN2, HIGH);
    doorState = eClosing;
    break;
  default:
    digitalWrite(DOOR_IN1, LOW);
    digitalWrite(DOOR_IN2, LOW);
  }
}

// Manage opening door
void doorOpen()
{
  radioOn();
  doorFree();
  manageDoor(eOpen);

  sleepTime = ACTIVE;

  eepromUpdateDoorState();
  prln("Start Open");
}

// Manage door stop moving
void isStopNeeded()
{
  if (((doorState == eOpenning) || (doorState == eClosing)) && (turn >= TURN_NBR))
  {
    manageDoor(eStop);
    turn = 0;

    if (doorState == eOpenning) {
      prln("Stop - Door Open");
      doorState = eOpened;
    }
    else {
      doorState = eClosed;
      prln("Stop - Door Close");
      radioOff();
      doorLock();
    }

    eepromUpdateDoorState();
    sleepTime = STANDBY;
  }
}

// Manage closing door
// return the sleep value for arduino
void doorClose()
{
  manageDoor(eClose);

  sleepTime = ACTIVE;
  prln("Start Close");
}

void counter() {
  if ((analogRead(CODER) > 10) && !turnCounted) {
    turn++;
    turnCounted = true;
    pr("Number of turn = ");
    Serial.println(turn);
  }
  else if ((analogRead(CODER) < 10) && turnCounted) {
    turnCounted = false;
  }
}

void radioOn()
{
  digitalWrite(RADIO, ON);
  prln("Radio On");
}

void radioOff()
{
  digitalWrite(RADIO, OFF);
  prln("Radio Off");
}

void doorLock() {
  digitalWrite(LOCKER, ON);
  prln("Door locked");
}

void doorFree() {
  digitalWrite(LOCKER, OFF);
  delay(WAITING_TIME_LOCKER);
  prln("Door unlocked");
}

void manageTimeBeforeCloseDoor() {
  unsigned long waitingTimeBeforeCloseInSec = WAITING_TIME_BEFORE_CLOSE;
  for (int i = 0; i < waitingTimeBeforeCloseInSec; i++) {
    delay(60000); //wait 60 seconds
  }

}

unsigned long timeLux;
bool luxHighDetected, luxLowDetected;
DoorRequest luxFilter(int lux) {
  if (lux > 900 && !luxHighDetected) {
    timeLux = millis();
    luxHighDetected = true;
    luxLowDetected = false;
    Serial.println("lux ot CLOSE Detected");
  }
  else if ((millis() - timeLux) > 10000 && luxHighDetected) {
    luxHighDetected = false;
    Serial.println("lux ot CLOSE Validat");
    return eClose;
  }

  if (lux < 500 && !luxLowDetected) {
    timeLux = millis();
    luxLowDetected = true;
    luxHighDetected = false;
    Serial.println("lux ot OPEN Detected");
  }
  else if ((millis() - timeLux) > 10000 && luxLowDetected) {
    luxLowDetected = false;
    Serial.println("lux ot OPEN Validat");
    return eOpen;
  }
  return eNo;
}

void setup() {
  Serial.begin(115200);
  for (int i = 2; i < 12;i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  doorState = eUnknown;
  pinMode(DOOR_IN1, OUTPUT);
  pinMode(DOOR_IN2, OUTPUT);
  pinMode(RADIO, OUTPUT);
  pinMode(LOCKER, OUTPUT);

  digitalWrite(RADIO, OFF);
  digitalWrite(LOCKER, OFF);

  manageDoor(eStop);

  sleepTime = STANDBY;
  turnCounted = true;
  turn = 0;

  doorState = eepromGetDoorState();
  Serial.println(doorState);
}

void loop() {

  int lux = analogRead(LUX_SENSOR);

  DoorRequest req = luxFilter(lux);

  if (req == eClose && ((doorState == eOpened) || (doorState == eUnknown))) {
    //manageTimeBeforeCloseDoor();
    doorClose();
  }
  else if (req == eOpen && ((doorState == eClosed) || (doorState == eUnknown))) {
    doorOpen();
  }

  if ((doorState == eOpenning) || (doorState == eClosing)) {
    counter();
  }

  isStopNeeded();

  delay(sleepTime);
}
