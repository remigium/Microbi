#include "Microbi.h"


// {{{ MicrobiEngine
MicrobiEngine::MicrobiEngine(uint8_t pinPWM, uint8_t pinIn1, uint8_t pinIn2)
{
  _pinPWM = pinPWM;
  _pinIn1 = pinIn1;
  _pinIn2 = pinIn2;
}

void MicrobiEngine::init(uint8_t pinPWM, uint8_t pinIn1, uint8_t pinIn2) 
{
  _pinPWM = pinPWM;
  _pinIn1 = pinIn1;
  _pinIn2 = pinIn2;
  init();
} 

void MicrobiEngine::init() 
{
  pinMode(_pinPWM, OUTPUT);
  pinMode(_pinIn1, OUTPUT);
  pinMode(_pinIn2, OUTPUT);
}

void MicrobiEngine::stop() 
{
  DBG_PRINTLN("Engine stop"); 
  digitalWrite(_pinIn1, LOW);
  digitalWrite(_pinIn2, LOW);
}

// direction according to defines above
// speed between 0 and 255
void MicrobiEngine::go(uint8_t direction, uint8_t speed) 
{
  DBG_PRINT("Engine go:"); DBG_PRINTLN(direction); 
  switch (direction) {
    case FORWARD:
      digitalWrite(_pinIn1, LOW);
      digitalWrite(_pinIn2, HIGH);
      break;
    case BACKWARD:
      digitalWrite(_pinIn1, HIGH);
      digitalWrite(_pinIn2, LOW);
      break;
  }
  analogWrite(_pinPWM, speed);

}

void MicrobiEngine::forward() 
{
  DBG_PRINTLN("Engine go forward");
  go(FORWARD, 255);
}

void MicrobiEngine::backward() 
{
  DBG_PRINTLN("Engine go backward");
  go(BACKWARD, 255);
}
// }}}



// {{{ MicrobiDrive
MicrobiDrive::MicrobiDrive(uint8_t engine1[], uint8_t engine2[])
{
  _engine1 = new MicrobiEngine(engine1[0], engine1[1], engine1[2]);
  _engine2 = new MicrobiEngine(engine2[0], engine2[1], engine2[2]);
  _engine1->init();
  _engine2->init();

}

void MicrobiDrive::stop() {
  _engine1->stop();
  _engine2->stop();
}

void MicrobiDrive::rotate(uint8_t direction)
{
  rotate(direction, 250, 600);
}

// t=600 gives rotation around 90 degrees
void MicrobiDrive::rotate(uint8_t direction, uint8_t speed, int t)
{
  DBG_PRINT("drive rotate: "); DBG_PRINTLN(direction);
  stop();
  switch(direction) {
    case LEFT:
      // here possible addition of speed
      _engine1->backward();
      _engine2->forward();
      break;
    case RIGHT:
      // here possible addition of speed
      _engine1->forward();
      _engine2->backward();
      break;
  }
  delay(t);
  stop();
}

void MicrobiDrive::turnRight() {
  rotate(RIGHT, 250, 600);
}

void MicrobiDrive::turnLeft() {
  rotate(LEFT, 250, 600);
}

void MicrobiDrive::forward ()
{
  forward(1200);
}

// forward as the only method applies not the
// stop command to the engines
void MicrobiDrive::forward (int t)
{
  DBG_PRINT("drive forward: "); DBG_PRINTLN(t);
  _engine1->forward();
  _engine2->forward();
  delay(t);
  //stop();
}

void MicrobiDrive::backward() 
{
  backward(1200);
}

void MicrobiDrive::backward(int t)
{
  stop();
  DBG_PRINT("drive backward: "); DBG_PRINTLN(t);
  _engine1->backward();
  _engine2->backward();
  delay(t);
  stop();
}

void MicrobiDrive::turnAround() {
  DBG_PRINT("drive turn: "); DBG_PRINTLN("around");
  rotate(LEFT, 250, 1200);
}

// }}}
