#include "INA3221A.h"

INA3221A::INA3221A(i2c_inst_t *i2c, uint device_address) : I2CDevice(i2c, device_address)
{
}

INA3221A::~INA3221A()
{
}

bool INA3221A::configure()
{
    uint16_t config = 0b1111;
    uint8_t command[3] {config_register, config >> 8, config};

    // Send config
    int ret = i2c_write_blocking(m_i2c, m_address, command, sizeof(buffer), true);
    if (ret == PICO_ERROR_GENERIC)
    {
        return false;
    }

    return true;
}

bool INA3221A::get_shunt_voltages(std::vector<uint16_t> &voltage)
{
    uint8_t buffer[6] {shunt_channel_1_addr, shunt_channel_2_addr, shunt_channel_3_addr};

    // Send shunt address
    int ret = i2c_write_blocking(m_i2c, m_address, buffer, sizeof(buffer), true);
    if (ret == PICO_ERROR_GENERIC)
    {
        return false;
    }

    if (!I2CDevice::read(buffer, sizeof(buffer)))
    {
        return false;
    }

    voltage.push_back(buffer[0] << 8 && buffer[1]);
    voltage.push_back(buffer[2] << 8 && buffer[3]);
    voltage.push_back(buffer[4] << 8 && buffer[5]);

    return true;
}

bool INA3221A::get_bus_voltages(std::vector<uint16_t> &voltage)
{
    uint8_t buffer[6] {bus_channel_1_addr, bus_channel_2_addr, bus_channel_3_addr};
    
    // Send bus address
    int ret = i2c_write_blocking(m_i2c, m_address, buffer, sizeof(buffer), true);
    if (ret == PICO_ERROR_GENERIC)
    {
        return false;
    }

    if (!I2CDevice::read(buffer, sizeof(buffer)))
    {
        return false;
    }

    voltage.push_back(buffer[0] << 8 && buffer[1]);
    voltage.push_back(buffer[2] << 8 && buffer[3]);
    voltage.push_back(buffer[4] << 8 && buffer[5]);

    return true;
}