#include "lcdlight.h"

LCDLight::LCDLight()
{
    wiringPiSetup();
    softPwmCreate(pwm_pin,0,100);
}

void LCDLight::open(int lightValue)
{
    if(lightValue>100)
    {
        lightValue = 100;
    }
    if(lightValue<0)
    {
        lightValue = 0;
    }
    softPwmWrite(pwm_pin,lightValue);
}
