#pragma once
#ifndef CITY_SCENE_H
#define CITY_SCENE_H

#include <irrlicht.h>
#include "array2d.h"

enum E_CITY_OBJECT_TYPE {ROAD = 0, BUILDING = 1, CLEAR = 2};
enum E_ROAD_TYPE {HORIZONTAL = 0, VERTICAL = 1, CROSS = 2};

// ============================================================================
// City Object
// some physical thing in the city that takes up a chunk of space
// base class is just the bare ground
// ============================================================================
class CityObject
{
public:
	CityObject(irr::core::vector3df start, irr::core::vector3df max_size,
		E_CITY_OBJECT_TYPE type_ = CLEAR);

	void set_bounds(irr::core::vector3df start, irr::core::vector3df max_size);
	void render(irr::video::IVideoDriver * driver,
		bool show_bounds = false);

	irr::core::aabbox3df get_bounds() const {return bounds;} 
	virtual E_CITY_OBJECT_TYPE get_type() {return type;}

	irr::f32 get_height() const {return height;}

private:
	irr::core::aabbox3df bounds;
	E_CITY_OBJECT_TYPE type;

protected:
	irr::f32 height;
	irr::scene::SMesh smesh;
};

// ============================================================================
// City Road
// ============================================================================
class CityRoad : public CityObject
{
public:
	~CityRoad();
	CityRoad(irr::core::vector3df start, irr::core::vector3df max_size,
		E_ROAD_TYPE type);
};

// ============================================================================
// City Building
// a building representation
// ============================================================================
class CityBuilding : public CityObject
{
public:
	CityBuilding(irr::core::vector3df start, irr::core::vector3df max_size);
};

// ============================================================================
// City Sector
// one section of city buildings
// ============================================================================
class CitySector
{
public:
	CitySector() : from_file(false) {}
	~CitySector();

	void generate_ground();
	void build_sector(irr::core::dimension2di size, irr::core::vector3df start,
		irr::core::vector3df building_size, irr::core::dimension2di pos,
		bool load = false, irr::core::stringc irrFile = "",
		irr::IrrlichtDevice * device = 0);
	void prerender(bool selected);
	void render(irr::video::IVideoDriver * driver, bool sector_bounds = false,
		bool building_bounds = false);
	void postrender();
	const irr::core::aabbox3df &get_bounds() const;

	// editor functions
	void unload_scene(irr::core::dimension2di size, irr::core::vector3df start,
		irr::core::vector3df building_size, irr::core::dimension2di pos);
	void load_scene(const irr::core::stringc irrFile,
		irr::core::vector3df start, irr::IrrlichtDevice * device);
	bool is_from_file() const {return from_file;}

private:
	irr::scene::SMeshBuffer ground;
	array2d<CityObject *> buildings;

	irr::core::list<irr::scene::ISceneNode *> nodes;
	irr::scene::ISceneNode * node;
	bool from_file;
	bool selected;

	irr::core::aabbox3df bounds;	// bounding box
	irr::s32 id;					// id (could be helpful?)
	irr::s32 max_height;			// max building height in section
};

// ============================================================================
// City Scene
// the entire city scene
// ============================================================================
class CityScene
{
public:
	CityScene(irr::IrrlichtDevice * device,
		irr::core::dimension2di size_,
		irr::core::dimension2di sector_size_,
		irr::core::vector3df building_size_);
	~CityScene();

	void prerender();
	void render();
	const irr::core::aabbox3df& getBoundingBox() const;

	bool set_selected_sector(const irr::core::position2di &cursor);
	irr::core::position2di get_selected_sector() const {return selected_sector;}

	void set_position(const irr::core::vector3df &pos);
	irr::core::vector3df get_position() const {return position;}
	bool visible(irr::core::aabbox3df box) const;
	void show_bounds(bool city = true, bool sector = true, bool building = true);

	static irr::video::ITexture * getTexture(irr::s32 id);

	void set_camera(irr::core::vector3df position)	{camera = position;}
	irr::core::vector3df get_camera() const			{return camera;}

	// debug and editor options
	irr::s32 get_total_sectors() const {return size.Width * size.Height;}
	irr::s32 get_visible_sectors() const {return visible_sectors;}
	irr::core::dimension2di get_cam_sector() const {return cam_sector;}

	// editor commands
	void remove_scene(irr::s32 x, irr::s32 y);
	void load_scene(irr::s32 x, irr::s32 y, const irr::core::stringc &file,
		irr::IrrlichtDevice * device);
	bool is_scene_loaded(irr::s32 x, irr::s32 y);

private:
	irr::core::dimension2di size;			// size of the city (sectors)
	irr::core::dimension2di sector_size;	// size of each sector (buildings)
	irr::core::vector3df building_size;		// max size of a building (pixels)

	array2d<CitySector> * sectors;			// actual sectors
	irr::core::aabbox3df bounds;			// bounding box

	bool sector_selection;					// whether to highlight selected sectors
	irr::core::position2di selected_sector;	// current moused-over sector

	irr::core::vector3df camera;			// camera coordinates
	irr::core::vector3df position;		// base position of entire city

	irr::core::list<CitySector *> render_sectors; // list of sectors to render
	irr::core::list<CitySector *>::Iterator render_it;

	// debug and editor options
	bool show_city_bounds;
	bool show_sector_bounds;
	bool show_building_bounds;
	irr::s32 visible_sectors;
	irr::core::dimension2di cam_sector;

	// Irrlicht stuff
	irr::IrrlichtDevice			* device;
	irr::video::IVideoDriver	* driver;
	irr::scene::ISceneManager	* smgr;

	// textures
	static irr::core::array<irr::video::ITexture *> textures;
};

#endif