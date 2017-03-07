#include "color.h"
#include <irrlicht.h>

namespace color {
	irr::video::SColor COLOR1(255,  79, 129, 189);  // blue
	irr::video::SColor COLOR2(255, 209,  99,   9);  // orange
	irr::video::SColor COLOR3(255, 112, 139,  57);  // vomit
	irr::video::SColor COLOR4(255, 192,  80,  77);  // pink
	irr::video::SColor COLOR5(255, 128, 100, 162);  // purple
	irr::video::SColor COLOR6(255,  45, 119, 119);  // teal
	irr::video::SColor COLOR7(255, 207, 172,   0);  // yellow
	irr::video::SColor COLOR8(255, 152,  72,   7);  // brown
	irr::video::SColor COLOR9(255, 158,  10,  10);  // red

	const irr::video::SColor COLOR_GRAY(255, 200, 200, 200);    // gray
	const irr::video::SColor COLOR_DARK_GRAY(255,150,150,150);  // dark gray
	const irr::video::SColor COLOR_WHITE(255, 255, 255, 255);   // white
	const irr::video::SColor COLOR_GREEN(255, 32, 120, 72);     // green
	const irr::video::SColor COLOR_BLACK(255, 0, 0, 0);         // black8
	const irr::video::SColor COLOR_RED(255, 255, 0, 0);         // red red
	const irr::video::SColor COLOR_YELLOW(255, 255, 255, 0);    // yellow
	const irr::video::SColor COLOR_EVENT_BG(255,33,33,33);      // very dark gray

	irr::video::SColor WAYPOINT_UNSURE = COLOR_GRAY;
	irr::video::SColor WAYPOINT_CLEAR = COLOR_RED;
	irr::video::SColor WAYPOINT_CONFIRMED = COLOR_GREEN;

	irr::video::SColor getColorFromId(int id) {
		//switch(id)
		//{
		//case 1: return COLOR1;
		//case 2: return COLOR2;
		//case 3: return COLOR3;
		//case 4: return COLOR4;
		//case 5: return COLOR5;
		//case 6: return COLOR6;
		//case 7: return COLOR7;
		//case 8: return COLOR8;
		//default: return COLOR9;
		//}
		return COLOR1;
	}

}