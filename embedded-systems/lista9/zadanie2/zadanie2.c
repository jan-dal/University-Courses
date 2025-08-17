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

void LCD_LoadCGRAM()
{

  LCD_WriteCommand(HD44780_CGRAM_SET);
  uint8_t bar = 0x10;
  for(uint8_t i=0; i < 5; i++){
    for(uint8_t j=0; j < 7; j++){
      LCD_WriteData(bar);
    }
    LCD_WriteData(0);
    bar |= (bar >> 1);
  }
  
  LCD_WriteCommand(HD44780_DDRAM_SET);
}

int main()
{
  uart_init();
  // skonfiguruj wyświetlacz
  LCD_Initialize();
  LCD_LoadCGRAM();
  LCD_Clear();
  // skonfiguruj strumienie wyjściowe
  fdev_setup_stream(&hd44780_file, hd44780_transmit, uart_receive, _FDEV_SETUP_RW);
  stdout = stderr = &hd44780_file;
  


  while(1) {
    printf("Hello world!");
    for(uint8_t j=0; j < 16; j++){
      for(uint8_t i=0; i < 5; i++){
        LCD_GoTo(j, 1);
        LCD_WriteData(i);
        _delay_ms(100);
      }
    }
    _delay_ms(2000);
    LCD_Clear();
  }
}