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

void read_date()
{
  i2cStart();
  i2cSend(0xd0);
  i2cSend(0x04);
  i2cStart();
  i2cSend(0xd1);
  uint8_t day = i2cReadAck();
  uint8_t month = i2cReadAck();
  uint8_t year = i2cReadNoAck();
  i2cStop();

  uint8_t day10 = (day & 0x30) >> 4;
  uint8_t day01 = day & 0x0f;

  uint8_t month10 = (month & 0x10) >> 4;
  uint8_t month01 = month & 0x0f;

  uint8_t year10 = (year & 0xf0) >> 4;
  uint8_t year01 = year & 0x0f;

  printf("The date is: %"PRIu8"%"PRIu8"-%"PRIu8"%"PRIu8"-20%"PRIu8"%"PRIu8"\r\n", day10, day01, month10, month01, year10, year01);
}

void read_time()
{
  i2cStart();
  i2cSend(0xd0);
  i2cSend(0x00);
  i2cStart();
  i2cSend(0xd1);
  uint8_t second = i2cReadAck();
  uint8_t minute = i2cReadAck();
  uint8_t hour = i2cReadNoAck();
  i2cStop();

  uint8_t second10 = (second & 0x70) >> 4;
  uint8_t second01 = second & 0x0f;

  uint8_t minute10 = (minute & 0x70) >> 4;
  uint8_t minute01 = minute & 0x0f;
  
  uint8_t hour10 = (hour & 0x30) >> 4;
  uint8_t hour01 = hour & 0x0f;

  printf("The time is: %"PRIu8"%"PRIu8":%"PRIu8"%"PRIu8":%"PRIu8"%"PRIu8"\r\n", hour10, hour01, minute10, minute01, second10, second01);
}

void set_date(uint8_t day10, uint8_t day01, uint8_t month10, uint8_t month01, uint8_t year1000, uint8_t year0100, uint8_t year0010, uint8_t year0001)
{

  printf("Setting date to: %"PRIu8"%"PRIu8"-%"PRIu8"%"PRIu8"-%"PRIu8"%"PRIu8"%"PRIu8"%"PRIu8"\r\n", day10, day01, month10, month01, year1000, year0100, year0010, year0001);

  i2cStart();
  i2cSend(0xd0);
  i2cSend(0x04);
  i2cSend(((day10 << 4) | (day01 & 0x0f)) & 0x3f);
  i2cSend(((month10 << 4) | (month01 & 0x0f)) & 0x1f);
  i2cSend((year0010 << 4) | (year0001 & 0x0f));
  i2cStop();

  printf("Date set\r\n");
}

void set_time(uint8_t hour10, uint8_t hour01, uint8_t minute10, uint8_t minute01, uint8_t second10, uint8_t second01)
{
  printf("Setting time to: %"PRIu8"%"PRIu8":%"PRIu8"%"PRIu8":%"PRIu8"%"PRIu8"\r\n", hour10, hour01, minute10, minute01, second10, second01);

  i2cStart();
  i2cSend(0xd0);
  i2cSend(0x00);
  i2cSend(((second10 << 4) | (second01 & 0x0f)) & 0x7f);
  i2cSend(((minute10 << 4) | (minute01 & 0x0f)) & 0x7f);
  i2cSend(((hour10 << 4) | (hour01 & 0x0f)) & 0x3f);
  i2cStop();
  printf("Time set\r\n");
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

  while(1){

      char command[12] = "";
      scanf("%s", command);
      
      if(cmp(command, "date")){
        read_date();
      
      } else if(cmp(command, "time")){
        read_time();

      } else if(cmp(command, "set")){
        char arg[8] = "";
        scanf("%s", arg);

        if(cmp(arg, "date")){
          printf("set date\r\n");
          uint8_t day10;
          uint8_t day01;
          uint8_t month10;
          uint8_t month01;
          uint8_t year1000;
          uint8_t year0100;
          uint8_t year0010;
          uint8_t year0001;
          scanf("%1"SCNu8"%1"SCNu8"%1"SCNu8"%1"SCNu8"%1"SCNu8"%1"SCNu8"%1"SCNu8"%1"SCNu8, &day10, &day01, &month10, &month01, &year1000, &year0100, &year0010, &year0001);
          set_date(day10, day01, month10, month01, year1000, year0100, year0010, year0001);

        } else if(cmp(arg, "time")){

          printf("set time\r\n");
          uint8_t second10;
          uint8_t second01;
          uint8_t minute10;
          uint8_t minute01;
          uint8_t hour10;
          uint8_t hour01;
          scanf("%1"SCNu8"%1"SCNu8"%1"SCNu8"%1"SCNu8"%1"SCNu8"%1"SCNu8, &hour10, &hour01, &minute10, &minute01, &second10, &second01);
          set_time(hour10, hour01, minute10, minute01, second10, second01);
        }
      } else {
        printf("Bad command\r\n");
      }

  }


}