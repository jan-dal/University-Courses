

#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include "pid.h"

#define K_P     1
#define K_I     1
#define K_D     0

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int uart_transmit(char data)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc
  DIDR0   = _BV(ADC0D) | _BV(ADC1D); // wyłącz wejście cyfrowe na ADC0, ADC1
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

void timer1_init()
{
  // ustaw tryb licznika
  // COM1A = 10   -- non-inverting mode
  // WGM1  = 1000 -- PWM phase and frequency correct top=ICR1
  // CS1   = 010  -- prescaler 8
  // TOIE1 = 1    -- overflow interrupt enable
  // ICIE1 = 1    -- input capture interrupt enable
  // częstotliwość 16e6/(2*8*1024) = 976Hz
  ICR1 = 1024;
  TCCR1A = _BV(COM1A1);
  TCCR1B = _BV(WGM13) | _BV(CS11);
  TIMSK1 = _BV(ICIE1);

  // ustaw pin OC1A (PB1) jako wyjście
  DDRB |= _BV(PB1);
}

static int32_t measurementValue;
static int16_t reference_old;
static int16_t reference_new;
static volatile uint8_t counter;

uint16_t adc_conversion(uint8_t channel){
  ADMUX &= 0xF0;
  ADMUX |= channel;
  ADCSRA |= _BV(ADSC); // wykonaj konwersję
  loop_until_bit_is_clear(ADCSRA, ADSC); // czekaj na wykonanie pomiaru
  uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
  return v;
}

// ISR(TIMER1_OVF_vect) {
//   measurementValue = adc_conversion(1);
//   reference_old = reference_new;
//   reference_new = adc_conversion(0);
//   counter++;
// }


ISR(TIMER1_CAPT_vect) {
  measurementValue += 0x3FF - adc_conversion(1);
  reference_old = reference_new;
  reference_new = adc_conversion(0);
  counter++;
}

void print_hex(uint16_t hex){
  for(uint8_t i=3; i < 0xFF; i--) {
    char half_byte = (hex >> (4*i)) & 0x000F;
    if(half_byte < 10){
      uart_transmit(half_byte + 0x30);
    } else {
      uart_transmit(half_byte - 10 + 0x41);
    }

  }
  uart_transmit(0x0D);
  uart_transmit(0x0A);
}

int main()
{
  uart_init();
  // zainicjalizuj ADC
  adc_init();
  // zainicjalizuj liczniki
  timer1_init();

  struct PID_DATA pidData;
  pid_Init(K_P, K_I, K_D, &pidData);

  sei();
  int16_t pid_val = 0;
  int16_t old_pid = 0;
  while(1) {

    if(counter == 0xFF){
      cli();
      measurementValue = measurementValue/counter;
      pid_val = pid_Controller(reference_old, measurementValue, &pidData);
      pid_val = old_pid + pid_val;
      if(pid_val < 0){
        pid_val = 0;
      }
      //measurementValue = ((uint32_t)measurementValue * 57 / 46) - 486;
      uart_transmit(0x0D);
      uart_transmit(0x0A);
      print_hex(reference_old);
      print_hex(measurementValue);
      print_hex(pid_val);
      //print_hex((measurementValue - reference_old) > 0 ? measurementValue - reference_old : reference_old - measurementValue);
      uart_transmit(0x0D);
      uart_transmit(0x0A);
      measurementValue = 0;
      OCR1A = pid_val;
      old_pid = pid_val;
      sei();
    }
  }
}