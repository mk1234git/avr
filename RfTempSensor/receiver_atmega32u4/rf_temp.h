
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 9


void printAddress(DeviceAddress deviceAddress);

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int numberOfDevices; // Number of temperature devices found

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address


void setup_rftemp()
{
  // Start up the library
  sensors.begin();
  
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();


}

uint8_t transmitCnt = 0;

void loop_rftemp(uint8_t *pMsg, uint8_t msgLenMax)
{
  uint8_t m = 0;
  pMsg[m++] = 'T';
  pMsg[m++] = transmitCnt++;
    
//  sensors.requestTemperaturesByIndex(0);
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  Serial.print("#temp: ");
  Serial.println(numberOfDevices, DEC);
 
  DeviceAddress deviceAddress;
  uint8_t i = 0;
  for(i = 0; sensors.getAddress(deviceAddress, i); i++)
  {
    int16_t t = sensors.getTemp((uint8_t*)deviceAddress);
    float tC = sensors.rawToCelsius(t);
    
    Serial.print(i, DEC);
    Serial.print(": ");
    printAddress(deviceAddress);
    Serial.print(": ");
    Serial.print(t, DEC);
    Serial.print(" raw, ");
    Serial.print(tC, DEC);
    Serial.println(" C.");  
    
    pMsg[m++] = deviceAddress[0];
    pMsg[m++] = t >> 8;
    pMsg[m++] = t & 0xFF;
  }
  pMsg[m++] = 'E';
  pMsg[m++] = 0;

}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
