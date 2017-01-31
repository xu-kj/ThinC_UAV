#ifndef EVENT_RECEIVER_H
#define EVENT_RECEIVER_H

#include <irrlicht.h>

class UAVWindow;

/* Basic event receiver class.
   Is able to detect simple keyboard and mouse events */
class EventReceiver : public irr::IEventReceiver
{
public:
    EventReceiver(UAVWindow * callback_);

    virtual bool OnEvent(const irr::SEvent& event);
    virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const;

private:
    bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
    UAVWindow * callback;
};

#endif /* EVENT_RECEIVER_H */