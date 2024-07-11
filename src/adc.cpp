#include "adc.h"

ADC::ADC()
{
    adc_init();

    gpio_init(P13V_PGOOD_PIN);
    gpio_set_dir(P13V_PGOOD_PIN, GPIO_IN);
    
    gpio_init(P13V_ENABLE_PIN);
    gpio_set_dir(P13V_ENABLE_PIN, GPIO_OUT);

    set_p13v_enable_pin();
}

ADC::~ADC()
{
    set_p13v_disable();
}

uint16_t ADC::read_raw_adc(uint pin)
{
    adc_select_input(pin);
    return adc_read();
}

float ADC::read_voltage_adc(uint pin)
{
    uint16_t adc_value = read_raw_adc(pin);
    return (3.3 * adc_value) / (1 << 12);
}

void ADC::read_all()
{
    voltage_P13V_PGOOD = gpio_get(P13V_PGOOD_PIN);
}

void ADC::set_p13v_enable_pin()
{
    gpio_put(P13V_ENABLE_PIN, 1);
}

void ADC::set_p13v_disable()
{
    gpio_put(P13V_ENABLE_PIN, 0);
}
