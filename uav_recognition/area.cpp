#include "area.h"
#include "image.h"

namespace SaraMatching {
    Area::Area()
        : x(0), y(0), w(0), h(0)
    {}

    Area::Area(float _x, float _y, float _w, float _h)
        : x(_x), y(_y), w(_w), h(_h)
    {}

    Area Area::Split(SplitType type, float percentKept) {
        Area a1 = *this;

        switch(type) {
        case SPLIT_LEFT:
            a1.x += w * percentKept;
            a1.w *= (1.0f - percentKept);
            w *= percentKept;
            break;

        case SPLIT_RIGHT:
            x += w * (1.0f - percentKept);
            a1.w *= (1.0f - percentKept);
            w *= percentKept;
            break;

        case SPLIT_TOP:
            a1.y += h * (percentKept);
            a1.h *= (1.0f - percentKept);
            h *= percentKept;
            break;

        case SPLIT_BOTTOM:
            y += h * (1.0f - percentKept);
            a1.h *= (1.0f - percentKept);
            h *= percentKept;
            break;

        default:
            break;
        }

        return a1;
    }

    void Area::ShrinkRelative(float percent)
    {
        x += w * (percent / 2.0f);
        y += h * (percent / 2.0f);
        w -= w * percent;
        h -= h * percent;
    }

    /// <summary>
    /// Make this area a square by shrinking it on its larger axis.
    /// </summary>
    void Area::NormalizeDown() {
        if(w > h) {
            x += (w - h) / 2.0f;
            w -= (w - h);
        }
        else if(h > w) {
            y += (h - w) / 2.0f;
            h -= (h - w);
        }
    }

    /// <summary>
    /// Make this area a square by expanding it on its smaller axis.
    /// </summary>
    void Area::NormalizeUp() {
        if(w > h) {
            y -= (w - h) / 2.0f;
            h += (w - h);
        }
        else if(h > w) {
            x -= (h - w) / 2.0f;
            w += (h - w);
        }
    }

    void Area::Render(irr::video::IVideoDriver * driver, irr::video::SColor color) {
        GUIImage::draw2DRectangle(
            driver,
            irr::core::rect<irr::s32>(int(x), int(y), int(x + w), int(y + h)),
            color);
    }
}