#include "cam_window.h"
#include "nav_window.h"
#include "cityscene_node.h"
#include "config.h"
#include "controller.h"
#include "network.h"
#include "output.h"
#include "random.h"
#include "target_struct.h"
#include "uav_tactors.h"

#include <irrlicht.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <conio.h>

#pragma warning (disable:4996)

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::basic_ifstream;

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// =========================================================================
// GENERAL SETUP
// =========================================================================

const char * const CONFIG_FILE = "..\\config.cfg";

//WindowResolution_e NAV_RESOLUTION = R1024X768;
//WindowResolution_e CAM_RESOLUTION = R1024X768;
irr::video::E_DRIVER_TYPE NAV_DRIVER = EDT_OPENGL;
irr::video::E_DRIVER_TYPE CAM_DRIVER = EDT_DIRECT3D9;
irr::core::dimension2di NAV_POS(0,0);
irr::core::dimension2di CAM_POS(0,0);

static int CAM_WIDTH;
static int CAM_HEIGHT;
static int CAM_INTERVAL;
static int NAV_WIDTH;
static int NAV_HEIGHT;

irr::core::stringc SCENARIO_FILE;
irr::core::stringc OUTPUT_FILE;

bool TIMESTAMP_OUTPUT    = true;
bool RUN_TACTOR_TEST     = false;
bool ALLOW_SPEED_CHANGES = false;
irr::s32 BUTTON_MS       = 300;

bool ALLOW_PAUSE         = true;
bool simulation_paused   = false;
bool was_paused          = false;

irr::video::SColor COLOR1(255,  79, 129, 189);  // blue
irr::video::SColor COLOR2(255, 209,  99,   9);  // orange
irr::video::SColor COLOR3(255, 112, 139,  57);  // vomit
irr::video::SColor COLOR4(255, 192,  80,  77);  // pink
irr::video::SColor COLOR5(255, 128, 100, 162);  // purple
irr::video::SColor COLOR6(255,  45, 119, 119);  // teal
irr::video::SColor COLOR7(255, 207, 172,   0);  // yellow
irr::video::SColor COLOR8(255, 152,  72,   7);  // brown
irr::video::SColor COLOR9(255, 158,  10,  10);  // red
irr::video::SColor WAYPOINT_UNSURE    = COLOR_GRAY;
irr::video::SColor WAYPOINT_CLEAR     = COLOR_RED;
irr::video::SColor WAYPOINT_CONFIRMED = COLOR_GREEN;

void * TARGETS = 0; // pointer to array of scenes to load when the main driver starts
irr::f32 UAV_HEIGHT = 60.f;

irr::u32 MINUTES      = 0;
irr::u32 SECONDS      = 0;
irr::u32 MILLISECONDS = 0;
irr::u32 TICKS        = 0;

bool OTHER_SIM_STARTED = false;
bool OTHER_SIM_ENDED   = false;
bool USE_NETWORK       = false;

// =========================================================================
// CONSTRUCTOR
// =========================================================================

