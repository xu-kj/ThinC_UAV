#include "network.h"
#include "config.h"

#include <string>
#include <iostream>
#include <windows.h>

#pragma warning(disable : 4706 4996)
#define NETWORK_ERROR 2

using namespace std;

Network Network::n;

// Function prototype!
// returns 0 on failure (note that we won't actually be connected until
// updateNetwork pumps a few times).
RakPeerInterface * startNetwork(unsigned short port);

// Actual Network implementation!
Network::Network()
{
    established = false;
}

Network::~Network()
{
    if(established)
        RakNetworkFactory::DestroyRakPeerInterface(n.peer);
}

void Network::establish()
{
    bool isServer = true;
    string serverIP;
    int serverPort = 5000;
    int thisPort = 5000;

    bool error = false;

    error &= LoadConfigBoolean(isServer, "Network", "RUN_AS_SERVER", NET_CONFIG_PATH);
    error &= LoadConfigInteger(thisPort, "Network", "THIS_PORT", NET_CONFIG_PATH);
    if(!isServer)
    {
        error &= LoadConfigString(serverIP, "Network", "SERVER_IP", NET_CONFIG_PATH);
        error &= LoadConfigInteger(serverPort, "Network", "SERVER_PORT", NET_CONFIG_PATH);
    }
    if(error)
    {
        MessageBoxA(0, "Cannot load values from config file", "Error", 0);
        exit(NETWORK_ERROR);
    }

    n.established = true;

    if(isServer)
    {
        n.peer = startNetwork((unsigned short)thisPort);
        if(n.peer == 0)
            exit(NETWORK_ERROR);
    }
    else
    {
        n.peer = startNetwork((unsigned short)thisPort);
        if(n.peer == 0)
            exit(NETWORK_ERROR);

        if(!n.awaitConnection(serverIP, (unsigned short)serverPort))
            exit(NETWORK_ERROR);
    }
}

void Network::subscribeToNetworkUpdates(const NetworkDelegate &newDelegate)
{
    n.simUpdateDelegate = newDelegate;
}

bool Network::sendMessageStart()
{
    if(!n.established)
        return false;

    SimStartPacket pkt;
    pkt.id = (unsigned char)SIM_START;

    if(!n.peer->Send((char *)&pkt, sizeof(SimStartPacket), LOW_PRIORITY, RELIABLE_SEQUENCED, 0, UNASSIGNED_SYSTEM_ADDRESS, true))
        return false;

    return true;
}

bool Network::sendMessageUpdate(int type, int objectID, float x, float y, int parent)
{
    //MessageBoxA(0, "sending update", "", 0);
    if(!n.established)
        return false;

    SimUpdatePacket pkt;
    pkt.id = (unsigned char)SIM_UPDATE;
    pkt.type = type;
    pkt.objectID = objectID;
    pkt.x = x;
    pkt.y = y;
    pkt.parent = parent;

    if(!n.peer->Send((char *)&pkt, sizeof(SimUpdatePacket), LOW_PRIORITY, RELIABLE_SEQUENCED, 0, UNASSIGNED_SYSTEM_ADDRESS, true))
        return false;

    return true;
}

bool Network::sendMessageFailure(int type, int uav, float changeTime, float changeTo, float duration)
{
    if(!n.established)
        return false;

    FailurePacket pkt;
    pkt.id = (unsigned char)SIM_FAILURE;
    pkt.uav = uav;
    pkt.changeTime = changeTime;
    pkt.changeTo = changeTo;
    pkt.duration = duration;

    if(!n.peer->Send((char *)&pkt, sizeof(FailurePacket), LOW_PRIORITY, RELIABLE_SEQUENCED, 0, UNASSIGNED_SYSTEM_ADDRESS, true))
        return false;

    return true;
}

bool Network::sendMessageChat(const std::string &message)
{
    if(!n.established)
        return false;

    ChatPacket pkt;
    pkt.id = (unsigned char)SIM_CHAT;
    strcpy(pkt.message, message.c_str());

    if(!n.peer->Send((char *)&pkt, sizeof(ChatPacket), LOW_PRIORITY, RELIABLE_SEQUENCED, 0, UNASSIGNED_SYSTEM_ADDRESS, true))
        return false;
    return true;
}

bool Network::sendMessageCreate(int type, int id, float x, float y, float z, float r, float g, float b, float a, int parent)
{
    if(!n.established)
        return false;

    SimCreatePacket pkt;
    pkt.id = (unsigned char)SIM_CREATE;
    pkt.type = type;
    pkt.objectID = id;
    pkt.x = x;
    pkt.y = y;
    pkt.z = z;
    pkt.r = r;
    pkt.g = g;
    pkt.b = b;
    pkt.a = a;
    pkt.parent = parent;

    if(!n.peer->Send((char *)&pkt, sizeof(SimCreatePacket), LOW_PRIORITY, RELIABLE_SEQUENCED, 0, UNASSIGNED_SYSTEM_ADDRESS, true))
        return false;

    return true;
}

