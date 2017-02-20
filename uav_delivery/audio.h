#ifndef AUDIO_H
#define AUDIO_H

#include "include/irrKlang.h"

namespace audio {
	//extern irrklang::ISoundEngine* audio;
	void initialize_audio();
	void play_drone_alarm(int i);
	void drop_engine();
}

#endif /* AUDIO_H */