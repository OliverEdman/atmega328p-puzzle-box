#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

/**
 * @brief Initierar displayen och dess beroenden (SPI).
 */
void display_init(void);

/**
 * @brief Skickar en siffra (0-9) till skiftregistret.
 * @param number Siffran som ska visas.
 */
void display_show_number(uint8_t number);

/**
 * @brief Rensar displayen (stänger av alla segment).
 */
void display_clear(void);
/**
 * @brief ställar in vilken siffra på display man vill kuna skriva till
 */
void display_write_pos(uint8_t number, uint8_t pos);

#endif /* DISPLAY_H_ */