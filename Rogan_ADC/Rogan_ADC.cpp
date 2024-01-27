/*!
 * @file Rogan_ADC.cpp
 * @author James Rogan
 * @date 23/12/2023
 * @section Decription
 * Handles ADC functionality for Evelyn's plant monitor
 * 
 * TODO:
 * Test it! Does the read function actually work?
 * 
 * 
 * 
*/

#include "Rogan_ADC.h"

void ADCHardwareInit() {
    // Set refrence voltage AVCC to 5V
    //! (TODO: an external 1 uF cap connecting AREF and 5V)
    ADMUX |= (1 << REFS0) | (1 << REFS1); 
    // Set prescaler value to 128 for increased accuracy. Enable ADC
    ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2) | (1 << ADEN);
}

void readSensor(Plant plant) {
    ADMUX = (ADMUX & 0xF0) | plant.snsrPin; // select 0,1,2,3
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC)); // wait...
    plant.percentage = (ADCH / 255) * 100;
    // SHOULD delay between readings, but will assume that
    // GUI update function should provide sufficient delay
    // _delay_ms(10);

}