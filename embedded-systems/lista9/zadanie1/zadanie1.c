#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include "hd44780.h"

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

int hd44780_transmit(char data, FILE *stream)
{
  LCD_WriteData(data);
  return 0;
}

int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE hd44780_file;
static char znaki[16];

void newLine(uint8_t index)
{
  LCD_Clear();
  for(uint8_t i=0; i < index; i++){
    LCD_GoTo(i, 0);
    LCD_WriteData(znaki[i]);
  }
  for(uint8_t i=index; i < 16; i++){
    LCD_GoTo(i, 0);
    LCD_WriteData(0x20);
  }
}


int main()
{
  uart_init();
  // skonfiguruj wyświetlacz
  LCD_Initialize();
  LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_ON | HD44780_CURSOR_BLINK);
  LCD_Clear();
  // skonfiguruj strumienie wyjściowe
  fdev_setup_stream(&hd44780_file, hd44780_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &hd44780_file;
  // program testowy
  printf("Hello world!");
  uint8_t index = 0;
  while(1) {
    if(index >= 16){
      newLine(index);
      index = 0;
    }
    LCD_GoTo(index, 1);
    scanf("%1c", &znaki[index]);
    if(znaki[index] == '\r'){
      newLine(index);
      index = 0;
    } else {
      printf("%c", znaki[index]); 
      index++;
    }
  }
}