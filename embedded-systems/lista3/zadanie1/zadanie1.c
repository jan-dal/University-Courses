// Zadanie 1 z listy 3


#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <math.h>

#define BPM 75

#define NOTE (uint32_t) 240000000/BPM
#define HALF_NOTE (NOTE >> 1)
#define QUARTER_NOTE (NOTE >> 2)
#define EIGHT_NOTE (NOTE >> 3)
#define SIXTEENTH_NOTE (NOTE >> 4)
#define THIRTY_NOTE (NOTE >> 5)

#define BUZZER_DDR DDRC
#define BUZZER_PORT PORTC
#define BUZZER PC5

uint16_t frequency(char note, int8_t octave);
void playTone(uint16_t frequency, uint32_t duration);
void delay(uint32_t time);

int main(){
        
    BUZZER_DDR |= _BV(BUZZER);

    const uint32_t note_time[] = {NOTE, HALF_NOTE, QUARTER_NOTE, QUARTER_NOTE + EIGHT_NOTE, EIGHT_NOTE, EIGHT_NOTE + SIXTEENTH_NOTE, SIXTEENTH_NOTE, THIRTY_NOTE};

    static const char music_notes[] PROGMEM = "DDDD DDDD CC^C^GGEEC CC^C^GGEEC CC^B^A^GFED CC^B^A^GFED CG$AECG$AE CG$AECG$AE C$ RR EE^E^C^C^GGE EE^E^C^C^GGE EE^D^C^B^A^GF EE^D^C^B^A^GF ECEGECEG ECEGECEG E$ R GG^G^E^E^C^C^G GG^G^E^E^C^C^G GG^F^E^D^C^B^A^ GG^F^E^D^C^B^A^ GEGC^GEGC^ GEGC^GEGC^ C R DDDD EEEE FF$F$F$ F$F$F$F$ E$GGGGGG GGGG ACAC ACAC DDDDDDD DDDD GDGD GDGD CCCCCCC CCCC FCFC FCFC BBBBBBB BBBB EBEB EBEB FEFGFGEF GFGAGAFG A^GA^B^A^B^GA^ B^A^B^C^B^C^A^B^ C^ RR C^D^E^F^C^D^E^F^ C^D^E^F^E^D^C^F^ E^D^C^F^E^D^C^F^ E^D^C^FGA^B^C^ D^E^F^FGA^B^C^ D^E^F^D^ R ";
    static const char music_time[] PROGMEM  = "4444 4444 77 7 77777 77 7 77777 77 7 7 7777 77 7 7 7777 77 7777 77 77 7777 77 2  33 77 7 7 7 777 77 7 7 7 777 77 7 7 7 7 77 77 7 7 7 7 77 77777777 77777777 2  2 77 7 7 7 7 7 7 77 7 7 7 7 7 7 77 7 7 7 7 7 7  77 7 7 7 7 7 7  7777 7777  7777 7777  3 3 4444 4444 66 6 6  6 6 6 6  6 777777 6666 6666 6666 6777777 6666 6666 6666 6777777 6666 6666 6666 6777777 6666 6666 6666 77777777 77777777 7 77 7 7 7 77  7 7 7 7 7 7 7 7  4  66 7 7 7 7 7 7 7 7  7 7 7 7 7 7 7 7  7 7 7 7 7 7 7 7  7 7 7 777 7 7  7 7 7 777 7 7  7 7 7 4  7 ";
    
    while(1){
        uint16_t next = 1;
        for(uint16_t i=0; i < sizeof(music_notes)-1; i++){
            char note = pgm_read_byte(&music_notes[i]);
            char time = pgm_read_byte(&music_time[i]);
            if(note == ' '){
                next++;
                continue;
            }
            if(note == 'R'){
                delay(note_time[time - '0']);
                next++;
                continue;
            }
            switch(pgm_read_byte(&music_notes[next])){
                case '^':
                    playTone(frequency(note, 1), note_time[time - '0']);
                    next++;
                    i++;
                    break;
                case '$':
                    playTone(frequency(note, -1), note_time[time - '0']);
                    next++;
                    i++;
                    break;
                default:
                    playTone(frequency(note, 0), note_time[time - '0']);
                    break;
            }
            next++;
        }
        
    }
}

uint16_t frequency(char note, int8_t octave){
    const uint16_t frequencies[] = {415, 466, 523, 554, 622, 698, 784}; // Ab4, Bb4, C5, Db5, Eb5, F5, G5   As-dur
    if(octave < 0){
        return frequencies[note - 'A'] >> -octave;
    } else if (octave > 0){
        return frequencies[note - 'A'] << octave;
    }
    return frequencies[note - 'A'];
}

void playTone(uint16_t frequency, uint32_t duration){
    
    uint16_t cycle_time = 1000000/(uint32_t)frequency;

    uint32_t i = duration/cycle_time;

    for(; i > 0; i--){
        BUZZER_PORT |= _BV(BUZZER);
        delay(cycle_time >> 1);
        BUZZER_PORT &= ~_BV(BUZZER);
        delay(cycle_time >> 1);
    }
}

void delay(uint32_t time){
    while(time-- > 0){
        _delay_us(1);
    }
}

// void delay(uint16_t time){
//     for(uint8_t i=0; i < 32; i++){
//         if((time & _BV(i)) == 1){
//             switch(i){
//                 case 0:
//                     _delay_us(_BV(0));
//                     break;
//                 case 1:
//                     _delay_us(_BV(1));
//                     break;
//                 case 2:
//                     _delay_us(_BV(2));
//                     break;
//                 case 3:
//                     _delay_us(_BV(3));
//                     break;
//                 case 4:
//                     _delay_us(_BV(4));
//                     break;
//                 case 5:
//                     _delay_us(_BV(5));
//                     break;
//                 case 6:
//                     _delay_us(_BV(6));
//                     break;
//                 case 7:
//                     _delay_us(_BV(7));
//                     break;
//                 case 8:
//                     _delay_us(_BV(8));
//                     break;
//                 case 9:
//                     _delay_us(_BV(9));
//                     break;
//                 case 10:
//                     _delay_us(_BV(10));
//                     break;
//                 case 11:
//                     _delay_us(_BV(11));
//                     break;
//                 case 12:
//                     _delay_us(_BV(12));
//                     break;
//                 case 13:
//                     _delay_us(_BV(13));
//                     break;
//                 case 14:
//                     _delay_us(_BV(14));
//                     break;
//                 case 15:
//                     _delay_us(_BV(15));
//                     break;
//             }
//         }
//     }
// }