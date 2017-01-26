#ifndef UAV_TACTORS_H
#define UAV_TACTORS_H

#include "tactor.h"
#include "tactors_3x3.h"

#include <list>
#include <vector>
#include <iostream>

// Defines a tactor signal for a UAV.
// Side signals (UAVs 1, 3, 4, 6, 7, 9) will have 1 tactor node associated.
// Center signals (UAVs 2, 5, 8) will have 2 tactor nodes associated. Care should be
// taken to insure center tactors are on the same bank on the same board.
struct TactorType
{
    TactorType(int board, int bank, int tactor)
        : board(board), bank(bank), tactor1(tactor), both(false)
    {}

    TactorType(int board, int bank, int tactor1, int tactor2)
        : board(board), bank(bank), tactor1(tactor1), tactor2(tactor2), both(true)
    {}

    int board;      // id of the board to use
    int bank;       // which bank on the board to use (0 or 1). this is for setting frequencies
    int tactor1;    // primary tactor to fire. value of 0-7
    int tactor2;    // optional tactor. center signals use two tactors. should be on the same bank as the first tactor
    bool both;      // fire tactor1 AND tactor2?
};

// Represents a complete tactor unit.
struct BoardType
{
    CTactor tactors;

    // each board has two banks
    int bankFreq[2];

    void SetSigSrc()
    {
        tactors.SetSigSrc(bankFreq[0], bankFreq[1]);
    }
};

class UAVTactors
{
public:
    bool Initialize(bool runTest = false);
    void SignalLow(int uav);
    void SignalHigh(int uav);

private:
    void LoadConfig();
    void SendSignals(int uav, int length);

    // non-implemented optimization
    bool sameFreq1; // optimization - if the freqs are the same, we don't have
    bool sameFreq2; // change them on each low -> high switch (and back)

    // the current tactor config
    static std::vector<TactorType> config;
    static std::vector<BoardType> tactors;
};

//extern UAVTactors tactors;
extern Sim::Tactors3x3 * tactors;

#endif /* UAV_TACTORS_H */