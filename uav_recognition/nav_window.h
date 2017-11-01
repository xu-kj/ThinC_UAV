#ifndef NAV_WINDOW_H
#define NAV_WINDOW_H

#include "window.h"
#include "map.h"
#include "chatbox.h"
#include "nav_console.h"
#include "sim_object.h"
#include "waypoint.h"
#include "event.h"
#include "ufo.h"
#include "uav.h"

#include <list>

class CamWindow;

irr::video::SColor getColorFromId(irr::s32 id);

class NavWindow : public UAVWindow {
public:
    NavWindow(std::list<WaypointObject *> * wps_,
        std::list<SimObject *> * bases_,
        std::list<UAVObject *> * uavs_,
        std::list<UFOObject *> * ufos_,
        std::list<Event *> * events_,
        int nav_width,
        int nav_height,
        irr::core::dimension2di position = irr::core::dimension2di(0,0),
        irr::video::E_DRIVER_TYPE driver = irr::video::EDT_OPENGL);

    virtual bool load();
    virtual void draw();
    virtual void load_images();
    void load_fonts();

    // timed-event handling
    void triggerEvent(Event * e);

    // input event handlers
    virtual void event_mouse_down();
    virtual void event_mouse_up();
    virtual void event_mouse_move();
    virtual void event_mouse_wheel_up();
    virtual void event_mouse_wheel_down();
    virtual void event_key_down(wchar_t key);

    virtual void force_render();
    // void set_other_window(CamWindow * other);

    bool get_started() const { return started; }
    void set_started(bool _started) { started = _started; }

	bool get_audio() const { return audio; }
	void set_audio(bool _audio) { audio = _audio; }

	bool get_visual() const { return visual; }
	void set_visual(bool _visual) { visual = _visual; }

private:
    void draw_map();
    void draw_states();

    // object state icons
    GUIImage * goto_icon;
    GUIImage * return_icon;

    // fonts
    irr::gui::IGUIFont * font;
    irr::gui::IGUIStaticText * small_text;
    irr::gui::IGUIStaticText * large_text;

    // UAV map
    UAVMap map;

    // UAV chatbox
    UAVChatbox chat;

    // UAV Consoles
    irr::core::list<UAVConsole *> consoles;
    irr::core::list<UAVConsole *>::Iterator cons_it;

    // data
    std::list<WaypointObject *> * wps;
    std::list<WaypointObject *>::iterator wp_it;
    std::list<SimObject *> * bases;
    std::list<SimObject *>::iterator base_it;
    std::list<UAVObject *> * uavs;
    std::list<UAVObject *>::iterator uav_it;
    std::list<UFOObject *> * ufos;
    std::list<UFOObject *>::iterator ufo_it;
    std::list<Event *> * events;
    std::list<Event *>::iterator e_it;

    // event locks
    bool moving_map;
    bool started;
	bool audio;
	bool visual;

    // other window
    CamWindow * cam_window;
};

#endif