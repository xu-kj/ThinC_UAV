#ifndef CAMERA_H
#define CAMERA_H

#include "button.h"
#include "cityscene_node.h"
#include "image.h"
#include "network.h"
#include "uav.h"

#include <irrlicht.h>

class CamWindow;

class UAVCamera {
public:
    UAVCamera(
		irr::core::position2di pos_, 
		std::pair<int, int> cam_size, 
		CamWindow * win_);
    ~UAVCamera();

    // load_images(...) must be called before draw()
    // load_images(...) should only be called ONCE, even if there
    // is more than one UAVCamera object
    void load_images(irr::IrrlichtDevice * device);
    // set_id(...) must be called before draw()
    void set_id(irr::IrrlichtDevice * device, irr::s32 id_);
    int get_id() const {return id;}
    // load_buttons() must be called before draw()
    void load_buttons();

    void set_uav(UAVObject * uav_, irr::IrrlichtDevice * device);
    void draw_overlay(irr::IrrlichtDevice * device);
    void draw_view(irr::IrrlichtDevice * device, CityScene * city);
    void draw_background(irr::IrrlichtDevice * device);

    // return true if a render is required
    bool button_click(irr::core::position2di cursor);

    void force_render();
    bool need_render();

    void toggle_light();
    void draw_shade(irr::IrrlichtDevice * device) const;
    bool draw_sara_shade(irr::IrrlichtDevice * device) const; // returns if we shouldn't draw the baseline shading

    void cam_message(int message);

    void set_static(bool value) {staticOn = value;}
    
    irr::s32 getLight() const {
        if(auto_light)
            return getLightLevel(HIGH);
        return getLightLevel(light_level);
    }

    // added for sara's version
    std::list<SaraMatching::SaraSignal *> signals;

private:
    // number of objects in existance
    static irr::s32 ref_count;

    /* 2D Interface */
    // upper right corner of this "box"
    irr::core::position2di pos;
	int cam_size_x;
    int cam_size_y;

    // interface images
    static GUIImage * cam_box;
    static GUIImage * outline;
    static GUIImage * outline_br;
    static GUIImage * cam_static1;
    static GUIImage * cam_static2;
    static GUIImage * cam_static3;
    static GUIImage * cam_static4;
    static GUIImage * cam_static5;

    // button images
    static GUIImage * button_check;
    static GUIImage * button_check_down;
    static GUIImage * button_x;
    static GUIImage * button_x_down;
    static GUIImage * button_unsure;
    static GUIImage * button_unsure_down;
    static GUIImage * button_light;
    static GUIImage * button_light_off;
    static GUIImage * button_target;
    static GUIImage * button_target_down;
    bool button_clicked;

    // buttons
	UAVButton * indicator;
    UAVButton * v_indicator;
	UAVButton * a_indicator;
	
	UAVButton * btnPositive;
    UAVButton * btnNegative;
	UAVButton * btnUnsure;

    // fonts (show number of UAV in corner)
    irr::gui::IGUIStaticText * large_text;

    /* 3D Interface */
    irr::scene::ICameraSceneNode * camera;
    rect2di viewport;

    /* Data */
    irr::s32 id;                        // id
    irr::core::vector3df start_pos;     // start position (for progress bar)
    UAVObject * uav;                    // UAV pointer
    bool auto_light;                    // flag for auto light
    bool need_update;                   // flag to force a redraw
    float zoom;                         // current camera zoom
    bool zooming_out;
    bool zooming_in;
    bool zooming;

    bool v_alert_on;
    bool a_alert_on;

    CamWindow * win;

    // added with version 3
    E_LIGHT_LEVEL light_level;
    bool buttons_on;
    // return true if there's an indicator
    bool set_indicator(bool status);
    // return true if there's the corresponding alarm button
    bool set_video_alert(bool status);
    bool set_audio_alert(bool status);

    bool staticOn;
    float timeSinceLastStatic;
    int curStaticImage;
};

#endif /* CAMERA_H */