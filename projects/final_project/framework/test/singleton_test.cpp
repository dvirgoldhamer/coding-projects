#include <iostream>
#include <thread>
#include <vector>

#include "singleton.hpp"

#define RED ("\033[31m")
#define GREEN ("\033[32m")
#define BLUE ("\x1B[34m")
#define DEFAULT ("\033[0m")

class A
{
    private:
    A() = default;
    friend class ilrd::Singleton<A>;
};

A* g_instance = ilrd::Singleton<A>::GetInstance();
int result = 0;

void ThreadFunc()
{
    A* instance = ilrd::Singleton<A>::GetInstance();

    if(instance != g_instance)
    {
        std::cout << RED << "not the same object" << DEFAULT << std::endl;
        result = 1;
    }
}

int main()
{
    A* instance = ilrd::Singleton<A>::GetInstance();
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i)
    {
        threads.emplace_back(ThreadFunc);
    }

    for (std::thread& t : threads)
    {
        t.join();
    }

    if(instance != g_instance)
    {
        std::cout << RED << "not the same object" << DEFAULT << std::endl;
        result = 1;
    }

    if(!result)
    {
        std::cout << GREEN << "all tests passed!" << DEFAULT << std::endl;
    }

    return 0;
}