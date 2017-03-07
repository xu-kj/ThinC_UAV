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

	void play_test_sound() {
		play_drone_alarm(1);
	}

	double getEngineVolume() {
		if (audio)
			return audio->getSoundVolume();
		else 
			return -1;
	}

	double increaseEngineVolume(double v) {
		if (audio && v > 0) {
			if (audio->getSoundVolume() + v <= 1.5) {
				audio->setSoundVolume(audio->getSoundVolume() + v);
			}
			return audio->getSoundVolume();
		}
		else 
			return -1;
	}

	double decreaseEngineVolume(double v) {
		if (audio && v > 0) {
			if (audio->getSoundVolume() - v <= 1.5) {
				audio->setSoundVolume(audio->getSoundVolume() - v);
			}
			return audio->getSoundVolume();
		}
		else 
			return -1;
	}
}
