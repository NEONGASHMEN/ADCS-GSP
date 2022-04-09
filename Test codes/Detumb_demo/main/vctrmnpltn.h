#ifndef VCTRMNPLTN_H
#define VCTRMNPLTN_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class vctrmnpltn
{
	public:
	void cross(double a[3],double b[3]);
	double resultant[3];
};

#endif