#include "stdafx.h"
#include "Image.h"

#include <iostream>
using namespace std;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

GUIImage::GUIImage(rect<s32> position, IrrlichtDevice * device, IGUIElement* parent)
	: IGUIElement(EGUIET_ELEMENT, device->getGUIEnvironment(), parent, -1, position),
	scaleX(1.),
	scaleY(1.),
	scale_image_only(false)
{
	driver = device->getVideoDriver();

	mesh.Vertices.set_used(4);
	mesh.Indices.set_used(6);

	SColor white(255,255,255,255);
	mesh.Vertices[0] = S3DVertex(-1.f, -1.f, 0.f, 0.f, 0.f, 1.f, white, 0.f, 1.f);
	mesh.Vertices[1] = S3DVertex(-1.f,  1.f, 0.f, 0.f, 0.f, 1.f, white, 0.f, 0.f);
	mesh.Vertices[2] = S3DVertex( 1.f,  1.f, 0.f, 0.f, 0.f, 1.f, white, 1.0f, 0.f);
	mesh.Vertices[3] = S3DVertex( 1.f, -1.f, 0.f, 0.f, 0.f, 1.f, white, 1.0f, 1.f);

	mesh.Indices[0] = 0;
	mesh.Indices[1] = 1;
	mesh.Indices[2] = 2;
	mesh.Indices[3] = 2;
	mesh.Indices[4] = 3;
	mesh.Indices[5] = 0;

	mesh.getMaterial().Lighting = false;
	mesh.getMaterial().MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

	setBilinearFilter(false);

	//clipImage();
}


void GUIImage::setColor(SColor color)
{
	mesh.Vertices[0].Color = color;
	mesh.Vertices[1].Color = color;
	mesh.Vertices[2].Color = color;
	mesh.Vertices[3].Color = color;
}

void GUIImage::setPosition(rect<s32> position)
{
	IGUIElement::setRelativePosition(position);
	//clipImage();
}

void GUIImage::setPosition(position2d<s32> position)
{
	setRelativePosition(rect<s32>(
		position.X,
		position.Y,
		position.X + getRelativePosition().LowerRightCorner.X - getRelativePosition().UpperLeftCorner.X,
		position.Y + getRelativePosition().LowerRightCorner.Y - getRelativePosition().UpperLeftCorner.Y));
	//clipImage();
}

void GUIImage::setTexture(ITexture * texture)
{
	mesh.getMaterial().setTexture(0,texture);
}

void GUIImage::setBilinearFilter(bool value)
{
	mesh.Material.setFlag(EMF_BILINEAR_FILTER, value);
}

void GUIImage::setRotation(f32 deg)
{
	matrix.makeIdentity();
	matrix.setRotationDegrees(vector3df(0, 0, deg));
	//clipImage();
}

void GUIImage::setScale(f32 x, f32 y)
{
	scaleX = x;
	scaleY = y;
	//clipImage();
}

void GUIImage::setScaleImageOnly(bool value)
{
	scale_image_only = value;
	//clipImage();
}

void GUIImage::draw()
{
	if(!(driver && IsVisible))
		return;

	// store the old view port and set the new one
	rect<s32> oldViewPort = driver->getViewPort();
	rect<s32> newViewPort = getViewPort();
	driver->setViewPort(newViewPort);

	clipImage();

	// clear the projection matrix
	matrix4 oldProjMat = driver->getTransform(ETS_PROJECTION);
	driver->setTransform(ETS_PROJECTION, matrix4());

	// clear the view and draw matrix
	matrix4 oldViewMat = driver->getTransform(ETS_VIEW);
	driver->setTransform(ETS_VIEW, matrix4());
	driver->setTransform(ETS_WORLD, matrix);

	driver->setMaterial(mesh.Material);
	driver->drawMeshBuffer(&mesh);

	// restore view matrix
	driver->setTransform(ETS_VIEW, oldViewMat);

	// restore projection matrix
	driver->setTransform(ETS_PROJECTION, oldProjMat);

	// restore the view area
	driver->setViewPort(oldViewPort);
}

