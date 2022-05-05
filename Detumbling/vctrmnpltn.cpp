#include "vctrmnpltn.h"

void vctrmnpltn::cross(double a[3],double b[3])
{
	resultant[0] = a[1] * b[2] - a[2] * b[1];
   	resultant[1] = -(a[0] * b[2] - a[2] * b[0]);
   	resultant[2] = a[0] * b[1] - a[1] * b[0];
}
