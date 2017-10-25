#include "LogHandler.hpp"


namespace logspace
{

LogHandler::LogHandler() : m_Running(false), m_isListening(false)
{

}

LogHandler::~LogHandler()
{
    if (isRunning())
    {
        m_Running = false;
        m_logThread.join();
        m_isListening = false;
    }
    m_logFileOstream.flush();
    m_logFileOstream.close();

}

bool LogHandler::run(const std::string &filename)
{
    if (isRunning())
    {
        return true;
    }
    m_filename = filename;
    m_logFileOstream.open(m_filename, std::ofstream::out | std::ofstream::app);
    std::cout << "LogHandler runned. Logfile opened" << std::endl;

    m_Running = true;
    m_logThread = std::thread(&LogHandler::logProcess, this);
    return true;
}

bool LogHandler::isRunning() const
{
    return m_Running;
}

void LogHandler::writeLog(const std::string &logString)
{
    std::stringstream logStream;
    std::time_t logTime;
    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    logTime = std::chrono::system_clock::to_time_t(today);
    logStream << "[LOGS]" << std::ctime(&logTime) << " " << logString;
    std::string tmpString = logStream.str();
    tmpString.erase(std::remove_if(tmpString.begin(), tmpString.end(), [&](const char &c)
    {
        if (c == '\n')
        {
            return true;
        }
        return false;
    }), tmpString.end());

    {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_logQueue.emplace(tmpString);
    }
}

void LogHandler::logProcess()
{
    m_isListening = true;
    while (m_Running)
    {
        if (m_logQueue.size() > 0)
        {
            std::string tmpLogString = m_logQueue.front();
            processString(tmpLogString);
            m_logQueue.pop();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void LogHandler::processString(const std::string &logString)
{
    m_logFileOstream << "[LOGS:File]" << logString << std::endl;
    m_logFileOstream.flush();
}


}
