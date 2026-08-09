#include "simon_says.h"
#include "gpio.h"
#include "spi.h"
#include "timer.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TOTAL_LEVELS 7

// Knappar Port D:
// PD3 -> D3 -> Simon button 1 (Pin ID 3)
// PD4 -> D4 -> Simon button 2 (Pin ID 4)
// PD5 -> D5 -> Simon button 3 (Pin ID 5)
#define SIMON_BTN_1 3
#define SIMON_BTN_2 4
#define SIMON_BTN_3 5

// Bit-masker för Reg3 i skiftregistret:
#define SIMON_LED_1_MASK (1 << 0) // Q0 (Pin 15)
#define SIMON_LED_2_MASK (1 << 1) // Q1 (Pin 1)
#define SIMON_LED_3_MASK (1 << 2) // Q2 (Pin 2)
#define LED_KLAR_1_MASK  (1 << 4) // Q4 (Pin 4) - Pussel klar 1!

// Fast sekvens på 7 nivåer (0 = LED1, 1 = LED2, 2 = LED3)
static const uint8_t FIXED_SEQUENCE[TOTAL_LEVELS] = {0, 1, 2, 0, 2, 1, 0};

typedef enum {
    STATE_IDLE,
    STATE_PLAY_SEQUENCE,
    STATE_WAIT_INPUT,
    STATE_SHOW_RESULT,
    STATE_SOLVED
} simon_state_t;

static gpio_t* btns[3] = {NULL, NULL, NULL};
static struct timer* game_timer = NULL;

static uint8_t current_level = 0;
static uint8_t player_step = 0;
static uint8_t sequence_index = 0;

static simon_state_t state = STATE_IDLE;
static volatile int8_t pending_button = -1;
static bool led_active = false;

static void update_shift_registers(uint8_t reg3_data) {
    //  behåll data för Reg1/Reg2 här:
    uint8_t reg2_data = 0x00; 
    uint8_t reg1_data = 0x00; 

    // Skicka i ordning sista till första i kedjan:
    spi_write(reg3_data); // Reg3 (Simon LEDs & LED Klar 1)
    spi_write(reg2_data); // Reg2 (Display Digit & Keypad)
    spi_write(reg1_data); // Reg1 (7-Segment)

    spi_latch(); // Trigga Latch för att uppdatera alla samtidigt
}

// -----------------------------------------------------------------------------
static void on_puzzle_solved(void) {
    state = STATE_SOLVED;
    if (game_timer) timer_stop(game_timer);

    // Tänd LED Klar 1 (Q4 på Reg3) 
    update_shift_registers(LED_KLAR_1_MASK);
}

// -----------------------------------------------------------------------------
static void timer_event_callback(void) {
    if (state == STATE_PLAY_SEQUENCE) {
        if (led_active) {
            update_shift_registers(0x00);
            led_active = false;
            timer_set_timeout_ms(game_timer, 250);
            timer_restart(game_timer);
        } else {
            if (sequence_index <= current_level) {
                uint8_t led_mask = (1 << FIXED_SEQUENCE[sequence_index]);
                update_shift_registers(led_mask);
                led_active = true;
                sequence_index++;
                timer_set_timeout_ms(game_timer, 400);
                timer_restart(game_timer);
            } else {
                timer_stop(game_timer);
                player_step = 0;
                state = STATE_WAIT_INPUT;
            }
        }
    } else if (state == STATE_SHOW_RESULT) {
        update_shift_registers(0x00);
        timer_stop(game_timer);

        if (player_step > current_level) {
            current_level++;

            if (current_level >= TOTAL_LEVELS) {
                on_puzzle_solved();
            } else {
                sequence_index = 0;
                led_active = false;
                state = STATE_PLAY_SEQUENCE;
                timer_set_timeout_ms(game_timer, 500);
                timer_restart(game_timer);
            }
        } else {
            state = STATE_WAIT_INPUT;
        }
    }
}

// -----------------------------------------------------------------------------
static void button_callback(void) {
    if (state != STATE_WAIT_INPUT) return;

    for (uint8_t i = 0; i < 3; i++) {
        if (!gpio_read(btns[i])) { 
            pending_button = (int8_t)i;
            break;
        }
    }
}

// -----------------------------------------------------------------------------
void simon_says_init(void) {
    spi_init();
    update_shift_registers(0x00);

    btns[0] = gpio_new(SIMON_BTN_1, GPIO_DIRECTION_INPUT_PULLUP, button_callback);
    btns[1] = gpio_new(SIMON_BTN_2, GPIO_DIRECTION_INPUT_PULLUP, button_callback);
    btns[2] = gpio_new(SIMON_BTN_3, GPIO_DIRECTION_INPUT_PULLUP, button_callback);

    for (uint8_t i = 0; i < 3; i++) {
        if (btns[i]) gpio_enable_pci(btns[i]);
    }

    game_timer = timer_new(400, timer_event_callback);
}

// -----------------------------------------------------------------------------
void simon_says_start(void) {
    current_level = 0;
    player_step = 0;
    sequence_index = 0;
    led_active = false;
    pending_button = -1;

    state = STATE_PLAY_SEQUENCE;
    timer_set_timeout_ms(game_timer, 400);
    timer_restart(game_timer);
}

// -----------------------------------------------------------------------------
void simon_says_update(void) {
    if (state == STATE_WAIT_INPUT && pending_button != -1) {
        int8_t btn = pending_button;
        pending_button = -1;

        update_shift_registers(1 << btn);

        if (btn == FIXED_SEQUENCE[player_step]) {
            player_step++;
            state = STATE_SHOW_RESULT;
            timer_set_timeout_ms(game_timer, 250);
            timer_restart(game_timer);
        } else {
            // FEL KNAPP 
            update_shift_registers(SIMON_LED_1_MASK | SIMON_LED_2_MASK | SIMON_LED_3_MASK);
            current_level = 0;
            sequence_index = 0;
            led_active = false;
            state = STATE_PLAY_SEQUENCE;
            timer_set_timeout_ms(game_timer, 600);
            timer_restart(game_timer);
        }
    }
}

// -----------------------------------------------------------------------------
void simon_says_stop(void) {
    state = STATE_IDLE;
    if (game_timer) timer_stop(game_timer);
    update_shift_registers(0x00);
}

// -----------------------------------------------------------------------------
bool simon_says_solved(void) {
    return (state == STATE_SOLVED);
}
