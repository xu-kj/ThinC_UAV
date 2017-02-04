#pragma once
#ifndef WAYPOINT_OBJECT_H
#define WAYPOINT_OBJECT_H

#include "Object.h"
#include "ModelGlobals.h"

class WaypointObject: public SimObject
{
public:
	WaypointObject(
		const irr::core::stringw &name,
		const irr::core::vector3df &position,
		const irr::video::SColor &color = irr::video::SColor())
	: SimObject(name, position, color), state(UNASSIGNED), has_feature(true)
	{setUnAssigned();}

	void setAssigned(irr::video::SColor color)		{state = ASSIGNED; this->color = color;}
	void setReAssigned(irr::video::SColor color)	{state = REASSIGNED; this->color = color;}
	void setUnAssigned()	{state = UNASSIGNED; color = WAYPOINT_UNSASSIGNED;}
	void setConfirmed()		{state = CONFIRMED; color = WAYPOINT_CONFIRMED;}
	void setCleared()		{state = DENIED; color = WAYPOINT_CLEAR;}
	void setUnsure()		{state = UNSURE; color = WAYPOINT_UNSURE;}

	void setFeature(bool exists) {has_feature = exists;}
	bool getFeature() const {return has_feature;}

private:
	enum E_WP_STATE {UNASSIGNED, ASSIGNED, REASSIGNED, CONFIRMED, DENIED, UNSURE};
	E_WP_STATE state;
	bool has_feature;
};

#endif