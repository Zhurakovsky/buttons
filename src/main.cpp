#include <iostream>
#include "buttonListener.hpp"
#include "commontypes.hpp"
#include "menuitem.hpp"

using namespace std;
using namespace rpibuttons;

int main()
{
    ButtonListener bl;


    bl.run();
    cout << "Hello, Buttons" << endl;
    while(1);
    return 0;
}
