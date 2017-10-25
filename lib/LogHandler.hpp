#pragma once
#include <iostream>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <iterator>
#include <ctime>
#include <chrono>
#include <algorithm>

namespace logspace
{
    class LogHandler
    {
    public:
        LogHandler(const LogHandler&) = delete;
        LogHandler& operator=(const LogHandler&) = delete;

        LogHandler();
        ~LogHandler();

        bool run(const std::string &filename);
        bool isRunning() const;

        void writeLog(const std::string &logString);
        void logProcess();
    private:
        void processString(const std::string &logString);
        std::ofstream m_logFileOstream;
        std::string m_filename;
        std::queue<std::string> m_logQueue;


        std::thread m_logThread;
        std::atomic<bool> m_Running;
        std::atomic<bool> m_isListening;

        mutable std::mutex m_mtx;

        //bool IsServiceChars(const char &c);

    };
}
