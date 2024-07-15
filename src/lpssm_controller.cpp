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

#define ADC_RESET 0

// I2C defines
#define I2C0_PORT i2c0
#define I2C0_SDA 4
#define I2C0_SCL 5

#define EEPROM_ADDR 0x50
#define DS1682_ADDR 0xD6
#define INA3221A_1_ADDR 0x80
#define INA3221A_2_ADDR 0x82

int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 100Khz.
    i2c_init(I2C0_PORT, 100'000);
    gpio_set_function(I2C0_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C0_SDA);
    gpio_pull_up(I2C0_SCL);

    spi_init(spi0, 1'000'000);
    gpio_set_function(SPI_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_CS_PIN, GPIO_FUNC_SPI);

    spi_set_format(spi0,8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    LED PA5_LED = LED(21);
    LED PA4_LED = LED(22);
    LED PA3_LED = LED(26);
    LED PA2_LED = LED(27);
    LED PA1_LED = LED(28);
    
    PSU PA1_PSU = PSU(2, 11);
    PSU PA2_PSU = PSU(3, 12);
    PSU PA3_PSU = PSU(6, 13);
    PSU PA4_PSU = PSU(7, 14);
    PSU PA5_PSU = PSU(10, 15);

    M24M02 m24m02 = M24M02(i2c0, EEPROM_ADDR);
    ADC adc = ADC();
    DS1682 ds1682 = DS1682(i2c0, DS1682_ADDR);
    INA3221A ina3221_1 = INA3221A(i2c0, INA3221A_1_ADDR);
    INA3221A ina3221_2 = INA3221A(i2c0, INA3221A_2_ADDR);
    ADS8166IRHBT ads8166 = ADS8166IRHBT(spi0, SPI_CS_PIN);

    UART_Handler uart_handler = UART_Handler(uart1, 9600, 9, 8, PA1_PSU, PA2_PSU, PA3_PSU, PA4_PSU, PA5_PSU
                                            , m24m02, adc, ds1682, ina3221_1, ina3221_2, ads8166);

    while (true) {
        if (PA1_PSU.pa_power_state())
        {
            PA5_LED.LED_on();
        }
        if (PA2_PSU.pa_power_state())
        {
            PA4_LED.LED_on();
        }
        if (PA3_PSU.pa_power_state())
        {
            PA3_LED.LED_on();
        }
        if (PA4_PSU.pa_power_state())
        {
            PA2_LED.LED_on();
        }
        if (PA5_PSU.pa_power_state())
        {
            PA1_LED.LED_on();
        }
        printf("Hello, world!\n");
 
        if (PA1_PSU.pa_status())
        {
            PA5_LED.LED_off();
        }
        if (PA2_PSU.pa_status())
        {
            PA4_LED.LED_off();
        }
        if (PA3_PSU.pa_status())
        {
            PA3_LED.LED_off();
        }
        if (PA4_PSU.pa_status())
        {
            PA2_LED.LED_off();
        }
        if (PA5_PSU.pa_status())
        {
            PA1_LED.LED_off();
        }
    }
}
