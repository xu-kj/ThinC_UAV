#include "uav.h"
#include "cam_window.h"
#include "uav_tactors.h"
#include "output.h"

#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

const double DEGREES_IN_RAD = 57.2957795;
const double PI = 3.14159265;

irr::f32 FUEL_FACTOR = .0001f;
irr::f32 SPEED_FACTOR = 100.f;
irr::f32 TURN_FACTOR = 1.f;

bool USE_LIGHT_CUES = true;
irr::s32 LOW_LIGHT_LEVEL;
irr::s32 MEDIUM_LIGHT_LEVEL;
irr::s32 HIGH_LIGHT_LEVEL;
irr::s32 BASE_LIGHT_LEVEL;
irr::f32 MEDIUM_LIGHT_DIST = 400.f;
irr::f32 HIGH_LIGHT_DIST = 200.f;

bool USE_TACTOR_CUES = false;
irr::f32 HIGH_TACTOR_DIST = 400.f;
irr::f32 LOW_TACTOR_DIST = 200.f;

irr::f32 FEATURE_VISIBLE_DIST = 450.f;
irr::f32 FEATURE_MISSED_DIST = 300.f;
bool LIGHT_AFTER_PASS = true;

irr::s32 getLightLevel(E_LIGHT_LEVEL level) {
    switch(level) {
    case LOW:
        return LOW_LIGHT_LEVEL;
    case MEDIUM:
        return MEDIUM_LIGHT_LEVEL;
    case HIGH:
        return HIGH_LIGHT_LEVEL;
    default:
        return LOW_LIGHT_LEVEL;
    }
}

UAVObject::UAVObject(const strw &name,
    const vec3d &position,
    const Color &color,
    bool asDegrees,
    const vec3d &forward,
    const vec3d &velocity)
    : SimObject(name, position, color, asDegrees),
    fuel(1.0f), base(0), state(DONE), stats_done(false),
    network_target(0), stopped(false), deliver(false)
{
    gps_fail = cam_fail = false;
    if(USE_LIGHT_CUES) {
        light = LOW;
    }
    else {
        light = HIGH;
    }

    speed = SPEED_FACTOR;

    start_position = position;
    facing = irr::core::vector3df(1,0,1);
    light_override = false;

    target_visible = false;
    target_passed = false;

    network_target = new SimObject("lafjksf", vec3d(0,0,0), Color(0,0,0,0));
}

void UAVObject::network_update(irr::f32 time) {}

void UAVObject::network_set_target(vec3d position_, vec3d target_) {
    network_target->setObjectPosition(target_);
    this->setObjectPosition(position_);
    position = position_;

    //std::cout << "position (" << this->getName().c_str() << "): " << position.X << ", " << position.Z << std::endl;

    facing = (target_ - position_).normalize();
    facing.Y = 0; // get rid of floating-point errors
    facing.normalize();
}

