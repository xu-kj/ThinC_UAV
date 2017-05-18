#include "cam_window.h"
#include "nav_window.h"
#include "globals.h"
#include "sara_signals.h"

#include <math.h>
#include <iostream>
#include <utility>

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

irr::s32 CAM_SIZE_X = 100; // Previously 341
irr::s32 CAM_SIZE_Y = 50; // Previously 256
irr::s32 CAM_INTERVAL = 0; // Previously 10

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
												_cam_height * 2 + _cam_interval * 1), 
                                 position),
                     wps(wps_), bases(bases_), uavs(uavs_), 
					 cam_width(_cam_width), cam_height(_cam_height), cam_interval(_cam_interval),
                     render(0), need_render(true), city(0), started(false), paused(false), current_uav(0)
{
    // CAM_SIZE_X = cam_width;
    // CAM_SIZE_Y = cam_height;
    // CAM_INTERVAL = cam_interval;

	// Hard coded to fit new monitor
	CAM_SIZE_X = 380;
	CAM_SIZE_Y = 330;
	CAM_INTERVAL = 70;


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

    cams.resize(6);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            cams[i * 3 + j] = 
                new UAVCamera(position2di((CAM_SIZE_X + CAM_INTERVAL) * j, 
                                          (CAM_SIZE_Y + CAM_INTERVAL) * i), 
                              std::make_pair(CAM_SIZE_X, CAM_SIZE_Y),
                              this);
        }
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

	// if paused draw the paused screen
	if (started && get_paused()) {
		paused_overlay_uav1->draw();
		paused_overlay_uav2->draw();
		paused_overlay_uav3->draw();
		paused_overlay_uav4->draw();
		paused_overlay_uav5->draw();
		paused_overlay_uav6->draw();
		switch(current_uav) {
			case 1:
				uav_one_scores[uav_one_score]->draw();
				break;
			case 2:
				uav_one_scores[uav_one_score]->draw();
				uav_two_scores[uav_two_score]->draw();
				break;
			case 3:
				uav_one_scores[uav_one_score]->draw();
				uav_two_scores[uav_two_score]->draw();
				uav_three_scores[uav_three_score]->draw();
				break;
			case 4:
				uav_one_scores[uav_one_score]->draw();
				uav_two_scores[uav_two_score]->draw();
				uav_three_scores[uav_three_score]->draw();
				uav_four_scores[uav_four_score]->draw();
				break;
			case 5:
				uav_one_scores[uav_one_score]->draw();
				uav_two_scores[uav_two_score]->draw();
				uav_three_scores[uav_three_score]->draw();
				uav_four_scores[uav_four_score]->draw();
				uav_five_scores[uav_five_score]->draw();
				break;
			case 6:
				uav_one_scores[uav_one_score]->draw();
				uav_two_scores[uav_two_score]->draw();
				uav_three_scores[uav_three_score]->draw();
				uav_four_scores[uav_four_score]->draw();
				uav_five_scores[uav_five_score]->draw();	
				uav_six_scores[uav_six_score]->draw();
				break;
			default:
				break;
		}
	}

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

        // load images and set i/d
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
    start_overlay = new GUIImage(rect<s32>(0,0,960,720), device(), guiElmRoot);
    start_overlay->setTexture(driver()->getTexture("../media/icons_temp/start_screen.png"));
    start_overlay->setPosition(position2d<s32>(140,20));

	paused_overlay_uav1 = new GUIImage(rect<s32>(0,0,385,330), device(), guiElmRoot);
    paused_overlay_uav1->setTexture(driver()->getTexture("../media/paused_interface/enter_uav1.png"));
    paused_overlay_uav1->setPosition(position2d<s32>(0,0));

	paused_overlay_uav2 = new GUIImage(rect<s32>(0,0,385,330), device(), guiElmRoot);
    paused_overlay_uav2->setTexture(driver()->getTexture("../media/paused_interface/enter_uav2.png"));
    paused_overlay_uav2->setPosition(position2d<s32>(450,0));
	
	paused_overlay_uav3 = new GUIImage(rect<s32>(0,0,385,330), device(), guiElmRoot);
    paused_overlay_uav3->setTexture(driver()->getTexture("../media/paused_interface/enter_uav3.png"));
    paused_overlay_uav3->setPosition(position2d<s32>(900,0));

	paused_overlay_uav4 = new GUIImage(rect<s32>(0,0,385,330), device(), guiElmRoot);
    paused_overlay_uav4->setTexture(driver()->getTexture("../media/paused_interface/enter_uav4.png"));
    paused_overlay_uav4->setPosition(position2d<s32>(0,400));

	paused_overlay_uav5 = new GUIImage(rect<s32>(0,0,385,330), device(), guiElmRoot);
    paused_overlay_uav5->setTexture(driver()->getTexture("../media/paused_interface/enter_uav5.png"));
    paused_overlay_uav5->setPosition(position2d<s32>(450,400));

	paused_overlay_uav6 = new GUIImage(rect<s32>(0,0,385,330), device(), guiElmRoot);
    paused_overlay_uav6->setTexture(driver()->getTexture("../media/paused_interface/enter_uav6.png"));
    paused_overlay_uav6->setPosition(position2d<s32>(900,400));

	// Load images for scores
	std::vector<std::string> score_files;
	score_files.push_back("../media/paused_interface/scores/zero.png");
	score_files.push_back("../media/paused_interface/scores/one.png");
	score_files.push_back("../media/paused_interface/scores/two.png");
	score_files.push_back("../media/paused_interface/scores/three.png");
	score_files.push_back("../media/paused_interface/scores/four.png");
	score_files.push_back("../media/paused_interface/scores/five.png");
	score_files.push_back("../media/paused_interface/scores/six.png");
	score_files.push_back("../media/paused_interface/scores/seven.png");
	score_files.push_back("../media/paused_interface/scores/eight.png");
	score_files.push_back("../media/paused_interface/scores/nine.png");

	for (int i = 0; i < (int) score_files.size(); ++i) {
		GUIImage *temp;
		temp = new GUIImage(rect<s32>(0,0,385,330), device(), guiElmRoot);
		temp->setTexture(driver()->getTexture(score_files[i].c_str()));
		temp->setPosition(position2d<s32>(0, 0));
		uav_one_scores.push_back(temp);

		temp = new GUIImage(rect<s32>(0,0,385,330), device(), guiElmRoot);
		temp->setTexture(driver()->getTexture(score_files[i].c_str()));
		temp->setPosition(position2d<s32>(450, 0));
		uav_two_scores.push_back(temp);

		temp = new GUIImage(rect<s32>(0,0,385,330), device(), guiElmRoot);
		temp->setTexture(driver()->getTexture(score_files[i].c_str()));
		temp->setPosition(position2d<s32>(900, 0));
		uav_three_scores.push_back(temp);

		temp = new GUIImage(rect<s32>(0,0,385,330), device(), guiElmRoot);
		temp->setTexture(driver()->getTexture(score_files[i].c_str()));
		temp->setPosition(position2d<s32>(0, 400));
		uav_four_scores.push_back(temp);
			
		temp = new GUIImage(rect<s32>(0,0,385,330), device(), guiElmRoot);
		temp->setTexture(driver()->getTexture(score_files[i].c_str()));
		temp->setPosition(position2d<s32>(450, 400));
		uav_five_scores.push_back(temp);

		temp = new GUIImage(rect<s32>(0,0,385,330), device(), guiElmRoot);
		temp->setTexture(driver()->getTexture(score_files[i].c_str()));
		temp->setPosition(position2d<s32>(900, 400));
		uav_six_scores.push_back(temp);
	}

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
    for (UAVCamera *x : cams) {

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
		
		// Check indicators of cameras
		x->check_last_indicator_click();
	}
}

