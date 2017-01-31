#include "uav_tactors.h"

#include <string>
#include <sstream>
#include <iostream>

using std::string;
using std::stringstream;
using std::cout; 
using std::endl;

int  TACTOR_COM_PORT_1  = 5;
int  TACTOR_COM_PORT_2  = 6;
int  TACTOR_COM_PORT_3  = 7;
int  TACTOR_COM_PORT_4  = -1;
int  TACTOR_COM_PORT_5  = -1;
int  TACTOR_COM_PORT_6  = -1;
int  TACTOR_GAIN        = 3;
int  TACTOR_FREQUENCY   = 250;

int  HIGH_TACTOR_FREQ_1 = 250;
int  HIGH_TACTOR_FREQ_2 = 245;
int  HIGH_TACTOR_SOURCE = 1;
int  HIGH_TACTOR_LENGTH = 200;
bool HIGH_TACTOR_PULSE  = false;
int  HIGH_TACTOR_PULSE_INTERVAL = 0;

int  LOW_TACTOR_FREQ_1  = 250;
int  LOW_TACTOR_FREQ_2  = 245;
int  LOW_TACTOR_SOURCE  = 3;
int  LOW_TACTOR_LENGTH  = 200;
bool LOW_TACTOR_PULSE   = false;
int  LOW_TACTOR_PULSE_INTERVAL = 0;

//UAVTactors tactors;
Sim::Tactors3x3 * tactors;
std::vector<TactorType> UAVTactors::config;
std::vector<BoardType> UAVTactors::tactors;
const bool debug = false;

