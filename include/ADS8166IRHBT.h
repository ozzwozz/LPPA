#pragma once

#include "pico/stdlib.h"
#include "SPIDevice.h"

/// @class ADS8166IRHBT
/// @brief ADS8166IRHBT Driver for temperature sensing
class ADS8166IRHBT : public SPIDevice
{
public:
    /// @brief ADS8166IRHBT Class Constructor
    /// @param spi_inst spi instance
    /// @param cs_pin chip select pin
    ADS8166IRHBT(spi_inst_t* spi_inst, uint cs_pin);

    /// @brief ADS8166IRHBT Class Destructor
    ~ADS8166IRHBT();

    /// @brief Read analogue input on given channel
    /// @param channel channel to read
    /// @return temperature on channel
    uint16_t read(const uint8_t channel);
};
