#include "PSU.h"

PSU::PSU(uint power_enable_pin, uint enable_pin, LED led) : m_inhibit_pin(enable_pin), m_power_enable_pin(power_enable_pin), m_led(led)
{
    gpio_init(m_inhibit_pin);
    gpio_set_dir(m_inhibit_pin, GPIO_OUT);

    gpio_init(m_power_enable_pin);
    gpio_set_dir(m_power_enable_pin, GPIO_OUT);

    pa_power_disable();
    pa_shutdown();
}

PSU::~PSU()
{
    pa_power_disable();
    pa_shutdown();
}

void PSU::pa_power_enable()
{
    gpio_put(m_inhibit_pin, 0);
    m_led.start_timer();
}

void PSU::pa_power_disable()
{
    gpio_put(m_inhibit_pin, 1);    
    m_led.start_timer();
}

bool PSU::pa_status()
{
    return gpio_get(m_inhibit_pin);
}

void PSU::pa_shutdown()
{
    gpio_put(m_power_enable_pin, 0);
    m_led.LED_off();
}

void PSU::pa_turn_on()
{
    gpio_put(m_power_enable_pin, 1);
    m_led.LED_on();
}

bool PSU::pa_power_state()
{
    return gpio_get(m_power_enable_pin);
}
