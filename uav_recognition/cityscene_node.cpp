#include "cityscene_node.h"
#include "globals.h"
#include "random.h"
#include "target_struct.h"

#include <irrlicht.h>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <conio.h>

using namespace std;

#pragma warning (disable:4996)

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

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

const stringc PEOPLE_CONFIG = "../media/config/city_people.cfg";
const stringc VEHICLE_CONFIG = "../media/config/city_vehicles.cfg";
std::vector<stringc> PEOPLE_MESHES;
std::vector<stringc> VEHICLE_MESHES;

// ============================================================================
// Utility Functions
// ============================================================================
void loadConfigs()
{
    { // load the people
        ifstream in(PEOPLE_CONFIG.c_str());
        if(in)
        {
            cout << endl;
            cout << "Loading mesh paths from \"" << PEOPLE_CONFIG.c_str() << "\"" << endl;
            do
            {
                std::string line;
                std::getline(in, line);

                if(line.size() == 0)
                    break;

                stringc str = line.c_str();
                PEOPLE_MESHES.push_back(str);
                cout << "\"" << line << "\"" << endl;
            } while(in);
        }
        else
        {
            cout << "Failed to open \"" << PEOPLE_CONFIG.c_str() << "\"" << endl;
            stringw str = "Missing file - cannot build city\n";
            str += "\"";
            str += PEOPLE_CONFIG;
            str += "\"";
            throw(str);
        }
        in.close();
    }

    { // load the vehicles
        ifstream in(VEHICLE_CONFIG.c_str());
        if(in)
        {
            cout << endl;
            cout << "Loading mesh paths from \"" << VEHICLE_CONFIG.c_str() << "\"" << endl;
            do
            {
                std::string line;
                std::getline(in, line);

                if(line.size() == 0)
                    break;

                stringc str = line.c_str();
                VEHICLE_MESHES.push_back(str);
                cout << "\"" << line << "\"" << endl;
            } while(in);
        }
        else
        {
            cout << "Failed to open \"" << VEHICLE_CONFIG.c_str() << "\"" << endl;
            stringw str = "Missing file - cannot build city\n";
            str += "\"";
            str += VEHICLE_CONFIG;
            str += "\"";
            throw(str);
        }
    }
    cout << endl << endl;
}

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


ISceneNode * readSceneNode(IXMLReader * reader, IrrlichtDevice * device,
    vector3df start)
{
    ISceneManager * smgr = device->getSceneManager();
    ISceneNode * node = 0;

    if (!reader)
        return 0;

    // read node name and type
    if(stringw(L"node") == reader->getNodeName() &&
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
                    if     (stringw(L"Name") == name)   node_name = value;
                    else if(stringw(L"Mesh") == name)   mesh = value;

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
                            if(j < 2)   j++;
                            break;
                        case ' ':
                            break;
                        default:
                            tmp[j] += value[i];
                        }
                    }
                    if(j < 2)   j++;
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

// ============================================================================
// City Object
// ============================================================================
CityObject::CityObject(vector3df start, vector3df max_size, E_CITY_OBJECT_TYPE type_)
: type(type_)
{
    set_bounds(start, max_size);
}

