#include "earth_params.h"
#include <math.h>
#include "Satellite.h"

double orbit_rad = earth_rad + 400000;
double x = orbit_rad;
double y = 0;
double z = 0;
double pi = 3.141592653589793238;
double velocity = sqrt(mu/orbit_rad);
double inclination = 98*(pi/180);
double xdot = 0;
double ydot = velocity*cos(inclination);
double zdot = velocity*sin(inclination);
double state[] = {x,y,z,xdot,ydot,zdot};
int leap = 1;


double period = 2*pi*sqrt(pow(orbit_rad,3)/mu);
double no_of_opt = round(period)+1;
double timestep = 1;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(leap==1)
  {
    for(double i = 0;i < no_of_opt;i = i + timestep)
    {
      double* k1;
      double* k2;
      double* k3;
      double* k4;
      double k[6];
      double state2[6];
      double state3[6];
      double state4[6];

      k1 = Satellite(state,i);

      
      for(int j = 0;j < 6;j++)
      {
        state2[j] = state[j] + (k1[j]*timestep/2);
      }
      k2 = Satellite(state2,i + (timestep/2));


      for(int j = 0;j < 6;j++)
      {
        state3[j] = state[j] + (k2[j]*timestep/2);
      }
      k3 = Satellite(state3,i + (timestep/2));


      for(int j = 0;j < 6;j++)
      {
        state4[j] = state[j] + (k3[j]*timestep);
      }
      k4 = Satellite(state4,i + timestep);


      for(int j = 0;j < 6;j++)
      {
        k[j] = (k1[j] + k2[j]*2 + k3[j]*2 + k4[j])/6;
      }


      for(int j = 0;j < 6;j++)
      {
        state[j] = state[j] + (k[j]*timestep);
      }



      for(int j = 0;j < 6;j++)
      {
        Serial.print(state[j]/1000);
        Serial.print("  ");
        
      }

      Serial.print(">>>>>>>>>");
      Serial.print(i);
      Serial.print(" out of ");
      Serial.print(no_of_opt);
      Serial.println();
      
    }
  }
  leap = 2;
}
