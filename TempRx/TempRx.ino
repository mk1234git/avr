#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void) {
Serial.begin(9600);
radio.begin();
radio.openReadingPipe(1, pipe);
radio.startListening();
Serial.begin(9600);
Serial.println("Temperature Starting.....");
delay(1000);
Serial.println("by www.elec-cafe.com");
delay(1000);
}

void loop(void)
{
if (radio.available()) {
float temperature = 0;
if (!radio.read(&temperature, sizeof(float))) {
Serial.println("ACK not received by client.");
}
Serial.print("Temperature : ");
Serial.println(temperature);
delay(1000);
}
}
