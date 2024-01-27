/*!
 * @file JamesLib.cpp
 * @author James Rogan
 * @date 23/12/2023
 * @section Decription
 * Some useful functions for the plant monitor
 * 
 * TODO:
 * 
 */

#include "JamesLib.h"

// Hardware initialization functions
void controllerHardwareInit() {
  DDRD |= PC_SRCLK | PC_RCLK | PC_CLK;
  // Inputs by defult, but just in case:
  DDRC &= ~(plants[0].snsrPin | plants[1].snsrPin | plants[2].snsrPin | plants[3].snsrPin);
}

void outShift (volatile uint8_t *port, uint8_t Data, uint8_t Clock, uint8_t bitOrder, uint8_t val) {
  for ( uint8_t i = 0; i < 8; i++)  {
    if (bitOrder) {
      (val & (1 << (7 - i))) ? m_setPin((port), Data) : m_clearPin((port), Data);             // Asks a question:
    }                                                                                         // Is the bit in posion i set?
    else {                                                                                    // If yes, set the data pin. If no, don't.
      (val & (1 << i)) ? m_setPin((port), Data) : m_clearPin((port), Data);
    }
    //rising edge after presenting one bit to the shift register
    m_setPin((port), Clock);
    m_clearPin((port), Clock);
  }
}

void interpretReading (struct Plant plant) {
  if (plant.percentage < plant.thresholdLB) {
      plant.waterNeeded = true;
  } else if (plant.percentage > plant.thresholdUB)
      plant.waterNeeded = false;
  // else, no change! this makes it so that there is hysteresis
}
/*
void dispense (Plant plant, uint8_t Data, uint8_t Clock) {
  if (plant.waterNeeded) {
      outShift(&PORTD, Data, Clock, LSBFIRST, plant.mosfet);
      while (plant.waterNeeded) {
        plant.percentage = readSensor(plant);
        updateReadings(____);
      }
      // After done dispensing
      outShift(&PORTD, Data, Clock, LSBFIRST, 0x00);
  }
}
*/