#include "i2c.h"
#include <avr/io.h>
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

void i2cInit() {
  // ustaw bitrate
  // 8MHz / (16+2*TWBR*1) ~= 100kHz
  TWBR = 32;
  // uruchom TWI
  TWCR |= _BV(TWEN);
}
void i2cWaitForComplete() {
  // czekaj na flagę TWINT
  while(!(TWCR & _BV(TWINT)));
}
void i2cStart() {
  // wyślij warunek startu
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTA);
  i2cWaitForComplete();
}
void i2cStop() {
  // wyślij warunek stopu
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
}
void i2cReset() {
  // wyślij warunek stopu i wyłącz TWI
  TWCR = _BV(TWINT) | _BV(TWSTO);
}
uint8_t i2cReadAck() {
  // odczytaj dane, wyślij ACK
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
  i2cWaitForComplete();
  return TWDR;
}
uint8_t i2cReadNoAck() {
  // odczytaj dane, wyślij NOACK
  TWCR = _BV(TWINT) | _BV(TWEN);
  i2cWaitForComplete();
  return TWDR;
}
void i2cSend(uint8_t data) {
  // wyślij dane
  TWDR = data;
  TWCR = _BV(TWINT) | _BV(TWEN);
  i2cWaitForComplete();
}

void help()
{
  printf("Available commands:\r\n");
  printf("read addr\r\n");
  printf("write addr value\r\n");
}

uint8_t len(char * str)
{
  uint8_t i = 0;
  while(str[i++] != '\0');
  return i-1;
}

uint8_t cmp(char * str1, char * str2)
{
  uint8_t a = len(str1);
  uint8_t b = len(str2);

  if(a != b){
    return 0;
  }
  for(uint8_t i=0; i < a; i++){
    if(str1[i] != str2[i]){
      return 0;
    }
  }
  return 1;
}

uint8_t eeprom_addr = 0xA0;

uint8_t read(uint16_t addr)
{
  i2cStart();
  i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
  i2cSend(addr & 0xFF);
  i2cStart();
  i2cSend(eeprom_addr | ((addr & 0x100) >> 7) | 0x1);
  uint8_t data = i2cReadNoAck();
  i2cStop();
  return data;
}

void write(uint16_t addr, uint8_t val)
{
  //PORTD &= ~_BV(PD3);
  i2cStart();
  i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
  i2cSend(addr & 0xFF);
  i2cSend(val);
  i2cStop();
  //PORTD |= _BV(PD3);
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  // zainicjalizuj i2c
  i2cInit();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  
  //DDRD |= _BV(PD3);
  //PORTD |= _BV(PD3);

  while(1) {
    uint16_t address = 0;
    uint8_t value    = 0;
    char command[10] = "";

    scanf("%s %"SCNx16, command, &address);

    if(cmp(command, "read")){
      printf("Reading address: 0x%"PRIx16"\r\n", address);
      uint8_t read_value = read(address);
      printf("Value: 0x%"PRIx8"\r\n", read_value);

    } else if (cmp(command, "write")){
      scanf("%"SCNx8, &value);
      printf("Writing 0x%"PRIx8" to address: 0x%"PRIx8"\r\n", value, address);
      write(address, value);
      printf("Done \r\n");
      } else {
      help();
    }
  }
}