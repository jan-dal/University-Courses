
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <inttypes.h>
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
void uart_wait()
{
  while(!(UCSR0A & _BV(TXC0)));
}
FILE uart_file;

void counter1_init()
{
  
  // Tryb Fast PWM z OCR1A = TOP
  TCCR1A = _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(WGM13);

  // Preskaler 8
  TCCR1B |= _BV(CS11);
  OCR1A = 0xFFFF;
  // Włącz przerwanie zdarzeń
  TIMSK1 = _BV(ICIE1);
}
uint16_t old;
uint8_t count;
uint32_t frequency;

ISR(TIMER1_CAPT_vect)
{
  count++;
  uint16_t new = ICR1;
  
  // Nie możemy wypisywać wartości z każdym wywołaniem przerwania,
  // ponieważ printf drastycznie opóźniałby przedział między old i new
  // Zmienna count powoduje, że wypisujemy co jakiś czas dokładniejszy pomiar
  // W moim przypadku 228 Hz
  if(count == 0x80){
    uint16_t frequency = 16000000/((uint32_t)8*(1+(new - old)));
    printf("Frequency: %"PRIu16"\r\n", frequency);
    count = 0;
  }

  old = new;
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  counter1_init();
  sei(); // Włącz przerwania globalnie
  set_sleep_mode(SLEEP_MODE_IDLE);
  PORTB = _BV(PB0);

  while(1){
    // Przełącz w stan bezczynności
    sleep_mode();
  }
}

