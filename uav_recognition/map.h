#ifndef MAP_H
#define MAP_H

#include "image.h"
#include "sim_object.h"
#include "waypoint.h"
#include "ufo.h"
#include "uav.h"

#include <irrlicht.h>

enum E_ICON_TYPE {ICON_UAV = 0, ICON_WAYPOINT, ICON_UFO, ICON_BASE, ICON_UAV_START};

class UAVMap {
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
	void draw_ext_uav_path(UAVObject * obj, irr::video::IVideoDriver * driver);

	void setWorldSize(irr::core::dimension2di size);
	void setOrigin(irr::s32 posX, irr::s32 posY);
	void setOriginRel(irr::f32 posX, irr::f32 posY);
	void setScale(irr::f32 scale);
	void setZoom(irr::f32 zoom_amount);	// adds zoom_amount to zoom

	bool isMouseOverMap(irr::core::position2di cursor);
	void moveMapRel(irr::core::position2di distance);

	irr::core::position2di worldToMapCoords(irr::core::position2df coords);
	irr::core::position2di mouseToMapCoords(irr::core::position2di cursor);

private:
	void set_image_positions();
	bool is_visible(pos2d position) const;

	irr::f32 getCurrentMapWidth() const;
	irr::f32 getCurrentMapHeight() const;
	irr::core::rect<irr::s32> getCurrentMapArea() const;

	/* State */
	irr::core::rect<irr::s32> drawArea;	// where the map will be drawn
	irr::core::dimension2di drawSize;	// dimensions of the drawArea
	irr::core::dimension2di world;		// world size
	irr::core::position2di origin;		// bottom-left corner of current view
	irr::f32 zoom;						// width to show
	irr::f32 max_zoom;					// max zoom in distance
	irr::f32 min_zoom;					// max zoom out distance (defined by image)

	/* Interface */
	// map image
	irr::video::ITexture * map_image;
	irr::s32 map_width;
	irr::s32 map_height;

	// object icons
	GUIImage * uav;
	GUIImage * waypoint;
	GUIImage * base;
	GUIImage * ufo;
};

#endif /* MAP_H */