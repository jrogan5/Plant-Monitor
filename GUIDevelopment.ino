#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"

#define TFT_CS 10
#define TFT_DC 9

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC);

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
uint8_t LB0 = 20;
uint8_t LB1 = 20;
uint8_t LB2 = 20;
uint8_t LB3 = 20;
// Variables that store the upperbound % of each plant
uint8_t UB0 = 60;
uint8_t UB1 = 60;
uint8_t UB2 = 60;
uint8_t UB3 = 60;
// Capacitive Moisture Sensors
#define snsr0 1 << PC0
#define snsr1 1 << PC1
#define snsr2 1 << PC2
#define snsr3 1 << PC3
// MOSFETs connected to HC595
#define fet0 1 << 0  // 0001
#define fet1 1 << 1  // 0010
#define fet2 1 << 2  // 0100
#define fet3 1 << 3  // 1000

Plant pothos = { snsr0, fet0, 49, LB0, UB0, false };
Plant zzPlant = { snsr1, fet1, 58, LB1, UB1, false };
Plant plant3 = { snsr2, fet2, 41, LB2, UB2, false };
Plant plant4 = { snsr3, fet3, 52, LB3, UB3, false };

#define chartOrgX 80
#define chart1OrgY 160
#define chart2OrgY 200
#define chart3OrgY 240
#define menuOrgY 120
#define nRows 3
#define nCols 4
#define cellWidth 100
#define cellHeight 40

uint8_t selRow = 0;
uint8_t selCol = 0;
uint8_t selSection = 0;

const char* menuArray[5] = {
  "PHOTO:",
  "NAME:",
  "RT%:",
  "LB%:",
  "UB%:"
};

const char* cellTextArray[4] = {
  "Pothos",
  "ZZplant",
  "Plant 3",
  "Plant 4"
};

uint8_t cellDataArray[nRows][nCols] = {
  { pothos.percentage, zzPlant.percentage, plant3.percentage, plant4.percentage },
  { LB0, LB1, LB2, LB3 },
  { UB0, UB1, UB2, UB3 }
};

// Rotary Encoder
#define RE_A 1 << PD5
uint8_t prevA = (PIND & RE_A) ? 1:0;
#define RE_B 1 << PD4
uint8_t prevB = (PIND & RE_B) ? 1:0;
#define RE_SW 1 << PD3
volatile bool REPress = false;

void configureInterrupts() {
  // Rotary encoder
  DDRD &= ~(RE_A | RE_B | RE_SW);
  // PCICR |= (1 << PCIE2);
  // PCMSK2 |= (1 << PCINT20) | (1 << PCINT21);
  EIMSK |= (1 << INT1);   // Rotary encoder's integrated PBNO
  EICRA |= (1 << ISC10);  // falling edge
  sei();
}

void GUIInit(uint16_t colour1, uint16_t colour2) {
  tft.fillScreen(HX8357_BLACK);
  // Top Menu Bar
  tft.fillRect(0, 0, 480, 40, colour1);
  tft.drawFastVLine(80, 0, 40, colour2);
  tft.drawFastVLine(380, 0, 40, colour2);
  tft.drawFastVLine(430, 0, 40, colour2);

  // Grid
  for (uint8_t i = 0; i < 4; i++) {
    tft.drawFastVLine(80 + 100 * i, 40, 280, HX8357_WHITE);
  }
  for (uint8_t i = 0; i < 4; i++) {
    tft.drawFastHLine(0, 160 + 40 * i, 480, HX8357_WHITE);
  }
  // Menu Titles
  for (uint8_t row = 0; row < 5; row++) {
    uint16_t cellOrgY = row * cellHeight + menuOrgY;
    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(2);
    displayText(5, cellOrgY + 15, menuArray[row]);
  }
}

void textUpdate() {
  if (selSection == 0 && selCol == 0) {
    tft.setTextSize(3);
    tft.setTextColor(HX8357_WHITE);
    displayText(5, 10, "MAIN");
    tft.setTextColor(HX8357_BLACK);
    displayText(120, 10, "PLANT MONITOR");
  } else if (selSection == 0 && selCol == 1) {
    tft.setTextSize(3);
    tft.setTextColor(HX8357_BLACK);
    displayText(5, 10, "MAIN");
    tft.setTextColor(HX8357_WHITE);
    displayText(120, 10, "PLANT MONITOR");
  } else {
    tft.setTextSize(3);
    tft.setTextColor(HX8357_BLACK);
    displayText(5, 10, "MAIN");
    tft.setTextColor(HX8357_BLACK);
    displayText(120, 10, "PLANT MONITOR");
  }

  for (uint8_t col = 0; col < 4; col++) {
    uint16_t cellOrgX = col * cellWidth + chartOrgX;
    if (selSection == 1 && col == selCol) {
      tft.setTextColor(HX8357_CYAN);
    } else {
      tft.setTextColor(HX8357_WHITE);
    }
    tft.setTextSize(2);
    displayText(cellOrgX + 5, chart1OrgY + 15, cellTextArray[col]);
  }

  for (uint8_t col = 0; col < 4; col++) {
    uint16_t cellOrgX = col * cellWidth + chartOrgX;
    tft.setTextColor(HX8357_WHITE);
    tft.setTextSize(2);
    displayText(cellOrgX + 5, chart2OrgY + 15, cellDataArray[0][col]);
  }

  for (uint8_t row = 0; row < 2; row++) {
    for (uint8_t col = 0; col < 4; col++) {
      uint16_t cellOrgX = col * cellWidth + chartOrgX;
      uint16_t cellOrgY = row * cellHeight + chart3OrgY;

      if (selSection == 2 && row == selRow && col == selCol) {
        tft.setTextColor(HX8357_CYAN);
      } else {
        tft.setTextColor(HX8357_WHITE);
      }
      tft.setTextSize(2);
      displayText(cellOrgX + 5, cellOrgY + 15, cellDataArray[row + 1][col]);
    }
  }
}


void displayText(uint16_t x0, uint16_t y0, const char* text) {
  tft.setCursor(x0, y0);  // Set the cursor position
  tft.print(text);        // Print the text from RAM
}

void displayText(uint16_t x0, uint16_t y0, uint8_t value) {
  tft.setCursor(x0, y0);
  tft.print(value);
}

void REStuff() {
  uint8_t currentA = (PIND & RE_A) ? 1 : 0;
  uint8_t currentB = (PIND & RE_B) ? 1 : 0;
  if (currentA != prevA || currentB != prevB) {
    if (currentA == 0 && currentB == 1) {
      selCol++;
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
    } else if (currentA == 1 && currentB == 0) {
      selCol--;
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
  }
  prevA = currentA;
  prevB = currentB;
}
ISR(INT1_vect) {
  REPress = true;
}


void setup() {
  tft.begin();
  tft.setRotation(3);
  GUIInit(HX8357_CYAN, HX8357_BLACK);
  configureInterrupts();
}

void loop() {
  REStuff();
  textUpdate();
}
