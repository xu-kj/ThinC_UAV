#include "audio.h"
#include "include/irrKlang.h"
#include <string>

namespace audio {
	// one audio engine is enough for multiple sounds
	irrklang::ISoundEngine* audio;
	std::string drone_alarm_directory = "C:\\sims\\uav_cameras_delivery\\source\\media\\Audio\\";

	void initialize_audio() {
		audio = irrklang::createIrrKlangDevice();
	}

	void play_drone_alarm(int i) {
		std::string alarm = drone_alarm_directory + "Drone_" + std::to_string(i) + ".mp3";
		audio->play2D(alarm.c_str());
	}

	void drop_engine() {
		audio->drop();
	}
}
