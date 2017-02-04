#pragma once
#ifndef NAV_VIEW_H
#define NAV_VIEW_H

#include <irrlicht.h>
#include <vector>
#include "Image.h"

class CityScene;
class SimObject;
class UAVObject;
class CUAVEditorDoc;
enum E_ICON_TYPE;

enum E_NAV_CONTEXT_TYPE {ENC_GENERAL, ENC_UAV, ENC_WP, ENC_BASE};

class NavView
{
public:
	NavView();

	void Log(LPCTSTR lpszString);
	void SetUp(CUAVEditorDoc * pDoc, irr::IrrlichtDevice * device, irr::scene::ICameraSceneNode * camera);

	// Drawing
	void Render(bool &newMousePos);
	void DrawSectorLines();
	void DrawGUIText();
	void DrawIcon(const irr::core::position2df &pos, const irr::video::SColor &color, E_ICON_TYPE icon, irr::f32 rotation);
	void DrawText(const irr::core::position2df &pos, irr::video::SColor color, const irr::core::stringw &text);
	void DrawUAVPath(UAVObject * obj);

	// Conversion
	irr::core::position2df ScreenToMapCoords(const irr::core::position2di &coords);
	irr::core::position2di MapToScreenCoords(const irr::core::position2df &coords);

	// Model interaction
	void AddUAV();
	void AddUAV(const irr::core::stringw &name, irr::core::vector3df position);
	void AddBase();
	void AddBase(const irr::core::stringw &name, irr::core::vector3df position, UAVObject * uav = 0);
	void AddWaypoint(UAVObject * uav = 0);
	void AddWaypoint(const irr::core::stringw &name, irr::core::vector3df position, bool has_feature, UAVObject * uav = 0);

	// Input
	void HandleMouseInput(CPoint mousePos, CPoint relMousePos, bool mouseLButton);
	void SetContextMousePosition(const irr::core::position2di &cursor); // call in OnContext
	void DropMouseObject() {mouseObject = 0; mouseObjectType = ENC_GENERAL; mouseLock = false;}

	// Current mouse object
	E_NAV_CONTEXT_TYPE GetContextType() const {return mouseObjectType;}
	SimObject * GetMouseObject() const {return mouseObject;}

	// Helpers
	void EnforceBounds(irr::core::position2df &pos);
	void CenterMapCoordsOnSector(irr::core::position2df &coords);

	// View preferences
	bool showPaths;
	bool snapToSectorCenters;
	bool showGrid;

private:
	// World/model data
	CUAVEditorDoc * pDoc;

	// Easy access to irrlicht
	irr::IrrlichtDevice			 * device;
	irr::video::IVideoDriver	 * driver;
	irr::scene::ISceneManager	 * smgr;
	irr::gui::IGUIEnvironment	 * guienv;
	irr::scene::ICameraSceneNode * camera;

	// Text display
	irr::gui::IGUIStaticText * text;
	irr::gui::IGUIStaticText * display[8];
	irr::gui::IGUIFont * font;

	// Input data
	irr::core::position2di contextMousePos; // the position the mouse was at when the context menu was invoked
	irr::core::position2di mousePos; // the current mouse position
	SimObject * mouseObject; // pointer to object the mouse is currently over (0 if nothing)
	E_NAV_CONTEXT_TYPE mouseObjectType;
	bool mouseLock;

	// Object image
	irr::video::ITexture * map_image;

	// Object icons
	GUIImage * uav;
	GUIImage * waypoint;
	GUIImage * base;
	GUIImage * ufo;
};

#endif
