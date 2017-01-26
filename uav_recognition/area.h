#ifndef AREA_H
#define AREA_H

#include <irrlicht.h>

namespace SaraMatching {
    class Area {
    public:
        enum SplitType {
            SPLIT_LEFT = 1,
            SPLIT_RIGHT,
            SPLIT_TOP,
            SPLIT_BOTTOM
        };

        Area();
        Area(float _x, float _y, float _w, float _h);
                
        Area Split(SplitType type, float percentKept = 0.5f);
        void ShrinkRelative(float percent);
        void NormalizeDown();
        void NormalizeUp();
        void Render(irr::video::IVideoDriver * driver, irr::video::SColor color);

        float x;
        float y;
        float w;
        float h;
    };
} 

#endif /* AREA_H */