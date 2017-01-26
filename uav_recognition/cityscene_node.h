#ifndef CITY_SCENE_H
#define CITY_SCENE_H

#include "array2d.h"

#include <irrlicht.h>
#include <map>
#include <list>

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
    ~CityObject();

    void set_bounds(irr::core::vector3df start, irr::core::vector3df max_size);
    void render(irr::video::IVideoDriver * driver,
        bool show_bounds = false);

    irr::core::aabbox3df get_bounds() const {return bounds;} 
    virtual E_CITY_OBJECT_TYPE get_type() {return type;}

private:
    irr::core::aabbox3df bounds;
    E_CITY_OBJECT_TYPE type;

protected:
    irr::scene::SMesh smesh;
};

// ============================================================================
// City Road
// ============================================================================
class CityRoad : public CityObject
{
public:
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
    CitySector(irr::IrrlichtDevice * device) : device(device), from_file(false) {}
    ~CitySector();

    void generate_ground();
    void build_sector(irr::core::dimension2di size, irr::core::vector3df start,
        irr::core::vector3df building_size, irr::core::dimension2di pos,
        bool load = false, irr::core::stringc irrFile = "",
        irr::IrrlichtDevice * device = 0);
    void prerender();
    void render(irr::video::IVideoDriver * driver, bool sector_bounds = false,
        bool building_bounds = false);
    void postrender();
    const irr::core::aabbox3df &get_bounds() const;
    irr::core::dimension2di get_coords() const {return coords;}

private:
    irr::IrrlichtDevice * device;

    irr::scene::SMeshBuffer ground;
    array2d<CityObject *> buildings;

    irr::core::list<irr::scene::ISceneNode *> nodes;
    bool from_file;

    irr::core::dimension2di coords; // the coordinates of the sector
    irr::core::aabbox3df bounds;    // bounding box
    irr::s32 id;                    // id (could be helpful?)
    irr::s32 max_height;            // max building height in section
};

// ============================================================================
// City Scene
// the entire city scene
// ============================================================================
class CityScene : public irr::scene::ISceneNode
{
public:
    CityScene(irr::IrrlichtDevice * device,
        irr::core::dimension2di size_,
        irr::core::dimension2di sector_size_,
        irr::core::vector3df building_size_);
    ~CityScene();

    void prerender();
    void render();
    virtual const irr::core::aabbox3df& getBoundingBox() const;

    void set_position(const irr::core::vector3df &pos);
    bool visible(irr::core::aabbox3df box) const;
    void show_bounds(bool city = true, bool sector = true, bool building = true);

    static irr::video::ITexture * getTexture() {return texture;}
    static irr::video::ITexture * getTexture(irr::s32 id);

    void set_camera(irr::core::vector3df position)  {camera = position;}
    irr::core::vector3df get_camera() const         {return camera;}

    // debug
    irr::s32 get_total_sectors() const {return size.Width * size.Height;}
    irr::s32 get_visible_sectors() const {return visible_sectors;}
    irr::core::dimension2di get_cam_sector() const {return cam_sector;}

private:
    irr::IrrlichtDevice * device;

    irr::core::dimension2di size;           // size of the city (sectors)
    irr::core::dimension2di sector_size;    // size of each sector (buildings)
    irr::core::vector3df building_size;     // max size of a building (pixels)

    // scenes is a map of positions to scene sectors
    // cache provides temporary storage for "filler" sectors, which are generated on the fly
    // the cache_map allows for fast searches in the cache
    std::map<std::pair<irr::s32, irr::s32>, CitySector *> scenes; // scenes, always present
    std::list<CitySector *> cache;          // list of created sectors, ordered by time used
    std::map<std::pair<irr::s32, irr::s32>, std::list<CitySector *>::iterator> cache_map;
    int cache_size_max;                     // size of the cache at which to kick things out
    int cache_size_min;                     // size of the cache after kicking things out

    irr::core::aabbox3df bounds;            // bounding box
    irr::core::vector3df camera;            // camera coordinates

    // render_sectors is the list of sectors to render for the current position
    irr::core::list<CitySector *> render_sectors; // sectors to render
    irr::core::list<CitySector *>::Iterator render_it;

    // debug
    bool show_city_bounds;
    bool show_sector_bounds;
    bool show_building_bounds;
    irr::s32 visible_sectors;
    irr::core::dimension2di cam_sector;

    // texture
    static irr::core::array<irr::video::ITexture *> textures;
    static irr::video::ITexture * texture;
    static irr::video::ITexture * ground_texture1;
    static irr::video::ITexture * road_texture1;
    static irr::video::ITexture * road_texture2;
    static irr::video::ITexture * road_texture3;
};

#endif /* CITY_SCENE_H */