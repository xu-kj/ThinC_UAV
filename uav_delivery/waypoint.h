#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "color.h"
#include "sim_object.h"
#include "globals.h"

#include <list>

enum E_WAYPOINT_RESPONSE {
    WAYPOINT_MISSED,
    WAYPOINT_IS_UNSURE,
    WAYPOINT_CORRECT,
    WAYPOINT_INCORRECT
};

class UAVObject;

class WaypointObject: public SimObject
{
public:
    WaypointObject(const strw &name, const vec3d &position, const Color &color, bool asDegrees = true);

    void setAssigned(irr::video::SColor color_) { 
        state = ASSIGNED; 
        color = color_;
    }
    void setReAssigned(irr::video::SColor color_) {
        state = REASSIGNED; 
        color = color_;
    }
    void setUnAssigned() {
        state = UNASSIGNED; 
        color = color::COLOR_GRAY;
    }
    void setConfirmed() {
        state = CONFIRMED; 
        color = color::WAYPOINT_CONFIRMED;
    }
    void setCleared() {
        state = DENIED; 
        color = color::WAYPOINT_CLEAR;
    }
    void setUnsure() {
        state = UNSURE; 
        color = color::WAYPOINT_UNSURE;
    }

    // for data collection
    void setFeature(bool exists) {
        has_feature = exists;
    }
    bool getFeature() const {
        return has_feature;
    }
    void set_indicated(bool _indicated) {
        indicated = _indicated;
    }
    bool get_indicated() const {
        return indicated;
    }

    void setActive(UAVObject * uav);
    void setLightingCue1(UAVObject * uav);
    void setLightingCue2(UAVObject * uav);
    void setTactorCue1(UAVObject * uav);
    void setTactorCue2(UAVObject * uav);
    void setSighted(UAVObject * uav);
    void setReached(UAVObject * uav);
    void setDone(UAVObject * uav, E_WAYPOINT_RESPONSE result);

    irr::u32 getTimeCue1() const {return time_light_cue1;}
    irr::u32 getTimeCue2() const {return time_light_cue2;}
    irr::u32 getTimeSighted() const {return time_sighted;}
    irr::u32 getTimeReached() const {return time_reached;}
    irr::u32 getTimeDone() const    {return time_done;}
    irr::u32 getTimeTotal() const   {return time_total;}
    irr::u32 getTimeSightedTotal() const {return time_sighted_total;}
    bool getDoneNotInSight() const {return clicked_before_sight;}
    E_WAYPOINT_RESPONSE getResult() const {return result;}

private:
    enum E_WAYPOINT_STATE {UNASSIGNED, ASSIGNED, REASSIGNED, CONFIRMED, DENIED, UNSURE};
    E_WAYPOINT_STATE state;

    // stats stuff
    // UAVObject * sighted_by;
    bool has_feature;
    bool indicated;
    bool reached;
    bool sighted;
    bool active;
    bool light_cue1;
    bool light_cue2;
    bool tactor_cue1;
    bool tactor_cue2;
    bool clicked_before_sight;

    irr::u32 time_active;
    irr::u32 time_light_cue1;
    irr::u32 time_light_cue2;
    irr::u32 time_tactor_cue1;
    irr::u32 time_tactor_cue2;
    irr::u32 time_sighted;
    irr::u32 time_reached;
    irr::u32 time_done;
    irr::u32 time_total;
    irr::u32 time_sighted_total;

    E_WAYPOINT_RESPONSE result;
};

#endif /* WAYPOINT_H */