#include "adc.h"
#include <avr/io.h>

void adc_init(void) {
    // Set Voltage Reference to AVcc (5V) with external capacitor at AREF pin.

    ADMUX = (1 << REFS0);

    // Enable the ADC and set the Prescaler to 128
    // System clock is 16 MHz / 128 = 125 kHz (ADC clock must be between 50-200 kHz)
    // ADEN = Enable, ADPS2/1/0 = 111 for 128 prescaler
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read_raw(uint8_t channel) {

    if (channel > 7) {
        return 0;
    }

    // 0xF0 keeps the reference bits untouched, clears the channel bits
    ADMUX &= 0xF0;

    // Set the new channel (maps directly to the lower 4 bits of ADMUX)
    ADMUX |= channel;

    // Start the conversion by setting the ADSC (ADC Start Conversion) bit
    ADCSRA |= (1 << ADSC);

    // Wait for the conversion to complete
    // The hardware will automatically clear the ADSC bit when it's done (polling)
    while (ADCSRA & (1 << ADSC)) {

    }

    // Return the 10-bit result. 
    // Reading the 'ADC' register reads both ADCL and ADCH correctly combined.
    return ADC;
}

float adc_read_voltage(uint8_t channel) {
    // Get the 10-bit raw value (0 - 1023)
    uint16_t raw_value = adc_read_raw(channel);

    // Convert to voltage using the formula: Voltage = (Raw * Vref) / 1024
    // We cast raw_value to float so the processor emulates float math correctly.
    float voltage = ((float)raw_value * 5.0f) / 1024.0f;

    return voltage;
}
