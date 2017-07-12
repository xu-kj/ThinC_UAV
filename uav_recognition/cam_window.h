#ifndef CAM_WINDOW_H
#define CAM_WINDOW_H

#include "camera.h"
#include "cityscene_node.h"
#include "uav.h"
#include "network.h"
#include "output.h"
#include "sim_object.h"
#include "waypoint.h"
#include "window.h"

#include <list>
#include <vector>
#include <iostream>

class NavWindow;

class CamWindow : public UAVWindow {
private:
    std::vector<UAVCamera *> cams;
	int cam_width;
	int cam_height;
	int cam_interval;

    // rendered background texture
    GUIImage *render_image;
    GUIImage *render_image_overlay;
    irr::video::ITexture *render;
    irr::video::ITexture *render_overlay;
    bool need_render;
    bool started; // show "PRESS ENTER TO START" if false

	// Boolean for paused functionality
	bool paused;
	int current_uav;

    // data
    std::list<WaypointObject *> *wps;
    std::list<WaypointObject *>::iterator wp_it;
    std::list<SimObject *> *bases;
    std::list<SimObject *>::iterator base_it;
    std::list<UAVObject *> *uavs;
    std::list<UAVObject *>::iterator uav_it;

    // the city information
    CityScene * city;

public:
    CamWindow(std::list<WaypointObject *> * wps_,
        std::list<SimObject *> * bases_,
        std::list<UAVObject *> * uavs_,
        int _cam_width,
        int _cam_height,
        int _cam_interval,
        irr::core::dimension2di position = irr::core::dimension2di(0,0),
        irr::video::E_DRIVER_TYPE driver = irr::video::EDT_DIRECT3D9);
    ~CamWindow();

    virtual bool load();
    virtual void draw();
    virtual void load_images();
    void render_to_texture();

    virtual void event_mouse_down();
    virtual void event_key_down(wchar_t key);


    virtual void force_render() {need_render = true;}

    CityScene * get_city() {return city;}

	int get_cam_width() const {
		return cam_width;
	}
	int get_cam_height() const {
		return cam_height;
	}
	int get_cam_interval() const {
		return cam_interval;
	}

    bool get_started() const { 
		return started; 
	}
    void set_started(bool _started) {
		started = _started; 
	}

	// Getter and setter functions for paused
	bool get_paused() const {
		return paused;	
	}

	void reset_paused() {
		paused = false;
	}

	void set_paused() {
		paused = true;
	}

	void set_score(int uav_number, int score) {
		switch(uav_number) {
			case 1:
			case 2:
			case 3:
				uav_one_score = score;	
				uav_two_score = score;
				uav_three_score = score;
			case 4:
			case 5:
			case 6:
				uav_four_score = score;
				uav_five_score = score;
				uav_six_score = score;
		}
	}

	void record_scores() {
		Output::Instance().RecordTrustScore(1, uav_one_score);
		Output::Instance().RecordTrustScore(2, uav_two_score);
		Output::Instance().RecordTrustScore(3, uav_three_score);
		Output::Instance().RecordTrustScore(4, uav_four_score);
		Output::Instance().RecordTrustScore(5, uav_five_score);
		Output::Instance().RecordTrustScore(6, uav_six_score);
	}

    void send_cam_message(int id, int message) {
		if (id < (int) cams.size() && cams[id] != nullptr) 
			cams[id]->cam_message(message);
	}
};

#endif /* CAM_WINDOW_H */