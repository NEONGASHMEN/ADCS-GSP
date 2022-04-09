#include "Navigation.h"


double W_prev[3] = {0,0,0};
double W_bias[3] = {0,0,0};
double s = 0.3;


void Navigation(double W[3])
{
 
  
  if(abs(W_prev[0]) + abs(W_prev[1]) + abs(W_prev[2]) == 0)
  {
    W_prev[0] = W[0];
    W_prev[1] = W[1];
    W_prev[2] = W[2];
  }

  else
  {
    for(int i = 0;i < 3;i++)
    {
      W[i] = W_prev[i]*(1.0 - s) + s*(W[i] - W_bias[i]);
    }
  }

  W_prev[0] = W[0];
  W_prev[1] = W[1];
  W_prev[2] = W[2];
}