bool UAVTactors::Initialize(bool runTest)
{
    cout << "Initializing tactor system" << endl;
    LoadConfig();

    // connect to the COM ports from the config file
    tactors.resize(2);
    stringstream strm;
    
    // connect to the first tactor unit
    strm << "COM" << TACTOR_COM_PORT_1;
    cout << "TACTORS: Connecting on " << strm.str() << "...";
    if(tactors[0].tactors.OpenBTPort(const_cast<char *>(strm.str().c_str())) == 8)
    {
        cout << "failed" << endl;
        cout << "TACTORS: No tactor system found" << endl;
        return false;
    }
    else
        cout << "successful" << endl;

    // connect to the second tactor unit
    strm.str("");
    strm << "COM" << TACTOR_COM_PORT_2;
    cout << "TACTORS: Connecting on " << strm.str() << "...";
    if(tactors[1].tactors.OpenBTPort(const_cast<char *>(strm.str().c_str())) == 8)
    {
        cout << "failed" << endl;
        cout << "TACTORS: No tactor system found" << endl;
        return false;
    }
    else
        cout << "successful" << endl;

    // check to see if the frequencies are the same for high and low
    if(LOW_TACTOR_FREQ_1 == HIGH_TACTOR_FREQ_1) sameFreq1 = true;
    else                                        sameFreq1 = false;
    if(LOW_TACTOR_FREQ_2 == HIGH_TACTOR_FREQ_2) sameFreq2 = true;
    else                                        sameFreq2 = false;

    // set initial tactor settings
    tactors[0].tactors.SetSinFreq1(LOW_TACTOR_FREQ_1, 0);
    tactors[0].tactors.SetSinFreq2(LOW_TACTOR_FREQ_2, 0);
    tactors[0].tactors.SetGain(TACTOR_GAIN, 0);
    // and on the second board
    tactors[1].tactors.SetSinFreq1(LOW_TACTOR_FREQ_1, 0);
    tactors[1].tactors.SetSinFreq2(LOW_TACTOR_FREQ_2, 0);
    tactors[1].tactors.SetGain(TACTOR_GAIN, 0);
    // set the bank frequencies
    tactors[0].bankFreq[0] = LOW_TACTOR_SOURCE;
    tactors[0].bankFreq[1] = LOW_TACTOR_SOURCE;
    tactors[1].bankFreq[0] = LOW_TACTOR_SOURCE;
    tactors[1].bankFreq[1] = LOW_TACTOR_SOURCE;

    Sleep(10);

    tactors[0].SetSigSrc();
    tactors[1].SetSigSrc();

    bool fullTest = true;
    if(runTest && !fullTest)
    {
        // Run a really short test (center tactor)
        cout << "TACTORS: Beginning tactor test" << endl;
        MessageBox(0, L"The tactor test is about to begin. The two center tactors should\nbuzz twice, once with a low signal and once with a high signal.\nThis test should take about 6 seconds.\nPress OK to start.", L"Tactor t3st :O", 0);

        cout << "TACTORS: Sending a LOW signal to position 5...";
        SignalLow(5);
        Sleep(3000);
        cout << "TACTORS: done" << endl;
        cout << "TACTORS: Sending a HIGH signal to position 5...";
        SignalHigh(5);
        Sleep(3000);
        cout << "TACTORS: done" << endl;
        
        MessageBox(0, L"There should have been a response from both the center tactors.\nIf there wasn't, there's a problem with the tactor set-up; if\nthat's the case, you should quit immediately and try to fix it.", L"Tactor t3st :O", 0);
        cout << "TACTORS: End of tactor test" << endl;
    }
    else if(runTest)
    {
        // Run a really long test (all the tactors)
        cout << "TACTORS: Beginning tactor test" << endl;
        MessageBox(0, L"The tactor test is about to begin. The two center tactors should\nbuzz twice, once with a low signal and once with a high signal.\nThis test should take about 6 seconds.\nPress OK to start.", L"Tactor t3st :O", 0);
        
        cout << "TACTORS: Sending a LOW signal to position 1..." << endl;
        SignalLow(1);
        Sleep(3000);
        cout << "TACTORS: done" << endl;
        cout << "TACTORS: Sending a HIGH signal to position 1..." << endl;
        SignalHigh(1);
        Sleep(3000);
        cout << "TACTORS: done" << endl;

        cout << "TACTORS: Sending a LOW signal to position 2..." << endl;
        SignalLow(2);
        Sleep(3000);
        cout << "TACTORS: done" << endl;
        cout << "TACTORS: Sending a HIGH signal to position 2..." << endl;
        SignalHigh(2);
        Sleep(3000);
        cout << "TACTORS: done" << endl;

        cout << "TACTORS: Sending a LOW signal to position 3..." << endl;
        SignalLow(3);
        Sleep(3000);
        cout << "TACTORS: done" << endl;
        cout << "TACTORS: Sending a HIGH signal to position 3..." << endl;
        SignalHigh(3);
        Sleep(3000);
        cout << "TACTORS: done" << endl;

        cout << "TACTORS: Sending a LOW signal to position 4..." << endl;
        SignalLow(4);
        Sleep(3000);
        cout << "TACTORS: done" << endl;
        cout << "TACTORS: Sending a HIGH signal to position 4..." << endl;
        SignalHigh(4);
        Sleep(3000);
        cout << "TACTORS: done" << endl;

        cout << "TACTORS: Sending a LOW signal to position 5..." << endl;
        SignalLow(5);
        Sleep(3000);
        cout << "TACTORS: done" << endl;
        cout << "TACTORS: Sending a HIGH signal to position 5..." << endl;
        SignalHigh(5);
        Sleep(3000);
        cout << "TACTORS: done" << endl;

        cout << "TACTORS: Sending a LOW signal to position 6..." << endl;
        SignalLow(6);
        Sleep(3000);
        cout << "TACTORS: done" << endl;
        cout << "TACTORS: Sending a HIGH signal to position 6..." << endl;
        SignalHigh(6);
        Sleep(3000);
        cout << "TACTORS: done" << endl;

        cout << "TACTORS: Sending a LOW signal to position 7..." << endl;
        SignalLow(7);
        Sleep(3000);
        cout << "TACTORS: done" << endl;
        cout << "TACTORS: Sending a HIGH signal to position 7..." << endl;
        SignalHigh(7);
        Sleep(3000);
        cout << "TACTORS: done" << endl;

        cout << "TACTORS: Sending a LOW signal to position 8..." << endl;
        SignalLow(8);
        Sleep(3000);
        cout << "TACTORS: done" << endl;
        cout << "TACTORS: Sending a HIGH signal to position 8..." << endl;
        SignalHigh(8);
        Sleep(3000);
        cout << "TACTORS: done" << endl;

        cout << "TACTORS: Sending a LOW signal to position 9..." << endl;
        SignalLow(9);
        Sleep(3000);
        cout << "TACTORS: done" << endl;
        cout << "TACTORS: Sending a HIGH signal to position 9..." << endl;
        SignalHigh(9);
        Sleep(3000);
        cout << "TACTORS: done" << endl;

        MessageBox(0, L"There should have been a response from all the tactors.\nIf there wasn't, there's a problem with the tactor set-up; if\nthat's the case, you should quit immediately and try to fix it.", L"Tactor t3st :O", 0);
        cout << "End of tactor test" << endl;
    }

    cout << endl;
    return true;
}

