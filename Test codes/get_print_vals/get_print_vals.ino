#include <EEPROM.h>

int rom_add = 0;
int t;
int omegaZ;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  omegaZ = EEPROM.read(rom_add);
  t = EEPROM.read(rom_add+1);
  Serial.print(rom_add);
  Serial.print("         ");
  Serial.print(omegaZ);
  Serial.print("         ");
  Serial.print(t);
  Serial.println("");

  rom_add = rom_add+2;

  if (rom_add > 800)
  {
    rom_add = 0;
  }

  delay(500);
}
