#include "cam_window.h"
#include "nav_window.h"
#include "globals.h"
#include "sara_signals.h"

#include <math.h>
#include <iostream>

using std::cout;
using std::endl;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma warning (disable:4154)

irr::s32 CITY_SIZE;
irr::s32 CITY_SIZE_X;
irr::s32 CITY_SIZE_Y;
irr::s32 CITY_SECTOR_SIZE;
irr::f32 CITY_BUILDING_SIZE;
irr::f32 CITY_BUILDING_HEIGHT;

irr::f32 WORLD_START_LON = 0;
irr::f32 WORLD_START_LAT = 0;
irr::f32 WORLD_END_LON   = 10;
irr::f32 WORLD_END_LAT   = 10;

irr::s32 CAM_SIZE_X = 341;
irr::s32 CAM_SIZE_Y = 256;
irr::s32 CAM_INTERVAL = 10;

bool USE_RTT = true;

int pow(int x, int y) {
    int result = 1;
    while (y--)
        result *= x;
    return result;
}

int nextPwr2(int num) {
    double log2num = log(num) / log(2);
    int i = int(floor(log2num)) + (log2num - int(log2num) == 0? 0: 1);
    return pow(2, i);
}

CamWindow::CamWindow(std::list<WaypointObject *> * wps_,
                     std::list<SimObject *> * bases_,
                     std::list<UAVObject *> * uavs_,
                     int _cam_width,
                     int _cam_height,
                     int _cam_interval,
                     irr::core::dimension2di position,
                     E_DRIVER_TYPE driver)
                     : UAVWindow("UAV Flight Sim - Camera Window", 
                                 false, 
                                 false, 
                                 driver, 
                                 std::make_pair(_cam_width * 3 + _cam_interval * 2, 
												_cam_height * 3 + _cam_interval * 2), 
                                 position),
                     wps(wps_), bases(bases_), uavs(uavs_), 
					 cam_width(_cam_width), cam_height(_cam_height), cam_interval(_cam_interval),
                     render(0), need_render(true), city(0), started(false)
{
    CAM_SIZE_X = cam_width;
    CAM_SIZE_Y = cam_height;
    CAM_INTERVAL = cam_interval;

    if (!load())
        throw Error("Cam window failed to initialize correctly.");
}

CamWindow::~CamWindow() {
    for (UAVCamera *x : cams)
        delete x;
}

bool CamWindow::load() {
    if (!UAVWindow::load())
        return false;

	cams.resize(9);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
			// std::cout << i << " " << j << "\n";
            cams[i * 3 + j] = 
                new UAVCamera(position2di((CAM_SIZE_X + CAM_INTERVAL) * j, 
                                          (CAM_SIZE_Y + CAM_INTERVAL) * i), 
                              std::make_pair(CAM_SIZE_X, CAM_SIZE_Y),
                              this);
        }

    need_render = true;
    load_images();

    // skybox
    ISceneNode * skybox = smgr()->addSkyBoxSceneNode(
        driver()->getTexture("../media/sb/irrlicht2_up.png"),
        driver()->getTexture("../media/sb/irrlicht2_dn.png"),
        driver()->getTexture("../media/sb/irrlicht2_lf.png"),
        driver()->getTexture("../media/sb/irrlicht2_rt.png"),
        driver()->getTexture("../media/sb/irrlicht2_ft.png"),
        driver()->getTexture("../media/sb/irrlicht2_bk.png"));

    // set city parameters (squares)
    s32 city_size       = CITY_SIZE;
    s32 sector_size     = CITY_SECTOR_SIZE;
    f32 building_size   = CITY_BUILDING_SIZE;
    f32 building_height = CITY_BUILDING_HEIGHT;

    // fog
    SColor fog_color(255,194,159,112);
    driver()->setFog(fog_color,
        true,
        building_size*sector_size*1.f,
        building_size*sector_size*2.f,
        0.05f);

    smgr()->addCameraSceneNodeFPS();

    // ambient light
    smgr()->setAmbientLight(video::SColor(0,255,255,255));

    // city scene;
    if(city)    
        delete city;
    city = new CityScene(
        device(),
        dimension2di(CITY_SIZE_X, CITY_SIZE_Y),
        dimension2di(sector_size, sector_size),
        vector3df(building_size, building_height, building_size));
    city->show_bounds(false, false, false);

    return true;
}

void CamWindow::draw() {
    driver()->beginScene();

    render_to_texture();
    if(USE_RTT && driver()->queryFeature(EVDF_RENDER_TO_TARGET)) {
        render_image->draw();
        for (UAVCamera *x : cams)
            x->draw_view(device(), city);
        render_image_overlay->draw();

        for (UAVCamera *x : cams) {
            bool s = false;
            if (!s && USE_FULL_CAM_SHADING)
                x->draw_shade(device());
        }
    }
    else {
        // just in case there's no RTT
        for (UAVCamera *x : cams)
            x->draw_background(device());
        for (UAVCamera *x : cams)
            x->draw_view(device(), city);
        for (UAVCamera *x : cams)
            x->draw_overlay(device());
    }

    // draw the start instructions
    if (!started)
        start_overlay->draw();

    driver()->endScene();
}

