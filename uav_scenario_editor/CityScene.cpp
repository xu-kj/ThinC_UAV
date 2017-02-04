#include "stdafx.h"
#include "CityScene.h"
#include "Random.h"
#include <irrlicht.h>
#include <iostream>
#include <list>
using namespace std;

#pragma warning(disable : 4996)
#pragma warning(disable : 4482)

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef BLAHBLAHBALH
ISceneNode * readSceneNode(IXMLReader * reader, IrrlichtDevice * device,
	vector3df start)
{
	ISceneManager * smgr = device->getSceneManager();
	ISceneNode * node = 0;

	if (!reader)
		return 0;

	// read node name and type
	if(stringw(L"node") == reader->getNodeName()&&
		stringw(L"mesh") == reader->getAttributeValue(stringw(L"type").c_str()))
	{
		//cout << "<node type=\"mesh\">" << endl;

		vector3df position;
		vector3df rotation;
		vector3df scale;
		stringc mesh;
		stringc node_name;
		s32 id;
		stringc texture1;
		stringc texture2;
		stringc texture3;
		stringc texture4;

		bool endReached = false;
		while(!endReached && reader->read())
		{
			stringw nodeName = reader->getNodeName();
			switch(reader->getNodeType())
			{
			case EXN_ELEMENT_END:
				//if(stringw(L"attributes") == nodeName)
				//	cout << "  </attributes>" << endl;
				//if(stringw(L"materials") == nodeName)
				//	cout << "  </materials>" << endl;
				if(stringw(L"node") == nodeName)
					endReached = true;
				break;

			case EXN_ELEMENT:
				//if(stringw(L"attributes") == nodeName)
				//	cout << "  <attributes>" << endl;

				//else if(stringw(L"materials") == nodeName)
				//	cout << "  <materials>" << endl;

				/*else*/ if(stringw(L"string") == nodeName)
				{
					stringc name = reader->getAttributeValue(stringw(L"name").c_str());
					stringc value = reader->getAttributeValue(stringw(L"value").c_str());

					// assign the value to the correct string
					if(stringw(L"Name") == name)
						node_name = value;
					if(stringw(L"Mesh") == name)
						mesh = value;

					// debug
					//cout << "    <string name=\"";
					//cout << "\" value=\"";
					//cout << value.c_str();
					//cout << "\" />" << endl;
				}

				else if(stringw(L"int") == nodeName)
				{
					stringc name = reader->getAttributeValue(stringw(L"name").c_str());
					int value = reader->getAttributeValueAsInt(stringw(L"value").c_str());

					// assign id if this is the right node
					if(stringw(L"Id") == name)
						id = value;

					// debug
					//cout << "    <int name=\"" << name.c_str() << "\" ";
					//cout << "value=\"" << value;
					//cout << "\" />" << endl;
				}

				else if(stringw(L"vector3d") == nodeName)
				{
					stringc name = reader->getAttributeValue(stringw(L"name").c_str());
					stringc value = reader->getAttributeValue(stringw(L"value").c_str());
					f32 x, y, z;

					// turn the value string into three floats
					// lose a lot of precision, cause i'm lazy as fuuuuuck
					stringc tmp[3];
					u32 j = 0;
					for(u32 i = 0; i < value.size(); i++)
					{
						switch(value[i])
						{
						case ',':
							if(j < 2)	j++;
							break;
						case ' ':
							break;
						default:
							tmp[j] += value[i];
						}
					}
					if(j < 2)	j++;
					sscanf(tmp[0].c_str(), "%f", &x);
					sscanf(tmp[1].c_str(), "%f", &y);
					sscanf(tmp[2].c_str(), "%f", &z);

					// assign the values to the correct vector
					if(stringw(L"Position") == name)
						position = vector3df(x, y, z);
					else if(stringw(L"Rotation") == name)
						rotation = vector3df(x, y, z);
					else if(stringw(L"Scale") == name)
						scale = vector3df(x, y, z);

					// debug
					//cout << "    <vector3d name=\"" << name.c_str() << "\" value=\"";
					//cout << x << ", " << y << ", " << z;
					//cout << "\" />" << endl;
				}

				else if(stringw(L"texture") == nodeName)
				{
					stringc name = reader->getAttributeValue(stringw(L"name").c_str());
					stringc value = reader->getAttributeValue(stringw(L"value").c_str());

					// assign the value to the correct texture string
					if(stringw(L"Texture1") == name)
						texture1 = value.c_str();
					else if(stringw(L"Texture2") == name)
						texture2 = value.c_str();
					else if(stringw(L"Texture3") == name)
						texture3 = value.c_str();
					else if(stringw(L"Texture4") == name)
						texture4 = value.c_str();

					// debug
					//cout << "    <texture name=\"" << name.c_str();
					//cout << "\" value=\"" << value.c_str();
					//cout << "\" />" << endl;
				}

				else
				{
					//cout << "    <unknown node />" << endl;
				}
				break;
			}

			// create the node
			if(endReached && stringw(L"") != mesh)
			{
				//cout << "</node>" << endl << endl;
				//cout << "Creating mesh node...";
				IMesh * imesh = smgr->getMesh(mesh.c_str());
				if(imesh)
				{
					//cout << "done" << endl;
					//cout << "Creating scene node from mesh...";
					node = smgr->addMeshSceneNode(imesh);
					if(node)
					{
						//cout << "done" << endl;
						//cout << texture1.c_str() << endl;

						// load model textures
						//cout << "Loading textures...";
						if(stringw(L"") != texture1)
							node->setMaterialTexture(0, device->getVideoDriver()->
								getTexture(texture1.c_str()));
						if(stringw(L"") != texture2)
							node->setMaterialTexture(1, device->getVideoDriver()->
								getTexture(texture2.c_str()));
						if(stringw(L"") != texture3)
							node->setMaterialTexture(2, device->getVideoDriver()->
								getTexture(texture3.c_str()));
						if(stringw(L"") != texture4)
							node->setMaterialTexture(3, device->getVideoDriver()->
								getTexture(texture4.c_str()));
						//cout << "done" << endl;

						// set position, etc
						node->setPosition(start + position);
						node->setScale(scale);
						node->setRotation(rotation);
						node->setVisible(false);
						for(u32 c = 0; c < node->getMaterialCount(); c++)
						{
							node->getMaterial(c).FogEnable = true;
							node->getMaterial(c).Lighting = false;
						}
					}
					else
					{
						//cout << "failed" << endl;
					}
				}
				else
				{
					//cout << "failed" << endl;
				}
				//cout << endl;
			}
			else if(endReached)
			{
				//cout << "</node>" << endl;
			}
		}
	}

	return node;
}

