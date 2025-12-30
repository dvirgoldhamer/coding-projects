/*name: Dvir Goldhamer

File Name: dir_monitor.hpp

purpose: API for a dir_monitor

reviewed by Dana Cohen

Date: 05.08.25*/

#ifndef IRLD_RD168_169__DIR_MONITOR_HPP
#define IRLD_RD168_169__DIR_MONITOR_HPP

#include <thread>
#include <atomic>
#include <sys/inotify.h>

#include "dispatcher.hpp"

namespace ilrd
{

class DirMonitor
{
public:
    explicit DirMonitor(const std::string& path_name);
    DirMonitor(const DirMonitor&) = delete;
    DirMonitor& operator=(const DirMonitor&) = delete;
    ~DirMonitor() noexcept;

    void Subscribe(CallbackBase<std::string>* callback); //may throw
    void Run(); //may throw

private:
    std::thread m_thread;
    Dispatcher<std::string> m_dispatcher;
    std::string m_path_name;

    std::atomic<bool> m_is_running;
    int m_fd;
    int m_watch_list;

    void ThreadFunc();

    static const int EVENT_NUM = 10;
    static const int DLL_NAME_LEN = 80;
    static const int EVENT_SIZE = sizeof(inotify_event);
    static const int BUF_SIZE = (EVENT_SIZE + DLL_NAME_LEN) * EVENT_NUM;

}; //class DirMonitor

} //namespace ilrd
#endif //IRLD_RD168_169__DLL_LOADER_HPP

