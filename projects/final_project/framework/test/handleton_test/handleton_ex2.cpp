#include <iostream>
#include <dlfcn.h> // For dlopen, dlsym, dlclose

#include "handleton.hpp"

using namespace ilrd;
using namespace std;

class A;

int main()
{
    A* instance = Handleton<A>::GetInstance();

    void* handle = dlopen("./plug.so", RTLD_LAZY);
    typedef void (*FuncPtr)();

    FuncPtr fptr = reinterpret_cast<FuncPtr>(dlsym(handle, "Foo"));
    if (!fptr)
    {
        std::cerr << "dlsym failed: " << fptr << std::endl;
        dlclose(handle);
        return 1;
    }
    cout << "this is the address from the plugin " << instance << endl;
    
    cout << "this is the address from the plugin " << endl;
    fptr();

    dlclose(handle);

    return 0;
}
