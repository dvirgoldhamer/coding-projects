/*name: Dvir Goldhamer

File Name: Logger.hpp

purpose: API for a Logger

reviewed by Ofir Hasson 

Date: 30.07.25*/

#ifndef IRLD_RD168_169__LOGGER_HPP
#define IRLD_RD168_169__LOGGER_HPP

#include <iostream>
#include <fstream>

#include "handleton.hpp"
#include "waitable_queue.hpp"
 
namespace ilrd
{
class Logger
{
public:
    enum Level
    {
        DEBUG,
        WARNING,
        ERROR,
        PRIVATE //internal usage, do not use
    };

    void Log(const std::string& log_message, Level level);


    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
private:
    explicit Logger(); //throws runtime error
    ~Logger() noexcept;
    void WorkerThread();
    
    struct TaskItem
    {
        std::string m_message;
        Level m_level;
        std::string m_timeStamp;
    };
    
    static std::string BuildMessage(TaskItem& task);
    
    std::thread m_thr;
    WaitableQueue<TaskItem> m_queue;
    std::ofstream m_file;

    friend class Handleton<Logger>;
};
} //namespace ilrd

#endif //IRLD_RD168_169__LOGGER_HPP
