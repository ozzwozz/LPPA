#include "UART_Handler.h"
#include <string.h>
#include <stdio.h>

UART_Handler::UART_Handler(uart_inst_t *uart, uint baud_rate, uint rx_pin, uint tx_pin
        , PSU &psu_1, PSU &psu_2, PSU &psu_3, PSU &psu_4, PSU &psu_5
        , M24M02 &m24m02, ADC &adc, DS1682 &ds1682, INA3221A &ina3221_1, INA3221A &ina3221_2
        , ADS8166IRHBT &ads8166IRHBT)
            : m_uart(uart)
            , m_m24m02(m24m02)
            , m_adc(adc)
            , m_psu_1(psu_1)
            , m_psu_2(psu_2)
            , m_psu_3(psu_3)
            , m_psu_4(psu_4)
            , m_psu_5(psu_5)
            , m_ds1682(ds1682)
            , m_ina3221_1(ina3221_1)
            , m_ina3221_2(ina3221_2)
            , m_ads8166(ads8166IRHBT)
{
    uart_init(uart, baud_rate);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    uart_set_baudrate(m_uart, baud_rate);
    uart_set_format(m_uart, 8, 1, UART_PARITY_NONE);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(m_uart, false);

    // Set UART_Handler flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(m_uart, false, false);

    // enable UART interrupt
    uart_set_irq_enables(m_uart, true, false);
    
    // check which instance of UART_Handler is being used to give the correct UART_Handler interrupt handler
    int UART_IRQ = uart == m_uart ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, (irq_handler_t)uart_irq_handler);
    irq_set_enabled(UART_IRQ, true);
}

void UART_Handler::write(const char *data)
{
    uart_write_blocking(m_uart, reinterpret_cast<const uint8_t*>(data), strlen(data));
}

void UART_Handler::write(const char *data, size_t len)
{
    uart_write_blocking(m_uart, reinterpret_cast<const uint8_t*>(data), len);
}

size_t UART_Handler::read(char *data, size_t len)
{
    size_t bytes_read = 0;
    while (!rx_buffer_.empty() && bytes_read < len)
    {
        data[bytes_read++] = rx_buffer_.front();
        rx_buffer_.pop();
    }
    return bytes_read > 0;
}

bool UART_Handler::available()
{
    return uart_is_readable(m_uart) ? 1 : 0;
}

void UART_Handler::flush_rx()
{
    rx_buffer_ = std::queue<char>();
}

void UART_Handler::flush_tx()
{
    // tx_buffer_ = std::queue<std::vector<char>>();
}

void UART_Handler::uart_irq_handler(void *context)
{
    UART_Handler *uart = static_cast<UART_Handler *>(context);

    gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));

    while (uart_is_readable(uart->m_uart))
    {
        char c = uart_getc(uart->m_uart);
        uart->rx_buffer_.push(c);
    }
}

