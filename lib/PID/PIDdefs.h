#ifndef PIDdefs_H_
#define PIDdefs_H_
#include <Arduino.h>

typedef void (*writeOutput)(uint8_t channel, uint32_t dutyClycle);
typedef float (*readInput)();
#endif