CityObject::~CityObject()
{
    if(smesh.getMeshBufferCount() > 0)
    {
        for(u32 i = 0; i < smesh.getMeshBufferCount(); i++)
            delete smesh.getMeshBuffer(i);
        smesh.MeshBuffers.clear();
    }
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

    smesh.addMeshBuffer(texturedMeshBuffer(s_x,s_z,e_x,e_z,height,color,texture,S_ROAD));

    // reset bounding box
    set_bounds(
        start,
        vector3df(max_size.X, height, max_size.Z));
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
    if(rnd(0,100) % 7)  type = 1;
    if(rnd(0,100) % 2)  type = 2;

    // randomly determine the height of the building
    f32 height;
    if(type == 0)       height = f32(rnd(int(max_size.Y * .4f), int(max_size.Y * .6)));
    else if(type == 1)  height = f32(rnd(int(max_size.Y * .3f), int(max_size.Y * .4f)));
    else if(type == 2)  height = f32(rnd(int(max_size.Y * .2f), int(max_size.Y * .3f)));

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
    if(type == 0)   building_texture = rnd(B_LARGE_START, B_LARGE_START + NUM_B_LARGE - 1);
    if(type == 1)   building_texture = rnd(B_MEDIUM_START, B_MEDIUM_START + NUM_B_MEDIUM - 1);
    if(type == 2)   building_texture = rnd(B_SMALL_START, B_SMALL_START + NUM_B_SMALL - 1);

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
            delete buildings(i,j);
    }


    irr::core::list<irr::scene::ISceneNode *>::Iterator it;
    for(it = nodes.begin(); it != nodes.end(); it++)
    {
        (*it)->remove();
        //delete (*it);
        //(*it)->drop();
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
    coords = pos;

    id = pos.Height * pos.Width + pos.Width;
    f32 density = .4f;

    // set bounding box
    bounds.MinEdge = start;
    bounds.MaxEdge = vector3df(
        f32(start.X + size.Width * building_size.X),
        start.Y + building_size.Y,
        f32(start.Z + size.Height * building_size.Z));

    // generate the ground
    generate_ground();

    // load the scene if it was specified
    if(load)
    {
        // load the target scenes from file
        loadScene(irrFile.c_str(), device, nodes, start);
        from_file = true;
    }

    // generate the buildings and/or roads in the sector
    buildings.reset(size.Width, size.Height);
    for(int i = 0; i < int(buildings.height()); i++)
    for(int j = 0; j < int(buildings.width()); j++)
    {
        buildings(i, j) = 0;

        vector3df start(
            start.X + building_size.X * j,
            0,
            start.Z + building_size.Z * i);

        if((j == 0 && pos.Width % 3 == 0) || (i == 0 && pos.Height % 3 == 0))
        {
            vector3df rotation(0.f,0.f,0.f);
            vector3df position(start);
            E_ROAD_TYPE type = CROSS;
            if(!(i == 0 && pos.Height % 3 == 0))
            {
                type = VERTICAL;
                position.X += 19.f;
                if(rnd(0,1) == 0)
                {
                    position.X -= 8.f;
                    rotation.Y += 180.f;
                }
            }
            else if(!(j == 0 && pos.Width % 3 == 0))
            {
                type = HORIZONTAL;
                rotation.Y = 90.f;
                position.Z += 19.f;
                if(rnd(0,1) == 0)
                {
                    position.Z -= 8.f;
                    rotation.Y += 180.f;
                }
            }
            buildings(i,j) = new CityRoad(start, building_size, type);


            if((f32(rnd(0,10))/2.f) < density && type != CROSS)
            {
                // PUT IN TEH CARS
                // get a random mesh
                int rmesh = rnd(0, VEHICLE_MESHES.size() - 1);

                // get the mesh and create the scene node
                ISceneNode * node = 0;
                IAnimatedMesh * imesh = device->getSceneManager()->getMesh(VEHICLE_MESHES[rmesh].c_str());
                if(imesh && (node = device->getSceneManager()->addMeshSceneNode(imesh)))
                {
                    // set position, etc
                    node->setPosition(position);
                    node->setScale(vector3df(0.05f,0.05f,0.05f));
                    node->setRotation(rotation);
                    node->setVisible(false);
                    for(u32 i = 0; i < node->getMaterialCount(); i++)
                    {
                        node->getMaterial(i).FogEnable = true;
                        node->getMaterial(i).Lighting = false;
                    }
                }

                // add the node to the node list
                if(node)
                    nodes.push_back(node);
            }

        }
        else if(!load)
        {
            if((f32(rnd(0,10))/10.f) < density)
                buildings(i,j) = new CityBuilding(start, building_size);
            else if((f32(rnd(0,10))/5.f) < density)
            {
                // PUT IN TEH PEOPLES
                // get a random mesh
                int rmesh = rnd(0, PEOPLE_MESHES.size() - 1);
                vector3df rotation(0.f,(f32)rnd(0,359),0.f);
                vector3df position(f32(rnd(0,300)/10), 0.f, f32(rnd(0,300)/10));

                // get the mesh and create the scene node
                ISceneNode * node = 0;
                IAnimatedMesh * imesh = device->getSceneManager()->getMesh(PEOPLE_MESHES[rmesh].c_str());
                if(imesh && (node = device->getSceneManager()->addMeshSceneNode(imesh)))
                {
                    // set position, etc
                    node->setPosition(start + position);
                    node->setScale(vector3df(0.1f,0.1f,0.1f));
                    node->setRotation(rotation);
                    node->setVisible(false);
                    for(u32 i = 0; i < node->getMaterialCount(); i++)
                    {
                        node->getMaterial(i).FogEnable = true;
                        node->getMaterial(i).Lighting = false;
                    }
                }

                // add the node to the node list
                if(node)
                    nodes.push_back(node);
            }
        }
    }
}

