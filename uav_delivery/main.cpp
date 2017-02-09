#include <irrlicht.h>
#include "include/irrKlang.h"
#include <iostream>

#include "controller.h"
#include "audio.h"

using std::cout; 
using std::endl;

#ifdef _IRR_SETUP_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
#endif

int main(int argc, char * argv[]) {
    cout << "Starting simulation" << endl;

	//irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
	/*engine->play2D("C:\\tools\\irrKlang-1.5.0\\media\\bell.wav");*/

	audio::initialize_audio();
    UAVController controller;
    controller.run();

    cout << "Simulation complete" << endl;
}