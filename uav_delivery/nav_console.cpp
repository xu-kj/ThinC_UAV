#include "nav_console.h"
#include "color.h"
#include "nav_window.h"
#include "globals.h"
#include "exceptions.h"

#include <iostream>

using std::cout; 
using std::endl;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

    const s32 OUTLINE_WIDTH = 8;

    // static variables
    s32 UAVConsole::ref_count = 0;
    IGUIFont * UAVConsole::font_small = 0;
    IGUIFont * UAVConsole::font_large = 0;
    GUIImage * UAVConsole::uav_box = 0;
    GUIImage * UAVConsole::uav_callsign = 0;
    GUIImage * UAVConsole::uav_fuel_back = 0;
    GUIImage * UAVConsole::uav_fuel_out = 0;
    GUIImage * UAVConsole::uav_gps = 0;
    GUIImage * UAVConsole::uav_cam = 0;
    IGUIStaticText * UAVConsole::text = 0;
    IGUIStaticText * UAVConsole::TEXT = 0;

UAVConsole::UAVConsole()
    : uav(0), need_render(true)
{
    ref_count++;
}

UAVConsole::~UAVConsole()
{
    // delete buttons
    if(gps_button)  delete gps_button;
    if(cam_button)  delete cam_button;
}

void UAVConsole::load_images(IrrlichtDevice * device)
{
    IVideoDriver * driver = device->getVideoDriver();
    IGUIEnvironment * guienv = device->getGUIEnvironment();
    IGUIElement * guiElmRoot = guienv->getRootGUIElement();

    device->getFileSystem()->addZipFileArchive(UAV_CONS_ZIP.c_str());

    uav_box = new GUIImage(rect<s32>(0,0,256,128), device, guiElmRoot);
    uav_box->setTexture(driver->getTexture("uav_box.png"));

    uav_callsign = new GUIImage(rect<s32>(0,0,256,128), device, guiElmRoot);
    uav_callsign->setTexture(driver->getTexture("uav_callsign.png"));

    uav_fuel_back = new GUIImage(rect<s32>(0,0,256,128), device, guiElmRoot);
    uav_fuel_back->setTexture(driver->getTexture("uav_fuel_back.png"));
    uav_fuel_back->setColor(color::COLOR_DARK_GRAY);

    uav_fuel_out = new GUIImage(rect<s32>(0,0,256,128), device, guiElmRoot);
    uav_fuel_out->setTexture(driver->getTexture("uav_fuel_out.png"));
    uav_fuel_out->setColor(color::COLOR_DARK_GRAY);

    uav_gps = new GUIImage(rect<s32>(0,0,256,128), device, guiElmRoot);
    uav_gps->setTexture(driver->getTexture("uav_gps.png"));

    uav_cam = new GUIImage(rect<s32>(0,0,256,128), device, guiElmRoot);
    uav_cam->setTexture(driver->getTexture("uav_cam.png"));

    // load fonts
    device->getFileSystem()->addZipFileArchive(FONTS_ZIP.c_str());
    IGUIFont * font_large = guienv->getFont(FONT_LARGE.c_str());
    IGUIFont * font_small = guienv->getFont(FONT_SMALL.c_str());
    stringc err = "Failed to load font";
    if(!(font_large && font_small))throw Error(err.c_str());

    // we need two statictexts with fonts set because you can't set
    // the font when you're in RTT mode

    // bigger text for the callsign
    TEXT = guienv->addStaticText(L"",rect<s32>(0,0,0,0),false, false);
    TEXT->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    TEXT->setOverrideColor(color::COLOR_WHITE);
    TEXT->setOverrideFont(font_large);

    // smaller text
    text = guienv->addStaticText(L"",rect<s32>(0,0,0,0),false, false);
    text->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    text->setOverrideColor(color::COLOR_WHITE);
    text->setOverrideFont(font_small);
}