void GUIImage::clipImage()
{
	f32 height = f32(getAbsolutePosition().LowerRightCorner.Y - getAbsolutePosition().UpperLeftCorner.Y);
	f32 width = f32(getAbsolutePosition().LowerRightCorner.X - getAbsolutePosition().UpperLeftCorner.X);
	rect<s32> viewPort = getViewPort();

	f32 tu_1 = 0.f;
	f32 tu_2 = 1.f;
	f32 tv_1 = 0.f;
	f32 tv_2 = 1.f;

	dimension2d<s32> screen = driver->getScreenSize();
	// bottom
	if(viewPort.LowerRightCorner.Y > (s32)screen.Height)
		tv_2 = 1 + f32(screen.Height - viewPort.LowerRightCorner.Y) / f32(height);
	// top
	if(viewPort.UpperLeftCorner.Y < 0.f)
		tv_1 = -(f32(viewPort.UpperLeftCorner.Y) / f32(height));
	// left
	if(viewPort.UpperLeftCorner.X < 0.f)
		tu_1 = -(f32(viewPort.UpperLeftCorner.X) / f32(width));
	// right
	if(viewPort.LowerRightCorner.X > (s32)screen.Width)
		tu_2 = 1 + f32(screen.Width - viewPort.LowerRightCorner.X) / f32(width);

	// set the texture vertices
	mesh.Vertices[0].TCoords = vector2d<f32>(tu_1, tv_2);
	mesh.Vertices[1].TCoords = vector2d<f32>(tu_1, tv_1);
	mesh.Vertices[2].TCoords = vector2d<f32>(tu_2, tv_1);
	mesh.Vertices[3].TCoords = vector2d<f32>(tu_2, tv_2);
}

rect<s32> GUIImage::getViewPort() const
{
	rect<s32> newViewPort;
	if(!scale_image_only)
	{
		// scales the image AND the current position
		newViewPort = rect<s32>(
			s32(scaleX * getAbsolutePosition().UpperLeftCorner.X),
			s32(scaleY * getAbsolutePosition().UpperLeftCorner.Y),
			s32(scaleX * getAbsolutePosition().LowerRightCorner.X),
			s32(scaleY * getAbsolutePosition().LowerRightCorner.Y));
	}
	else
	{
		// allows scaling of the image itself (position independent)
		newViewPort = rect<s32>(
			s32(getAbsolutePosition().UpperLeftCorner.X),
			s32(getAbsolutePosition().UpperLeftCorner.Y),
			s32(getAbsolutePosition().UpperLeftCorner.X + scaleX * (getRelativePosition().LowerRightCorner.X - getRelativePosition().UpperLeftCorner.X)),
			s32(getAbsolutePosition().UpperLeftCorner.Y + scaleY * (getRelativePosition().LowerRightCorner.Y - getRelativePosition().UpperLeftCorner.Y)));
	}
	return newViewPort;
}

