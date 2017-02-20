#include "audio.h"
#include "include/irrKlang.h"
#include <string>

namespace audio {
	// one audio engine is enough for multiple sounds
	// irrklang::ISoundEngine* audio[9];
	irrklang::ISoundEngine* audio;
	std::string drone_alarm_directory = "C:\\sims\\uav_cameras_delivery\\source\\media\\Audio\\";

	void initialize_audio() {
		// for (int i = 0; i < 9; ++i)
		//	audio[i] = irrklang::createIrrKlangDevice();
		audio = irrklang::createIrrKlangDevice();
	}

	void play_drone_alarm(int i) {
		// if (i >= 1 && i <= 9)
		//	audio[i - 1]->play2D("C:\\tools\\irrKlang-1.5.0\\media\\bell.wav");
		std::string alarm = drone_alarm_directory + "Drone_" + std::to_string(i) + ".mp3";
		audio->play2D(alarm.c_str());
	}

	void drop_engine() {
		// for (int i = 0; i < 9; ++i)
		//	audio[i]->drop();
		audio->drop();
	}
}
