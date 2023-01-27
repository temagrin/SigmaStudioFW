#include <Wire.h>
#include "USER_SETTINGS.h"

#include "Design_1_IC_1.h"
#include "Design_1_IC_1_PARAM.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin();
  Wire.setClock(I2C_SPEED);
  Serial.println("LOAD FIRMWARE");
  default_download_IC_1();
}

byte onComm[4] = {0x00,0x80,0x00,0x00};
byte offComm[4] = {0x00,0x00,0x00,0x00};

void loop() {
  SIGMA_WRITE_REGISTER_BLOCK(MOD_SQUARE1_ALG0_ON_ADDR, 4, onComm);
  delay(1000);
  SIGMA_WRITE_REGISTER_BLOCK(MOD_TONE1_ALG0_ON_ADDR, 4, onComm);
  delay(1000);
  SIGMA_WRITE_REGISTER_BLOCK(MOD_SQUARE1_ALG0_ON_ADDR, 4, offComm);
  delay(1000);
  SIGMA_WRITE_REGISTER_BLOCK(MOD_TONE1_ALG0_ON_ADDR, 4, offComm);
  delay(1000);
  
}
