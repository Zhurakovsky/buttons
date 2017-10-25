#pragma once

#include <iostream>

namespace logspace
{
    class ILogger
    {
    public:
        virtual ~ILogger() = default;

        virtual void writeLog(const std::string &logString) = 0;
        virtual void initLogger(const std::string &logFilePath) = 0;
    };

    ILogger* CreateLoggerInstance();
}
