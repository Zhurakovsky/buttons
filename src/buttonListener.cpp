#include "buttonListener.hpp"

#include <chrono>
#include <utility>
#include <cstdio>
#include <cstring>
#include <vector>
#include <mutex>
#include <algorithm>
#include <condition_variable>
#include <cassert>


namespace rpibuttons
{

ButtonListener::ButtonListener() :
    m_isRunning(false),
    m_isInit(false)
{
}

ButtonListener::~ButtonListener()
{
    m_isRunning = false;

    if (m_gpioThread.joinable())
    {
        m_gpioThread.join();
    }
}

bool ButtonListener::run()
{
    std::unique_lock<std::mutex> lock(m_mtx);

    if (m_isRunning)
    {
        return true;
    }

    m_gpioThread = std::thread(&ButtonListener::gpioListen, this);
    m_condVar.wait(lock, [&]() { return m_isInit; });

    return m_isRunning;
}

bool ButtonListener::isRunning() const
{
    return m_isRunning;
}

void ButtonListener::gpioListen()
{
    uint32_t maskGpioTest;

    {
        std::lock_guard<std::mutex> lock(m_mtx);

        if (!bcm2835_init())
        {
            m_condVar.notify_one();
            return;
        }
        if( !getPinMask(maskGpioTest))
        {
            m_condVar.notify_one();
            return;
        }
        // Set RPI pin P1-15 to be an input
        bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
        //  with a pullup
        bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_UP);
        // And a low detect enable
        bcm2835_gpio_len(PIN);

        m_isRunning = true;
        m_isInit = true;
        m_condVar.notify_one();
    }
    while (m_isRunning)
    {
        uint32_t returnedPinMask = 0;
        std::cout << "Hello from button listener" << std::endl;

        returnedPinMask = bcm2835_gpio_eds_multi(maskGpioTest);
        if (returnedPinMask == 0)
        {
            continue;

        }
        processButton(returnedPinMask);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    }
}

void ButtonListener::processButton(const uint32_t &valueMask)
{

}

bool ButtonListener::getPinMask(uint32_t &value)
{
    value = 0;

    return true;
}
}
