

//#define USE_SERIAL
#define USE_DHT
#define USE_DS18B20
#define USE_NRF24
#define USE_DISPLAY

/**********************************
 * DHT22
 **********************************/

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 9     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

/**********************************
 * DHT22
 **********************************/

/**********************************
 * DS18B20
 **********************************/
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
/**********************************
 * DS18B20
 **********************************/

/**********************************
 * DISPLAY SSD1306
 **********************************/
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);    // I2C / TWI

/**********************************
 * DISPLAY SSD1306
 **********************************/

//--------------------------------------------------------------------------------------------------
// Read current supply voltage
//--------------------------------------------------------------------------------------------------
long readVcc() {
  bitClear(PRR, PRADC); 
  ADCSRA |= bit(ADEN); // Enable the ADC
  long result;
  // Read 1.1V reference against Vcc
#if defined(__AVR_ATtiny84__) 
  ADMUX = _BV(MUX5) | _BV(MUX0); // For ATtiny84
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);  // For ATmega328
#endif 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate Vcc in mV
  ADCSRA &= ~ bit(ADEN); 
  bitSet(PRR, PRADC); // Disable the ADC to save power
  return result;
} 
//########################################################################################################################


void setup() {
  #ifdef USE_SERIAL
  Serial.begin(9600);
  Serial.println("DHT22 SSD1306 test!");
  #endif
  u8g.setColorIndex(1); //BW Display

  //u8g.setFont(u8g_font_8x13);
  //u8g.drawStr( 0, 57, "Hello World!");



  u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr( 0, 15, "DHT22 on Pin 9!");

  } while( u8g.nextPage() );

  dht.begin();

  //sensors.begin();
  //sensors.setResolution(10);

}

#define LH 12

void draw(float t, float h, float t_ds, long vcc)
{
   // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, 1*LH, "DHT22 on Pin 9!");

  u8g.setPrintPos(0, 2*LH);
  u8g.print("T: ");
  u8g.print(t, 1);
  u8g.print(" C");

  u8g.setPrintPos(0, 3*LH);
  u8g.print("H: ");
  u8g.print(h, 0);
  u8g.print(" %");

  u8g.setPrintPos(0, 4*LH);
  u8g.print("T2: ");
  u8g.print(t_ds, 1);
  u8g.print(" C");

  u8g.setPrintPos(0, 5*LH);
  u8g.print("Vcc: ");
  u8g.print(vcc);
  u8g.print(" mV");

}

void loop() {
  
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    #ifdef USE_SERIAL
    Serial.println("Failed to read from DHT sensor!");
    #endif
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
  #ifdef USE_SERIAL
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  #endif

/**********************************
 * DS18B20
 **********************************/
  sensors.requestTemperatures();
  float t_ds = sensors.getTempCByIndex(0);
  #ifdef USE_SERIAL
  Serial.print("DS18B20: ");
  Serial.print(t_ds);
  Serial.println(" *C ");
  #endif
/**********************************
 * DS18B20
 **********************************/


  long vcc = readVcc();
  #ifdef USE_SERIAL
  Serial.print("Voltage: ");
  Serial.print(vcc);
  Serial.println(" mV");
  #endif

    // picture loop
  u8g.firstPage();  
  do {
    draw(t, h, t_ds, vcc);
  } while( u8g.nextPage() );

}