#endif


ISceneNode * readSceneNode(IXMLReader * reader, IrrlichtDevice * device,
	vector3df start)
{
	ISceneManager * smgr = device->getSceneManager();
	ISceneNode * node = 0;

	if (!reader)
		return 0;

	// read node name and type
	if(stringw(L"node") == reader->getNodeName()&&
		stringw(L"mesh") == reader->getAttributeValue(stringw(L"type").c_str()))
	{
		vector3df position;
		vector3df rotation;
		vector3df scale;
		stringc mesh;
		stringc node_name;
		s32 id;

		bool do_materials = false;
		std::list<SMaterial> materials;

		bool endReached = false;
		while(!endReached && reader->read())
		{
			stringw nodeName = reader->getNodeName();
			switch(reader->getNodeType())
			{
			case EXN_ELEMENT_END:
				if(stringw(L"node") == nodeName)
					endReached = true;
				break;

			case EXN_ELEMENT:
				if(do_materials && stringw(L"attributes") == nodeName)
				{
					io::IAttributes* attr = device->getFileSystem()->createEmptyAttributes(device->getVideoDriver());
					attr->read(reader);

					materials.push_back(SMaterial());
					device->getVideoDriver()->fillMaterialStructureFromAttributes(materials.back(), attr);

					attr->drop();
				}
				else if(stringw(L"materials") == nodeName)
				{
					do_materials = true;
				}
				else if(stringw(L"string") == nodeName)
				{
					stringc name = reader->getAttributeValue(stringw(L"name").c_str());
					stringc value = reader->getAttributeValue(stringw(L"value").c_str());

					// assign the value to the correct string
					if     (stringw(L"Name") == name)	node_name = value;
					else if(stringw(L"Mesh") == name)	mesh = value;

				}
				else if(stringw(L"int") == nodeName)
				{
					stringc name = reader->getAttributeValue(stringw(L"name").c_str());
					int value = reader->getAttributeValueAsInt(stringw(L"value").c_str());

					// assign id if this is the right node
					if(stringw(L"Id") == name)
						id = value;
				}
				else if(stringw(L"vector3d") == nodeName)
				{
					stringc name = reader->getAttributeValue(stringw(L"name").c_str());
					stringc value = reader->getAttributeValue(stringw(L"value").c_str());
					f32 x, y, z;

					// turn the value string into three floats
					// lose a lot of precision, cause i'm lazy as fuuuuuck
					stringc tmp[3];
					u32 j = 0;
					for(u32 i = 0; i < value.size(); i++)
					{
						switch(value[i])
						{
						case ',':
							if(j < 2)	j++;
							break;
						case ' ':
							break;
						default:
							tmp[j] += value[i];
						}
					}
					if(j < 2)	j++;
					sscanf(tmp[0].c_str(), "%f", &x);
					sscanf(tmp[1].c_str(), "%f", &y);
					sscanf(tmp[2].c_str(), "%f", &z);

					// assign the values to the correct vector
					if(stringw(L"Position") == name)
						position = vector3df(x, y, z);
					else if(stringw(L"Rotation") == name)
						rotation = vector3df(x, y, z);
					else if(stringw(L"Scale") == name)
						scale = vector3df(x, y, z);
				}	
				break;
			}

			// create the node
			if(endReached && stringw(L"") != mesh)
			{
				// get the mesh
				IAnimatedMesh * imesh = smgr->getMesh(mesh.c_str());
				if(imesh)
				{
					// create the scene node
					node = smgr->addMeshSceneNode(imesh);
					if(node)
					{
						// set position, etc
						node->setPosition(start + position);
						node->setScale(scale);
						node->setRotation(rotation);
						node->setVisible(false);

						// apply the materials
						std::list<SMaterial>::iterator itr;
						itr = materials.begin();
						for(u32 i = 0; i < node->getMaterialCount(); i++)
						{
							if(itr == materials.end()) break;
							itr->FogEnable = true;
							itr->Lighting = false;
							node->getMaterial(i) = *itr;
							itr++;
						}
					}
				}
			}
		}
	}

	return node;
}

