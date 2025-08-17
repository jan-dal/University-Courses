#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>
#include <math.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define V_REF 5000
#define BETA 3850
#define BASE_RESISTANCE 4700
#define BASE_TEMPERATURE 298.15
// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
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

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja 5V, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

FILE uart_file;

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();

  while(1) {

    ADCSRA |= _BV(ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC); 

    uint16_t v = ADC;

    uint32_t mV = ((uint32_t)v * V_REF) >> 10;

    uint32_t resistance = (10000*mV)/(V_REF-mV);
    float log_result = logf((float)resistance/BASE_RESISTANCE);
    float temperature = ((float)BETA * BASE_TEMPERATURE)/(log_result*BASE_TEMPERATURE + BETA)-273.15;

    printf("Odczytano: %"PRIu32 "mV ,temperatura: %.2f\r\n", mV, temperature);
    _delay_ms(1000);
  }
}

// Odczyty

// 19.8C   1923mV -> 6249.59Ohm  b = 4786
// 19.4C   1879mV -> 6020.51Ohm  b = 3850

