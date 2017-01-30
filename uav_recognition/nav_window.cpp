#include "nav_window.h"
#include "cam_window.h"
#include "globals.h"
#include "output.h"

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

irr::f32 ZOOM_AMOUNT = 0.1f;
SColor CHAT_EVENT_COLOR(255, 255, 0, 0);
bool SHOW_TIME = false;
bool SHOW_VERSION = false;
bool USE_GPS_FAIL = true;

irr::video::SColor getColorFromId(irr::s32 id)
{
    switch(id)
    {
    case 1: return COLOR1;
    case 2: return COLOR2;
    case 3: return COLOR3;
    case 4: return COLOR4;
    case 5: return COLOR5;
    case 6: return COLOR6;
    case 7: return COLOR7;
    case 8: return COLOR8;
    default: return COLOR9;
    }
}

NavWindow::NavWindow(std::list<WaypointObject *> * wps_,
    std::list<SimObject *> * bases_,
    std::list<UAVObject *> * uavs_,
    std::list<UFOObject *> * ufos_,
    std::list<Event *> * events_,
    int nav_width,
    int nav_height,
    dimension2di position,
    E_DRIVER_TYPE driver)
    : UAVWindow("UAV Flight Sim - Navigation Window", false, false, driver, std::make_pair(800, 600), position),
    wps(wps_), bases(bases_), uavs(uavs_), ufos(ufos_), events(events_),
    moving_map(false), started(false)
{
    if(!load())
        throw Error("Nav window failed to initialize correctly.");
}

bool NavWindow::load()
{
    if(!UAVWindow::load())
        return false;

    load_images();
    load_fonts();
    map.load_images(device());
    chat.load_images(device());
    
    // we want 9 consoles (clear them first)
    consoles.clear();
    uav_it = uavs->begin();
    for(int i = 0; i < 9; i++) {
        UAVConsole * cons = new UAVConsole();
        if(i == 0)
            cons->load_images(device());
        if(uav_it != uavs->end())
        {
            cons->set_id(device(), i, *uav_it);
            uav_it++;
        }
        else
        {
            cons->set_id(device(), i, 0);
        }
        consoles.push_back(cons);
    }

    return true;
}

void NavWindow::draw()
{
    driver()->beginScene(true,true,0);

    // do any RTTs we need
    for(cons_it = consoles.begin(); cons_it != consoles.end(); cons_it++)
        (*cons_it)->render_to_texture(this);

    // draw the UAV states
    draw_states();

    // draw the map
    map.draw(driver());
    draw_map();

    // MOVED TO SHOW TEXT
    //map.draw_interface(driver());

    // draw the chatbox
    chat.draw(driver(), small_text);

    // draw the console boxes
    for(cons_it = consoles.begin(); cons_it != consoles.end(); cons_it++)
        (*cons_it)->draw(device());

    // draw the start instructions
    if(!started)
        start_overlay->draw();

    // draw the end instructions
    if(start_end)
        end_overlay->draw();

    driver()->endScene();
}

void NavWindow::load_images()
{
    IGUIElement * guiElmRoot = guienv()->getRootGUIElement();

    device()->getFileSystem()->addZipFileArchive(UAV_CONS_ZIP.c_str());
    device()->getFileSystem()->addZipFileArchive(OBJ_ICONS_ZIP.c_str());

    start_overlay = new GUIImage(rect<s32>(0,0,512,256), device(), guiElmRoot);
    start_overlay->setTexture(driver()->getTexture("start_overlay.png"));
    start_overlay->setPosition(position2d<s32>(325,150));

    end_overlay = new GUIImage(rect<s32>(0,0,512,256), device(), guiElmRoot);
    end_overlay->setTexture(driver()->getTexture("start_overlay.png"));
    end_overlay->setPosition(position2d<s32>(325,150));

    goto_icon = new GUIImage(rect<s32>(0,0,32,32), device(), guiElmRoot);
    goto_icon->setTexture(driver()->getTexture("goto_icon.png"));

    return_icon = new GUIImage(rect<s32>(0,0,32,32), device(), guiElmRoot);
    return_icon->setTexture(driver()->getTexture("return_icon.png"));
}

void NavWindow::load_fonts()
{
    // get the font image files
    device()->getFileSystem()->addZipFileArchive(FONTS_ZIP.c_str());
    IGUIFont * font1 = device()->getGUIEnvironment()->getFont(FONT_LARGE.c_str());
    IGUIFont * font2 = device()->getGUIEnvironment()->getFont(FONT_SMALL.c_str());
    stringc err = "Failed to load font";
    if(!(font1 && font2))throw Error(err.c_str());

    // bigger text for the callsign
    large_text = guienv()->addStaticText(L"",rect<s32>(0,0,0,0),false, false);
    large_text->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    large_text->setOverrideColor(COLOR_WHITE);
    large_text->setOverrideFont(font1);

    // small text for everything else
    small_text = guienv()->addStaticText(L"",rect<s32>(0,0,0,0),false, false);
    small_text->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    small_text->setOverrideColor(COLOR_WHITE);
    small_text->setOverrideFont(font2);
}

