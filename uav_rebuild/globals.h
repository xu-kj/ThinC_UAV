#ifndef GLOBALS_H
#define GLOBALS_H

#include <irrlicht.h>

#define RES_1024
#ifdef RES_1024

    // resolution
    const irr::s32 RES_X = 1024;
    const irr::s32 RES_Y = 768;

    // fonts
    const irr::core::stringc FONT_LARGE = "font_large.xml";
    const irr::core::stringc FONT_SMALL = "font_small.xml";

    // UAV information console
    const irr::core::stringc UAV_CONS_ZIP = "../media/uav_cons_1024.pk3";
    // UAV console box
    const irr::s32 HUD_UAV_CONS_X = 168;
    const irr::s32 HUD_UAV_CONS_Y = 85;
    // UAV name text coordinates
    const irr::s32 UAV_NAME_START_X = 10;
    const irr::s32 UAV_NAME_START_Y = 12;
    const irr::s32 UAV_NAME_END_X = 90;
    const irr::s32 UAV_NAME_END_Y = 42;
    // UAV NT/ET text coordinates
    const irr::s32 UAV_NTET_START_X = 8;
    const irr::s32 UAV_NTET_START_Y = 51;
    const irr::s32 UAV_NTET_END_X = 156;
    const irr::s32 UAV_NTET_END_Y = 60;
    // UAV fuel text coordinates
    const irr::s32 UAV_FUEL_START_X = 8;
    const irr::s32 UAV_FUEL_START_Y = 64;
    const irr::s32 UAV_FUEL_END_X = 37;
    const irr::s32 UAV_FUEL_END_Y = 76;
    // UAV GPS text coordinates
    const irr::s32 UAV_GPS_START_X = 98;
    const irr::s32 UAV_GPS_START_Y = 11;
    const irr::s32 UAV_GPS_END_X = 160;
    const irr::s32 UAV_GPS_END_Y = 22;
    // UAV Camera text coordinates
    const irr::s32 UAV_CAM_START_X = 98;
    const irr::s32 UAV_CAM_START_Y = 30;
    const irr::s32 UAV_CAM_END_X = 160;
    const irr::s32 UAV_CAM_END_Y = 42;
    // UAV fuel indicator
    const irr::s32 FUEL_START_X = 40;
    const irr::s32 FUEL_START_Y = 66;
    const irr::s32 FUEL_END_X = 158;
    const irr::s32 FUEL_END_Y = 76;

    // map textures
    const irr::core::stringc MAP_ZIP = "../media/map_1024.pk3";
    // map offset from upper-left corner of screen
        
        /*
        // with border
        const irr::s32 MAP_START_X = 190;
        const irr::s32 MAP_START_Y = 22;
        const irr::s32 MAP_END_X = 1002;
        const irr::s32 MAP_END_Y = 550;
        const irr::s32 MAP_BORDER_WIDTH = 22;
        const irr::s32 MAP_BORDER_IMG_SIZE = 32;
        */

        //without border
        const irr::s32 MAP_START_X = 190 - 22;
        const irr::s32 MAP_START_Y = 22 - 22;
        const irr::s32 MAP_END_X = 1002 + 22;
        const irr::s32 MAP_END_Y = 550 + 22;
        const irr::s32 MAP_BORDER_WIDTH = 0;
        const irr::s32 MAP_BORDER_IMG_SIZE = 0;

    // map file size
    //const irr::s32 MAP_SIZE_X = 1536;
    //const irr::s32 MAP_SIZE_Y = 1376;
    const irr::s32 MAP_SIZE_X = 1712;
    const irr::s32 MAP_SIZE_Y = 1144;

    // events area
    const irr::s32 EVENT_START_X = 170;
    const irr::s32 EVENT_START_Y = 570;
    const irr::s32 EVENT_ICON_WIDTH = 32;
    const irr::s32 EVENT_ICON_HEIGHT = 18;

    // chat texture
    const irr::core::stringc CHAT_ZIP = "../media/chat_1024.pk3";
    const irr::s32 CHAT_SCROLL_WIDTH = 18;
    const irr::s32 CHAT_TEXT_HEIGHT = 16;
    // chat area
    const irr::s32 CHAT_START_X = 190 + 200;
    const irr::s32 CHAT_START_Y = 550 + 22;
    const irr::s32 CHAT_END_X = 1024 - 200;
    const irr::s32 CHAT_END_Y = 768;

    // fonts
    const irr::core::stringc FONTS_ZIP = "../media/fonts.pk3";

    // object icons
    const irr::core::stringc OBJ_ICONS_ZIP = "../media/obj_icons.pk3";

    // cam view
    //const irr::core::stringc CAM_ZIP = "../media/cam_1024.pk3";
    extern irr::s32 CAM_SIZE_X;// = 341;
    extern irr::s32 CAM_SIZE_Y;// = 256;
    const irr::s32 BUTTON_SIZE_X = 47;
    const irr::s32 BUTTON_SIZE_Y = 47;
#endif

