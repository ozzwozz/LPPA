#pragma once

#include "pico/stdlib.h"
#include "hardware/adc.h"

/// @class ADC
/// @brief ADC Driver for reading from analogue to digital pins
class ADC
{
public:
    /// @brief Construct a new ADC object
    ADC();
    /// @brief Destroy the ADC object
    ~ADC();

    /// @param supply_13V_pin 13V supply pin
    const uint supply_13V_pin = 20;
    /// @param PA_PGOOD_13V_pin 13V PA PGOOD pin
    const uint PA_PGOOD_13V_pin = 1;

    /// @brief enable the 13V supply line
    void enable_13V();

    /// @brief disable the 13V supply line
    void disable_13V();
};
