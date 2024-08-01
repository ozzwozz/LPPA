#include "ADS8166IRHBT.h"

ADS8166IRHBT::ADS8166IRHBT(spi_inst_t* spi_inst, uint cs_pin) : SPIDevice(spi_inst, cs_pin)
{
    configure();
}

ADS8166IRHBT::~ADS8166IRHBT()
{
    // Should default us to master, but just being doubly sure
    // spi_set_slave(spi0, false);
}

bool ADS8166IRHBT::configure()
{
    uint8_t tx_data[2];

    tx_data[0] = ACCESS_REGISTER;
    tx_data[1] = 0b10101010;
    write(tx_data, 2);

    tx_data[0] = PD_CNTL_REGISTER;
    tx_data[1] = 0b00011110;
    write(tx_data, 2);

    tx_data[0] = SDI_CNTL_REGISTER;
    tx_data[1] = 0b00000000;
    write(tx_data, 2);

    tx_data[0] = SDO_CNTL1_REGISTER;
    tx_data[1] = 0b00000000;
    write(tx_data, 2);

    tx_data[0] = SDO_CNTL2_REGISTER;
    tx_data[1] = 0b00000000;
    write(tx_data, 2);

    // here would be configuring the SDO_CNTL3_REGISTER
    // but it's reserved so we won't

    tx_data[0] = SDO_CNTL4_REGISTER;
    tx_data[1] = 0b00000000;
    write(tx_data, 2);

    tx_data[0] = DATA_CNTL_REGISTER;
    tx_data[1] = 0b00000000;
    write(tx_data, 2);

    tx_data[0] = PARITY_CNTL_REGISTER;
    tx_data[1] = 0b00000000;
    write(tx_data, 2);

    tx_data[0] = ON_THE_FLY_REGISTER;
    tx_data[1] = 0b00000001;
    write(tx_data, 2);

    return true;
}

uint8_t ADS8166IRHBT::convert_adc_to_deg_c(uint16_t voltage_measured_by_adc)
{
    float result;

    result = pow((3.3436 * voltage_measured_by_adc), 4) - pow((28.581 * voltage_measured_by_adc), 3) + pow((88.622 * voltage_measured_by_adc), 2) - pow((141.07 * voltage_measured_by_adc), 2) + 129.64;

    return uint8_t(result);
}

void ADS8166IRHBT::change_channel(const uint8_t channel)
{
    uint8_t tx_data[2] {CHANNEL_ID_REGISTER, channel};
    write(tx_data, 2);
}

uint16_t ADS8166IRHBT::read_channel(const uint8_t channel)
{

    change_channel(channel);
    // bitwise OR to set the lower 4 bits to the channel number
    uint8_t tx_data[3] {0b00010000, 0x00, 0x00};
    uint8_t rx_data[3];

    // exchange(tx_data, rx_data, 3);
    write(tx_data, 3);
    read(rx_data, 3);

    return (rx_data[0] << 8) | rx_data[2];
}