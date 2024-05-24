#include "LED.h"

LED::LED(uint pin) : m_pin(pin)
{
}

LED::~LED()
{
}

void LED::LED_on()
{
    gpio_put(m_pin, 1);
}

void LED::LED_off()
{
    gpio_put(m_pin, 0);
}