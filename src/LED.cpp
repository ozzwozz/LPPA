#include "LED.h"

LED::LED(uint pin, struct repeating_timer timer) : m_pin(pin), m_timer(timer)
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

bool LED::LED_state() const
{
    return gpio_get(m_pin);
}

bool LED::start_timer()
{
    return add_repeating_timer_ms(100, flashing_timer_callback, this, &m_timer);
}

bool LED::stop_timer()
{
    return cancel_repeating_timer(&m_timer);
}

bool LED::flashing_timer_callback(struct repeating_timer *rt)
{
    LED *led_object = static_cast<LED*>(rt->user_data);
    gpio_put(led_object->m_pin, !led_object->LED_state());

    return true;
}