#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/sleep.h>


void counter2_init()
{

  TCCR2B = _BV(CS22) | _BV(CS21); // Preskaler 256
  TCCR2A = _BV(WGM21); // CTC TOP = OCRA
  OCR2A = 249; // 16000000/(2*(249+1)*256) = 125 Hz

}

void counter2_interrupt_init()
{
  TIMSK2 = _BV(OCIE2B); // Włącz flagę compare match
}

void pins_init()
{
  DDRC = _BV(PC5);
  PORTD = _BV(PD2);
}

volatile uint8_t index;
// 125 Razy wywołamy przerwanie w trakcie sekundy
volatile uint8_t flags[125];  

ISR(TIMER2_COMPB_vect)
{
  if(flags[index]){
    PORTC |= _BV(PC5);
    flags[index] = 0;
  } else {
    PORTC &= ~_BV(PC5);
  }
  if(bit_is_clear(PIND, PD2)){
    flags[index] = 1;
  }
  index++;
  if(index >= 125){
    index = 0;
  }
}

int main()
{
  counter2_init();
  counter2_interrupt_init();
  pins_init();
  sei(); // Włącz przerwania globalnie

  while(1){
    // W trybie power save licznik 2 nadal pracuje 
    // poza tym tryb jest identyczny do power down
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_mode();  
  }
}