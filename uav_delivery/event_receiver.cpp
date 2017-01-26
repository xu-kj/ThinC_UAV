#include "event_receiver.h"
#include "window.h"

EventReceiver::EventReceiver(UAVWindow * callback_)
    : callback(callback_) {
    for (irr::u32 i=0; i<irr::KEY_KEY_CODES_COUNT; ++i)
        KeyIsDown[i] = false;
}

bool EventReceiver::OnEvent(const irr::SEvent& event) {
    if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
        bool already_down = IsKeyDown(event.KeyInput.Key);
        KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

        if(!already_down)
            callback->event_key_down(event.KeyInput.Char);
    }
    else if(event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
        switch(event.MouseInput.Event) {
        case irr::EMIE_LMOUSE_LEFT_UP:
            callback->event_mouse_up();
            break;
        case irr::EMIE_LMOUSE_PRESSED_DOWN:
            callback->event_mouse_down();
            break;
        case irr::EMIE_MOUSE_MOVED:
            callback->event_mouse_move();
            break;
        case irr::EMIE_MOUSE_WHEEL:
            if(event.MouseInput.Wheel > 0)
                callback->event_mouse_wheel_up();
            else
                callback->event_mouse_wheel_down();
            break;
        }
    }
    return false;
}

bool EventReceiver::IsKeyDown(irr::EKEY_CODE keyCode) const {
    return KeyIsDown[keyCode];
}