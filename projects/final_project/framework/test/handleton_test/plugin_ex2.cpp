#include <iostream>

#include "handleton.hpp"

using namespace ilrd;
using namespace std;

class A;

extern "C" void Foo()
{
    A* instance = Handleton<A>::GetInstance();

    cout << instance << endl;
}
