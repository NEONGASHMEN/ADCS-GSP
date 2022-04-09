#include "Satellite.h"
#include "earth_params.h"
#include <math.h>

double* Satellite(double state[6],double t)
{
  double rhat[3];
  double rmod = sqrt(pow(state[0],2) + pow(state[1],2) + pow(state[2],2));
  rhat[0] = state[0]/rmod;
  rhat[1] = state[1]/rmod;
  rhat[2] = state[2]/rmod;
  
  double vel[3];
  vel[0] = state[3];
  vel[1] = state[4];
  vel[2] = state[5];

  double acc[3];

  for(int i = 0;i < 3;i++)
  {
    acc[i] = (mu*rhat[i]*(-1))/pow(rmod,2);
  }

  static double derivative[6];
  derivative[0] = vel[0];
  derivative[1] = vel[1];
  derivative[2] = vel[2];
  derivative[3] = acc[0];
  derivative[4] = acc[1];
  derivative[5] = acc[2];

  return derivative;
  
}
