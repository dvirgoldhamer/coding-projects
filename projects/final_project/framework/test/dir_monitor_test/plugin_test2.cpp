#include <iostream>

using namespace std;

extern "C" __attribute__((constructor)) void Foo()
{
    cout << "my plugin 2" << endl;
}
