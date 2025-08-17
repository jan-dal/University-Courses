#include <avr/io.h>
#include <inttypes.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

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
  // Włącz przerwania RX
  UCSR0B |= _BV(RXCIE0);

  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
void uart_transmit_byte(char data)
{
  // czekaj aż transmiter gotowy
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = data;
}

ISR(USART_RX_vect)
{
  UDR0 = UDR0;
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  sei();
  set_sleep_mode(SLEEP_MODE_IDLE);

  while(1) {
    // Przełącz w stan bezczynności
    sleep_mode();
  }
}
