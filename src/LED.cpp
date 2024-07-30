#include "LED.h"

LED::LED(uint pin) : m_pin(pin)
{
    gpio_init(m_pin);
    gpio_set_dir(m_pin, GPIO_OUT);
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

bool LED::LED_state()
{
    return gpio_get(m_pin);
}