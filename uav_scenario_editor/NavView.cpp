#include "stdafx.h"
#include "NavView.h"
#include "InputGlobals.h"
#include "SceneGlobals.h"
#include "Waypoint.h"
#include "CityScene.h"

// To get ahold of the different windows and data
#include "UAVEditor.h"
#include "UAVEditorDoc.h"
#include "MainFrm.h"
#include "OutputWnd.h"
#include "ClassView.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

enum E_ICON_TYPE {ICON_UAV = 0, ICON_WAYPOINT, ICON_UFO, ICON_BASE};

irr::s32 MAP_ICON_SIZE = 32;
irr::s32 MOUSE_OVER_WIDTH = 8;
irr::f32 ALTITUDE = 65.f;
irr::f32 UAV_SPEED = 25.f;

irr::video::SColor COLOR1(255, 79, 129, 189);	// blue
irr::video::SColor COLOR2(255, 209, 99, 9);		// orange
irr::video::SColor COLOR3(255, 112, 139, 57);	// vomit
irr::video::SColor COLOR4(255, 192, 80, 77);	// pink
irr::video::SColor COLOR5(255, 128, 100, 162);	// purple
irr::video::SColor COLOR6(255, 45, 119, 119);	// teal
irr::video::SColor COLOR7(255, 207, 172, 0);	// yellow
irr::video::SColor COLOR8(255, 152, 72, 7);		// brown
irr::video::SColor COLOR9(255, 158, 10, 10);	// red

irr::video::SColor GetColorFromId(irr::s32 id)
{
	switch(id)
	{
	case 1:	return COLOR1;
	case 2: return COLOR2;
	case 3: return COLOR3;
	case 4: return COLOR4;
	case 5: return COLOR5;
	case 6: return COLOR6;
	case 7: return COLOR7;
	case 8: return COLOR8;
	default: return COLOR9;
	}
}

NavView::NavView()
{
	Log(L"Setting up Navigation View");
	mouseObject = 0;
	mouseLock = false;
	mouseObjectType = ENC_GENERAL;
	showPaths = true;
	snapToSectorCenters = true;
	showGrid = false;
}

void NavView::Log(LPCTSTR lpszString)
{
	COutputWnd &output = ((CMainFrame*)theApp.GetMainWnd())->GetOutputWindow();
	output.Log(COutputWnd::ELW_NAVIGATION, lpszString);
}

void NavView::SetUp(CUAVEditorDoc * pDoc, irr::IrrlichtDevice * device,
	irr::scene::ICameraSceneNode * camera)
{
	// Keep track of the world/model data
	this->pDoc = pDoc;

	// Keep track of the device and the camera
	this->device = device;
	this->camera = camera;
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	// Load the font
	font = guienv->getFont("../media/fonts/font_small.xml");
	text = guienv->addStaticText(L"-",  core::rect<s32>(10,10,150,30));
	text->setOverrideFont(font);

	// Load up the controls to show the distances and times
	display[0] = guienv->addStaticText(L"Distance from UAV:",		core::rect<s32>(10,10,250,30), false);
	display[1] = guienv->addStaticText(L"Time from UAV:",			core::rect<s32>(10,30,250,50), false);
	display[2] = guienv->addStaticText(L"Distance from last WP:",	core::rect<s32>(10,50,250,70), false);
	display[3] = guienv->addStaticText(L"Time from last WP:",		core::rect<s32>(10,70,250,90), false);
	display[4] = guienv->addStaticText(L"-", core::rect<s32>(165,10,360,30), false);
	display[5] = guienv->addStaticText(L"-", core::rect<s32>(165,30,360,50), false);
	display[6] = guienv->addStaticText(L"-", core::rect<s32>(165,50,360,70), false);
	display[7] = guienv->addStaticText(L"-", core::rect<s32>(165,70,360,90), false);
	for(int i = 0; i < 8; i++)
		display[i]->setOverrideColor(COLOR_WHITE);

	IGUIElement * guiElmRoot = guienv->getRootGUIElement();

	// Load the map images
	map_image = driver->getTexture("../media/navmap/map.png");

	uav = new GUIImage(rect<s32>(0,0,32,32), device, guiElmRoot);
	uav->setTexture(driver->getTexture("../media/navmap/uav.png"));
	uav->setBilinearFilter(true);
	uav->setVisible(false);

	waypoint = new GUIImage(rect<s32>(0,0,32,32), device, guiElmRoot);
	waypoint->setTexture(driver->getTexture("../media/navmap/waypoint.png"));
	waypoint->setVisible(false);

	base = new GUIImage(rect<s32>(0,0,32,32), device, guiElmRoot);
	base->setTexture(driver->getTexture("../media/navmap/base.png"));
	base->setVisible(false);

	ufo = new GUIImage(rect<s32>(0,0,32,32), device, guiElmRoot);
	ufo->setTexture(driver->getTexture("../media/navmap/ufo.png"));
	ufo->setVisible(false);
}

