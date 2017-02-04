
// UAVEditorDoc.cpp : implementation of the CUAVEditorDoc class
//

#include "stdafx.h"
#include "UAVEditor.h"

#include "UAVEditorDoc.h"
#include "MainFrm.h"
#include "ClassView.h"
#include <list>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUAVEditorDoc

IMPLEMENT_DYNCREATE(CUAVEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CUAVEditorDoc, CDocument)
END_MESSAGE_MAP()


// CUAVEditorDoc construction/destruction

CUAVEditorDoc::CUAVEditorDoc()
{
	uavCount = baseCount = wpCount = ufoCount = sceneCount = 0;
}

CUAVEditorDoc::~CUAVEditorDoc()
{
}

int CUAVEditorDoc::GetParentUAVId(SimObject * parent)
{
	int id = 0;
	for(uav_it = uavs.begin(); uav_it != uavs.end(); uav_it++)
	{
		if(*uav_it == parent)
			return id;
		id++;
	}
	return -1;
}

void CUAVEditorDoc::ReFillObjectEventView()
{
	CObjectEventView &view = ((CMainFrame*)theApp.GetMainWnd())->GetClassView();
	view.ClearClassView();
	view.FillBaseClassView();

	// Add UAVs
	int id = 0;
	for(uav_it = uavs.begin(); uav_it != uavs.end(); uav_it++)
		view.AddObject((*uav_it)->getName().c_str(), ITEM_UAV, id++);

	// Add waypoints
	id = 0;
	for(wp_it = wps.begin(); wp_it != wps.end(); wp_it++)
	{
		int uavId = GetParentUAVId((*wp_it)->getParent());
		view.AddObject((*wp_it)->getName().c_str(), ITEM_WAYPOINT, id++, uavId);
	}

	// Add bases
	id = 0;
	for(base_it = bases.begin(); base_it !=bases.end(); base_it++)
	{
		int uavId = GetParentUAVId((*base_it)->getParent());
		view.AddObject((*base_it)->getName().c_str(), ITEM_BASE, id++, uavId);
	}

	//TODO: UFOs
}

void CUAVEditorDoc::DeleteWaypoint(WaypointObject * wp)
{
	// If the waypoint was assigned to a UAV, unassign it
	if(wp->getParent())
		((UAVObject *)wp->getParent())->removeWaypoint(wp);

	// Remove the waypoint from the model
	for(wp_it = wps.begin(); wp_it != wps.end(); wp_it++)
	{
		if(*wp_it == wp)
		{
			delete *wp_it;
			wps.erase(wp_it);
			break;
		}
	}
	// Refresh the Object/Event View
	ReFillObjectEventView();
}

void CUAVEditorDoc::DeleteBase(SimObject * base)
{
	// Remove the base from the model
	for(base_it = bases.begin(); base_it != bases.end(); base_it++)
	{
		if(*base_it == base)
		{
			delete *base_it;
			bases.erase(base_it);
			break;
		}
	}
	// Refresh the Object/Event View
	ReFillObjectEventView();
}

void CUAVEditorDoc::DeleteUAV(UAVObject * uav)
{
	// Delete waypoints
	while(uav->getWPList()->size())
		DeleteWaypoint(uav->getWPList()->front());

	// Delete base
	if(uav->getBase())
	{
		SimObject * base = const_cast<SimObject *>(uav->getBase());
		DeleteBase(base);
	}

	// Remove the UAV from the model
	for(uav_it = uavs.begin(); uav_it != uavs.end(); uav_it++)
	{
		if(*uav_it == uav)
		{
			delete *uav_it;
			uavs.erase(uav_it);
			break;
		}
	}
	// Refresh the Object/Event View
	ReFillObjectEventView();
}

BOOL CUAVEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

