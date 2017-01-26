#include "controller.h"

#include <irrlicht.h>
#include <iostream>

using std::cout; 
using std::endl;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main(int argc, char * argv[]) {
    cout << "Starting simulation" << endl;

    UAVController controller;
    controller.run();

    cout << "Simulation complete" << endl;
}