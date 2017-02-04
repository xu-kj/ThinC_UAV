#include "stdafx.h"
#include "UAV.h"

const double DEGREES_IN_RAD = 57.2957795;
irr::f32 SPEED_FACTOR = 100.f;
irr::f32 TURN_FACTOR = 1.f;

using namespace irr;
using namespace core;

UAVObject::UAVObject(
	const irr::core::stringw &name,
	const irr::core::vector3df &position,
	const irr::video::SColor &color)
: SimObject(name, position, color), fuel(1.f), base(0)
{
	facing = vector3df(1.f, 0, 1.f);
	speed = SPEED_FACTOR;
}

void UAVObject::assignBase(SimObject * base)
{
	this->base = base;
	this->base->setParent(this);
	this->base->setColor(getColor());
}

void UAVObject::assignWaypoint(WaypointObject * wp)
{
	wp->setAssigned(getColor());
	wp->setParent(this);
	wps.push_back(wp);

	reOrient();
}

void UAVObject::removeWaypoint(WaypointObject * wp)
{
	std::list<WaypointObject *>::iterator itr;
	for(itr = wps.begin(); itr != wps.end(); itr++)
	{
		if(*itr == wp)
		{
			wp->setUnAssigned();
			wps.erase(itr);
			break;
		}
	}
}

irr::f32 UAVObject::getRotAngle2D() const
{
	irr::f32 angle = atan(facing.X / (-facing.Z));
	angle *= irr::f32(DEGREES_IN_RAD);

	if(facing.Z >= 0)
		angle += 180.0f;
	//angle += 180.0f;

	return angle;
}

void UAVObject::reOrient()
{
	if(wps.size() > 0)
		facing = (position - wps.front()->getPosition()).normalize();
}

irr::f32 UAVObject::getDistance(SimObject * obj) const
{
	if(obj == base)
		return getBaseDistance();
	else
		return getWPDistance((WaypointObject *)obj);
}

irr::f32 UAVObject::getWPDistance(WaypointObject * wp) const
{
	WaypointObject * lastWP = 0;
	WaypointObject * thisWP = 0;
	f32 totalDistance = 0.f;

	if(wps.size() > 0)
		totalDistance += abs(wps.front()->getPosition().getDistanceFrom(getPosition()));

	std::list<WaypointObject *>::const_iterator itr;
	for(itr = wps.begin(); itr != wps.end(); itr++)
	{
		lastWP = thisWP;
		thisWP = *itr;
		if(lastWP && thisWP)
		{
			f32 dist = thisWP->getPosition().getDistanceFrom(lastWP->getPosition());
			totalDistance += abs(dist);
		}
		if(thisWP == wp)
			break;
	}

	return totalDistance;
}

irr::f32 UAVObject::getBaseDistance() const
{
	WaypointObject * thisWP = wps.back();
	f32 totalDistance = getWPDistance(wps.back());

	if(base && thisWP)
	{
		f32 dist = base->getPosition().getDistanceFrom(thisWP->getPosition());
		totalDistance += abs(dist);
	}

	return totalDistance;
}

irr::f32 UAVObject::getDistanceFromLastWP(SimObject * obj) const
{
	if(obj == base)
	{
		WaypointObject * thisWP = wps.back();
		if(base && thisWP)
			return abs(base->getPosition().getDistanceFrom(thisWP->getPosition()));
	}
	else
	{
		WaypointObject * lastWP = 0;
		WaypointObject * thisWP = 0;
		f32 totalDistance = 0.f;

		std::list<WaypointObject *>::const_iterator itr;
		for(itr = wps.begin(); itr != wps.end(); itr++)
		{
			if(*itr == (WaypointObject *)obj)
			{
				thisWP = *itr;
				break;
			}
			lastWP = *itr;
		}

		if(thisWP && lastWP)
			return abs(thisWP->getPosition().getDistanceFrom(lastWP->getPosition()));
	}

	return -1.f;
}