void UAVObject::update(irr::f32 time) {
    // the following code has confusing names, so here's a key:
    // 1) getTarget() : the currently ACTIVE waypoint (or base) - buttons clicked refer to this
    // 2) cur_target  : the waypoint or base the UAV is actually MOVING toward

    // move the UAV
    if (!stopped && (state == WP || state == BASE)) {
        position += facing * (time * speed);
    }

	if (stopped) {
		cout << "alt: " << alt << ", pos.y: " << position.Y << ", f_alt: " << f_alt << endl;
		if (deliver && position.Y > alt) {
			position.Y -= 0.25;
			if (position.Y <= alt)
				deliver = false;
		}
		else if (!deliver && position.Y < f_alt) {
			position.Y += 0.25;
			if (position.Y >= f_alt) {
				position.Y = f_alt;
				stopped = false;
				Output::Instance().RecordEvent(cam_id + 1, UAV_EVENT::UAV_MOVING,
					(double) position.X, (double) position.Y, (double) position.Z);
			}
		}
	}

    // decide which target to move toward
    vec3d cur_target;
    if (target_passed) 
        cur_target = getNextTarget();
    else 
        cur_target = getTarget();

    switch(state) {
    case WP:
        // handle cues
        if (USE_LIGHT_CUES) 
            light = LOW;
        if (!goingToBase()) {
            // display when the button activated in the log
            if (activeButtons())
                wps.front()->setActive(this);

            // Light Cue stuff
            if (USE_LIGHT_CUES) {
                if (position.getDistanceFrom(cur_target) < HIGH_LIGHT_DIST) {
                    wps.front()->setLightingCue2(this);
                    light = HIGH;
                }
                else if (LIGHT_AFTER_PASS && target_passed && position.getDistanceFrom(getTarget()) < FEATURE_MISSED_DIST)
                    light = HIGH;
                else if (position.getDistanceFrom(cur_target) < MEDIUM_LIGHT_DIST) {
                    wps.front()->setLightingCue1(this);
                    light = MEDIUM;
                }
            }

            // Tactor Cue stuff
            if (USE_TACTOR_CUES) {
                if (position.getDistanceFrom(cur_target) < HIGH_TACTOR_DIST) {
                    if (!tactorHighSwitch) {
                        wps.front()->setTactorCue2(this);
                        //tactors.SignalHigh(cam->get_id());
                        tactorHighSwitch = true;
                    }
                }
                else if (position.getDistanceFrom(cur_target) < LOW_TACTOR_DIST) {
                    if (!tactorLowSwitch) {
                        wps.front()->setTactorCue1(this);
                        //tactors.SignalLow(cam->get_id());
                        tactorLowSwitch = true;
                    }
                }
                else {
                    tactorLowSwitch = false;
                    tactorHighSwitch = false;
                }
            }
        }

		//if (cam_id == 1) {
		//	cout << "distance: " << position.getDistanceFrom(cur_target) << endl;
		//	cout << "\tthreshold: " << FEATURE_VISIBLE_DIST << endl;
		//	cout << "\tflag: " << target_visible << endl;
		//}

        // decide whether the target has been reached
        if (!target_visible && position.getDistanceFrom(getTarget()) < FEATURE_VISIBLE_DIST) {
            target_visible = true;
            wps.front()->setSighted(this);

			Output::Instance().RecordEvent(cam_id + 1, 
                wps.front()->getFeature() ? UAV_EVENT::WAYPOINT_TARGET_SIGHTED : UAV_EVENT::WAYPOINT_NONTARGET_SIGHTED, 
				(double) position.X, (double) position.Y, (double) position.Z);

            Output::Instance().WriteTick();
            Output::Instance().Write(getName());
            Output::Instance().Write(" - ");
            Output::Instance().Write(getTargetName());
            Output::Instance().WriteLine(" - waypoint visible");

            Output::Instance().WriteTick((E_OUTPUT)cam->get_id());
            Output::Instance().Write(getName(), (E_OUTPUT)cam->get_id());
            Output::Instance().Write(" - ", (E_OUTPUT)cam->get_id());
            Output::Instance().Write(getTargetName(), (E_OUTPUT)cam->get_id());
            Output::Instance().WriteLine(" - waypoint visible", (E_OUTPUT)cam->get_id());
        }
        if (!target_passed && position.getDistanceFrom(getTarget()) < 10.0) {
            stopped = true;

            Output::Instance().RecordEvent(cam_id + 1, 
                UAV_EVENT::INDICATOR_ON, 
                (double) position.X, (double) position.Y, (double) position.Z);
            send_cam_message(0);
			
            Output::Instance().RecordEvent(cam_id + 1, 
                wps.front()->getFeature() ? UAV_EVENT::WAYPOINT_TARGET_ARRIVED : UAV_EVENT::WAYPOINT_NONTARGET_ARRIVED, 
				(double) position.X, (double) position.Y, (double) position.Z);

			Output::Instance().RecordEvent(cam_id + 1, 
				UAV_EVENT::UAV_STOPPED, 
				(double) position.X, (double) position.Y, (double) position.Z);

            target_passed = true;
            wps.front()->setReached(this);
        }
        if (target_passed && position.getDistanceFrom(getTarget()) >= FEATURE_MISSED_DIST) {
            setUnsure(true);

            send_cam_message(1);
        }

        break;

    case BASE:
        if (position.getDistanceFrom(getTarget()) < 10.0) {
            state = DONE;
        }
        if (USE_LIGHT_CUES) {
            light = LOW;
        }
        break;

    default:
        break;
    }

    // update facing
    vec3d from = facing.normalize();
    vec3d to = (cur_target - position).normalize();
    irr::f32 angle = acos(from.dotProduct(to));
    if (angle != angle) angle = 0;

    if (angle > .01) {
        // get the rotational axis
        irr::f32 rot_angle = (TURN_FACTOR * time);
        if (rot_angle > angle) rot_angle = angle;
        vec3d rot_axis = (from.crossProduct(to)).normalize();
        rot_axis.X = rot_axis.Z = 0;
        if (rot_axis.Y == 0) rot_axis.Y = 1.f;

        // get the rotation matrix
        irr::core::quaternion quat;
        irr::core::matrix4 rot_matrix;
        quat.fromAngleAxis(rot_angle, rot_axis);
        quat.getMatrix(rot_matrix);

        // turn a little bit
        rot_matrix.rotateVect(facing);
    }
    else {
        facing = (cur_target - position).normalize();
    }
    facing.Y = 0; // get rid of floating-point errors
    facing.normalize();

    // update fuel
    if (state != DONE && fuel > 0.01f) {
        fuel -= (time * FUEL_FACTOR);
        if (fuel < 0.01f)
            fuel = 0.01f;
    }
}

