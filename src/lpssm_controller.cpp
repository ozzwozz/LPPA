#include <stdio.h>
#include "pico/stdlib.h"
#include <hardware/watchdog.h>

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

    // Watchdog restart code
    if (watchdog_caused_reboot())
    {
        printf("Rebooted by Watchdog!\n");
        // Whatever action you may take if a watchdog caused a reboot
    }

    // 0x7fffff is roughly 8.3 seconds and the maximum
    watchdog_enable(0x7fffff, 1);
    
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1); // Turn LED on

    uart_init(uart1, 9600);
    gpio_set_function(9, GPIO_FUNC_UART);
    gpio_set_function(8, GPIO_FUNC_UART);
    uart_set_format(uart1, 8, 1, UART_PARITY_NONE);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(uart1, true);

    // Set UART_Handler flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(uart1, false, false);

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

    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    struct repeating_timer timer;

    sleep_ms(2000);

    LED PA5_LED = LED(21, timer);
    LED PA4_LED = LED(22, timer);
    LED PA3_LED = LED(26, timer);
    LED PA2_LED = LED(27, timer);
    LED PA1_LED = LED(28, timer);

    PSU PA1_PSU = PSU(2, 11, PA1_LED);
    PSU PA2_PSU = PSU(3, 12, PA2_LED);
    PSU PA3_PSU = PSU(6, 13, PA3_LED);
    PSU PA4_PSU = PSU(7, 14, PA4_LED);
    PSU PA5_PSU = PSU(10, 15, PA5_LED);

    M24M02 m24m02 = M24M02(i2c0, EEPROM_ADDR);
    ADC adc = ADC();
    DS1682 ds1682 = DS1682(i2c0, DS1682_ADDR);
    INA3221A ina3221_1 = INA3221A(i2c0, INA3221A_1_ADDR);
    INA3221A ina3221_2 = INA3221A(i2c0, INA3221A_2_ADDR);
    ADS8166IRHBT ads8166 = ADS8166IRHBT(spi0, SPI_CS_PIN);

    UART_Handler uart_handler = UART_Handler(uart1, 9600, 9, 8, PA1_PSU, PA2_PSU, PA3_PSU, PA4_PSU, PA5_PSU
                                            , m24m02, adc, ds1682, ina3221_1, ina3221_2, ads8166);

    gpio_put(PICO_DEFAULT_LED_PIN, 0); // Turn LED off
    sleep_ms(50);

    while (true)
    {
        watchdog_update();
    
        if (uart_is_readable_within_us(uart1, 0))
        {
            uint8_t message[6];
            int8_t counter = 0;

            message[counter] = uart_getc(uart1);

            uart_handler.decode_message(message);
        }
    }
}
