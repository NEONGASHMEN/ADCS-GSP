#ifndef SATELLITE_H
#define SATELLITE_H

#if ARDUINO >= 100
#include "Arduino.h"
#else 
#include "Wprogram.h"
#endif

double* Satellite(double state[6],double t);

#endif
