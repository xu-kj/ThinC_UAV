#ifndef EVENT_H
#define EVENT_H

#include "globals.h"
#include <irrlicht.h>

enum UAV_EVENT_E { CAMERA_FAIL, GPS_FAIL, CHAT_REQUEST };

class Event {
public:
    Event(const irr::core::stringc &type, const irr::core::stringw &text_,
        irr::u32 start, irr::s32 id_)
        : text(text_), start_time(start), id(id_), activated(false)
    {
        if(type == "CAMERA_FAIL")
            this->type = CAMERA_FAIL;
        else if(type == "GPS_FAIL")
            this->type = GPS_FAIL;
        else if(type == "CHAT_REQUEST")
            this->type = CHAT_REQUEST;
        else
            this->type = CAMERA_FAIL;
    }

    bool activate() {
        //if(!activated && start_time < TICKS)
        irr::u32 time = MINUTES * 1000 * 60 + SECONDS * 1000;
        if(!activated && start_time < time) {
            activated = true;
            return true;
        }
        return false;
    }

    UAV_EVENT_E get_type() const { return type; }
    const irr::core::stringw & get_text() const { return text; }
    irr::u32 get_start_time() const { return start_time; }
    irr::s32 get_id() const { return id; }
    
private:
    UAV_EVENT_E type;
    irr::core::stringw text;
    irr::u32 start_time;
    irr::s32 id;

    bool activated;
};

#endif /* EVENT_H */