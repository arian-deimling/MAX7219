/**
 * @file SPI.h
 * @author Arian Deimling
 * @version 0.1.0
 */

#ifndef SPI_H_
#define SPI_H_

#include <string>
#include "bcm2835.h"

int spi_init();

void spi_set_options(std::string bit_order, std::string data_mode, 
                     std::string clock_frequency, std::string chip_select);

void spi_send_data(char* buf, std::uint32_t len);

int spi_close();

#endif  // SPI_H_