void CamWindow::event_mouse_down() {
    for (UAVCamera *x : cams)
        if (x->button_click(
            device()->getCursorControl()->getPosition())) {
                force_render();
        }
}

#include <iostream>

void CamWindow::event_key_down(wchar_t key) {
    UAVWindow::event_key_down(key);
    if (!started && event_recv->IsKeyDown(irr::KEY_RETURN)
        && !event_recv->IsKeyDown(irr::KEY_SHIFT))
        started = true;

    if (event_recv->IsKeyDown(irr::KEY_SHIFT) 
        && event_recv->IsKeyDown(irr::KEY_RETURN)) {
            // toggleFullScreen = true;
    }

	if (get_paused() && current_uav != 6 && event_recv->IsKeyDown(irr::KEY_KEY_1)) {
		current_uav += 1;
		set_score(current_uav, 1);
	} else if (get_paused() && current_uav != 6 && event_recv->IsKeyDown(irr::KEY_KEY_2)) {
		current_uav += 1;
		set_score(current_uav, 2);
	} else if (get_paused() && current_uav != 6 && event_recv->IsKeyDown(irr::KEY_KEY_3)) {
		current_uav += 1;
		set_score(current_uav, 3);
		uav_one_scores[0]->draw();
	} else if (get_paused() && current_uav != 6 && event_recv->IsKeyDown(irr::KEY_KEY_4)) {
		current_uav += 1;
		set_score(current_uav, 4);
	} else if (get_paused() && current_uav != 6 && event_recv->IsKeyDown(irr::KEY_KEY_5)) {
		current_uav += 1;
		set_score(current_uav, 5);
	} else if (get_paused() && current_uav != 6 && event_recv->IsKeyDown(irr::KEY_KEY_6)) {
		current_uav += 1;
		set_score(current_uav, 6);
	} else if (get_paused() && current_uav != 6 && event_recv->IsKeyDown(irr::KEY_KEY_7)) {
		current_uav += 1;
		set_score(current_uav, 7);
	} else if (get_paused() && current_uav != 6 && event_recv->IsKeyDown(irr::KEY_KEY_8)) {
		current_uav += 1;
		set_score(current_uav, 8);
	} else if (get_paused() && current_uav != 6 && event_recv->IsKeyDown(irr::KEY_KEY_9)) {
		current_uav += 1;
		set_score(current_uav, 9);
	} else if (get_paused() && event_recv->IsKeyDown(irr::KEY_KEY_0)) {
		current_uav += 1;
		set_score(current_uav, 0);
	} else if (get_paused() && current_uav == 6 && event_recv->IsKeyDown(irr::KEY_RETURN)) {
		current_uav = 0;	
		record_scores();
		reset_paused();
	} else if (get_paused() && current_uav > 0 && event_recv->IsKeyDown(irr::KEY_BACK)) {
		current_uav -= 1;
	}


	/*
	if (started) {
		if (cams[0] != nullptr && event_recv->IsKeyDown(irr::KEY_KEY_1)) {
			cams[0]->cam_message(11);
		}
		draw_score()
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
		draw_score()
		}
		else if (cams[8] != nullptr && event_recv->IsKeyDown(irr::KEY_KEY_9)) {
			cams[8]->cam_message(11);
		}
	}
	*/
}