void NavWindow::triggerEvent(Event * e)
{
    if(e)
    {
        stringw output;

        // really ugly way to change id into iterator (cons_it)
        // this depends on consoles being ordered in their list
        int id = -1;
        if(e->get_type() == CAMERA_FAIL || e->get_type() == GPS_FAIL)
        {
            id = e->get_id();
            
            cons_it = consoles.begin();
            int i = 1;
            while(i < id)
            {
                cons_it++;
                if(cons_it == consoles.end())
                {
                    cout << "Failed event (type: " << e->get_type() << ")" << endl;
                    return; // this event is fail :(
                }
                i++;
            }
            output += (*cons_it)->get_uav()->getName();
            output += " - ";
        }

        u32 time = MILLISECONDS + (1000 * SECONDS) + (60 * 1000 * MINUTES);
        output += "event triggered: ";

        // handle events
        switch(e->get_type()) {
        case CAMERA_FAIL:
            output += "camera failure";
            (*cons_it)->set_cam_fail(true);
            break;

        case GPS_FAIL:
            output += "GPS failure";
            (*cons_it)->set_gps_fail(true);
            break;

        case CHAT_REQUEST:
            output += "chat request - \"";
            output += e->get_text();
            output += "\"";
            
            chat.add_text(e->get_text(), CHAT_EVENT_COLOR);
            break;

        default:
            break;
        }

        // spit to file
        Output::Instance().WriteTick();
        Output::Instance().WriteLine(output);
        Output::Instance().WriteTick(OUTPUT_EVENTS);
        Output::Instance().WriteLine(output, OUTPUT_EVENTS);
        if(e->get_type() == CHAT_REQUEST)
        {
            Output::Instance().WriteTick(OUTPUT_CHAT);
            Output::Instance().WriteLine(output, OUTPUT_CHAT);
        }
    }
}

/* Input Event Handlers */
void NavWindow::event_mouse_down()
{
    position2di cursor = device()->getCursorControl()->getPosition();

    // if the mouse is over the map
    if(map.isMouseOverMap(cursor))
    {
        // we need to know if the mouse click is "used"
        bool mouse_used = false;

        // get the cursor position on the map
        position2di cursorPos = map.mouseToMapCoords(cursor);

        // check for UFOs on that pixel
        for(ufo_it = ufos->begin(); ufo_it != ufos->end(); ufo_it++)
        {
            position2di ufoPos = map.worldToMapCoords((*ufo_it)->getPosition2D());
            ufoPos.Y -= 28; // NO TITLEBAR FIX
            ufoPos.X -= 2;  // NO TITLEBAR FIX
            if(!(*ufo_it)->reported()
                && (ufoPos.X - 16 < cursorPos.X && ufoPos.X + 16 > cursorPos.X)
                && (ufoPos.Y - 16 < cursorPos.Y && ufoPos.Y + 16 > cursorPos.Y))
            {
                mouse_used = true;
                (*ufo_it)->report();
                break;
            }
        }

        // check for UAVs on that pixel
        if(!mouse_used) {
            // DO NOTHING
        }

        // maybe move the map
        if(!mouse_used)
            moving_map = true;
    }

    // if the mouse is over the chatbox
    if(chat.is_mouse_over(cursor))
        chat.focus();

    // check for camera and gps fail clicks
    for(cons_it = consoles.begin(); cons_it != consoles.end(); cons_it++)
    {
        // camera fail
        if((*cons_it)->get_cam_fail())
        {
            if((*cons_it)->cam_button_over(cursor))
            {
                (*cons_it)->set_cam_fail(false);
                (*cons_it)->force_render();

                stringw output = "camera fail fixed on ";
                output += (*cons_it)->get_uav()->getName();
                Output::Instance().WriteTick();
                Output::Instance().WriteLine(output);
                Output::Instance().WriteTick(OUTPUT_EVENTS);
                Output::Instance().WriteLine(output, OUTPUT_EVENTS);
            }
        }
        // gps fail
        if((*cons_it)->get_gps_fail())
        {
            if((*cons_it)->gps_button_over(cursor))
            {
                (*cons_it)->set_gps_fail(false);
                (*cons_it)->force_render();

                stringw output = "GPS fail fixed on ";
                output += (*cons_it)->get_uav()->getName();
                Output::Instance().WriteTick();
                Output::Instance().WriteLine(output);
                Output::Instance().WriteTick(OUTPUT_EVENTS);
                Output::Instance().WriteLine(output, OUTPUT_EVENTS);
            }
        }
    }
}

