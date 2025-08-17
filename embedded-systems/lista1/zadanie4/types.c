//LIBS = -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm

#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define P_IN(name) printf("Podaj "name": ")
#define SHOW(spec, var) printf("%"spec"\r\n\n", var)
#define P_OUT_ADD(spec, wynik) printf("\r\nWynik + : %"spec, wynik)
#define P_OUT_MUL(spec, wynik) printf("\r\nWynik * : %"spec, wynik)
#define P_OUT_DIV(spec, wynik) printf("\r\nWynik / : %"spec, wynik)

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

int main(){
	// zainicjalizuj UART
	uart_init();
	// skonfiguruj strumienie wejścia/wyjścia
	fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
	stdin = stdout = stderr = &uart_file;

  printf("Hello world!\r\n");

  while(1){

    uint8_t choice = 0;

    printf("\r\nWybierz format:\r\n0: int8_t\r\n1: int16_t\r\n2: int32_t\r\n3: int64_t\r\n4: float\r\n");
    scanf("%"SCNd8, &choice);

    switch(choice){
      case 0: {
        int8_t a8 = 0;
        int8_t b8 = 0;

        P_IN("a8");
        scanf("%"SCNd8, &a8);
        SHOW(PRId8, a8);

        P_IN("b8");
        scanf("%"SCNd8, &b8);
       	SHOW(PRId8, b8);
        
	int8_t add = a8 + b8;
	int8_t mul = a8 * b8;
	int8_t div = a8 / b8;


        P_OUT_ADD(PRId8, add);
      	P_OUT_MUL(PRId8, mul);
	P_OUT_DIV(PRId8, div);
        break;
	}

      case 1: {

        int16_t a16 = 0;
        int16_t b16 = 0;

        P_IN("a16");
        scanf("%"SCNd16, &a16);
        SHOW(PRId16, a16);
        
	P_IN("b16");
        scanf("%"SCNd16, &b16);
        SHOW(PRId16, b16);

    	int16_t add = a16 + b16;
        int16_t mul = a16 * b16;
       	int16_t div = a16 / b16;

	P_OUT_ADD(PRId16, add);
	P_OUT_MUL(PRId16, mul);
	P_OUT_DIV(PRId16, div);
        break;
	}

      case 2: {
        int32_t a32 = 0;
        int32_t b32 = 0;

        P_IN("a32");
        scanf("%"SCNd32, &a32);
        SHOW(PRId32, a32);
        
	P_IN("b32");
        scanf("%"SCNd32, &b32);
        SHOW(PRId32, b32);
        
        int32_t add = a32 + b32;
        int32_t mul = a32 * b32;
        int32_t div = a32 / b32;

	P_OUT_ADD(PRId32, add);
	P_OUT_MUL(PRId32, mul);
	P_OUT_DIV(PRId32, div);
        break;
	}

      case 3: {
        int32_t a32h = 0;
        int32_t a32l = 0;

        int32_t b32h = 0;
        int32_t b32l = 0;
        
        P_IN("a32h (hex)");
        scanf("%"SCNx32, &a32h);
	SHOW(PRIx32, a32h);
        
	P_IN("a32l (hex)");
	scanf("%"SCNx32, &a32l);
        SHOW(PRIx32, a32l);

        P_IN("b32h (hex)");
        scanf("%"SCNx32, &b32h);
	SHOW(PRIx32, b32h);

        P_IN("b32l (hex)");
        scanf("%"SCNx32, &b32l);
	SHOW(PRIx32, b32l);
        
        int64_t a64 = a32h;
        a64 = (a64 << 32) | a32l;
        int64_t b64 = b32h;
        b64 = (b64 << 32) | b32l;
        
        int64_t radd = a64 + b64;
        int64_t rmul = a64 * b64;
        int64_t rdiv = a64 / b64;

        printf("\r\nWynik +: 0x%"PRIx32" 0x%"PRIx32, (int32_t) (radd >> 32), (int32_t) radd);
        printf("\r\nWynik *: 0x%"PRIx32" 0x%"PRIx32, (int32_t) (rmul >> 32), (int32_t) rmul);
        printf("\r\nWynik /: 0x%"PRIx32" 0x%"PRIx32, (int32_t) (rdiv >> 32), (int32_t) rdiv);

        break;
	}

      case 4: {
        float af = 0.0;
        float bf = 0.0;

        P_IN("af");
        scanf("%f", &af);
        SHOW("f", af);
        
	P_IN("bf");
        scanf("%f", &bf);
        SHOW("f", bf);
        
	float add = af + bf;
	float mul = af * bf;
	float div = af / bf;

        P_OUT_ADD("f", add);
	P_OUT_MUL("f", mul);
	P_OUT_DIV("f", div);
        break;
	}
    }
    printf("\r\n");
  }
}
