#pragma once

#include <list>
#include <unistd.h>
#include "pico/multicore.h"
#include "LED.h"

/// @class LED
/// @brief LED Driver Class
class LEDFlash
{
public:
    /// @brief LEDFlash Class Constructor
    LEDFlash();

    /// @brief LEDFlash Class Destructor
    ~LEDFlash();

    /// @brief Add an LED that needs to be flashed
    /// @param led The LED that needs to be flashed
    void add(LED &led);

    /// @brief Start the LEDs flashing
    void start();

    /// @brief Stop the LEDs flashing
    void stop();

private:
    /// @brief The LEDs that need flashing
    std::list<LED> m_lights;

    /// @brief m_is_flashing member variable used to terminate thread
    bool m_is_flashing;

    /// @brief thread that flashes the LEDs when transmitting
    static void flashing_thread()
    {
        void *obj = reinterpret_cast<void *>(multicore_fifo_pop_blocking());
        LEDFlash* that = static_cast<LEDFlash*>(obj);
        while (that->m_is_flashing)
        {
            // Set the lights on:
            for (auto &led : that->m_lights)
            {
                led.LED_on();
            }
            sleep(1);
            // Set the lights off:
            for (auto &led : that->m_lights)
            {
                led.LED_off();
            }
            sleep(1);
        }
    }

};