void NavWindow::event_mouse_up()
{
    moving_map = false;
}

void NavWindow::event_mouse_move()
{
    UAVWindow::event_mouse_move();
    position2di cursor = device()->getCursorControl()->getPosition();

    if(moving_map)
        map.moveMapRel(rel_cursor);
}

void NavWindow::event_mouse_wheel_up()
{
    position2di cursor = device()->getCursorControl()->getPosition();

    if(map.isMouseOverMap(cursor))
        map.setZoom(ZOOM_AMOUNT);

    if(chat.is_mouse_over(cursor))
        chat.scroll(-1);
}

void NavWindow::event_key_down(wchar_t key) {
    UAVWindow::event_key_down(key);
    if (!started && event_recv->IsKeyDown(irr::KEY_RETURN)
        && !event_recv->IsKeyDown(irr::KEY_SHIFT))
        started = true;
    chat.keystroke(key);
}

void NavWindow::event_mouse_wheel_down()
{
    position2di cursor = device()->getCursorControl()->getPosition();
    if(map.isMouseOverMap(cursor))
        map.setZoom(-ZOOM_AMOUNT);

    if(chat.is_mouse_over(cursor))
        chat.scroll(1);
}

/* Specialized draw functions */
void NavWindow::draw_map() {
    // draw the UAV path lines
    if(SHOW_UAV_LINES) {
        for(uav_it = uavs->begin(); uav_it != uavs->end(); uav_it++) {
            if(USE_GPS_FAIL && !(*uav_it)->hasGPSFail())
                map.draw_uav_path(*uav_it, driver());
        }
    }

    // draw all the icons
    if(SHOW_BASE_ICONS) {
        for(base_it = bases->begin(); base_it != bases->end(); base_it++)
            map.draw_icon((*base_it)->getPosition2D(), (*base_it)->getColor(), ICON_BASE);
    }
    if(SHOW_UAV_START_ICONS) {
        for(uav_it = uavs->begin(); uav_it != uavs->end(); uav_it++)
            map.draw_icon((*uav_it)->getStartPosition2D(), (*uav_it)->getColor(), ICON_UAV_START);
    }
    if(SHOW_WAYPOINT_ICONS) {
        for(wp_it = wps->begin(); wp_it != wps->end(); wp_it++)
            map.draw_icon((*wp_it)->getPosition2D(), (*wp_it)->getColor(), ICON_WAYPOINT);
    }
    if(SHOW_UFO_ICONS) {
        for(ufo_it = ufos->begin(); ufo_it != ufos->end(); ufo_it++)
        {
            if((*ufo_it)->show())
                map.draw_icon((*ufo_it)->getPosition2D(), (*ufo_it)->getColor(), ICON_UFO);
        }
    }
    if(SHOW_UAV_ICONS) {   
        for(uav_it = uavs->begin(); uav_it != uavs->end(); uav_it++)
        {
            if(USE_GPS_FAIL && !(*uav_it)->hasGPSFail())
                map.draw_icon((*uav_it)->getPosition2D(), (*uav_it)->getColor(), ICON_UAV, (*uav_it)->getRotAngle2D());
        }
    }

    // MOVED HERE TO SHOW TEXT
    map.draw_interface(driver());

    // draw all the text
    if(SHOW_BASE_TEXT)
    {
        for(base_it = bases->begin(); base_it != bases->end(); base_it++)
            map.draw_text((*base_it)->getPosition2D(), COLOR_WHITE, (*base_it)->getName(), small_text->getOverrideFont());
    }
    if(SHOW_WAYPOINT_TEXT)
    {
        for(wp_it = wps->begin(); wp_it != wps->end(); wp_it++)
            map.draw_text((*wp_it)->getPosition2D(), COLOR_WHITE, (*wp_it)->getName(), small_text->getOverrideFont());
    }
    if(SHOW_UAV_TEXT)
    {
        for(uav_it = uavs->begin(); uav_it != uavs->end(); uav_it++)
        {
            if(USE_GPS_FAIL && !(*uav_it)->hasGPSFail())
                map.draw_text((*uav_it)->getPosition2D(), COLOR_WHITE, (*uav_it)->getName(), small_text->getOverrideFont());
        }
    }
    if(SHOW_UFO_TEXT)
    {
        for(ufo_it = ufos->begin(); ufo_it != ufos->end(); ufo_it++)
        {
            if((*ufo_it)->show())
                map.draw_text((*ufo_it)->getPosition2D(), COLOR_WHITE, (*ufo_it)->getName(), small_text->getOverrideFont());
        }
    }
}

