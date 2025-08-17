

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

uint8_t test_signal_and_measurments();
uint8_t detected_ir();

void counter_init()
{
  // ustaw tryb licznika
  // COM1A = 10   -- non-inverting mode
  // CS1   = 001  -- preskaler 1
  // WGM1  = 1110 -- fast PWM top=ICR1
  // ICR1  = 421
  // częstotliwość 16e6/8*(1+421)) -- 37.9 kHz
  
  ICR1 = 421;
  OCR1A = ICR1 >> 1;
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);
}

int main()
{
  // Włącz io do sterowania wewnętrzną diodą
  DDRB |=  _BV(PB5);
  PORTB |= _BV(PB0);
  counter_init();

  while(1) {

    uint8_t measurments = test_signal_and_measurments();

    if(measurments >= 10){  // Turn led on if enough measurments were true
      PORTB |= _BV(PB5);
    } else {
      PORTB &= ~_BV(PB5);
    }
    _delay_ms(100);
  } 
}

uint8_t test_signal_and_measurments(){
  uint8_t measured = 0;

  for(uint8_t i=0; i < 6; i++){
    DDRB |= _BV(PB1); // Output on

    for(uint8_t i=0; i < 6; i++){ // Try detecting low signal on PB0
      if(detected_ir()){
        measured++;
      }
      _delay_us(100);
    }
    DDRB &= ~_BV(PB1); // Output off
    _delay_us(600);
  }
  return measured;
}

uint8_t detected_ir(){
  return !(PINB & _BV(PB0));
}