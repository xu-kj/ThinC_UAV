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

class NavWindow;

class CamWindow : public UAVWindow {
private:
    std::vector<UAVCamera*> cams;

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

public:
    CamWindow(std::list<WaypointObject *> * wps_,
        std::list<SimObject *> * bases_,
        std::list<UAVObject *> * uavs_,
        WindowResolution_e resolution = R1024X768,
        irr::core::dimension2di position = irr::core::dimension2di(0,0),
        irr::video::E_DRIVER_TYPE driver = irr::video::EDT_DIRECT3D9,
        int numCams = 3);
    ~CamWindow();

    virtual bool load(int numCams);
    virtual void draw();
    virtual void load_images();
    void render_to_texture();

    virtual void event_mouse_down();
    virtual void event_key_down(wchar_t key);

    virtual void force_render() {need_render = true;}

    CityScene * get_city() {return city;}

    bool get_started() const { return started; }
    void set_started(bool _started) { started = _started; }

    void send_cam_message(int id, int message) {cams[id]->cam_message(message);}
    void send_sara_cam_message(int id, SimSaraCamPacket * p);

    void update_cams(float time);
};

#endif /* CAM_WINDOW_H */