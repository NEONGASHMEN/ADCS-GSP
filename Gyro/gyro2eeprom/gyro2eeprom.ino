#include <Wire.h>
//#include "Navigation.h"
#include <EEPROM.h>

const int gyro_add = 0x68;
const double pi = 3.14159;

float Wx,Wy,Wz;
float Wx_e,Wy_e,Wz_e;
float W[3];
int rom_add = 0;
float omgZ;            //omega Z
int t_in_S;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(53,INPUT);
  pinMode(51,INPUT);
  Serial.begin(9600);
  Wire.begin();

  if (digitalRead(53))
  {
    digitalWrite(LED_BUILTIN,HIGH);
    delay(10000);
    check_error();
    digitalWrite(LED_BUILTIN,LOW);
    delay(5000);
    digitalWrite(LED_BUILTIN,HIGH);

    Wire.beginTransmission(gyro_add);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission(true);

    Wire.beginTransmission(gyro_add);
    Wire.write(0x1B);
    Wire.endTransmission(false);
    Wire.requestFrom(gyro_add,1,true);
    byte def_reg_val = Wire.read();
    byte new_reg_val = (def_reg_val&0x7)|0x10;

    Wire.beginTransmission(gyro_add);
    Wire.write(0x1B);
    Wire.write(new_reg_val);
    Wire.endTransmission(true);
  }

  
  
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

    Wx_e = Wx_e + Wx/32.8;
    Wy_e = Wy_e + Wy/32.8;
    Wz_e = Wz_e + Wz/32.8;
  }

  Wx_e = Wx_e/200;
  Wy_e = Wy_e/200;
  Wz_e = Wz_e/200;
}

void loop()
{
  if (rom_add>0)
  {
    rom_add = 0;
  }
  while(digitalRead(53))
  {
    if (rom_add<4096)
    {
        Wire.beginTransmission(gyro_add);
        Wire.write(0x43);
        Wire.endTransmission(false);
        Wire.requestFrom(gyro_add,6,true);
        Wx = Wire.read()<<8|Wire.read();
        Wy = Wire.read()<<8|Wire.read();
        Wz = Wire.read()<<8|Wire.read();

        W[0] = (Wx/32.8 - Wx_e);
        W[1] = (Wy/32.8 - Wy_e);
        W[2] = (Wz/32.8 - Wz_e);
  
        omgZ = abs(W[2]);
        t_in_S = millis()/1000;

        EEPROM.put(rom_add,omgZ);
        EEPROM.put(rom_add+sizeof(omgZ),t_in_S);

        rom_add = rom_add + sizeof(omgZ)+sizeof(t_in_S);
        delay(1000);
    }

    else
    {
      digitalWrite(LED_BUILTIN,HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN,LOW);
      delay(500);
    }
  }

  if (rom_add>0)
  {
    rom_add = 0;
  }
  while (digitalRead(51))
  {
    if(rom_add < 4096)
    {
      Serial.print(rom_add);
      Serial.print("\t");
      Serial.print(EEPROM.get(rom_add,omgZ));
      Serial.print("\t");
      Serial.println(EEPROM.get(rom_add+sizeof(omgZ),t_in_S));

      rom_add = rom_add + sizeof(omgZ)+sizeof(t_in_S);
      delay(500);
    }
  }

  Serial.println("Switch to either read mode (pin 51) or eeprom write mode (pin 53) !!");
  
}
