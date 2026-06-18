#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

/**
 * @brief Initialize the hardware.
 */
void display_init(void);

/**
 * @brief Send a number (0-9) to the shift regsiter.
 * @param number to show on display.
 */
void display_show_number(uint8_t number);

/**
 * @brief Clear the display.(Turn off all segments).
 */
void display_clear(void);
/**
 * @brief Choose wich postion u wanna show.
 */
void display_write_pos(uint8_t number, uint8_t pos);

#endif /* DISPLAY_H_ */
