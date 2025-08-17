#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>


#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define DOT_DELAY 300
#define DASH_DELAY 900
#define LETTER_DELAY 900

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

void to_morse_code(char tekst[]);
void blink(const char *code);


int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  printf("Hello world!\r\n");

  LED_DDR |= _BV(LED);
  while(1) {
    char tekst[30];

    scanf("%s", tekst);

    to_morse_code(tekst);

  }
}

void to_morse_code(char tekst[]){
  //a-z0-9
  const char * morse[36] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..","-----",".----","..---","...--","....-",".....","-....","--...","---..","----."};

  for(uint8_t index=0; tekst[index] != '\0'; index++){

    char character = tekst[index];

    if(character >= 'a' && character <= 'z'){
      blink(morse[character-'a']);
    } else if(character >= 'A' && character <= 'Z'){
      blink(morse[character-'A']);
    } else if(character >= '0' && character <= '9'){
      blink(morse[character-'0' + 26]);
    }
    _delay_ms(LETTER_DELAY);
  }

  printf("\r\n");
}

void blink(const char *code){

  for(uint8_t index=0; code[index] != '\0'; index++){
    
    if(code[index] == '.'){
      LED_PORT |= _BV(LED);
      _delay_ms(DOT_DELAY);
      LED_PORT &= ~_BV(LED);
    }
    
    if(code[index] == '-'){
      LED_PORT |= _BV(LED);
      _delay_ms(DASH_DELAY);
      LED_PORT &= ~_BV(LED);
    }
    _delay_ms(DOT_DELAY);
  }

}
