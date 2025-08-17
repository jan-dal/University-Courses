
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <inttypes.h>
#include <stdio.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define SAMPLES 50
#define RESOLUTION 1000

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


// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc
  ADMUX  |= _BV(MUX3) | _BV(MUX2) | _BV(MUX1); // Mierz napięcie wewnętrzne 1.1V 
  
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADIE); // Włącz przerwania ADC
  ADCSRA |= _BV(ADEN); // włącz ADC
}

uint16_t min;
uint16_t max;
uint16_t mV;
uint16_t samples[SAMPLES];

uint16_t mode()
{
  uint32_t avg = 0;
  for(uint16_t i=0; i < SAMPLES; i++){
    avg += samples[i];
  }
  return RESOLUTION*avg/SAMPLES;
}

int32_t variance()
{
  uint16_t avg = mode();
  int32_t var = 0;
  for(uint16_t i=0; i < SAMPLES; i++){
    var = (RESOLUTION*samples[i] - avg)*(RESOLUTION*samples[i] - avg);
  }
  return var/SAMPLES;
}

void normal_measure()
{
  printf("Start ADC sampling in normal mode \r\n");

  min = 0xFFFF;
  max = 0;

  for(uint16_t i=0; i < SAMPLES; i++){
    ADCSRA |= _BV(ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    mV = ((uint32_t)ADC * 5000) >> 10;
    samples[i] = mV;
    min = mV < min ? mV : min;
    max = mV > max ? mV : max;
  }
  printf("Min: %"PRIu16"mV Max: %"PRIu16"mV Variance: %"PRId32"\r\n", min, max, variance());
}

void noise_reduction_measure()
{
  printf("Start ADC sampling in noise reduction mode \r\n");
  uart_wait();
  _delay_ms(400);

  SMCR = _BV(SM0); // Ustaw tryb redukcji szumu

  for(uint16_t i=0; i < SAMPLES; i++){
    // Uśpij procesor, ADC wykona pomiar automatycznie i wywoła odpowiednie przerwanie na koniec
    sleep_mode();  
    samples[i] = mV;
    min = mV < min ? mV : min;
    max = mV > max ? mV : max;
  }
  printf("Min: %"PRIu16"mV Max: %"PRIu16"mV Variance: %"PRId32"\r\n", min, max, variance());
}

ISR(ADC_vect)
{
  mV = ((uint32_t)ADC * 5000) >> 10;
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  adc_init();
  sei(); // Włącz przerwania globalnie

  while(1){
    normal_measure();
    noise_reduction_measure();
    printf("\r\n");
    _delay_ms(5000);
  }
}


