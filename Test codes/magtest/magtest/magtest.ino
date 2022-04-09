#include <Wire.h>

const uint8_t addrs = 0x1E;

double bx,by,bz;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  
  write2reg(addrs,0x00,0x70);       //output rate 15 per sec
  write2reg(addrs,0x01,0x20);       //range slctn +/-1.3Gauss
  write2reg(addrs,0x02,0x00);       //cont measuremnt mode

}

void write2reg(uint8_t add, uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(add);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}


void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(addrs);
  Wire.write(0x03);
  Wire.endTransmission(false);
  Wire.requestFrom(addrs,6,true);                    //0x03,04,05,06,07,08 LSB and MSB values
  bx = ((Wire.read()<<8)|(Wire.read()))/1090.0;
  bz = ((Wire.read()<<8)|(Wire.read()))/1090.0;
  by = ((Wire.read()<<8)|(Wire.read()))/1090.0;             //Lsb per unit gauss = 1090 for +/-1.3Ga
  bx = bx*1E2;
  by = by*1E2;
  bz = bz*1E2;
  double B = sqrt(pow(bx,2) + pow(by,2) + pow(bz,2));
  delay(500);
  
  Serial.print(bx,6);
  Serial.print("      ");
  Serial.print(by,6);
  Serial.print("      ");
  Serial.print(bz,6);
  Serial.print("      ");
  Serial.println(B,6);
}
