/*name: Dvir Goldhamer

File Name: dll_loader.cpp

purpose: implementation for a dll_loader

reviewed by Dana Cohen

Date: 05.08.25*/

#include <dlfcn.h>
#include <iostream> //cerr

#include "dll_loader.hpp"

namespace ilrd
{

void DllLoader::Load(const std::string& path_name_dll)
{
    if ((m_handle = dlopen(path_name_dll.c_str(), RTLD_LOCAL | RTLD_NOW)) == NULL)
    {
        std::cerr << "failed to load plugin" << std::endl;
    };
}

DllLoader::~DllLoader() noexcept
{
    dlclose(m_handle);
}

}// namespaceilrd