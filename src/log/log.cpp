#include "log.hpp"
#include <iostream>
#include <vector>
#include <ctime>

Log_class::Log_class(std::string filename, int tolerance)
    : m_Log_file(filename, std::ios::app)
{
    if(!m_Log_file)
    {
        if(tolerance == 0)
        {
            std::cout<<"[FATAL] Failed to open or create log file!"<<std::endl;
            exit(1);
        }
        else
            std::cout<<"[WARNING] Failed to open or create log file!"<<std::endl;
    }
}

void Log_class::write(LOG_LEVEL log_level, std::string message, bool print)
{
    std::string final_message;
    const std::vector log_level_indicators = {"[DEBUG]: ", "[WARNING]: ", "[ERROR]: ", "[FATAL]: ", "[INFO]: "};
    const std::vector months = {
        "JAN",
        "FEB",
        "MAR",
        "APR",
        "MAY",
        "JUN"
        "JUL",
        "AUG",
        "SEP",
        "OCT",
        "NOV",
        "DEC"
    };
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::string time_string;
    time_string += std::to_string((1+(ltm->tm_mon))) + "th ";
    time_string += months[ltm->tm_mon];
    time_string += " ";
    time_string += std::to_string(1900 + ltm->tm_year) + ", at ";
    time_string += std::to_string(ltm->tm_hour % 12 == 0 ? 12 : ltm->tm_hour % 12) + ":";
    time_string += std::to_string(ltm->tm_min) + ":";
    time_string += std::to_string(ltm->tm_sec) + " ";
    time_string += ltm->tm_hour >= 12 ? "PM" : "AM";

    final_message += log_level_indicators.at(log_level);
    final_message += message + " - ";
    final_message += time_string;

    if(print)
    {
        if(log_level == FATAL || log_level == ERROR)
        {
            std::cerr<<final_message<<std::endl;
        }
        else
        {
            std::cout<<final_message<<std::endl;
        }
    }
    this->m_Log_file<<final_message<<std::endl;
    if(log_level == LOG_LEVEL::FATAL)
    {
        exit(1);
    }
}