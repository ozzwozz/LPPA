#include <stdio.h>
#include "pico/stdlib.h"

#include "adc.h"
#include "ADS8166IRHBT.h"
#include "DS1682.h"
#include "INA3221A.h"
#include "LED.h"
#include "M24M02.h"
#include "PSU.h"
#include "UART_Handler.h"

#define SPI_MISO_PIN 16
#define SPI_MOSI_PIN 19
#define SPI_SCK_PIN 18
#define SPI_CS_PIN 17

#define EEPROM_ADDR 0x50
#define DS1682_ADDR 0xD6
#define INA3221A_1_ADDR 0x80
#define INA3221A_2_ADDR 0x82

int main()
{
    stdio_init_all();

    spi_init(spi0, 1'000'000);
    gpio_set_function(SPI_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_CS_PIN, GPIO_FUNC_SPI);

    spi_set_format(spi0,8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    LED led_17 = LED(21);
    LED led_18 = LED(22);
    LED led_19 = LED(26);
    LED led_20 = LED(27);
    LED led_21 = LED(28);
    
    PSU psu_1 = PSU(2, 11);
    PSU psu_2 = PSU(3, 12);
    PSU psu_3 = PSU(6, 13);
    PSU psu_4 = PSU(7, 14);
    PSU psu_5 = PSU(10, 15);

    M24M02 m24m02 = M24M02(i2c0, EEPROM_ADDR);
    ADC adc = ADC();
    DS1682 ds1682 = DS1682(i2c0, DS1682_ADDR);
    INA3221A ina3221_1 = INA3221A(i2c0, INA3221A_1_ADDR);
    INA3221A ina3221_2 = INA3221A(i2c0, INA3221A_2_ADDR);
    ADS8166IRHBT ads8166 = ADS8166IRHBT(spi0, SPI_CS_PIN);

    UART_Handler uart_handler = UART_Handler(uart1, 9600, 9, 8, psu_1, psu_2, psu_3, psu_4, psu_5
                                            , m24m02, adc, ds1682, ina3221_1, ina3221_2, ads8166);

    while (true) {
        if (psu_1.pa_power_state())
        {
            led_17.LED_on();
        }
        if (psu_2.pa_power_state())
        {
            led_18.LED_on();
        }
        if (psu_3.pa_power_state())
        {
            led_19.LED_on();
        }
        if (psu_4.pa_power_state())
        {
            led_20.LED_on();
        }
        if (psu_5.pa_power_state())
        {
            led_21.LED_on();
        }
        printf("Hello, world!\n");
 
        if (psu_1.pa_status())
        {
            led_17.LED_off();
        }
        if (psu_2.pa_status())
        {
            led_18.LED_off();
        }
        if (psu_3.pa_status())
        {
            led_19.LED_off();
        }
        if (psu_4.pa_status())
        {
            led_20.LED_off();
        }
        if (psu_5.pa_status())
        {
            led_21.LED_off();
        }
    }
}
