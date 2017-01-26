#ifndef TACTORS_3X3_H
#define TACTORS_3X3_H

#include "tactor.h"

#include <vector>
#include <list>

namespace Sim
{
    struct TactorLocation
    {
        TactorLocation(int board, int tactor);

        int board;
        int tactor;

        static int getBank(int tactor)
        {
            switch(tactor)
            {
            case TAC1:
            case TAC2:
            case TAC3:
            case TAC4:
                return 0;
            case TAC5:
            case TAC6:
            case TAC7:
            case TAC8:
                return 1;
            default:
                return -1;
            };
        }
    };

    struct Sector
    {
        Sector(TactorLocation t);
        Sector(TactorLocation t1, TactorLocation t2);

        std::list<TactorLocation> tactors;
    };

    class Tactors3x3
    {
    public:

        Tactors3x3(
            const std::string& port1,
            const std::string& port2,
            const std::string& port3,
            bool enabled);

        ~Tactors3x3();

        void connect();
        void fireSector(int id, int duration = 40);
        void fireSectors(const std::vector<int>& ids, int duration = 40);
        void test();

        void setGain(int gain);
        void setGain(int gain, int sector);
        void setGains(int gain, std::vector<int>& ids);
        void setFreqs(int freq1, int freq2);

        void stop();

    private:

        void connectBox(int id, const std::string& port);

    private:

        CTactor boxes[3];
        bool connected[3];

        std::vector<Sector> sectors;
    };
}

#endif /* TACTORS_3X3_H */