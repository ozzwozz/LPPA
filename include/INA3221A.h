#pragma once

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "I2CDevice.h"
#include <vector>

/// @class INA3221A
/// @brief Driver for the INA3221A Shunt and Bus Voltage Monitor
class INA3221A : I2CDevice
{
protected:
    bool configure();
public:
    /// @brief INA3221A Class Constructor
    /// @param i2c i2c instance
    /// @param device_address Device address
    INA3221A(i2c_inst_t* i2c, uint device_address);
    /// @brief INA3221A Class Destructor
    ~INA3221A();

    /// @brief read all shunt voltages
    /// @param voltage retrieve shunt voltages by reference
    /// @return success is true
    bool get_shunt_voltages(std::vector<uint16_t> &voltage);

    /// @brief read all bus voltages
    /// @param voltage retrieve bus voltages by reference
    /// @return success is true
    bool get_bus_voltages(std::vector<uint16_t> &voltage);

    bool pa_id_to_ina3221A_address(uint8_t pa_id, uint16_t &bus_voltage, uint16_t &shunt_voltage);

    bool get_voltages_for_pa(uint8_t command, uint16_t &voltage);

private:
    /// @param config_register Register address for device configuration
    uint8_t config_register = 0x00;

    /// @param shunt_channel_1_addr Register address of the shunt voltage for channel 1
    const uint8_t shunt_channel_1_addr = 0x01;
    /// @param shunt_channel_2_addr Register address of the shunt voltage for channel 2
    const uint8_t shunt_channel_2_addr = 0x03;
    /// @param shunt_channel_3_addr Register address of the shunt voltage for channel 3
    const uint8_t shunt_channel_3_addr = 0x05;
    
    /// @param bus_channel_1_addr Register address of the bus voltage for channel 1
    const uint8_t bus_channel_1_addr = 0x02;
    /// @param bus_channel_2_addr Register address of the bus voltage for channel 2
    const uint8_t bus_channel_2_addr = 0x04;
    /// @param bus_channel_3_addr Register address of the bus voltage for channel 3
    const uint8_t bus_channel_3_addr = 0x06;
};
