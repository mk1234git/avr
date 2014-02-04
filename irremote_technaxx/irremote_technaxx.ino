/*
 * mapping of key of Technaxx DigiTape DT-02 IR remote
 * Martin Kohlmann 2014-02-02
 */

#include <IRremote.h>

int RECV_PIN = 9;

IRrecv irrecv(RECV_PIN);

decode_results results;


#define DT02_ONOFF   0xFFA25D
#define DT02_STOP    0xFF629D
#define DT02_MUTE    0xFFE21D
#define DT02_MODE    0xFF22DD
#define DT02_REPEAT  0xFF02FD
#define DT02_EQ      0xFFC23D
#define DT02_PREV    0xFFE01F
#define DT02_NEXT    0xFFA857
#define DT02_PLAY    0xFF906F
#define DT02_VOLDOWN 0xFF6897
#define DT02_VOLUP   0xFF9867
#define DT02_0       0xFFB04F
#define DT02_1       0xFF30CF
#define DT02_2       0xFF18E7
#define DT02_3       0xFF7A85
#define DT02_4       0xFF10EF
#define DT02_5       0xFF38C7
#define DT02_6       0xFF5AA5
#define DT02_7       0xFF42BD
#define DT02_8       0xFF4AB5
#define DT02_9       0xFF52AD

char irremote_decode_technnaxx(unsigned long value)
{
  switch(value)
  { 
    case DT02_ONOFF:   return 'o'; 
    case DT02_STOP:    return 's';
    case DT02_MUTE:    return '~';
    case DT02_MODE:    return 'm';    
    case DT02_REPEAT:  return 'r';
    case DT02_EQ:      return 'e';    
    case DT02_PREV:    return '<';
    case DT02_NEXT:    return '>';    
    case DT02_PLAY:    return 'p';
    case DT02_VOLDOWN: return '-';
    case DT02_VOLUP:   return '+';
    case DT02_0:       return '0';
    case DT02_1:       return '1';
    case DT02_2:       return '2';
    case DT02_3:       return '3';
    case DT02_4:       return '4';
    case DT02_5:       return '5';
    case DT02_6:       return '6';
    case DT02_7:       return '7';
    case DT02_8:       return '8';
    case DT02_9:       return '9';
    default:           return 0;
  }
}


void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    char c = irremote_decode_technnaxx(results.value);
    if(c) Serial.println(c);
      
//    dump(&results);
    irrecv.resume(); // Receive the next value
  }
}
