#ifndef IMAGE_H
#define IMAGE_H

#include <irrlicht.h>

/* 3D-accelerated image class. Requires images to
   be in the power of 2 to insure correct display */
class GUIImage : public irr::gui::IGUIElement
{
public:
    GUIImage(irr::core::rect<irr::s32> position,
        irr::IrrlichtDevice * device,
        irr::gui::IGUIElement* parent);
    
    void setColor(irr::video::SColor color);
    void setPosition(irr::core::rect<irr::s32> position);
    void setPosition(irr::core::position2d<irr::s32> position);
    void setTexture(irr::video::ITexture * texture);
    void setBilinearFilter(bool value);
    void setRotation(irr::f32 deg);
    void setScale(irr::f32 x, irr::f32 y);
    void setScaleImageOnly(bool value);

    virtual void draw();

    static void draw(irr::video::IVideoDriver * driver,
        irr::video::ITexture * texture,
        irr::core::dimension2di dim,
        irr::core::rect<irr::s32> source,
        irr::core::rect<irr::s32> dest);
    static void draw2DRectangle(irr::video::IVideoDriver * driver,
        irr::core::rect<irr::s32> dest, irr::video::SColor color);

private:
    void clipImage();
    irr::core::rect<irr::s32> getViewPort() const;

    irr::video::IVideoDriver * driver;
    irr::scene::SMeshBuffer mesh;
    irr::core::matrix4 matrix;
    irr::f32 scaleX;
    irr::f32 scaleY;
    bool scale_image_only;
};

#endif /* IMAGE_H */