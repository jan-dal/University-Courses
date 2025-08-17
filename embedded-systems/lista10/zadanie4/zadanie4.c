

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

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
  // CS1   = 010  -- prescaler 8
  // częstotliwość 16e6/(8*(1+39999)) = 50Hz
  ICR1 = 39999;
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

  while(1) {
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    loop_until_bit_is_clear(ADCSRA, ADSC); // czekaj na wykonanie pomiaru
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    uint16_t val = 2000 + ((uint32_t)(4000-2000)*(uint32_t)v/1023);
    OCR1A = val; // Impulsy między 1ms - 2ms
  }
}