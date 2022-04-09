#include <Wire.h>
//#include "Navigation.h"
#include <EEPROM.h>

const int gyro_add = 0x68;
const double pi = 3.14159;

double Wx,Wy,Wz;
double Wx_e,Wy_e,Wz_e;
double W[3];
int rom_add = 0;
int omgXten;            //omega Z multiplied by 10

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(gyro_add);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
  check_error();
}

void check_error()
{
  for(int c = 0;c < 200;c++)
  {
    Wire.beginTransmission(gyro_add);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(gyro_add,6,true);
    Wx = Wire.read()<<8|Wire.read();
    Wy = Wire.read()<<8|Wire.read();
    Wz = Wire.read()<<8|Wire.read();

    Wx_e = Wx_e + Wx/131;
    Wy_e = Wy_e + Wy/131;
    Wz_e = Wz_e + Wz/131;
  }

  Wx_e = Wx_e/200;
  Wy_e = Wy_e/200;
  Wz_e = Wz_e/200;
}

void loop()
{
  Wire.beginTransmission(gyro_add);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(gyro_add,6,true);
  Wx = Wire.read()<<8|Wire.read();
  Wy = Wire.read()<<8|Wire.read();
  Wz = Wire.read()<<8|Wire.read();

  W[0] = (Wx/131 - Wx_e);
  W[1] = (Wy/131 - Wy_e);
  W[2] = (Wz/131 - Wz_e);
  
  /*
  Serial.print(W[0],3);
  Serial.print("  ");
  Serial.print(W[1],3);
  Serial.print("  ");
  Serial.print(W[2],3);
  Serial.println();
  */
  
  omgXten = int(abs(W[2]*10));
  /*
  Serial.print(omgXten);
  Serial.print("     ");
  Serial.print(millis()/1000);
  Serial.println(" ");
  */
  
  if (rom_add < 800)
  {
    EEPROM.write(rom_add,omgXten);
    EEPROM.write(rom_add+1,int(millis()/1000));
  }
  rom_add = rom_add + 2;
  
  delay(500);
}
