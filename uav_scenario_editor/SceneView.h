#pragma once
#ifndef SCENE_VIEW_H
#define SCENE_VIEW_H

#include <irrlicht.h>
#include <vector>
#include <list>
#include "CityScene.h"

class CUAVEditorDoc;

class SceneView
{
public:
	SceneView();
	~SceneView();

	void Log(LPCTSTR lpszString);
	void SetUp(CUAVEditorDoc * pDoc, irr::IrrlichtDevice * device, irr::scene::ICameraSceneNode * camera);
	void PreRender();
	void Render(bool &newMousePos);
	bool JumpToSector(int x, int y);
	void JumpToSectorFromAbsolute(irr::f32 x, irr::f32 y);
	bool IsCurrentSceneLoaded();
	void RemoveCurrentScene();
	void LoadCurrentScene();
	void AddScene(const irr::core::stringc &file, const irr::core::stringw &name, int x, int y);

	// Input
	void HandleKeyInput(irr::f32 timeDiff, std::vector<bool> &keys);
	void HandleMouseInput(irr::f32 timeDiff, CPoint relMousePos);
	void HandleBoundaryInput();
	void HandleMoveBoundaries(bool shiftHeld);

	// User view choices
	bool enableFog;
	bool fogChanged;
	bool showCityBounds;
	bool showSectorBounds;
	bool showBuildingBounds;
	bool showDataIcons;
	bool showSelectedSector;
	CPoint selectedSector;

private:
	// World/Model data
	CUAVEditorDoc * pDoc;

	// Easy access to irrlicht
	irr::IrrlichtDevice			 * device;
	irr::video::IVideoDriver	 * driver;
	irr::scene::ISceneManager	 * smgr;
	irr::gui::IGUIEnvironment	 * guienv;
	irr::scene::ICameraSceneNode * camera;

	// The city
	CityScene * city;

	// Stats text and billboards
	irr::gui::IGUIStaticText * text[10];
	std::vector<irr::scene::IBillboardSceneNode *> bUAVs;
	std::vector<irr::scene::IBillboardSceneNode *> bWPs;
	std::vector<irr::scene::IBillboardSceneNode *> bBases;

	// Movement stuff
	irr::f32 oldPosY;	// old Y value in case SHIFT is held
};

#endif