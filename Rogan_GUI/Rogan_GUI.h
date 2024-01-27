/*!
 * @file Rogan_GUI.h
 * 
 * TODO:
 * Timer stuff? WDT? Possibly reduce power consumption 
 *      write timerHardwareInit()
*/

#ifndef ROGAN_GUI_H
#define ROGAN_GUI_H

#include <JamesLib.h>
#include <Rogan_RotaryEnc.h>

#define cellWidth 100
#define cellHeight 40

#define chartOrgX 80      
#define chart1OrgY 160  
#define chart2OrgY chart1OrgY + cellHeight
#define chart3OrgY chart2OrgY + cellHeight
#define menuOrgY 120
#define nrows 3
#define ncols 4

// SPI for TFT
// #define SCK      1<<PB5  //Hardware predefines
// #define MISO     1<<PB4
// #define MOSI     1<<PB3
#define TFT_DC      1<<PB2
#define TFT_CS      1<<PB1
// #define TFT_RST     -1
uint8_t currentScreen = 0;

//! @todo Timer stuff used (possibly) to update GUI
#define CPU_F 16000000UL
#define CompareValueTIMER1 (CPU_F * 4 / 1024)

const char* plantNames[nplants] = {
    "pothos", 
    "zzPlant", 
    "Plant3", 
    "Plant4"
};

const char* menuArray[5] = {
  "PHOTO:",
  "NAME:",
  "RT%:",
  "LB%:",
  "UB%:"
};

volatile uint8_t cellDataArray[nrows][ncols] = {
  { plants[0].percentage, plants[1].percentage, plants[2].percentage, plants[3].percentage },
  { plants[0].thresholdLB, plants[1].thresholdLB, plants[2].thresholdLB, plants[3].thresholdLB },
  { plants[0].thresholdUB, plants[1].thresholdUB, plants[2].thresholdUB, plants[3].thresholdUB }
};

// Timer stuff
void timerHardwareInit();

//Refresh cell data
void cellDataRefresh(Plant plants[ncols]);

// Next two are overloaded
void displayText(Adafruit_HX8357 self, uint16_t x0, uint16_t y0, uint8_t value);

void displayText(Adafruit_HX8357 self, uint16_t x0, uint16_t y0, const char* text);

//Custom functions for GUI tasks
void menuUpdate(Adafruit_HX8357 self);

void namesUpdate(Adafruit_HX8357 self);

void readingsUpdate(Adafruit_HX8357 self);

void boundUpdate(Adafruit_HX8357 self);

void GUIInit(Adafruit_HX8357 self, uint16_t colour1, uint16_t colour2);


#endif