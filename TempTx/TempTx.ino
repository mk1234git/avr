#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void) {
Serial.begin(9600);
sensors.begin();
radio.begin();
radio.openWritingPipe(pipe);
}

void loop(void)
{
sensors.requestTemperatures();
float temperature = sensors.getTempCByIndex(0);
radio.write(&temperature, sizeof(float));
delay(1000);
}