void loadScene(const c8 * file, IrrlichtDevice * device,
	irr::core::list<ISceneNode *> &nodes, vector3df start)
{
	IXMLReader * reader = device->getFileSystem()->createXMLReader(file);
	if(!reader)
	{
		cout << "\nFailed to open scene file \"" << file << "\"" << endl;
	}
	else
	{
		//cout << "\nOpened scene file \"" << file << "\"" << endl;
		while(reader->read())
		{
			ISceneNode * node = readSceneNode(reader, device, start);
			if(node)
				nodes.push_back(node);
		}
		reader->drop();
	}
}

const s32 ROADS_START = 2;
const s32 NUM_ROADS = 3;
const s32 ROOFS_START = 5;
const s32 NUM_ROOFS = 5;
const s32 GROUND = 1;
const s32 B_LARGE_START = 10;
const s32 NUM_B_LARGE = 2;
const s32 B_MEDIUM_START = 12;
const s32 NUM_B_MEDIUM = 2;
const s32 B_SMALL_START = 14;
const s32 NUM_B_SMALL = 2;

const s32 NUM_TEXTURES = NUM_ROADS + NUM_ROOFS + NUM_B_LARGE + NUM_B_MEDIUM +
						 NUM_B_SMALL + 2;
const stringc texture_files[NUM_TEXTURES] = {
		"../media/citytextures/texture.png",
		"../media/citytextures/pebble_dirt.png",
		"../media/citytextures/road1.png",
		"../media/citytextures/road2.png",
		"../media/citytextures/road3.png",
		"../media/citytextures/roof1.png",
		"../media/citytextures/roof2.png",
		"../media/citytextures/roof3.png",
		"../media/citytextures/roof4.png",
		"../media/citytextures/roof5.png",
		"../media/citytextures/building_large1.png",
		"../media/citytextures/building_large2.png",
		"../media/citytextures/building_medium1.png",
		"../media/citytextures/building_medium2.png",
		"../media/citytextures/building_small1.png",
		"../media/citytextures/building_small2.png"
	};

enum E_SIDE_TYPE {S_TOP, S_SIDE1, S_SIDE2, S_SIDE3, S_SIDE4, S_ROAD, S_GROUND};

