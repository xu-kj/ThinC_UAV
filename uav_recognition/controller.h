#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "event.h"
#include "sim_object.h"
#include "uav.h"
#include "ufo.h"
#include "waypoint.h"
#include "window.h"

#include <list>
#include <vector>
#include <fstream>

struct Packet;

class UAVController {
public:
    UAVController();
    void run();
    void get_final_stats();

    void start_network();
    void network_update(Packet * pkt);

    void load_config();
    void set_world_params();
    void load_world();

    void readEventNode(irr::io::IXMLReader * reader);
    void readUFONode(irr::io::IXMLReader * reader);
    SimObject * readBaseNode(irr::io::IXMLReader * reader);
    WaypointObject * readWaypointNode(irr::io::IXMLReader * reader);
    void readUAVNode(irr::io::IXMLReader * reader);
    void readSceneNode(irr::io::IXMLReader * reader);
    void readMapNode(irr::io::IXMLReader * reader);

    void generate_random();

    void broadcastEvent(Event * e);

private:
    UAVWindow * win1;
    UAVWindow * win2;

    std::vector<UAVObject *> uav_ids;

    std::list<WaypointObject *> wps;
    std::list<WaypointObject *>::iterator wp_it;

    std::list<SimObject *> bases;
    std::list<SimObject *>::iterator base_it;

    std::list<UAVObject *> uavs;
    std::list<UAVObject *>::iterator uav_it;

    std::list<UFOObject *> ufos;
    std::list<UFOObject *>::iterator ufo_it;

    std::list<Event *> events;
    std::list<Event *>::iterator e_it;

    irr::core::vector3df uav_start_pos;
    bool uav_start_pos_found;

	bool audio;
	bool visual;
};

#endif /* CONTROLLER_H */