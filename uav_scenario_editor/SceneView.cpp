#include "stdafx.h"
#include "SceneView.h"
#include "SceneGlobals.h"
#include "InputGlobals.h"

// To get ahold of the different windows and data
#include "UAVEditor.h"
#include "UAVEditorDoc.h"
#include "MainFrm.h"
#include "OutputWnd.h"
#include "ClassView.h"

#pragma warning (disable : 4996)

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

irr::s32 city_size = 300;		// in sectors
irr::s32 sector_size = 3;		// in buildings
irr::f32 building_size = 30;	// in pixels
irr::f32 building_height = 60;	// in pixels

const irr::f32 MOVE_SPEED = .2f; // move speed factor

// Moveable area in 3D (basically a box)
const f32 MAX_HEIGHT = 100.f;
const f32 MIN_HEIGHT = 10.f;
const f32 MAX_X = city_size * sector_size * building_size;
const f32 MAX_Y = city_size * sector_size * building_size;
const f32 MIN_X = 0.f;
const f32 MIN_Y = 0.f;

SceneView::SceneView()
{
	Log(L"Setting up Scene View");

	city = 0;
	enableFog = false;
	fogChanged = true; // have to set the initial fog the first time through
	showCityBounds = false;
	showSectorBounds = false;
	showBuildingBounds = false;
	showDataIcons = true;
	showSelectedSector = false;
}

SceneView::~SceneView()
{
	Log(L"Cleaning up Scene View");
	if(city) delete city;
	Log(L"Done cleaing up Scene View");
}

void SceneView::Log(LPCTSTR lpszString)
{
	COutputWnd &output = ((CMainFrame*)theApp.GetMainWnd())->GetOutputWindow();
	output.Log(COutputWnd::ELW_CITY, lpszString);
}

void SceneView::SetUp(CUAVEditorDoc * pDoc, irr::IrrlichtDevice * device, irr::scene::ICameraSceneNode * camera)
{
	// Keep track of the world/model data
	this->pDoc = pDoc;
	this->pDoc->worldSize.Width = city_size * sector_size * building_size;
	this->pDoc->worldSize.Height = city_size * sector_size * building_size;

	// Keep track of the device and the camera
	this->device = device;
	this->camera = camera;
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	// Set up text to show some stats
	text[0] = guienv->addStaticText(L"FPS",  core::rect<s32>(10,10,150,30), false);
	text[1] = guienv->addStaticText(L"Current Sector",  core::rect<s32>(10,30,150,50), false);
	text[2] = guienv->addStaticText(L"Selected Sector", core::rect<s32>(10,50,150,70), false);
	text[3] = guienv->addStaticText(L"Rendered Sectors", core::rect<s32>(10,70,150,90), false);
	// Placeholders for the actual data
	text[4] = guienv->addStaticText(L"-", core::rect<s32>(135,10,360,30), false);
	text[5] = guienv->addStaticText(L"-", core::rect<s32>(135,30,360,50), false);
	text[6] = guienv->addStaticText(L"-", core::rect<s32>(135,50,360,70), false);
	text[7] = guienv->addStaticText(L"-", core::rect<s32>(135,70,360,90), false);

	Log(L"Creating city scene");

	// Create city
	city = new CityScene(
		device,
		dimension2di(city_size, city_size),
		dimension2di(sector_size, sector_size),
		vector3df(building_size, building_height, building_size));
}

