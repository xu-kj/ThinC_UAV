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
#include <string>

class NavWindow;

class CamWindow : public UAVWindow {
private:
    std::vector<UAVCamera*> cams;
	int cam_width;
	int cam_height;
	int cam_interval;

    // rendered background texture
    GUIImage * render_image;
    GUIImage * render_image_overlay;
    irr::video::ITexture * render;
    irr::video::ITexture * render_overlay;
    bool need_render;
    bool started; // show "PRESS ENTER TO START" if false

    // data
    std::list<WaypointObject *> * wps;
    std::list<WaypointObject *>::iterator wp_it;
    std::list<SimObject *> * bases;
    std::list<SimObject *>::iterator base_it;
    std::list<UAVObject *> * uavs;
    std::list<UAVObject *>::iterator uav_it;

    // the city information
    CityScene * city;

	std::string text;
	irr::gui::IGUIStaticText * alarm_text;
	void draw_visual_alarm_text();

	irr::u32 xmodel_timer;

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

    virtual void force_render() {
		need_render = true;
	}

    CityScene * get_city() {
		return city;
	}

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

    void send_cam_message(int id, int message) {
		if (id < (int) cams.size() && cams[id] != nullptr) 
			cams[id]->cam_message(message);
	}

	std::string get_alarm_text() {
		return this->text;
	}

	void set_alarm_text(std::string _s) {
		this->text = _s;
	}
};

#endif /* CAM_WINDOW_H */