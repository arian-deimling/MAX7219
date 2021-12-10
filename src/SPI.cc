/**
 * @file SPI.cc
 * @author Arian Deimling
 * @version 0.1.0
 */

#include "SPI.h"
#include "bcm2835.h"

int spi_init() {

    int return_code{0};
    
    // attempt to initialize the bcm2835 library and return if it fails
    return_code = bcm2835_init();
    if (return_code == 0) {
        return return_code;
    }

    // return the result of the SPI initialization
    return_code = bcm2835_spi_begin();
    return return_code;

}

void spi_set_options(std::string bit_order, std::string data_mode, 
                     std::string clock_frequency, std::string chip_select) {

    // FOR NOW - IGNORING ALL PARAMS AND HARDCODING THESE OPTIONS FOR EASE

    // set the bit order in which to send data
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);

    // set the SPI mode
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

    // set the SPI frequency
    bcm2835_spi_set_speed_hz(6'250'000); // 6.25MHz

    // select chip select pin and set it to LOW voltage
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

}

void spi_send_data(char* buffer, std::uint32_t buffer_length) {
    bcm2835_spi_transfern(buffer, buffer_length);
}

int spi_close() {

    // attempt to uninitialize the bcm2835 library and return result
    bcm2835_spi_end();
    return bcm2835_close();
    
}