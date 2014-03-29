
/*
The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
#define LCD_RS 18
#define LCD_E  19
#define LCD_D4 20
#define LCD_D5 21
#define LCD_D6 22
#define LCD_D7 23
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

uint8_t row = 0;
uint8_t col = 0;
uint8_t blink = 0;
uint8_t cursor = 0;

#define NUM_COLS 20
#define NUM_ROWS 4

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(NUM_COLS, NUM_ROWS);
  // Print a message to the LCD.
  lcd.clear();
  lcd.print("display v0.1");

  Serial.begin(9600);
  Serial1.begin(9600);

  delay(1000);
  lcd.clear();
  lcd.setCursor(row, col);
}

int esc = 0;
void lcdPrint(char c)
{
  if(c == 27)
    esc = 1;
  else if(esc == 1)
  {
    if(c == 'A')
    {
      if(row > 0)
        row--;
      lcd.setCursor(col, row);
    }
    else if(c == 'B')
    {
      if(row < NUM_ROWS - 1)
        row++;
      lcd.setCursor(col, row);
    }
    else if(c == 'C')
    {
      if(col < NUM_COLS - 1)
        col++;
      lcd.setCursor(col, row);
    }
    else if(c == 'D')
    {
      if(col > 0)
        col--;
      lcd.setCursor(col, row);
    }
    else if(c == 'H')
    {
      row = col = 0;
      lcd.setCursor(col, row);
    }
    else if(c == 'J')
    {
      row = col = 0;
      lcd.setCursor(col, row);
      lcd.clear();
    }
    else if(c == 'b')
    {
      blink = !blink;
      if(blink)
        lcd.blink();
      else
        lcd.noBlink();
    }
    else if(c == 'c')
    {
      cursor = !cursor;
      if(cursor)
        lcd.cursor();
      else
        lcd.noCursor();
    }


    esc = 0;
  }
  else if(c == '\r')
  {
    col = 0;
    lcd.setCursor(col, row);
  }
  else if(c == '\n')
  {
    col = 0; 
    row++;
    lcd.setCursor(col, row);
  }
  else
    lcd.write(c);
}

void loop() {
  // read from port 0, send to port 1:
  if (Serial.available()) {
    int c = Serial.read();
    lcdPrint(c);
    //Serial1.print(inByte, BYTE);

  }
  // read from port 1, send to port 0:
  if (Serial1.available()) {
    int c = Serial1.read();
    lcdPrint(c);
    //    Serial.print(inByte, BYTE);
  }
}