UAVController::UAVController()
    : win1(0), win2(0)
{
    load_config();
    Output::Instance().StartLog();

    // intialize the hardware and simulation model
    //std::vector<std::string> ports;
    //for(int i = 0; i < 3; i++) {
    //    int value;
    //    if(i == 0) value = TACTOR_COM_PORT_1;
    //    if(i == 1) value = TACTOR_COM_PORT_2;
    //    if(i == 2) value = TACTOR_COM_PORT_3;

    //    stringstream ss;
    //    ss << "COM" << value;
    //    ports.push_back("");
    //    ports[i] = ss.str();
    //}

    //tactors = new Sim::Tactors3x3(
    //    ports[0], ports[1], ports[2],
    //    !USE_TACTOR_CUES);

    //try {
    //    tactors->connect();

    //    tactors->setFreqs(
    //        TACTOR_FREQUENCY,
    //        TACTOR_FREQUENCY);

    //    if(RUN_TACTOR_TEST)
    //        tactors->test();
    //} catch(std::exception e) {
    //    MessageBox(0, L"Warning: tactile cues are enabled, but the tactor\nhardware could not be initialized.", L"Tactor Error :'(", 0);
    //}

    //Network::subscribeToNetworkUpdates(fastdelegate::MakeDelegate(this, &UAVController::network_update));
    //start_network();
    // wait for connection

    set_world_params();
    load_world();

    try {
        // create the windows
        win1 = new NavWindow(&wps, &bases, &uavs, &ufos, &events,
            NAV_WIDTH,
            NAV_HEIGHT,
            NAV_POS,
            NAV_DRIVER);
        win2 = new CamWindow(&wps, &bases, &uavs,
            CAM_WIDTH, 
            CAM_HEIGHT, 
            CAM_INTERVAL,
            CAM_POS,
            CAM_DRIVER);

        if(!win1 || !win2)
            throw Error("Failed to create windows");

    } catch(Error e) {
        // failed to create a window
        if(win1)    delete win1;
        if(win2)    delete win2;

        Output::Instance().EndLogError(e.msg);
        MessageBox(0, L"An error occured and the program must exit.", L"Error", 0);
        exit(1);
    } catch(...) {
        // something bad happened
        if(win1)    delete win1;
        if(win2)    delete win2;

        Output::Instance().EndLogError("Unknown error while loading the windows");
        MessageBox(NULL,L"Unknown error while loading the windows",L"Error",MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
        exit(1);
    }
}

// =========================================================================
// MAIN LOOP
// =========================================================================

void UAVController::run() {
    int run_timer = 0;

    try {
        bool running = true;
        bool started = false;

        // keep the model paused until the user starts
        while(!started && running && !OTHER_SIM_ENDED) {
            started = ((NavWindow *)win1)->get_started() || ((CamWindow *)win2)->get_started();
            ((NavWindow *)win1)->set_started(started);
            ((CamWindow *)win2)->set_started(started);

            // draw the windows
            if(win1) win1->update();
            if(win2) win2->update();

            if(USE_NETWORK) Network::update();

            // did the window close?
            if((win1 && win1->windowClosed()) ||
                (win2 && win2->windowClosed()))
                running = false;

            // force a render if one of the windows restarted
            if(win1 && win2) {
                if(win1->getRestarted())
                    win2->force_render();
                if(win2->getRestarted())
                    win1->force_render();
            }
        }

        if(USE_NETWORK && !OTHER_SIM_ENDED)
            Network::sendMessageStart();

		Output::Instance().WriteColumnName();
		Output::Instance().RecordEvent(-1, UAV_EVENT::SIMULATION_STARTED, -1, -1, -1);

        // log the scenario beginning
        Output::Instance().WriteTick();
        Output::Instance().Write("Simulation started: ");
        Output::Instance().WriteDateTime();
        Output::Instance().WriteLine("");

        // main loop
        u32 then = win2->device()->getTimer()->getTime();
        while(running && !OTHER_SIM_ENDED) {
            if(simulation_paused) {
                if(!was_paused)
                    was_paused = true;
            }
            else {
                // if we just came off a pause, reset the clock
                if(was_paused) {
                    was_paused = false;
                    then = win2->device()->getTimer()->getTime();
                }

                // get change in time
                const u32 now = win2->device()->getTimer()->getTime();
                TICKS += now - then;
                MILLISECONDS += now - then;
                SECONDS += MILLISECONDS / 1000;
                MILLISECONDS = MILLISECONDS % 1000;
                MINUTES += SECONDS / 60;
                SECONDS = SECONDS % 60;

				// pause the simulation every 6 seconds to let the tester
				// fill out a questionaire
				//run_timer += (now - then);
				//if (run_timer >= 6 * 1000) {
				//	Output::Instance().RecordEvent(-1, UAV_EVENT::SIMULATION_PAUSED, -1, -1, -1);
				//	simulation_paused = true;
				//	run_timer = 0;
				//}

                f32 time = (f32)(now - then) * .001f;
                then = now;
                if(time > 1.f)   time = 1.f;

                if (!USE_NETWORK) {
                    // update the model
                    int done = 0;

                    // change to foreach loop
                    for (uav_it = uavs.begin(); uav_it != uavs.end(); uav_it++) {
                        if ((*uav_it)->atBase())
                            done++;
                        (*uav_it)->update(time);
                    }
                    if (done == 9) running = false;

                    for (ufo_it = ufos.begin(); ufo_it != ufos.end(); ufo_it++)
                        (*ufo_it)->update(time);
                    for (e_it = events.begin(); e_it != events.end(); e_it++) {
                        if((*e_it)->activate()) {
                            broadcastEvent(*e_it);
                            ((NavWindow *)win1)->triggerEvent(*e_it);
                        }
                    }
                }

                // update the signals added for Sara's version of the sim
                // ((CamWindow *)win2)->update_cams(time);
            }

            if(USE_NETWORK) Network::update();

            // draw the windows
            if(win1) win1->update();
            if(win2) win2->update();

            // did the window close?
            if((win1 && win1->windowClosed()) ||
                (win2 && win2->windowClosed()))
                running = false;

            // force a render if one of the windows restarted
            if(win1 && win2) {
                if(win1->getRestarted())
                    win2->force_render();
                if(win2 && win2->getRestarted())
                    win1->force_render();
            }
        }

		Output::Instance().RecordEvent(-1, UAV_EVENT::SIMULATION_ENDED, -1, -1, -1);

        // log the scenario ending
        Output::Instance().WriteTick();
        Output::Instance().Write("Simulation ended: ");
        Output::Instance().WriteDateTime();
        Output::Instance().NewLine();

        // wait for the user to press a key
        running = true;
        win1->set_ended();
        win2->set_ended();
        while(running && (!win1->get_ended()) && (!win2->get_ended())) {
            // draw the windows
            if(win1) win1->update();
            if(win2) win2->update();

            // did the window close?
            if((win1 && win1->windowClosed()) ||
                (win2 && win2->windowClosed()))
                running = false;
        }

        // time to quit
        if(win1) delete win1;
        if(win2) delete win2;
    }
    catch(Error) {
        // some error happened
        if(win1) delete win1;
        if(win2) delete win2;

        Output::Instance().EndLogError("Unknown error during program execution");
        MessageBox(0, L"Unknown error!", L"Error <:O", 0);
        exit(1);
    }

    get_final_stats();
    Output::Instance().EndLog();

    if(USE_NETWORK) Network::shutdown();
}

// =========================================================================
// GET FINAL ANALYSIS
// =========================================================================

void UAVController::get_final_stats() {
    int missed = 0;
    int unsure = 0;
    int correct = 0;
    int incorrect = 0;
    int total = 0;

    double missedPercent = 0;
    double unsurePercent = 0;
    double correctPercent = 0;
    double incorrectPercent = 0;

    for(uav_it = uavs.begin(); uav_it != uavs.end(); uav_it++) {
        (*uav_it)->writeSummary();
        missed += (*uav_it)->missed;
        unsure += (*uav_it)->unsure;
        correct += (*uav_it)->correct;
        incorrect += (*uav_it)->incorrect;
    }
    total = correct + incorrect + unsure + missed;
    if(total > 0) {
        correctPercent = (double(correct) / double(total)) * 100;
        incorrectPercent = (double(incorrect) / double(total)) * 100;
        unsurePercent = (double(unsure) / double(total)) * 100;
        missedPercent = (double(missed) / double(total)) * 100;
    }

    stringstream strm;
    strm << fixed << setprecision(2);
    strm << endl;
    strm << "Correct:  " << setw(5) << right << correct << "  " << setw(10) << right << correctPercent << "%" << endl;
    strm << "Incorrect:" << setw(5) << incorrect << "  " << setw(10) << right << incorrectPercent << "%" << endl;
    strm << "Unsure:   " << setw(5) << unsure << "  " << setw(10) << right << unsurePercent << "%" << endl;
    strm << "Missed:   " << setw(5) << missed << "  " << setw(10) << right << missedPercent << "%" << endl;
    strm << "Total:    " << setw(5) << total << endl;

    Output::Instance().WriteLine(strm.str().c_str(), OUTPUT_UAV_TABLE);
}

// =========================================================================
// INITIALIZE NETWORK CONNECTION IN NETWORK MODE
// =========================================================================

void UAVController::start_network() {
    bool found = LoadConfigBoolean(USE_NETWORK, "Network", "USE_NETWORK", NET_CONFIG_PATH);

    if(!found) {
        MessageBox(NULL,L"Failed to load config value USE_NETWORK",L"Error",MB_OK | MB_ICONEXCLAMATION | MB_TOPMOST);
        exit(1);
    }

    if(USE_NETWORK) {
        cout << setw(28) << left;
        cout << "Network Status: " << "using network" << endl;
        Network::establish();

        while(!OTHER_SIM_STARTED)
            Network::update();
    } else {
        cout << setw(28) << left;
        cout << "Network Status: " << "not using network" << endl;
    }
}

// =========================================================================
// UPDATE MODEL IN NETWORK
// =========================================================================

void UAVController::network_update(Packet * pkt)
{
    if(pkt->data[0] == SIM_START)
    {
        OTHER_SIM_STARTED = true;
    }
    else if(pkt->data[0] == SIM_END)
    {
        Output::Instance().WriteTick();
        Output::Instance().WriteLine("Simulation terminated");
        OTHER_SIM_ENDED = true;
    }
    else if(pkt->data[0] == SIM_CAM)
    {
        SimCamPacket * cam = (SimCamPacket *)pkt->data;

        vec3d position(cam->x, cam->y, cam->z);
        vec3d target(cam->targetX, cam->targetY, cam->targetZ);
        uav_ids[cam->objectID]->network_set_target(position, target);
    }
    else if(pkt->data[0] == SIM_CAM_BUTTON)
    {
        SimCamButtonPacket * p = (SimCamButtonPacket *)pkt->data;
        static_cast<CamWindow *>(win2)->send_cam_message(p->objectID, p->type);
    }
}

// =========================================================================
// BROADCAST EVENT IN NETWORK MODE
// =========================================================================

void UAVController::broadcastEvent(Event * e) {
    if(!USE_NETWORK || e == nullptr)
        return;

    stringw output;
    output += "SENDING NETWORK EVENT: ";

    switch(e->get_type()) {
    case CAMERA_FAIL:
        output += "camera failure";
        Network::sendMessageFailure(1, e->get_id(), 0, 0, 0);
        break;

    case GPS_FAIL:
        output += "GPS failure";
        Network::sendMessageFailure(2, e->get_id(), 0, 0, 0);
        break;

    case CHAT_REQUEST:
        output += "chat request - \"";
        output += e->get_text();
        output += "\"";
        break;
    }

    Output::Instance().WriteLine(output);
}

// =========================================================================
// WORLD CONFIGURATION
// =========================================================================

void UAVController::set_world_params() {
    WORLD_START_LON = 0;
    WORLD_END_LON   = 10;

    CITY_SIZE_X          = 300;  // in sectors
    CITY_SIZE_Y          = 300;  // in sectors
    CITY_SECTOR_SIZE     = 3;    // in buildings
    CITY_BUILDING_SIZE   = 30;   // in pixels
    CITY_BUILDING_HEIGHT = 60;   // in pixels
}

// =========================================================================
// XML READERS
// =========================================================================

void UAVController::readEventNode(IXMLReader * reader) {
    if(reader == nullptr)
        return;

    stringc node_name;
    stringc text;

    int id;
    int start_min;
    int start_sec;

    bool end = false;
    while(!end && reader->read()) {
        stringw nodeName = reader->getNodeName();
        switch(reader->getNodeType()) {
        case EXN_ELEMENT_END:
            if(stringw(L"event") == nodeName)
                end = true;
            break;

        case EXN_ELEMENT:
            if(stringw(L"string") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                stringc value = reader->getAttributeValue(stringw(L"value").c_str());
                if     (stringw(L"Type") == name) node_name = value;
                else if(stringw(L"Text") == name) text = value;
            }

            else if(stringw(L"int") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                int value = reader->getAttributeValueAsInt(stringw(L"value").c_str());

                if     (stringw(L"StartMin") == name) start_min = value;
                else if(stringw(L"StartSec") == name) start_sec = value;
                else if(stringw(L"Id")       == name) id = value;
            }
            break;
        }
    }

    // create the object
    if(node_name != "") {
        u32 start_time = u32(start_min) * 1000 * 60;
        start_time += u32(start_sec) * 1000;

        Event *e = new Event(node_name, text, start_time, id);
        events.push_back(e);
        cout << "Loaded Event \"" << node_name.c_str() << "\" [" << start_time << "]" << endl;
    }
}

void UAVController::readUFONode(IXMLReader * reader) {
    if(!reader)
        return;

    stringc node_name;
    float pos_x;
    float pos_y;
    float face_x;
    float face_y;
    float speed;
    int start_min;
    int start_sec;
    int end_min;
    int end_sec;

    bool end = false;
    while(!end && reader->read()) {
        stringw nodeName = reader->getNodeName();
        switch(reader->getNodeType()) {
        case EXN_ELEMENT_END:
            if(stringw(L"ufo") == nodeName)
                end = true;
            break;

        case EXN_ELEMENT:
            if(stringw(L"string") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                stringc value = reader->getAttributeValue(stringw(L"value").c_str());
                if(stringw(L"Name") == name) node_name = value;
            }

            else if(stringw(L"float") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                float value = reader->getAttributeValueAsFloat(stringw(L"value").c_str());
                if     (stringw(L"PosX")  == name) pos_x = value;
                else if(stringw(L"PosY")  == name) pos_y = value;
                else if(stringw(L"FaceX") == name) face_x = value;
                else if(stringw(L"FaceY") == name) face_y = value;
                else if(stringw(L"Speed") == name) speed = value;
            }

            else if(stringw(L"int") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                int value = reader->getAttributeValueAsInt(stringw(L"value").c_str());
                if     (stringw(L"StartMin") == name) start_min = value;
                else if(stringw(L"StartSec") == name) start_sec = value;
                else if(stringw(L"EndMin")   == name) end_min = value;
                else if(stringw(L"EndSec")   == name) end_sec = value;
            }
            break;
        }
    }

    // create the object
    if(node_name != "") {
        vec3d position(pos_x, UAV_HEIGHT, pos_y);
        vec3d forward(face_x, 0.f, face_y);
        u32 start_time = u32(start_min) * 1000 * 60;
        start_time += u32(start_sec) * 1000;
        u32 end_time = u32(end_min) * 1000 * 60;
        end_time += u32(end_sec) * 1000;

        UFOObject * ufo = new UFOObject(node_name, position, forward, speed,
            COLOR_GRAY, start_time, end_time);
        ufos.push_back(ufo);

        cout << "Loaded UFO \"" << node_name.c_str() << "\"" << endl;
    }
}

SimObject * UAVController::readBaseNode(IXMLReader * reader) {
    if(!reader) return 0;

    SimObject * base = 0;
    stringc node_name;
    float pos_x;
    float pos_y;
    float pos_z;

    bool end = false;
    while(!end && reader->read()) {
        stringw nodeName = reader->getNodeName();
        switch(reader->getNodeType()) {
        case EXN_ELEMENT_END:
            if(stringw(L"base") == nodeName)
                end = true;
            break;

        case EXN_ELEMENT:
            if(stringw(L"string") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                stringc value = reader->getAttributeValue(stringw(L"value").c_str());
                if(stringw(L"Name") == name) node_name = value;
            }
            else if(stringw(L"float") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                float value = reader->getAttributeValueAsFloat(stringw(L"value").c_str());
                if     (stringw(L"PosX") == name)   pos_x = value;
                else if(stringw(L"PosY") == name)   pos_y = value;
                else if(stringw(L"PosZ") == name)   pos_z = value;
            }
            break;
        }
    }

    // create the object
    if(node_name != "") {
        vec3d position(pos_x, pos_y, pos_z);
        base = new SimObject(node_name, position, COLOR_GRAY, false);
        bases.push_back(base);

        cout << "Loaded base \"" << node_name.c_str() << "\"" << endl;
    }
    return base;
}

void UAVController::readUAVNode(IXMLReader * reader) {
    if(!reader) return;

    stringc node_name;
    float fuel;
    float pos_x;
    float pos_y;
    float pos_z;
    int color;

    std::list<WaypointObject *> wps;
    SimObject * base;

    bool end = false;
    while(!end && reader->read()) {
        stringw nodeName = reader->getNodeName();
        switch(reader->getNodeType()) {
        case EXN_ELEMENT_END:
            if(stringw(L"uav") == nodeName) end = true;
            break;

        case EXN_ELEMENT:
            if(stringw(L"string") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                stringc value = reader->getAttributeValue(stringw(L"value").c_str());
                if(stringw(L"Name") == name) node_name = value;
            }
            else if(stringw(L"float") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                float value = reader->getAttributeValueAsFloat(stringw(L"value").c_str());
                if     (stringw(L"Fuel") == name)   fuel = value;
                else if(stringw(L"PosX") == name)   pos_x = value;
                else if(stringw(L"PosY") == name)   pos_y = value;
                else if(stringw(L"PosZ") == name)   pos_z = value;
            }
            else if(stringw(L"int") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                int value = reader->getAttributeValueAsInt(stringw(L"value").c_str());
                if(stringw(L"Color") == name) color = value;
            }
            else if(stringw(L"waypoint") == nodeName) {
                WaypointObject * wp = readWaypointNode(reader);
                if(wp) wps.push_back(wp);
            }
            else if(stringw(L"base") == nodeName)
                base = readBaseNode(reader);
            break;
        }
    }

    // create the object
    if(node_name != "") {
        cout << "Loaded UAV \"" << node_name.c_str() << "\"" << endl;

        vec3d position(pos_x, pos_y, pos_z);
        UAVObject * uav = new UAVObject(node_name, position, getColorFromId(color), false);
        uavs.push_back(uav);
        uav->setFuel(fuel);
        uav_ids.push_back(uav);

        uav->assign_base(base);
        std::list<WaypointObject *>::iterator it;
        for(it = wps.begin(); it != wps.end(); it++)
            uav->assign_waypoint(*it);

        static int uavId = 0;
        uavId++;

        if(USE_NETWORK) {
            Network::sendMessageCreate(
                1, uavId,
                pos_x, pos_y, pos_z,
                (float)getColorFromId(color).getRed() / 255.f,
                (float)getColorFromId(color).getGreen() / 255.f,
                (float)getColorFromId(color).getBlue() / 255.f,
                (float)getColorFromId(color).getAlpha() / 255.f,
                0);
        }

        static int wpId = 0;
        for(it = wps.begin(); it != wps.end(); it++)
        {
            wpId++;

            if(USE_NETWORK)
            {
                Network::sendMessageCreate(
                    2, wpId,
                    (*it)->getPosition().X, (*it)->getPosition().Y, (*it)->getPosition().Z,
                    (float)getColorFromId(color).getRed() / 255.f,
                    (float)getColorFromId(color).getGreen() / 255.f,
                    (float)getColorFromId(color).getBlue() / 255.f,
                    (float)getColorFromId(color).getAlpha() / 255.f,
                    uavId);
            }
        }

        if(USE_NETWORK) {
            static int baseId = 0;
            const SimObject * base = uav->getBase();
            if(base)
            {
                Network::sendMessageCreate(
                    3, baseId,
                    base->getPosition().X, base->getPosition().Y, base->getPosition().Z,
                    (float)getColorFromId(color).getRed() / 255.f,
                    (float)getColorFromId(color).getGreen() / 255.f,
                    (float)getColorFromId(color).getBlue() / 255.f,
                    (float)getColorFromId(color).getAlpha() / 255.f,
                    uavId);
                baseId++;
            }
        }

    }
}

WaypointObject * UAVController::readWaypointNode(IXMLReader * reader) {
    if(!reader) return 0;

    WaypointObject * wp;
    stringc node_name;
    float pos_x;
    float pos_y;
    float pos_z;
    bool has_feature = true;
	bool indicated = true;

    bool end = false;
    while(!end && reader->read()) {
        stringw nodeName = reader->getNodeName();
        switch(reader->getNodeType()) {
        case EXN_ELEMENT_END:
            if(stringw(L"waypoint") == nodeName) end = true;
            break;

        case EXN_ELEMENT:
            if(stringw(L"string") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                stringc value = reader->getAttributeValue(stringw(L"value").c_str());
                if(stringw(L"Name") == name) node_name = value;
            }
            else if(stringw(L"float") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                float value = reader->getAttributeValueAsFloat(stringw(L"value").c_str());
                if(stringw(L"PosX") == name)        pos_x = value;
                else if(stringw(L"PosY") == name)   pos_y = value;
                else if(stringw(L"PosZ") == name)   pos_z = value;
            }
            else if(stringw(L"bool") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                stringc value = reader->getAttributeValue(stringw(L"value").c_str());
                if (stringw(L"Feature") == name) {
                    if (stringc("True") == value) 
                        has_feature = true;
                    else if(stringc("False") == value) 
                        has_feature = false;
                }
                else if (stringw(L"Indicated") == name) {
                    if (stringc("True") == value)
                        indicated = true;
                    else if (stringc("False") == value)
                        indicated = false;
                }
            }
            break;
        }
    }

    // create the object
    if(node_name != "") {
        cout << "Loaded Waypoint \"" << node_name.c_str() << "\"" << endl;

        vec3d position(pos_x, pos_y, pos_z);
        wp = new WaypointObject(node_name, position, COLOR_GRAY, false);
        wp->setFeature(has_feature);
        wps.push_back(wp);
    }

    return wp;
}

void UAVController::readSceneNode(IXMLReader * reader) {
    if(!reader)
        return;

    stringc node_name;
    stringc file;
    int cityX;
    int cityY;

    bool end = false;
    while(!end && reader->read()) {
        stringw nodeName = reader->getNodeName();
        switch(reader->getNodeType()) {
        case EXN_ELEMENT_END:
            if(stringw(L"scene") == nodeName)
                end = true;
            break;

        case EXN_ELEMENT:
            if(stringw(L"string") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                stringc value = reader->getAttributeValue(stringw(L"value").c_str());
                if     (stringw(L"Name") == name) node_name = value;
                else if(stringw(L"File") == name) file = value;
            }

            else if(stringw(L"int") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                int value = reader->getAttributeValueAsInt(stringw(L"value").c_str());
                if     (stringw(L"CityX") == name) cityX = value;
                else if(stringw(L"CityY") == name) cityY = value;
            }
            break;
        }
    }

    // create the object
    if(node_name != "" && file != "") {
        std::list<TargetStruct> * targets = (std::list<TargetStruct> *)TARGETS;
        targets->push_back(TargetStruct(file, cityX, cityY));
    }
}

void UAVController::readMapNode(IXMLReader * reader) {
    if(!reader)
        return;

    float pos_x = -1.f;
    float pos_y = -1.f;
    float pos_z = -1.f;

    bool end = false;
    while(!end && reader->read()) {
        stringw nodeName = reader->getNodeName();
        switch(reader->getNodeType()) {
        case EXN_ELEMENT_END:
            if(stringw(L"map") == nodeName)
                end = true;
            break;

        case EXN_ELEMENT:
            if(stringw(L"float") == nodeName) {
                stringc name = reader->getAttributeValue(stringw(L"name").c_str());
                float value = reader->getAttributeValueAsFloat(stringw(L"value").c_str());
                if     (stringw(L"StartPosX") == name)  pos_x = value;
                else if(stringw(L"StartPosY") == name)  pos_y = value;
                else if(stringw(L"StartPosZ") == name)  pos_z = value;
            }
            break;
        }
    }

    // save the start position
    if(pos_x > 0 && pos_y > 0 && pos_z > 0) {
        cout << "Set general start position for UAVs: (";
        cout << pos_x << ", " << pos_y << ", " << pos_z << ")" << endl;
        uav_start_pos = vector3df(pos_x, pos_y, pos_z);
        uav_start_pos_found = true;
    }
}

// =========================================================================
// LOAD SIMULATION WORLD
// =========================================================================

void UAVController::load_world() {
    if(stringc("") != SCENARIO_FILE) {
        IrrlichtDevice * device;
        device = irr::createDevice(EDT_NULL);
        if(!device) {
            MessageBox(0, L"Failed to create NULL device - scenario cannot be loaded", L"Error", 0);
            cout << "Failed to create NULL device - scenario cannot be loaded" << endl;
            exit(1);
        }

        uav_start_pos_found = false;
        IXMLReader * reader = device->getFileSystem()->createXMLReader(SCENARIO_FILE.c_str());
        if(!reader) {
            stringw str = "Failed to open scenario file \"";
            str += SCENARIO_FILE;
            str += "\"";
            MessageBox(0, str.c_str(), L"Error", 0);

            cout << "\nFailed to open scenario file \"" << SCENARIO_FILE.c_str() << "\"" << endl;
            exit(1);
        }
        else {
            cout << "\nOpened scenario file \"" << SCENARIO_FILE.c_str() << "\"" << endl;
            std::list<TargetStruct> * targets = new std::list<TargetStruct>;
            TARGETS = (void *)targets;
            while(reader->read()) {
                stringc node = reader->getNodeName();
                if     (stringw(L"map")    == node) readMapNode(reader);
                else if(stringw(L"scene")  == node) readSceneNode(reader);
                else if(stringw(L"uav")    == node) readUAVNode(reader);
                else if(stringw(L"base")   == node) readBaseNode(reader);
                else if(stringw(L"ufo")    == node) readUFONode(reader);
                else if(stringw(L"event")  == node) readEventNode(reader);
            }
            reader->drop();
            device->drop();
            cout << "Done loading scenario" << endl << endl;

            // if an original position for UAVs was found, relay the data
            // (i.e. the UAV would still start where defined by the <uav> tags, but
            // a line would be drawn from the new position to the current one)
            if(uav_start_pos_found) {
                for(uav_it = uavs.begin(); uav_it != uavs.end(); uav_it++)
                    (*uav_it)->setStartPosition(uav_start_pos);
            }
        }
    }
    else
        generate_random();

    if(uavs.size() < 9) {
        MessageBox(0, L"This scenario contains less than 9 UAVs. This simulation requires 9 UAVs.\nIt will now exit.", L">:|", 0);
        exit(0);
    }
}

// =========================================================================
// RANDOM GENERATOR
// =========================================================================

void UAVController::generate_random() {
    const irr::f32 height = CITY_BUILDING_HEIGHT * .6f;
    const irr::f32 uavh = 6;
    const irr::f32 baseh = 6;

    s32 wp_id = 0;
    for(s32 id = 0; id < s32(1); id++) {
        // UAV
        stringw name = "Mav-";
        name += id + 1;
        f32 lat = f32(rnd(1, 100)) / 10.f;
        f32 lon = f32(rnd(1, 100)) / 10.f;
        SColor color = getColorFromId(id);
        uavs.push_back(new UAVObject(name, vec3d(lat,height,lon), color));

        // waypoints
        s32 n = 6;
        for(s32 i = 0; i < n; i++) {
            s32 dist = 30;

            s32 val = rnd(1, dist);
            if(val % 2 == 0) val *= -1;
            f32 new_lat = f32(val)/10.f + lat;
            while(new_lat < 1 || new_lat > 9) {
                val = rnd(1, dist);
                if(val % 2 == 0) val *= -1;
                new_lat = f32(val)/10.f + lat;
            }
            val = rnd(1, dist);
            if(val % 2 == 0) val *= -1;
            f32 new_lon = f32(val)/10.f + lon;
            while(new_lon < 1 || new_lon > 9) {
                val = rnd(1, dist);
                if(val % 2 == 0) val *= -1;
                new_lon = f32(val)/10.f + lon;
            }

            if(i == n - 1) {
                stringw name = "BASE-";
                name += id + 1;

                bases.push_back(new SimObject(name, vec3d(new_lat,height,new_lon), COLOR_GRAY));
                uavs.back()->assign_base(bases.back());
            }
            else {
                stringw name = "EO-";
                name += ++wp_id;

                wps.push_back(new WaypointObject(name, vec3d(new_lat,height,new_lon), COLOR_GRAY));
                uavs.back()->assign_waypoint(wps.back());
            }
        }

    }
}

// =========================================================================
// LOAD CONFIGURATION
// =========================================================================

void UAVController::load_config() {
    ifstream in(CONFIG_FILE);
    if(in) {
        cout << "Opening config file: \"" << CONFIG_FILE << "\"\n";
        bool error = false;
        std::string var;
        while(in >> var) {
            std::string str_val;
            float f_val;
            int int_val;
            bool b_val;
            bool rv = false;
            // read in the variable and skip commented lines
            if(var == "#" || var == "" || var == "\n") {
                getline(in, var);
                continue;
            }

            // get configurations
            cout << setw(28) << left;
            // =========================================================================
            // RESOLUTION AND DRIVERS
            // =========================================================================
            if(var == "NAV_RESOLUTION") {
                if(!(in >> str_val)) {error = true; break;}
                // only one resolution, and it's default
                cout << "NAV_RESOLUTION: " << str_val << endl;
                rv = true;
            }
            else if (var == "NAV_WIDTH") {
                if (!(in >> int_val)) {
                    error = true; 
                    break;
                }
                NAV_WIDTH = int_val;
                cout << "NAV_WIDTH: " << NAV_WIDTH << "\n";
            }
            else if (var == "NAV_HEIGHT") {
                if (!(in >> int_val)) {
                    error = true; 
                    break;
                }
                NAV_HEIGHT = int_val;
                cout << "NAV_HEIGHT: " << NAV_HEIGHT << "\n";
            }
            else if(var == "NAV_DRIVER") {
                if(!(in >> str_val)) {error = true; break;}
                if(str_val == "OPENGL")         NAV_DRIVER = EDT_OPENGL;
                else if(str_val == "DIRECT3D9") NAV_DRIVER = EDT_DIRECT3D9;
                else if(str_val == "DIRECT3D8") NAV_DRIVER = EDT_DIRECT3D8;
                else                            NAV_DRIVER = EDT_OPENGL;
                cout << "NAV_DRIVER: " << str_val << endl;
                rv = true;
            } 
            else if(var == "NAV_POS_X") {
                if(!(in >> f_val)) {error = true; break;}
                NAV_POS.Width = s32(f_val);
                cout << "NAV_POS_X: " << f_val << endl;
                rv = true;
            } 
            else if(var == "NAV_POS_Y") {
                if(!(in >> f_val)) {error = true; break;}
                NAV_POS.Height = s32(f_val);
                cout << "NAV_POS_Y: " << f_val << endl;
                rv = true;
            } 
            //else if(var == "CAM_RESOLUTION") {
            //    if(!(in >> str_val)) {error = true; break;}
            //    if     (str_val == "1024x768")  CAM_RESOLUTION = R1024X768;
            //    else if(str_val == "1152x864")  CAM_RESOLUTION = R1152X864;
            //    else if(str_val == "2560x1600") CAM_RESOLUTION = R2560X1600;
            //    cout << "CAM_RESOLUTION: " << str_val << endl;
            //    rv = true;
            //} 
            else if (var == "CAM_WIDTH") {
                if (!(in >> int_val)) {
                    error = true; 
                    break;
                }
                CAM_WIDTH = int_val;
                cout << "CAM_WIDTH: " << CAM_WIDTH << "\n";
            }
            else if (var == "CAM_HEIGHT") {
                if (!(in >> int_val)) {
                    error = true;
                    break;
                }
                CAM_HEIGHT = int_val;
                cout << "CAM_HEIGHT: " << CAM_HEIGHT << "\n";
            }
            else if (var == "CAM_INTERVAL") {
                if (!(in >> int_val)) {
                    error = true;
                    break;
                }
                CAM_INTERVAL = int_val;
                cout << "CAM_INTERVAL: " << CAM_INTERVAL << "\n";
            }
            else if(var == "CAM_DRIVER") {
                if(!(in >> str_val)) {error = true; break;}
                if(str_val == "OPENGL")         CAM_DRIVER = EDT_OPENGL;
                else if(str_val == "DIRECT3D9") CAM_DRIVER = EDT_DIRECT3D9;
                else if(str_val == "DIRECT3D8") CAM_DRIVER = EDT_DIRECT3D8;
                else                            CAM_DRIVER = EDT_DIRECT3D9;
                cout << "CAM_DRIVER: " << str_val << endl;
                rv = true;
            } 
            else if(var == "CAM_POS_X") {
                if(!(in >> f_val)) {error = true; break;}
                CAM_POS.Width = s32(f_val);
                cout << "CAM_POS_X: " << f_val << endl;
                rv = true;
            } 
            else if(var == "CAM_POS_Y") {
                if(!(in >> f_val)) {error = true; break;}
                CAM_POS.Height = s32(f_val);
                cout << "CAM_POS_Y: " << f_val << endl;
                rv = true;
            }
            // =========================================================================
            // GENERAL CONFIGURATION
            // =========================================================================
            if(var == "SCENARIO_FILE") {
                getline(in, str_val);

                // front
                size_t index = str_val.find_first_of("\"");
                if(index != std::string::npos)
                    str_val = str_val.substr(index + 1, str_val.size() - index - 1);
                else {error = true; break;}
                index = str_val.find_last_of("\"");
                // back
                if(index != std::string::npos)
                    str_val = str_val.substr(0, index);
                else {error = true; break;}

                // save out the filename
                SCENARIO_FILE = str_val.c_str();
                cout << "SCENARIO_FILE: " << SCENARIO_FILE.c_str() << endl;
                rv = true;
            } 
            else if(var == "OUTPUT_FILE") {
                getline(in, str_val);

                // front
                size_t index = str_val.find_first_of("\"");
                if(index != std::string::npos)
                    str_val = str_val.substr(index + 1, str_val.size() - index - 1);
                else {error = true; break;}
                index = str_val.find_last_of("\"");
                // back
                if(index != std::string::npos)
                    str_val = str_val.substr(0, index);
                else {error = true; break;}

                // save out the filename
                OUTPUT_FILE = str_val.c_str();
                cout << "OUTPUT_FILE: " << OUTPUT_FILE.c_str() << endl;
                rv = true;
            } 
            else if(var == "TIMESTAMP_OUTPUT") {
                if(!(in >> b_val)) {error = true; break;}
                TIMESTAMP_OUTPUT = b_val;
                cout << "TIMESTAMP_OUTPUT: " << b_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_TIME") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_TIME = b_val;
                cout << "SHOW_TIME: " << b_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_VERSION") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_VERSION = b_val;
                cout << "SHOW_VERSION: " << b_val << endl;
                rv = true;
            } 
            else if(var == "ZOOM_NEAR_TARGET") {
                if(!(in >> b_val)) {error = true; break;}
                ZOOM_NEAR_TARGET = b_val;
                cout << "ZOOM_NEAR_TARGET: " << b_val << endl;
                rv = true;
            } 
            else if(var == "FEATURE_VISIBLE_DIST") {
                if(!(in >> f_val)) {error = true; break;}
                FEATURE_VISIBLE_DIST = f_val;
                cout << "FEATURE_VISIBLE_DIST: " << f_val << endl;
                rv = true;
            } 
            else if(var == "FEATURE_MISSED_DIST") {
                if(!(in >> f_val)) {error = true; break;}
                FEATURE_MISSED_DIST = f_val;
                cout << "FEATURE_MISSED_DIST: " << f_val << endl;
                rv = true;
            } 
            else if(var == "LIGHT_AFTER_PASS") {
                if(!(in >> b_val)) {error = true; break;}
                LIGHT_AFTER_PASS = b_val;
                cout << "LIGHT_AFTER_PASS: " << b_val << endl;
                rv = true;
            } 
            else if(var == "USE_CAMERA_FAIL") {
                if(!(in >> b_val)) {error = true; break;}
                USE_CAMERA_FAIL = b_val;
                cout << "USE_CAMERA_FAIL: " << b_val << endl;
                rv = true;
            } 
            else if(var == "CAM_FAIL_AS_ANGLE") {
                if(!(in >> b_val)) {error = true; break;}
                CAM_FAIL_AS_ANGLE = b_val;
                cout << "CAM_FAIL_AS_ANGLE: " << b_val << endl;
                rv = true;
            } 
            else if(var == "USE_GPS_FAIL") {
                if(!(in >> b_val)) {error = true; break;}
                USE_GPS_FAIL = b_val;
                cout << "USE_GPS_FAIL: " << b_val << endl;
                rv = true;
            } 
            else if(var == "ALLOW_SPEED_CHANGES") {
                if(!(in >> b_val)) {error = true; break;}
                ALLOW_SPEED_CHANGES = b_val;
                cout << "ALLOW_SPEED_CHANGES: " << b_val << endl;
                rv = true;
            } 
            else if(var == "ALLOW_PAUSE") {
                if(!(in >> b_val)) {error = true; break;}
                ALLOW_PAUSE= b_val;
                cout << "ALLOW_PAUSE: " << b_val << endl;
                rv = true;
            } 
            else if(var == "CAM_SHADE_AREA") {
                if(!(in >> str_val)) {error = true; break;}
                if(str_val == "VIEWPORT") {
                    USE_VIEW_CAM_SHADING = true;
                    USE_FULL_CAM_SHADING = false;
                } else if(str_val == "FULL") {
                    USE_VIEW_CAM_SHADING = false;
                    USE_FULL_CAM_SHADING = true;
                } else {
                    USE_VIEW_CAM_SHADING = false;
                    USE_FULL_CAM_SHADING = false;
                }
                rv = true;
            } 
            else if(var == "SPEED_FACTOR") {
                if(!(in >> f_val)) {error = true; break;}
                SPEED_FACTOR = f_val;
                cout << "SPEED_FACTOR: " << f_val << endl;
                rv = true;
            } 
            else if(var == "FUEL_FACTOR") {
                if(!(in >> f_val)) {error = true; break;}
                FUEL_FACTOR = f_val;
                cout << "FUEL_FACTOR: " << f_val << endl;
                rv = true;
            } 
            else if(var == "DOWN_ANGLE") {
                if(!(in >> f_val)) {error = true; break;}
                DOWN_ANGLE = f_val;
                cout << "DOWN_ANGLE: " << f_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_PROGRESS_BAR") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_PROGRESS_BAR = b_val;
                cout << "SHOW_PROGRESS_BAR: " << b_val << endl;
                rv = true;
            } 
            else if(var == "BUTTON_CLICK_MS") {
                if(!(in >> int_val)) {error = true; break;}
                BUTTON_MS = int_val;
                cout << "BUTTON_CLICK_MS: " << int_val << endl;
                rv = true;
            }
            // =========================================================================
            // MAP INTERFACE
            // =========================================================================
            if(var == "ZOOM_AMOUNT") {
                if(!(in >> f_val)) {error = true; break;}
                ZOOM_AMOUNT = f_val;
                cout << "ZOOM_AMOUNT: " << f_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_BASE_TEXT") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_BASE_TEXT = b_val;
                cout << "SHOW_BASE_TEXT: " << b_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_BASE_ICONS") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_BASE_ICONS = b_val;
                cout << "SHOW_BASE_ICONS: " << b_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_WAYPOINT_TEXT") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_WAYPOINT_TEXT = b_val;
                cout << "SHOW_WAYPOINT_TEXT: " << b_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_WAYPOINT_ICONS") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_WAYPOINT_ICONS = b_val;
                cout << "SHOW_WAYPOINT_ICONS: " << b_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_UAV_TEXT") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_UAV_TEXT = b_val;
                cout << "SHOW_UAV_TEXT: " << b_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_UAV_ICONS") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_UAV_ICONS = b_val;
                cout << "SHOW_UAV_ICONS: " << b_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_UAV_LINES") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_UAV_LINES = b_val;
                cout << "SHOW_UAV_LINES: " << b_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_EXT_UAV_LINES") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_EXT_UAV_LINES = b_val;
                cout << "SHOW_EXT_UAV_LINES: " << b_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_UFO_TEXT") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_UFO_TEXT = b_val;
                cout << "SHOW_UFO_TEXT: " << b_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_UFO_ICONS") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_UFO_ICONS = b_val;
                cout << "SHOW_UFO_ICONS: " << b_val << endl;
                rv = true;
            } 
            else if(var == "SHOW_UAV_START_ICONS") {
                if(!(in >> b_val)) {error = true; break;}
                SHOW_UAV_START_ICONS = b_val;
                cout << "SHOW_UAV_START_ICONS: " << b_val << endl;
                rv = true;
            } 
            else if(var == "MAP_ICON_SIZE") {
                if(!(in >> int_val)) {error = true; break;}
                MAP_ICON_SIZE = int_val;
                cout << "MAP_ICON_SIZE: " << int_val << endl;
                rv = true;
            }
            // =========================================================================
            // LIGHTING CUES
            // =========================================================================
            if(var == "USE_LIGHT_CUES") {
                if(!(in >> b_val)) {error = true; break;}
                USE_LIGHT_CUES = b_val;
                cout << "USE_LIGHT_CUES: " << b_val << endl;
                rv = true;
            } 
            else if(var == "LOW_LIGHT_LEVEL") {
                if(!(in >> int_val)) {error = true; break;}
                LOW_LIGHT_LEVEL = int_val;
                cout << "LOW_LIGHT_LEVEL: " << int_val << endl;
                rv = true;
            } 
            else if(var == "MEDIUM_LIGHT_LEVEL") {
                if(!(in >> int_val)) {error = true; break;}
                MEDIUM_LIGHT_LEVEL = int_val;
                cout << "MEDIUM_LIGHT_LEVEL: " << int_val << endl;
                rv = true;
            } 
            else if(var == "HIGH_LIGHT_LEVEL") {
                if(!(in >> int_val)) {error = true; break;}
                HIGH_LIGHT_LEVEL = int_val;
                cout << "HIGH_LIGHT_LEVEL: " << int_val << endl;
                rv = true;
            } 
            else if(var == "BASE_LIGHT_LEVEL") {
                if(!(in >> str_val)) {error = true; break;}
                if(str_val == "LOW")            BASE_LIGHT_LEVEL = LOW_LIGHT_LEVEL;
                else if(str_val == "MEDIUM")    BASE_LIGHT_LEVEL = MEDIUM_LIGHT_LEVEL;
                else if(str_val == "HIGH")      BASE_LIGHT_LEVEL = HIGH_LIGHT_LEVEL;
                else                            BASE_LIGHT_LEVEL = LOW_LIGHT_LEVEL;
                cout << "BASE_LIGHT_LEVEL: " << BASE_LIGHT_LEVEL << endl;
                rv = true;
            } 
            else if(var == "MEDIUM_LIGHT_DIST") {
                if(!(in >> f_val)) {error = true; break;}
                MEDIUM_LIGHT_DIST = f_val;
                cout << "MEDIUM_LIGHT_DIST: " << f_val << endl;
                rv = true;
            } 
            else if(var == "HIGH_LIGHT_DIST") {
                if(!(in >> f_val)) {error = true; break;}
                MEDIUM_LIGHT_DIST = f_val;
                cout << "HIGH_LIGHT_DIST: " << f_val << endl;
                rv = true;
            }
            // =========================================================================
            // TACTOR CUES
            // =========================================================================
            if(var == "RUN_TACTOR_TEST") {
                if(!(in >> b_val)) {error = true; break;}
                RUN_TACTOR_TEST = b_val;
                cout << "RUN_TACTOR_TEST: " << b_val << endl;
                rv = true;
            } 
            else if(var == "USE_TACTOR_CUES") {
                if(!(in >> b_val)) {error = true; break;}
                USE_TACTOR_CUES = b_val;
                cout << "USE_TACTOR_CUES: " << b_val << endl;
                rv = true;
            } 
            else if(var == "TACTOR_COM_PORT_1") {
                if(!(in >> int_val)) {error = true; break;}
                TACTOR_COM_PORT_1 = int_val;
                cout << "TACTOR_COM_PORT_1: " << int_val << endl;
                rv = true;
            }
            else if(var == "TACTOR_COM_PORT_2") {
                if(!(in >> int_val)) {error = true; break;}
                TACTOR_COM_PORT_2 = int_val;
                cout << "TACTOR_COM_PORT_2: " << int_val << endl;
                rv = true;
            }
            else if(var == "TACTOR_COM_PORT_3") {
                if(!(in >> int_val)) {error = true; break;}
                TACTOR_COM_PORT_3 = int_val;
                cout << "TACTOR_COM_PORT_3: " << int_val << endl;
                rv = true;
            }
            else if(var == "TACTOR_COM_PORT_4") {
                if(!(in >> int_val)) {error = true; break;}
                TACTOR_COM_PORT_4 = int_val;
                cout << "TACTOR_COM_PORT_4: " << int_val << endl;
                rv = true;
            }
            else if(var == "TACTOR_COM_PORT_5") {
                if(!(in >> int_val)) {error = true; break;}
                TACTOR_COM_PORT_5 = int_val;
                cout << "TACTOR_COM_PORT_5: " << int_val << endl;
                rv = true;
            }
            else if(var == "TACTOR_COM_PORT_6") {
                if(!(in >> int_val)) {error = true; break;}
                TACTOR_COM_PORT_6 = int_val;
                cout << "TACTOR_COM_PORT_6: " << int_val << endl;
                rv = true;
            }
            else if(var == "TACTOR_GAIN") {
                if(!(in >> int_val)) {error = true; break;}
                TACTOR_GAIN = int_val;
                cout << "TACTOR_GAIN: " << int_val << endl;
                rv = true;
            }
            else if(var == "TACTOR_FREQ") {
                if(!(in >> int_val)) {error = true; break;}
                TACTOR_FREQUENCY = int_val;
                cout << "TACTOR_FREQ: " << int_val << endl;
                rv = true;
            }
            else if(var == "HIGH_TACTOR_FREQ_1") {
                if(!(in >> int_val)) {error = true; break;}
                HIGH_TACTOR_FREQ_1 = int_val;
                cout << "HIGH_TACTOR_FREQ_1: " << int_val << endl;
                rv = true;
            }
            else if(var == "HIGH_TACTOR_FREQ_2") {
                if(!(in >> int_val)) {error = true; break;}
                HIGH_TACTOR_FREQ_2 = int_val;
                cout << "HIGH_TACTOR_FREQ_2: " << int_val << endl;
                rv = true;
            }
            else if(var == "HIGH_TACTOR_SOURCE") {
                if(!(in >> str_val)) {error = true; break;}
                if     (str_val == "FREQ1") HIGH_TACTOR_SOURCE = 1;
                else if(str_val == "FREQ2") HIGH_TACTOR_SOURCE = 2;
                else if(str_val == "MIXED") HIGH_TACTOR_SOURCE = 3;
                else                        HIGH_TACTOR_SOURCE = 0;
                rv = true;
            }
            else if(var == "HIGH_TACTOR_LENGTH") {
                if(!(in >> int_val)) {error = true; break;}
                HIGH_TACTOR_LENGTH = int_val;
                cout << "HIGH_TACTOR_LENGTH: " << int_val << endl;
                rv = true;
            }
            else if(var == "HIGH_TACTOR_DIST") {
                if(!(in >> f_val)) {error = true; break;}
                HIGH_TACTOR_DIST = f_val;
                cout << "HIGH_TACTOR_DIST: " << f_val << endl;
                rv = true;
            }
            else if(var == "HIGH_TACTOR_PULSE") {
                if(!(in >> b_val)) {error = true; break;}
                HIGH_TACTOR_PULSE = b_val;
                cout << "HIGH_TACTOR_PULSE: " << b_val << endl;
                rv = true;
            }
            else if(var == "HIGH_TACTOR_PULSE_INTERVAL") {
                if(!(in >> int_val)) {error = true; break;}
                HIGH_TACTOR_PULSE_INTERVAL = int_val;
                cout << "HIGH_TACTOR_PULSE_INTERVAL: " << int_val << endl;
                rv = true;
            }
            else if(var == "LOW_TACTOR_FREQ_1") {
                if(!(in >> int_val)) {error = true; break;}
                LOW_TACTOR_FREQ_1 = int_val;
                cout << "LOW_TACTOR_FREQ_1: " << int_val << endl;
                rv = true;
            }
            else if(var == "LOW_TACTOR_FREQ_2") {
                if(!(in >> int_val)) {error = true; break;}
                LOW_TACTOR_FREQ_2 = int_val;
                cout << "LOW_TACTOR_FREQ_2: " << int_val << endl;
                rv = true;
            }
            else if(var == "LOW_TACTOR_SOURCE") {
                if(!(in >> str_val)) {error = true; break;}
                if     (str_val == "FREQ1") LOW_TACTOR_SOURCE = 1;
                else if(str_val == "FREQ2") LOW_TACTOR_SOURCE = 2;
                else if(str_val == "MIXED") LOW_TACTOR_SOURCE = 3;
                else                        LOW_TACTOR_SOURCE = 0;
                rv = true;
            }
            else if(var == "LOW_TACTOR_LENGTH") {
                if(!(in >> int_val)) {error = true; break;}
                LOW_TACTOR_LENGTH = int_val;
                cout << "LOW_TACTOR_LENGTH: " << int_val << endl;
                rv = true;
            }
            else if(var == "LOW_TACTOR_DIST") {
                if(!(in >> f_val)) {error = true; break;}
                LOW_TACTOR_DIST = f_val;
                cout << "LOW_TACTOR_DIST: " << f_val << endl;
                rv = true;
            }
            else if(var == "LOW_TACTOR_PULSE") {
                if(!(in >> b_val)) {error = true; break;}
                LOW_TACTOR_PULSE = b_val;
                cout << "LOW_TACTOR_PULSE: " << b_val << endl;
                rv = true;
            }
            else if(var == "LOW_TACTOR_PULSE_INTERVAL") {
                if(!(in >> int_val)) {error = true; break;}
                LOW_TACTOR_PULSE_INTERVAL = int_val;
                cout << "LOW_TACTOR_PULSE_INTERVAL: " << int_val << endl;
                rv = true;
            }
            // =========================================================================
            // WAYPOINT FEATURE COLORS
            // =========================================================================
            if(var == "COLOR_FCA") {
                if(!(in >> int_val)) {error = true; break;}
                WAYPOINT_CONFIRMED.setAlpha(int_val);
                cout << "COLOR CONFIRMED ALPHA: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_FCR") {
                if(!(in >> int_val)) {error = true; break;}
                WAYPOINT_CONFIRMED.setRed(int_val);
                cout << "COLOR CONFIRMED RED: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_FCG") {
                if(!(in >> int_val)) {error = true; break;}
                WAYPOINT_CONFIRMED.setGreen(int_val);
                cout << "COLOR CONFIRMED GREEN: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_FCB") {
                if(!(in >> int_val)) {error = true; break;}
                WAYPOINT_CONFIRMED.setBlue(int_val);
                cout << "COLOR CONFIRMED BLUE: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_FDA") {
                if(!(in >> int_val)) {error = true; break;}
                WAYPOINT_CLEAR.setAlpha(int_val);
                cout << "COLOR CLEAR ALPHA: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_FDR") {
                if(!(in >> int_val)) {error = true; break;}
                WAYPOINT_CLEAR.setRed(int_val);
                cout << "COLOR CLEAR RED: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_FDG") {
                if(!(in >> int_val)) {error = true; break;}
                WAYPOINT_CLEAR.setGreen(int_val);
                cout << "COLOR CLEAR GREEN: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_FDB") {
                if(!(in >> int_val)) {error = true; break;}
                WAYPOINT_CLEAR.setBlue(int_val);
                cout << "COLOR CLEAR BLUE: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_FUA") {
                if(!(in >> int_val)) {error = true; break;}
                WAYPOINT_UNSURE.setAlpha(int_val);
                cout << "COLOR UNSURE ALPHA: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_FUR") {
                if(!(in >> int_val)) {error = true; break;}
                WAYPOINT_UNSURE.setRed(int_val);
                cout << "COLOR UNSURE RED: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_FUG") {
                if(!(in >> int_val)) {error = true; break;}
                WAYPOINT_UNSURE.setGreen(int_val);
                cout << "COLOR UNSURE GREEN: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_FUB") {
                if(!(in >> int_val)) {error = true; break;}
                WAYPOINT_UNSURE.setBlue(int_val);
                cout << "COLOR UNSURE BLUE: " << int_val << endl;
                rv = true;
            }
            // =========================================================================
            // UAV COLORS
            // =========================================================================
            if(var == "COLOR_UAV1A") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR1.setAlpha(int_val);
                cout << "COLOR1 ALPHA: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV1R") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR1.setRed(int_val);
                cout << "COLOR1 RED: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV1G") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR1.setGreen(int_val);
                cout << "COLOR1 GREEN: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV1B") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR1.setBlue(int_val);
                cout << "COLOR1 BLUE: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV2A") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR2.setAlpha(int_val);
                cout << "COLOR2 ALPHA: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV2R") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR2.setRed(int_val);
                cout << "COLOR2 RED: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV2G") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR2.setGreen(int_val);
                cout << "COLOR2 GREEN: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV2B") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR2.setBlue(int_val);
                cout << "COLOR2 BLUE: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV3A") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR3.setAlpha(int_val);
                cout << "COLOR3 ALPHA: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV3R") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR3.setRed(int_val);
                cout << "COLOR3 RED: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV3G") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR3.setGreen(int_val);
                cout << "COLOR3 GREEN: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV3B") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR3.setBlue(int_val);
                cout << "COLOR3 BLUE: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV4A") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR4.setAlpha(int_val);
                cout << "COLOR4 ALPHA: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV4R") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR4.setRed(int_val);
                cout << "COLOR4 RED: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV4G") { 
                if(!(in >> int_val)) {error = true; break;}
                COLOR4.setGreen(int_val);
                cout << "COLOR4 GREEN: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV4B") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR4.setBlue(int_val);
                cout << "COLOR4 BLUE: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV5A") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR5.setAlpha(int_val);
                cout << "COLOR5 ALPHA: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV5R") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR5.setRed(int_val);
                cout << "COLOR5 RED: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV5G") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR5.setGreen(int_val);
                cout << "COLOR5 GREEN: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV5B") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR5.setBlue(int_val);
                cout << "COLOR5 BLUE: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV6A") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR6.setAlpha(int_val);
                cout << "COLOR6 ALPHA: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV6R") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR6.setRed(int_val);
                cout << "COLOR6 RED: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV6G") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR6.setGreen(int_val);
                cout << "COLOR6 GREEN: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV6B") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR6.setBlue(int_val);
                cout << "COLOR6 BLUE: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV7A") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR7.setAlpha(int_val);
                cout << "COLOR7 ALPHA: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV7R") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR7.setRed(int_val);
                cout << "COLOR7 RED: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV7G") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR7.setGreen(int_val);
                cout << "COLOR7 GREEN: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV7B") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR7.setBlue(int_val);
                cout << "COLOR7 BLUE: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV8A") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR8.setAlpha(int_val);
                cout << "COLOR8 ALPHA: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV8R") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR8.setRed(int_val);
                cout << "COLOR8 RED: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV8G") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR8.setGreen(int_val);
                cout << "COLOR8 GREEN: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV8B") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR8.setBlue(int_val);
                cout << "COLOR8 BLUE: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV9A") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR9.setAlpha(int_val);
                cout << "COLOR9 ALPHA: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV9R") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR9.setRed(int_val);
                cout << "COLOR9 RED: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV9G") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR9.setGreen(int_val);
                cout << "COLOR9 GREEN: " << int_val << endl;
                rv = true;
            }
            else if(var == "COLOR_UAV9B") {
                if(!(in >> int_val)) {error = true; break;}
                COLOR9.setBlue(int_val);
                cout << "COLOR9 BLUE: " << int_val << endl;
                rv = true;
            }
            // =========================================================================
            // GARBAGE
            // =========================================================================
            if (!rv) { 
                cout << "  garbage: " << var << endl; 
            }
        }
        if(error)
            cout << "Warning: error reading configuration" << endl;
        cout << endl << endl;
        in.close();
    }
    else {
        cout << "Warning: unable to open \"" << CONFIG_FILE << "\"; using default parameters\n";
    }
}