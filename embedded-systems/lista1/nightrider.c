#include <avr/io.h>
#include <util/delay.h>

#define LED_STRIP PORTD
#define LED_DDR DDRD

#define DELAY 100

int main() {

  UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
  LED_DDR = 0xff;

  uint8_t i = 0;

  while(1){
    
    while(i < 7){
        LED_STRIP = _BV(i);
        _delay_ms(DELAY);
        i++;
    }


    while(i > 0){
        LED_STRIP = _BV(i);
        _delay_ms(DELAY);
        i--;
    }

  }
}

