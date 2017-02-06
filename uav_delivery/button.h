#ifndef UAV_BUTTON_H
#define UAV_BUTTON_H

#include "image.h"

#include <irrlicht.h>
typedef irr::core::rect<irr::s32> rect2di;

class UAVButton {
public:
    UAVButton(rect2di area_, GUIImage * image_, GUIImage * highlight_ = 0, GUIImage * down_ = 0);

    bool is_mouse_over(irr::core::position2di cursor);
    void draw();
    void draw(irr::core::position2di position);

    void set_textures(GUIImage * image, GUIImage * highlight = 0, GUIImage * down = 0);
    void click(irr::IrrlichtDevice * device);

    bool is_over() const;
    bool is_in_click(irr::IrrlichtDevice * device);

    rect2di get_area() const {return area;}

    void set_titlebar_offset(int _x, int _y) {
        TITLEBAR_OFFSET_X = _x;
        TITLEBAR_OFFSET_Y = _y;
    };

	void set_highlighted(bool highlighted_);

private:
    rect2di area;

    GUIImage * image;
    GUIImage * highlight;
    GUIImage * down;

    irr::u32 click_timer;
    bool mouse_over;
    bool clicked;
    bool highlighted;

    irr::s32 TITLEBAR_OFFSET_X;
    irr::s32 TITLEBAR_OFFSET_Y;
};

#endif /* UAV_BUTTON_H */