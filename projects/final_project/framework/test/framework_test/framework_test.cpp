/*name: Dvir Goldhamer

File Name: framework_test.cpp

purpose: API for a framework prototype

reviewed by : 

Date: */

#include <unistd.h> //STDFILENO
#include <utility> // std::pair, std::make_pair

#include "framework.hpp"

using namespace ilrd;
using namespace ilrd::interfaces;

#define RED ("\033[31m")
#define GREEN ("\033[32m")
#define BLUE ("\x1B[34m")
#define DEFAULT ("\033[0m")

class ReadArgs : public interfaces::IKeyArgs
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

class WriteArgs : public interfaces::IKeyArgs
{
public:
    WriteArgs(int how_many,int offset, std::shared_ptr<char[]> buffer) : m_how_many(how_many), m_offset(offset), m_buffer(buffer) {}
    
    int GetNum() const{ return m_how_many; }
    int GetOffset() const { return m_offset; }
    std::shared_ptr<char[]> GetBuffer() { return m_buffer;}
    int GetKey() {return (WRITE);};

private:
    int m_how_many;
    int m_offset;
    std::shared_ptr<char[]> m_buffer;
};

class NBDProxy : public interfaces::IInputProxy

{
public:
    std::shared_ptr<interfaces::IKeyArgs> GetKeyArgs(int fd, Mode mode)
    {
        (void)mode;
        static bool test_toggle = true;
        // Hard coded
        char buffer[1024];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0)
        {
            return nullptr;
        }

        buffer[bytes_read] = '\0';

        if (test_toggle)
        {
            test_toggle = !test_toggle;
            return std::make_shared<ReadArgs>(5,100);
        }
        else
        {
            auto buffer = 
            std::shared_ptr<char[]>(new char[6]{'h','e','l','l','o','\0'});
            test_toggle = !test_toggle;
            return std::make_shared<WriteArgs>(3,400, buffer);
        }
    }
};

class ReadCommand : public interfaces::ICommand
{
    std::pair<std::function<bool()>,std::chrono::milliseconds> Execute(std::shared_ptr<interfaces::IKeyArgs> key_args)
    {
        auto read_args = std::static_pointer_cast<ReadArgs>(key_args);
        std::cout << BLUE << "Read Command:" << DEFAULT << std::endl;
        std::cout << "Read Bytes : " << read_args->GetNum() << std::endl;
        std::cout << "Offset : " << read_args->GetOffset() << std::endl;

        auto p = std::make_pair(nullptr, std::chrono::milliseconds(100));    
        return p;
    }
};

class WriteCommand : public interfaces::ICommand
{
     std::pair<std::function<bool()>,std::chrono::milliseconds> Execute(std::shared_ptr<interfaces::IKeyArgs> key_args)
    {
        auto write_args = std::static_pointer_cast<WriteArgs>(key_args);
           std::cout << BLUE << "Write Command:" << DEFAULT << std::endl;
        std::cout << "Write Bytes : " << write_args->GetNum() << std::endl;
        std::cout << "Offset : " << write_args->GetOffset() << std::endl;
        std::cout << "Buffer : " << write_args->GetBuffer().get() << std::endl;

        auto p = std::make_pair(nullptr, std::chrono::milliseconds(100));    
        return p;
    }
};

std::shared_ptr<interfaces::ICommand> CreateReadCommand()
{
    return std::make_shared<ReadCommand>();
}

std::shared_ptr<interfaces::ICommand> CreateWriteCommand()
{
    return std::make_shared<WriteCommand>();
}

int main()
{
    std::unordered_map<FdModeWrapper,                  std::shared_ptr<interfaces::IInputProxy>, FdModeHash> proxy_map;

    proxy_map[{STDIN_FILENO, READ}] = std::make_shared<NBDProxy>();

    std::unordered_map<int, std::function<std::shared_ptr<interfaces::ICommand>()>> creators;

    creators[READ] = CreateReadCommand;
    creators[WRITE] = CreateWriteCommand;

    Framework framework(proxy_map,
                        creators,
                        "/home/dvir-goldhamer/git/projects/final_project/framework/test/framework_test");
    
    framework.Run();//Blocking

    return 0;
}