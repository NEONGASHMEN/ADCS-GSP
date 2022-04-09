#include <Wire.h>

const int gyro_add = 0x68;
const int magmeter_add = 0x1E;
const float pi = 3.14159;

double Bx,By,Bz;
float Bx_e,By_e,Bz_e;
float Wx,Wy,Wz;
float Wx_e,Wy_e,Wz_e;
int c = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(gyro_add);
  Wire.write(0x6B);                             //reset registry
  Wire.write(0x00);
  Wire.endTransmission(true);

  Wire.beginTransmission(magmeter_add);
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.endTransmission(true);

  find_error();
  delay(50);
}

void find_error()
{
  while (c < 200) {
    Wire.beginTransmission(gyro_add);
    Wire.write(0x43);                             //0x43,44,45,46,47,48 LSB and MSB values
    Wire.endTransmission(false);
    Wire.requestFrom(gyro_add, 6, true);
    Wx = Wire.read() << 8 | Wire.read();
    Wy = Wire.read() << 8 | Wire.read();
    Wz = Wire.read() << 8 | Wire.read();
    // Sum all readings
    Wx_e = Wx_e + (Wx / 131.0);
    Wy_e = Wy_e + (Wy / 131.0);                  //LSB per unit is 131.0 for +/-250deg/sec range
    Wz_e = Wz_e + (Wz / 131.0);
    c++;
  }
  //Divide the sum by 200 to get the error value
  Wx_e = Wx_e / 200;
  Wy_e = Wy_e / 200;
  Wz_e = Wz_e / 200;
  // Print the error values on the Serial Monitor
  Serial.print("GyroErrorX: ");
  Serial.println(Wx_e);
  Serial.print("GyroErrorY: ");
  Serial.println(Wy_e);
  Serial.print("GyroErrorZ: ");
  Serial.println(Wz_e);
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(gyro_add);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(gyro_add,6,true);
  Wx = (Wire.read()<<8)|(Wire.read());
  Wy = (Wire.read()<<8)|(Wire.read());
  Wz = (Wire.read()<<8)|(Wire.read());
  Wx = Wx/131.0 - Wx_e;
  Wy = Wy/131.0 - Wy_e;
  Wz = Wz/131.0 - Wz_e;
  Wx = (Wx*pi)/180.00;                                    //into radians
  Wy = (Wy*pi)/180.00;
  Wz = (Wz*pi)/180.00;
  
  //-----------------------------------------------------//

  Wire.beginTransmission(magmeter_add);
  Wire.write(0x03);
  Wire.endTransmission(false);
  Wire.requestFrom(magmeter_add,6,true);                    //0x03,04,05,06,07,08 LSB and MSB values
  Bx = ((Wire.read()<<8)|(Wire.read()))/1090.0;
  Bz = ((Wire.read()<<8)|(Wire.read()))/1090.0;
  By = ((Wire.read()<<8)|(Wire.read()))/1090.0;               //Lsb per unit = 1090 for +/-1.3Ga (WTF IS GA ???? sounds like gay)
  Bx = Bx*1E-4;
  By = By*1E-4;
  Bz = Bz*1E-4;                                             //1 gauss = 1E-4 T
  //----------------------------------------------------//

  Serial.print("Omegas (rad/s):  ");
  Serial.print(Wx,6);
  Serial.print("  ");
  Serial.print(Wy,6);
  Serial.print("  ");
  Serial.print(Wz,6);
  Serial.print("          Bs (mT): ");
  Serial.print(Bx*1000.0,6);
  Serial.print("  ");
  Serial.print(By*1000.0,6);
  Serial.print("  ");
  Serial.print(Bz*1000.0,6);
  Serial.println();
  delay(1000);

  
}
