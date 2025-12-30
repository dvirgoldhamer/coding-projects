/*name: Dvir Goldhamer

File Name: dir_monitor_test.cpp

purpose: implementation for a dir_monitor

reviewed by

Date: */


#include "dir_monitor.hpp"
#include "dll_loader.hpp"

using namespace ilrd;


int main()
{
    DirMonitor monitor("/home/dvir-goldhamer/git/projects/final_project/framework/test/dir_monitor_test");
    DllLoader loader;
    Callback<std::string, DllLoader> callback(loader,&DllLoader::Load,nullptr);
    monitor.Subscribe(&callback);
    monitor.Run();
    sleep(25);
}