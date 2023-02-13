#ifndef LCDLIGHT_H
#define LCDLIGHT_H
#include "wiringPi.h"
#include "softPwm.h"

#define pwm_pin 1 //使用gpio1引脚

class LCDLight
{
public:
    LCDLight();
    void open(int lightValue);
};

#endif // LCDLIGHT_H
