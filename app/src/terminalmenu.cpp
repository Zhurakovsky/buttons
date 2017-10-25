#include "terminalmenu.hpp"
#include <chrono>

namespace rpibuttons
{

TerminalMenu::TerminalMenu(DisplayOled &displ, std::vector<MenuItem *> &menu) :
    m_isRunning(false),
    m_isInit(false),
    m_menu(menu),
    m_displ(displ)
{

}

TerminalMenu::~TerminalMenu()
{
    m_isRunning = false;

    if (m_terminalThread.joinable())
    {
        m_terminalThread.join();
    }
}

bool TerminalMenu::run()
{
    std::unique_lock<std::mutex> lock(m_mtx);

    if (m_isRunning)
    {
        return true;
    }
    m_terminalThread = std::thread(&TerminalMenu::terminalListen, this);
    m_condVar.wait(lock, [&]() { return m_isInit; });
    return m_isRunning;
}

bool TerminalMenu::isRunning() const
{
    return m_isRunning;
}

}