void NavView::Render(bool &newMousePos)
{
	driver->beginScene(true, true, irr::video::SColor(255,50,50,50));

	// Reorient the UAVs
	for(pDoc->uav_it = pDoc->uavs.begin(); pDoc->uav_it != pDoc->uavs.end(); pDoc->uav_it++)
		(*pDoc->uav_it)->reOrient();

	// Draw the map
	rect<s32> map_source(0,0,map_image->getSize().Width,map_image->getSize().Height);
	rect<s32> map_dest(driver->getViewPort());
	dimension2d<s32> image_size(map_image->getSize().Width, map_image->getSize().Height);
	GUIImage::draw(driver, map_image, image_size, map_source, map_dest);

	if(showGrid)
		DrawSectorLines();

	if(showPaths)
	{
		// Draw UAV lines
		for(pDoc->uav_it = pDoc->uavs.begin(); pDoc->uav_it != pDoc->uavs.end(); pDoc->uav_it++)
			DrawUAVPath(*pDoc->uav_it);
	}

	// Draw bases
	for(pDoc->base_it = pDoc->bases.begin(); pDoc->base_it != pDoc->bases.end(); pDoc->base_it++)
	{
		SColor color = (*pDoc->base_it)->getColor();
		if((*pDoc->base_it) == mouseObject) color = COLOR_RED;
		DrawIcon((*pDoc->base_it)->getPosition2D(), color, ICON_BASE, 0.f);
		DrawText((*pDoc->base_it)->getPosition2D(), color, (*pDoc->base_it)->getName());
	}

	// Draw waypoints
	for(pDoc->wp_it = pDoc->wps.begin(); pDoc->wp_it != pDoc->wps.end(); pDoc->wp_it++)
	{
		SColor color = (*pDoc->wp_it)->getColor();
		if((*pDoc->wp_it) == mouseObject) color = COLOR_RED;
		DrawIcon((*pDoc->wp_it)->getPosition2D(), color, ICON_WAYPOINT, 0.f);
		DrawText((*pDoc->wp_it)->getPosition2D(), color, (*pDoc->wp_it)->getName());
	}

	// Draw UAVs
	for(pDoc->uav_it = pDoc->uavs.begin(); pDoc->uav_it != pDoc->uavs.end(); pDoc->uav_it++)
	{
		SColor color = (*pDoc->uav_it)->getColor();
		if((*pDoc->uav_it) == mouseObject) color = COLOR_RED;
		DrawIcon((*pDoc->uav_it)->getPosition2D(), color, ICON_UAV, (*pDoc->uav_it)->getRotAngle2D());
		DrawText((*pDoc->uav_it)->getPosition2D(), color, (*pDoc->uav_it)->getName());
	}

	DrawGUIText();
	driver->endScene();
}

void NavView::DrawSectorLines()
{
	rect<s32> window = driver->getViewPort();
	int width = window.LowerRightCorner.X;
	int height = window.LowerRightCorner.Y;
	int step;

	// Vertical lines
	step = width / city_size;
	for(int i = 0; i < width; i+=step)
		driver->draw2DLine(position2di(i, 0), position2di(i, height), SColor(50,255,255,255));

	// Horizontal lines
	step = height / city_size;
	for(int i = 0; i < height; i+=step)
		driver->draw2DLine(position2di(0, i), position2di(width, i), SColor(50,255,255,255));
}

void NavView::DrawGUIText()
{
	// Set GUI text (only display for things with a UAV parent)
	display[4]->setText(L"-");
	display[5]->setText(L"-");
	display[6]->setText(L"-");
	display[7]->setText(L"-");
	if(mouseObject && mouseObject->getParent())
	{
		UAVObject * uav = (UAVObject *)mouseObject->getParent(); // dangerous xD
		
		f32 distance = uav->getDistance(mouseObject);
		f32 time = distance / UAV_SPEED;

		// Distance from UAV
		stringw str; str += distance;
		display[4]->setText(str.c_str());

		// Time from UAV
		str = ""; str += time;
		display[5]->setText(str.c_str());

		distance = uav->getDistanceFromLastWP(mouseObject);
		if(distance >= 0)
		{
			time = distance / UAV_SPEED;

			// Distance from last waypoint
			str=""; str += distance;
			display[6]->setText(str.c_str());

			// Time from last waypoint
			str = ""; str += time;
			display[7]->setText(str.c_str());		
		}
	}

	// Draw GUI text
	for(int i = 0; i < 8; i++)
		display[i]->draw();
}

