#include <iostream>

#include "ilrd_uid.hpp"

using namespace ilrd;


int main()
{
    UID u1;
    UID u2;

    std::cout << u1.m_counter << " " << u1.m_ip_addr << " " << u1.m_pid << " " << u1.m_time << std::endl;

 std::cout << u2.m_counter << " " << u2.m_ip_addr << " " << u2.m_pid << " " << u2.m_time << std::endl;

    return 0;
}