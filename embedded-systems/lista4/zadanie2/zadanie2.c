

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

#define MIN_RANGE 200
#define MAX_RANGE 500


// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

void timer1_init()
{
  // ustaw tryb licznika
  // COM1A = 10   -- non-inverting mode
  // WGM1  = 1110 -- fast PWM top=ICR1
  // CS1   = 101  -- prescaler 1024
  // ICR1  = 15624
  // częstotliwość 16e6/(8*(1+1999)) = 1 kHz
  // wzór: datasheet 20.12.3 str. 164
  ICR1 = 1999;
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS11);
  // ustaw pin OC1A (PB1) jako wyjście
  DDRB |= _BV(PB1);
}

int main()
{
  // zainicjalizuj ADC
  adc_init();
  // zainicjalizuj licznik
  timer1_init();
  // wypełnienie skorygowane funkcją wykładniczą
  uint16_t values[33] = {0, 7, 15, 24, 34, 45, 57, 71, 87, 104, 123, 145, 169, 196, 225, 259, 296, 337, 384, 435, 492, 557, 628, 708, 797, 896, 1007, 1131, 1269, 1422, 1594, 1785, 1999};
  
  while(1) {
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    loop_until_bit_is_clear(ADCSRA, ADSC); // czekaj na wykonanie pomiaru
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)

    if(v <= MIN_RANGE){
      OCR1A = 0;
    } else if (v >= MAX_RANGE){
      OCR1A = ICR1;
    } else {
      OCR1A = values[(32 * ((uint32_t)v - MIN_RANGE) * 100 / (MAX_RANGE - MIN_RANGE)) / 100];
    }
  }
}