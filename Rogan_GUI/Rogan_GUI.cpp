/*!
 * @file Rogan_GUI.cpp
 *
 * @section Decription
 *
 * This is the documentation for the GUI functions of Evelyn's Plant Monitor

 * This library works with the Adafruit 3.5" self 320x480 + Touchscreen Breakout
 * http://www.adafruit.com/products/2050
 *
 * @section dependencies Dependencies
 *
 * The following non-standard Arduino libraries are used:
 * https://github.com/adafruit/Adafruit_GFX
 * https://github.com/adafruit/Adafruit_HX8357_Library
 * 
 * @author James Rogan
 *
 * TODO:
 * Connect rotary encoder functions and GUI functions in main
 * 
 */

#include "Rogan_GUI.h"
#include <avr/wdt.h>

void timerHardwareInit() {
  wdt_reset(); // Reset the watchdog timer
  wdt_enable(WDTO_8S);
  // WDTCSR |= (1 << WDCE) | (1 << WDE); // Enable changing the watchdog settings
  // WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0); // Set prescaler for 10s and enable interrupt
}


//Refresh cell data
void cellDataRefresh(Plant plants[ncols]) { // cellDataArray defined gobally in .h
  for (uint8_t i = 0; i < nrows; i++) {
    for (uint8_t j = 0; j < ncols; j++) {
      if (i ==0) {
        cellDataArray[i][j] = plants[j].percentage;
      } else if (i == 1) {
        cellDataArray[i][j] = plants[j].thresholdLB;
      } else if (i == 2) {
        cellDataArray[i][j] = plants[j].thresholdUB;
      }
    }
  }
}

void displayText(Adafruit_HX8357 self, uint16_t x0, uint16_t y0, const char* text) {
  self.setCursor(x0, y0);  // Set the cursor position
  self.print(text);        // Print the text from RAM (using char pointer)
}

void displayText(Adafruit_HX8357 self, uint16_t x0, uint16_t y0, uint8_t value) {
  self.setCursor(x0, y0);
  self.print(value);
}

//! @note only call if selSection == 0
void menuUpdate(Adafruit_HX8357 self){
// Chart 0: Menu
  if (selCol == 0) {               // If 'MAIN' is selected
    self.setTextSize(3);
    self.setTextColor(HX8357_WHITE);
    displayText(self, 5, 10, "MAIN");
    self.setTextColor(HX8357_BLACK);
    displayText(self, 120, 10, "PLANT MONITOR");
  } else if (selCol == 1) {  //
    self.setTextSize(3);
    self.setTextColor(HX8357_BLACK);
    displayText(self, 5, 10, "MAIN");
    self.setTextColor(HX8357_WHITE);
    displayText(self, 120, 10, "PLANT MONITOR");
  } else {
    self.setTextSize(3);
    self.setTextColor(HX8357_BLACK);
    displayText(self, 5, 10, "MAIN");
    self.setTextColor(HX8357_BLACK);
    displayText(self, 120, 10, "PLANT MONITOR");
  }
}

// selSection  == 1
void namesUpdate(Adafruit_HX8357 self) {
// Chart 1: Names
  for (uint8_t col = 0; col < 4; col++) {
    uint16_t cellOrgX = col * cellWidth + chartOrgX;
    if (col == selCol) {   // must pass function selCol from RE functions
      self.setTextColor(HX8357_CYAN);
    } else {
      self.setTextColor(HX8357_WHITE);
    }
    self.setTextSize(2);
    displayText(self, cellOrgX + 5, chart1OrgY + 15, plantNames[col]);
  }
}
// selSection == 2
void readingsUpdate(Adafruit_HX8357 self) {
// Chart 2: Percentages
  for (uint8_t col = 0; col < 4; col++) {
    uint16_t cellOrgX = col * cellWidth + chartOrgX;
    self.setTextColor(HX8357_WHITE);
    self.setTextSize(2);
    displayText(self, cellOrgX + 5, chart2OrgY + 15, cellDataArray[0][col]);
  }
}

// selSection == 3
void boundUpdate(Adafruit_HX8357 self) {

// Chart 3: the last two rows (containing LB and UB values)
  for (uint8_t row = 0; row < 2; row++) {
    for (uint8_t col = 0; col < 4; col++) {
      uint16_t cellOrgX = col * cellWidth + chartOrgX;
      uint16_t cellOrgY = row * cellHeight + chart3OrgY;

      if (row == selRow && col == selCol) {
        self.setTextColor(HX8357_CYAN);
      } else {
        self.setTextColor(HX8357_WHITE);
      }
      self.setTextSize(2);
      displayText(self, cellOrgX + 5, cellOrgY + 15, cellDataArray[row + 1][col]);
    }
  }
}

void GUIInit(Adafruit_HX8357 self, uint16_t colour1, uint16_t colour2) {
  self.begin();
  self.setRotation(3);
  self.fillScreen(HX8357_BLACK);
  // Top Menu Bar
  self.fillRect(0, 0, 480, 40, colour1);
  self.drawFastVLine(80, 0, 40, colour2);
  self.drawFastVLine(380, 0, 40, colour2);
  self.drawFastVLine(430, 0, 40, colour2);

  // Grid
  for (uint8_t i = 0; i < 4; i++) {
    self.drawFastVLine(80 + 100 * i, 40, 280, HX8357_WHITE);
  }
  for (uint8_t i = 0; i < 4; i++) {
    self.drawFastHLine(0, 160 + 40 * i, 480, HX8357_WHITE);
  }
  // Menu Titles
  for (uint8_t row = 0; row < 5; row++) {
    uint16_t cellOrgY = row * cellHeight + menuOrgY;
    self.setTextColor(HX8357_WHITE);
    self.setTextSize(2);
    displayText(self, 5, cellOrgY + 15, menuArray[row]);
  
  }
}