SMeshBuffer * texturedMeshBuffer(f32 s_x, f32 s_z, f32 e_x, f32 e_z, f32 height,
							   SColor color, s32 texture_id, E_SIDE_TYPE type)
{
	SMeshBuffer * mesh = new SMeshBuffer();
	SMaterial material;

	// set up the material
	if(type == S_ROAD)
	{
		mesh->Material.Lighting = false;
		mesh->Material.FogEnable = true;
		mesh->Material.ZWriteEnable = false;
		mesh->Material.AmbientColor = color;
		mesh->Material.DiffuseColor = color;
		mesh->Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
		mesh->Material.setTexture(0,CityScene::getTexture(texture_id));
	}
	else
	{
		mesh->Material.Lighting = false;
		mesh->Material.FogEnable = true;
		mesh->Material.BackfaceCulling = false;
		mesh->Material.AmbientColor = color;
		mesh->Material.DiffuseColor = color;
		mesh->Material.MaterialType = video::EMT_DETAIL_MAP;
		mesh->Material.setTexture(0,CityScene::getTexture(texture_id));
	}
	matrix4 matrix;
	matrix[0] = 1; matrix[5] = -1; matrix[10] = 1; matrix[15] = 1;
	mesh->Material.setTextureMatrix(0,matrix);

	mesh->Vertices.set_used(4);
	mesh->Indices.set_used(6);

	mesh->Indices[0] = 0;
	mesh->Indices[1] = 1;
	mesh->Indices[2] = 2;
	mesh->Indices[3] = 2;
	mesh->Indices[4] = 3;
	mesh->Indices[5] = 0;

	if(type == S_TOP)
	{
		mesh->Vertices[0] = S3DVertex(s_x,height,s_z, 0,0,0, color, 0.f, 1.f);
		mesh->Vertices[1] = S3DVertex(s_x,height,e_z, 0,0,0, color, 0.f, 0.f);
		mesh->Vertices[2] = S3DVertex(e_x,height,e_z, 0,0,0, color, 1.0f, 0.f);
		mesh->Vertices[3] = S3DVertex(e_x,height,s_z, 0,0,0, color, 1.0f, 1.f);
	}
	else if(type == S_SIDE1)
	{
		mesh->Vertices[0] = S3DVertex(s_x,height,s_z, 0,0,0, color, 0.f, 1.f);
		mesh->Vertices[1] = S3DVertex(s_x,0,     s_z, 0,0,0, color, 0.f, 0.f);
		mesh->Vertices[2] = S3DVertex(e_x,0,     s_z, 0,0,0, color, 1.0f, 0.f);
		mesh->Vertices[3] = S3DVertex(e_x,height,s_z, 0,0,0, color, 1.0f, 1.f);
	}
	else if(type == S_SIDE2)
	{
		mesh->Vertices[0] = S3DVertex(e_x,height,e_z, 0,0,0, color, 0.f, 1.f);
		mesh->Vertices[1] = S3DVertex(e_x,0,     e_z, 0,0,0, color, 0.f, 0.f);
		mesh->Vertices[2] = S3DVertex(s_x,0,     e_z, 0,0,0, color, 1.0f, 0.f);
		mesh->Vertices[3] = S3DVertex(s_x,height,e_z, 0,0,0, color, 1.0f, 1.f);
	}
	else if(type == S_SIDE3)
	{
		mesh->Vertices[0] = S3DVertex(e_x,height,s_z, 0,0,0, color, 0.f, 1.f);
		mesh->Vertices[1] = S3DVertex(e_x,0,     s_z, 0,0,0, color, 0.f, 0.f);
		mesh->Vertices[2] = S3DVertex(e_x,0,     e_z, 0,0,0, color, 1.0f, 0.f);
		mesh->Vertices[3] = S3DVertex(e_x,height,e_z, 0,0,0, color, 1.0f, 1.f);
	}
	else if(type == S_SIDE4)
	{
		mesh->Vertices[0] = S3DVertex(s_x,height,s_z, 0,0,0, color, 0.f, 1.f);
		mesh->Vertices[1] = S3DVertex(s_x,0,     s_z, 0,0,0, color, 0.f, 0.f);
		mesh->Vertices[2] = S3DVertex(s_x,0,     e_z, 0,0,0, color, 1.0f, 0.f);
		mesh->Vertices[3] = S3DVertex(s_x,height,e_z, 0,0,0, color, 1.0f, 1.f);
	}
	else if(type == S_ROAD)
	{
		mesh->Vertices[0] = S3DVertex(s_x,height,s_z, 0,0,0, color, 0.f, 1.f);
		mesh->Vertices[1] = S3DVertex(s_x,height,e_z, 0,0,0, color, 0.f, 0.f);
		mesh->Vertices[2] = S3DVertex(e_x,height,e_z, 0,0,0, color, 1.0f, 0.f);
		mesh->Vertices[3] = S3DVertex(e_x,height,s_z, 0,0,0, color, 1.0f, 1.f);
	}

	return mesh;
}

// ============================================================================
// City Object
// ============================================================================
CityObject::CityObject(vector3df start, vector3df max_size, E_CITY_OBJECT_TYPE type_)
: type(type_)
{
	set_bounds(start, max_size);
}

void CityObject::set_bounds(vector3df start, vector3df max_size)
{
	bounds.MinEdge = start;
	bounds.MaxEdge = start + max_size;
}

void CityObject::render(IVideoDriver * driver, bool show_bounds)
{
	for(u32 i = 0; i < smesh.getMeshBufferCount(); i++)
	{
		IMeshBuffer * mesh = smesh.getMeshBuffer(i);
		driver->setMaterial(mesh->getMaterial());
		driver->drawMeshBuffer(mesh);
	}

	if(show_bounds)
	{
		SMaterial m;
		m.Lighting = false;
		m.FogEnable = false;
		driver->setMaterial(m);
		driver->draw3DBox(get_bounds(), SColor(255,255,255,255));
	}
}

// ============================================================================
// City Road
// ============================================================================
CityRoad::CityRoad(vector3df start, vector3df max_size, E_ROAD_TYPE type)
: CityObject(start, max_size)
{
	// height
	height = 0.f;

	// road texture
	s32 texture = (s32)type + 2;

	// set material
	SColor color(0,255,255,255);
	SMaterial material;
	material.Lighting = false;
	material.FogEnable = true;
	material.AmbientColor = color;
	material.DiffuseColor = color;
	material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
	material.ZWriteEnable = false;
	material.setTexture(0,CityScene::getTexture(texture));

	// bounds
	f32 s_x = get_bounds().MinEdge.X;
	f32 s_z = get_bounds().MinEdge.Z;
	f32 e_x = get_bounds().MaxEdge.X;
	f32 e_z = get_bounds().MaxEdge.Z;
	f32 height = 0.f;

	// create the textured mesh
	SMeshBuffer * sMeshBuffer = texturedMeshBuffer(s_x,s_z,e_x,e_z,height,color,texture,S_ROAD);
	sMeshBuffer->Material = material;
	smesh.addMeshBuffer(sMeshBuffer);

	// reset bounding box
	set_bounds(
		start,
		vector3df(max_size.X, height, max_size.Z));
}

