#ifndef CUE_H
#define CUE_H

#include "area.h"

#include <sstream>
#include <vector>

namespace SaraMatching
{
    enum CueType
    {
        CUE_UNDEFINED = 0,
        CUE_STEADY,
        CUE_TACTOR,
        CUE_MUDSPLASH,
        CUE_FLICKER,
        CUE_GRADUAL,
        CUE_TOTAL
    };

    class Cue
    {
        friend std::stringstream& operator>>(std::stringstream&, Cue *);

    public:

        Cue(CueType type = CUE_UNDEFINED)
            : type(type)
            , useRange1(true)
            , useRange2(true)
        {}

        void Render(float progress, bool playing);

        virtual void Load(std::stringstream& in);
        virtual void DrawSector(int sectorID, float progress) = 0;
        virtual void DrawBase(int sectorID, float progress, bool playing);
        virtual void OnCueBegin() {}
        virtual void OnCueEnd() {}

        void SetSectors(const std::vector<int>& sectors);
        virtual void SetValue1(float value) {selected1 = value;}
        virtual void SetValue2(float value) {selected2 = value;}
        virtual void SetValuesFromSelection(float selection1, float selection2) = 0;

        CueType GetType() const {return type;}
        bool GetMatchToTactor() const {return matchToTactor;}
        bool GetInvertSlider1() const {return invertSlider1;}
        bool GetInvertSlider2() const {return invertSlider2;}
        bool GetUseRange1() const {return useRange1;}
        bool GetUseRange2() const {return useRange2;}
        float GetSelected1() const {return selected1;}
        float GetSelected2() const {return selected2;}
        float GetRange1Value(float value) {return range1Low + value * (range1High - range1Low);}
        float GetRange2Value(float value) {return range2Low + value * (range2High - range2Low);}
        float GetRange1Start() const {return range1Start;}
        float GetRange2Start() const {return range2Start;}
        const bool GetChainResult() const {return chainResult;}

        void PrintDebug() const;

    public:

        static void GetSector(int sectorID, int& row, int& col);
        static Area GetSectorCoords(int sectorID);
        static std::vector<int> LoadSectorList(std::stringstream& in);

    protected:

        CueType type;
        bool matchToTactor;
        std::vector<int> sectors;

        int tactorStart;
        int tactorEnd;

        float baseOpacity;
        float startOpacity;
        float endOpacity;

        float size;

        bool invertSlider1;
        bool useRange1;
        float range1Start;
        float range1Low;
        float range1High;
        float selected1;

        bool invertSlider2;
        bool useRange2;
        float range2Start;
        float range2Low;
        float range2High;
        float selected2;

        bool chainResult;

        std::string tactorText1; // for when you are adjusting the tactors
        std::string tactorText2; // for when you are adjusting the tactors
        std::string visualText1; // for when you are adjusting the visual
        std::string visualText2; // for when you are adjusting the visual
    };
}

#endif /* CUE_H */