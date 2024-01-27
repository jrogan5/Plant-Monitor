
/*!
 * @file Rogan_RotaryEnc.cpp
 * @author James Rogan
 * @date 22/12/2023
 * @section Decription
 * Handles the rotary encoder input for the UI of Evelyn's Plant Monitor
 * 
 * TODO:
 * Fix bug where every other RE turn is ignored
 * Fix bug where can't turn backwards
 * Write func for ... (forgot lol)
 * Set selected col/row/section to zero in initilization
 * 
*/

#include "Rogan_RotaryEnc.h"

void REHardwareInit(void) {
    uint8_t prevA = (PIND & RE_A) ? 1:0;
    uint8_t prevB = (PIND & RE_B) ? 1:0;
    // Rotary encoder pins
    DDRD &= ~(RE_A | RE_B | RE_SW);
    PORTB |= RE_A | RE_B | RE_SW;

    // PCICR |= (1 << PCIE2);       // Pin change interrupts on RE pins
    // PCMSK2 |= (1 << PCINT20) | (1 << PCINT21);
    EIMSK |= (1 << INT1);   // Rotary encoder's integrated PBNO
    EICRA |= (1 << ISC10);  // falling edge

}   

void REInterperet() {
  uint8_t currentA = PIND & RE_A;
  uint8_t currentB = PIND & RE_B;
  uint8_t currentEnc = currentA << 1 | currentB;
  uint8_t sumEnc = prevEnc << 2 | currentEnc;
  if (sumEnc == 0b1110 || sumEnc == 0b0111 || sumEnc == 0b0001 || sumEnc == 0b1000) {
    selCol++; // clockwise turn
    if (selSection == 0 && selCol > 1) {
      selCol = 0;
      selSection++;
    } else if (selSection == 1 && selCol > 3) {
      selCol = 0;
      selSection++;
    } else if (selSection == 2 && selCol > 3 && selRow == 0) {
      selCol = 0;
      selRow++;
    } else if (selSection == 2 && selCol > 3 && selRow == 1) {
      selCol = 0;
      selRow = 0;
      selSection = 0;
    }
  } else if (sumEnc == 0b1101 || sumEnc == 0b0100 || sumEnc == 0b0010 || sumEnc == 0b1011) {
    selCol--; // counter-clockwise turn
    if (selSection == 0 && selCol == 255) {
      selCol = 3;
      selRow = 1;
      selSection = 2;
    } else if (selSection == 1 && selCol == 255) {
      selCol = 1;
      selSection--;
    } else if (selSection == 2 && selCol == 255 && selRow == 0) {
      selCol = 3;
      selSection--;
    } else if (selSection == 2 && selCol == 255 && selRow == 1) {
      selCol = 3;
      selRow--;
    }
  }
  prevEnc = currentEnc;
}
