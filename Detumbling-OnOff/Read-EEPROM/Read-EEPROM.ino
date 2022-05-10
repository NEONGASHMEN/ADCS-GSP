  #include<EEPROM.h>

int rom_add = 0;
float omegaZ;
unsigned long t_in_s;

void setup() {
  
  Serial.begin(9600);
//  Serial.print("Address:\t\tValue:\t\tTime:");

}

void loop() {
  
  if(rom_add < 4096)
    {
      Serial.print(rom_add);
      Serial.print("\t");
      Serial.print(EEPROM.get(rom_add,omegaZ));
      Serial.print("\t");
      Serial.println(EEPROM.get(rom_add+sizeof(omegaZ),t_in_s));

      rom_add = rom_add + sizeof(omegaZ)+sizeof(t_in_s);
      delay(10);
    }
}
