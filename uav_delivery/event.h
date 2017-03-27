#ifndef EVENT_H
#define EVENT_H

#include "globals.h"
#include <irrlicht.h>

enum UAV_EVENT_E { 
	CHECK_POINT,
	CAMERA_FAIL, GPS_FAIL, CHAT_REQUEST,
	VIDEO_ALERT, VIDEO_ALERT_OFF, VIDEO_TEXT_CLEAR,
    AUDIO_ALERT, AUDIO_ALERT_OFF
};

class Event {
public:
    Event(const irr::core::stringc &type, const irr::core::stringw &text_,
        irr::u32 start, irr::s32 id_, int _ps, int _fs)
        : text(text_), start_time(start), id(id_), activated(false), 
        pair_seq(_ps), flood_seq(_fs)
    {
        if (type == "CAMERA_FAIL")
            this->type = UAV_EVENT_E::CAMERA_FAIL;
        else if (type == "GPS_FAIL")
            this->type = UAV_EVENT_E::GPS_FAIL;
        else if (type == "CHAT_REQUEST")
            this->type = UAV_EVENT_E::CHAT_REQUEST;
		else if (type == "VIDEO_ALERT")
			this->type = UAV_EVENT_E::VIDEO_ALERT;
        else if (type == "VIDEO_ALERT_OFF")
            this->type = UAV_EVENT_E::VIDEO_ALERT_OFF;
		else if (type == "VIDEO_TEXT_CLEAR")
			this->type = UAV_EVENT_E::VIDEO_TEXT_CLEAR;
		else if (type == "AUDIO_ALERT")
			this->type = UAV_EVENT_E::AUDIO_ALERT;
        else if (type == "AUDIO_ALERT_OFF")
            this->type = UAV_EVENT_E::AUDIO_ALERT_OFF;
        else
            this->type = UAV_EVENT_E::CHECK_POINT;
    }

    bool activate() {
        //if(!activated && start_time < TICKS)
        irr::u32 time = MINUTES * 1000 * 60 + SECONDS * 1000 + MILLISECONDS;
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
    int get_pair_seq() const { return pair_seq; }
    int get_flood_seq() const { return flood_seq; }
    
private:
    UAV_EVENT_E type;
    irr::core::stringw text;
    irr::u32 start_time;
    irr::s32 id;

    bool activated;
    int pair_seq;
    int flood_seq;
};

#endif /* EVENT_H */