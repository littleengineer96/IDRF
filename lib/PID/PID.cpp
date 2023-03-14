#include "PID.h"

/*working variables*/

using namespace std;
// class PID
// {
// private:
//     unsigned long lastTime;
//     uint8_t pinPWM;
//     double *Input, *Output, *Setpoint;
//     double errSum, lastErr;
//     double P, I, D;
//     double Kp, Ki, Kd;
//     double Ku, Pu;
//     unsigned long rangeAtuador; // valor maximo do atuador
//     double amplitudeMax;        // amplitude maxima na obtenção de parametros

// public:
//     writeOutput write;

//     void begin();
//     void setInput(double *value) { Input = value; };
//     void setOutput(double *value) { Output = value; };
//     void setSetpoint(double *value) { Setpoint = value; };
//     void setRangeAtuador(unsigned long value) { rangeAtuador = value + 1; };
//     void setAmplitudeMax(double value) { amplitudeMax = value; };
//     void setKp(double value) { Kp = value; };
//     void setKi(double value) { Ki = value; };
//     void setKd(double value) { Kd = value; };
//     void setKu(double value) { Ku = value; };
//     void setPu(double value) { Pu = value; };
//     void setPinPWM(uint8_t value) { pinPWM = value; };

//     double getInput() { return *Input; };
//     double getOutput() { return *Output; };
//     double getSetpoint() { return *Setpoint; };
//     unsigned long getRangeAtuador() { return rangeAtuador; };
//     double getAmplitudeMax() { return amplitudeMax; };
//     double getKp() { return Kd; };
//     double getKi() { return Ki; };
//     double getKd() { return Kd; };
//     double getKu() { return Ku; };
//     double getPu() { return Pu; };
//     uint8_t getPinPWM() { return pinPWM; };
//     void riseParameters();
//     void Compute();

//     //construtor
//     PID(double p=1.0, double i=0.0, double d=0.0)
//     {
//         Kp = p;
//         Ki = i;
//         Kd = d;
//     };
//     ~PID();
// };

PID::PID(double a = 1.0, double b = 0.0, double c = 0.0)
// PID::PID()
{
    Kp = a;
    Ki = b;
    Kd = c;
}
PID::~PID()
{
}

void PID::begin(uint8_t pin = 17, uint8_t channel = 0, uint32_t frequencia = 1000, uint8_t resolution = 8)
{
    // write = ledcWrite;

    setPinPWM(pin);
    setChannelPWM(channel);
    setFrequenciaPWM(frequencia);
    setResolutionPWM(resolution);
    setRangeAtuador(255);

    ledcSetup(channelPWM, frequenciaPWM, resolutionPWM);
    ledcAttachPin(pinPWM, channelPWM);

    // 15V
    //  Kp = 113.16;
    //  Ki = 1.3;
    //  Kd = 2477.27;

    // 18V
    Kp = 87.16;
    Ki = 1.01;
    Kd = 1910.99;
}

// void PID::setInput(double *value) { Input = value; };
// void PID::setOutput(double *value) { Output = value; };
// void PID::setSetpoint(double *value) { Setpoint = value; };
void PID::setAmplitudeMax(double value) { amplitudeMax = value; };
void PID::setKp(double value) { Kp = value; };
void PID::setKi(double value) { Ki = value; };
void PID::setKd(double value) { Kd = value; };
void PID::setKu(double value) { Ku = value; };
void PID::setPu(double value) { Pu = value; };
void PID::setPinPWM(uint8_t value) { pinPWM = value; };
void PID::setChannelPWM(uint8_t value) { channelPWM = value; };
void PID::setResolutionPWM(uint8_t value) { resolutionPWM = value; };
void PID::setFrequenciaPWM(uint32_t value) { frequenciaPWM = value; };
void PID::setRangeAtuador(uint32_t value) { rangeAtuador = value; };

// double PID::getInput() { return *Input; };
// double PID::getOutput() { return *Output; };
// double PID::getSetpoint() { return *Setpoint; };
double PID::getAmplitudeMax() { return amplitudeMax; };
double PID::getKp() { return Kd; };
double PID::getKi() { return Ki; };
double PID::getKd() { return Kd; };
double PID::getKu() { return Ku; };
double PID::getPu() { return Pu; };

uint8_t PID::getPinPWM() { return pinPWM; };
uint8_t PID::getChannelPWM() { return channelPWM; };
uint8_t PID::getResolutionPWM() { return resolutionPWM; };
uint32_t PID::getFrequenciaPWM() { return frequenciaPWM; };
uint32_t PID::getRangeAtuador() { return rangeAtuador; };

void PID::riseParameters()
{
    /* potencia maxima encima da resistencia*/
    write(channelPWM, rangeAtuador);

    Input = read();
    while (Input <= Setpoint)
    {
        Input = read();
        Serial.println("Temp(1):" + String(Input) + " oC");
        delay(1000);
    }

    write(channelPWM, 0);
    unsigned long deltaTime = millis();

    amplitudeMax = Input;
    while (Input > Setpoint)
    {
        Input = read();
        if (Input > amplitudeMax)
        {
            amplitudeMax = Input;
        }

        Serial.println("Temp(2):" + String(Input) + " oC");
        delay(1000);
    }
    deltaTime = millis() - deltaTime;

    Pu = deltaTime / 1000.0;
    Ku = (4 * (rangeAtuador + 1)) / (PI * (amplitudeMax - Input));
    Kp = Ku / 1.7;
    Ki = Kp * 2 / Pu;
    Kd = Kp * (Pu / 8);
    Serial.println("Pu: " + String(Pu));
    Serial.println("Ku: " + String(Ku));
    Serial.println("Kp: " + String(Kp));
    Serial.println("Ki: " + String(Ki));
    Serial.println("Kd: " + String(Kd));
}

void PID::Compute()
{
    /*How long since we last calculated*/
    unsigned long now = millis();
    double deltaTime = (double)(now - lastTime) / 1000.0;

    /*Compute all the working error variables*/
    Input = read();
    // P
    double error = Setpoint - Input;

    // I
    errSum += (error * deltaTime);

    double dErr;
    if (deltaTime)
    {
        // D
        dErr = (error - lastErr) / deltaTime;
    }
    Serial.println("\n[PID Control] deltaTime: " + String(deltaTime));
    Serial.println("[PID Control] error: " + String(error));
    Serial.println("[PID Control] errSum: " + String(errSum));
    Serial.println("[PID Control] dErr: " + String(dErr));

    P = Kp * error;
    I = Ki * errSum;
    D = Kd * dErr;

    /*Compute PID Output*/
    Output = P + I + D;

    Serial.println("[PID Control] P: " + String(P));
    Serial.println("[PID Control] I: " + String(I));
    Serial.println("[PID Control] D: " + String(D));
    Serial.println("[PID Control] Output: " + String(Output));

    if (Output > 255)
    {
        Output = 255;
    }
    else if (Output < 0)
    {
        Output = 0;
    }

    write(channelPWM, Output);

    Serial.println("[PID Control] Temp: " + String(Input) + " oC");
    Serial.println("[PID Control] Setpoint: " + String(Setpoint) + " oC");
    Serial.println("[PID Control] Output: " + String(Output));

    /*Remember some variables for next time*/
    lastErr = error;
    lastTime = now;
}
