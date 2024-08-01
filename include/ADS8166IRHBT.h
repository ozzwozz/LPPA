#pragma once

#include "pico/stdlib.h"
#include "SPIDevice.h"
#include <cmath>

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

    bool configure();

    uint8_t convert_adc_to_deg_c(uint16_t voltage_measured_by_adc);

    void change_channel(const uint8_t channel);

    /// @brief Read analogue input on given channel
    /// @param channel channel to read
    /// @return temperature on channel
    uint16_t read_channel(const uint8_t channel);

private:
    /// @brief Enables read/write access to the device configuration registers
    const uint8_t ACCESS_REGISTER = 0x00;
    /// @brief Enable/disable control for the reference register
    const uint8_t PD_CNTL_REGISTER = 0x04;
    /// @brief SPI-00, SPI-01, SPI-10, SPI-11 protocol selection
    const uint8_t SDI_CNTL_REGISTER = 0x08;
    /// @brief SDO output protocol selection
    const uint8_t SDO_CNTL1_REGISTER = 0x0C;
    /// @brief Output data rate selection
    const uint8_t SDO_CNTL2_REGISTER = 0x0D;
    /// @brief Reserved
    const uint8_t SDO_CNTL3_REGISTER = 0x0E;
    /// @brief Configuration for the SEQSTS pin when not using SDO-1 for data transfer
    const uint8_t SDO_CNTL4_REGISTER = 0x0F;
    /// @brief Output data word configuration
    const uint8_t DATA_CNTL_REGISTER = 0x10;
    /// @brief Parity configuration register
    const uint8_t PARITY_CNTL_REGISTER = 0x11;

    const uint8_t CHANNEL_ID_REGISTER = 0x1D;
 
    /// @brief This register enables the on-the-fly mode of operation
    const uint8_t ON_THE_FLY_REGISTER = 0x2A;

};
