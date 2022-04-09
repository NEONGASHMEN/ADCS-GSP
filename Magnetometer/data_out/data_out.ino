#include <Wire.h>

const uint8_t magmeter_add = 0x1E;

double Bx,By,Bz;
double B[3];
double Bx_e,By_e,Bz_e;
double Ainv[3][3];
double BBias[3];
double Bcal[3];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();

  write2reg(magmeter_add,0x00,0x70);       //output rate 15 per sec
  write2reg(magmeter_add,0x01,0x20);       //range slctn +/-1.3Gauss
  write2reg(magmeter_add,0x02,0x00);       //cont measuremnt mode

  //Ainv
  Ainv[0][0] = 1.261081;
  Ainv[0][1] = 0.045358;
  Ainv[0][2] = 0.063171;
  Ainv[1][0] = 0.045358;
  Ainv[1][1] = 1.215031;
  Ainv[1][2] = 0.007947;
  Ainv[2][0] = 0.063171;
  Ainv[2][1] = 0.007947;
  Ainv[2][2] = 1.348147;

  //Mag bias
  BBias[0] = -0.797181;
  BBias[1] = -20.952939;
  BBias[2] = -1.829073;
}

void mycalibr(double Bx, double By, double Bz)
{
  double temp[3];
  temp[0] = Bx - BBias[0];
  temp[1] = By - BBias[1];
  temp[2] = Bz - BBias[2];

  Bcal[0] = Ainv[0][0]*temp[0] + Ainv[0][1]*temp[1] + Ainv[0][2]*temp[2];
  Bcal[1] = Ainv[1][0]*temp[0] + Ainv[1][1]*temp[1] + Ainv[1][2]*temp[2];
  Bcal[2] = Ainv[2][0]*temp[0] + Ainv[2][1]*temp[1] + Ainv[2][2]*temp[2];

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
  Wire.beginTransmission(magmeter_add);
  Wire.write(0x03);
  Wire.endTransmission(false);
  Wire.requestFrom(magmeter_add,6,true);                    //0x03,04,05,06,07,08 LSB and MSB values
  Bx = ((Wire.read()<<8)|(Wire.read()))/1090.0;
  Bz = ((Wire.read()<<8)|(Wire.read()))/1090.0;
  By = ((Wire.read()<<8)|(Wire.read()))/1090.0;             //Lsb per unit = 1090 for +/-1.3Gauss
  Bx = Bx*1E-4;
  By = By*1E-4;
  Bz = Bz*1E-4;                                             //into Teslas
  B[0] = Bx*1E6;
  B[1] = By*1E6;
  B[2] = Bz*1E6;

  mycalibr(B[0],B[1],B[2]);
  //Bcal[0] = B[0];
  //Bcal[1] = B[1];
  //Bcal[2] = B[2];
  Serial.print(B[0]);
  Serial.print("\t");
  Serial.print(B[1]);
  Serial.print("\t");
  Serial.print(B[2]);
  Serial.print("\t");
  Serial.print(sqrt(B[0]*B[0]+B[1]*B[1]+B[2]*B[2]));
  Serial.print("\t\t");
  //Serial.println();

  Serial.print(Bcal[0]);
  Serial.print("\t");
  Serial.print(Bcal[1]);
  Serial.print("\t");
  Serial.print(Bcal[2]);
  Serial.print("\t");
  Serial.print(sqrt(Bcal[0]*Bcal[0]+Bcal[1]*Bcal[1]+Bcal[2]*Bcal[2]));
  Serial.println("");

  delay(100);
}