void UAVObject::setConfirmed() {
    irr::core::stringw str;
    if (state == WP) {
        WaypointObject * removed_wp = 0;
        removed_wp = wps.front();
        removed_wp->setConfirmed();
        wps.pop_front();
        done_wps.push_back(removed_wp);

		//stopped = false;
        deliver = true;
		f_alt = position.Y;
		alt = 30.0;

		Output::Instance().RecordEvent(cam_id + 1, 
			UAV_EVENT::USER_TARGET, 
			(double) position.X, (double) position.Y, (double) position.Z);

        bool correct = (removed_wp->getFeature() == true);
        if (correct) 
            removed_wp->setDone(this, WAYPOINT_CORRECT);
        else 
            removed_wp->setDone(this, WAYPOINT_INCORRECT);

        if (wps.empty())
            state = BASE;
        if (USE_LIGHT_CUES)
            light = LOW;

        target_passed = target_visible = false;
    }
}

void UAVObject::setNotThere() {
    irr::core::stringw str;
    if (state == WP) {
        WaypointObject * removed_wp = 0;
        removed_wp = wps.front();
        removed_wp->setCleared();
        wps.pop_front();
        done_wps.push_back(removed_wp);

        stopped = false;

		Output::Instance().RecordEvent(cam_id + 1, 
			UAV_EVENT::USER_NONTARGET, 
			(double) position.X, (double) position.Y, (double) position.Z);

		Output::Instance().RecordEvent(cam_id + 1, 
			UAV_EVENT::UAV_MOVING, 
			(double) position.X, (double) position.Y, (double) position.Z);

        bool correct = (removed_wp->getFeature() == false);
        if (correct) 
            removed_wp->setDone(this, WAYPOINT_CORRECT);
        else 
            removed_wp->setDone(this, WAYPOINT_INCORRECT);

        if (wps.empty())
            state = BASE;
        if (USE_LIGHT_CUES)
            light = LOW;

        target_passed = target_visible = false;
    }
}

void UAVObject::setUnsure(bool missed) {
    irr::core::stringw str;
    if (state == WP) {
        WaypointObject * removed_wp = 0;
        removed_wp = wps.front();
        removed_wp->setUnsure();
        wps.pop_front();
        done_wps.push_back(removed_wp);

        stopped = false;

		Output::Instance().RecordEvent(cam_id + 1, 
			UAV_EVENT::USER_UNSURE, 
			(double) position.X, (double) position.Y, (double) position.Z);

        if (missed) 
            removed_wp->setDone(this, WAYPOINT_MISSED);
        else 
            removed_wp->setDone(this, WAYPOINT_IS_UNSURE);

        if (wps.empty())
            state = BASE;
        if (USE_LIGHT_CUES)
            light = LOW;

        target_passed = target_visible = false;
    }
}

void UAVObject::assign_base(SimObject * base_) {
    base = base_;
    base->setColor(getColor());
}

