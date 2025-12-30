
/*name: Dvir Goldhamer

File Name: Logger.cpp

purpose: Logger Implementation

reviewed by  Ofir Hasson: 

Date: 30.07.25*/

#include <cstring>

#include "handleton.hpp"
#include "logger.hpp"

namespace ilrd
{
    
Logger::Logger() :m_queue(), m_file("./log.txt", std::ios::out | std::ios::app)
{
    if (!m_file.is_open())
    {
        throw std::runtime_error("file not opened successfully");
    }

    m_thr = std::thread( [this] { WorkerThread(); });
}

Logger::~Logger() noexcept
{
    try
    {
        Log("", PRIVATE);
        m_file.close();
        m_thr.join();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Logger::Log(const std::string& log_message, Level level)
{
    std::time_t now = std::time(nullptr);
    std::tm* ltm = std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::put_time(ltm, "%d.%m.%Y %H:%M:%S");
    std::string time_stamp = oss.str();

    m_queue.Push(TaskItem(const_cast<std::string&>(log_message), level, time_stamp));
}

void Logger::WorkerThread()
{
    try
    {
        while(1)
        {
            TaskItem task;
            m_queue.Pop(task);
            if (task.m_level == PRIVATE)
            {
                break;
            }
            m_file << BuildMessage(task);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
}

std::string Logger::BuildMessage(TaskItem& task)
{
    std::string output_str;
    output_str = task.m_timeStamp;
    switch (task.m_level)
    {
        case DEBUG:
        output_str += "DEBUG ";
        break;
        case WARNING:
        output_str += "WARNING ";
        break;
        default:
        output_str += "ERROR ";
    }

    output_str += task.m_message;
    output_str += '\n';

    return output_str;
}

} // namespace ilrd