void CamWindow::load_images() {
    cams[0]->load_images(device());

    uav_it = uavs->begin();
    for (unsigned i = 0; i < cams.size(); ++i) {
        // if there's UAVs left to assign, assign the next one
        if(uav_it != uavs->end()) {
            cams[i]->set_uav(*uav_it, device());
            (*uav_it)->set_camera(i, cams[i]);
            uav_it++;
        }

        // load images and set id
        cams[i]->load_buttons();
        cams[i]->set_id(device(), i + 1);
    }

    render_image = new GUIImage(
        rect<s32>(0,0,nextPwr2(windowWidth()),nextPwr2(windowHeight())),
        device(),
        guienv()->getRootGUIElement());
    render_image_overlay = new GUIImage(
        rect<s32>(0,0,nextPwr2(windowWidth()),nextPwr2(windowHeight())),
        device(),
        guienv()->getRootGUIElement());

    IGUIElement * guiElmRoot = guienv()->getRootGUIElement();
    start_overlay = new GUIImage(rect<s32>(0,0,512,256), device(), guiElmRoot);
    start_overlay->setTexture(driver()->getTexture("start_overlay.png"));
    start_overlay->setPosition(position2d<s32>(325,150));

    if (USE_RTT && driver()->queryFeature(EVDF_RENDER_TO_TARGET)) {
        stringc name = "RT1";
        render = driver()->addRenderTargetTexture(
            dimension2d<s32>(nextPwr2(windowWidth()),nextPwr2(windowHeight())), name.c_str());
        render_image->setTexture(render);

        name = "RT2";
        render_overlay = driver()->addRenderTargetTexture(
            dimension2d<s32>(nextPwr2(windowWidth()),nextPwr2(windowHeight())), name.c_str());
        render_image_overlay->setTexture(render_overlay);
    }
    else {
        cout << "RTT not supported" << endl;
    }
}

void CamWindow::render_to_texture() {
    for (UAVCamera *x : cams) 
        if (x->need_render()) {
            need_render = true;
            break;
        }

        if (need_render) {
            need_render = false;
            if (driver()->queryFeature(EVDF_RENDER_TO_TARGET)) { 
                // set the render target and draw the console, then set it back
                driver()->setRenderTarget(render,true,true,SColor(0,0,0,255));
                for (UAVCamera *x : cams)
                    x->draw_background(device());

                driver()->setRenderTarget(render_overlay,true,true,SColor(0,0,0,255));
                for (UAVCamera *x : cams)
                    x->draw_overlay(device());
                driver()->setRenderTarget(0,true,true,0);

                // set the viewport back to the full size
                rect<s32> viewport(0,0,windowWidth(),windowHeight());
                driver()->setViewPort(viewport);
            }
        }
}

void CamWindow::event_mouse_down() {
    for (UAVCamera *x : cams)
        if (x->button_click(
            device()->getCursorControl()->getPosition())) {
                force_render();
        }
}

void CamWindow::event_key_down(wchar_t key) {
    UAVWindow::event_key_down(key);
    if (!started && event_recv->IsKeyDown(irr::KEY_RETURN)
        && !event_recv->IsKeyDown(irr::KEY_SHIFT))
        started = true;

    if (event_recv->IsKeyDown(irr::KEY_SHIFT) 
        && event_recv->IsKeyDown(irr::KEY_RETURN)) {
            // toggleFullScreen = true;
    }

	if (started) {
			if (cams[0] != nullptr && event_recv->IsKeyDown(irr::KEY_KEY_1)) {
				cams[0]->cam_message(11);
			}
			else if (cams[1] != nullptr && event_recv->IsKeyDown(irr::KEY_KEY_2)) {
				cams[1]->cam_message(11);
			}
			else if (cams[2] != nullptr && event_recv->IsKeyDown(irr::KEY_KEY_3)) {
				cams[2]->cam_message(11);
			}
			else if (cams[3] != nullptr && event_recv->IsKeyDown(irr::KEY_KEY_4)) {
				cams[3]->cam_message(11);
			}
			else if (cams[4] != nullptr && event_recv->IsKeyDown(irr::KEY_KEY_5)) {
				cams[4]->cam_message(11);
			}
			else if (cams[5] != nullptr && event_recv->IsKeyDown(irr::KEY_KEY_6)) {
				cams[5]->cam_message(11);
			}
			else if (cams[6] != nullptr && event_recv->IsKeyDown(irr::KEY_KEY_7)) {
				cams[6]->cam_message(11);
			}
			else if (cams[7] != nullptr && event_recv->IsKeyDown(irr::KEY_KEY_8)) {
				cams[7]->cam_message(11);
			}
			else if (cams[8] != nullptr && event_recv->IsKeyDown(irr::KEY_KEY_9)) {
				cams[8]->cam_message(11);
			}
	}
}