// TODO: Issue #4 - Finish Implementation of decoding and responding to UART messages
void UART_Handler::decode_message(const uint8_t message[5])
{
    uint8_t mutable_message[5];
    uint8_t response[24];

    mutable_message[0] = message[0];
    mutable_message[1] = message[1];
    mutable_message[2] = message[2];
    mutable_message[3] = message[3];
    mutable_message[4] = message[4];

    uint8_t header = message[0];

    switch (header)
    {
        case message_headers::SET_PSU:
            // mutable_message[1] = uart_getc(m_uart);
            set_psu(mutable_message);
            uart_putc(m_uart, header);
            break;
        case message_headers::GET_PSU:
            get_psu(response);
            uart_putc(m_uart, header);
            uart_putc(m_uart, response[1]);
            break;
        case message_headers::SET_PA_ENABLE:
            // mutable_message[1] = uart_getc(m_uart);
            set_pa_power_enable(mutable_message);
            uart_putc(m_uart, header);
            break;
        case message_headers::GET_PA_ENABLE:
            get_pa_power_enable(response);
            uart_putc(m_uart, header);
            uart_putc(m_uart, response[1]);
            break;
        case message_headers::SET_LED_STATE:
            // mutable_message[1] = uart_getc(m_uart);
            // mutable_message[2] = uart_getc(m_uart);
            set_led_state(mutable_message);
            uart_putc(m_uart, header);
            break;
        case message_headers::GET_LED_STATE: 
            get_led_state(response);
            uart_putc(m_uart, header);
            uart_putc(m_uart, response[1]);
            break;
        case message_headers::SET_CHARACTERISATION:
            // still undefined afaik
            set_characterisation(mutable_message);
            uart_write_blocking(m_uart, response, sizeof(response));
            break;
        case message_headers::GET_CHARACTERISATION:
            // still undefined afaik
            get_characterisation(response);
            uart_write_blocking(m_uart, response, sizeof(response));
            break;
        case message_headers::GET_BITS:
            get_bits(response);
            // should be the full size of response (24 bytes)
            uart_putc(m_uart, header);
            uart_putc(m_uart, response[1]);
            uart_putc(m_uart, response[2]);
            uart_putc(m_uart, response[3]);
            uart_putc(m_uart, response[4]);
            uart_putc(m_uart, response[5]);
            uart_putc(m_uart, response[6]);
            uart_putc(m_uart, response[7]);
            uart_putc(m_uart, response[8]);
            uart_putc(m_uart, response[9]);
            uart_putc(m_uart, response[10]);
            uart_putc(m_uart, response[11]);
            uart_putc(m_uart, response[12]);
            uart_putc(m_uart, response[13]);
            uart_putc(m_uart, response[14]);
            uart_putc(m_uart, response[15]);
            uart_putc(m_uart, response[16]);
            uart_putc(m_uart, response[17]);
            uart_putc(m_uart, response[18]);
            uart_putc(m_uart, response[19]);
            uart_putc(m_uart, response[20]);
            uart_putc(m_uart, response[21]);
            uart_putc(m_uart, response[22]);
            uart_putc(m_uart, response[23]);
            uart_putc(m_uart, response[24]);
            break;
        case message_headers::GET_HARDWARE_NUMBERS:
            get_hardware_numbers(response);
            uart_putc(m_uart, header);
            uart_putc(m_uart, response[1]);
            uart_putc(m_uart, response[2]);
            uart_putc(m_uart, response[3]);
            uart_putc(m_uart, response[4]);
            break;
        case message_headers::GET_SOFTWARE_NUMBERS:
            get_software_numbers(response);
            uart_putc(m_uart, header);
            uart_putc(m_uart, response[1]);
            uart_putc(m_uart, response[2]);
            uart_putc(m_uart, response[3]);
            uart_putc(m_uart, response[4]);
            break;
        case message_headers::SET_HARDWARE_NUMBERS:
            // mutable_message[1] = uart_getc(m_uart);
            // mutable_message[2] = uart_getc(m_uart);
            // mutable_message[3] = uart_getc(m_uart);
            // mutable_message[4] = uart_getc(m_uart);
            set_hardware_numbers(response, mutable_message);
            uart_putc(m_uart, header);
            break;
        case message_headers::SET_SOFTWARE_NUMBERS:
            // mutable_message[1] = uart_getc(m_uart);
            // mutable_message[2] = uart_getc(m_uart);
            // mutable_message[3] = uart_getc(m_uart);
            // mutable_message[4] = uart_getc(m_uart);            
            set_software_numbers(response, mutable_message);
            uart_putc(m_uart, header);
            break;
        case message_headers::GET_MONITORING_DETAILS:
            // mutable_message[1] = uart_getc(m_uart);
            get_monitoring_details(response, mutable_message);
            uart_putc(m_uart, header);
            break;
        case message_headers::SET_P13V_OXCO_PIN:
            // mutable_message[1] = uart_getc(m_uart);
            set_p13v_oxco_pin(mutable_message);
            uart_putc(m_uart, header);
            break;
        default:
            break;
    }

}

// TODO: Issue #4 - Finish Implementation of decoding and responding to UART messages
size_t UART_Handler::send_message()
{
    size_t bytes_sent = 0;
    char data[128];
    while (!tx_buffer_.empty() && bytes_sent < sizeof(data))
    {
        // data[bytes_sent++] = tx_buffer_.front();
        tx_buffer_.pop();
    }

    write(data);

    return bytes_sent > 0;
}

void UART_Handler::set_psu(uint8_t data[5])
{
    uint8_t band_mask = data[1];

    if ((band_mask & 1) != 0)
    {
        m_psu_1.pa_power_enable();
    }
    if ((band_mask & (1 << 1)) != 0)
    {
        m_psu_2.pa_power_enable();
    }
    if ((band_mask & (1 << 2)) != 0)
    {
        m_psu_3.pa_power_enable();
    }
    if ((band_mask & (1 << 3)) != 0)
    {
        m_psu_4.pa_power_enable();
    }
    if ((band_mask & (1 << 4)) != 0)
    {
        m_psu_5.pa_power_enable();
    }
}

void UART_Handler::get_psu(uint8_t response[24])
{
    uint8_t psu_status;

    psu_status |= (m_psu_1.pa_power_state());
    psu_status |= (m_psu_2.pa_power_state() << 1);
    psu_status |= (m_psu_3.pa_power_state() << 2);
    psu_status |= (m_psu_4.pa_power_state() << 3);
    psu_status |= (m_psu_5.pa_power_state() << 4);

    response[1] = psu_status;
}

