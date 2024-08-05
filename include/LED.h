#pragma once

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

/// @class LED
/// @brief LED Driver Class
class LED
{
public:
    /// @brief LED Class Constructor
    /// @param pin GPIO pin to be driven in this object
    LED(uint pin, struct repeating_timer timer);

    /// @brief LED Class Destructor
    ~LED();

    /// @brief Turn on the LED
    void LED_on();

    /// @brief Turn off the LED
    void LED_off();

    /// @brief Get the state of the LED
    /// @return true if LED is on
    bool LED_state() const;

    /// @brief Start a timer to trigger every second
    /// @return starting timer successful -> true
    bool start_timer();

    /// @brief Stop timer from triggering
    /// @return stopping timer successful -> true
    bool stop_timer();

    /// @brief timer callback that flashes the led
    /// @param t user data to pass to store in the repeating_timer structure for use by the callback.
    /// @return true after callback finished
    static bool flashing_timer_callback(struct repeating_timer *t);

private:
    /// @brief m_pin member variable for the pin number
    uint m_pin;

    /// @brief repeating timer structure
    repeating_timer m_timer;
};
