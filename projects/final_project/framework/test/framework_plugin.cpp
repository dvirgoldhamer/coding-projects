#include <unistd.h> //STDFILENO
#include <utility> // std::pair, std::make_pair
#include "framework.hpp"
#include <iostream>

#define RED ("\033[31m")
#define GREEN ("\033[32m")
#define BLUE ("\x1B[34m")
#define DEFAULT ("\033[0m")
using namespace ilrd;

class ReadArgs2 : public interfaces::IKeyArgs
{
public:
    ReadArgs(int how_many , int offset) : m_how_many(how_many), m_offset(offset)
    {
    }

    int GetNum() const { return m_how_many; }
    int GetOffset() const { return m_offset; }
    int GetKey() {return (READ);};
private:
    
    int m_how_many;
    int m_offset;
};

class ReadCommand2 : public interfaces::ICommand
{
    std::pair<std::function<bool()>,std::chrono::milliseconds> Execute(std::shared_ptr<interfaces::IKeyArgs> key_args)
    {
        // Print:
        auto read_args = std::static_pointer_cast<ReadArgs>(key_args);
        std::cout << BLUE << "Read Command plugin:" << DEFAULT << std::endl;
        std::cout << "Read Bytes plugin:" << read_args->GetNum() << std::endl;
        std::cout << "Offset : " << read_args->GetOffset() << std::endl;

        // ...
        auto p = std::make_pair(nullptr, std::chrono::milliseconds(100));    
        return p;
    }
};

std::shared_ptr<interfaces::ICommand> CreateReadCommand()
{
    return std::make_shared<ReadCommand>();
}

using namespace std;

extern "C" __attribute__((constructor)) void Foo()
{
	cout << "hello" << endl;
	
    factory_details::Factory<int, interfaces::ICommand>* factory = Handleton<factory_details::Factory<int, interfaces::ICommand>>::GetInstance();

    auto* thread_pool = Handleton<ThreadPool>::GetInstance();

    thread_pool->Pause();
    factory->Register(3, CreateReadCommand);
    thread_pool->Resume();
}
