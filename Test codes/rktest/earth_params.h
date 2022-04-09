#ifndef EARTH_PARAMS_H
#define EARTH_PARAMS_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "Wprogram.h"
#endif

#include <math.h>

const double earth_rad = 6378000;
const double mu = 3.986004418E14;
const double earth_mass = 5.9722E24;


#endif