void SceneView::PreRender()
{
	if(showDataIcons)
	{
		// UAVs
		while(pDoc->uavs.size() > bUAVs.size())
		{
			IBillboardSceneNode * bnode = 0;
			bnode = smgr->addBillboardSceneNode(0, dimension2df(16,16), vector3df(100,-500,100));
			bnode->setMaterialFlag(EMF_LIGHTING, false);
			bnode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
			bnode->setMaterialTexture(0, driver->getTexture("../media/navmap/uav.png"));
			bUAVs.push_back(bnode);
		}
		int i = 0;
		for(pDoc->uav_it = pDoc->uavs.begin(); pDoc->uav_it != pDoc->uavs.end(); pDoc->uav_it++)
		{
			bUAVs[i]->setPosition((*pDoc->uav_it)->getPosition());
			bUAVs[i]->setColor((*pDoc->uav_it)->getColor());
			i++;
		}

		// Waypoints
		while(pDoc->wps.size() > bWPs.size())
		{
			IBillboardSceneNode * bnode = 0;
			bnode = smgr->addBillboardSceneNode(0, dimension2df(16,16), vector3df(100,-500,100));
			bnode->setMaterialFlag(EMF_LIGHTING, false);
			bnode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
			bnode->setMaterialTexture(0, driver->getTexture("../media/navmap/waypoint.png"));
			bWPs.push_back(bnode);
		}
		i = 0;
		for(pDoc->wp_it = pDoc->wps.begin(); pDoc->wp_it != pDoc->wps.end(); pDoc->wp_it++)
		{
			bWPs[i]->setPosition((*pDoc->wp_it)->getPosition());
			bWPs[i]->setColor((*pDoc->wp_it)->getColor());
			i++;
		}

		// Bases
		while(pDoc->bases.size() > bBases.size())
		{
			IBillboardSceneNode * bnode = 0;
			bnode = smgr->addBillboardSceneNode(0, dimension2df(16,16), vector3df(100,-500,100));
			bnode->setMaterialFlag(EMF_LIGHTING, false);
			bnode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
			bnode->setMaterialTexture(0, driver->getTexture("../media/navmap/base.png"));
			bBases.push_back(bnode);
		}
		i = 0;
		for(pDoc->base_it = pDoc->bases.begin(); pDoc->base_it != pDoc->bases.end(); pDoc->base_it++)
		{
			bBases[i]->setPosition((*pDoc->base_it)->getPosition());
			bBases[i]->setColor((*pDoc->base_it)->getColor());
			i++;
		}
	}
}

void SceneView::Render(bool &newMousePos)
{
	PreRender();

	// Update city
	city->show_bounds(showCityBounds, showSectorBounds, showBuildingBounds);
	city->set_camera(camera->getPosition());
	if(newMousePos)
	{
		newMousePos = false;
		position2di cursor = device->getCursorControl()->getPosition();
		if(city->set_selected_sector(cursor))
		{
			showSelectedSector = true;
			selectedSector.x = city->get_selected_sector().X;
			selectedSector.y = city->get_selected_sector().Y;
		}
		else
		{
			showSelectedSector = false;
		}
	}

	// Check for fog changes
	if(fogChanged)
	{
		fogChanged = false;
		SColor fog_color(255,194,159,112);
		if(enableFog)
		{
			driver->setFog(
				fog_color,
				true,
				building_size*sector_size*1.f,
				building_size*sector_size*2.f,
				0.05f);
		}
		else
		{
			driver->setFog(
				fog_color,
				true,
				camera->getFarValue() + 100.f,
				camera->getFarValue() + 200.f,
				0.00f);
		}
	}

	// Find the stats to display
	wchar_t tmp[1024];
	// FPS
	s32 fps = driver->getFPS();
	swprintf(tmp, 1024, L"%i", fps);
	text[4]->setText(tmp);

	// Current sector
	s32 cam_x = city->get_cam_sector().Width;
	s32 cam_y = city->get_cam_sector().Height;
	swprintf(tmp, 1024, L"%i, %i", cam_x, cam_y);
	text[5]->setText(tmp);

	// Selected sector
	if(!showSelectedSector)
		swprintf(tmp, 1024, L"-");
	else
		swprintf(tmp, 1024, L"%i, %i", selectedSector.x, selectedSector.y);
	text[6]->setText(tmp);

	// Visible sectors
	s32 sectors = city->get_visible_sectors();
	s32 total_sectors = city->get_total_sectors();
	swprintf(tmp, 1024, L"%i/%i", sectors, total_sectors);
	text[7]->setText(tmp);

	// Draw the scene
	driver->beginScene(true, true, irr::video::SColor(255,50,50,50));
	city->prerender();
	smgr->drawAll();
	city->render();
	for(int i = 0; i < 8; i++)
		text[i]->draw();

	// Draw the billboards
	if(showDataIcons)
	{
		for(unsigned int i = 0; i < bUAVs.size(); i++)
			bUAVs[i]->render();
		for(unsigned int i = 0; i < bWPs.size(); i++)
			bWPs[i]->render();
		for(unsigned int i = 0; i < bBases.size(); i++)
			bBases[i]->render();
	}

	driver->endScene();
}

