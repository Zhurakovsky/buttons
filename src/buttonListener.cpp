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
#include <iostream>

#define PIN RPI_GPIO_P1_15

namespace rpibuttons
{

ButtonListener::ButtonListener() :
    m_isRunning(false),
    m_isInit(false)
{
    //std::cout << "[LOGS:ButtonListener] Constructor" << std::endl;
}

ButtonListener::~ButtonListener()
{
    m_isRunning = false;

    if (m_gpioThread.joinable())
    {
        m_gpioThread.join();
    }
    //std::cout << "[LOGS:ButtonListener] Destructor" << std::endl;
}

bool ButtonListener::run()
{
    //std::cout << "[LOGS:ButtonListener] Come to Run()" << std::endl;
    std::unique_lock<std::mutex> lock(m_mtx);

    if (m_isRunning)
    {
        //std::cout << "[LOGS:ButtonListener] Already runed" << std::endl;
        m_isRunning = false;
        //return true;
    }
    m_gpioThread = std::thread(&ButtonListener::gpioListen, this);
    m_condVar.wait(lock, [&]() { return m_isInit; });
    //std::cout << "[LOGS:ButtonListener] Run completed" << std::endl;
    return m_isRunning;
}

bool ButtonListener::isRunning() const
{
    return m_isRunning;
}

void ButtonListener::gpioListen()
{
    //std::cout << "[LOGS:ButtonListener] Come to buttonListener " << std::endl;
    uint32_t maskGpioTest;
    {
        std::lock_guard<std::mutex> lock(m_mtx);

        if (!bcm2835_init())
        {
            //std::cout << "[LOGS:ButtonListener]bcm2835_init fails" << std::endl;
            m_condVar.notify_one();
            return;
        }
        else
        {
            //std::cout << "[LOGS:ButtonListener]bcm2835_init OK" << std::endl;
        }
        if( !getPinMask(maskGpioTest))
        {
            //std::cout << "[LOGS:ButtonListener]getPinMask failed" << std::endl;
            m_condVar.notify_one();
            bcm2835_close();
            return;
        }
        else
        {
            //std::cout << "[LOGS:ButtonListener]getPinMask OK" << std::endl;
        }
        // Set RPI subscribed pins to be an input
        // Do this for all pins from subscription
        if (!m_mapOfCallbacks.size())
        {
            m_condVar.notify_one();
            bcm2835_close();
            //std::cout << "[LOGS:ButtonListener]List of subscribers empty" << std::endl;
            return;
        }
        else
        {
            //std::cout << "[LOGS:ButtonListener]List of subscribers OK" << std::endl;
        }

        //std::vector<int> tmpPins = {8, 10, 12, 16, 18, 22};
        for (auto it = m_mapOfCallbacks.begin(); it != m_mapOfCallbacks.end(); ++it)
        //for (int it : tmpPins)
        {
            //int tmpPin = it;
            int tmpPin = it->first;
            //8 10 12 16 18 22
            bcm2835_gpio_fsel(tmpPin, BCM2835_GPIO_FSEL_INPT);
            //std::cout << "Set gpio_fsel for PIN " << tmpPin << std::endl;
            //  with a pullup
            auto iter = pullUpPinsSet.find(tmpPin);
            if (iter != pullUpPinsSet.end())
            {
                //bcm2835_gpio_set_pud(tmpPin, BCM2835_GPIO_PUD_UP);
                std::cout << "Set gpio UP for PIN " << tmpPin << std::endl;
                // And a low detect enable
                bcm2835_gpio_len(tmpPin);
            }
            else
            {
                bcm2835_gpio_set_pud(tmpPin, BCM2835_GPIO_PUD_DOWN);
                std::cout << "Set gpio DOWN for PIN " << tmpPin << std::endl;
                // And a low detect enable
                bcm2835_gpio_hen(tmpPin);
            }

        }

        m_isRunning = true;
        m_isInit = true;
        m_condVar.notify_one();
    }
    //std::cout << "[LOGS:ButtonListener] ButtonListener() Before thread's loop" << std::endl;
    while (m_isRunning)
    {
        std::cout << "Loop... " << std::endl;
        uint32_t returnedPinMask = 0;
        for (auto it = m_mapOfCallbacks.begin(); it != m_mapOfCallbacks.end(); ++it)
        {
            uint32_t tmpPin = it->first;
            auto itr = pullUpPinsSet.find(tmpPin);
            if (itr != pullUpPinsSet.end())
            {
                if (bcm2835_gpio_lev(tmpPin) == 0)
                {
                    returnedPinMask |= (1 << tmpPin);
                }
            }
            else
            {
                if (bcm2835_gpio_lev(tmpPin) == 1)
                {
                    returnedPinMask |= (1 << tmpPin);
                }
            }
        }

        processButton(returnedPinMask);
    }
    bcm2835_close();
}

bool ButtonListener::subscribeOnPin(const uint32_t &pinNumber, const std::function<void()> &cbFunc)
{
    //std::cout << "[LOGS:ButtonListener] Subscribe on pin " << pinNumber << std::endl;
    uint32_t purePin = 0; // without pull info
    purePin |= (pinNumber & 0x00FF);
    if ((pinNumber >> 8) == 1)
    {
        pullUpPinsSet.insert(purePin);
    }
    if (!m_mapOfPrevValues.emplace(std::make_pair(purePin,0)).second)
    {
        //std::cout << "[LOGS:ButtonListener] Error emplacing prev pin::" << purePin << std::endl;
        return false;
    }
    //std::cout << "[LOGS:ButtonListener] Emplaced pin::" << purePin << std::endl;
    if (!m_mapOfCallbacks.emplace(std::make_pair(purePin,cbFunc)).second)
    {
        //std::cout << "[LOGS:ButtonListener] Error emplacing Callback::" << purePin << std::endl;
        return false;
    }
    return true;
}

void ButtonListener::processButton(const uint32_t &valueMask)
{
    int pinsPressed = getPinsPressed(valueMask);
    if (pinsPressed <= 1)
    {
        for (auto pin : rpibuttons::RPiGPIOPins)
        {
            uint32_t pinIntValue = (uint32_t)pin;
            if (valueMask & (1 << pinIntValue))
            {
                auto it = m_mapOfCallbacks.find(pinIntValue);
                if (it != m_mapOfCallbacks.end())
                {
                    auto iter = m_mapOfPrevValues.find(pinIntValue);
                    if (iter != m_mapOfPrevValues.end())
                    {
                        if (iter->second == 0)
                        {
                            it->second();
                            iter->second = 1;
                        }
                    }   
                }
            }
            else
            {
                auto it = m_mapOfPrevValues.find((uint32_t)pin);
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
        uint32_t tmpShift = it->first;
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
    //std::cout << "Get mask values len == " << value << std::endl;
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
