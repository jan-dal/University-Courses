#include <avr/io.h>
#include <util/delay.h>

#define SWITCH_DDR DDRC
#define SEGMENT_DDR DDRD
#define SEGMENT_LED PORTD
#define SWITCH PORTC

int main() {

    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    SEGMENT_DDR |= 0xff;
    SWITCH_DDR |= (_BV(PC0) | _BV(PC1));

    int8_t mask[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

    uint8_t ones_index = 0;
    uint8_t tens_index = 0;

    while(1){

        for(uint8_t i=0; i < 100; i++){
            SWITCH &= ~_BV(PC0);
            SWITCH |= _BV(PC1);
            SEGMENT_LED = mask[ones_index];
            _delay_ms(5);
            SWITCH &= ~_BV(PC1);
            SWITCH |= _BV(PC0);
            SEGMENT_LED = mask[tens_index];
            _delay_ms(5);
        }

        ones_index++;
        if(ones_index >= 10){
            ones_index = 0;
            tens_index++;
        } 
        if(tens_index >= 10){
            tens_index = 0;
        }
    }
}
