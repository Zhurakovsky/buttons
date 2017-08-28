#include "buttonListener.hpp"
#include "commontypes.hpp"

#include <chrono>
#include <utility>
#include <cstdio>
#include <cstring>
#include <vector>
#include <mutex>
#include <algorithm>
#include <condition_variable>
#include <cassert>

#define PIN RPI_GPIO_P1_15

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
            bcm2835_close();
            return;
        }
        // Set RPI subscribed pins to be an input
        // Do this for all pins from subscription
        if (!m_mapOfCallbacks.size())
        {
            m_condVar.notify_one();
            bcm2835_close();
            std::cout << "List of subscribers empty" << std::endl;
            return;
        }

        for (auto it = m_mapOfCallbacks.begin(); it != m_mapOfCallbacks.end(); ++it)
        {
            //int tmpPin = *it;
            int tmpPin = it->first;
            //8 10 12 16 18 22
            bcm2835_gpio_fsel(tmpPin, BCM2835_GPIO_FSEL_INPT);
            std::cout << "Set gpio_fsel for PIN " << tmpPin << std::endl;
            //  with a pullup
            bcm2835_gpio_set_pud(tmpPin, BCM2835_GPIO_PUD_DOWN);
            // And a low detect enable
            //bcm2835_gpio_hen(tmpPin);
        }

        m_isRunning = true;
        m_isInit = true;
        m_condVar.notify_one();
    }
    while (m_isRunning)
    {
        uint32_t returnedPinMask = 0;
        //std::cout << "Hello from button listener" << std::endl;

        returnedPinMask = bcm2835_gpio_eds_multi(maskGpioTest);
        if (returnedPinMask == 0)
        {
            std::cout << "Returned buttons mask == 0" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        //bcm2835_gpio_set_eds_multi(maskGpioTest);
        printBitMask(returnedPinMask);
        processButton(returnedPinMask);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    bcm2835_close();
}

bool ButtonListener::subscribeOnPin(const int &pinNumber, const std::function<void()> &cbFunc)
{
    m_mapOfPrevValues.emplace(std::make_pair(pinNumber,0));
    return m_mapOfCallbacks.emplace(std::make_pair(pinNumber,cbFunc)).second;
}

void ButtonListener::processButton(const uint32_t &valueMask)
{
    int pinsPressed = getPinsPressed(valueMask);
    if (pinsPressed <= 1)
    {
        for (auto pin : rpibuttons::RPiGPIOPins)
        {
            int pinIntValue = (int)pin;
            if (valueMask & (1 << pinIntValue) && (bcm2835_gpio_lev(pinIntValue) == 1))
            {
                auto it = m_mapOfCallbacks.find(pinIntValue);
                if (it != m_mapOfCallbacks.end())
                {
                    auto iter = m_mapOfPrevValues.find(pinIntValue);
                    if (iter != m_mapOfPrevValues.end())
                    {
                        if (iter->second == 0)
                        {
                            iter->second = 1;
                            it->second();
                        }
                        else
                        {
                            //Protection from button slippage
                        }
                    }

                }
            }
            else
            {
                auto it = m_mapOfPrevValues.find((int)pin);
                if (it != m_mapOfPrevValues.end())
                {
                    it->second = 0;
                }
            }
        }
    }
    else
    {
        //Pressed more than one button
        //Probably, need to clean pin mask
        //Or some extended logic needed
        std::cout << "More than one button pressed" << std::endl;
    }
}

bool ButtonListener::getPinMask(uint32_t &value)
{
    uint32_t maskValues = 0;
    for(auto it = m_mapOfCallbacks.begin(); it != m_mapOfCallbacks.end(); ++it)
    {
        int tmpShift = it->first;
        maskValues |= (1 << tmpShift);
    }
    value = maskValues;
    //printBitMask(value);
    //
    return true;
}

int ButtonListener::getPinsPressed(const uint32_t &valueMask)
{
    int value = 0;
    for (int i = 0; i < 32; i++)
    {
        if ((valueMask >> i) & 0x1)
        {
            value += 1;
        }
    }
    std::cout << "Get mask values len == " << value << std::endl;
    return value;
}

void ButtonListener::printBitMask(const uint32_t &mask)
{
    for (int i = 31; i >= 0; i--)
    {
        if ((mask >> i) & 0x1)
        {
            std::cout << 1 << " ";
        }
        else
        {
            std::cout << 0 << " ";
        }
    }
    std::cout << std::endl;
}
}
