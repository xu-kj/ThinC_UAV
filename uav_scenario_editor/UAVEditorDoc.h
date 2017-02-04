
// UAVEditorDoc.h : interface of the CUAVEditorDoc class
//


#pragma once
#include <irrlicht.h>
#include <vector>
#include "Object.h"
#include "Waypoint.h"
#include "UAV.h"
#include "Scene.h"

class CUAVEditorDoc : public CDocument
{
protected: // create from serialization only
	CUAVEditorDoc();
	DECLARE_DYNCREATE(CUAVEditorDoc)

// Attributes
public:
	CCriticalSection dataLock;

	// General Model Data
	irr::core::dimension2df worldSize;

	// Model Data Objects
	std::vector<UAVObject *> uavs;
	std::vector<SimObject *> bases;
	std::vector<WaypointObject *> wps;
	std::vector<SimObject *> ufos;
	std::vector<SceneObject *> scenes;

	// Model Data Iterators
	std::vector<UAVObject *>::iterator uav_it;
	std::vector<SimObject *>::iterator base_it;
	std::vector<WaypointObject *>::iterator wp_it;
	std::vector<SimObject *>::iterator ufo_it;
	std::vector<SceneObject *>::iterator scene_it;

	// Model Data Counters
	int uavCount;
	int baseCount;
	int wpCount;
	int ufoCount;
	int sceneCount;

// Operations
public:
	int GetParentUAVId(SimObject * parent);
	void ReFillObjectEventView();
	void DeleteWaypoint(WaypointObject *wp);
	void DeleteBase(SimObject * base);
	void DeleteUAV(UAVObject * uav);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void DeleteContents();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CUAVEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

// Loading and saving functions
private:
	void SerializeScenario(CArchive& ar);

	void SerializeUAV(CArchive& ar, UAVObject * obj, int color);
	void SerializeScene(CArchive& ar, SceneObject * obj);

	void SerializeBase(CArchive& ar, SimObject * obj, int spacer = 0);
	void SerializeWaypoint(CArchive& ar, WaypointObject * obj, int spacer = 0);

	void SerializeSpacer(CArchive& ar, int spacer);
	void SerializeInteger(CArchive& ar, const irr::core::stringc &name, int value, int spacer = 0);
	void SerializeFloat(CArchive& ar, const irr::core::stringc &name, float value, int spacer = 0);
	void SerializeBool(CArchive& ar, const irr::core::stringc &name, bool value, int spacer = 0);
	void SerializeString(CArchive& ar, const irr::core::stringc &name, const irr::core::stringc &value, int spacer = 0);
	void SerializeVector(CArchive& ar, const irr::core::stringc &name, irr::core::vector3df vector, int spacer = 0);
};


