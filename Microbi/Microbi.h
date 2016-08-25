#ifndef Microbi_h
#define Microbi_h
#include "Arduino.h"

// {{{ Setup debug printing macros.
#define DBG_DEVICE Serial
#ifdef MICROBI_DEBUG
  #define DBG_PRINT(...)   { DBG_DEVICE.print(__VA_ARGS__); }
  #define DBG_PRINTLN(...) { DBG_DEVICE.println(__VA_ARGS__); }
#else
  #define DBG_PRINT(...) {}
  #define DBG_PRINTLN(...) {}
#endif
// }}}

#define FORWARD  90
#define BACKWARD 104 // 360-256
#define LEFT     0
#define RIGHT    180

// {{{ class MicrobiEngine
class MicrobiEngine
{
  public:
    
    MicrobiEngine();
    MicrobiEngine(uint8_t pinPWM, uint8_t pinIn1, uint8_t pinIn2);
    void init();
    void init(uint8_t pinPWM, uint8_t pinIn1, uint8_t pinIn2);
    void forward();
    void backward();
    void stop();
    void go(uint8_t direction, uint8_t speed);
    
  private:
    uint8_t _pinPWM;
    uint8_t _pinIn1;
    uint8_t _pinIn2;
    
};
// }}}

// {{{ class MicrobiDrive
class MicrobiDrive
{
  public:
    MicrobiDrive(uint8_t engine1[], uint8_t engine2[]);
    //void init();
    void rotate(uint8_t direction);
    void rotate(uint8_t direction, uint8_t speed, int t);
    void forward();
    void forward(int t);
    void backward();
    void backward(int t);
    void turnLeft();
    void turnRight();
    void turnAround();
    void stop();
    
  private:
    MicrobiEngine* _engine1;
    MicrobiEngine* _engine2;
    
};
// }}}

#endif
