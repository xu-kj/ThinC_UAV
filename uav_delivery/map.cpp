#include "map.h"
#include "color.h"
#include "globals.h"

#include <iostream>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

irr::s32 MAP_ICON_SIZE = 32;
bool SHOW_BASE_TEXT = false;
bool SHOW_WAYPOINT_TEXT = true;
bool SHOW_UAV_TEXT = false;
bool SHOW_UAV_LINES = true;
bool SHOW_EXT_UAV_LINES = true;
bool SHOW_UFO_TEXT = true;
bool SHOW_UAV_ICONS = true;
bool SHOW_WAYPOINT_ICONS = true;
bool SHOW_BASE_ICONS = true;
bool SHOW_UFO_ICONS = true;
bool SHOW_UAV_START_ICONS = true;

UAVMap::UAVMap() {
    world = dimension2di(
        s32((f32)CITY_SIZE_X*(f32)CITY_SECTOR_SIZE*CITY_BUILDING_SIZE),
        s32((f32)CITY_SIZE_Y*(f32)CITY_SECTOR_SIZE*CITY_BUILDING_SIZE));

    drawArea = rect<s32>(
        MAP_START_X,
        MAP_START_Y,
        MAP_END_X,
        MAP_END_Y);

    drawSize = dimension2di(
        MAP_END_X - MAP_START_X,
        MAP_END_Y - MAP_START_Y);
}

void UAVMap::load_images(IrrlichtDevice * device)
{
    IVideoDriver * driver = device->getVideoDriver();
    IGUIEnvironment * guienv = device->getGUIEnvironment();
    IGUIElement * guiElmRoot = guienv->getRootGUIElement();

    device->getFileSystem()->addZipFileArchive(MAP_ZIP.c_str());
    device->getFileSystem()->addZipFileArchive(OBJ_ICONS_ZIP.c_str());

    // map
    map_image = driver->getTexture("map.png");
    set_image_positions();

    // object icons
    uav = new GUIImage(rect<s32>(0,0,32,32), device, guiElmRoot);
    uav->setTexture(driver->getTexture("uav.png"));
    uav->setBilinearFilter(true);

    waypoint = new GUIImage(rect<s32>(0,0,32,32), device, guiElmRoot);
    waypoint->setTexture(driver->getTexture("waypoint.png"));

    base = new GUIImage(rect<s32>(0,0,32,32), device, guiElmRoot);
    base->setTexture(driver->getTexture("base.png"));

    ufo = new GUIImage(rect<s32>(0,0,32,32), device, guiElmRoot);
    ufo->setTexture(driver->getTexture("ufo.png"));
}

void UAVMap::draw(IVideoDriver * driver)
{
    GUIImage::draw(driver, map_image, map_image->getSize(), getCurrentMapArea(), drawArea);
}

void UAVMap::draw_text(const pos2d &pos, SColor color, const stringw &text,
    IGUIFont * font)
{
    if(is_visible(pos))
    {
        position2di pixels = worldToMapCoords(pos);
        rect<s32> text_rect(
            MAP_START_X + s32(pixels.X) - MAP_ICON_SIZE,
            MAP_START_Y + s32(pixels.Y),
            MAP_START_X + s32(pixels.X) + MAP_ICON_SIZE,
            MAP_START_Y + s32(pixels.Y) + MAP_ICON_SIZE);

        font->draw(text.c_str(), text_rect, color, true, true, &text_rect);
    }
}

void UAVMap::draw_icon(const pos2d &pos, SColor color, E_ICON_TYPE icon, f32 rotation)
{
    if(is_visible(pos))
    {
        GUIImage * image;
        switch(icon)
        {
        case ICON_UAV:      image = uav; break;
        case ICON_WAYPOINT: image = waypoint; break;
        case ICON_UFO:      image = ufo; break;
        case ICON_BASE:     image = base; break;
        case ICON_UAV_START:image = base; break;
        default:            image = 0;
        }

        if(image)
        {
            position2di pixels = worldToMapCoords(pos);
            position2di image_pos(
                MAP_START_X + s32(pixels.X) - MAP_ICON_SIZE / 2,
                MAP_START_Y + s32(pixels.Y) - MAP_ICON_SIZE / 2);
            image->setPosition(image_pos);
            image->setColor(color);
            image->setRotation(rotation);
            image->draw();
        }
    }
}