CityRoad::~CityRoad()
{
}

// ============================================================================
// City Building
// ============================================================================
CityBuilding::CityBuilding(vector3df start, vector3df max_size)
: CityObject(start, max_size, BUILDING)
{
	// decide what height range the building will be in
	// 0 = tall
	// 1 = medium
	// 2 = short
	int type = 0;
	if(rnd(0,100) % 7)	type = 1;
	if(rnd(0,100) % 2)	type = 2;

	// randomly determine the height of the building
	if(type == 0)		height = f32(rnd(int(max_size.Y * .7f), int(max_size.Y)));
	else if(type == 1)	height = f32(rnd(int(max_size.Y * .3f), int(max_size.Y * .5f)));
	else if(type == 2)	height = f32(rnd(int(max_size.Y * .1f), int(max_size.Y * .3f)));

	// randomly make some building dimensions
	f32 s_x = start.X + rnd(int(max_size.X / 10),    int(max_size.X / 3));
	f32 e_x = start.X + rnd(2 * int(max_size.X) / 3, int(max_size.X));
	f32 s_z = start.Z + rnd(int(max_size.X / 10),    int(max_size.Z / 3));
	f32 e_z = start.Z + rnd(2 * int(max_size.Z / 3), int(max_size.Z));

	// color
	SColor color(0,255,255,255);

	// randomize some textures
	s32 roof_texture = rnd(ROOFS_START, ROOFS_START + NUM_ROOFS - 1);
	s32 building_texture = 0;
	if(type == 0)	building_texture = rnd(B_LARGE_START, B_LARGE_START + NUM_B_LARGE - 1);
	if(type == 1)	building_texture = rnd(B_MEDIUM_START, B_MEDIUM_START + NUM_B_MEDIUM - 1);
	if(type == 2)	building_texture = rnd(B_SMALL_START, B_SMALL_START + NUM_B_SMALL - 1);

	// make a basic box building
	smesh.addMeshBuffer(texturedMeshBuffer(s_x,s_z,e_x,e_z,height,color,roof_texture,S_TOP));
	smesh.addMeshBuffer(texturedMeshBuffer(s_x,s_z,e_x,e_z,height,color,building_texture,S_SIDE1));
	smesh.addMeshBuffer(texturedMeshBuffer(s_x,s_z,e_x,e_z,height,color,building_texture,S_SIDE2));
	smesh.addMeshBuffer(texturedMeshBuffer(s_x,s_z,e_x,e_z,height,color,building_texture,S_SIDE3));
	smesh.addMeshBuffer(texturedMeshBuffer(s_x,s_z,e_x,e_z,height,color,building_texture,S_SIDE4));

	// reset bounding box
	set_bounds(
		vector3df(s_x, 0.f, s_z),
		vector3df(e_x, height, e_z) - vector3df(s_x, 0.f, s_z));
}

// ============================================================================
// City Sector
// ============================================================================
CitySector::~CitySector()
{
	for(int i = 0; i < int(buildings.height()); i++)
	for(int j = 0; j < int(buildings.width()); j++)
	{
		if(buildings(i,j))
		{
			delete buildings(i,j);
		}
	}
}

void CitySector::generate_ground()
{
	// set material
	SColor color(255,255,255,255);
	SMaterial material;
	material.Lighting = false;
	material.FogEnable = true;
	material.AmbientColor = color;
	material.DiffuseColor = color;
	material.MaterialType = EMT_SOLID;
	material.ZWriteEnable = false;
	material.setTexture(0,CityScene::getTexture(1));
	ground.Material = material;

	// bounds
	f32 s_x = bounds.MinEdge.X;
	f32 s_z = bounds.MinEdge.Z;
	f32 e_x = bounds.MaxEdge.X;
	f32 e_z = bounds.MaxEdge.Z;
	f32 height = 0;

	// set up for 1 square
	ground.Vertices.set_used(4);
	ground.Indices.set_used(6);
	// create mesh
	ground.Vertices[0] = S3DVertex(s_x,height,s_z, 0,0,0, color, 0.f, 1.f);
	ground.Vertices[1] = S3DVertex(s_x,height,e_z, 0,0,0, color, 0.f, 0.f);
	ground.Vertices[2] = S3DVertex(e_x,height,e_z, 0,0,0, color, 1.0f, 0.f);
	ground.Vertices[3] = S3DVertex(e_x,height,s_z, 0,0,0, color, 1.0f, 1.f);
	ground.Indices[0] = 0;
	ground.Indices[1] = 1;
	ground.Indices[2] = 2;
	ground.Indices[3] = 2;
	ground.Indices[4] = 3;
	ground.Indices[5] = 0;
}

