#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <Arduino.h>
#include "touchDefs.h"

class touch
{
private:
    //habilita o touch
    bool enableTouch0;
    bool enableTouch1;
    bool enableTouch2;
    bool enableTouch3;
    bool enableTouch4;
    bool enableTouch5;
    bool enableTouch6;
    bool enableTouch7;
    bool enableTouch8;
    bool enableTouch9;

    // armazena o valor da leitura inicial
    unsigned long startValueTouch0;
    unsigned long startValueTouch1;
    unsigned long startValueTouch2;
    unsigned long startValueTouch3;
    unsigned long startValueTouch4;
    unsigned long startValueTouch5;
    unsigned long startValueTouch6;
    unsigned long startValueTouch7;
    unsigned long startValueTouch8;
    unsigned long startValueTouch9;

    // percentual com relação a primeira leitura
    float percentTouch0;
    float percentTouch1;
    float percentTouch2;
    float percentTouch3;
    float percentTouch4;
    float percentTouch5;
    float percentTouch6;
    float percentTouch7;
    float percentTouch8;
    float percentTouch9;

    // fator com relação a primeira leitura
    unsigned long factorTouch0;
    unsigned long factorTouch1;
    unsigned long factorTouch2;
    unsigned long factorTouch3;
    unsigned long factorTouch4;
    unsigned long factorTouch5;
    unsigned long factorTouch6;
    unsigned long factorTouch7;
    unsigned long factorTouch8;
    unsigned long factorTouch9;

    // valor atual lido
    unsigned long valueTouch0;
    unsigned long valueTouch1;
    unsigned long valueTouch2;
    unsigned long valueTouch3;
    unsigned long valueTouch4;
    unsigned long valueTouch5;
    unsigned long valueTouch6;
    unsigned long valueTouch7;
    unsigned long valueTouch8;
    unsigned long valueTouch9;

    // debounce touch
    unsigned long debounceTouch0;
    unsigned long debounceTouch1;
    unsigned long debounceTouch2;
    unsigned long debounceTouch3;
    unsigned long debounceTouch4;
    unsigned long debounceTouch5;
    unsigned long debounceTouch6;
    unsigned long debounceTouch7;
    unsigned long debounceTouch8;
    unsigned long debounceTouch9;

    // indica que o Touch foi tocado
    bool leanTouch0;
    bool leanTouch1;
    bool leanTouch2;
    bool leanTouch3;
    bool leanTouch4;
    bool leanTouch5;
    bool leanTouch6;
    bool leanTouch7;
    bool leanTouch8;
    bool leanTouch9;

public:
    touch(/* args */);
    ~touch();
};

touch::touch(/* args */)
{
}

touch::~touch()
{
}

#endif