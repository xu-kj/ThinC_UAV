#ifndef AUDIO_H
#define AUDIO_H

#include "include/irrKlang.h"

namespace audio {
	//extern irrklang::ISoundEngine* audio;
	void initialize_audio();
	void play_drone_alarm(int i);
	void drop_engine();
	void play_test_sound();

	double getEngineVolume();
	double increaseEngineVolume(double v);
	double decreaseEngineVolume(double v);
}

#endif /* AUDIO_H */