void NavView::DrawIcon(const irr::core::position2df &pos, const irr::video::SColor &color, E_ICON_TYPE icon, irr::f32 rotation)
{
	//if(IsVisible(pos))
	//{
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
			position2di pixels(MapToScreenCoords(pos));
			pixels.X -= (MAP_ICON_SIZE / 2);
			pixels.Y -= (MAP_ICON_SIZE / 2);
			image->setVisible(true);
			image->setPosition(pixels);
			image->setColor(color);
			image->setRotation(rotation);
			image->draw();
			image->setVisible(false);
		}
	//}
}

void NavView::DrawText(const irr::core::position2df &pos, irr::video::SColor color, const irr::core::stringw &text)
{
	//if(IsVisible(pos))
	//{
		position2di pixels = MapToScreenCoords(pos);
		rect<s32> text_rect(
			pixels.X - MAP_ICON_SIZE * 2,
			pixels.Y,
			pixels.X + MAP_ICON_SIZE * 2,
			pixels.Y + MAP_ICON_SIZE);
		font->draw(text.c_str(), text_rect, color, true, true, &text_rect);
	//}
}

void NavView::DrawUAVPath(UAVObject * obj)
{
	// Get the position of the UAV
	position2di line_start = MapToScreenCoords(obj->getPosition2D());
	position2di line_end;
	position2di uav_pos = line_start;

	// Draw lines between each of the UAV's waypoints
	const std::list<WaypointObject *> * wps = obj->getWPList();
	std::list<WaypointObject *>::const_iterator wp = wps->begin();
	while(wp != wps->end())
	{
		line_end = MapToScreenCoords((*wp)->getPosition2D());
		position2di start(
			s32(line_start.X),
			s32(line_start.Y));
		position2di end(
			s32(line_end.X),
			s32(line_end.Y));
		driver->draw2DLine(start, end, obj->getColor());

		line_start = line_end;
		wp++;
	}

	// and the line from the last waypoint to the base
	if(obj->getBase())
	{
		line_end = MapToScreenCoords(obj->getBase()->getPosition2D());
			position2di start(
				s32(line_start.X),
				s32(line_start.Y));
			position2di end(
				s32(line_end.X),
				s32(line_end.Y));
		driver->draw2DLine(start, end, obj->getColor());
	}
}

irr::core::position2df NavView::ScreenToMapCoords(const irr::core::position2di &coords)
{
	f32 screenWidth = f32(driver->getViewPort().getWidth());
	f32 screenHeight = f32(driver->getViewPort().getHeight());
	f32 mapWidth = pDoc->worldSize.Width;
	f32 mapHeight = pDoc->worldSize.Height;

	position2df map(
		f32(coords.X) * mapWidth / screenWidth,
		f32(coords.Y) * mapHeight / screenHeight);

	// flip everything vertically
	map.Y = mapHeight - map.Y;

	return map;
}

irr::core::position2di NavView::MapToScreenCoords(const irr::core::position2df &coords)
{
	s32 screenWidth = driver->getViewPort().getWidth();
	s32 screenHeight = driver->getViewPort().getHeight();
	f32 mapWidth = pDoc->worldSize.Width;
	f32 mapHeight = pDoc->worldSize.Height;

	position2di screen(
		s32(coords.X * f32(screenWidth) / mapWidth),
		s32(coords.Y * f32(screenHeight) / mapHeight));

	// flip everything vertically
	screen.Y = screenHeight - screen.Y;

	return screen;
}

