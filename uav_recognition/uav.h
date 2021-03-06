#ifndef UAV_H
#define UAV_H

#include "sim_object.h"
#include "waypoint.h"
#include "sara_signals.h"

#include <iostream>
#include <fstream>

enum E_UAV_STATE {WP, BASE, DONE};
enum E_LIGHT_LEVEL {LOW, MEDIUM, HIGH};
irr::s32 getLightLevel(E_LIGHT_LEVEL level);

class UAVCamera;
class UAVObject : public SimObject
{
public:
    UAVObject(const strw &name,
        const vec3d &position,
        const Color &color,
        bool asDegrees = true,
        const vec3d &forward = vec3d(1.0, 0.0, 0.0),
        const vec3d &velocity = vec3d());
    ~UAVObject();

    void set_camera(int _cam_id, UAVCamera *_cam_ptr) { 
        cam_id = _cam_id; 
        cam_ptr = _cam_ptr;
		
		// Initialize confidence file.
		std::string comment;
		switch (cam_id) {
			case 0:
				confidence_fs.open("..\\confidence\\confidence1.txt", std::fstream::in);
				break;
			case 1:
				confidence_fs.open("..\\confidence\\confidence2.txt", std::fstream::in);
				break;
			case 2:
				confidence_fs.open("..\\confidence\\confidence3.txt", std::fstream::in);
				break;
			case 3:
				confidence_fs.open("..\\confidence\\confidence4.txt", std::fstream::in);
				break;
			case 4:
				confidence_fs.open("..\\confidence\\confidence5.txt", std::fstream::in);
				break;
			case 5:
				confidence_fs.open("..\\confidence\\confidence6.txt", std::fstream::in);
				break;
			default:
				break;
		}
		std::getline(confidence_fs, comment);
		Output::Instance().RecordConfidenceFile(cam_id, comment);
	}

	void set_audio(bool _audio) { audio = _audio; }
	void set_visual(bool _visual) { visual = _visual; }

    void send_cam_message(int i);

    void network_update(irr::f32 time);
    void network_set_target(vec3d position, vec3d target);

    void update(irr::f32 time);
    void setConfirmed();
    void setNotThere();
    void setUnsure(bool missed = false);

    void assign_base(SimObject * base_);
    void assign_waypoint(WaypointObject * wp);

    // position and targets
    void setStartPosition(irr::core::vector3df pos) {start_position = pos;}
    irr::core::vector3df getStartPosition() const {return start_position;}
    irr::core::position2df getStartPosition2D() const {return irr::core::position2df(start_position.X, start_position.Z);}
    irr::core::vector3df getForward() const {return facing;}
    irr::f32 getRotAngle2D() const;
    vec3d getTarget() const;
    irr::core::stringw getTargetName() const;
    vec3d getNextTarget() const;
    irr::f32 getFuel() const {return fuel;}
    void setFuel(float amount) { fuel = amount; }
    bool atBase() const {return state == DONE;}
    bool goingToBase() const;

    // lighting
    irr::s32 getLight() const {if(light_override) return getLightLevel(HIGH); return getLightLevel(light);}
    bool getLightOverride() const {return light_override;}
    void setLightOverride(bool value) {light_override = value;}

    // buttons
    bool activeButtons() const;
	bool getBottom() const;
	void setBottom(bool b);

	int getBotcor();
	int getBotincor();
	int getCorrej();
	void setBotcor();
	void setBotincor();
	void setCorrej();

    // camera/gps failure
    void setCamera(UAVCamera * cam_) {cam = cam_;}
    UAVCamera * getCamera() const {return cam;}
    bool hasGPSFail() const {return gps_fail;}
    bool hasCamFail() const {return cam_fail;}
    void setGPSFail(bool value) {gps_fail = value;}
    void setCamFail(bool value) {cam_fail = value;}

    /* terrible */
    const std::list<WaypointObject *> * getWPList() const;
    const std::list<WaypointObject *> * getDoneWPList() const;
    const SimObject * getBase() const;

    // output
    void writeSummary();
    bool getStatsDone() const {return stats_done;}

	std::string get_next_confidence_level() {
		std::string next_confidence_level;
		std::cout << "Reading next confidence." << std::endl;
		confidence_fs >> next_confidence_level;
		std::cout << "Finished reading next confidence." << std::endl;
		return next_confidence_level;
	}

private:
    int cam_id;
    UAVCamera * cam_ptr;

    WaypointObject * getCurWP() const;
    WaypointObject * getNextWP() const;

    irr::f32 fuel;
    E_UAV_STATE state;

    bool target_visible;
    bool target_passed;
	bool bottom;

	int botcor;
	int botincor;
	int correj;

    irr::core::vector3df start_position;
    irr::core::vector3df facing;

    SimObject * base;
    std::list<WaypointObject *> wps;
    std::list<WaypointObject *> done_wps;

    bool light_override;
    E_LIGHT_LEVEL light;

    bool cam_fail;
    bool gps_fail;

    UAVCamera * cam;

    // tactor stuff - to make each tactor buzz only once
    bool tactorLowSwitch;
    bool tactorHighSwitch;

    // stats crap
    bool stats_done;

    // override all this functionality and shit
    SimObject * network_target;

	std::fstream confidence_fs;

	bool audio;
	bool visual;

public:
    int missed;
    int unsure;
    int correct;
    int incorrect;
    int total;
    double missedPercent;
    double unsurePercent;
    double correctPercent;
    double incorrectPercent;
};

#endif /* UAV_H */