void UAVConsole::set_id(IrrlichtDevice * device, s32 id_, UAVObject * uav_)
{
    id = id_;
    uav = uav_;

    IVideoDriver * driver = device->getVideoDriver();
    IGUIEnvironment * guienv = device->getGUIEnvironment();
    IGUIElement * guiElmRoot = guienv->getRootGUIElement();

    position2d<s32> cons_pos(0, id * HUD_UAV_CONS_Y);
    render_image = new GUIImage(rect<s32>(0,0,256,128), device, guiElmRoot);
    render_image->setPosition(cons_pos);

    if(driver->queryFeature(EVDF_RENDER_TO_TARGET))
    {
        stringc name = "RT";
        name += id;
        render = driver->addRenderTargetTexture(dimension2d<s32>(256,128), name.c_str());
        render_image->setTexture(render);
    }
    else
    {
        cout << "RTT not supported" << endl;
    }

    load_buttons();
}

void UAVConsole::load_buttons()
{
    gps_button = new UAVButton(
        rect2di(
            UAV_GPS_START_X,
            id * HUD_UAV_CONS_Y + UAV_GPS_START_Y,
            UAV_GPS_END_X,
            id * HUD_UAV_CONS_Y + UAV_GPS_END_Y),
        0);

    cam_button = new UAVButton(
        rect2di(
            UAV_CAM_START_X,
            id * HUD_UAV_CONS_Y + UAV_CAM_START_Y,
            UAV_CAM_END_X,
            id * HUD_UAV_CONS_Y + UAV_CAM_END_Y),
        0);
}

void UAVConsole::render_to_texture(const NavWindow * win)
{
    if(need_render)
    {
        need_render = false;

        IVideoDriver * driver = win->driver();
        if(driver->queryFeature(EVDF_RENDER_TO_TARGET))
        {   
            // set the render target and draw the console, then set it back
            driver->setRenderTarget(render,true,true,SColor(0,0,0,255));
            draw_console(driver);
            driver->setRenderTarget(0,true,true,0);

            // set the viewport back to the full size
            rect<s32> viewport(0,0,win->windowWidth(),win->windowHeight());
            driver->setViewPort(viewport);
        }
    }
}

void UAVConsole::draw_console(IVideoDriver * driver, bool to_texture)
{
    // get the console box position
    position2di cons_pos;
    if(to_texture)  cons_pos = position2di(0,0);
    else            cons_pos = position2di(0, id * HUD_UAV_CONS_Y);

    uav_box->setPosition(cons_pos);
    uav_box->draw();

    // if this console doesn't have a UAV assigned, gray it out and return
    if(!uav)
    {
        rect<s32> cons_rect(
            cons_pos.X,
            cons_pos.Y,
            cons_pos.X + HUD_UAV_CONS_X,
            cons_pos.Y + HUD_UAV_CONS_Y);
        GUIImage::draw2DRectangle(driver, cons_rect, SColor(200,0,0,0));
        return;
    }

    // callsign image
    uav_callsign->setPosition(cons_pos);
    uav_callsign->setColor(uav->getColor());
    uav_callsign->draw();

    // fuel background
    uav_fuel_back->setPosition(cons_pos);
    uav_fuel_back->draw();

    // gps indicator
    SColor gps_color = color::COLOR_GREEN;
    if(uav->hasGPSFail()) gps_color = color::COLOR_RED;
    uav_gps->setPosition(cons_pos);
    uav_gps->setColor(gps_color);
    uav_gps->draw();

    // camera indicator
    SColor cam_color = color::COLOR_GREEN;
    if(uav->hasCamFail()) cam_color = color::COLOR_RED;
    uav_cam->setPosition(cons_pos);
    uav_cam->setColor(cam_color);
    uav_cam->draw();

    // callsign text
    rect<s32> text_rect(
        UAV_NAME_START_X,
        cons_pos.Y + UAV_NAME_START_Y,
        UAV_NAME_END_X,
        cons_pos.Y + UAV_NAME_END_Y);
    TEXT->setText(uav->getName().c_str());
    TEXT->setRelativePosition(text_rect);
    TEXT->draw();

    stringw details;

    // fuel
    text_rect = rect<s32>(
        UAV_FUEL_START_X,
        cons_pos.Y + UAV_FUEL_START_Y,
        UAV_FUEL_END_X,
        cons_pos.Y + UAV_FUEL_END_Y);
    details = "Fuel:";
    text->setText(details.c_str());
    text->setRelativePosition(text_rect);
    text->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);
    text->draw();

    // gps text
    text_rect = rect<s32>(
        UAV_GPS_START_X,
        cons_pos.Y + UAV_GPS_START_Y,
        UAV_GPS_END_X,
        cons_pos.Y + UAV_GPS_END_Y);
    details = "GPS";
    text->setText(details.c_str());
    text->setRelativePosition(text_rect);
    text->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    text->draw();

    // camera text
    text_rect = rect<s32>(
        UAV_CAM_START_X,
        cons_pos.Y + UAV_CAM_START_Y,
        UAV_CAM_END_X,
        cons_pos.Y + UAV_CAM_END_Y);
    details = "Camera";
    text->setText(details.c_str());
    text->setRelativePosition(text_rect);
    text->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    text->draw();
}

