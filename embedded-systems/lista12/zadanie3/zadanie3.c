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
static uint16_t old;
static uint16_t new;

void timer1_init()
{
  // 16e6/(1024*(1+31249)) = 0.5Hz
  TCCR1A = _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(WGM13);
  TCCR1B |= _BV(CS10) | _BV(CS12);
  OCR1A = 31249;
  TIMSK1 = _BV(ICIE1) | _BV(TOIE1);
} 


ISR(TIMER1_CAPT_vect)
{
  old = new;
  new = ICR1;
}

ISR(TIMER1_OVF_vect)
{
  printf("%"PRIu16"µs\r\n", (uint16_t)((uint32_t)(new-old)*4000000/(OCR1A+1))); // 2 * 2 * (range * 1_000_000) / (OCR1A+1)
  // 1) Rozszerz new-old do mikrosekund  (range * 1_000_000)
  // 2) Podziel przez OCR1A+1 uzystkując proporcję range do okresu 2 sekund
  // 3) Pomnóż razy 2 ponieważ liczymy czas połowy fali (wyzwalamy 2 razy na 1 okres)
  // 4) Pomnóż razy 2 ponieważ chcemy proporcję do 1 sekundy
}

int main()
{
  timer1_init();
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  PORTD = _BV(PD6) | _BV(PD7);
  DIDR1 = _BV(AIN0D) | _BV(AIN1D);
  ACSR = _BV(ACIC);

  sei();
  while(1) {

  }
}
