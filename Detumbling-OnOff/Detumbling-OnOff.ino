#include <Wire.h>
#include <EEPROM.h>

const uint8_t magmeter_add = 0x1E;
const uint8_t gyro_add = 0x68;

const int calib_LED = 13;
const int idle_Pin = 22;
const int coil_LED = 24;
const int idle_LED = 26;

float muX_hat[] = {1,0,0};
float muY_hat[] = {0,1,0};
float B[3];
float B_hat[3];
float W[3];
float Wx_e,Wy_e,Wz_e;
int rodX = 8;
int rodX_ = 7;
int rodY = 6;
int rodY_ = 5;
unsigned long t_in_S;
int rom_add = 0;
unsigned long last_mill = 0;
void read_eeprom();

void setup() {

  pinMode(coil_LED, OUTPUT);
  pinMode(idle_LED, OUTPUT);
  pinMode(idle_Pin,INPUT_PULLUP);

  if (digitalRead(idle_Pin))  read_eeprom();
  while(digitalRead(idle_Pin))
  {                                                           //switch when on prevents the code from running
    digitalWrite(idle_LED, HIGH);
    delay(500);                                               //idle mode 
    digitalWrite(idle_LED, LOW);
    delay(500);
    
  }
  
  
  pinMode(calib_LED,OUTPUT);
  for(int j = 0;j < 5;j++)
  {
    digitalWrite(calib_LED,LOW);
    delay(1000);
    digitalWrite(calib_LED,HIGH);                             //Indication to hold the CubeSat still for calibration of gyro
    delay(1000);
  }
  digitalWrite(calib_LED,LOW);

  //Serial.begin(9600);
  Wire.begin();

  write2reg(magmeter_add,0x00,0x78);                          //output rate 75 per sec
  write2reg(magmeter_add,0x01,0xE0);                          //range slctn +/-8.1Gauss
  write2reg(magmeter_add,0x02,0x00);                          //cont measuremnt mode

  write2reg(gyro_add,0x6B,0x00);                              //gyro reset
  Wire.beginTransmission(gyro_add);
  Wire.write(0x1B);
  Wire.endTransmission(false);
  Wire.requestFrom(gyro_add,1,true);
  byte def_reg_val = Wire.read();                             //read register data for range
  byte new_reg_val = (def_reg_val&0x7)|0x10;
  write2reg(gyro_add,0x1B,new_reg_val);                       // Set range at 1000deg/s
  find_error();

  Serial.println("Magnetic field intensity:\t\t\tTorque hat:");
  
}

