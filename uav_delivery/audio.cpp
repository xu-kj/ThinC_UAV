#include "audio.h"
#include "include/irrKlang.h"

namespace audio {
	irrklang::ISoundEngine* audio;

	void initialize_audio() {
		audio = irrklang::createIrrKlangDevice();
	}

	void ring_bell() {
		audio->play2D("C:\\tools\\irrKlang-1.5.0\\media\\bell.wav");
	}

	void drop_engine() {
		audio->drop();
	}
}