void UAVMap::draw_uav_path(UAVObject * obj, IVideoDriver * driver)
{
    if(SHOW_EXT_UAV_LINES)
        draw_ext_uav_path(obj, driver);

    // get the position of the UAV
    position2di line_start = worldToMapCoords(obj->getPosition2D());
    position2di line_end;
    position2di uav_pos = line_start;

    // draw lines between each of the UAV's waypoints
    const std::list<WaypointObject *> * wps = obj->getWPList();
    std::list<WaypointObject *>::const_iterator wp = wps->begin();
    while(wp != wps->end())
    {
        line_end = worldToMapCoords((*wp)->getPosition2D());
        position2di start(
            MAP_START_X + s32(line_start.X),
            MAP_START_Y + s32(line_start.Y));
        position2di end(
            MAP_START_X + s32(line_end.X),
            MAP_START_Y + s32(line_end.Y));
        driver->draw2DLine(start, end, obj->getColor());

        line_start = line_end;
        wp++;
    }

    // and the line from the last waypoint to the base
    if(obj->getBase())
    {
        line_end = worldToMapCoords(obj->getBase()->getPosition2D());
            position2di start(
                MAP_START_X + s32(line_start.X),
                MAP_START_Y + s32(line_start.Y));
            position2di end(
                MAP_START_X + s32(line_end.X),
                MAP_START_Y + s32(line_end.Y));
        driver->draw2DLine(start, end, obj->getColor());
    }
}

void UAVMap::draw_ext_uav_path(UAVObject * obj, IVideoDriver * driver)
{
    // get the position of the UAV's start point
    position2di line_start = worldToMapCoords(obj->getStartPosition2D());
    position2di line_end;
    position2di uav_pos = line_start;

    // draw lines between each of the UAV's completed waypoints
    const std::list<WaypointObject *> * wps = obj->getDoneWPList();
    std::list<WaypointObject *>::const_iterator wp = wps->begin();
    while(wp != wps->end())
    {
        line_end = worldToMapCoords((*wp)->getPosition2D());
        position2di start(
            MAP_START_X + s32(line_start.X),
            MAP_START_Y + s32(line_start.Y));
        position2di end(
            MAP_START_X + s32(line_end.X),
            MAP_START_Y + s32(line_end.Y));

        SColor color = obj->getColor();
        color.setAlpha(125);
        driver->draw2DLine(start, end, color);

        line_start = line_end;
        wp++;
    }

    // and the line from the last waypoint to the UAV
    line_end = worldToMapCoords(obj->getPosition2D());
    position2di start(
        MAP_START_X + s32(line_start.X),
        MAP_START_Y + s32(line_start.Y));
    position2di end(
        MAP_START_X + s32(line_end.X),
        MAP_START_Y + s32(line_end.Y));
    SColor color = obj->getColor();
    color.setAlpha(125);
    driver->draw2DLine(start, end, color);
}


void UAVMap::draw_interface(IVideoDriver * driver)
{
    // draw a black rectange on the bottom of the screen to prevent bleed from
    // the map objects (lines)
    rect<s32> pos1(0,RES_Y-3,RES_X,RES_Y);
    GUIImage::draw2DRectangle(driver, pos1, color::COLOR_BLACK);

    // draw a black rectange on the side to prevent bleed
    rect<s32> pos2(0,0,MAP_START_X,RES_Y);
    GUIImage::draw2DRectangle(driver, pos2, color::COLOR_BLACK);
}

void UAVMap::setWorldSize(dimension2di size)
{
    world = size;
}

