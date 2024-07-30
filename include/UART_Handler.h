#pragma once

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include <queue>
#include <stdio.h>

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
        SET_LED_STATE = 0X04,
        GET_LED_STATE = 0X05,
        SET_CHARACTERISATION = 0x10,
        GET_CHARACTERISATION = 0x11,
        GET_BITS = 0X20,
        GET_HARDWARE_NUMBERS = 0X21,
        GET_SOFTWARE_NUMBERS = 0X22,
        SET_HARDWARE_NUMBERS = 0X23,
        SET_SOFTWARE_NUMBERS = 0X24,
        GET_MONITORING_DETAILS = 0X25,
        SET_P13V_OXCO_PIN = 0X40
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

    /// @brief Check if UART_Handler is available to interact with
    /// @return true if available 
    bool available();

    /// @brief Parse the received message
    void decode_message(const uint8_t message[5]);

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
    std::queue<char> tx_buffer_;

    /// @brief Interrupt handler for when data is received
    /// @param context pointer to calling instance.
    static void uart_irq_handler(void *context);

    /// @brief Set the psu power status
    /// @param data 
    void set_psu(uint8_t data[5]);

    /// @brief Get the current psu power status
    /// @param response response passed as object to be built in function
    /// @param band_mask bit mask for the bands to get information about 
    void get_psu(uint8_t response[24]);

    /// @brief Set the bands for which PA is enabled/disabled
    /// @param data value to set the PA to 
    void set_pa_power_enable(uint8_t data[5]);
    
    /// @brief Get the bands on which PA is enabled
    /// @param response response passed as object to be built in function
    void get_pa_power_enable(uint8_t response[24]);

    void set_led_state(uint8_t mutable_message[5]);

    void get_led_state(uint8_t response[24]);

    /// @brief Set the characterisation table on the EEPROM 
    /// @param data value to set the PA to
    void set_characterisation(uint8_t data[5]);

    /// @brief Get the current characterisation table on the EEPROM
    /// @param response response passed as object to be built in function
    void get_characterisation(uint8_t response[24]);

    /// @brief A list of the PA stage bits
    /// @param response response passed as object to be built in function
    void get_bits(uint8_t response[24]);

    /// @brief A list of the PA stage hardware numbers
    /// @param response response passed as object to be built in function
    void get_hardware_numbers(uint8_t response[24]);

    /// @brief A list of the PA stage software numbers
    /// @param response response passed as object to be built in function
    void get_software_numbers(uint8_t response[24]);

    void set_hardware_numbers(uint8_t response[24], uint8_t mutable_message[5]);

    void set_software_numbers(uint8_t response[24], uint8_t mutable_message[5]);

    void get_monitoring_details(uint8_t response[24], uint8_t mutable_message[5]);

    void set_p13v_oxco_pin(uint8_t mutable_message[5]);
};
