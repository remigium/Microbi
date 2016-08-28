//Center Servos
#include <Servo.h>

// pin 13 is bound with LED

// SERVO
#define PIN_SERVO 12

// HC-04
#define PIN_TRIG 7    //Trig yellow
#define PIN_ECHO 2    //Echo dark blue; it is IRQ 0


// L298N
#define PIN_APWM 6
#define PIN_AIN1 10
#define PIN_AIN2 11

#define PIN_BPWM 5
#define PIN_BIN1 8
#define PIN_BIN2 9

// constants
#define LEFT 180
#define AHEAD 90
#define RIGHT 0

#define FORWARD  1
#define BACKWARD 2

// engines
int engineA[] = {PIN_APWM, PIN_AIN1, PIN_AIN2};
int engineB[] = {PIN_BPWM, PIN_BIN1, PIN_BIN2};

// servo
Servo servo;
int pos = 0;
void setupServo() {
  Serial.println("initialize servo");
  servo.attach(PIN_SERVO);
  
  servo.write(LEFT);
  delay(300);
  servo.write(RIGHT);
  delay(300);
  servo.write(AHEAD);
  delay(300);
}


void setupUltraSensor() {
  Serial.println("initialize distance sensor");
  pinMode(PIN_TRIG, OUTPUT); //Define outputs
  pinMode(PIN_ECHO, INPUT);  //Define inputs  
}


void initializeEngine(int engine[]) {
  Serial.println("initialize engine");
  for (int i=0; i<3; i++) {
     pinMode(engine[i], OUTPUT);
  }
}

void setup()
{

  setupUltraSensor();
  setupServo(); 
  initializeEngine(engineA);
  initializeEngine(engineB);

  Serial.begin(9600);
  Serial.println("setup - end");
}

float getDistance() {
  // The sensor will going to triggered by a HIGH pulse of 10 or more microseconds.
  // Provide a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(15); // was 10
  digitalWrite(PIN_TRIG, LOW);

  pinMode(PIN_ECHO, INPUT);
  int duration = pulseIn(PIN_ECHO, HIGH);
  String msg = "duration:";
  Serial.println(msg + duration);

  // distance is sound speed * duration
  // sound speed is 340 m/s or 29 microseconds per centimeter - 34cm/ms - 0.034cm/us
  return 0.034*(duration/2);
}

void stopEngine(int engine[]) {
  Serial.println("stop engine");
  for (int i = 1; i<3; i++) {
    digitalWrite(engine[i], LOW);
  }
}

void stopEngines() {
  stopEngine(engineA);
  stopEngine(engineB);
}

void engineGo(int engine[], int direction, int speed=255) {
  
  switch (direction) {
    case FORWARD:
      Serial.println("start engine forward");
      digitalWrite(engine[1], LOW);
      digitalWrite(engine[2], HIGH);
      break;
    case BACKWARD:
      Serial.println("start engine backward");
      digitalWrite(engine[1], HIGH);
      digitalWrite(engine[2], LOW);
      break;
  }
  analogWrite(engine[0], speed);
}


// t=600 gives rotation around 90 degrees
void rotate(int direction, int speed=250, int t=600)
{
  Serial.println("rotate");
  switch(direction) {
    case LEFT:
      engineGo(engineA, BACKWARD, speed);
      engineGo(engineB, FORWARD,  speed);
      break;
    case RIGHT:
      engineGo(engineA, FORWARD,  speed);
      engineGo(engineB, BACKWARD, speed);
      break;
  }
  delay(t);
  stopEngines();
}

void goForward (int t=1200)
{
  engineGo(engineA, FORWARD);
  engineGo(engineB, FORWARD);
  delay(t);
  stopEngines();
}

void goBackward (int t=1200)
{
  engineGo(engineA, BACKWARD);
  engineGo(engineB, BACKWARD);
  delay(t);
  stopEngines();
}

void turnAround() {
  rotate(LEFT, 250, 1200);
}

void loop()
{
  int t = 300;
  
  // read the distance
  float cm = 0;
  cm = getDistance();

  // if the distance is satisfactory move forward
  if (cm > 3) {
    Serial.println("move forward"); 
    goForward(300);
  // if the distance is unsatisfactory 
  } else {
  //   look left
    servo.write(LEFT);
    delay(t);

    cm = getDistance();
    //   if the distance is satisfactory
    if (cm > 3) {
      //     rotate left a bit
      Serial.println("rotate left a bit"); 
      rotate(LEFT);
    //   else
    } else {
      //     look right
      servo.write(RIGHT);
      delay(t);

      cm = getDistance();
      //     if the distance is satisfactory
      if (cm > 3) {
        //       rotate right a bit
        Serial.println("rotate right a bit");
        rotate(RIGHT);
      } else {
        Serial.println("move back");
        goBackward();    
        Serial.println("turn around");
        turnAround();
      }
    }
    delay(t);
    servo.write(AHEAD);
    
  }
  delay(30);  
}
  
