#include "waypoint.h"
#include "uav.h"
#include "output.h"
#include "camera.h"

#include <iostream>
#include <sstream>
#include <iomanip>

WaypointObject::WaypointObject(const strw &name, const vec3d &position, const Color &color, bool asDegrees)
    : SimObject(name, position, color, asDegrees)
    , state(UNASSIGNED)
    , has_feature(true)
    , reached(false)
    , sighted(false)
    , clicked_before_sight(false)
    , light_cue1(false)
    , light_cue2(false)
    , active(false)
    , tactor_cue1(false)
    , tactor_cue2(false)
{}

void WaypointObject::setActive(UAVObject * uav) {
    if(!active) {
        active = true;
        time_active = TICKS;

        // write to general log
        Output::Instance().WriteTick();
        Output::Instance().Write(uav->getName());
        Output::Instance().Write(" - ");
        Output::Instance().Write(uav->getTargetName());
        Output::Instance().WriteLine(" - buttons activated");

        // write to UAV file
        E_OUTPUT logTo = E_OUTPUT(uav->getCamera()->get_id());
        Output::Instance().WriteTick(logTo);
        Output::Instance().Write(uav->getName(), logTo);
        Output::Instance().Write(" - ", logTo);
        Output::Instance().Write(uav->getTargetName(), logTo);
        Output::Instance().WriteLine(" - buttons activated", logTo);
    }
}

void WaypointObject::setLightingCue1(UAVObject * uav) {
    if(!light_cue1) {
        light_cue1 = true;
        time_light_cue1 = TICKS;

        // write to general log
        Output::Instance().WriteTick();
        Output::Instance().Write(uav->getName());
        Output::Instance().Write(" - ");
        Output::Instance().Write(uav->getTargetName());
        Output::Instance().WriteLine(" - lighting cue 1 activated");

        // write to UAV file
        E_OUTPUT logTo = E_OUTPUT(uav->getCamera()->get_id());
        Output::Instance().WriteTick(logTo);
        Output::Instance().Write(uav->getName(), logTo);
        Output::Instance().Write(" - ", logTo);
        Output::Instance().Write(uav->getTargetName(), logTo);
        Output::Instance().WriteLine(" - lighting cue 1 activated", logTo);
    }
}

void WaypointObject::setLightingCue2(UAVObject * uav) {
    if(!light_cue2) {
        light_cue2 = true;
        time_light_cue2 = TICKS;

        // write to general log
        Output::Instance().WriteTick();
        Output::Instance().Write(uav->getName());
        Output::Instance().Write(" - ");
        Output::Instance().Write(uav->getTargetName());
        Output::Instance().WriteLine(" - lighting cue 2 activated");

        // write to UAV file
        E_OUTPUT logTo = E_OUTPUT(uav->getCamera()->get_id());
        Output::Instance().WriteTick(logTo);
        Output::Instance().Write(uav->getName(), logTo);
        Output::Instance().Write(" - ", logTo);
        Output::Instance().Write(uav->getTargetName(), logTo);
        Output::Instance().WriteLine(" - lighting cue 2 activated", logTo);
    }
}

void WaypointObject::setTactorCue1(UAVObject * uav) {
    if(!tactor_cue1) {
        tactor_cue1 = true;
        time_tactor_cue1 = TICKS;

        // write to general log
        Output::Instance().WriteTick();
        Output::Instance().Write(uav->getName());
        Output::Instance().Write(" - ");
        Output::Instance().Write(uav->getTargetName());
        Output::Instance().WriteLine(" - tactor cue 1 activated");

        // write to UAV file
        E_OUTPUT logTo = E_OUTPUT(uav->getCamera()->get_id());
        Output::Instance().WriteTick(logTo);
        Output::Instance().Write(uav->getName(), logTo);
        Output::Instance().Write(" - ", logTo);
        Output::Instance().Write(uav->getTargetName(), logTo);
        Output::Instance().WriteLine(" - tactor cue 1 activated", logTo);
    }
}

void WaypointObject::setTactorCue2(UAVObject * uav) {
    if(!tactor_cue2) {
        tactor_cue2 = true;
        time_tactor_cue2 = TICKS;

        // write to general log
        Output::Instance().WriteTick();
        Output::Instance().Write(uav->getName());
        Output::Instance().Write(" - ");
        Output::Instance().Write(uav->getTargetName());
        Output::Instance().WriteLine(" - tactor cue 2 activated");

        // write to UAV file
        E_OUTPUT logTo = E_OUTPUT(uav->getCamera()->get_id());
        Output::Instance().WriteTick(logTo);
        Output::Instance().Write(uav->getName(), logTo);
        Output::Instance().Write(" - ", logTo);
        Output::Instance().Write(uav->getTargetName(), logTo);
        Output::Instance().WriteLine(" - tactor cue 2 activated", logTo);
    }
}

void WaypointObject::setSighted(UAVObject * uav) {
    time_sighted = TICKS;
    // this->sighted_by = uav;
    sighted = true;
}

void WaypointObject::setReached(UAVObject * uav) {
    time_reached = TICKS;
    reached = true;

    // write to general log
    Output::Instance().WriteTick();
    Output::Instance().Write(uav->getName());
    Output::Instance().Write(" - reached ");
    Output::Instance().WriteLine(uav->getTargetName());

    // write to UAV file
    E_OUTPUT logTo = E_OUTPUT(uav->getCamera()->get_id());
    Output::Instance().WriteTick(logTo);
    Output::Instance().Write(uav->getName(), logTo);
    Output::Instance().Write(" - reached ", logTo);
    Output::Instance().WriteLine(uav->getTargetName(), logTo);
}

