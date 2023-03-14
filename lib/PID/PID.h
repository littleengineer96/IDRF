#ifndef _PID_H_
#define _PID_H_

#include <Arduino.h>
#include "PIDdefs.h"

// #include "PID_.h"

/*working variables*/

class PID
{
private:
    unsigned long lastTime;
    double errSum, lastErr, lastInput;
    double P, I, D;
    double Kp, Ki, Kd;
    double Ku, Pu;

    uint8_t pinPWM, channelPWM, resolutionPWM;
    uint32_t frequenciaPWM;
    unsigned long rangeAtuador; // valor maximo do atuador
    double amplitudeMax;        // amplitude maxima na obtenção de parametros
    // double Kp;

public:
    //intervalo de tempo entre as computaçoes
    unsigned long regularTime;

    //faz o controle da computação
    unsigned long timeCompute;

    double Input, Output, Setpoint;

    writeOutput write;
    readInput read;
    void begin(uint8_t pin, uint8_t channel, uint32_t frequencia, uint8_t resolution);
    // void setInput(double *value);
    // void setOutput(double *value);
    // void setSetpoint(double *value);
    void setAmplitudeMax(double value);
    void setKp(double value);
    void setKi(double value);
    void setKd(double value);
    void setKu(double value);
    void setPu(double value);
    void setPinPWM(uint8_t value);
    void setChannelPWM(uint8_t value);
    void setResolutionPWM(uint8_t value);
    void setFrequenciaPWM(uint32_t value);
    void setRangeAtuador(uint32_t value);

    // double getInput();
    // double getOutput();
    // double getSetpoint();
    double getAmplitudeMax();
    double getKp();
    double getKi();
    double getKd();
    double getKu();
    double getPu();
    uint8_t getPinPWM();
    uint8_t getChannelPWM();
    uint8_t getResolutionPWM();
    uint32_t getFrequenciaPWM();
    uint32_t getRangeAtuador();

    void riseParameters();
    void Compute();

    // construtor
    PID(double a, double b, double c);
    ~PID();
};

#endif