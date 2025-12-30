#define DLL_IMPLEMENTOR

#include <iostream>
#include <unistd.h>
#include "logger.hpp"
#include "handleton.hpp"

#define RED ("\033[31m")
#define GREEN ("\033[32m")
#define BLUE ("\x1B[34m")
#define DEFAULT ("\033[0m")

INSTANTIATE_HANDLETON(ilrd::Logger)

std::atomic<int> counter = 0;

void ThreadFunc()
{
    try
    {
        ilrd::Logger* logger = ilrd::Handleton<ilrd::Logger>::GetInstance();
        for(int i = 0 ; i < 10 ; ++i)
        {
            logger->Log(std::to_string(counter), ilrd::Logger::DEBUG);
            ++counter;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

int main()
{
    try
    {
        std::vector<std::thread> threads;

        for(int i = 0 ; i < 5 ; ++i)
        {
            threads.emplace_back(std::thread(ThreadFunc));
        }

        for(int i = 0 ; i < 5 ; ++i)
        {
            threads[i].join();
        }

        std::cout << "finish!" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