void SceneView::JumpToSectorFromAbsolute(irr::f32 x, irr::f32 y)
{
	f32 cityWidth = city_size * sector_size * building_size;
	f32 cityHeight = cityWidth;
	if(x >= 0 && x < cityWidth && y >= 0 && y < cityWidth)
	{
		int secX = int(x / (f32(sector_size) * building_size));
		int secY = int(y / (f32(sector_size) * building_size));
		JumpToSector(secX, secY);
	}
}

bool SceneView::JumpToSector(int x, int y)
{
	s32 cityWidth = city_size;
	s32 cityHeight = cityWidth;
	if(x >= 0 && y >= 0 && x < cityWidth && y < cityHeight)
	{
		// move to the sector
		vector3df camPos;
		camPos.X = f32(sector_size * building_size * x);
		camPos.Y = 600.f;
		camPos.Z = f32(sector_size * building_size * y);
		camera->setPosition(camPos);

		// look toward the sector
		vector3df camDir;
		camDir.X = .5f;
		camDir.Y = -1.f;
		camDir.Z = .5f;
		camDir *= 100.f;
		camera->setTarget(camPos + camDir);

		return true;
	}
	return false;
}

bool SceneView::IsCurrentSceneLoaded()
{
	return city->is_scene_loaded(selectedSector.x, selectedSector.y);
}

void SceneView::RemoveCurrentScene()
{
	// If there's no selected sector, we can't delete from it
	if(!showSelectedSector)
		return;

	if(IsCurrentSceneLoaded())
	{
		// Prompt the user to see if they actually want to delete the scene
		int response = AfxMessageBox(L"Delete this scene?", MB_YESNO|MB_ICONQUESTION);
		if(response == IDYES)
		{
			// Remove the scene from the city
			city->remove_scene(selectedSector.x, selectedSector.y);

			// Remove the scene from the model data
			for(pDoc->scene_it = pDoc->scenes.begin(); pDoc->scene_it != pDoc->scenes.end(); pDoc->scene_it++)
			{
				if((*(pDoc->scene_it))->x == selectedSector.x && (*(pDoc->scene_it))->y == selectedSector.y)
				{
					delete (*(pDoc->scene_it));
					pDoc->scenes.erase(pDoc->scene_it);
					break;
				}
			}

			// Refresh the Scene View
			CFileView &view = ((CMainFrame*)theApp.GetMainWnd())->GetFileView();
			view.FillFileView();
			int index = 0;
			for(pDoc->scene_it = pDoc->scenes.begin(); pDoc->scene_it != pDoc->scenes.end(); pDoc->scene_it++)
			{
				CString str((*(pDoc->scene_it))->name.c_str());
				view.AddScene(str, index++);
			}
		}
	}
}

void SceneView::LoadCurrentScene()
{
	// If there's no selected sector, we can't load to it
	if(!showSelectedSector)
		return;

	if(!IsCurrentSceneLoaded())
	{
		CFileDialog fOpenDlg(TRUE, L"irr", L"", OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR,
			L"UAV Scene Files (*.irr)|*.irr||");

		fOpenDlg.m_pOFN->lpstrTitle = L"Select UAV Scene File";
		fOpenDlg.m_pOFN->lpstrInitialDir = L"../";

		// Get the current directory and transform it to the media/scenes/ folder
		wchar_t lpBuffer[1024];
		GetCurrentDirectory(1024, lpBuffer);
		stringw dir = lpBuffer;
		s32 slash = dir.findLast(wchar_t('\\'));
		dir = dir.subString(0, slash + 1);
		dir += "media\\scenes";

		if(fOpenDlg.DoModal() == IDOK)
		{
			// Convert the filename to ASCII
			CString file = fOpenDlg.GetFileName();
			char buffer[1024];
			wcstombs(buffer, file, 1024);
			
			// Load in the file
			stringc irrFile = "../media/scenes/";
			irrFile += buffer;

			// Add the scene
			stringw sceneName = "Scene ";
			sceneName += ++pDoc->sceneCount;
			AddScene(irrFile, sceneName, selectedSector.x, selectedSector.y);
		}
	}
	else
	{
		AfxMessageBox(L"There is already a scene loaded into this sector.\nYou must delete it before loading a new scene in.");
	}
}