void CitySector::build_sector(dimension2di size, vector3df start,
	vector3df building_size, dimension2di pos, bool load, stringc irrFile,
	IrrlichtDevice * device)
{
	id = pos.Height * pos.Width + pos.Width;
	f32 density = 0.4f;

	// set bounding box
	bounds.MinEdge = start;
	bounds.MaxEdge = vector3df(
		f32(start.X + size.Width * building_size.X),
		start.Y + building_size.Y,
		f32(start.Z + size.Height * building_size.Z));

	// generate the ground
	generate_ground();

	// generate the buildings and/or load the scene from file
	if(load)
	{	
		loadScene(irrFile.c_str(), device, nodes, start);
		from_file = true;
	}
	// for the editor, we always want to load the buildings
	//else
	{
		// generate the buildings and roads in the sector
		buildings.reset(size.Width, size.Height);
		f32 height = 5.f;
		for(int i = 0; i < int(buildings.height()); i++)
		for(int j = 0; j < int(buildings.width()); j++)
		{
			vector3df start(
				start.X + building_size.X * j,
				0,
				start.Z + building_size.Z * i);
			s32 total_x = pos.Width * size.Width + i;
			s32 total_y = pos.Height * size.Height + j;

			if(total_x % 9 == 0 || total_y % 9 == 0)
			{
				E_ROAD_TYPE type = HORIZONTAL;
				if(!(total_x % 9 == 0))
					type = VERTICAL;
				else if(total_y % 9 == 0)
					type = CROSS;

				buildings(i,j) = new CityRoad(start, building_size, type);
			}
			else if((f32(rnd(0,10))/10.f) < density)
				buildings(i,j) = new CityBuilding(start, building_size);
			else
				buildings(i,j) = 0;

			if(buildings(i,j) && buildings(i,j)->get_height() > height)
				height = buildings(i,j)->get_height();
		}

		// reflect the height of the tallest building in the sector in the bounds
		bounds.MaxEdge.Y = start.Y + height;
	}
}

void CitySector::prerender(bool selected)
{
	irr::core::list<ISceneNode *>::Iterator it;
	for(it = nodes.begin(); it != nodes.end(); it++)
		(*it)->setVisible(true);
	this->selected = selected;
}

void CitySector::render(IVideoDriver * driver, bool sector_bounds,
	bool building_bounds)
{
	// render the ground
	driver->setMaterial(ground.getMaterial());
	driver->drawMeshBuffer(&ground);

	if(from_file)
	{
		// scene was loaded in
	}
	// Always render the roads
	//else
	{
		// render the buildings
		for(int i = 0; i < int(buildings.height()); i++)
		for(int j = 0; j < int(buildings.width()); j++)
		{
			if(buildings(i,j) && (!from_file || dynamic_cast<CityRoad *>(buildings(i,j))))
				buildings(i,j)->render(driver, building_bounds);
		}
	}

	// show bounding boxes
	if(sector_bounds)
	{
		SMaterial m;
		m.Lighting = false;
		m.FogEnable = false;
		driver->setMaterial(m);
		driver->draw3DBox(bounds, SColor(255,0,255,0));
	}
	if(selected)
	{
		float padding = 1.f;
		aabbox3df sBox;
		sBox.MinEdge = vector3df(bounds.MinEdge.X+padding, bounds.MinEdge.Y+padding, bounds.MinEdge.Z+padding);
		sBox.MaxEdge = vector3df(bounds.MaxEdge.X-padding, bounds.MaxEdge.Y-padding, bounds.MaxEdge.Z-padding);
		SMaterial m;
		m.Lighting = false;
		m.FogEnable = false;
		driver->setMaterial(m);
		driver->draw3DBox(sBox, SColor(255,0,0,255));
	}
}


void CitySector::postrender()
{
	irr::core::list<ISceneNode *>::Iterator it;
	for(it = nodes.begin(); it != nodes.end(); it++)
		(*it)->setVisible(false);
	selected = false;
}

const irr::core::aabbox3df &CitySector::get_bounds() const
{
	return bounds;
}

// delete all the objects from the sector
void CitySector::unload_scene(irr::core::dimension2di size, irr::core::vector3df start,
		irr::core::vector3df building_size, irr::core::dimension2di pos)
{
	irr::core::list<ISceneNode *>::Iterator itr;
	for(itr = nodes.begin(); itr != nodes.end(); itr++)
	{
		// TODO: find a way to actually get rid of the scene nodes
		(*itr)->setVisible(false);
	}
	nodes.clear();
	from_file = false;
}

void CitySector::load_scene(const irr::core::stringc irrFile,
	irr::core::vector3df start, irr::IrrlichtDevice * device)
{
	loadScene(irrFile.c_str(), device, nodes, start);
	from_file = true;
}

// ============================================================================
// City Scene
// ============================================================================
array<ITexture *> CityScene::textures;

CityScene::CityScene(IrrlichtDevice * device, dimension2di size_,
	dimension2di sector_size_, vector3df building_size_)
