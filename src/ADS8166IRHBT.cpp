#include "ADS8166IRHBT.h"

ADS8166IRHBT::ADS8166IRHBT(spi_inst_t* spi_inst, uint cs_pin) : SPIDevice(spi_inst, cs_pin)
{
}

ADS8166IRHBT::~ADS8166IRHBT()
{
    // Should default us to master, but just being doubly sure
    spi_set_slave(spi0, false);
}

uint16_t ADS8166IRHBT::read(const uint8_t channel)
{
    // bitwise OR to set the lower 4 bits to the channel number
    uint8_t tx_data[2] {0x80 | (channel << 4), 0x00};
    uint8_t rx_data[2];

    exchange(tx_data, rx_data, 2);

    return (rx_data[0] << 8) | rx_data[1];
}