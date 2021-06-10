#define LUX_SENSOR A7
#define CODER A6
#define DOOR_MAIN 12 // After R1 relay to switch off the motor
#define DOOR_R1 11   // Inverser relay with + on NC and - on NO
#define DOOR_R2 10   // Inverser relay with - on NC and + on NO
#define RADIO 9
#define LOCKER 8

#define OPEN HIGH
#define CLOSE LOW
#define ON LOW
#define OFF HIGH

// The time of arduino sleeping in ms
#define ACTIVE 20
#define STANDBY 20000 // 3min = 36000 or 15min = 900000
#define WAITING_TIME_BEFORE_CLOSE 10 // in minutes
#define WAITING_TIME_LOCKER 1000 // 1 sec before do something else to be sure the magnet is free
#define TURN_NBR 5    // Number of turn to open or close the door

bool isDoorOpen;
bool isInMoving;
bool toOpen;
unsigned long time;
unsigned long sleepTime;
unsigned int turn; // contain the current nomber of turn of the door motor
bool turnCounted;  // true if this turn is already counted

void prln(String str)
{
  Serial.println(str);
}

void pr(String str)
{
  Serial.print(str);
}

// Manage opening door
void openDoor()
{
  radioOn();
  freeDoor();
  digitalWrite(DOOR_R1, CLOSE);
  digitalWrite(DOOR_R2, CLOSE);
  digitalWrite(DOOR_MAIN, CLOSE);

  toOpen = true;
  isInMoving = true;

  sleepTime = ACTIVE;

  prln("Start Open");
}

// Manage door stop moving
void isStopNeeded()
{
  if (isInMoving && (turn >= TURN_NBR))
  {
    digitalWrite(DOOR_MAIN, OPEN);
    digitalWrite(DOOR_R1, OPEN);
    digitalWrite(DOOR_R2, OPEN);
    isInMoving = false;
    turn = 0;

    if (toOpen)
    {
      prln("Stop - Door Open");
      isDoorOpen = true;
    }
    else
    {
      prln("Stop - Door Close");
      isDoorOpen = false;
      radioOff();
      lockDoor();
    }

    sleepTime = STANDBY;
  }
}

// Manage closing door
// return the sleep value for arduino
void closeDoor()
{
  digitalWrite(DOOR_R1, OPEN);
  digitalWrite(DOOR_R2, OPEN);
  digitalWrite(DOOR_MAIN, CLOSE);

  toOpen = false;
  isInMoving = true;

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

void lockDoor() {
  digitalWrite(LOCKER, ON);
  prln("Door locked");
}

void freeDoor() {
  digitalWrite(LOCKER, OFF);
  delay(WAITING_TIME_LOCKER);
  prln("Door unlocked");
}

void manageTimeBeforeCloseDoor() {
  unsigned long waitingTimeBeforeCloseInSec = WAITING_TIME_BEFORE_CLOSE * 60;
  for (int i = 0; i < waitingTimeBeforeCloseInSec; i++) {
    delay(1000); //wait 1 second
  }

}

void setup() {
  Serial.begin(115200);
  pinMode(DOOR_R1, OUTPUT);
  pinMode(DOOR_R2, OUTPUT);
  pinMode(DOOR_MAIN, OUTPUT);
  pinMode(RADIO, OUTPUT);
  pinMode(LOCKER, OUTPUT);
  pinMode(CODER, INPUT);

  digitalWrite(DOOR_MAIN, OPEN);
  digitalWrite(DOOR_R1, OPEN);
  digitalWrite(DOOR_R2, OPEN);
  digitalWrite(RADIO, OFF);
  digitalWrite(LOCKER, OFF);

  isInMoving = false;
  sleepTime = STANDBY;
  toOpen = false;
  turnCounted = true;
  turn = 0;

  int lux = analogRead(LUX_SENSOR);
  // Check lux value to init the door state
  if (lux > 900) {
    isDoorOpen = false;
    radioOff();
  }
  else if (lux < 500) {
    isDoorOpen = true;
    radioOn();
  }

}

void loop() {

  int lux = analogRead(LUX_SENSOR);
  if (lux > 900 && isDoorOpen && !isInMoving) {
    manageTimeBeforeCloseDoor();
    closeDoor();
  }
  else if (lux < 500 && !isDoorOpen && !isInMoving) {
    openDoor();
  }
  if (isInMoving) {
    counter();
  }

  isStopNeeded();

  delay(sleepTime);
}
