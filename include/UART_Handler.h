#pragma once

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include <queue>

#include "M24M02.h"
#include "DS1682.h"
#include "INA3221A.h"
#include "adc.h"
#include "PSU.h"
#include "ADS8166IRHBT.h"

/// @class UART_Handler
/// @brief UART_Handler Driver Class
class UART_Handler
{
public:
    // TODO: Issue #7 - Implement Payload Definitions
    enum message_headers
    {
        SET_PSU = 0x00,
        GET_PSU = 0x01,
        SET_PA_ENABLE = 0x02, 
        GET_PA_ENABLE = 0x03,
        SET_CHARACTERISATION = 0x10,
        GET_CHARACTERISATION = 0x11,
        GET_BITS = 0X20,
        GET_HARDWARE_NUMBERS = 0X21,
        GET_SOFTWARE_NUMBERS = 0X22,
    }; // Enum inside the class
    
    /// @brief Construct a new UART_Handler object
    /// @param uart uart instance
    /// @param baud_rate baud rate
    /// @param rx_pin rx pin
    /// @param tx_pin tx pin
    /// @param psu_1 Instantiated PSU driver object
    /// @param psu_2 Instantiated PSU driver object
    /// @param psu_3 Instantiated PSU driver object
    /// @param psu_4 Instantiated PSU driver object
    /// @param psu_5 Instantiated PSU driver object
    /// @param m24m02 Instantiated M24M02 driver object
    /// @param adc Instantiated ADC driver object
    /// @param ds1682 Instantiated driver object
    /// @param ina3221_1 Instantiated INA3221A driver object
    /// @param ina3221_2 Instantiated INA3221A driver object
    /// @param ads8166IRHBT Instantiated ADS8166IRHBT driver object
    UART_Handler(uart_inst_t *uart, uint baud_rate, uint rx_pin, uint tx_pin
        , PSU &psu_1, PSU &psu_2, PSU &psu_3, PSU &psu_4, PSU &psu_5
        , M24M02 &m24m02, ADC &adc, DS1682 &ds1682, INA3221A &ina3221_1, INA3221A &ina3221_2
        , ADS8166IRHBT &ads8166IRHBT);

    /// @brief Destroy the UART_Handler object
    ~UART_Handler();

    /// @brief Write to UART_Handler
    /// @param data data to write to UART
    void write(const char *data);

    /// @brief Write to UART_Handler
    /// @param data data to write to UART
    /// @param len length of the data
    void write(const char *data, size_t len);

    /// @brief Read from UART_Handler
    /// @param data object to store data from UART
    /// @param len length of the data expected
    /// @return size_t size of the data read
    size_t read(char *data, size_t len);

    /// @brief Check if UART_Handler is available to interact with
    /// @return true if available 
    bool available();

    /// @brief flush the Rx buffer
    void flush_rx();

    /// @brief flush the Tx buffer
    void flush_tx();

    /// @brief Send data in the tx buffer
    /// @return size_t 
    size_t send_message();

private:
    /// @param m_uart uart instance
    uart_inst_t *m_uart;

    /// @param m_m24m02 M24M02 object - EEPROM
    M24M02 &m_m24m02;
    /// @param m_ds1682 DS1682 object - ETR
    DS1682 &m_ds1682;

    /// @param m_ina3221_1 INA3221A object - Shunt and Bus Monitor 
    INA3221A &m_ina3221_1;
    /// @param m_ina3221_2 INA3221A object - Shunt and Bus Monitor
    INA3221A &m_ina3221_2;

    /// @param m_ads8166 ADS8166IRHBT object - 8 Channel ADC for Temperature Sensing
    ADS8166IRHBT &m_ads8166;

    /// @param m_psu_1 PSU object - Power enable and shutdown
    PSU &m_psu_1;
    /// @param m_psu_2 PSU object - Power enable and shutdown
    PSU &m_psu_2;
    /// @param m_psu_3 PSU object - Power enable and shutdown
    PSU &m_psu_3;
    /// @param m_psu_4 PSU object - Power enable and shutdown
    PSU &m_psu_4;
    /// @param m_psu_5 PSU object - Power enable and shutdown
    PSU &m_psu_5;

    /// @param m_adc ADC object - ADC Pins 
    ADC &m_adc;
    
    /// @param m_rx_pin rx pin
    uint m_rx_pin;
    /// @param m_tx_pin tx pin
    uint m_tx_pin;
    /// @param rx_buffer_ rx buffer queue
    std::queue<char> rx_buffer_;
    /// @param tx_buffer_ tx buffer queue
    std::queue<std::vector<char>> tx_buffer_;

    /// @brief Interrupt handler for when data is received
    /// @param context pointer to calling instance.
    static void uart_irq_handler(void *context);

    /// @brief Parse the received message
    void decode_message();

    /// @brief Set the psu power status
    /// @param data 
    void set_psu(char* data);

    /// @brief Get the current psu power status
    /// @param response response passed as object to be built in function
    /// @param band_mask bit mask for the bands to get information about 
    void get_psu(std::vector<char>& response, uint8_t band_mask);

    /// @brief Set the bands for which PA is enabled/disabled
    /// @param data value to set the PA to 
    void set_pa_enable(char* data);
    
    /// @brief Get the bands on which PA is enabled
    /// @param response response passed as object to be built in function
    void get_pa_enable(std::vector<char>& response);

    /// @brief Set the characterisation table on the EEPROM 
    /// @param data value to set the PA to
    void set_characterisation(char* data);

    /// @brief Get the current characterisation table on the EEPROM
    /// @param response response passed as object to be built in function
    void get_characterisation(std::vector<char>& response);

    /// @brief A list of the PA stage bits
    /// @param response response passed as object to be built in function
    void get_bits(std::vector<char>& response);

    /// @brief A list of the PA stage hardware numbers
    /// @param response response passed as object to be built in function
    void get_hardware_numbers(std::vector<char>& response);

    /// @brief A list of the PA stage software numbers
    /// @param response response passed as object to be built in function
    void get_software_numbers(std::vector<char>& response);
};
