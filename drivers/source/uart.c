#include "uart.h"
#include "utils.h"
#include <avr/io.h>
#include <stdlib.h>

static const uint32_t cpu_frequency_hz = 16000000UL;
static const uint8_t baud_calculation_multiplier = 16;
static const uint8_t ubrr_high_shift_bits = 8;

static const uint8_t transmit_enable_bit = TXEN0;
static const uint8_t data_frame_bit_0 = UCSZ00;
static const uint8_t data_frame_bit_1 = UCSZ01;
static const uint8_t transmit_buffer_empty_bit = UDRE0;

static const uint8_t int_buffer_size = 12;
static const uint8_t numeric_base_decimal = 10;
static const char string_terminator = '\0';

void uart_init(uint32_t baudrate) {
    uint16_t ubrr_value = (cpu_frequency_hz / (baud_calculation_multiplier * baudrate)) - 1;
    
    UBRR0H = (uint8_t)(ubrr_value >> ubrr_high_shift_bits);
    UBRR0L = (uint8_t)(ubrr_value);
    
    SET(UCSR0B, transmit_enable_bit);
    
    SET(UCSR0C, data_frame_bit_1);
    SET(UCSR0C, data_frame_bit_0);
}

void uart_write_char(char ch) {
    while (!READ(UCSR0A, transmit_buffer_empty_bit));
    UDR0 = ch;
}

void uart_write_string(const char *str) {
    while (*str != string_terminator) {
        uart_write_char(*str);
        str++;
    }
}

void uart_write_int(int32_t num) {
    char buffer[int_buffer_size];
    itoa(num, buffer, numeric_base_decimal);
    uart_write_string(buffer);
}