void CUAVEditorDoc::DeleteContents()
{
	for(uav_it = uavs.begin(); uav_it != uavs.end(); uav_it++)
		delete *uav_it;
	for(base_it = bases.begin(); base_it != bases.end(); base_it++)
		delete *base_it;
	for(wp_it = wps.begin(); wp_it != wps.end(); wp_it++)
		delete *wp_it;
	for(ufo_it = ufos.begin(); ufo_it != ufos.end(); ufo_it++)
		delete *ufo_it;
	for(scene_it = scenes.begin(); scene_it != scenes.end(); scene_it++)
		delete *scene_it;

	uavs.clear();
	bases.clear();
	wps.clear();
	ufos.clear();
	scenes.clear();
}

// CUAVEditorDoc serialization

void CUAVEditorDoc::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		dataLock.Lock();
		SerializeScenario(ar);
		dataLock.Unlock();
	}
	else
	{
		// TODO: add loading code here
		AfxMessageBox(L"Load");
	}
}


// CUAVEditorDoc diagnostics

#ifdef _DEBUG
void CUAVEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUAVEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Loading and saving functions
void CUAVEditorDoc::SerializeScenario(CArchive& ar)
{
	// header
	stringc str;
	str += "<!-- UAV Scenario created with the UAV Simulation Editor -->\n";
	str += "<!-- THInC Lab, University of Michigan -->\n";
	str += "<?xml version=\"1.0\"?>\n\n";
	ar.Write(str.c_str(), str.size());

	// Show all the UAVs
	int count = 0;
	for(uav_it = uavs.begin(); uav_it != uavs.end(); uav_it++)
	{
		stringc str = "<!-- UAV "; str += ++count; str += " -->\n";
		ar.Write(str.c_str(), str.size());
		SerializeUAV(ar, *uav_it, count);
	}

	// Show all the scenes
	count = 0;
	for(scene_it = scenes.begin(); scene_it != scenes.end(); scene_it++)
	{
		stringc str = "<!-- SCENE "; str += ++count; str += " -->\n";
		ar.Write(str.c_str(), str.size());
		SerializeScene(ar, *scene_it);
	}

	// Show unassigned bases
	count = 0;
	for(base_it = bases.begin(); base_it != bases.end(); base_it++)
	{
		if(!(*base_it)->getParent())
		{
			stringc str = "<!-- UNASSIGNED BASE "; str += ++count; str += " -->\n";
			ar.Write(str.c_str(), str.size());
			SerializeBase(ar, *base_it);
			str = "\n"; ar.Write(str.c_str(), str.size());
		}
	}

	// Show unassigned waypoints
	count = 0;
	for(wp_it = wps.begin(); wp_it != wps.end(); wp_it++)
	{
		if(!(*wp_it)->getParent())
		{
			stringc str = "<!-- UNASSIGNED WAYPOINT "; str += ++count; str += " -->\n";
			ar.Write(str.c_str(), str.size());
			SerializeWaypoint(ar, *wp_it);
			str = "\n"; ar.Write(str.c_str(), str.size());
		}
	}
}

void CUAVEditorDoc::SerializeUAV(CArchive& ar, UAVObject * obj, int color)
{
	stringc str = "<uav>\n";
	ar.Write(str.c_str(), str.size());

	// Name
	SerializeString(ar, "Name", obj->getName(), 4);

	// Position
	SerializeFloat(ar, "PosX", obj->getPosition().X, 4);
	SerializeFloat(ar, "PosY", obj->getPosition().Y, 4);
	SerializeFloat(ar, "PosZ", obj->getPosition().Z, 4);

	// Color
	SerializeInteger(ar, "Color", color, 4);

	// Fuel
	SerializeFloat(ar, "Fuel", obj->getFuel(), 4);

	str = "\n"; ar.Write(str.c_str(), str.size());

	// Write the base
	SerializeBase(ar, const_cast<SimObject *>(obj->getBase()), 4);

	// Write all the waypoints
	std::list<WaypointObject *>::const_iterator itr;
	for(itr = obj->getWPList()->begin(); itr != obj->getWPList()->end(); itr++)
		SerializeWaypoint(ar, *itr, 4);

	str = "</uav>\n\n";
	ar.Write(str.c_str(), str.size());
}

