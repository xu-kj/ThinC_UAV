#include "tactors_3x3.h"
#include "output.h"

#include <sstream>
#include <iostream>
#include <exception>
#include <map>

using namespace std;

namespace {
    bool IGNORE_TACTORS = false;
    std::string TACTOR_PORT1;
    std::string TACTOR_PORT2;
    std::string TACTOR_PORT3;
}

namespace Sim {
    TactorLocation::TactorLocation(int board, int tactor)
        : board(board)
        , tactor(tactor)
    {}

    Sector::Sector(TactorLocation t)
    {
        tactors.push_back(t);
    }

    Sector::Sector(TactorLocation t1, TactorLocation t2)
    {
        tactors.push_back(t1);
        tactors.push_back(t2);
    }

    Tactors3x3::Tactors3x3(
            const std::string& port1,
            const std::string& port2,
            const std::string& port3,
            bool enabled)
    {
        TACTOR_PORT1 = port1;
        TACTOR_PORT2 = port2;
        TACTOR_PORT3 = port3;
        IGNORE_TACTORS = enabled;

        for(int i = 0; i < 3; i++)
            connected[i] = false;

        sectors.push_back(Sector(TactorLocation(1, TAC2)));                             // sector 1
        sectors.push_back(Sector(TactorLocation(0, TAC5), TactorLocation(0, TAC6)));    // sector 2
        sectors.push_back(Sector(TactorLocation(1, TAC3)));                             // sector 3

        sectors.push_back(Sector(TactorLocation(1, TAC1)));                             // sector 4
        sectors.push_back(Sector(TactorLocation(1, TAC5), TactorLocation(1, TAC6)));    // sector 5
        sectors.push_back(Sector(TactorLocation(1, TAC4)));                             // sector 6

        sectors.push_back(Sector(TactorLocation(2, TAC1)));                             // sector 7
        sectors.push_back(Sector(TactorLocation(2, TAC5), TactorLocation(2, TAC6)));    // sector 8
        sectors.push_back(Sector(TactorLocation(2, TAC4)));                             // sector 9
    }

    Tactors3x3::~Tactors3x3()
    {
        stop();

        for(int i = 0; i < 3; i++)
        {
            std::stringstream ss;
            ss << "Shutting down tactor box " << i + 1;
            Output::Instance().WriteLine(ss.str().c_str());

            if(connected[i])
                boxes[i].Shutdown();
        }
    }

    void Tactors3x3::connect()
    {
        connectBox(0, TACTOR_PORT1);
        connectBox(1, TACTOR_PORT2);
        connectBox(2, TACTOR_PORT3);
    }

    void Tactors3x3::fireSector(int id, int duration)
    {
        if(IGNORE_TACTORS)
            return;

        std::list<TactorLocation>::iterator it = sectors[id].tactors.begin();
        for(; it != sectors[id].tactors.end(); it++)
        {
            int box = it->board;
            int tactor = it->tactor;
            boxes[box].TacOnTime(tactor, duration);
        }
    }

    void Tactors3x3::fireSectors(const std::vector<int>& ids, int duration)
    {
        if(IGNORE_TACTORS)
            return;

        std::map<int, int> boxToTacs;
        boxToTacs.insert(std::make_pair(0, 0));
        boxToTacs.insert(std::make_pair(1, 0));
        boxToTacs.insert(std::make_pair(2, 0));

        for(int i = 0; i < (int)ids.size(); i++)
        {
            std::list<TactorLocation>::iterator it = sectors[ids[i]].tactors.begin();
            for(; it != sectors[ids[i]].tactors.end(); it++)
            {
                int box = it->board;
                int tactor = it->tactor;
                boxToTacs[box] |= tactor;
                std::cout << "[sector " << i+1 << "] adding " << box << "/" << tactor << std::endl;
            }
        }

        if(boxToTacs[0] != 0) {boxes[0].TacOnTime(boxToTacs[0], duration); std::cout << "firing box 0" << std::endl;}
        if(boxToTacs[1] != 0) {boxes[1].TacOnTime(boxToTacs[1], duration); std::cout << "firing box 1" << std::endl;}
        if(boxToTacs[2] != 0) {boxes[2].TacOnTime(boxToTacs[2], duration); std::cout << "firing box 2" << std::endl;}
    }

