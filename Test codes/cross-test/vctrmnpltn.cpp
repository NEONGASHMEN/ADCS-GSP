#include "vctrmnpltn.h"



void vctrmnpltn::cross(double a[3],double b[3])
{
	product[0] = a[1] * b[2] - a[2] * b[1];
   	product[1] = -(a[0] * b[2] - a[2] * b[0]);
   	product[2] = a[0] * b[1] - a[1] * b[0];
}