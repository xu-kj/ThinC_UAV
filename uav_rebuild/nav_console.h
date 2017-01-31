#ifndef NAV_CONSOLE_H
#define NAV_CONSOLE_H

#include "image.h"
#include "button.h"
#include "uav.h"

#include <irrlicht.h>

class NavWindow;

class UAVConsole {
public:
    UAVConsole();
    ~UAVConsole();

    // load_images(...) must be called before draw()
    // load_images(...) should only be called ONCE, even if there
    // is more than one UAVCamera object (lol, needs better management)
    void load_images(irr::IrrlichtDevice * device);
    // set_id(...) must be called before draw()
    void set_id(irr::IrrlichtDevice * device, irr::s32 id_, UAVObject * uav_);
    // load_buttons() must be called before draw()
    void load_buttons();

    void render_to_texture(const NavWindow * win);
    void draw_console(irr::video::IVideoDriver * driver, bool to_texture = true);
    void draw(irr::IrrlichtDevice * device);

    void force_render() {need_render = true;}
    //void check_buttons_over(irr::core::position2di cursor);

    // gps and camera fail
    bool gps_button_over(irr::core::position2di cursor) const {return gps_button->is_mouse_over(cursor);}
    bool cam_button_over(irr::core::position2di cursor) const {return cam_button->is_mouse_over(cursor);}
    bool get_gps_fail() const {return uav->hasGPSFail();}
    bool get_cam_fail() const {return uav->hasCamFail();}
    void set_gps_fail(bool value) {uav->setGPSFail(value); need_render = true;}
    void set_cam_fail(bool value) {uav->setCamFail(value); need_render = true;}

    UAVObject * get_uav() const {return uav;}

private:
    // number of objects in existance
    static irr::s32 ref_count;

    /* 2D Interface */
    // fonts
    static irr::gui::IGUIFont * font_small;
    static irr::gui::IGUIFont * font_large;
    static irr::gui::IGUIStaticText * text;
    static irr::gui::IGUIStaticText * TEXT;

    // images
    static GUIImage * uav_box;
    static GUIImage * uav_callsign;
    static GUIImage * uav_fuel_back;
    static GUIImage * uav_fuel_out;
    static GUIImage * uav_gps;
    static GUIImage * uav_cam;

    // rendered console texture
    GUIImage * render_image;
    irr::video::ITexture * render;
    bool need_render;

    // fuel level rectangle
    irr::core::rect<irr::s32> fuel_rect;

    // buttons
    UAVButton * gps_button;
    UAVButton * cam_button;

    /* Data */
    irr::s32 id;
    UAVObject * uav;
};

#endif /* NAV_CONSOLE_H */