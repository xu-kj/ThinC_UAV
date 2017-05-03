#ifndef WINDOW_H
#define WINDOW_H

#include "event_receiver.h"
#include "exceptions.h"
#include "image.h"

#include <irrlicht.h>
#include <string>
#include <vector>
#include <utility>

/* Simple window class to support an Irrlicht device window. Allows the user to
   switch the window to fullscreen. */
class UAVWindow {
private:
    void toggle_fullscreen();

    irr::IrrlichtDevice *m_device;
    irr::core::stringw title;
    irr::core::dimension2di startPosition;
    bool fullScreen;

    int lastFPS;
    bool closed;

protected:
    EventReceiver *event_recv;
    GUIImage *start_overlay;
    GUIImage *end_overlay;

	// Overlay for paused screen
	GUIImage *paused_overlay;
	GUIImage *continue_overlay;
	std::vector<GUIImage*> uav_one_scores;
	std::vector<GUIImage*> uav_two_scores;
	std::vector<GUIImage*> uav_three_scores;
	std::vector<GUIImage*> uav_four_scores;
	std::vector<GUIImage*> uav_five_scores;
	std::vector<GUIImage*> uav_six_scores;
	
	int uav_one_score;
	int uav_two_score;
	int uav_three_score;
	int uav_four_score;
	int uav_five_score;
	int uav_six_score;

    bool toggleFullScreen;

    /* Start Parameters */
    irr::video::E_DRIVER_TYPE driver_type;
    std::pair<int, int> resolution;

    /* Event Trackers (protected due to lazy :|) */
    irr::core::position2di last_cursor;
    irr::core::position2di rel_cursor;

    /* to find out if the window was restarted - set to false if true */
    bool restarted;

    /* time to quit? */
    bool start_end;
    bool ended;

public:
    UAVWindow(
        const irr::core::stringw &title_ = "Irrlicht Window",
        bool load_ = true, 
        bool fullScreen_ = false,
        irr::video::E_DRIVER_TYPE driver_type_ = irr::video::EDT_DIRECT3D9,
        std::pair<int, int> resolution_ = std::make_pair(800, 600),
        irr::core::dimension2di startPosition_ = irr::core::dimension2di(0,0));
    ~UAVWindow();

    virtual bool load();
    virtual void draw();
    virtual void update();
    virtual void load_images();
    virtual void force_render() {}

    virtual void updateFPS(const irr::core::stringw &title);

    /* Setters */
    void setDevice(irr::IrrlichtDevice * device) {
        m_device = device;
    }

    /* Getters */
    irr::IrrlichtDevice * device() const { 
        return m_device; 
    }
    irr::video::IVideoDriver * driver() const { 
        return m_device->getVideoDriver(); 
    }
    irr::scene::ISceneManager * smgr() const { 
        return m_device->getSceneManager(); 
    }
    irr::gui::IGUIEnvironment * guienv() const { 
        return m_device->getGUIEnvironment(); 
    }
    EventReceiver * receiver() const { 
        return event_recv; 
    }
    
    /* Window state getters */
    bool windowClosed() const { 
        return closed; 
    }
	irr::s32 windowWidth() const { 
        return (irr::s32) resolution.first; 
    }
    irr::s32 windowHeight() const { 
        return (irr::s32) resolution.second; 
    }
    bool getRestarted();

    /* Event Handlers */
    virtual void event_mouse_down();
    virtual void event_mouse_up();
    virtual void event_mouse_move();
    virtual void event_mouse_wheel_up();
    virtual void event_mouse_wheel_down();
    virtual void event_key_down(wchar_t key);

    /* End of scenario */
    void set_ended() {
        start_end = true;
    }
    bool get_ended() const {
        return ended;
    }
};

#endif /* WINDOW_H */