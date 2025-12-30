/*name: Dvir Goldhamer

File Name: dll_loader.hpp

purpose: API for a dll_loader

reviewed by Dana Cohen

Date: 05.08.25*/

#ifndef IRLD_RD168_169__DLL_LOADER_HPP
#define IRLD_RD168_169__DLL_LOADER_HPP

#include <string>

namespace ilrd
{
class DllLoader
{
public:
    explicit DllLoader() = default;
    DllLoader(const DllLoader&) = delete;
    DllLoader& operator=(const DllLoader&) = delete;

    ~DllLoader() noexcept;

    void Load(const std::string& path_name_dll);

private:
    void* m_handle;
};


} //namespace ilrd
#endif //IRLD_RD168_169__DLL_LOADER_HPP
