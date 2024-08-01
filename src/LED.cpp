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

bool LED::start_timer()
{
    return add_repeating_timer_ms(1000, flashing_timer_callback, this, &m_timer);
}

bool LED::stop_timer()
{
    return cancel_repeating_timer(&m_timer);
}

bool LED::flashing_timer_callback(repeating_timer *rt)
{
    LED *led_class = static_cast<LED*>(rt->user_data);
    gpio_put(led_class->m_pin, !led_class->LED_state());

    return true;
}