void UAVTactors::SignalLow(int uav)
{
    int board = config[uav-1].board;
    int bank = config[uav-1].bank;

    if(debug)
        cout << "TACTORS: - board " << board << ", bank " << bank << endl;

    if(tactors[board].bankFreq[bank] != LOW_TACTOR_SOURCE)
    {
        tactors[board].bankFreq[bank] = LOW_TACTOR_SOURCE;
        tactors[board].SetSigSrc();
        Sleep(10);
    }
    
    // ACTIVATE!
    SendSignals(uav, LOW_TACTOR_LENGTH);
}

void UAVTactors::SignalHigh(int uav)
{
    int board = config[uav-1].board;
    int bank = config[uav-1].bank;

    if(debug)
        cout << "TACTORS: - board " << board << ", bank " << bank << endl;

    if(tactors[board].bankFreq[bank] != HIGH_TACTOR_SOURCE)
    {
        tactors[board].bankFreq[bank] = HIGH_TACTOR_SOURCE;
        tactors[board].SetSigSrc();
        Sleep(10);
    }
    
    // ACTIVATE!
    SendSignals(uav, HIGH_TACTOR_LENGTH);
}

void UAVTactors::SendSignals(int uav, int length)
{
    int board = config[uav-1].board;
    int tactor1 = config[uav-1].tactor1;
    int tactor2 = config[uav-1].tactor2;

    if(debug)
        cout << "TACTORS: - firing tactor1 [" << config[uav-1].tactor1 << "]" << endl;

    tactors[board].tactors.TacOnTime(tactor1, length, 0xFF);
    if(config[uav-1].both)
    {
        if(debug)
            cout << "TACTORS: - firing tactor2 [" << config[uav-1].tactor2 << "]" << endl;

        tactors[board].tactors.TacOnTime(tactor2, length, 0xFF);
    }
}

void UAVTactors::LoadConfig()
{
    // You have to push back 9 times here.
    // there are 2 boards:   0, 1
    //
    // there are 4 banks:    0, 1  on board 0
    //                       0, 1  on board 1
    //
    // there are 16 tactors: 0-3  on board 0, bank 0
    //                       4-7  on board 0, bank 1
    //                       0-3  on board 1, bank 2
    //                       4-7  on board 1, bank 3         
    //
    // TactorType(BOARD, BANK, TACTOR1, TACTOR2(optional))

    config.push_back(TactorType(0, 0, 0));          // 1
    config.push_back(TactorType(0, 0, 1, 2));       // 2
    config.push_back(TactorType(0, 0, 3));          // 3
    config.push_back(TactorType(0, 1, 4));          // 4
    config.push_back(TactorType(0, 1, 5, 6));       // 5
    config.push_back(TactorType(0, 1, 7));          // 6
    config.push_back(TactorType(1, 0, 1));          // 7
    config.push_back(TactorType(1, 0, 2, 3));     // 8
    config.push_back(TactorType(1, 1, 4));          // 9
}