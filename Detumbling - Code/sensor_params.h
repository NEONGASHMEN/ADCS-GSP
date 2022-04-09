#ifndef SENSOR_PARAMS_H
#define SENSOR_PARAMS_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//Torquer params
double const pi = 3.14159265;
double rodRadius = 0.005;
double A = pi*rodRadius*rodRadius;
double muR = 1453;                    //steel rod
double rodLen = 0.07;
double RperLen = 0.155;               //Resistance per unit meter length
double n = 250;
double Rnet = RperLen*2*pi*rodRadius*n;
double saturation_current = 0.04;
double Sb = 0.3;                      //sensor reliance fctrs
double Sw = 0.3;
double muBmax = 0.2;

//Demagnetisation fctr
double Nd = (4*(log(rodLen/rodRadius) - 1))/(((sq(rodLen))/(sq(rodRadius)))-(4*log(rodLen/rodRadius)));
double aFctr = 1 + ((muR - 1)/(1 + (Nd*(muR - 1))));
#endif
