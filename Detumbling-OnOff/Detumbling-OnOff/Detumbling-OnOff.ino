#include <Wire.h>

const uint8_t magmeter_add = 0x1E;

float mu_hat[] = {1,0,0};
float B[3];
float B_hat[3];
int rodX = 18;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  pinMode(rodX,OUTPUT);

  write2reg(magmeter_add,0x00,0x78);       //output rate 75 per sec
  write2reg(magmeter_add,0x01,0xE0);       //range slctn +/-8.1Gauss
  write2reg(magmeter_add,0x02,0x00);       //cont measuremnt mode

  
}

void write2reg(uint8_t add, uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(add);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}

float norm(float vctr[3])
{
  return sqrt(vctr[0]*vctr[0] + vctr[1]*vctr[1] + vctr[2]*vctr[2]);
}

void unify(float vctr[3],float hat[3])
{
  float mod;
  mod = norm(vctr);
  hat[0] = vctr[0]/mod;
  hat[1] = vctr[1]/mod;
  hat[2] = vctr[2]/mod;
}

void cross(float vctr1[3],float vctr2[3],float res[3])
{
  res[0] = vctr1[1]*vctr2[2] - vctr2[1]*vctr1[2];
  res[1] = vctr2[0]*vctr1[2] - vctr1[0]*vctr2[2];
  res[2] = vctr1[0]*vctr2[1] - vctr2[0]*vctr1[1];
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(magmeter_add);
  Wire.write(0x03);
  Wire.endTransmission(false);
  Wire.requestFrom(magmeter_add,6,true);                      //0x03,04,05,06,07,08 LSB and MSB values
  B[0] = ((Wire.read()<<8)|(Wire.read()))/230.0;
  B[2] = ((Wire.read()<<8)|(Wire.read()))/230.0;
  B[1] = ((Wire.read()<<8)|(Wire.read()))/230.0;             //Lsb per unit = 1090 for +/-1.3Gauss
  B[0] = B[0]*1E-4;
  B[1] = B[1]*1E-4;
  B[2] = 0;                                                   //into Teslas

  float muXb[3];
  cross(mu_hat,B,muXb);
  float muXb_hat[3];
  unify(muXb,muXb_hat);

  if(muXb_hat[2] < 0)
  {
    digitalWrite(rodX,HIGH);
  }
  

  
}