void write2reg(uint8_t add, uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(add);
  Wire.write(reg);
  Wire.write(value);                                        //write to a register fn
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


void find_error()                                             //finding gyro error
{
  int c = 0;
  while (c < 200) {
    Wire.beginTransmission(gyro_add);
    Wire.write(0x43);                                         //0x43,44,45,46,47,48 LSB and MSB values
    Wire.endTransmission(false);
    Wire.requestFrom(gyro_add, 6, true);
    W[0] = Wire.read() << 8 | Wire.read();
    W[1] = Wire.read() << 8 | Wire.read();
    W[2] = Wire.read() << 8 | Wire.read();
    
    Wx_e = Wx_e + (W[0] / 32.8);
    Wy_e = Wy_e + (W[1] / 32.8);                              //LSB per unit is 32.8 for +/-1000deg/sec range
    Wz_e = Wz_e + (W[2] / 32.8);
    c++;
  }
  
  Wx_e = Wx_e / 200;
  Wy_e = Wy_e / 200;                                          //Divide the sum by 200 to get the error value
  Wz_e = Wz_e / 200;

  Serial.print("GyroErrorX: ");
  Serial.println(Wx_e);
  Serial.print("GyroErrorY: ");
  Serial.println(Wy_e);
  Serial.print("GyroErrorZ: ");
  Serial.println(Wz_e);
}

void read_eeprom(){                                           //Function to read EEPROM
  rom_add = 0;
  float omegaZ;
  unsigned long t_in_s;
  Serial.begin(9600);
  digitalWrite(idle_LED, HIGH);
  while(rom_add < 4096)
    {
      Serial.print(rom_add);
      Serial.print("\t");
      Serial.print(EEPROM.get(rom_add,omegaZ));
      Serial.print("\t");
      Serial.println(EEPROM.get(rom_add+sizeof(omegaZ),t_in_s));

      rom_add = rom_add + sizeof(omegaZ)+sizeof(t_in_s);
      delay(10);
    }
  Serial.end();
  digitalWrite(idle_LED, LOW);
  rom_add = 0;
}

void loop() {
  
  Wire.beginTransmission(magmeter_add);
  Wire.write(0x03);
  Wire.endTransmission(false);
  Wire.requestFrom(magmeter_add,6,true);                        //0x03,04,05,06,07,08 LSB and MSB values
  B[0] = ((Wire.read()<<8)|(Wire.read()))/230.0;
  B[2] = ((Wire.read()<<8)|(Wire.read()))/230.0;  
  B[1] = ((Wire.read()<<8)|(Wire.read()))/230.0;                //Lsb per unit = 230 for +/-8.1Gauss
  B[0] = B[0]*1E-4;
  B[1] = B[1]*1E-4;
  B[2] = 0;                                                     //into Teslas

  Wire.beginTransmission(gyro_add);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(gyro_add,6,true);
  W[0] = (Wire.read()<<8)|(Wire.read());
  W[1] = (Wire.read()<<8)|(Wire.read());
  W[2] = (Wire.read()<<8)|(Wire.read());
  W[0] = W[0]/32.8 - Wx_e;                                      //lsb per unit rad = 32.8
  W[1] = W[1]/32.8 - Wy_e;
  W[2] = W[2]/32.8 - Wz_e;

  
  float muXb[3];
  cross(muX_hat,B,muXb);
  float muXb_hat[3];                                            //muXb_hat gives direction of torque
  unify(muXb,muXb_hat);

  float muYb[3]; 
  cross(muY_hat,B,muYb);
  float muYb_hat[3];                                            //muYb_hat gives direction of torque
  unify(muYb,muYb_hat);

  if(muXb_hat[2]*W[2] < 0)
  {
    analogWrite(rodX,255);                                      //NB: direction of mu is paralell to X - axis of the body frame (magnetometer)
    digitalWrite(coil_LED,HIGH);
    analogWrite(rodX_,0);
  }

  else
  {
    analogWrite(rodX,0);
    digitalWrite(coil_LED,LOW);
    analogWrite(rodX_,255);
  }

  if(muYb_hat[2]*W[2] < 0)
  {
    analogWrite(rodY,255);                                      //NB: direction of mu is paralell to y - axis of the body frame (magnetometer)
    digitalWrite(idle_LED,HIGH);  //dual
    analogWrite(rodY_,0);
  }

  else
  {
    analogWrite(rodY,0);
    digitalWrite(idle_LED,LOW);  //dual
    analogWrite(rodY_,255);
  }

  if (rom_add<4096 && (millis()-last_mill)>100)
  {
      float omgZ = W[2];
      t_in_S = millis();
                                                                
      EEPROM.put(rom_add,omgZ);                                 //Write the values of omega and time in the ROM 
      EEPROM.put(rom_add+sizeof(omgZ),t_in_S);

      rom_add = rom_add + sizeof(omgZ)+sizeof(t_in_S);
      last_mill = millis();   
  }
  
  if (rom_add>=4096)
  {
    digitalWrite(calib_LED,HIGH);                                //ROM full indicator
  }

  Serial.print(B[0],6);
  Serial.print("  ");
  Serial.print(B[1],6);
  Serial.print("  ");
  Serial.print(B[2],6);
  Serial.print("  ");
  Serial.print(norm(B),6);
  Serial.print("\t\t\t");
  Serial.print(muXb_hat[0]);
  Serial.print("  ");
  Serial.print(muXb_hat[1]);
  Serial.print("  ");
  Serial.print(muXb_hat[2]);
  Serial.print("\t");
  Serial.print(W[2],6);
  Serial.println("  ");
  

  
}
