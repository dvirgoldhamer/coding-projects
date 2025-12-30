/*name: Dvir Goldhamer

File Name: file_manager.hpp

purpose: API for FileManager

reviewed by : 

Date: */

#ifndef IRLD_RD168_169_FILE_MANAGER_HPP
#define IRLD_RD168_169_FILE_MANAGER_HPP

#include <fstream>
#include <memory>

#include "minion_key_args.hpp"
#include "handleton.hpp"
#include "concrete_utils.hpp"

namespace ilrd
{

class FileManager
{
public:
    std::shared_ptr<char[]> ReadFromFile(std::shared_ptr<MinionReadArgs> read_args); //throw std::runtime_error

    STATUS WriteToFile(std::shared_ptr<MinionWriteArgs> write_args);

    void AddFilePath(const std::string& path);

private:

    explicit FileManager() = default;
    ~FileManager() noexcept;

    int m_fd;
    
    friend class Handleton<FileManager>;

    static const size_t MINION_SIZE;

};

} //namespace ilrd

#endif //IRLD_RD168_169_FILE_MANAGER_HPP