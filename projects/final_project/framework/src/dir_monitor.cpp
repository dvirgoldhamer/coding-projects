/*name: Dvir Goldhamer

File Name: dir_monitor.cpp

purpose: implementation for a dir_monitor

reviewed by Dana Cohen

Date: 05.08.25*/

#include <iostream> //cerr

#include "dir_monitor.hpp"

namespace ilrd
{

DirMonitor::DirMonitor(const std::string& path_name) : m_dispatcher(), m_path_name(path_name), m_is_running(true), m_fd(0), m_watch_list(0)
{
}

DirMonitor::~DirMonitor() noexcept
{
    try
    {
    m_is_running.store(false);
    inotify_rm_watch(m_fd, m_watch_list);
    close(m_fd);

        m_thread.join();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void DirMonitor::Subscribe(CallbackBase<std::string>* callback)
{
    m_dispatcher.Subscribe(callback);
}

void DirMonitor::Run()
{
    m_thread = std::thread( [this] { ThreadFunc(); });
}

void DirMonitor::ThreadFunc()
{
    int length = 0;
    char buffer[BUF_SIZE] = {0};

    if ((m_fd = inotify_init()) < 0)
    {
        throw std::runtime_error("failed to monitor dir");
    }

    if ((m_watch_list = inotify_add_watch(m_fd, m_path_name.c_str(), IN_CREATE)) < 0)
    {
        close(m_fd);
        throw std::runtime_error("failed to add to watch list"); 
    } 

    while (m_is_running)
    {
        if ((length = read(m_fd, buffer, BUF_SIZE)) <= 0)
        {    
            std::cerr << "read failed" << std::endl;
            continue;
        }

        for (int i = 0; i < length;)
        {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            m_dispatcher.Notify(m_path_name + "/" + event->name);

            i += EVENT_SIZE + event->len + 1;
        }
    }

}

}//namespace ilrd