void SceneView::AddScene(const irr::core::stringc &file, const irr::core::stringw &name, int x, int y)
{
	// Create the scene
	city->load_scene(x, y, file, device);

	// Add the scene to the list of scenes
	pDoc->scenes.push_back(new SceneObject(file, name, x, y));

	// Add the scene to the Scene View
	CFileView &view = ((CMainFrame*)theApp.GetMainWnd())->GetFileView();
	int index = pDoc->scenes.size() - 1;
	view.AddScene(name.c_str(), index);
}

void SceneView::HandleKeyInput(irr::f32 timeDiff, std::vector<bool> &keys)
{
	if(keys[KEY_W] || keys[KEY_S] || keys[KEY_A] || keys[KEY_D] || keys[KEY_Q] || keys[KEY_E])
	{
		// Get position, rotation, etc.
		vector3df pos = camera->getPosition();
		vector3df target =(camera->getTarget() - camera->getAbsolutePosition());
		vector3df moveDir = target.normalize();

		// Forward/backward
		if(keys[KEY_W])	pos += moveDir * (f32)timeDiff * MOVE_SPEED;
		if(keys[KEY_S])	pos -= moveDir * (f32)timeDiff * MOVE_SPEED;

		// Left/right
		moveDir = moveDir.crossProduct(camera->getUpVector()).normalize();
		if(keys[KEY_A])	pos += moveDir * (f32)timeDiff * MOVE_SPEED;
		if(keys[KEY_D])	pos -= moveDir * (f32)timeDiff * MOVE_SPEED;

		// Up/down
		moveDir = camera->getUpVector();
		if(keys[KEY_E])	pos += moveDir * (f32)timeDiff * MOVE_SPEED;
		if(keys[KEY_Q])	pos -= moveDir * (f32)timeDiff * MOVE_SPEED;

		// Set the camera position
		camera->setTarget(pos + target);
		camera->setPosition(pos);
	}
}

void SceneView::HandleMouseInput(irr::f32 timeDiff, CPoint relMousePos)
{
	// Grab the camera data
	vector3df pos = camera->getPosition();
	vector3df target =(camera->getTarget() - camera->getAbsolutePosition());
	vector3df relativeRotation = target.getHorizontalAngle();

	// Convert mouse movement to rotation
	f32 rotateSpeed = 0.1f;
	relativeRotation.Y -= relMousePos.x * rotateSpeed;
	relativeRotation.X -= relMousePos.y * rotateSpeed;

	// Don't rotate all the way around
	f32 MaxVerticalAngle = 88.0f;
	if(relativeRotation.X > MaxVerticalAngle*2 && relativeRotation.X < 360.0f-MaxVerticalAngle)
		relativeRotation.X = 360.0f-MaxVerticalAngle;
	else if(relativeRotation.X > MaxVerticalAngle && relativeRotation.X < 360.0f-MaxVerticalAngle)
		relativeRotation.X = MaxVerticalAngle;

	// Rotate with a matrix
	matrix4 mat;
	target.set(0,0, max_(1.f, pos.getLength()));
	mat.setRotationDegrees(vector3df(relativeRotation.X, relativeRotation.Y, 0));
	mat.transformVect(target);

	// Set camera position and rotation
	camera->setPosition(pos);
	camera->setTarget(target + pos);
}

void SceneView::HandleBoundaryInput()
{
	// Grab the old Y value
	oldPosY = camera->getPosition().Y;
}

void SceneView::HandleMoveBoundaries(bool shiftHeld)
{
	// Find the new position (snap to old position if shift was held)
	vector3df camPos = camera->getPosition();
	if(shiftHeld)				camPos.Y = oldPosY;
	if(camPos.Y > MAX_HEIGHT)	camPos.Y = MAX_HEIGHT;
	if(camPos.Y < MIN_HEIGHT)	camPos.Y = MIN_HEIGHT;

	// Make sure the new position isn't out of bounds
	if(camPos.X < MIN_X)	camPos.X = MIN_X;
	if(camPos.Z < MIN_Y)	camPos.Z = MIN_Y;
	if(camPos.X > MAX_X)	camPos.X = MAX_X;
	if(camPos.Z > MAX_Y)	camPos.Z = MAX_Y;

	// Update the target by the same amount
	vector3df target = camera->getTarget();
	vector3df oldCam = camera->getPosition();
	target.X += camPos.X - oldCam.X;
	target.Y += camPos.Y - oldCam.Y;
	target.Z += camPos.Z - oldCam.Z;

	// Apply new camera settings
	camera->setTarget(target);
	camera->setPosition(camPos);
}