void NavWindow::draw_states()
{
    // draw the states background right below the map
    rect<s32> box(
        MAP_START_X - MAP_BORDER_WIDTH,
        MAP_END_Y + MAP_BORDER_WIDTH,
        MAP_END_X + MAP_BORDER_WIDTH,
        windowHeight() - 3);
    GUIImage::draw2DRectangle(driver(), box, COLOR_EVENT_BG);
    box = rect<s32>(
        MAP_START_X - MAP_BORDER_WIDTH,
        MAP_END_Y + MAP_BORDER_WIDTH,
        MAP_END_X + MAP_BORDER_WIDTH,
        MAP_END_Y + MAP_BORDER_WIDTH + 2);
    GUIImage::draw2DRectangle(driver(), box, COLOR_BLACK);

    uav_it = uavs->begin();
    for(s32 id = 0; id < 9; id++)
    {
        // don't waste my time if we ran out of UAVs
        if(uav_it == uavs->end())
            continue;

        // draw event icon
        if(!(*uav_it)->atBase())
        {
            position2d<s32> icon_pos(
                180 + EVENT_START_X + 6,
                6 + EVENT_START_Y + (EVENT_ICON_HEIGHT + 3) * id);
            //if(!(*it)->isReChecking())
            //{
                goto_icon->setPosition(icon_pos);
                goto_icon->draw();
            //}
            //else
            //{
            //  return_icon->setPosition(icon_pos);
            //  return_icon->draw();
            //}
        }

        // draw colored callsign
        rect<s32> event_source = rect<s32>(
            EVENT_START_X + 6,
            EVENT_START_Y + (EVENT_ICON_HEIGHT + 3) * id,
            EVENT_START_X + EVENT_ICON_WIDTH + 100,
            EVENT_START_Y + (EVENT_ICON_HEIGHT + 3) * id + EVENT_ICON_WIDTH);
        small_text->setText((*uav_it)->getName().c_str());
        small_text->setRelativePosition(event_source);
        small_text->setOverrideColor((*uav_it)->getColor());
        small_text->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);
        small_text->draw();

        stringw text;

        // draw destination
        event_source = rect<s32>(
            EVENT_START_X + 6 + 34,
            EVENT_START_Y + (EVENT_ICON_HEIGHT + 3) * id,
            EVENT_START_X + EVENT_ICON_WIDTH + 200,
            EVENT_START_Y + (EVENT_ICON_HEIGHT + 3) * id + EVENT_ICON_WIDTH);
        if((*uav_it)->atBase())
            text = ": At base";
        else
        {
            text = ": En route to ";
            if(!(*uav_it)->getWPList()->empty())
                text += (*uav_it)->getWPList()->front()->getName();
            else
                text += "base";
        }
        small_text->setText(text.c_str());
        small_text->setOverrideColor(COLOR_WHITE);
        small_text->setRelativePosition(event_source);
        small_text->draw();

        // go to the next UAV
        uav_it++;
    }

    // show the current time
    if(SHOW_TIME)
    {
        rect<s32> time(
            windowWidth()-176,
            windowHeight()-40,
            windowWidth()-2,
            windowHeight()-28);
        stringw text = "Time [";
        text += MINUTES;
        text += ":";
        if(SECONDS < 10)        text += "0";
        text += SECONDS;
        text += ":";
        if(MILLISECONDS < 100)  text += "0";
        if(MILLISECONDS < 10)   text += "0";
        text += MILLISECONDS;
        text += "]";
        small_text->setText(text.c_str());
        small_text->setOverrideColor(COLOR_DARK_GRAY);
        small_text->setTextAlignment(EGUIA_LOWERRIGHT, EGUIA_CENTER);
        small_text->setRelativePosition(time);
        small_text->draw();
    }

    // show the version/build/whatever
    if(SHOW_VERSION)
    {
        rect<s32> version(
            windowWidth()-176,
            windowHeight()-30,
            windowWidth()-2,
            windowHeight()-15);
        stringw text = "THInC Lab, 2009";
        small_text->setText(text.c_str());
        small_text->setOverrideColor(COLOR_DARK_GRAY);
        small_text->setRelativePosition(version);
        small_text->draw();

        // we're the THInC lab >:O
        rect<s32> lab(
            windowWidth()-176,
            windowHeight()-20,
            windowWidth()-2,
            windowHeight());
        text = "UAV Flight Sim (1.0)";
        small_text->setText(text.c_str());
        small_text->setOverrideColor(COLOR_DARK_GRAY);
        small_text->setRelativePosition(lab);
        small_text->draw();
        small_text->setOverrideColor(COLOR_WHITE);
    }
}

void NavWindow::force_render()
{
    for(cons_it = consoles.begin(); cons_it != consoles.end(); cons_it++)
        (*cons_it)->force_render();
}

// void NavWindow::set_other_window(CamWindow * other)
// {
//     cam_window = other;
// }