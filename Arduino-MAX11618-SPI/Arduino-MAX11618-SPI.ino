#include <SPI.h>
#define CS_PIN 2

void setup() {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  Serial.begin(250000);
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  Serial.println(captureValue());
  delay(5);
}

unsigned int captureValue()
{
  digitalWrite(CS_PIN, LOW);
  byte msb = SPI.transfer(B10000110);
  delayMicroseconds(20);
  byte lsb = SPI.transfer(0x00);
  delayMicroseconds(20);
  digitalWrite(CS_PIN, HIGH);
  return (msb<<6 | lsb>>2);
}

