
/*!
 * @file JamesLib.h
 * 
 * TODO:
 * 
*/


#ifndef JAMESLIB_H
#define JAMESLIB_H


#include <Adafruit_GFX.h>
#include <Adafruit_HX8357.h>

#define nplants 4

// Some useful macros
#define m_setPin(port,pinNumber)     ((*port) |= (pinNumber))       // Function-like macro further optimizes memory. 
#define m_clearPin(port,pinNumber)   ((*port) &= ~(pinNumber))

// Pump Control
#define PC_SRCLK    1<<PB0
#define PC_RCLK     1<<PD7
#define PC_CLK      1<<PD6  

// Data structure used for a plant
struct Plant {
  uint8_t snsrPin;
  uint8_t mosfet;
  uint8_t percentage;
  uint8_t thresholdLB;
  uint8_t thresholdUB;
  bool waterNeeded;
};

// Variables that store the Lowerbound % of each plant
// See this resource for details: https://eos.com/blog/soil-moisture/
uint8_t LB[nplants] = {20, 20, 20, 20};
uint8_t UB[nplants] = {60, 60, 60, 60};

const uint8_t sensorPins[nplants] = {
    1 << PC0,
    1 << PC1, 
    1 << PC2, 
    1 << PC3
};

Plant plants[nplants] {
  {sensorPins[0], (1 << 0), 50, LB[0], UB[0], false},
  {sensorPins[1], (1 << 1), 50, LB[1], UB[1], false},
  {sensorPins[2], (1 << 2), 50, LB[2], UB[2], false},
  {sensorPins[3], (1 << 3), 50, LB[3], UB[3], false}
};   // Initialize array of plants

void controllerHardwareInit();

void outShift(volatile uint8_t *port, uint8_t Data, uint8_t Clock, uint8_t bitOrder, uint8_t val);

void interpretReading(Plant plant); // Returns if plant needs water

// void dispense(Plant plant, uint8_t Data, uint8_t Clock); // only called for plant when plant needs water

#endif