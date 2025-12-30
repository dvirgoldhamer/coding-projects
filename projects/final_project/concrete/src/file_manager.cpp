
/*name: Dvir Goldhamer

File Name: file_manager.cpp

purpose: file manager Implementation

reviewed by: 

Date: */

#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <memory>

#include "file_manager.hpp"

namespace ilrd
{

const size_t FileManager::MINION_SIZE = 8 * 1024 * 1024;

void FileManager::AddFilePath(const std::string& path)
{
       // Try to open existing file
        m_fd = open(path.c_str(), O_RDWR, 0666);
        if (m_fd == -1)
        {
            if (errno == ENOENT)
            {
                // File doesn't exist: create it
                m_fd = open(path.c_str(), O_RDWR | O_CREAT, 0666);
                if (m_fd == -1)
                {
                    throw std::runtime_error("Failed to create file:");
                }

                // Fill with zeros up to 8 MB
                const size_t file_size = MINION_SIZE;
                if (ftruncate(m_fd, file_size) == -1)
                {
                    close(m_fd);
                    throw std::runtime_error("Failed to set file size:");
                }
            }
            else
            {
                throw std::runtime_error("Failed to open file:");
            }
        }
}


FileManager::~FileManager()
{
    close(m_fd);
}

std::shared_ptr<char[]> FileManager::ReadFromFile(std::shared_ptr<MinionReadArgs> read_args)
{
    uint64_t bytes_to_read = read_args->GetHowMany();
    auto read_buffer = std::shared_ptr<char[]>(new char[bytes_to_read]);

    uint64_t ret = pread(m_fd, read_buffer.get(), bytes_to_read, read_args->GetOffset());

    if (ret != (uint64_t)bytes_to_read)
    {
        std::cout << " pread failed or EOF reached" << std::endl;
        return nullptr;
    }

    return read_buffer;

}

STATUS FileManager::WriteToFile(std::shared_ptr<MinionWriteArgs> write_args)
{
    uint64_t bytes_to_write = write_args->GetHowMany();

    ssize_t ret = pwrite(m_fd, write_args->GetBuffer().get(), bytes_to_write, write_args->GetOffset());

    if (ret != (ssize_t)bytes_to_write)
    {
        std::cout << "[WriteToFile] pwrite failed" << std::endl;
        return FAILURE;
    }

    // flush to disk
    if (fsync(m_fd) == -1)
    {
        std::cout << "[WriteToFile] fsync failed" << std::endl;
        return FAILURE;
    }

    return SUCCESS;
}

}//namespace ilrd
