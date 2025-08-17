#include <avr/io.h>
#include <util/delay.h>

#define SEGMENT_DISPLAY PORTD
#define DISPLAY_DDR DDRD

int main(){

    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    DISPLAY_DDR = 0xff;

    const uint8_t numbers[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
    uint8_t i = 0;

    while(1){
        SEGMENT_DISPLAY = numbers[i++];
        _delay_ms(1000);
        if(i > 9){
            i = 0;
        }
    }

}
