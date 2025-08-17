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
  printf("\r\n\nAvailable commands:\r\n");
  printf("read addr length\r\n");
  printf("write\r\n\n");
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

void read(uint16_t addr, uint16_t n)
{
  uint32_t sum = 0;
  printf(":%.2"PRIx16"%.4"PRIx16"00", n, addr);
  // Start reading by choosing device and address of the memory cell
  i2cStart();
  i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
  i2cSend(addr & 0xFF);
  // Start listening for eeprom data
  i2cStart();
  i2cSend(eeprom_addr | ((addr & 0x100) >> 7) | 0x1);
  // Read new data
  for(uint16_t i=0; i < n-1; i++){
    uint8_t data = i2cReadAck();
    sum += data;
    printf("%.2"PRIx16, data);
  }
  // Last data read
  uint8_t data = i2cReadNoAck();
  sum += data;
  printf("%.2"PRIx16, data);
  i2cStop();
  // Calculate checksum
  uint8_t checksum = (sum ^ 0xFF); 
  printf("%.2"PRIx8"\r\n", checksum);
}

void write_page(uint16_t addr, uint8_t * data, uint8_t n)
{
  PORTD &= ~_BV(PD4);
  printf("New page write\r\n");
  // Start by choosing device and starting memory address
  i2cStart();
  i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
  i2cSend(addr & 0xFF);
  // Write maximum one page 
  for(uint8_t i=0; i < n; i++){
    printf("writing: %"PRIx8" under: %"PRIx16"\r\n", data[i], addr+i);
    i2cSend(data[i]);
  }
  i2cStop();
  PORTD |= _BV(PD3);
} 

void write(uint16_t addr, uint8_t * data, uint16_t n)
{
  uint8_t page_data[8];
  uint8_t size;
  // Starting address might not be a multiple of 8 (page size)
  // thus we might need to adjust addr by not doing a full page write
  if(((addr >> 3) << 3) != addr){
    size = (8 - (addr & 0x7));
    if(n < size){
      for(uint8_t i = 0; i < n; i++){
        page_data[i] = data[i];
      }
      
      write_page(addr, page_data, n);
      n = 0;
    } else {
      for(uint8_t i = 0; i < size; i++){
        page_data[i] = data[i];
      }
      
      write_page(addr, page_data, size);
      n -= size;
      addr += size;
      addr &= 0x1ff;
    }
  }

  // Address is now a multiple of 8 - do full page writes

  while(n > 0x8){
    for(uint8_t i = 0; i < 8; i++){
      page_data[i] = data[i];
    }
    write_page(addr, page_data, 8);
    n -= 8;
    addr += 8;
    addr &= 0x1ff;
  }

  // Do a part page write if there is some data left

  if(n > 0){
    for(uint8_t i = 0; i < n; i++){
      page_data[i] = data[i];
    }
    write_page(addr, page_data, n);
  }
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

  DDRD |= _BV(PD4);
  PORTD |= _BV(PD4);

  while(1) {
    uint16_t address = 0;
    uint16_t n = 0;
    uint8_t mode = 0;
    char command[8] = "";
    uint8_t data[512];

    scanf("%s", command);

    if(cmp(command, "read")){
      printf("read ");
      scanf("%"SCNx16, &address);
      printf("%"PRIx16" ", address);
      scanf("%"SCNu16, &n);
      printf("%"PRIu16"\r\n", n);
      printf("Reading %"PRIu16" byte%s from address: 0x%.3"PRIx16"\r\n", n, n != 1 ? "s":"",address);
      while(n > 0xFF)
      {
        read(address, 0xFF);
        address += 0xFF;
        n -= 0xFF;
      }
      read(address, n);
 
    } else if (cmp(command, "write")){
      printf("write\r\n");
      printf("Write data. Input I8HEX:\r\n");
      while(mode == 0x00){
        uint32_t sum = 0;
        uint8_t checksum;
        scanf("%2"SCNx16, &n);
        printf(":%.2"PRIx16, n);
        scanf("%4"SCNx16, &address);
        printf("%.4"PRIx16, address);
        scanf("%2"SCNx8, &mode);
        printf("%.2"PRIx8, mode);
        for(uint16_t i=0; i < n; i++){
          scanf("%2"SCNx8, &data[i]);
          printf("%.2"PRIx8, data[i]);
          sum += data[i];
        }
        scanf("%2"SCNx8, &checksum);
        printf("%.2"PRIx8"\r\n", checksum);
        uint8_t calculated_checksum = (sum ^ 0xFF); 
        if(calculated_checksum != checksum){
          printf("Bad checksum! given: %.2"PRIx8" but was: %.2"PRIx8"\r\n", checksum, calculated_checksum);
          break;
        }

        if(mode == 0x00){
          write(address, data, n);
          printf("Data written\r\n");
        }
      }
      printf("Write ended\r\n");

      } else {
      help();
    }
  }
}

// 10000000ffffffffffffffffffffffffffffffff0f