void NavView::HandleMouseInput(CPoint mousePos, CPoint relMousePos, bool mouseLButton)
{
	this->mousePos.X = mousePos.x;
	this->mousePos.Y = mousePos.y;

	// Make sure the mouse is still over the current object
	if(mouseObject)
	{
		// Check for mouseLock
		mouseLock = mouseLButton;

		// Handle dragging
		if(mouseLock)
		{
			position2df mPos = ScreenToMapCoords(position2di(mousePos.x, mousePos.y));
			if(snapToSectorCenters)
				CenterMapCoordsOnSector(mPos);
			EnforceBounds(mPos);
			mouseObject->setPosition(vector3df(mPos.X, mouseObject->getPosition().Y, mPos.Y));
		}
		else
		{
			// Check and see if the mouse is still over the object
			position2di sPos = MapToScreenCoords(mouseObject->getPosition2D());
			if(sPos.X-5 < mousePos.x && sPos.X+5 > mousePos.x &&
				sPos.Y-5 < mousePos.y && sPos.Y+5 > mousePos.y)
			{} else
			{
				mouseObject = 0;
				mouseObjectType = ENC_GENERAL;
			}
		}
	}

	// Check for new mouseovers (by most recently added depth)
	if(!mouseObject)
	{
		// UAVs
		for(pDoc->uav_it = pDoc->uavs.begin(); pDoc->uav_it != pDoc->uavs.end(); pDoc->uav_it++)
		{
			position2di sPos = MapToScreenCoords((*pDoc->uav_it)->getPosition2D());
			if(sPos.X-MOUSE_OVER_WIDTH < mousePos.x && sPos.X+MOUSE_OVER_WIDTH > mousePos.x &&
				sPos.Y-MOUSE_OVER_WIDTH < mousePos.y && sPos.Y+MOUSE_OVER_WIDTH > mousePos.y)
			{
				mouseObject = (*pDoc->uav_it);
				mouseObjectType = ENC_UAV;
			}
		}
		// Waypoints
		for(pDoc->wp_it = pDoc->wps.begin(); pDoc->wp_it != pDoc->wps.end(); pDoc->wp_it++)
		{
			position2di sPos = MapToScreenCoords((*pDoc->wp_it)->getPosition2D());
			if(sPos.X-MOUSE_OVER_WIDTH < mousePos.x && sPos.X+MOUSE_OVER_WIDTH > mousePos.x &&
				sPos.Y-MOUSE_OVER_WIDTH < mousePos.y && sPos.Y+MOUSE_OVER_WIDTH > mousePos.y)
			{
				mouseObject = (*pDoc->wp_it);
				mouseObjectType = ENC_WP;
			}
		}
		// Bases
		for(pDoc->base_it = pDoc->bases.begin(); pDoc->base_it != pDoc->bases.end(); pDoc->base_it++)
		{
			position2di sPos = MapToScreenCoords((*pDoc->base_it)->getPosition2D());
			if(sPos.X-MOUSE_OVER_WIDTH < mousePos.x && sPos.X+MOUSE_OVER_WIDTH > mousePos.x &&
				sPos.Y-MOUSE_OVER_WIDTH < mousePos.y && sPos.Y+MOUSE_OVER_WIDTH > mousePos.y)
			{
				mouseObject = (*pDoc->base_it);
				mouseObjectType = ENC_BASE;
			}
		}
	}
}

void NavView::SetContextMousePosition(const irr::core::position2di &cursor)
{
	contextMousePos = cursor;
}

void NavView::AddUAV()
{
	Log(L"Adding UAV");

	// We can only have 9 UAVs
	if(pDoc->uavs.size() > 8)
	{
		AfxMessageBox(L"There can be at most 9 UAVs.");
		return;
	}

	// Find the indicies for the objects we'll create
	int uavId = pDoc->uavs.size();
	int baseId = pDoc->bases.size();
	int wpId = pDoc->wps.size();

	// Create the names of the objects
	stringw uavStr = "Mav-"; uavStr += uavId + 1;
	stringw baseStr = "Base-"; baseStr += baseId + 1;
	stringw wpStr = "EO-"; wpStr += wpId + 1;

	position2df coords = ScreenToMapCoords(contextMousePos);
	EnforceBounds(coords);

	// Create the UAV and its required objects
	// UAV
	AddUAV(uavStr, vector3df(coords.X,ALTITUDE,coords.Y));
	// Base
	coords.X -= 2000; EnforceBounds(coords);
	pDoc->bases.push_back(new SimObject(baseStr, vector3df(coords.X,ALTITUDE,coords.Y)));
	pDoc->uavs.back()->assignBase(pDoc->bases.back());
	// Waypoint
	coords.X += 4000; EnforceBounds(coords);
	pDoc->wps.push_back(new WaypointObject(wpStr, vector3df(coords.X,ALTITUDE,coords.Y)));
	pDoc->uavs.back()->assignWaypoint(pDoc->wps.back());

	// Add the UAV and its required objects to the Object/Event View
	CObjectEventView &view = ((CMainFrame*)theApp.GetMainWnd())->GetClassView();
	view.AddObject(wpStr.c_str(), ITEM_WAYPOINT, wpId, uavId);
	view.AddObject(baseStr.c_str(), ITEM_BASE, wpId, uavId);
}

