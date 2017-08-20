#include <iostream>
#include "buttonListener.hpp"

using namespace std;
using namespace rpibuttons;

int main()
{
    ButtonListener bl;
    bl.run();
    cout << "Hello, Buttons" << endl;
    return 0;
}