void GUIImage::draw(irr::video::IVideoDriver * driver,
		irr::video::ITexture * texture,
		irr::core::dimension2di dim,
		irr::core::rect<irr::s32> source,
		irr::core::rect<irr::s32> dest)
{
	SMeshBuffer mesh;
	matrix4 matrix;

	mesh.Vertices.set_used(4);
	mesh.Indices.set_used(6);

	f32 tu_1 = f32(source.UpperLeftCorner.X) / f32(dim.Width);
	f32 tu_2 = 1.f - f32(dim.Width - source.LowerRightCorner.X) / f32(dim.Width);
	f32 tv_1 = f32(source.UpperLeftCorner.Y) / f32(dim.Height);
	f32 tv_2 = 1.f - f32(dim.Height - source.LowerRightCorner.Y) / f32(dim.Height);

	SColor white(255,255,255,255);
	mesh.Vertices[0] = S3DVertex(-1.f, -1.f, 0.f, 0.f, 0.f, 1.f, white, tu_1, tv_2);
	mesh.Vertices[1] = S3DVertex(-1.f,  1.f, 0.f, 0.f, 0.f, 1.f, white, tu_1, tv_1);
	mesh.Vertices[2] = S3DVertex( 1.f,  1.f, 0.f, 0.f, 0.f, 1.f, white, tu_2, tv_1);
	mesh.Vertices[3] = S3DVertex( 1.f, -1.f, 0.f, 0.f, 0.f, 1.f, white, tu_2, tv_2);

	mesh.Indices[0] = 0;
	mesh.Indices[1] = 1;
	mesh.Indices[2] = 2;
	mesh.Indices[3] = 2;
	mesh.Indices[4] = 3;
	mesh.Indices[5] = 0;

	mesh.getMaterial().Lighting = false;
	mesh.getMaterial().MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
	mesh.getMaterial().setTexture(0,texture);

	rect<s32> oldViewPort = driver->getViewPort();
	rect<s32> newViewPort = dest;
	driver->setViewPort(newViewPort);
	matrix4 oldProjMat = driver->getTransform(ETS_PROJECTION);
	driver->setTransform(ETS_PROJECTION, matrix4());
	matrix4 oldViewMat = driver->getTransform(ETS_VIEW);
	driver->setTransform(ETS_VIEW, matrix4());
	driver->setTransform(ETS_WORLD, matrix);
	driver->setMaterial(mesh.Material);
	driver->drawMeshBuffer(&mesh);
	driver->setTransform(ETS_VIEW, oldViewMat);
	driver->setTransform(ETS_PROJECTION, oldProjMat);
	driver->setViewPort(oldViewPort);
}

void GUIImage::draw2DRectangle(irr::video::IVideoDriver * driver,
	irr::core::rect<irr::s32> dest, irr::video::SColor color)
{
	SMeshBuffer mesh;
	matrix4 matrix;

	mesh.Vertices.set_used(4);
	mesh.Indices.set_used(6);
	mesh.Vertices[0] = S3DVertex(-1.f, -1.f, 0.f, 0.f, 0.f, 1.f, color, 0.f, 1.f);
	mesh.Vertices[1] = S3DVertex(-1.f,  1.f, 0.f, 0.f, 0.f, 1.f, color, 0.f, 0.f);
	mesh.Vertices[2] = S3DVertex( 1.f,  1.f, 0.f, 0.f, 0.f, 1.f, color, 1.0f, 0.f);
	mesh.Vertices[3] = S3DVertex( 1.f, -1.f, 0.f, 0.f, 0.f, 1.f, color, 1.0f, 1.f);
	mesh.Indices[0] = 0;
	mesh.Indices[1] = 1;
	mesh.Indices[2] = 2;
	mesh.Indices[3] = 2;
	mesh.Indices[4] = 3;
	mesh.Indices[5] = 0;

	mesh.getMaterial().Lighting = false;
	mesh.getMaterial().MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

	rect<s32> oldViewPort = driver->getViewPort();
	rect<s32> newViewPort = dest;
	driver->setViewPort(newViewPort);
	matrix4 oldProjMat = driver->getTransform(ETS_PROJECTION);
	driver->setTransform(ETS_PROJECTION, matrix4());
	matrix4 oldViewMat = driver->getTransform(ETS_VIEW);
	driver->setTransform(ETS_VIEW, matrix4());
	driver->setTransform(ETS_WORLD, matrix);
	driver->setMaterial(mesh.Material);
	driver->drawMeshBuffer(&mesh);
	driver->setTransform(ETS_VIEW, oldViewMat);
	driver->setTransform(ETS_PROJECTION, oldProjMat);
	driver->setViewPort(oldViewPort);
}