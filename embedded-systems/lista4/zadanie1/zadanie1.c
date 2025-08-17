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

// Poprawka na instrukcje wczytania i zapisu
#define ADJ(time, bit) time - (4 + 3*(bit >> 2))

#define TIME_ADD(a, b, r, bit) start = TCNT1; r = a + b; stop = TCNT1; printf("Measured addition time: %"PRIu16"\r\n", ADJ(stop - start, bit));
#define TIME_SUB(a, b, r, bit) start = TCNT1; r = a - b; stop = TCNT1; printf("Measured subtraction time: %"PRIu16"\r\n", ADJ(stop - start, bit));
#define TIME_MUL(a, b, r, bit) start = TCNT1; r = a * b; stop = TCNT1; printf("Measured multiplication time: %"PRIu16"\r\n", ADJ(stop - start, bit));
#define TIME_DIV(a, b, r, bit) start = TCNT1; r = a / b; stop = TCNT1; printf("Measured division time: %"PRIu16"\r\n", ADJ(stop - start, bit));

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

void timer1_init(){
	TCCR1B = _BV(CS10);
}

int main(){
	// zainicjalizuj UART
	uart_init();
	// skonfiguruj strumienie wejścia/wyjścia
	fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
	stdin = stdout = stderr = &uart_file;

	timer1_init();

	uint16_t start = 0;
	uint16_t stop  = 0;

  while(1){

    uint8_t choice = 0;

    printf("\r\nWybierz format:\r\n0: int8_t\r\n1: int16_t\r\n2: int32_t\r\n3: int64_t\r\n4: float\r\n");
    scanf("%"SCNd8, &choice);

    switch(choice){
      case 0: {
        volatile int8_t a8 = 0;
        volatile int8_t b8 = 0;
				volatile int8_t result = 0;

        P_IN("a8");
        scanf("%"SCNd8, &a8);
        SHOW(PRId8, a8);

        P_IN("b8");
        scanf("%"SCNd8, &b8);
       	SHOW(PRId8, b8);
        
				TIME_ADD(a8, b8, result, 8);
				TIME_SUB(a8, b8, result, 8);
				TIME_MUL(a8, b8, result, 8);
        TIME_DIV(a8, b8, result, 8);
				break;
			}

      case 1: {

        volatile int16_t a16    = 0;
        volatile int16_t b16    = 0;
				volatile int16_t result = 0;
        
				P_IN("a16");
        scanf("%"SCNd16, &a16);
        SHOW(PRId16, a16);
        
				P_IN("b16");
        scanf("%"SCNd16, &b16);
        SHOW(PRId16, b16);

				TIME_ADD(a16, b16, result, 16);
				TIME_SUB(a16, b16, result, 16);
				TIME_MUL(a16, b16, result, 16);
        TIME_DIV(a16, b16, result, 16);
        break;
			}

      case 2: {
        volatile int32_t a32    = 0;
        volatile int32_t b32    = 0;
				volatile int32_t result = 0;

        P_IN("a32");
        scanf("%"SCNd32, &a32);
        SHOW(PRId32, a32);
        
				P_IN("b32");
        scanf("%"SCNd32, &b32);
        SHOW(PRId32, b32);
        
        TIME_ADD(a32, b32, result, 32);
				TIME_SUB(a32, b32, result, 32);
				TIME_MUL(a32, b32, result, 32);
        TIME_DIV(a32, b32, result, 32);
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
        
        volatile int64_t a64 = a32h;
        a64 = (a64 << 32) | a32l;
        volatile int64_t b64 = b32h;
        b64 = (b64 << 32) | b32l;

				volatile int64_t result = 0;
        
        TIME_ADD(a64, b64, result, 64);
				TIME_SUB(a64, b64, result, 64);
				TIME_MUL(a64, b64, result, 64);
        TIME_DIV(a64, b64, result, 64);
        break;
			}

      case 4: {
        // Float jest w osobnej funkcji, ponieważ kompilator generuje
        // dodatkowe instrukcje zapisu i odczytu dla wszystkich pomiarów 
        // gdy pod tym casem zrobimy zmienną float volatile
				measure_time_float();
        break;
			}
    }
    printf("\r\n");
  }
}

void measure_time_float(){
	volatile float af     = 0;
	volatile float bf     = 0;
	volatile float result = 0;
	uint16_t start = 0;
	uint16_t stop  = 0;

	P_IN("af");
	scanf("%f", &af);
	SHOW("f", af);
	
	P_IN("bf");
	scanf("%f", &bf);
	SHOW("f", bf);


	TIME_ADD(af, bf, result, 32);
	TIME_SUB(af, bf, result, 32);
	TIME_MUL(af, bf, result, 32);
	TIME_DIV(af, bf, result, 32);
}