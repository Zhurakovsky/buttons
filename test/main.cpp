#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "ILogger.hpp"

using namespace std;
using namespace logspace;

int main(int argc, char* argv[])
{
    std::string filename{"logfile.txt"};
    if (argc == 2)
    {
        filename.clear();
        filename = argv[1];
        if (filename.size() < 1)
        {
            std::cout << "Wrong logfile name" << std::endl;
            exit(1);
        }

    }
    logspace::ILogger *logger = logspace::CreateLoggerInstance();
    logger->initLogger(filename);
    for (int i = 0; i < 10; i++)
    {
        std::string tmpLog = "My test log " + std::to_string(i);
        logger->writeLog(tmpLog);
    }

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
