#pragma once
#ifndef SIM_OBJECT_H
#define SIM_OBJECT_H

#include <irrlicht.h>

class SimObject
{
public:
	SimObject::SimObject(
		const irr::core::stringw &name,
		const irr::core::vector3df &position = irr::core::vector3df(0.f,0.f,0.f),
		const irr::video::SColor &color = irr::video::SColor(255,255,255,255),
		SimObject * parent = 0)
	: name(name), position(position), color(color), parent(parent)
	{}

	// Setters
	void setColor(irr::video::SColor color) {this->color = color;}
	void setPosition(irr::core::vector3df position) {this->position = position;}
	void setParent(SimObject * parent) {this->parent = parent;}

	// Getters
	irr::core::vector3df	getPosition() const		{return position;}
	irr::core::position2df	getPosition2D() const	{return irr::core::position2df(position.X, position.Z);}
	irr::core::stringw		getName() const			{return name;}
	irr::video::SColor		getColor() const		{return color;}
	SimObject			  * getParent() const		{return parent;}

protected:
	irr::core::vector3df position;
	irr::core::stringw name;
	irr::video::SColor color;
	SimObject * parent;
};

#endif