void WaypointObject::setDone(UAVObject * uav, E_WAYPOINT_RESPONSE result) {
    this->result = result;
    time_done = TICKS;

    if(!light_cue1) {
        clicked_before_sight = true;
        time_total = 0;
    }
    else {
        time_sighted_total = time_done - time_sighted_total;
    }
    time_total = time_done - time_light_cue1;

    // Write the general output
    std::stringstream gen_strm;
    gen_strm << irr::core::stringc(uav->getName()).c_str() << " - ";
    if(result == WAYPOINT_CORRECT && has_feature)
        gen_strm << "user confimed feature at ";
    else if(result == WAYPOINT_CORRECT && (!has_feature))
        gen_strm << "user cleared ";
    else if(result == WAYPOINT_INCORRECT && (!has_feature))
        gen_strm << "user gave false alarm at ";
    else if(result == WAYPOINT_INCORRECT && has_feature)
        gen_strm << "user gave false clearance at ";
    else if(result == WAYPOINT_MISSED)
        gen_strm << "user missed ";
    else
        gen_strm << "user is unsure of ";
    gen_strm << irr::core::stringc(getName()).c_str();
    if(result == WAYPOINT_CORRECT)
        gen_strm << " [CORRECT]";
    if(result == WAYPOINT_INCORRECT)
        gen_strm << " [INCORRECT]";
    if(result == WAYPOINT_MISSED)
        gen_strm << " [MISSED]";
    if(result == WAYPOINT_IS_UNSURE)
        gen_strm << " [UNSURE]";
    Output::Instance().WriteTick();
    Output::Instance().WriteLine(gen_strm.str().c_str());
    Output::Instance().WriteTick((E_OUTPUT)uav->getCamera()->get_id());
    Output::Instance().WriteLine(gen_strm.str().c_str(), (E_OUTPUT)uav->getCamera()->get_id());

    // Write the table output
    // format (| = some blank spaces):
    // UAV_NAME | WP_NAME | TIME_ACTIVE | TIME_LIGHT_CUE1 | TIME_LIGHT_CUE2 | TIME_TACTOR_CUE1 | TIME_TACTOR_CUE2 | TIME_SIGHTED | TIME_REACHED | TIME_DONE | USER_RESPONSE | CORRECTNESS | FEATURE_OR_NOT | BUTTON_PRESSED
    std::stringstream strm;
    int col = 10;
    strm << std::setprecision(3) << std::fixed;
    strm << std::setw(col) << std::left << irr::core::stringc(uav->getName()).c_str();
    strm << std::setw(col) << std::left << irr::core::stringc(getName()).c_str();
    strm << std::setw(col) << std::right << double(time_active) / 1000;

    if(light_cue1)  strm << std::right << std::setw(col) << double(time_light_cue1) / 1000;
    else            strm << std::right << std::setw(col) << "-";
    if(light_cue2)  strm << std::right << std::setw(col) << double(time_light_cue2) / 1000;
    else            strm << std::right << std::setw(col) << "-";

    if(tactor_cue1) strm << std::right << std::setw(col) << double(time_tactor_cue1) / 1000;
    else            strm << std::right << std::setw(col) << "-";
    if(tactor_cue2) strm << std::right << std::setw(col) << double(time_tactor_cue2) / 1000;
    else            strm << std::right << std::setw(col) << "-";

    if(sighted)     strm << std::setw(col) << std::right << double(time_sighted) / 1000;
    else            strm << std::right << std::setw(col) << "-";

    if(reached)     strm << std::setw(col) << std::right << double(time_reached) / 1000;
    else            strm << std::right << std::setw(col) << "-";

    strm << std::setw(col) << std::right << double(time_done) / 1000;
    strm << std::setw(2) << " ";

    if(result == WAYPOINT_MISSED)
        strm << std::setw(col) << std::left << "MISSED";
    else if(result == WAYPOINT_CORRECT)
        strm << std::setw(col) << std::left << "CORRECT";
    else if(result == WAYPOINT_INCORRECT)
        strm << std::setw(col) << std::left << "INCORRECT";
    else if(result == WAYPOINT_IS_UNSURE)
        strm << std::setw(col) << std::left << "UNSURE";

    strm << std::setw(2) << " ";
    if(has_feature)
        strm << std::setw(col) << std::left << "FEATURE";
    else
        strm << std::setw(col) << std::left << "NO FEATURE";

    strm << std::setw(6) << " ";
    strm << std::setw(col) << std::left;
    if(result == WAYPOINT_CORRECT && has_feature)
        strm << "CONFIRM";
    else if(result == WAYPOINT_CORRECT && (!has_feature))
        strm << "CLEAR";
    else if(result == WAYPOINT_INCORRECT && (!has_feature))
        strm << "CONFIRM";
    else if(result == WAYPOINT_INCORRECT && has_feature)
        strm << "CLEAR";
    else if(result == WAYPOINT_MISSED)
        strm << "-";
    else
        strm << "?";

    // Write to the current UAV's log
    E_OUTPUT logTo = E_OUTPUT(uav->getCamera()->get_id() + OUTPUT_TABLE_FILE);
    Output::Instance().WriteLine(strm.str().c_str(), logTo);

    // Write to the main UAV table log
    Output::Instance().WriteLine(strm.str().c_str(), OUTPUT_UAV_TABLE);
}