void UART_Handler::set_pa_power_enable(uint8_t data[5])
{
    uint8_t band_mask = data[2];

    if ((band_mask & 1) != 0)
    {
        m_psu_1.pa_power_enable();
    }
    if ((band_mask & (1 << 1)) != 0)
    {
        m_psu_2.pa_power_enable();
    }
    if ((band_mask & (1 << 2)) != 0)
    {
        m_psu_3.pa_power_enable();
    }
    if ((band_mask & (1 << 3)) != 0)
    {
        m_psu_4.pa_power_enable();
    }
    if ((band_mask & (1 << 4)) != 0)
    {
        m_psu_5.pa_power_enable();
    }
}

void UART_Handler::get_pa_power_enable(uint8_t response[24])
{
    uint8_t lna_status;
    bool value;

    lna_status |= (m_psu_1.pa_status());
    lna_status |= (m_psu_2.pa_status() << 1);
    lna_status |= (m_psu_3.pa_status() << 2);
    lna_status |= (m_psu_4.pa_status() << 3);
    lna_status |= (m_psu_5.pa_status() << 4);
    
    response[1] = lna_status;
}

void UART_Handler::set_led_state(uint8_t mutable_message[5])
{

}

void UART_Handler::get_led_state(uint8_t response[24])
{
    
}

void UART_Handler::set_characterisation(uint8_t data[5])
{
    uint8_t calibration_table = data[1];
    uint8_t band_mask = data[2];
}

/// TODO: Issue #3- Implement undefined behaviour
void UART_Handler::get_characterisation(uint8_t response[24])
{
    // unknown return
}

void UART_Handler::get_bits(uint8_t response[24])
{
    uint32_t timestamp;
    uint16_t counter;
    float temperature;

    // Get timestamp from DS1682
    if (m_ds1682.getTime(timestamp))
    {
        response[1] = (timestamp >> 24);
        response[2] = (timestamp >> 16);
        response[3] = (timestamp >> 8);
        response[4] = timestamp & 0xFF;
    }

    // Get timestamp from DS1682
    if (m_ds1682.getEventCounter(counter))
    {
        response[5] = counter >> 8;
        // Extract lower 8 bits
        response[6] = counter & 0xFF;
    }
}

void UART_Handler::get_hardware_numbers(uint8_t response[24])
{
    uint32_t hardware_id;

    // EEPROM Hardware Number
    if (!m_m24m02.get_hardware_id(hardware_id))
    {
        // printf("Unable to retrieve the Software numbers.");
    }

    response[1] = (hardware_id >> 24);
    response[2] = (hardware_id >> 16);
    response[3] = (hardware_id >> 8);
    response[4] = hardware_id & 0xFF;
}

void UART_Handler::get_software_numbers(uint8_t response[24])
{
    uint32_t software_id;

    // EEPROM Software Number
    if (!m_m24m02.get_software_id(software_id))
    {
        // printf("Unable to retrieve the Software numbers.");
    }
    response[1] = (software_id >> 24);
    response[2] = (software_id >> 16);
    response[3] = (software_id >> 8);
    response[4] = software_id & 0xFF;
}

void UART_Handler::set_hardware_numbers(uint8_t response[24], uint8_t mutable_message[5])
{
    uint32_t hardware_id = (mutable_message[1] << 24) | (mutable_message[2] << 16) | (mutable_message[3] << 8) | (mutable_message[4] & 0xFF);
    m_m24m02.set_hardware_id(hardware_id);
}

void UART_Handler::set_software_numbers(uint8_t response[24], uint8_t mutable_message[5])
{
    uint32_t software_id = (mutable_message[1] << 24) | (mutable_message[2] << 16) | (mutable_message[3] << 8) | (mutable_message[4] & 0xFF);
    m_m24m02.set_software_id(software_id);
}

void UART_Handler::get_monitoring_details(uint8_t response[24], uint8_t mutable_message[5])
{
    std::vector<uint16_t> voltages;

    m_ina3221_1.get_bus_voltages(voltages);
    m_ina3221_1.get_shunt_voltages(voltages);

    m_ina3221_2.get_bus_voltages(voltages);
    m_ina3221_2.get_shunt_voltages(voltages);
}

void UART_Handler::set_p13v_oxco_pin(uint8_t mutable_message[5])
{
    if (mutable_message[1])
    {
        m_adc.set_p13v_enable_pin();
    }
    else
    {
        m_adc.set_p13v_disable();
    }
}
