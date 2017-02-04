#pragma once
#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <irrlicht.h>

struct SceneObject
{
	SceneObject(const irr::core::stringc &file, const irr::core::stringw &name, int x, int y) : file(file), name(name), x(x), y(y) {}
	int x;	// x sector
	int y;	// y sector
	irr::core::stringc file;
	irr::core::stringw name;
};

#endif