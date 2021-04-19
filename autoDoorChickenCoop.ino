#define LUX_SENSOR A7
#define DOOR_MAIN 2 // After R1 relay to switch off the motor
#define DOOR_R1 3   // Inverser relay with + on NC and - on NO
#define DOOR_R2 4   // Inverser relay with - on NC and + on NO
#define RADIO 5

#define OPEN HIGH
#define CLOSE LOW

// The time of arduino sleeping in ms
#define ACTIVE 500
#define STANDBY 500 // 15min = 900000
#define DOOR_MOVE_TIME 86000

bool isDoorOpen;
bool isInMoving;
bool toOpen;
unsigned long time;
unsigned long startDoorMovingTime;
unsigned long sleepTime;

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
  startDoorMovingTime = millis();
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
  if (isInMoving && ((millis() - startDoorMovingTime) >= DOOR_MOVE_TIME))
  {
    digitalWrite(DOOR_MAIN, OPEN);
    digitalWrite(DOOR_R1, OPEN);
    digitalWrite(DOOR_R2, OPEN);
    isInMoving = false;

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
    }
  }

  sleepTime = STANDBY;
}

// Manage closing door
// return the sleep value for arduino
void closeDoor()
{
  startDoorMovingTime = millis();
  digitalWrite(DOOR_R1, OPEN);
  digitalWrite(DOOR_R2, OPEN);
  digitalWrite(DOOR_MAIN, CLOSE);

  toOpen = false;
  isInMoving = true;

  sleepTime = ACTIVE;

  prln("Start Close");
}

void radioOn()
{
  digitalWrite(RADIO, CLOSE);
  prln("Radio On");
}

void radioOff()
{
  digitalWrite(RADIO, OPEN);
  prln("Radio Off");
}

void setup()
{
  Serial.begin(115200);
  pinMode(DOOR_R1, OUTPUT);
  pinMode(DOOR_R2, OUTPUT);
  pinMode(DOOR_MAIN, OUTPUT);
  pinMode(RADIO, OUTPUT);

  digitalWrite(DOOR_MAIN, OPEN);
  digitalWrite(DOOR_R1, OPEN);
  digitalWrite(DOOR_R2, OPEN);
  digitalWrite(RADIO, OPEN);

  isInMoving = false;
  sleepTime = STANDBY;
  toOpen = false;

  int lux = analogRead(LUX_SENSOR);
  // Check lux value to init the door state
  if (lux > 900)
  {
    isDoorOpen = false;
    radioOff();
  }
  else if (lux < 500)
  {
    isDoorOpen = true;
    radioOn();
  }

  startDoorMovingTime = millis();
}

void loop()
{

  int lux = analogRead(LUX_SENSOR);
  if (lux > 900 && isDoorOpen && !isInMoving)
    closeDoor();
  else if (lux < 500 && !isDoorOpen && !isInMoving)
    openDoor();

  isStopNeeded();

  delay(sleepTime);
}
