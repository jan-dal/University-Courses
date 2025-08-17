

// BLUE  - PB1
// GREEN - PB2
// RED   - PB3

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

void delay(uint8_t time);
uint64_t random(uint64_t seed);
uint64_t next_proper_seed(uint64_t seed);
void toggle(uint8_t leds, uint8_t partial, uint8_t brightnes_scaling, uint8_t value);

void counter2_init()
{
  // ustaw tryb licznika
  // COM2B = 11   -- inverting mode
  // WGM2  = 011  -- fast PWM top=0xFF
  // CS2   = 001  -- prescaler 1
  // częstotliwość 16e6/256 = 62.5 kHz

  TCCR2A = _BV(COM2A1) | _BV(COM2A0) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS20);
  // ustaw pin OC2 (PB3) jako wyjście
  DDRB |= _BV(PB3);
}

void counter1_init()
{
  // ustaw tryb licznika
  // COM1A/B = 11 -- inverting mode
  // WGM1  = 0101 -- fast PWM top=0x00FF
  // CS1   = 001  -- prescaler 1
  // częstotliwość 16e6/256 = 62.5 kHz

  TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(CS10);
  // ustaw pin OC1A (PB1) jako wyjście
  DDRB |= _BV(PB1) | _BV(PB2);
}

int main()
{
  // Inicjalizacja liczników
  counter1_init(); // Blue, Green
  counter2_init(); // Red
  
  // Wypełnienie skorygowane funkcją wykładniczą
  uint8_t values[33] = {0, 0, 1, 3, 4, 5, 7, 9, 11, 13, 15, 18, 21, 25, 28, 33, 37, 43, 48, 55, 62, 71, 80, 90, 101, 114, 128, 144, 161, 181, 203, 227, 255};
  // Czasy sinusoidalne
  uint8_t times[33] = {40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 59, 61, 63, 64, 66, 68, 69, 70, 72, 73, 74, 75, 76, 77, 78, 78, 79, 79, 80, 80, 80, 80};
  
  uint8_t i = 0;
  uint8_t leds = 0; // 00000RGB - zawsze tylko 2 wartości wysokie 
  uint8_t partial = 0; // Określa który led nie będzie świecił pełną jasnością
  uint64_t seed = 1;
  uint8_t brightnes_scaling = 0; // skalowanie jasności świecenia jednego koloru
  while(1) {
    seed = next_proper_seed(seed);
    leds = seed & 0x07;
    partial = (seed & 0x08) >> 3; 
    brightnes_scaling = (seed & 0xFF00) >> 8;

    i = 0;
    while(i < 32){
      toggle(leds, partial, brightnes_scaling, values[i]);
      delay(times[i++]);
      //_delay_ms(50);
    }
    while(i > 0){
      toggle(leds, partial, brightnes_scaling, values[i]);
      delay(times[i--]);
      //_delay_ms(50);
    }
  }
}

void delay(uint8_t time){
  while(time-- > 0){
    _delay_ms(1);
  };
}
// Xorshift*
uint64_t random(uint64_t seed){
  seed ^= seed >> 12;
  seed ^= seed << 25;
  seed ^= seed >> 27;
  return seed * 0x2545F4914F6CDD1DULL;
}

void toggle(uint8_t leds, uint8_t partial, uint8_t brightnes_scaling, uint8_t value){

  // Flaga skalowania jasności - 255 dla pełnej jasności
  uint8_t scaling = 255;
  for(uint8_t j=0; j < 3; j++){
    if((leds >> j) & 0x1){
      scaling = (partial == 0) ? brightnes_scaling : 255;
      partial = (partial == 0) ? 1 : 0;
      uint8_t cover = (((uint32_t)value * 1000 * scaling) >> 8 ) / 1000;
      switch(j){
        case 0:
          OCR1A = cover;
          break;
        case 1:
          OCR1B = cover;
          break;
        case 2:
          OCR2A = cover;
          break;
      }
    }
  }
}

uint64_t next_proper_seed(uint64_t seed){
  uint8_t leds = 0;
  do {
      seed = random(seed);
      leds = seed & 0x07; 
  } while(leds == 0 || leds == 7 || leds == 4 || leds == 1 || leds == 2);
  return seed;
}