#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem
// inicjalizacja UART
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

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc
  DIDR0   = _BV(ADC0D) | _BV(ADC1D); // wyłącz wejście cyfrowe na ADC0, ADC1
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  // zainicjalizuj ADC
  adc_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  DDRB = _BV(PB0);

  uint8_t ceil = 0;
  uint8_t on = 0;
  uint16_t v1 = 0;
  uint16_t v2 = 0;

  while(1) {
    ADMUX &= ~_BV(MUX0);
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    loop_until_bit_is_clear(ADCSRA, ADSC); // czekaj na wykonanie pomiaru
    v1 = ADC;

    ADMUX |= _BV(MUX0);
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    loop_until_bit_is_clear(ADCSRA, ADSC); // czekaj na wykonanie pomiaru
    v2 = ADC;

    uint16_t a1 = (5000 * (uint32_t)v1) >> 10;
    uint16_t a2 = (5000 * (uint32_t)v2) >> 10;
    printf("Collector %"PRIu16"mV, Emitter %"PRIu16 "mV\r\n", a1, a2); 
    
    ceil++;
    if(ceil == 20){
      if(on){
        printf("OFF\r\n");
      } else {
        printf("ON\r\n");
      }
      on = !on;
      ceil = 0;
      PORTB ^= _BV(PB0);
    }
    _delay_ms(50);
  }
}

