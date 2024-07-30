#pragma once

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

/// @class ADC
/// @brief ADC Driver for reading from analogue to digital pins
class ADC
{
private:
    /// @param P13V_ENABLE_PIN P13V voltage enable Pin
    const uint P13V_ENABLE_PIN = 20;

    /// @param P13V_PGOOD_PIN P13V PGOOD Pin
    const uint P13V_PGOOD_PIN = 1;

public:
    /// @brief Construct a new ADC object
    ADC();
    /// @brief Destroy the ADC object
    ~ADC();
    
    /// @param voltage_P13V member variable to store the P13V voltage
    uint16_t voltage_P13V;
    /// @param voltage_P13V_PGOOD member variable to store the P13V_PGOOD voltage
    uint16_t voltage_P13V_PGOOD;

    /// @brief read the raw value of the given adc pin
    /// @param pin to read the raw adc value for 
    /// @return uint16_t value on the pin
    uint16_t read_raw_adc(uint pin);

    /// @brief read the converted voltage value of the given adc pin
    /// @param pin 
    /// @return float the voltage value at the pin
    float read_voltage_adc(uint pin);

    /// @brief read the voltage on all of the pins
    void read_all_voltages();

    /// @brief Set the p13v enable pin
    void set_p13v_enable_pin();

    /// @brief Unset the p13v enable pin
    void set_p13v_disable();
};