void UAVMap::setOrigin(s32 posX, s32 posY)
{
    /* The origin's UpperLeftCorner is ACTUALLY the lower left corner
       and the LowerRightCorner is ACTUALLY the upper right corner. */

    /*
    // test the lower bound
    if(posX < area.UpperLeftCorner.X)   posY = area.UpperLeftCorner.X;
    if(posY< area.UpperLeftCorner.Y)    posX = area.UpperLeftCorner.Y;

    // get the map's current size
    f32 map_width = getCurrentMapWidth();
    f32 map_height = getCurrentMapHeight();

    // find the upper bound for placement of the origin
    s32 upper_bound_x = s32(area.LowerRightCorner.X - map_width);
    s32 upper_bound_y = s32(area.LowerRightCorner.Y - map_height);

    // bottom corner
    if(posX > upper_bound_x)    posX = upper_bound_x;
    if(posY > upper_bound_y)    posY = upper_bound_y;
    */

    // and finally set the origin
    origin = position2di(posX, posY);
}

void UAVMap::setOriginRel(f32 posX, f32 posY)
{
    posX += origin.X;
    posY += origin.Y;
    setOrigin((s32)posX, (s32)posY);
}

void UAVMap::setZoom(f32 zoom_amount)
{
    f32 old_zoom = zoom;

    // apply the zoom
    zoom += zoom_amount;
    if(zoom > max_zoom)
        zoom = max_zoom;
    if(zoom < min_zoom)
        zoom = min_zoom;
    
    // center the zoom a bit
    f32 off_x = 0.f;
    f32 off_y = 0.f;
    if(old_zoom != zoom)
    {
        f32 factor_y = f32(MAP_END_Y - MAP_START_Y) / f32(MAP_END_X - MAP_START_X);
        off_x = -(zoom_amount / 2);
        off_y = -((factor_y * zoom_amount) / 2);
    }

    // don't let the map wander out of range
    // (and apply the origin offset)
    setOrigin(
        s32(origin.Y + off_y),
        s32(origin.X + off_x));
}

bool UAVMap::isMouseOverMap(position2di cursor)
{
    if(cursor.X > MAP_START_X &&
        cursor.X < MAP_END_X &&
        cursor.Y > MAP_START_Y &&
        cursor.Y < MAP_END_Y)
    {
        return true;
    }
    return false;
}

void UAVMap::moveMapRel(position2di distance)
{
    // use factor to convert the pixels moved into full-scale pixels
    f32 factor = zoom / f32(MAP_END_X - MAP_START_X);
    setOriginRel(distance.Y * factor, -(distance.X * factor));
}

void UAVMap::set_image_positions()
{
}

bool UAVMap::is_visible(pos2d position) const
{
    return true;
}

f32 UAVMap::getCurrentMapWidth() const
{
    return zoom;
}

f32 UAVMap::getCurrentMapHeight() const
{
    // get the factor we'll use to find the map's full height
    // (zoom is number of degrees longitude in the map's width)
    f32 map_factor_x = 1.f; // just to help people understand what's going on
    f32 map_factor_y = f32(MAP_END_Y - MAP_START_Y) / f32(MAP_END_X - MAP_START_X);

    // get the map's width and height
    f32 map_width = zoom;
    f32 map_height = map_width * map_factor_y;

    return map_height;
}

rect<s32> UAVMap::getCurrentMapArea() const
{
    // later this should return the portion of the map that's visible in map pixels,
    // not the whole thing
    return rect<s32>(0,0,map_image->getSize().Width, map_image->getSize().Height);
}

position2di UAVMap::worldToMapCoords(position2df coords)
{
    // convert
    position2di map(
        s32(coords.X * ((f32)drawSize.Width) / (f32)world.Width),
        s32(coords.Y * ((f32)drawSize.Height) / (f32)world.Height));

    // now flip it vertically >:O
    map.Y = drawSize.Height - map.Y;

    return map;
}

position2di UAVMap::mouseToMapCoords(position2di cursor)
{
    return position2di(
        cursor.X - MAP_START_X,
        cursor.Y - MAP_START_Y);
}