: device(device), driver(device->getVideoDriver()), size(size_),
smgr(device->getSceneManager()),
sector_size(sector_size_), building_size(building_size_),
show_city_bounds(false), show_sector_bounds(false),
show_building_bounds(false), visible_sectors(0),
sector_selection(false), sectors(0)
{
	ISceneManager * smgr = device->getSceneManager();
	IVideoDriver * driver = smgr->getVideoDriver();

	// load all the textures
    bool mmflag = driver->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
	for(int i = 0; i < NUM_TEXTURES; i++)
	{
		if(i >= ROADS_START && i < ROADS_START + NUM_ROADS)
			driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
		else
			driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
		textures.push_back(driver->getTexture(texture_files[i]));
	}
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, mmflag);

	// set bounds for entire city
	bounds.MinEdge = vector3df(0,0,0);
	bounds.MaxEdge = vector3df(
		f32(size.Width * sector_size.Width * building_size.X),
		building_size.Y,
		f32(size.Height * sector_size.Height * building_size.Z));

	// load or generate each sector
	sectors = new array2d<CitySector>(size.Width, size.Height);
	for(int i = 0; i < int(sectors->height()); i++)
	for(int j = 0; j < int(sectors->width()); j++)
	{
		// sector parameters
		dimension2di size(i, j);
		bool load = false;
		stringc irrFile;
		vector3df start(
			f32(j * sector_size.Width * building_size.X),
			0,
			f32(i * sector_size.Height * building_size.Z));

		// create the sector
		(*sectors)(j,i).build_sector(
			sector_size, start, building_size, size, load, irrFile, device);
	}
}

CityScene::~CityScene()
{
	if(sectors)
		delete sectors;
}

void CityScene::prerender()
{
	// relative position of the camera to the city
	vector3df rel_position = camera - get_position();

	// camera's current sector
	s32 cam_x = s32(rel_position.X / (sector_size.Width * building_size.X));
	s32 cam_z = s32(rel_position.Z / (sector_size.Height * building_size.Z));
	cam_sector.Width = cam_x;	// debug
	cam_sector.Height = cam_z;	// debug

	// render the sectors around the camera
	s32 s_x = cam_x - 4;	if(s_x < 0) s_x = 0;
	s32 e_x = cam_x + 4;	if(e_x >= size.Width) e_x = size.Width - 1;
	s32 s_z = cam_z - 4;	if(s_z < 0) s_z = 0;
	s32 e_z = cam_z + 4;	if(e_z >= size.Height) e_z = size.Height - 1;

	// post-render on the sectors rendered last frame
	for(render_it = render_sectors.begin(); render_it != render_sectors.end();
		render_it++)
	{
		(*render_it)->postrender();
	}

	// remove all the render sectors from last frame
	render_sectors.clear();

	// refill the render list
	visible_sectors = 0;
	for(s32 i = s_z; i <= e_z; i++)
	for(s32 j = s_x; j <= e_x; j++)
	{
		// skip the corners
		if(s_z > 0 || s_x > 0)
		if(i == s_z && j == s_x)	continue;
		if(e_z < size.Height-1 || e_x < size.Width-1)
		if(i == e_z && j == e_x)	continue;
		if(s_z > 0 || e_x < size.Width-1)
		if(i == s_z && j == e_x)	continue;
		if(e_z < size.Height-1 || s_x > 0)
		if(i == e_z && j == s_x)	continue;

		// set the current sector to visible
		if(visible((*sectors)(j,i).get_bounds()))
		{
			visible_sectors++;
			bool isSelected = (sector_selection && (j==selected_sector.X) && (i==selected_sector.Y));
			(*sectors)(j,i).prerender(isSelected);
			render_sectors.push_back(&(*sectors)(j,i));
		}
	}
}

void CityScene::render()
{
	if(!driver)	return;
	driver->setTransform(video::ETS_WORLD, irr::core::matrix4(matrix4::eConstructor::EM4CONST_IDENTITY));

	// render visible sectors
	for(render_it = render_sectors.begin(); render_it != render_sectors.end();
		render_it++)
	{
		(*render_it)->render(driver, show_sector_bounds,
			show_building_bounds);
	}

	// draw bounds
	if(show_city_bounds)
	{
		float padding = 1.f;
		aabbox3df sBox;
		sBox.MinEdge = vector3df(bounds.MinEdge.X-padding, bounds.MinEdge.Y-padding, bounds.MinEdge.Z-padding);
		sBox.MaxEdge = vector3df(bounds.MaxEdge.X+padding, bounds.MaxEdge.Y+padding, bounds.MaxEdge.Z+padding);

		SMaterial m;
		m.Lighting = false;
		m.FogEnable = false;
		driver->setMaterial(m);
		driver->draw3DBox(sBox, SColor(255,255,0,0));
	}
}

const aabbox3df& CityScene::getBoundingBox() const
{
	return bounds;
}

