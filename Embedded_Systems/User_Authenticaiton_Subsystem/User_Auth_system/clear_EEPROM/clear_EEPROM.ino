#include <EEPROM.h>

const int EEPROM_SIZE = 512;  // You can set this to 4096 max on ESP32

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  // Clear EEPROM by writing 0xFF to every byte
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0xFF);  // or use EEPROM.write(i, 0)
  }

  EEPROM.commit();  // Save changes to flash

  Serial.println("EEPROM cleared.");
}

void loop() {
  // Nothing here
}
