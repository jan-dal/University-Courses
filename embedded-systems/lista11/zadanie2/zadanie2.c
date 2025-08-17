#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include "pid.h"
#include <util/delay.h>

#define K_P     4
#define K_I     10
#define K_D     2

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

void timer1_init()
{
  // ustaw tryb licznika
  // COM1A = 10   -- non-inverting mode
  // WGM1  = 1000 -- PWM phase and frequency correct top=ICR1
  // CS1   = 010  -- prescaler 8
  // częstotliwość 16e6/(2*8*1024) = 976Hz
  ICR1 = 1024;
  TCCR1A = _BV(COM1A1);
  TCCR1B = _BV(WGM13) | _BV(CS11);
  // ustaw pin OC1A (PB1) jako wyjście
  DDRB |= _BV(PB1);
}

void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // włącz przerwania RX
  UCSR0B |= _BV(RXCIE0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

char in_buffer[256];
char out_buffer[256];
volatile uint8_t in_head;
volatile uint8_t in_tail;
volatile uint8_t out_head;
volatile uint8_t out_tail;
volatile uint8_t counter;
volatile uint16_t set_point = 300;
volatile uint8_t modify;
volatile uint16_t temp;
FILE uart_file;

ISR(USART_RX_vect)
{  
  uint8_t size = in_tail - in_head;
  if(size < 0xFF){
    char data = UDR0;
    if(data == 's'){
      modify = 1;
    } else {
      in_buffer[in_tail++] = data;
    }
  }
}

ISR(USART_UDRE_vect)
{
  uint8_t size = out_tail - out_head;
  if(size > 0){
    UDR0 = out_buffer[out_head++];
  } else {
    UCSR0B &= ~_BV(UDRIE0); // Nie ma nic do przesłania, zamaskuj przerwanie
  }
}
// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // zapisz data do out_buffer jeżeli nie jest przepełniony
  cli();
  UCSR0B |= _BV(UDRIE0); // Odmaskuj przerwanie do przesyłania
  uint8_t size = out_tail - out_head;
  while(size == 0xFF){
    sei();
    _delay_ms(1);
    cli();
    size = out_tail - out_head;
  };
  out_buffer[out_tail++] = data;
  sei();
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // Wyłącz przerwania na czas obliczeń
  cli();
  uint8_t size = in_tail - in_head;
  // czekaj aż znak pojawi się w kolejce
  while(size == 0){
    // Włącz przerwania aby zapełnić bufor
    sei();
    _delay_ms(1);
    cli();
    // Sprawdź czy pojawił się nowy element
    size = in_tail - in_head;
  };
  char return_value = in_buffer[in_head++];
  sei();
  return return_value;
}

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0) | _BV(REFS1); // referencja 1.1V
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

uint16_t adc_conversion(uint8_t channel){
  ADMUX &= 0xF0;
  ADMUX |= channel;
  ADCSRA |= _BV(ADSC); // wykonaj konwersję
  loop_until_bit_is_clear(ADCSRA, ADSC); // czekaj na wykonanie pomiaru
  uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
  return v;
}

int main()
{
  uart_init();
  // zainicjalizuj ADC
  adc_init();
  timer1_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  
  DDRB |= _BV(PB5);

  struct PID_DATA pidData;
  pid_Init(128*K_P, K_I, K_D, &pidData);
  int16_t pid_val = 0;
  int16_t old_pid = 0;
  uint16_t ocra = 0;
  while(1) {
    cli();
    if(modify){
      scanf("%"SCNu16, &set_point);
      modify = 0;
    }

    temp = adc_conversion(0);
    temp = (((uint32_t)temp * 1100) >> 10) - 500;

    pid_val = pid_Controller(set_point, temp, &pidData);

    if(pid_val <= 0){
      ocra = 0;
    } else if(pid_val / set_point < 1){
      ocra = 1024*((int32_t)pid_val*1000/set_point)/1000;
    } else {
      ocra = 1024;
    }
    OCR1A = ocra;
    //printf("\r\n%"PRIx16"\r\n",set_point);
    //printf("%"PRIx16"\r\n",temp);
    //printf("%"PRIx16"\r\n\r\n",pid_val);
    printf("Measurment: %"PRIu16".%"PRIu16" set point: %"PRIu16" pid: %"PRId16" OCR1A: %"PRIu16"\r\n", temp/10, temp - ((temp/10)*10), set_point, pid_val, ocra);
    _delay_ms(500);
  }
}