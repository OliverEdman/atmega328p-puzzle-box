#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>


void spi_init(void);

void spi_write(uint8_t data);

void spi_latch(void);

#endif