#include "LEDFlash.h"


LEDFlash::LEDFlash() : m_is_flashing(false)
{

}

LEDFlash::~LEDFlash()
{

}

void LEDFlash::add(LED &led)
{
    m_lights.push_back(led);
}

void LEDFlash::start()
{
    m_is_flashing = true;
    multicore_fifo_push_blocking(reinterpret_cast<uint32_t>(this));
    multicore_launch_core1(LEDFlash::flashing_thread);
}

void LEDFlash::stop()
{
    m_is_flashing = false;
}
