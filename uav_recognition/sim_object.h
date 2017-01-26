#ifndef SIM_OBJECT_H
#define SIM_OBJECT_H

#include "globals.h"

#include <irrlicht.h>
#include <list>

typedef irr::core::vector3d<irr::f32> vec3d;
typedef irr::core::position2d<irr::f32> pos2d;
typedef irr::core::stringw strw;
typedef irr::video::SColor Color;

class SimObject
{
public:
    SimObject(const strw &name_, const vec3d &position_, const Color &color_,
        bool asDegrees = true);
    void setColor(Color new_color) { color = new_color; }

    // returns the position in 3D pixels
    vec3d getPosition() const {return position;}
    pos2d getPosition2D() const;
    //pos2d getPositionDegrees() const;
    strw getName() const {return name;}
    Color getColor() const {return color;}

    void setObjectPosition(vec3d position_) {position = position_;}

protected:
    vec3d position;
    strw name;
    Color color;
};

#endif