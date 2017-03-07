#ifndef UFO_H
#define UFO_H

#include "color.h"
#include "sim_object.h"
#include "globals.h"
#include "output.h"

#include <irrlicht.h>

const irr::video::SColor UFO_COLOR = color::COLOR_GRAY;
const irr::video::SColor UFO_REPORTED_COLOR = color::COLOR_YELLOW;

class UFOObject: public SimObject
{
public:
    UFOObject(const strw &name, const vec3d &position, const vec3d &forward_,
        irr::f32 speed_, const Color &color, irr::u32 start_time_, irr::u32 end_time_)
        : SimObject(name, position, color, false),
        speed(speed_), forward(forward_), start_time(start_time_),
        end_time(end_time_), is_reported(false), started(false), ended(false)
    {}

    // return true when it's time to get rid of this object
    bool update(irr::f32 time) {
        if(start_time < TICKS) {
            if(!started) {
                started = true;

                Output::Instance().WriteTick();
                Output::Instance().Write(getName());
                Output::Instance().WriteLine(" - bogie appeared");

                Output::Instance().WriteTick(OUTPUT_EVENTS);
                Output::Instance().Write(getName(), OUTPUT_EVENTS);
                Output::Instance().WriteLine(" - bogie appeared", OUTPUT_EVENTS);
            }
            position += forward * speed * time;
        }

        if(end_time < TICKS) {
            if(!ended) {
                ended = true;
                
                Output::Instance().WriteTick();
                Output::Instance().Write(getName());
                Output::Instance().WriteLine(" - bogie disappeared");

                Output::Instance().WriteTick(OUTPUT_EVENTS);
                Output::Instance().Write(getName(), OUTPUT_EVENTS);
                Output::Instance().WriteLine(" - bogie disappeared", OUTPUT_EVENTS);
            }
            return true;
        }
        return false;
    }

    bool show() const {
        if(start_time < TICKS && end_time > TICKS)
            return true;
        return false;
    }

    void report() {
        Output::Instance().WriteTick();
        Output::Instance().Write(getName());
        Output::Instance().WriteLine(" - bogie reported");

        Output::Instance().WriteTick(OUTPUT_EVENTS);
        Output::Instance().Write(getName(), OUTPUT_EVENTS);
        Output::Instance().WriteLine(" - bogie reported", OUTPUT_EVENTS);

        is_reported = true;
        setColor(UFO_REPORTED_COLOR);
    }

    bool reported() const {return is_reported;}

private:
    irr::f32 speed;
    irr::u32 start_time;
    irr::u32 end_time;
    vec3d forward;

    bool started;
    bool ended;
    bool is_reported;
};

#endif /* UFO_H */