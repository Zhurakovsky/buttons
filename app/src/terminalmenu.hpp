#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <string>

#include "displayoled.hpp"
#include "commontypes.hpp"
#include "menuitem.hpp"

namespace rpibuttons
{

class TerminalMenu
{
public:
    TerminalMenu(DisplayOled &displ, std::vector<MenuItem*> &menu);
    ~TerminalMenu();

    void terminalListen();
    bool run();
    bool isRunning() const;
private:
    std::thread m_terminalThread;
    std::atomic_bool m_isRunning;
    bool m_isInit;
    mutable std::condition_variable m_condVar;
    mutable std::mutex m_mtx;

    std::vector<MenuItem*> m_menu;
    DisplayOled m_displ;
};
}
