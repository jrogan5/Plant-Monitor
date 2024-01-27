
/*!
 * @file Rogan_RotaryEnc.h
 * 
 * TODO:
 * 
 * 
*/

#include <JamesLib.h>



// Rotary Encoder
#define RE_A 1 << PD5
#define RE_B 1 << PD4
#define RE_SW 1 << PD3
volatile bool REPress = false;
uint8_t prevEnc = 0b00; // used for sum of current and previous to determine direction 

uint8_t selSection = 0;
uint8_t selRow = 0;
uint8_t selCol = 0;


void REHardwareInit(void);

void REInterperet();