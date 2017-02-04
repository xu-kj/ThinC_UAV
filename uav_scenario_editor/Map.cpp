#include "Map.h"
#include "Globals.h"

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
bool SHOW_UFO_TEXT = true;
bool SHOW_UAV_ICONS = true;
bool SHOW_WAYPOINT_ICONS = true;
bool SHOW_BASE_ICONS = true;
bool SHOW_UFO_ICONS = true;

UAVMap::UAVMap()
	: zoom(10.f), min_zoom(1.f), max_zoom(10.f),
	area(0,0,10,10)
{
	setOrigin(0.f, 0.f);
}

void UAVMap::load_images(IrrlichtDevice * device)
{
	IVideoDriver * driver = device->getVideoDriver();
	IGUIEnvironment * guienv = device->getGUIEnvironment();
	IGUIElement * guiElmRoot = guienv->getRootGUIElement();

	device->getFileSystem()->addZipFileArchive(MAP_ZIP.c_str());
	device->getFileSystem()->addZipFileArchive(OBJ_ICONS_ZIP.c_str());

	if(MAP_BORDER_WIDTH > 0)
	{
		corner1 = new GUIImage(rect<s32>(0,0,MAP_BORDER_IMG_SIZE,MAP_BORDER_IMG_SIZE), device, guiElmRoot);
		corner1->setTexture(driver->getTexture("corner1.png"));

		corner2 = new GUIImage(rect<s32>(0,0,MAP_BORDER_IMG_SIZE,MAP_BORDER_IMG_SIZE), device, guiElmRoot);
		corner2->setTexture(driver->getTexture("corner2.png"));

		corner3 = new GUIImage(rect<s32>(0,0,MAP_BORDER_IMG_SIZE,MAP_BORDER_IMG_SIZE), device, guiElmRoot);
		corner3->setTexture(driver->getTexture("corner3.png"));

		corner4 = new GUIImage(rect<s32>(0,0,MAP_BORDER_IMG_SIZE,MAP_BORDER_IMG_SIZE), device, guiElmRoot);
		corner4->setTexture(driver->getTexture("corner4.png"));

		top = new GUIImage(rect<s32>(0,0,MAP_BORDER_IMG_SIZE,MAP_BORDER_IMG_SIZE), device, guiElmRoot);
		top->setTexture(driver->getTexture("top.png"));

		bottom = new GUIImage(rect<s32>(0,0,MAP_BORDER_IMG_SIZE,MAP_BORDER_IMG_SIZE), device, guiElmRoot);
		bottom->setTexture(driver->getTexture("bottom.png"));

		left = new GUIImage(rect<s32>(0,0,MAP_BORDER_IMG_SIZE,MAP_BORDER_IMG_SIZE), device, guiElmRoot);
		left->setTexture(driver->getTexture("left.png"));

		right = new GUIImage(rect<s32>(0,0,MAP_BORDER_IMG_SIZE,MAP_BORDER_IMG_SIZE), device, guiElmRoot);
		right->setTexture(driver->getTexture("right.png"));
	}

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
	rect<s32> map_source(getMapPixels());
	rect<s32> map_dest(
		MAP_START_X,
		MAP_START_Y,
		MAP_END_X,
		MAP_END_Y);
	GUIImage::draw(driver, map_image, map_image->getSize(), map_source, map_dest);
}

