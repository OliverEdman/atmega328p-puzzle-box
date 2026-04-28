#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// 'static' gör att variabeln bara syns i denna fil (inkapsling)

static volatile uint32_t millis_counter = 0;

void timer_init(void) {
    /* * Vi använder Timer0 i CTC-läge (Clear Timer on Compare Match).
     * För 16MHz klocka:
     * (16,000,000 Hz / 64 prescaler) = 250,000 ticks/sekund.
     * 250,000 / 1000 (för 1ms) = 250 ticks.
     * Vi sätter OCR0A till 249 (eftersom den räknar 0-249 = 250 steg).
     */

    // Sätt Mode till CTC (WGM01 = 1)
    TCCR0A = (1 << WGM01);

    // Sätt prescaler till 64 (CS01 = 1, CS00 = 1)
    TCCR0B = (1 << CS01) | (1 << CS00);

    // Sätt jämförelsevärdet för 1ms
    OCR0A = 249;

    // Aktivera avbrott när timern når OCR0A
    TIMSK0 = (1 << OCIE0A);

    // Aktivera globala avbrott
    sei();
}

// ISR (Interrupt Service Routine) för Timer0 Compare Match A
ISR(TIMER0_COMPA_vect) {
    millis_counter++;
}

uint32_t timer_get_millis(void) {
    uint32_t current_millis;

    /* * ATmega328P är en 8-bitars MCU. Att läsa en 32-bitars uint32_t tar 4 klockcykler.
     * Om ett avbrott sker mitt i läsningen kan värdet bli korrupt.
     * Därför stänger vi av avbrott (cli) precis under själva kopieringen.
     */
    cli(); 
    current_millis = millis_counter;
    sei(); 

    return current_millis;
}

void timer_delay_ms(uint32_t ms) {
    uint32_t start_time = timer_get_millis();
    
    // Vänta tills differensen mellan nuvarande tid och starttid når 'ms'
    while ((timer_get_millis() - start_time) < ms) {
        // Gör ingenting, bara vänta.
        // Här kan man i framtiden lägga in en 'sleep' för att spara ström.
    }
}