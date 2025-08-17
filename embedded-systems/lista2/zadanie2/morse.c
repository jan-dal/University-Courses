#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED PB5
#define BUTTON PC4

#define DOT_TIME 300
#define DASH_TIME 900
#define WORD_TIME 2100

uint8_t debounce();
uint16_t measure_idle_time();
uint16_t measure_press_time();
char morse_to_ascii(char * code);

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

int main() {

	// zainicjalizuj UART
	uart_init();
	// skonfiguruj strumienie wejścia/wyjścia
	fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
	stdin = stdout = stderr = &uart_file;

    LED_DDR |= _BV(LED);
    PORTC |= _BV(BUTTON);

    uint16_t press_time = 0;
    uint16_t idle_time = 0;
    uint8_t space = 0;
    char code[10] = "";
    
    while(1){

        if(debounce()){
            press_time = measure_press_time();
        	space = 0;

            if(press_time < DASH_TIME && press_time > 10){
                strcat(code, ".");
            }

            if(press_time >= DASH_TIME){
                strcat(code, "-");
            }

            _delay_ms(DOT_TIME);
            idle_time = measure_idle_time();

            if((idle_time + DOT_TIME) >= DASH_TIME && (idle_time + DOT_TIME) < WORD_TIME){
                printf("%c", morse_to_ascii(code));
                memset(code, 0, 10);
            } 
            else if((idle_time+DOT_TIME) >= WORD_TIME && !space){
                space = 1;
                char result = morse_to_ascii(code);
                if(result > 0){
                    printf("%c ", morse_to_ascii(code));
                }
                memset(code, 0, 10);
            }
        }

        press_time = 0;
        idle_time = 0;
    }
}

uint8_t debounce(){
    if(bit_is_clear(PINC, BUTTON)){
        _delay_ms(1);
        if(bit_is_clear(PINC, BUTTON)){
            return 1;
        }
    }
    return 0;
}

uint16_t measure_idle_time(){
    uint16_t count = 0;
    while(bit_is_set(PINC, BUTTON)){
        _delay_ms(1);
        count++;
        if(count >= DASH_TIME){
            LED_PORT |= _BV(LED);
        }
        if(count >= WORD_TIME){
            LED_PORT &= ~_BV(LED);
            return count;
        }
    }
    LED_PORT &= ~_BV(LED);
    return count;
}

uint16_t measure_press_time(){
    uint16_t count = 1;
    while(bit_is_clear(PINC, BUTTON)){
        _delay_ms(1);
        count++;
        if(count >= DASH_TIME){
            LED_PORT |= _BV(LED);
        }
    }
    LED_PORT &= ~_BV(LED);
    return count;
}

char morse_to_ascii(char * code){
    const char * morse[36] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..","-----",".----","..---","...--","....-",".....","-....","--...","---..","----."};

    for(int i=0; i < 36; i++){
        if(!strcmp(morse[i], code)){
            if(i < 26){
                return 'a' + i;
            } else {
                return '0' + i - 26;
            }
        }
    }
    return '\0';
}