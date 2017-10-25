#pragma once
#include "ILogger.hpp"
#include "LogHandler.hpp"

#include <iostream>
#include <string>

namespace logspace
{
    class Logger : public ILogger
    {
    public:
        Logger();
        Logger(const Logger&) = delete;
        Logger & operator=(const Logger&) = delete;
        virtual ~Logger();

        virtual void writeLog(const std::string &logString) override;
        virtual void initLogger(const std::string &logFilePath) override;
    private:
        LogHandler m_logHandler;
        std::string m_logFilePath = {""};

    };
}
