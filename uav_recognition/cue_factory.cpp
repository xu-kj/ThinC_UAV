#include "cue_factory.h"
// #include "mudsplash_cue.h"

namespace SaraMatching {
    Cue * CreateCue(const std::string& type)
    {
        Cue * cue = 0;

        //if     (type == "MUDSPLASH")  return new MudsplashCue();
        //else if(type == "FLICKER")        return new FlickerCue();
        //else if(type == "GRADUAL")        return new GradualCue();
        //else if(type == "STEADY")     return new SteadyCue();

        return cue;
    }
}