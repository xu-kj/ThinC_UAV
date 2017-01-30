#include "globals.h"
#include "window.h"

#include <iostream>

using std::cout;
using std::endl;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/* Resolution Key */
// ResCode(int) : Resolution(pixels) : WindowResolution_e
// 1 : 1024x768  : R1024X768
// 2 : 1152x864  : R1152X864
// 3 : 2560x1600 : R2560X1600

//irr::s32 getResolutionWidth(WindowResolution_e resolution) {
//    switch(resolution) {
//        case 1:     return 1024;
//        case 2:     return 1152;
//        case 3:     return 2560;
//        default:    return 1024;
//    }
//}
//irr::s32 getResolutionHeight(WindowResolution_e resolution) {
//    switch(resolution) {
//        case 1:     return 768;
//        case 2:     return 864;
//        case 3:     return 1600;
//        default:    return 768;
//    }
//}

UAVWindow::UAVWindow(
    const irr::core::stringw &title_,
    bool load_, 
    bool fullScreen_,
    E_DRIVER_TYPE driver_type_,
    std::pair<int, int> resolution_,
    irr::core::dimension2di startPosition_)
    : m_device(nullptr),
    event_recv(nullptr),
    title(title_),
    fullScreen(fullScreen_),
    toggleFullScreen(false),
    closed(false),
    driver_type(driver_type_),
    resolution(resolution_),
    startPosition(startPosition_),
    start_end(false),
    ended(false)
{
    if(load_ && !load())
        throw Error("Irrlicht window failed to initialize correctly.");
}

UAVWindow::~UAVWindow() {
    m_device->closeDevice();
    m_device->drop();

    if(event_recv != nullptr)
        delete event_recv;
}

bool UAVWindow::load() {
    // close the device if it is already open
    // (only need to do this when restarting it)
    if(m_device != nullptr) {
        m_device->closeDevice();
        m_device->drop();
        cout << "device dropped" << endl;
    }

    // create an event receiver if one has not already been created
    if(event_recv == nullptr)
        event_recv = new EventReceiver(this);

    // create the video device
    irr::SIrrlichtCreationParameters deviceParameter;
        deviceParameter.DriverType = driver_type;
        deviceParameter.AntiAlias = false;
        deviceParameter.Bits = 32;
        deviceParameter.Fullscreen = fullScreen;
        deviceParameter.Titlebar = fullScreen? false: true;
        // deviceParameter.Titlebar = false;
        deviceParameter.WindowPosition = startPosition;
        deviceParameter.Vsync = false;
        deviceParameter.WindowSize = irr::core::dimension2di(windowWidth(), windowHeight());
        deviceParameter.EventReceiver = event_recv;
    setDevice(irr::createDeviceEx(deviceParameter));

    return device() != nullptr;
}

void UAVWindow::draw() {
    driver()->beginScene();
    driver()->endScene();
}

void UAVWindow::update() {
    // fullscreen/unfullscreen the window
    if(toggleFullScreen) 
        toggle_fullscreen();

    // only update if the window's not closed
    else if(!closed) {
        if(!device()->run()) {
            // close the window
            closed = true;
        }
        else {
            // update and draw the window
            updateFPS(title);
            draw();
        }
    }
}

void UAVWindow::updateFPS(const irr::core::stringw &title) {
    int fps = driver()->getFPS();
    if (lastFPS != fps) {
        irr::core::stringw str = title + L" [" + 
            driver()->getName() + L"] FPS:";
        str += fps;
        device()->setWindowCaption(str.c_str());
        lastFPS = fps;
    }
}

bool UAVWindow::getRestarted() {
    // wait a frame to take effect (until after the device fails)
    static bool wait = true;
    if(restarted) {
        if(wait) {
            wait = false;
            return false;
        }
        else {
            wait = true;
            restarted = false;
            return true;
        }
    }
    return false;
}

void UAVWindow::event_mouse_down() {}

void UAVWindow::event_mouse_up() {}

void UAVWindow::event_mouse_wheel_up() {}

void UAVWindow::event_mouse_wheel_down() {}

void UAVWindow::event_mouse_move() {
    // keeps track of the relative cursor movements
    position2di cursor = device()->getCursorControl()->getPosition();
    rel_cursor.X = cursor.X - last_cursor.X;
    rel_cursor.Y = cursor.Y - last_cursor.Y;
    last_cursor = cursor;
}

void UAVWindow::event_key_down(wchar_t key) {
    // pause, if avaiable
    if(ALLOW_PAUSE && event_recv->IsKeyDown(irr::KEY_ESCAPE))
        simulation_paused = !simulation_paused;

    // switch to "ended" if necessary
    if(start_end)
        ended = true;

    if(ALLOW_SPEED_CHANGES) {
        if(key == 61) {
            // plus
            SPEED_FACTOR += 50;
            cout << "Speed: " << SPEED_FACTOR << endl;
        }
        if(key == 45 && SPEED_FACTOR >= 50) {
            // minus
            SPEED_FACTOR -= 50;
            cout << "Speed: " << SPEED_FACTOR << endl;
        }
    }
}

void UAVWindow::load_images() {}

void UAVWindow::toggle_fullscreen() {
    toggleFullScreen = false;
    fullScreen = !fullScreen;
    if(!load())
        throw Error("ERROR: Unable to restart device.\n");
    device()->run();
    restarted = true;
}