#pragma once

#include "hardware/gpio.h"

/// @class LED
/// @brief LED Driver Class
class LED
{
public:
    /// @brief LED Class Constructor
    /// @param pin GPIO pin to be driven in this object
    LED(uint pin);

    /// @brief LED Class Destructor
    ~LED();

    /// @brief Turn on the LED
    void LED_on();

    /// @brief Turn off the LED
    void LED_off();

    /// @brief thread that flashes the LED when transmitting
    void flashing_thread();

private:
    /// @param m_pin member variable for the pin number
    uint m_pin;
};
