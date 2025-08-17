#include <inttypes.h>
#include <util/delay.h>
#include <avr/io.h>

#define LA PB1
#define OE PB2
#define MOSI PB3
#define SCK PB5

void spi_init()
{
  DDRB |= _BV(LA) | _BV(OE) | _BV(MOSI) | _BV(SCK); // Ustaw pin LA, OE, MOSI, SCK na output
  PORTB |= _BV(OE);

  SPCR |= _BV(SPR0); // 1MHz
  SPCR |= _BV(MSTR); // Ustaw master jako SPI
  SPCR |= _BV(SPE);  // Włącz SPI
}

void send_number(uint8_t number)
{
  PORTB |= _BV(OE);
  PORTB |= _BV(LA); // Rozpocznij transmisję
  SPDR = number;
  loop_until_bit_is_set(SPSR, SPIF);  // Przesyłanie bajtu
  PORTB &= ~_BV(LA); // zakończ transmisję
  PORTB &= ~_BV(OE);
}

uint8_t numbers[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
                //  {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
int main()
{
  spi_init();

  while(1){
    for(uint8_t i = 0; i < 10; i++){
      send_number(numbers[i]);
      _delay_ms(1000);
    }
  }
}