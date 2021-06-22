#include <EEPROM.h>

#define LUX_SENSOR A7
#define CODER A6
#define INTER A1
#define DOOR_IN1 3   // IN3 on H Bridge board
#define DOOR_IN2 4   // IN4 on H Bridge board
#define RADIO 12
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
}DoorRequest;

// The time of arduino sleeping in ms
#define ACTIVE 20
#define STANDBY 20000 // 3min = 36000 or 15min = 900000
#define WAITING_TIME_BEFORE_CLOSE 10 // in minutes
#define WAITING_TIME_LOCKER 1000 // 1 sec before do something else to be sure the magnet is free
#define TURN_NBR 20    // Number of turn to open or close the door

unsigned long time;
unsigned long sleepTime;
unsigned int turn; // contain the current nomber of turn of the door motor
bool turnCounted;  // true if this turn is already counted
DoorState doorState;

void prln(String str)
{
  Serial.println(str);
}

void pr(String str)
{
  Serial.print(str);
}

void manageDoor(DoorRequest requestedState) {
  switch (requestedState) {
  case eOpen:
    digitalWrite(DOOR_IN1, HIGH);
    digitalWrite(DOOR_IN2, LOW);
    doorState = eOpenning;
    break;
  case eClose:
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
  digitalWrite(DOOR_IN1, CLOSE);
  manageDoor(eOpen);

  sleepTime = ACTIVE;

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

    sleepTime = STANDBY;
  }
}

// Manage closing door
// return the sleep value for arduino
void doorClose()
{
  manageDoor(eOpen);

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

bool isButton() {
  if (analogRead(INTER) > 900) {
    return true;
  }
  return false;
}

void manageTimeBeforeCloseDoor() {
  unsigned long waitingTimeBeforeCloseInSec = WAITING_TIME_BEFORE_CLOSE * 60;
  for (int i = 0; i < waitingTimeBeforeCloseInSec; i++) {
    delay(1000); //wait 1 second
  }

}

void setup() {
  Serial.begin(115200);
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

  int lux = analogRead(LUX_SENSOR);
  // Check lux value to init the door state
  if (lux > 900) {
    radioOff();
  }
  else if (lux < 500) {
    radioOn();
  }

}

void loop() {

  int lux = analogRead(LUX_SENSOR);

  if ((lux > 900 || isButton()) && ((doorState == eOpened) || (doorState == eUnknown))) {
    if (!isButton())
      manageTimeBeforeCloseDoor();
    doorClose();
  }
  else if ((lux < 500 || isButton()) && ((doorState == eClosed) || (doorState == eUnknown))) {
    doorOpen();
  }

  if ((doorState == eOpenning) || (doorState == eClosing)) {
    counter();
  }

  isStopNeeded();

  delay(sleepTime);
}
