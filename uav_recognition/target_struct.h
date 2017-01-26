#ifndef TARGET_STRUCT_H
#define TARGET_STRUCT_H

#include <irrlicht.h>

struct TargetStruct {
    TargetStruct(const irr::core::stringc &file, irr::s32 x, irr::s32 y)
        : file(file), x(x), y(y)
    {}

    irr::core::stringc file;
    irr::s32 x;
    irr::s32 y;
};

#endif /* TARGET_STRUCT_H */