#include "sim_object.h"

#include <iostream>

using namespace std;

SimObject::SimObject(const strw &name_, const vec3d &position_, const Color &color_, bool asDegrees)
    : name(name_), position(position_), color(color_)
{
    static int id = 0;
    id++;
}

pos2d SimObject::getPosition2D() const
{
    return pos2d(position.X, position.Z);
}

// FUNCTION NO LONGER REQUIRED
/*
pos2d SimObject::getPositionDegrees() const
{
    //irr::s32 div = irr::s32(irr::f32(WORLD_END_PIXELS_X - WORLD_START_PIXELS_X) /
    //  (WORLD_END_LON - WORLD_START_LON));
    //return pos2d(position.X / div, position.Z / div);

    return pos2d(position.X / PIXELS_PER_DEGREE, position.Z / PIXELS_PER_DEGREE);
}
*/