    void Tactors3x3::test()
    {
        for(unsigned int i = 0; i < sectors.size(); i++)
            fireSector(i, 1000);
    }

    void Tactors3x3::setGain(int gain)
    {
        if(IGNORE_TACTORS)
            return;

        int debugGain = gain;
        int gainBase = gain / 64;
        int att = gain % 64;

        if(att > 0)
            gain = 64 * (gainBase + 1) - att - 1;
        else
            gain = 64 * gainBase + 63;

        //System::Diagnostics::Debug::WriteLine(
        //  "[all boxes/banks] = " + debugGain);

        for(int i = 0; i < 3; i++)
            boxes[i].SetGain(gain);
    }

    void Tactors3x3::setGain(int gain, int sector)
    {
        if(IGNORE_TACTORS)
            return;

        int debugGain = gain;
        int gainBase = gain / 64;
        int att = gain % 64;

        gain = 64 * gainBase + 63 - att;
        //if(att > 0)
        //  gain = 64 * (gainBase + 1) - att - 1;
        //else
        //  gain = 64 * gainBase + 63;

        //std::cout << debugGain << ": " << "base=" << gainBase << ", att=" << att << ", gain=" << gain << std::endl;

        // assume that all the tactors assigned to this sector are on
        // the same box
        Sector& s = sectors[sector];
        boxes[s.tactors.front().board].SetGain(gain);

        //std::cout << "[board=" << s.tactors.front().board << ", gain=" << gain << "]" << std::endl;
        //System::Diagnostics::Debug::WriteLine(
        //  "[board=" + s.tactors.front().board +
        //  ", bank=" + TactorLocation::getBank(s.tactors.front().tactor) +
        //  "] = " + debugGain);
    }

    void Tactors3x3::setGains(int gain, std::vector<int>& ids)
    {
        if(IGNORE_TACTORS)
            return;

        std::map<int, bool> boxToTacs;
        boxToTacs.insert(std::make_pair(0, false));
        boxToTacs.insert(std::make_pair(1, false));
        boxToTacs.insert(std::make_pair(2, false));

        for(int i = 0; i < (int)ids.size(); i++)
        {
            std::list<TactorLocation>::iterator it = sectors[ids[i]].tactors.begin();
            for(; it != sectors[ids[i]].tactors.end(); it++)
            {
                int box = it->board;
                boxToTacs[box] = true;
            }
        }

        int debugGain = gain;
        int gainBase = gain / 64;
        int att = gain % 64;

        if(att > 0)
            gain = 64 * (gainBase + 1) - att - 1;
        else
            gain = 64 * gainBase + 63;

        if(boxToTacs[0]) {boxes[0].SetGain(gain);}// std::cout << "[box 0] gain=" << gain << std::endl;}
        if(boxToTacs[1]) {boxes[1].SetGain(gain);}// std::cout << "[box 1] gain=" << gain << std::endl;}
        if(boxToTacs[2]) {boxes[2].SetGain(gain);}// std::cout << "[box 2] gain=" << gain << std::endl;}
    }

    void Tactors3x3::setFreqs(int freq1, int freq2)
    {
        //Sim::Output::Instance().Write("FREQ SET TO ");
        //Sim::Output::Instance().Write(freq1);
        //Sim::Output::Instance().WriteLine("");

        if(IGNORE_TACTORS)
            return;

        for(int i = 0; i < 3; i++)
        {
            boxes[i].SetSinFreq1(freq1);
            boxes[i].SetSinFreq2(freq2);
        }
    }

    void Tactors3x3::stop()
    {
        if(IGNORE_TACTORS)
            return;

        for(int i = 0; i < 3; i++)
            boxes[i].TurnAllOff();
    }

    void Tactors3x3::connectBox(int id, const std::string& port)
    {
        int result = boxes[id].OpenBTPort(port.c_str());
        if(!IGNORE_TACTORS && result == 8)
        {
            stringstream ss;
            ss << "Error: Could not connect to tactor box " << id + 1 << " on port \"" << port << "\".";
            Output::Instance().WriteLine(ss.str().c_str());
            throw exception(ss.str().c_str());
        }
        else
        {
            stringstream ss;
            ss << "Connected to tactor box " << id + 1 << " on port \"" << port << "\"";
            Output::Instance().WriteLine(ss.str().c_str());
            std::cout << ss.str() << std::endl;

            connected[id] = true;
        }
    }
}