bool Network::sendMessageCamButton(int type, int id)
{
    if(!n.established)
        return false;

    SimCamButtonPacket pkt;
    pkt.id = SIM_CAM_BUTTON;
    pkt.objectID = id;
    pkt.type = type;

    //cout << "creating packet with type " << pkt.type << " and objectID " << pkt.objectID << endl;

    if(!n.peer->Send((char *)&pkt, sizeof(SimCamButtonPacket), LOW_PRIORITY, RELIABLE_SEQUENCED, 0, UNASSIGNED_SYSTEM_ADDRESS, true))
        return false;

    return true;
}

void Network::shutdown()
{
    if(n.established)
        n.peer->Shutdown(0);
}

RakPeerInterface * startNetwork(unsigned short port)
{
    int networkSleep;
    if(!LoadConfigInteger(networkSleep, "Network", "NETWORK_SLEEP", NET_CONFIG_PATH))
    {
        MessageBoxA(0, "Cannot load values from config file", "Error", 0);
        exit(NETWORK_ERROR);
    }

    RakPeerInterface * peer = RakNetworkFactory::GetRakPeerInterface();

    SocketDescriptor sock(port, 0);
    if(!peer->Startup(6, networkSleep, &sock, 1))
    {
        printf("ERROR: could not open network connection.\n");
        return 0;
    }

    peer->SetMaximumIncomingConnections(3);
    peer->SetOccasionalPing(true);

    printf("starting network peer as %s:%d\n", peer->GetLocalIP(0), port);

    return peer;
}

bool Network::awaitConnection(const string &serverIP, unsigned short serverPort)
{
    printf("attempting to connect to %s:%d\n", serverIP.c_str(), serverPort);
    printf("awaiting connection...\n");

    bool first = true;
    while(n.peer->NumberOfConnections() < 1)
    {
        if(first || update() & CONNECTION_ATTEMPT_FAILED)
        {
            if(!first)
                printf("retrying connection...\n");
            first = false;

            if(!n.peer->Connect(serverIP.c_str(), serverPort, 0, 0))
            {
                printf("ERROR: could not connect\n");
                return false;
            }
        }
    }
    return true;
}

int Network::update()
{
    if(!n.established)
        return NETWORK_NOT_ESTABLISHED;

    int status = 0;

    Packet * p;
    while(p = n.peer->Receive())
    {
        // Somebody sent us an application-level packet
        if(p->data[0] >= ID_USER_PACKET_ENUM)
        {
            //printf("Received a user defined packet!\n");
            n.simUpdateDelegate(p);
            n.peer->DeallocatePacket(p);
            continue;
        }

        switch(p->data[0])
        {
            case ID_NEW_INCOMING_CONNECTION:
            // Some host is trying to connect with us
            {
                status |= NEW_INCOMING_CONNECTION;
                printf("%s connected\n", p->systemAddress.ToString());
                n.connected = true;
                break;
            }

            case ID_CONNECTION_REQUEST_ACCEPTED:
            // Some host accepted our connection request
            {
                status |= CONNECTION_REQUEST_ACCEPTED;
                printf("%s accepted connection\n", p->systemAddress.ToString());
                n.connected = true;
                break;
            }

            case ID_CONNECTION_ATTEMPT_FAILED:
            // We tried to connect to some host, but they rejected us or the request
            // timed out
            {   
                status |= CONNECTION_ATTEMPT_FAILED;
                printf("%s refused connection\n", p->systemAddress.ToString());
                break;
            }

            case ID_NO_FREE_INCOMING_CONNECTIONS:
            // Some host tried to connect, but we don't have any free sockets
            {
                status |= NO_FREE_INCOMING_CONNECTIONS;
                printf("%s connection request rejected: too many connections\n", p->systemAddress.ToString());
                break;
            }

            case ID_DISCONNECTION_NOTIFICATION:
            // Some other host disconnected from us
            {
                status |= DISCONNECTION_NOTIFICATION;
                printf("%s disconnected\n", p->systemAddress.ToString());
                break;
            }

            case ID_CONNECTION_LOST:
            // Lost connection to some other host
            {
                status |= CONNECTION_LOST;
                printf("%s lost connection\n", p->systemAddress.ToString());
                break;
            }
        }
        n.peer->DeallocatePacket(p);
    }

    return status;
}