void CUAVEditorDoc::SerializeScene(CArchive& ar, SceneObject * obj)
{
	stringc str = "<scene>\n";
	ar.Write(str.c_str(), str.size());

	// Name
	SerializeString(ar, "Name", obj->name.c_str(), 4);

	// File
	SerializeString(ar, "File", obj->file.c_str(), 4);

	// Position
	SerializeInteger(ar, "CityX", obj->x, 4);
	SerializeInteger(ar, "CityY", obj->y, 4);

	str = "</scene>\n\n";
	ar.Write(str.c_str(), str.size());
}

void CUAVEditorDoc::SerializeBase(CArchive& ar, SimObject * obj, int spacer)
{
	stringc str = "<base>\n";
	SerializeSpacer(ar, spacer);
	ar.Write(str.c_str(), str.size());

	// Name
	SerializeString(ar, "Name", obj->getName(), spacer + 4);

	// Position
	SerializeFloat(ar, "PosX", obj->getPosition().X, spacer + 4);
	SerializeFloat(ar, "PosY", obj->getPosition().Y, spacer + 4);
	SerializeFloat(ar, "PosZ", obj->getPosition().Z, spacer + 4);

	str = "</base>\n";
	if(spacer > 0) str += "\n";
	SerializeSpacer(ar, spacer);
	ar.Write(str.c_str(), str.size());
}

void CUAVEditorDoc::SerializeWaypoint(CArchive& ar, WaypointObject * obj, int spacer)
{
	stringc str = "<waypoint>\n";
	SerializeSpacer(ar, spacer);
	ar.Write(str.c_str(), str.size());

	// Name
	SerializeString(ar, "Name", obj->getName(), spacer + 4);

	// Position
	SerializeFloat(ar, "PosX", obj->getPosition().X, spacer + 4);
	SerializeFloat(ar, "PosY", obj->getPosition().Y, spacer + 4);
	SerializeFloat(ar, "PosZ", obj->getPosition().Z, spacer + 4);
	SerializeBool(ar, "Feature", obj->getFeature(), spacer + 4);

	str = "</waypoint>\n";
	SerializeSpacer(ar, spacer);
	ar.Write(str.c_str(), str.size());
}

void CUAVEditorDoc::SerializeSpacer(CArchive& ar, int spacer)
{
	stringc str;
	while(spacer-- > 0)
		str += " ";
	ar.Write(str.c_str(), str.size());
}

void CUAVEditorDoc::SerializeInteger(CArchive& ar, const irr::core::stringc &name, int value, int spacer)
{
	stringc str = "<int name=\"";
	str += name;
	str += "\" value=\"";
	str += value;
	str += "\" />\n";

	SerializeSpacer(ar, spacer);
	ar.Write(str.c_str(), str.size());
}

void CUAVEditorDoc::SerializeFloat(CArchive& ar, const irr::core::stringc &name, float value, int spacer)
{
	stringc str = "<float name=\"";
	str += name;
	str += "\" value=\"";
	str += value;
	str += "\" />\n";

	SerializeSpacer(ar, spacer);
	ar.Write(str.c_str(), str.size());
}

void CUAVEditorDoc::SerializeBool(CArchive& ar, const irr::core::stringc &name, bool value, int spacer)
{
	stringc str = "<bool name=\"";
	str += name;
	str += "\" value=\"";
	if(value)	str += "True";
	else		str += "False";
	str += "\" />\n";

	SerializeSpacer(ar, spacer);
	ar.Write(str.c_str(), str.size());
}

void CUAVEditorDoc::SerializeString(CArchive& ar, const irr::core::stringc &name, const irr::core::stringc &value, int spacer)
{
	stringc str = "<string name=\"";
	str += name;
	str += "\" value=\"";
	str += value;
	str += "\" />\n";

	SerializeSpacer(ar, spacer);
	ar.Write(str.c_str(), str.size());
}

void CUAVEditorDoc::SerializeVector(CArchive& ar, const irr::core::stringc &name, irr::core::vector3df vector, int spacer)
{
}