#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "LED.h"

/// @class PSU
/// @brief PSU Driver Class - Control functions for controlling a PA
class PSU
{               
public:
    /// @brief PSU Class Constructor
    /// @param enable_pin 
    /// @param shutdown_pin 
    /// @param led_pin
    PSU(uint enable_pin, uint shutdown_pin, LED led);
    /// @brief PSU Class Destructor
    ~PSU();

    /// @brief enable the PA
    void pa_power_enable();
    /// @brief disable the PA
    void pa_power_disable();
    /// @brief status of the PA
    /// @return enabled is true
    bool pa_status();
    /// @brief shutdown the PA
    void pa_shutdown();
    /// @brief Allow the PA to be turned on
    void pa_turn_on();
    /// @brief check the power state of the PA
    /// @return turned on is true
    bool pa_power_state();

private:
    /// @brief m_inhibit_pin power enable pin
    uint m_inhibit_pin;
    /// @brief m_power_enable_pin PA shutdown pin
    uint m_power_enable_pin;
    /// @brief m_led LED associated with the PSU
    LED m_led;
};