void NavView::AddUAV(const irr::core::stringw &name, irr::core::vector3df position)
{
	pDoc->uavs.push_back(new UAVObject(name, position, GetColorFromId(pDoc->uavs.size())));

	CObjectEventView &view = ((CMainFrame*)theApp.GetMainWnd())->GetClassView();
	view.AddObject(name.c_str(), ITEM_UAV, pDoc->uavs.size() - 1);
}

void NavView::AddBase()
{
	position2df coords = ScreenToMapCoords(contextMousePos);
	EnforceBounds(coords);

	stringw baseStr = "Base-"; baseStr += pDoc->bases.size() + 1;
	AddBase(baseStr, vector3df(coords.X,ALTITUDE,coords.Y));
}

void NavView::AddBase(const irr::core::stringw &name, irr::core::vector3df position, UAVObject * uav)
{
	// Find the UAV id
	int uavId = -1;
	for(pDoc->uav_it = pDoc->uavs.begin(); pDoc->uav_it != pDoc->uavs.end(); pDoc->uav_it++)
	{
		uavId++;
		if((*pDoc->uav_it) == uav)
			break;
	}

	// Create the base
	pDoc->bases.push_back(new SimObject(name, position));

	// Assign to the uav if necessary
	if(uav) uav->assignBase(pDoc->bases.back());

	// Add the new base to the Object/Event View
	CObjectEventView &view = ((CMainFrame*)theApp.GetMainWnd())->GetClassView();
	view.AddObject(name.c_str(), ITEM_BASE, pDoc->bases.size() - 1, uavId);
}

void NavView::AddWaypoint(UAVObject * uav)
{
	position2df coords;
	if(uav)
	{
		// This waypoint will be attached to a UAV
		coords = position2df(uav->getPosition().X, uav->getPosition().Z - 2000.f);
		EnforceBounds(coords);
	}
	else
	{
		// No UAV attachment required
		coords = ScreenToMapCoords(contextMousePos);
		EnforceBounds(coords);
	}

	// Create the new waypoint
	int wpId = pDoc->wps.size();
	stringw wpStr = "EO-"; wpStr += wpId + 1;
	AddWaypoint(wpStr, vector3df(coords.X,ALTITUDE,coords.Y), true, uav);
}

void NavView::AddWaypoint(const irr::core::stringw &name, irr::core::vector3df position, bool has_feature, UAVObject * uav)
{
	// Find the UAV id
	int uavId = -1;
	for(pDoc->uav_it = pDoc->uavs.begin(); pDoc->uav_it != pDoc->uavs.end(); pDoc->uav_it++)
	{
		uavId++;
		if((*pDoc->uav_it) == uav)
			break;
	}

	// Create the new waypoint
	pDoc->wps.push_back(new WaypointObject(name, position));
	pDoc->wps.back()->setFeature(has_feature);
	
	// Attach the waypoint to the UAV if we're adding to the UAV's list
	if(uav) uav->assignWaypoint(pDoc->wps.back());

	// Add the new waypoint to the Object/Event View
	CObjectEventView &view = ((CMainFrame*)theApp.GetMainWnd())->GetClassView();
	view.AddObject(name.c_str(), ITEM_WAYPOINT, pDoc->wps.size() - 1, uavId);
}

void NavView::EnforceBounds(irr::core::position2df &pos)
{
	float sectorSize = sector_size * building_size;
	float minWidth = 0.f;
	float minHeight = 0.f;
	float maxWidth = city_size * sectorSize;
	float maxHeight = city_size * sectorSize;

	// if the city is sizeable enough, don't let anything be too close to the edges
	if(city_size > 8)
	{
		minWidth = 3 * sectorSize + (sectorSize / 2);
		minHeight = 3 * sectorSize + (sectorSize / 2);
		maxWidth -= minWidth;
		maxHeight -= minHeight;
	}

	if(pos.X < minWidth)	pos.X = minWidth;
	if(pos.Y < minHeight)	pos.Y = minHeight;
	if(pos.X > maxWidth)	pos.X = maxWidth;
	if(pos.Y > maxHeight)	pos.Y = maxHeight;
}

void NavView::CenterMapCoordsOnSector(irr::core::position2df &coords)
{
	float sectorWidth = sector_size * building_size;
	float sectorHeight = sector_size * building_size;
	int secX = int(coords.X / sectorWidth);
	int secY = int(coords.Y / sectorHeight);

	coords.X = (secX * sectorWidth) + (sectorWidth / 2);
	coords.Y = (secY * sectorHeight) + (sectorHeight / 2);
}