void UAVObject::assign_waypoint(WaypointObject * wp) {
    wp->setAssigned(getColor());
    wps.push_back(wp);

    state = WP;
    facing = (getTarget() - position).normalize();
    facing.Y = 0; // get rid of floating-point errors
    facing.normalize();
}

vec3d UAVObject::getTarget() const {
    if (!getCurWP())
        return base->getPosition();
    else
        return getCurWP()->getPosition();
}

irr::core::stringw UAVObject::getTargetName() const {
    if (!getCurWP())
        return base->getName();
    else
        return getCurWP()->getName();
}

vec3d UAVObject::getNextTarget() const {
    if (!getNextWP())
        return base->getPosition();
    else
        return getNextWP()->getPosition();
}

bool UAVObject::goingToBase() const {
    if (!atBase()) {
        if (!getCurWP())
            return true;
    }
    return false;
}

const std::list<WaypointObject *> * UAVObject::getWPList() const {
    return &wps;
}

const std::list<WaypointObject *> * UAVObject::getDoneWPList() const {
    return &done_wps;
}

irr::f32 UAVObject::getRotAngle2D() const {
    if (state == DONE)
        return 0.0f;

    irr::f32 angle = atan(facing.X / (-facing.Z));
    angle *= irr::f32(DEGREES_IN_RAD);

    if (facing.Z >= 0)
        angle += 180.0f;
    angle += 180.0f;

    return angle;
}

bool UAVObject::activeButtons() const {
    if (!atBase() && !goingToBase()) {
        if (position.getDistanceFrom(getTarget()) < MEDIUM_LIGHT_DIST)
            return true;
    }
    return false;
}

const SimObject * UAVObject::getBase() const {
    return base;
}

WaypointObject * UAVObject::getCurWP() const {
    if (wps.empty())
        return 0;
    else
        return wps.front();
}

WaypointObject * UAVObject::getNextWP() const {
    if (wps.size() < 2)
        return 0;
    else
        return *(++wps.begin());
}

void UAVObject::writeSummary() {
    std::stringstream strm;
    strm << endl << "SUMMARY" << endl;

    stats_done = true;
    missed = 0;
    unsure = 0;
    correct = 0;
    incorrect = 0;
    total = 0;

    missedPercent = 0;
    unsurePercent = 0;
    correctPercent = 0;
    incorrectPercent = 0;

    // find some shit
    std::list<WaypointObject *>::iterator it;
    for (it = done_wps.begin(); it != done_wps.end(); it++) {
        E_WAYPOINT_RESPONSE result = (*it)->getResult();
        switch(result) {
            case WAYPOINT_MISSED:
                missed++;
                break;
            case WAYPOINT_IS_UNSURE:
                unsure++;
                break;
            case WAYPOINT_CORRECT:
                correct++;
                break;
            case WAYPOINT_INCORRECT:
                incorrect++;
                break;
            default:
                break;
        }
    }
    total = correct + incorrect + unsure + missed;
    if (total > 0) {
        correctPercent = (double(correct) / double(total)) * 100;
        incorrectPercent = (double(incorrect) / double(total)) * 100;
        unsurePercent = (double(unsure) / double(total)) * 100;
        missedPercent = (double(missed) / double(total)) * 100;
    }

    strm << fixed << setprecision(2);
    strm << "Correct:  " << setw(5) << right << correct << "  " << setw(10) << right << correctPercent << "%" << endl;
    strm << "Incorrect:" << setw(5) << incorrect << "  " << setw(10) << right << incorrectPercent << "%" << endl;
    strm << "Unsure:   " << setw(5) << unsure << "  " << setw(10) << right << unsurePercent << "%" << endl;
    strm << "Missed:   " << setw(5) << missed << "  " << setw(10) << right << missedPercent << "%" << endl;
    strm << "Total:    " << setw(5) << total << endl;

    E_OUTPUT logTo = E_OUTPUT(getCamera()->get_id() + OUTPUT_TABLE_FILE);
    Output::Instance().WriteLine(strm.str().c_str(), logTo);
}

void UAVObject::send_cam_message(int i) { 
	cam_ptr->cam_message(i); 
}