void CitySector::prerender()
{
    irr::core::list<ISceneNode *>::Iterator it;
    for(it = nodes.begin(); it != nodes.end(); it++)
        (*it)->setVisible(true);
}

void CitySector::render(IVideoDriver * driver, bool sector_bounds,
    bool building_bounds)
{
    // render the ground
    driver->setMaterial(ground.getMaterial());
    driver->drawMeshBuffer(&ground);

    // render the buildings
    for(int i = 0; i < int(buildings.height()); i++)
    for(int j = 0; j < int(buildings.width()); j++)
    {
        if(buildings(i,j))
            buildings(i,j)->render(driver, building_bounds);
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
}


void CitySector::postrender()
{
    irr::core::list<ISceneNode *>::Iterator it;
    for(it = nodes.begin(); it != nodes.end(); it++)
        (*it)->setVisible(false);
}

const irr::core::aabbox3df &CitySector::get_bounds() const
{
    return bounds;
}

// ============================================================================
// City Scene
// ============================================================================
array<ITexture *> CityScene::textures;

CityScene::CityScene(IrrlichtDevice * device, dimension2di size_,
    dimension2di sector_size_, vector3df building_size_)
    : ISceneNode(
        device->getSceneManager()->getRootSceneNode(),
        device->getSceneManager(), 40),
    device(device),
    size(size_), sector_size(sector_size_), building_size(building_size_),
    show_city_bounds(false), show_sector_bounds(false),
    show_building_bounds(false), visible_sectors(0)
{
    ISceneManager * smgr = device->getSceneManager();
    IVideoDriver * driver = smgr->getVideoDriver();

    // load all the config stuff
    loadConfigs();

    // set the cache size
    cache_size_max = 1000;
    cache_size_min = 500;

    // load all the textures
    textures.clear();
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

    // retrieve the TARGETS array
    std::list<TargetStruct> * targets = (std::list<TargetStruct> *)TARGETS;
    if(targets != 0)
    {
        cout << "Adding scenes" << endl;
        // load the TARGETS array content into the scene map
        std::list<TargetStruct>::iterator it;
        for(it = targets->begin(); it != targets->end(); it++)
        {
            cout << "Adding scene file \"" << it->file.c_str() << "\" at [" << (int)it->x << "," << (int)it->y << "]" << endl;
            stringc irrFile = it->file;
            dimension2di coords(it->x, it->y);
            vector3df start(
                f32(it->x * sector_size.Width * building_size.X),
                0,
                f32(it->y * sector_size.Height * building_size.Z));

            CitySector * s = new CitySector(device);
            scenes.insert(std::make_pair(std::make_pair(it->x, it->y), s));

            s->build_sector(
                sector_size, start, building_size, coords, true, irrFile, device);
        }
        cout << "Done" << endl;
    }
}
CityScene::~CityScene()
{
    // empty the scene map
    std::map<std::pair<irr::s32, irr::s32>, CitySector *>::iterator s_it;
    for(s_it = scenes.begin(); s_it != scenes.end(); s_it++)
        delete s_it->second;

    // empty the cache
    std::list<CitySector *>::iterator c_it;
    for(c_it = cache.begin(); c_it != cache.end(); c_it++)
        delete (*c_it);
}

void CityScene::prerender()
{
    // relative position of the camera to the city
    vector3df rel_position = camera - getPosition();

    // camera's current sector
    s32 cam_x = s32(rel_position.X / (sector_size.Width * building_size.X));
    s32 cam_z = s32(rel_position.Z / (sector_size.Height * building_size.Z));
    cam_sector.Width = cam_x;   // debug
    cam_sector.Height = cam_z;  // debug

    // render the sectors around the camera
    s32 s_x = cam_x - 3;    if(s_x < 0) s_x = 0;
    s32 e_x = cam_x + 3;    if(e_x > size.Width) e_x = size.Width - 1;
    s32 s_z = cam_z - 3;    if(s_z < 0) s_z = 0;
    s32 e_z = cam_z + 3;    if(e_z > size.Height) e_z = size.Height - 1;

    // post-render on the sectors rendered last frame
    for(render_it = render_sectors.begin(); render_it != render_sectors.end();
        render_it++)
    {
        (*render_it)->postrender();
    }

    //cout << "clear cache" << endl;

    // kick extra things out of the cache
    if(cache.size() > (unsigned int)cache_size_max)
    {
        std::list<CitySector *>::iterator it1;
        std::list<CitySector *>::iterator it2;
    
        int count = 0;
        it1 = cache.begin();
        while(count < cache_size_min && it1 != cache.end()) {
            count++;
            it1++;
        }
        while(it1 != cache.end()) {
            count++;
            it2 = it1;
            it1++;

            // delete from cache and cache_map
            //std::map<std::pair<irr::s32, irr::s32>, std::list<CitySector *>::iterator>::iterator meh;
            //meh = cache_map.find(std::make_pair((*it2)->get_coords().Width, (*it2)->get_coords().Height));
            //if(meh != cache_map.end())
            //  cache_map.erase(meh);
            delete (*it2);
            cache.erase(it2);
        }
    }

    //cout << "fill render list" << endl;

    // remove all the render sectors from last frame
    render_sectors.clear();

    // refill the render list
    visible_sectors = 0;
    CitySector * s = 0;
    for(s32 i = s_z; i < e_z; i++)
    for(s32 j = s_x; j < e_x; j++)
    {
        // add the sector to the cache if it doesn't exist
        // look for the sector in the scenes map
        std::map<std::pair<irr::s32, irr::s32>, CitySector *>::iterator s_it;
        if((s_it = scenes.find(std::make_pair(j, i))) == scenes.end())
        {
            // the sector was not in the scene map
            // look for the sector in the cache
            bool found = false;
            //std::map<std::pair<irr::s32, irr::s32>, std::list<CitySector *>::iterator>::iterator meh;
            //found = (meh = cache_map.find(std::make_pair(j, i))) != cache_map.end();

            std::list<CitySector *>::iterator c_it;
            for(c_it = cache.begin(); c_it != cache.end(); c_it++)
            {
                if((*c_it)->get_coords().Width == j && (*c_it)->get_coords().Height == i)
                {
                    found = true;
                    break;
                }
            }

            // create or move the sector in the cache list
            if(found)
            {
                // the sector was found, so move it to the front of the list
                //std::list<CitySector *>::iterator c_it = meh->second;
                s = *c_it;
                if(s)
                {
                    cache.erase(c_it);
                    cache.push_front(s);
                }
            }
            else
            {
                // create the sector
                dimension2di coords(j,i);
                vector3df start(
                    f32(j * sector_size.Width * building_size.X),
                    0,
                    f32(i * sector_size.Height * building_size.Z));

                s = new CitySector(device);
                s->build_sector(
                    sector_size, start, building_size, coords, false, "", device);
                cache.push_front(s);
                cache_map.insert(std::make_pair(std::make_pair(j, i), cache.begin()));
            }
        }
        else
        {
            // the sector was in the scene map
            s = (*s_it).second;
        }

        // add the sector to the scene list if it's visible
        if(s && visible(s->get_bounds()))
        {
            visible_sectors++;
            s->prerender();
            render_sectors.push_back(s);
        }
    }
    
    //cout << "reader to render" << endl;
}

void CityScene::render()
{
    // get the video driver
    IVideoDriver * driver = SceneManager->getVideoDriver();
    if(!driver) return;
    driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

    // render visible sectors
    for(render_it = render_sectors.begin(); render_it != render_sectors.end();
        render_it++)
    {
        if(*render_it)
            (*render_it)->render(
                driver,
                show_sector_bounds,
                show_building_bounds);
    }

    // draw bounds
    if(show_city_bounds)
    {
        SMaterial m;
        m.Lighting = false;
        m.FogEnable = false;
        driver->setMaterial(m);
        driver->draw3DBox(bounds, SColor(255,255,0,0));
    }
}

const aabbox3df& CityScene::getBoundingBox() const
{
    return bounds;
}

void CityScene::set_position(const irr::core::vector3df &pos)
{
    ISceneNode::setPosition(pos);
}

bool CityScene::visible(aabbox3df box) const
{
    // get absolute position of bounding box
    box.MinEdge = box.MinEdge + getPosition();
    box.MaxEdge = box.MaxEdge + getPosition();

    // to test sector bounding box against planes
    s32 leftRel, rightRel, topRel, bottomRel, nearRel, farRel;

    // get camera frustrum planes
    const scene::SViewFrustum * frustrum = SceneManager->getActiveCamera()->getViewFrustum();
    
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

    core::plane3d<f32> near = frustrum->planes[scene::SViewFrustum::VF_NEAR_PLANE];
    nearRel = box.classifyPlaneRelation(near);
    if(nearRel == core::ISREL3D_FRONT) {return false;}

    core::plane3d<f32> far = frustrum->planes[scene::SViewFrustum::VF_FAR_PLANE];
    farRel = box.classifyPlaneRelation(far);
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