



#include "nbd_proxy.hpp"

using namespace ilrd;


int main()
{
    NBDProxy nbd_proxy;
    
    Handleton<NBDHandler>::GetInstance()->Init();

    Handleton<NBDHandler>::GetInstance()->HandleReadWrite();

    while(1) {}

    return 0;

}