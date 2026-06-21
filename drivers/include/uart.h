/**
 * @file uart.h
 * @brief Driver interface for UART communication (Debug console).
 * @author Oliver Edman <o.edman@icloud.com>
 *
 * @note Licensed under the MIT License.
 */

#pragma once

#include <stdint.h>

/**
 * @brief Initialize the UART hardware.
 * @param baudrate The desired communication speed (9600 or 115200).
 */
void uart_init(uint32_t baudrate);

/**
 * @brief Transmits a single character over UART.
 * @param ch The character to send.
 */
void uart_write_char(char ch);

/**
 * @brief Transmits a null terminated string (char pointer) over UART.
 * @param str Pointer to the character string.
 */
void uart_write_string(const char *str);

/**
 * @brief Converts an integer to text and transmits it over UART.
 * @param num The integer value to print.
 */
void uart_write_int(int32_t num);
