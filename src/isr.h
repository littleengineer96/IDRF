#ifndef ISR_H
#define ISR_H

#include <Arduino.h>
#include <prototypes.h>

extern unsigned long TimeCheck;
extern unsigned long TimeOutConnect;
extern unsigned long TimeOut;

extern ESP_32 MyESP32;
extern hw_timer_t *timer;

void Start_Timer(uint64_t time_in_us)
{
    /*
    https://www.dobitaobyte.com.br/isr-interrupcoes-e-timer-com-esp32/
    */
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &My_Timer, true);
    timerAlarmWrite(timer, time_in_us, true);
    // timerAlarmWrite(timer, 1000000, true);
    // timerAlarmWrite(timer, TIME_ISR, true);
    timerAlarmEnable(timer);
}

void My_Timer()
{
    if (TimeCheck)
    {
        TimeCheck -= 1;
        if (!TimeCheck)
        {
            MyESP32.CheckConnection = true;
        }
    }

    if (TimeOutConnect)
    {
        TimeOutConnect -= 1;
    }

    if (TimeOut)
    {
        TimeOut -= 1;
    }
}
#endif