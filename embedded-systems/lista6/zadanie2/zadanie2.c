#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <inttypes.h>

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

ISR(USART_RX_vect)
{  
  uint8_t size = in_tail - in_head;
  if(size < 0xFF){
    in_buffer[in_tail++] = UDR0;
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


FILE uart_file;

char * large_sample = "Unwilling sportsmen he in questions september therefore described so. Attacks may set few believe moments was. Reasonably how possession shy way introduced age inquietude. Missed he engage no exeter of. Still tried means we aware order among on. Eldest father can design tastes did joy settle. Roused future he ye an marked. Arose mr rapid in so vexed words. Gay welcome led add lasting chiefly say looking.\r\n\n""Alteration literature to or an sympathize mr imprudence. Of is ferrars subject as enjoyed or tedious cottage. Procuring as in resembled by in agreeable. Next long no gave mr eyes. Admiration advantages no he celebrated so pianoforte unreserved. Not its herself forming charmed amiable. Him why feebly expect future now.\r\n";

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  printf("Hello World!\r\n");

  while(1){
    char message[20];
    uint8_t number;
    scanf("%s", message);
    printf("%s\r\n", message); 

    scanf("%"SCNu8, &number);
    printf("%"PRIu8"\r\n", number);

    printf("%s", large_sample);

    printf("Start writing:\r\n");
    scanf("%"SCNu8, &number);
    for(uint16_t i = 0; i < 1000; i++){
      printf("Test");
    }
    printf("\r\n");
  }
}