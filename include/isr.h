#ifndef ISR_H
#define ISR_H

#include "prototypes.h"

// extern unsigned long TimeCheck;
// extern unsigned long TimeOutConnect;
// extern unsigned long TimeOut;

// extern ESP_32 MyESP32;
// extern hw_timer_t *timer;

void My_Timer();

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
    CountTime += 1;

    // if ((CountTime % 2) == 0)
    // {
    //     digitalWrite(LED_BORD, LOW);
    // }
    // else
    // {
    //     digitalWrite(LED_BORD, HIGH);
    // }

    // verifica a conexão wifi
    if (TimeCheck)
    {
        TimeCheck -= 1;

        if (TimeCheck == 0)
        {
            MyESP32.CheckConnection = true;
            TimeCheck = TIME_CHECK_CONNECTION;
        }
    }
}
#endif