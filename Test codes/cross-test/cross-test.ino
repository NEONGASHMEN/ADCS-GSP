#include "vctrmnpltn.h"

vctrmnpltn vctr;
double v1[] = {1,2,3};
double v2[] = {3,2,1};

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
vctr.cross(v1,v2);
digitalWrite(13,LOW);
Serial.print(vctr.product[0]);
Serial.print("  ");
Serial.print(vctr.product[1]);
Serial.print("  ");
Serial.println(vctr.product[2]);
delay(100);

}
