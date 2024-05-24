#include "PSU.h"

PSU::PSU(uint enable_pin, uint shutdown_pin) : m_enable_pin(enable_pin), m_shutdown_pin(shutdown_pin)
{
}

PSU::~PSU()
{
}

void PSU::pa_enable()
{
    gpio_put(m_enable_pin, 1);
}

void PSU::pa_disable()
{
    gpio_put(m_enable_pin, 1);
}

bool PSU::pa_status()
{
    return gpio_get(m_enable_pin);
}

void PSU::pa_shutdown()
{
    gpio_put(m_shutdown_pin, 1);
}

void PSU::pa_turn_on()
{
    gpio_put(m_shutdown_pin, 0);
}

bool PSU::pa_power_state()
{
    return gpio_get(m_shutdown_pin);
}
