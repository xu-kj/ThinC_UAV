#ifndef CUE_FACTORY_H
#define CUE_FACTORY_H

#include <string>
#include <fstream>

namespace SaraMatching {
    class Cue;

    Cue * CreateCue(const std::string& type);
}

#endif /* CUE_FACTORY_H */