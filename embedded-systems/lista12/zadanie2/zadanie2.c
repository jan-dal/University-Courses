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

void timer1_init()
{
  // 16e6/(1024*(1+15624)) = 1Hz
  TCCR1A = _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13);
  TCCR1B |= _BV(CS10) | _BV(CS12);
  ICR1 = 15624;
  TIMSK1 = _BV(TOIE1);
} 

FILE uart_file;

static uint32_t counter = 0;
static volatile uint32_t newValue = 0;
static volatile uint8_t flag = 0;

ISR(TIMER1_OVF_vect) {
  flag = 1;
  newValue = counter;
  counter = 0;
}

ISR(INT0_vect){
  counter++;
}

int main()
{
  timer1_init();
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  DDRD &= ~_BV(PD2);
  PORTD |= _BV(PD2);

  EIMSK = _BV(INT0);
  EICRA = _BV(ISC00) | _BV(ISC01);

  sei();
  while(1) {
    if(flag){
      printf("%"PRIu32"\r\n", newValue);
      flag = 0;
    }
  }
}

