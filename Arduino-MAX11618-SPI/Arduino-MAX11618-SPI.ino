#include <SPI.h>
#define CS_PIN 2

#define SETUP_FLAG     B10000000
#define SCAN_MODE_NONE B00000110
#define SCAN_MODE_0_N  B00000000
#define SCAN_MODE_N_4  B00000100

void setup() {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  Serial.begin(250000);
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  //Serial.println(readChannel0());
  uint32_t ch0and1 = readChannel0and1();
  Serial.print(ch0and1 >> 16);
  Serial.print(" ");
  Serial.println(ch0and1 & 0x03FF); //least sig 10 bits
  delay(25);
}

unsigned int readChannel0()
{
  digitalWrite(CS_PIN, LOW);
  byte msb = SPI.transfer(B10000110);
  delayMicroseconds(20);
  byte lsb = SPI.transfer(0x00);
  delayMicroseconds(20);
  digitalWrite(CS_PIN, HIGH);
  return (msb<<6 | lsb>>2);
}

uint32_t readChannel0and1()
{
  uint32_t data;
  uint16_t regData = SETUP_FLAG | 1 << 3 | SCAN_MODE_0_N;
  digitalWrite(CS_PIN, LOW);
  delayMicroseconds(20);
  uint16_t msb = SPI.transfer(regData);
  delayMicroseconds(20);
  uint16_t lsb = SPI.transfer(0x00);
  data = (msb<<6 | lsb>>2);
  data <<= 16;
  
  msb = SPI.transfer(0x00);
  delayMicroseconds(20);
  lsb = SPI.transfer(0x00);
  data |= (msb<<6 | lsb>>2);
  delayMicroseconds(20);
  digitalWrite(CS_PIN, HIGH);
  
  return data;
}

unsigned int readMode(byte channel, byte scanMode)
{
  byte regData = SETUP_FLAG | channel << 3 | scanMode;
  digitalWrite(CS_PIN, LOW);
  byte msb = SPI.transfer(regData);
  delayMicroseconds(20);
  byte lsb = SPI.transfer(0x00);
  delayMicroseconds(20);
  digitalWrite(CS_PIN, HIGH);
  return (msb<<6 | lsb>>2);

  
}