void UAVConsole::draw(IrrlichtDevice * device)
{
    IVideoDriver * driver = device->getVideoDriver();
    if(driver->queryFeature(EVDF_RENDER_TO_TARGET))
    {
        render_image->draw();
    }
    else
    {
        draw_console(driver, false);
    }

    position2di cons_pos(0, id * HUD_UAV_CONS_Y);

    if(uav)
    {
        // fuel level
        f32 cur_fuel = uav->getFuel();
        if(cur_fuel > 0.01)
        {
            SColor fuel_color = color::COLOR_GREEN;
            if(cur_fuel < .45)
                fuel_color = color::COLOR_RED;
            // fuel level rectange
            rect<s32> fuel_rect(
                FUEL_START_X,
                id * HUD_UAV_CONS_Y + FUEL_START_Y,
                s32(FUEL_END_X - f32(FUEL_END_X - FUEL_START_X) * (1.f - cur_fuel)),
                id * HUD_UAV_CONS_Y + FUEL_END_Y);
            GUIImage::draw2DRectangle(driver, fuel_rect, fuel_color);
        }

        // fuel outline
        uav_fuel_out->setPosition(cons_pos);
        uav_fuel_out->draw();

        // estimated time of arrival text
        stringw details;
        details = "ET: ";
        if(uav->getWPList()->empty() && uav->atBase())
            details += "-";
        else
        {
            f32 dist = uav->getPosition().getDistanceFrom(uav->getTarget());
            s32 time = s32(dist / SPEED_FACTOR);
            if(time < 0) time = 0; // just in case
            details += stringw(time) + stringw("sec");
        }

        rect<s32> text_rect(
            UAV_NTET_START_X,
            id * HUD_UAV_CONS_Y + UAV_NTET_START_Y,
            UAV_NTET_END_X,
            id * HUD_UAV_CONS_Y + UAV_NTET_END_Y);

        text->setRelativePosition(text_rect);
        text->setText(details.c_str());
        text->setTextAlignment(EGUIA_LOWERRIGHT, EGUIA_CENTER);
        text->draw();

        // next target text
        text_rect = rect<s32>(
            UAV_NTET_START_X,
            cons_pos.Y + UAV_NTET_START_Y,
            UAV_NTET_END_X,
            cons_pos.Y + UAV_NTET_END_Y);
        details = "NT: ";
        if(uav->goingToBase())              details += uav->getBase()->getName();
        else if(!uav->getWPList()->empty()) details += uav->getWPList()->front()->getName();
        else                                details += "-";
        text->setText(details.c_str());
        text->setRelativePosition(text_rect);
        text->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);
        text->draw();
    }
}