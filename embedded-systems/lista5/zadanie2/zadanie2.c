

#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

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
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADATE); // Włącz ADC Auto Trigger
  ADCSRA |= _BV(ADIE); // Włącz ADC Interrupt 
  ADCSRA |= _BV(ADEN); // włącz ADC

  ADCSRB = _BV(ADTS1); // Auto Trigger przy INT0
}

void io_init()
{
  // Inicjalizacja dla INT0
  PORTD |= _BV(PD2);
  // Zbocze narastające
  EICRA |= _BV(ISC00) | _BV(ISC01);
}

volatile uint32_t ohm; // Ostatnio zmierzony opór
volatile uint32_t v;

ISR(ADC_vect)
{
  v = ((uint32_t)ADC * 5000) >> 10;  // Zmierzone napięcie w mV

  ohm = (10000 * v) / (5000 - v); // Przekształć na opór

  EIFR |= _BV(INTF0);  // Wyczyść flagę INT0
}


int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  adc_init();
  io_init();
  sei(); // Włącz system przerwań

  while(1) {
    _delay_ms(2000);
    printf("Ostatnio zmierzony opór wynosił: %"PRIu32"\r\n", ohm);
  }
}

// Naciśnięcie przycisku wyzwala INT0, który z kolei wyzwala ADC.
// ADC po wykonaniu pomiaru automatycznie wywołuje przerwanie.
// W przerwaniu odczytywany jest wykonany pomiar i przekształcany
// do wartości oporu fotorezystora