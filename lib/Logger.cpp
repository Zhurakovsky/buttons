#include "Logger.hpp"

namespace logspace
{
ILogger* CreateLoggerInstance()
{
    static Logger loggerInstance;
    return &loggerInstance;
}


Logger::Logger()
{

}

Logger::~Logger()
{

}

void Logger::writeLog(const std::string &logString)
{
    m_logHandler.writeLog(logString);
}

void Logger::initLogger(const std::string &logFilePath)
{
    std::cout << "Logger Init file:" << logFilePath.c_str() << std::endl;
    m_logHandler.run(logFilePath);
}

}