// some colors
extern irr::video::SColor COLOR1;
extern irr::video::SColor COLOR2;
extern irr::video::SColor COLOR3;
extern irr::video::SColor COLOR4;
extern irr::video::SColor COLOR5;
extern irr::video::SColor COLOR6;
extern irr::video::SColor COLOR7;
extern irr::video::SColor COLOR8;
extern irr::video::SColor COLOR9;
// more colors
const irr::video::SColor COLOR_GRAY(255, 200, 200, 200);    // gray
const irr::video::SColor COLOR_DARK_GRAY(255,150,150,150);  // dark gray
const irr::video::SColor COLOR_WHITE(255, 255, 255, 255);   // white
const irr::video::SColor COLOR_GREEN(255, 32, 120, 72);     // green
const irr::video::SColor COLOR_BLACK(255, 0, 0, 0);         // black8
const irr::video::SColor COLOR_RED(255, 255, 0, 0);         // red red
const irr::video::SColor COLOR_YELLOW(255, 255, 255, 0);    // yellow
const irr::video::SColor COLOR_EVENT_BG(255,33,33,33);      // very dark gray
// waypoint colors
extern irr::video::SColor WAYPOINT_UNSURE;
extern irr::video::SColor WAYPOINT_CLEAR;
extern irr::video::SColor WAYPOINT_CONFIRMED;

// general preferences
extern bool SHOW_TIME;
extern bool SHOW_VERSION;
extern bool USE_CAMERA_FAIL;
extern bool CAM_FAIL_AS_ANGLE;
extern irr::f32 CAM_FAIL_ANGLE;
extern bool USE_GPS_FAIL;
extern bool ALLOW_SPEED_CHANGES;
extern bool USE_FULL_CAM_SHADING;
extern bool USE_VIEW_CAM_SHADING;
extern bool SHOW_PROGRESS_BAR;
extern irr::s32 BUTTON_MS;

extern bool ALLOW_PAUSE;
extern bool simulation_paused; // exception :\

// general UAV configuration
extern irr::f32 FUEL_FACTOR;
extern irr::f32 SPEED_FACTOR;
extern irr::f32 DOWN_ANGLE;
extern irr::f32 UAV_HEIGHT;
extern bool ZOOM_NEAR_TARGET;
extern irr::f32 FEATURE_VISIBLE_DIST;
extern irr::f32 FEATURE_MISSED_DIST;
extern bool LIGHT_AFTER_PASS;

// map size
extern long unsigned int WORLD_START_PIXELS_X;
extern long unsigned int WORLD_START_PIXELS_Z;
extern long unsigned int WORLD_END_PIXELS_X;
extern long unsigned int WORLD_END_PIXELS_Z;
extern irr::f32 WORLD_START_LON;
extern irr::f32 WORLD_START_LAT;
extern irr::f32 WORLD_END_LON;
extern irr::f32 WORLD_END_LAT;
//extern irr::f32 PIXELS_PER_DEGREE;

// city configuration
extern irr::s32 CITY_SIZE_X;
extern irr::s32 CITY_SIZE_Y;
extern irr::s32 CITY_SECTOR_SIZE;
extern irr::f32 CITY_BUILDING_SIZE;
extern irr::f32 CITY_BUILDING_HEIGHT; 

// map configuration
extern irr::s32 MAP_ICON_SIZE;
extern irr::f32 ZOOM_AMOUNT;
extern bool SHOW_BASE_TEXT;
extern bool SHOW_WAYPOINT_TEXT;
extern bool SHOW_UAV_TEXT;
extern bool SHOW_UAV_LINES;
extern bool SHOW_EXT_UAV_LINES;
extern bool SHOW_UFO_TEXT;
extern bool SHOW_UAV_ICONS;
extern bool SHOW_WAYPOINT_ICONS;
extern bool SHOW_BASE_ICONS;
extern bool SHOW_UFO_ICONS;
extern bool SHOW_UAV_START_ICONS;

// light cue configuration
extern bool USE_LIGHT_CUES;
extern irr::s32 LOW_LIGHT_LEVEL;
extern irr::s32 MEDIUM_LIGHT_LEVEL;
extern irr::s32 HIGH_LIGHT_LEVEL;
extern irr::s32 BASE_LIGHT_LEVEL;
extern irr::f32 MEDIUM_LIGHT_DIST;
extern irr::f32 HIGH_LIGHT_DIST;

// tactor cue configuration
extern bool  RUN_TACTOR_TEST;
extern bool  USE_TACTOR_CUES;
extern int   TACTOR_COM_PORT_1;
extern int   TACTOR_COM_PORT_2;
extern int   TACTOR_COM_PORT_3;
extern int   TACTOR_COM_PORT_4;
extern int   TACTOR_COM_PORT_5;
extern int   TACTOR_COM_PORT_6;
extern int   TACTOR_GAIN;
extern int   TACTOR_FREQUENCY;
extern int   HIGH_TACTOR_FREQ_1;
extern int   HIGH_TACTOR_FREQ_2;
extern int   HIGH_TACTOR_SOURCE;
extern int   HIGH_TACTOR_LENGTH;
extern bool  HIGH_TACTOR_PULSE;
extern int   HIGH_TACTOR_PULSE_INTERVAL;
extern int   LOW_TACTOR_FREQ_1;
extern int   LOW_TACTOR_FREQ_2;
extern int   LOW_TACTOR_SOURCE;
extern int   LOW_TACTOR_LENGTH;
extern bool  LOW_TACTOR_PULSE;
extern int   LOW_TACTOR_PULSE_INTERVAL;
extern float HIGH_TACTOR_DIST;
extern float LOW_TACTOR_DIST;

// world load file
extern irr::core::stringc SCENARIO_FILE;
extern irr::core::stringc OUTPUT_FILE;
extern bool TIMESTAMP_OUTPUT;

// pointer to array of targets; used to delay-load the scenes
extern void * TARGETS;

// time-keeping
extern irr::u32 MINUTES;
extern irr::u32 SECONDS;
extern irr::u32 MILLISECONDS;
extern irr::u32 TICKS;

// network
extern bool USE_NETWORK;

// sara simulation overrides
extern bool USE_SARA_SHADING;

#endif /* GLOBALS_H */