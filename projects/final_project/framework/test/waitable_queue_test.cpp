#include <iostream>
#include <thread>

#include "waitable_queue.hpp"

ilrd::WaitableQueue<int, ilrd::PriorityQueue<int>> g_queue;

void Producer()
{
    try
    {
        for(int i = 0 ; i < 10 ; ++i)
        {
            g_queue.Push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        g_queue.Push(-1);
    }
    catch(const std::exception& e)
    {
        std::cout << "Caught an exception: " << e.what() << std::endl;
    }
}

void ConsumerTimedOut()
{
    try
    {
        while(1)
        {
            int data;
            if(!g_queue.Pop(data, 2000))
            {
                break;
            }
            std::cout << data << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cout << "Caught an exception: " << e.what() << std::endl;
    }
}

void Consumer()
{
    try
    {
        while(true)
        {
            int data;
            g_queue.Pop(data);
            if(-1 == data)
            {
                break;
            }

            std::cout << data << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cout << "Caught an exception: " << e.what() << std::endl;
    }
}

int main()
{
    std::vector<std::thread> consumers_timed_out;

    for (int i = 0; i < 5; ++i)
    {
        consumers_timed_out.emplace_back(ConsumerTimedOut);
    }

    std::thread producer(Producer);

    for (std::thread& c : consumers_timed_out)
    {
        c.join();
    }
    producer.join();

    std::thread consumer(Consumer);
    std::thread producer1(Producer);

    consumer.join();
    producer1.join();

    return 0;
}