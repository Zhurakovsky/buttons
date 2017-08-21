#ifndef BUTTONLISTENER_HPP
#define BUTTONLISTENER_HPP

#include <iostream>
#include <functional>
#include <thread>
#include <atomic>
#include<condition_variable>

#include <unordered_map>
#include <set>
#include <mutex>
#include <string>
#include <cinttypes>

#include <bcm2835.h>

namespace rpibuttons
{
    class ButtonListener
    {
    public:
        ButtonListener(const ButtonListener&) = delete;
        ButtonListener& operator=(const ButtonListener&) = delete;

        ButtonListener();
        ~ButtonListener();

        bool run();
        bool isRunning() const;

        void gpioListen();
        bool subscribeOnPin(const int& pinNumber, const std::function<void()> &cbFunc);

    private:
        std::thread m_gpioThread;
        std::atomic_bool m_isRunning;
        bool m_isInit;
        mutable std::condition_variable m_condVar;
        mutable std::mutex m_mtx;

        void processButton(const uint32_t &valueMask);
        bool getPinMask(uint32_t &value);
        std::unordered_map<int, std::function<void()>> m_mapOfCallbacks;
        std::unordered_map<int, int> m_mapOfPrevValues;
        int gepPinsPressed(const uint32_t &valueMask);
    };
}

#endif //BUTTONLISTENER_HPP
