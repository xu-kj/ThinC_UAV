#ifndef UAV_MAP_H
#define UAV_MAP_H

#include <irrlicht.h>
#include "Image.h"
#include "SimObject.h"
#include "Waypoint.h"
#include "UFO.h"
#include "UAV.h"

enum E_ICON_TYPE {ICON_UAV = 0, ICON_WAYPOINT, ICON_UFO, ICON_BASE};

class UAVMap
{
public:
	UAVMap();

	void load_images(irr::IrrlichtDevice * device);
	void draw(irr::video::IVideoDriver * driver);
	void draw_interface(irr::video::IVideoDriver * driver);
	void draw_text(const pos2d &pos, irr::video::SColor color,
		const irr::core::stringw &text,	irr::gui::IGUIFont * font);
	void draw_icon(const pos2d &pos, irr::video::SColor color,
		E_ICON_TYPE icon, irr::f32 rotation = 0.f);
	void draw_uav_path(UAVObject * obj, irr::video::IVideoDriver * driver);


	void draw_waypoint(WaypointObject * wp, irr::gui::IGUIStaticText * small_text);
	void draw_base(SimObject * obj);
	void draw_uav(UAVObject * obj, irr::video::IVideoDriver * driver);
	void draw_ufo(UFOObject * obj, irr::video::IVideoDriver * driver);

	void setArea(irr::core::rect<irr::f32> area_);
	void setOrigin(irr::f32 latitude, irr::f32 longitude);
	void setOriginRel(irr::f32 latitude, irr::f32 longitude);
	void setScale(irr::f32 pixels_per_degree);
	void setZoom(irr::f32 zoom_amount);	// adds zoom_amount to zoom

	bool isMouseOverMap(irr::core::position2di cursor);
	void moveMapRel(irr::core::position2di distance);

private:
	void set_image_positions();
	bool is_visible(pos2d position) const;

	irr::f32 getMapLong() const;
	irr::f32 getMapLat() const;

	irr::core::rect<irr::s32> getMapPixels() const;
	pos2d getPixelsFromDegrees(pos2d degrees) const;

	/* State */
	irr::core::rect<irr::f32> area;	// rectange in degrees long/lat
	irr::core::position2df origin;	// bottom-left corner to be viewed in degrees long/lat
	irr::f32 zoom;					// number of degrees longitude to show
	irr::f32 max_zoom;				// max zoom in distance
	irr::f32 min_zoom;				// max zoom out distance (defined by image)

	/* Interface */
	// map image
	GUIImage * map;
	irr::video::ITexture * map_image;

	// corners
	GUIImage * corner1;	// upper left
	GUIImage * corner2;	// lower left
	GUIImage * corner3;	// lower right
	GUIImage * corner4;	// upper right

	// sides
	GUIImage * top;
	GUIImage * bottom;
	GUIImage * left;
	GUIImage * right;

	// object icons
	GUIImage * uav;
	GUIImage * waypoint;
	GUIImage * base;
	GUIImage * ufo;
};

#endif