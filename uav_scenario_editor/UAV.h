#pragma once
#ifndef UAV_OBJECT_H
#define UAV_OBJECT_H

#include "Object.h"
#include "Waypoint.h"
#include <list>

class UAVObject : public SimObject
{
public:
	// Constructor
	UAVObject(
		const irr::core::stringw &name,
		const irr::core::vector3df &position,
		const irr::video::SColor &color = irr::video::SColor(255,255,255,255));

	// SubObjects
	void assignBase(SimObject * base_);
	void assignWaypoint(WaypointObject * wp);
	void removeWaypoint(WaypointObject * wp);

	// Rotation
	irr::core::vector3df getForward() const {return facing;}
	irr::f32 getRotAngle2D() const;
	void reOrient();

	// Fuel
	irr::f32 getFuel() const {return fuel;}
	void setFuel(float amount) { fuel = amount; }

	// Terrible
	const std::list<WaypointObject *> * getWPList() const {return &wps;}
	const SimObject * getBase() const {return base;}

	// Distances and times
	irr::f32 getSpeed() const {return speed;}
	irr::f32 getDistance(SimObject * obj) const;
	irr::f32 getWPDistance(WaypointObject * wp) const;
	irr::f32 getBaseDistance() const;
	irr::f32 getDistanceFromLastWP(SimObject * obj) const;

private:
	// SubObjects
	SimObject * base;
	std::list<WaypointObject *> wps;

	// Rotation
	irr::core::vector3df facing;

	// Speed
	irr::f32 speed;

	// Fuel
	irr::f32 fuel;
};

#endif