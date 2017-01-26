#ifndef NETWORK_H
#define NETWORK_H

#include "RakNet/RakNetworkFactory.h"
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/GetTime.h"
#include "RakNet/BitStream.h"
#include "FastDelegate.h"
#include <string>

// The data structure that Raknet moves around
#pragma pack(push, 1)
struct SimUpdatePacket
{
    unsigned char id;   // type of packet = SIM_UPDATE
    int type;           // what type of object: 1 - uav
                        //                      2 - wp
    int objectID;       // id of object (aka uav ONE, target EIGHTEEN)
    float x;            // x position
    float y;            // y position
    int parent;         // id of parent uav (if applicable). -1 for no change
};
#pragma pack(push, 1)
#pragma pack(pop)
struct FailurePacket
{
    unsigned char id;   // type of packet = SIM_FAILURE
    int type;           // type of failure: 1 - camera
                        //                  2 - gps
                        //                  3 - fuel
                        //                  4 - lag in schedule
                        //                  5 - risk
                        //                  6 - coverage
                        //                  7 - scenario time
                        //                  10 - safety graph value
                        //                  11 - schedule graph value
                        //                  12 - coverage graph value
                        //                  13 - health graph value
                        //                  14 - clicked on graph on OLD sim
    int uav;            // which UAV this applies to
    float changeTime;   // how long it takes for the overview to animate
    float changeTo;     // what to change the value to (boolean: if < 0, false, > 0 true)
    float duration;     // how long this applies for
};
#pragma pack(push, 1)
#pragma pack(pop)
struct ChatPacket
{
    unsigned char id;   // type of packet = SIM_CHAT
    char message[100];  // message
};
#pragma pack(push, 1)
#pragma pack(pop)
struct SimCreatePacket
{
    unsigned char id;   // type of packet = SIM_CREATE
    int type;           // 1 - uav
                        // 2 - wp
                        // 3 - base
    int objectID;       //
    float x;            //
    float y;            //
    float z;            //
    float r;            // red
    float g;            // green
    float b;            // blue
    float a;            // alpha
    int parent;         // uav for wps
};
#pragma pack(push, 1)
#pragma pack(pop)
struct SimStartPacket
{
    unsigned char id;   // this is SIM_START
};
#pragma pack(push, 1)
#pragma pack(pop)
struct SimEndPacket
{
    unsigned char id;   // this is SIM_END
};
#pragma pack(push, 1)
#pragma pack(pop)
struct SimCamPacket
{
    unsigned char id;   // type of packet = SIM_CAM
    int objectID;
    float x;
    float y;
    float z;
    float targetX;
    float targetY;
    float targetZ;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct SimCamButtonPacket
{
    unsigned char id;   // type of packet = SIM_CAM_BUTTON
    int type;           // 0 - turn on buttons
                        // 1 - turn off buttons
                        // 2 - user clicked "found"
                        // 3 - user clicked "clear"
                        // 4 - user clicked "unsure"
                        // 5 - user turned on light
                        // 6 - user turned off light
                        // 7 - set brightness to dim
                        // 8 - set brightness to medium
                        // 9 - set brightness to full
                        // 10 - lose video feel (show blue screen)
                        // 11 - user clicked bottom "clear"
                        // 12 - cue completed
    int objectID;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct SimSaraCamPacket
{
    unsigned char id;   // type of packet = SIM_SARA_CAM
    int type;           // visual mudsplash, etc
    int correctResponse;
    float value1;
    float value2;
    float value3;
    float value4;
    float value5;
    float value6;
    float value7;
    float value8;
    float value9;
    int objectID;
    char sectors[9];
};
#pragma pack(pop)

// These are the packet types you can send
enum
{
    SIM_UPDATE = ID_USER_PACKET_ENUM,
    SIM_FAILURE,
    SIM_CHAT,
    SIM_CREATE,
    SIM_START,
    SIM_END,
    SIM_CAM,
    SIM_CAM_BUTTON,
    SIM_SARA_CAM
};

// These are the basic connection statuses returned from Network::update()
// Use example: if(Network::update() & NEW_INCOMING_CONNECTION)
enum NET_STATUS_E
{
    NETWORK_NOT_ESTABLISHED = -1,
    NEW_INCOMING_CONNECTION = 1,
    CONNECTION_REQUEST_ACCEPTED = 2,
    CONNECTION_ATTEMPT_FAILED = 4,
    NO_FREE_INCOMING_CONNECTIONS = 8,
    DISCONNECTION_NOTIFICATION = 16,
    CONNECTION_LOST = 32
};

typedef fastdelegate::FastDelegate1<Packet *> NetworkDelegate;

class Network
{
public:

    // Sends a message to the other machine
    //static bool sendMessage(int id, int data, int moreData = 0);
    static bool sendMessageStart();
    static bool sendMessageUpdate(int type, int objectID, float x, float y, int parent = -1);
    static bool sendMessageFailure(int type, int uav, float changeTime, float changeTo, float duration);
    static bool sendMessageChat(const std::string &message);
    static bool sendMessageCreate(int type, int id, float x, float y, float z, float r, float g, float b, float a, int parent);
    static bool sendMessageCamButton(int type, int id);

    // Checks for new messages on the network interface.
    // returns a value indicating some status (can be ignored, mostly).
    static int update();
    static void establish();
    static void subscribeToNetworkUpdates(const NetworkDelegate &newDelegate);
    static void shutdown();

    static bool getConnected() { return n.connected; }

private:
    
    // Chills out until a response is received from whoever we're trying to connect to.
    // returns true if connected, false if failed.
    bool awaitConnection(const std::string &serverIP, unsigned short serverPort);
    void handleSimUpdate(Packet * p);

    Network();
    ~Network();
    Network(const Network &);
    Network & operator=(const Network &);
    
    static Network n;

    NetworkDelegate simUpdateDelegate;  // you can subscribe to receive updates to your application
    RakPeerInterface * peer;            // network peer
    bool established;                   // established yet?
    bool connected;                     // connected to any peers yet?
};

#endif /* NETWORK_H */