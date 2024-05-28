#include "adc.h"

ADC::ADC()
{
    adc_init();
    adc_gpio_init(supply_13V_pin);
}

ADC::~ADC()
{
}

void ADC::enable_13V()
{
    gpio_put(supply_13V_pin, 1);
}

void ADC::disable_13V()
{
    gpio_put(supply_13V_pin, 0);
}
