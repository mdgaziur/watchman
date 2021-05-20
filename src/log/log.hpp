#pragma once
#include <iostream>
#include <fstream>

#define PLATFORM_NOT_SUPPORTED "Platform is not currently supported"
#define INOTIFY_INIT_ERROR     "Error occured while trying to initialize inotify(Linux)"
#define FAILED_TO_OPEN_CONFIG  "Failed to open config file!"

enum LOG_LEVEL
{
    DEBUG,
    WARNING,
    ERROR,
    FATAL,
    INFO
};

class Log_class
{
public:
    Log_class(std::string filename = "/tmp/watchman-log.txt", int tolerance = 0);
    void write(LOG_LEVEL log_level, std::string message, bool print = false);
private:
    std::fstream m_Log_file;
};