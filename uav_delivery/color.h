#ifndef COLOR_H
#define COLOR_H

#include <irrlicht.h>

namespace color {
	// uav colors
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
	extern const irr::video::SColor COLOR_GRAY;
	extern const irr::video::SColor COLOR_DARK_GRAY;
	extern const irr::video::SColor COLOR_WHITE;
	extern const irr::video::SColor COLOR_GREEN;
	extern const irr::video::SColor COLOR_BLACK;
	extern const irr::video::SColor COLOR_RED;
	extern const irr::video::SColor COLOR_YELLOW;
	extern const irr::video::SColor COLOR_EVENT_BG;
	// waypoint colors
	extern irr::video::SColor WAYPOINT_UNSURE;
	extern irr::video::SColor WAYPOINT_CLEAR;
	extern irr::video::SColor WAYPOINT_CONFIRMED;

	irr::video::SColor getColorFromId(int id);
}

#endif /* COLOR_H */