bool CityScene::set_selected_sector(const irr::core::position2di &cursor)
{
	// NOTE that this whole process can be done really fast in hardware.
	// HOWEVER, to do that I would have to make every sector a scene node, and
	// I'm simply just too lazy to.

	// get the "ray" from the camera to the mouse cursor
	line3df line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(cursor);

	// we're interested in the line crossing the XZ plane from above, so the end
	// height has to be less than 0
	if(line.end.Y <= 0)
	{
		vector3df pNorm(0,1,0);
		vector3df lOrigin = line.start;
		vector3df lDir = line.getVector().normalize();

		// get the nominator (return false if it's 0)
		// notice that we can skip the plane's distance from the origin because
		// we're using the XZ plane (it contains the origin, a distance of 0),
		// but we still do it for educational purposes
		float pDist = 0.f;
		float nom = -pDist - lOrigin.dotProduct(pNorm);
		if(nom == 0)
			return false;

		// get the denominator (return false it's 0)
		float denom = lDir.dotProduct(pNorm);
		if(denom == 0)
			return false;

		// get the distance from the camera to the XZ plane
		float dist = nom / denom;

		// get the position of the interection (vector from the origin)
		vector3df intersection = lOrigin + lDir * dist;

		// determine the sector the intersection point is in
		s32 sec_x = s32(intersection.X / (sector_size.Width * building_size.X));
		s32 sec_z = s32(intersection.Z / (sector_size.Height * building_size.Z));
		if(sec_x >= 0 && sec_x < size.Width && sec_z >= 0 && sec_z < size.Height)
		{
			sector_selection = true;
			selected_sector.X = sec_x;
			selected_sector.Y = sec_z;
			return true;
		}
	}
	// the line doesn't intersect with the XZ plane
	return false;
}

void CityScene::set_position(const irr::core::vector3df &pos)
{
	position = pos;
}

bool CityScene::visible(aabbox3df box) const
{
    // get absolute position of bounding box
    box.MinEdge = box.MinEdge + get_position();
    box.MaxEdge = box.MaxEdge + get_position();

    // to test sector bounding box against planes
    s32 leftRel, rightRel, topRel, bottomRel, nearRel, farRel;

    // get camera frustrum planes
    const scene::SViewFrustum * frustrum = smgr->getActiveCamera()->getViewFrustum();

	// test all the frustrum planes (return false if any fail)
	core::plane3d<f32> left = frustrum->planes[scene::SViewFrustum::VF_LEFT_PLANE];
    leftRel = box.classifyPlaneRelation(left);
	if(leftRel == core::ISREL3D_FRONT) {return false;}

	core::plane3d<f32> right = frustrum->planes[scene::SViewFrustum::VF_RIGHT_PLANE];
    rightRel = box.classifyPlaneRelation(right);
	if(rightRel == core::ISREL3D_FRONT) {return false;}

	core::plane3d<f32> top = frustrum->planes[scene::SViewFrustum::VF_TOP_PLANE];
	topRel = box.classifyPlaneRelation(top);
	if(topRel == core::ISREL3D_FRONT) {return false;}

	core::plane3d<f32> bottom = frustrum->planes[scene::SViewFrustum::VF_BOTTOM_PLANE];
    bottomRel = box.classifyPlaneRelation(bottom);
	if(bottomRel == core::ISREL3D_FRONT) {return false;}

	core::plane3d<f32> near1 = frustrum->planes[scene::SViewFrustum::VF_NEAR_PLANE];
    nearRel = box.classifyPlaneRelation(near1);
	if(nearRel == core::ISREL3D_FRONT) {return false;}

	core::plane3d<f32> far1 = frustrum->planes[scene::SViewFrustum::VF_FAR_PLANE];
	farRel = box.classifyPlaneRelation(far1);
	if(farRel == core::ISREL3D_FRONT) {return false;}

	return true;
}

void CityScene::show_bounds(bool city, bool sector, bool building)
{
	show_city_bounds = city;
	show_sector_bounds = sector;
	show_building_bounds = building;
}

ITexture * CityScene:: getTexture(s32 id)
{
	return textures[id];
}

void CityScene::remove_scene(irr::s32 x, irr::s32 y)
{
	if(x >= 0 && x < (s32)sectors->width() && y >= 0 && y < (s32)sectors->height())
	{
		dimension2di size(x, y);
		vector3df start(
			f32(x * sector_size.Width * building_size.X),
			0,
			f32(y * sector_size.Height * building_size.Z));

		(*sectors)(x,y).unload_scene(sector_size, start, building_size, size);
	}
}

void CityScene::load_scene(irr::s32 x, irr::s32 y, const irr::core::stringc &file,
	irr::IrrlichtDevice * device)
{
	if(x >= 0 && x < (s32)sectors->width() && y >= 0 && y < (s32)sectors->height())
	{
		vector3df start(
			f32(x * sector_size.Width * building_size.X),
			0,
			f32(y * sector_size.Height * building_size.Z));

		(*sectors)(x,y).load_scene(file, start, device);
	}
}

bool CityScene::is_scene_loaded(irr::s32 x, irr::s32 y)
{
	if(x >= 0 && x < (s32)sectors->width() && y >= 0 && y < (s32)sectors->height())
	{
		return (*sectors)(x,y).is_from_file();
	}
	return false;
}