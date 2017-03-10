#include "button.h"

#include <iostream>

using namespace std;

extern irr::s32 BUTTON_MS;
// const irr::s32 TITLEBAR_OFFSET_X = 2;
// const irr::s32 TITLEBAR_OFFSET_Y = 28;

UAVButton::UAVButton(rect2di area_, GUIImage * image_, GUIImage * highlight_, GUIImage * down_)
    : area(area_), image(image_), highlight(highlight_), down(down_), 
    click_timer(0), clicked(false),
    TITLEBAR_OFFSET_X(0), TITLEBAR_OFFSET_Y(0),
	highlighted(false)
{}

bool UAVButton::is_mouse_over(irr::core::position2di cursor) {
    if(cursor.X > area.UpperLeftCorner.X - TITLEBAR_OFFSET_X &&
        cursor.X < area.LowerRightCorner.X - TITLEBAR_OFFSET_X &&
        cursor.Y > area.UpperLeftCorner.Y - TITLEBAR_OFFSET_Y &&
        cursor.Y < area.LowerRightCorner.Y - TITLEBAR_OFFSET_Y) {
        mouse_over = true;
    }
    else {
        mouse_over = false;
    }
    return mouse_over;
}

void UAVButton::draw()
{
    //if(down && clicked) {
    //    down->setPosition(area.UpperLeftCorner);
    //    down->draw();
    //}
    if (highlighted) {
        down->setPosition(area.UpperLeftCorner);
        down->draw();
    }
    else {
        image->setPosition(area.UpperLeftCorner);
        image->draw();
    }
}

void UAVButton::draw(irr::core::position2di position)
{
    image->setPosition(position);
    image->draw();
}

void UAVButton::set_textures(GUIImage * image_, GUIImage * highlight_, GUIImage * down_)
{
    image = image_;
    highlight = highlight_;
    down = down_;
}

void UAVButton::click(irr::IrrlichtDevice * device)
{
    click_timer = device->getTimer()->getTime();
    clicked = true;
}

bool UAVButton::is_over() const
{
    return mouse_over;
}

bool UAVButton::is_in_click(irr::IrrlichtDevice * device)
{
    if(clicked) {
        irr::u32 now = device->getTimer()->getTime();
        if(now - click_timer >= (irr::u32)BUTTON_MS) {
            clicked = false;
            return true;
        }
    }
    return false;
}

void UAVButton::set_highlighted(bool highlighted_)
{
    highlighted = highlighted_;
}