void UAVMap::draw_text(const pos2d &pos, SColor color, const stringw &text,
	IGUIFont * font)
{
	if(is_visible(pos))
	{
		pos2d pixels = getPixelsFromDegrees(pos);

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
		case ICON_UAV:		image = uav; break;
		case ICON_WAYPOINT:	image = waypoint; break;
		case ICON_UFO:		image = ufo; break;
		case ICON_BASE:		image = base; break;
		default:			image = 0;
		}

		if(image)
		{
			pos2d pixels = getPixelsFromDegrees(pos);
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

void UAVMap::draw_waypoint(WaypointObject * wp, IGUIStaticText * small_text)
{
	if(is_visible(wp->getPosition2D()))
	{
		// get the pixel position on the current map
		pos2d pixels = getPixelsFromDegrees(wp->getPosition2D());

		// draw the waypoint image
		position2di wp_pos(
			MAP_START_X + s32(pixels.X) - 16,
			MAP_START_Y + s32(pixels.Y) - 16);
		waypoint->setPosition(wp_pos);
		waypoint->setColor(wp->getColor());
		waypoint->draw();

		// waypoint name
		rect<s32> text_rect(
			MAP_START_X + s32(pixels.X) - 50,
			MAP_START_Y + s32(pixels.Y),
			MAP_START_X + s32(pixels.X) + 50,
			MAP_START_Y + s32(pixels.Y) + 30);
		small_text->setText(wp->getName().c_str());
		small_text->setRelativePosition(text_rect);
		small_text->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
		small_text->draw();
	}
}

void UAVMap::draw_uav_path(UAVObject * obj, IVideoDriver * driver)
{
	// get the position of the UAV
	pos2d line_start = getPixelsFromDegrees(obj->getPosition2D());
	pos2d line_end;
	pos2d uav_pos = line_start;

	// draw lines between each of the UAV's waypoints
	const std::list<WaypointObject *> * wps = obj->getWPList();
	std::list<WaypointObject *>::const_iterator wp = wps->begin();
	while(wp != wps->end())
	{
		line_end = getPixelsFromDegrees((*wp)->getPosition2D());
		position2di start(
			MAP_START_X + s32(line_start.X),
			MAP_START_Y + s32(line_start.Y));
		position2di end(
			MAP_START_X + s32(line_end.X),
			MAP_START_Y + s32(line_end.Y));
		driver->draw2DLine(start, end, obj->getColor());

		/*
		irr::video::SColor color(255,255,255,255);
		irr::scene::SMeshBuffer mesh;
		mesh.Vertices.set_used(4);
		mesh.Indices.set_used(6);
		mesh.Vertices[0] = S3DVertex(start.X, start.Y, 0.f, 0.f, 0.f, 1.f, color, 0.f, 1.f);
		mesh.Vertices[1] = S3DVertex(start.X, end.Y,   0.f, 0.f, 0.f, 1.f, color, 0.f, 0.f);
		mesh.Vertices[2] = S3DVertex(end.X,   end.Y,   0.f, 0.f, 0.f, 1.f, color, 1.0f, 0.f);
		mesh.Vertices[3] = S3DVertex(end.X,   start.Y, 0.f, 0.f, 0.f, 1.f, color, 1.0f, 1.f);
		mesh.Indices[0] = 0;
		mesh.Indices[1] = 1;
		mesh.Indices[2] = 2;
		mesh.Indices[3] = 2;
		mesh.Indices[4] = 3;
		mesh.Indices[5] = 0;
		mesh.getMaterial().Lighting = false;
		// store the old view port and set the new one
		rect<s32> oldViewPort = driver->getViewPort();
		rect<s32> newViewPort = getViewPort();
		driver->setViewPort(newViewPort);
		// clear the projection matrix
		matrix4 oldProjMat = driver->getTransform(ETS_PROJECTION);
		driver->setTransform(ETS_PROJECTION, matrix4());
		// clear the view and draw matrix
		matrix4 oldViewMat = driver->getTransform(ETS_VIEW);
		driver->setTransform(ETS_VIEW, matrix4());
		driver->setTransform(ETS_WORLD, matrix);

		driver->setMaterial(mesh.Material);
		driver->drawMeshBuffer(&mesh);
		// restore view matrix
		driver->setTransform(ETS_VIEW, oldViewMat);
		// restore projection matrix
		driver->setTransform(ETS_PROJECTION, oldProjMat);
		// restore the view area
		driver->setViewPort(oldViewPort);
		*/

		line_start = line_end;
		wp++;
	}

	// and the line from the last waypoint to the base
	if(obj->getBase())
	{
		line_end = getPixelsFromDegrees(obj->getBase()->getPosition2D());
			position2di start(
				MAP_START_X + s32(line_start.X),
				MAP_START_Y + s32(line_start.Y));
			position2di end(
				MAP_START_X + s32(line_end.X),
				MAP_START_Y + s32(line_end.Y));
		driver->draw2DLine(start, end, obj->getColor());
	}
}

void UAVMap::draw_base(SimObject * obj)
{
	if(is_visible(obj->getPosition2D()))
	{
		// get the pixel position on the current map
		pos2d pixels = getPixelsFromDegrees(obj->getPosition2D());

		// draw the base image
		position2di pos(
			MAP_START_X + s32(pixels.X) - 16,
			MAP_START_Y + s32(pixels.Y) - 16);
		base->setPosition(pos);
		base->setColor(obj->getColor());
		base->draw();
	}
}

void UAVMap::draw_uav(UAVObject * obj, IVideoDriver * driver)
{
	// get the position of the UAV
	pos2d line_start = getPixelsFromDegrees(obj->getPosition2D());
	pos2d line_end;
	pos2d uav_pos = line_start;

	// draw lines between each of the UAV's waypoints
	const std::list<WaypointObject *> * wps = obj->getWPList();
	std::list<WaypointObject *>::const_iterator wp = wps->begin();
	while(wp != wps->end())
	{
		line_end = getPixelsFromDegrees((*wp)->getPosition2D());
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
		line_end = getPixelsFromDegrees(obj->getBase()->getPosition2D());
			position2di start(
				MAP_START_X + s32(line_start.X),
				MAP_START_Y + s32(line_start.Y));
			position2di end(
				MAP_START_X + s32(line_end.X),
				MAP_START_Y + s32(line_end.Y));
		driver->draw2DLine(start, end, obj->getColor());
	}

	// draw the UAV if it's on the map
	if(is_visible(obj->getPosition2D()))
	{
		position2di pos(
			MAP_START_X + s32(uav_pos.X) - 16,
			MAP_START_Y + s32(uav_pos.Y) - 16);
		uav->setRotation(obj->getRotAngle2D());
		uav->setPosition(pos);
		uav->setColor(obj->getColor());
		uav->draw();
	}
}

void UAVMap::draw_ufo(UFOObject * obj, IVideoDriver * driver)
{
	if(is_visible(obj->getPosition2D()))
	{
		// get the pixel position on the current map
		pos2d pixels = getPixelsFromDegrees(obj->getPosition2D());

		// draw the base image
		position2di pos(
			MAP_START_X + s32(pixels.X) - 16,
			MAP_START_Y + s32(pixels.Y) - 16);
		ufo->setPosition(pos);
		ufo->setColor(obj->getColor());
		ufo->draw();
	}
}

void UAVMap::draw_interface(IVideoDriver * driver)
{
	if(MAP_BORDER_WIDTH > 0)
	{
		corner1->draw();
		corner2->draw();
		corner3->draw();
		corner4->draw();

		top->draw();
		left->draw();
		right->draw();
		bottom->draw();
	}

	// draw a black rectange on the bottom of the screen to prevent bleed from
	// the map objects (lines)
	rect<s32> pos1(0,RES_Y-3,RES_X,RES_Y);
	GUIImage::draw2DRectangle(driver, pos1, COLOR_BLACK);

	// draw a black rectange on the side to prevent bleed
	rect<s32> pos2(0,0,MAP_START_X,RES_Y);
	GUIImage::draw2DRectangle(driver, pos2, COLOR_BLACK);
}

void UAVMap::setArea(rect<f32> area_)
{
	area = area_;
}

void UAVMap::setOrigin(f32 latitude, f32 longitude)
{
	/* The origin's UpperLeftCorner is ACTUALLY the lower left corner
	   and the LowerRightCorner is ACTUALLY the upper right corner. */

	// test the lower bound
	if(longitude < area.UpperLeftCorner.X)	longitude = area.UpperLeftCorner.X;
	if(latitude < area.UpperLeftCorner.Y)	latitude = area.UpperLeftCorner.Y;

	// get the map's longitude and latitude
	f32 map_long = getMapLong();
	f32 map_lat = getMapLat();

	// find the upper bound for placement of the origin
	f32 upper_bound_x = area.LowerRightCorner.X - map_long;
	f32 upper_bound_y = area.LowerRightCorner.Y - map_lat;

	// bottom corner
	if(longitude > upper_bound_x)	longitude = upper_bound_x;
	if(latitude > upper_bound_y)	latitude = upper_bound_y;

	// and finally set it
	origin = position2df(longitude, latitude);
}

void UAVMap::setOriginRel(f32 latitude, f32 longitude)
{
	longitude += origin.X;
	latitude += origin.Y;
	setOrigin(latitude, longitude);
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
	setOrigin(origin.Y + off_y, origin.X + off_x);
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
	// use factor to convert the pixels moved into latitude/longitude
	f32 factor = zoom / f32(MAP_END_X - MAP_START_X);
	setOriginRel(distance.Y * factor, -(distance.X * factor));
}

void UAVMap::set_image_positions()
{
	if(MAP_BORDER_WIDTH > 0)
	{
		corner1->setPosition(position2d<s32>(MAP_START_X - MAP_BORDER_WIDTH, MAP_START_Y - MAP_BORDER_WIDTH));
		corner2->setPosition(position2d<s32>(MAP_START_X - MAP_BORDER_WIDTH, MAP_END_Y));
		corner3->setPosition(position2d<s32>(MAP_END_X, MAP_END_Y));
		corner4->setPosition(position2d<s32>(MAP_END_X, MAP_START_Y - MAP_BORDER_WIDTH));

		f32 scale_width = f32(MAP_END_X - MAP_START_X) / f32(MAP_BORDER_IMG_SIZE);
		f32 scale_height = f32(MAP_END_Y - MAP_START_Y) / f32(MAP_BORDER_IMG_SIZE);

		top->setPosition(position2d<s32>(MAP_START_X, MAP_START_Y - MAP_BORDER_WIDTH));
		top->setScale(scale_width,1.f);
		top->setScaleImageOnly(true);

		bottom->setPosition(position2d<s32>(MAP_START_X, MAP_END_Y));
		bottom->setScale(scale_width,1.f);
		bottom->setScaleImageOnly(true);

		left->setPosition(position2d<s32>(MAP_START_X - MAP_BORDER_WIDTH, MAP_START_Y));
		left->setScale(1.f,scale_height);
		left->setScaleImageOnly(true);

		right->setPosition(position2d<s32>(MAP_END_X, MAP_START_Y));
		right->setScale(1.f,scale_height);
		right->setScaleImageOnly(true);
	}
}

bool UAVMap::is_visible(pos2d position) const
{
	if(position.X < origin.X || position.Y < origin.Y)
		return false;

	if(position.X > origin.X + getMapLong())
		return false;

	if(position.Y > origin.Y + getMapLat())
		return false;

	return true;
}

f32 UAVMap::getMapLong() const
{
	return zoom;
}

f32 UAVMap::getMapLat() const
{
	// get the factor we'll use to find the map's latitude
	// (zoom is number of degrees longitude in the map's width)
	f32 map_factor_x = 1.f; // just to help people understand what's going on
	f32 map_factor_y = f32(MAP_END_Y - MAP_START_Y) / f32(MAP_END_X - MAP_START_X);

	// get the map's longitude and latitude
	f32 map_long = zoom;
	f32 map_lat = map_long * map_factor_y;

	return map_lat;
}

rect<s32> UAVMap::getMapPixels() const
{
	// get the factor we'll use to find the map's latitude
	// (zoom is number of degrees longitude in the map's width)
	f32 map_factor_x = 1.f; // just to help people understand what's going on
	f32 map_factor_y = f32(MAP_END_Y - MAP_START_Y) / f32(MAP_END_X - MAP_START_X);

	// width and height of the current window in degrees
	f32 map_long = zoom;
	f32 map_lat = map_long * map_factor_y;

	// total degrees of lat/long in the area
	f32 area_long = area.LowerRightCorner.X - area.UpperLeftCorner.X;
	f32 area_lat = area.LowerRightCorner.Y - area.UpperLeftCorner.Y;

	// area of the map to draw
	f32 start_long = area_long - (area_long - origin.X);
	f32 start_lat = area_lat - (map_lat + origin.Y);
	f32 end_long = start_long + map_long;
	f32 end_lat = area_lat - origin.Y;

	// start point (upper left of viewing rectangle) as a ratio
	f32 start_x_ratio = start_long / area_long;
	f32 start_y_ratio = start_lat / area_lat;
	f32 end_x_ratio = end_long / area_long;
	f32 end_y_ratio = end_lat / area_lat;

	// and here's the result
	rect<s32> pixels(
		s32(f32(MAP_SIZE_X) * start_x_ratio),
		s32(f32(MAP_SIZE_Y) * start_y_ratio),
		s32(f32(MAP_SIZE_X) * end_x_ratio),
		s32(f32(MAP_SIZE_Y) * end_y_ratio));
	return pixels;
}

pos2d UAVMap::getPixelsFromDegrees(pos2d degrees) const
{
	// total degrees of lat/long in the area
	f32 area_long = area.LowerRightCorner.X - area.UpperLeftCorner.X;
	f32 area_lat = area.LowerRightCorner.Y - area.UpperLeftCorner.Y;

	// position as a ratio
	f32 x_ratio = degrees.X / area_long;
	f32 y_ratio = 1 - (degrees.Y / area_lat);

	// position in pixels on the complete map
	pos2d pixels(f32(MAP_SIZE_X) * x_ratio, f32(MAP_SIZE_Y) * y_ratio);

	// map pixels
	rect<s32> map_pixels(getMapPixels());

	// scale factor
	f32 map_factor_x = f32(map_pixels.LowerRightCorner.X - map_pixels.UpperLeftCorner.X);
	f32 map_factor_y = f32(map_pixels.LowerRightCorner.Y - map_pixels.UpperLeftCorner.Y);
	map_factor_x /= f32(MAP_END_X - MAP_START_X);
	map_factor_y /= f32(MAP_END_Y - MAP_START_Y);
	map_factor_x = 1 / map_factor_x;
	map_factor_y = 1 / map_factor_y;

	// return the position on the current map
	pixels.X -= map_pixels.UpperLeftCorner.X;
	pixels.Y -= map_pixels.UpperLeftCorner.Y;
	pixels.X *= map_factor_x;
	pixels.Y *= map_factor_y;
	return pixels;
}