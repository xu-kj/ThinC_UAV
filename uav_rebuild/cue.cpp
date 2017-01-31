#include "cue.h"
#include "output.h"
//#include "Controller.h"
//#include <GL/gl.h>

#include <stdlib.h>
#include <algorithm>

namespace SaraMatching
{
    std::stringstream& operator>>(std::stringstream& in, Cue * cue)
    {
        cue->Load(in);
        return in;
    }

    void Cue::Render(float progress, bool playing)
    {
        if(!playing)
        {
            for(int i = 0; i < 9; i++)
            {
                DrawBase(i, progress, playing);
            }
            return;
        }

        for(int i = 0; i < 9; i++)
        {
            if(find(sectors.begin(), sectors.end(), i) == sectors.end())
            {
                DrawBase(i, progress, playing);
            }
            else
            {
                DrawSector(i, progress);
            }
        }
    }

    int LoadInt(std::stringstream& in)
    {
        std::string str;
        in >> str;

        if(find(str.begin(), str.end(), '.') != str.end())
        {
            throw std::exception("invalid line in input file");
        }

        std::stringstream ss;
        ss << str;

        int result;
        ss >> result;

        return result;
    }

    float LoadFloat(std::stringstream& in)
    {
        std::string str;
        in >> str;

        if(find(str.begin(), str.end(), '.') == str.end())
        {
            throw std::exception("invalid line in input file");
        }

        std::stringstream ss;
        ss << str;

        float result;
        ss >> result;

        return result;
    }

    void Cue::Load(std::stringstream& in)
    {
        in >> matchToTactor;
        SetSectors(LoadSectorList(in));

        if(matchToTactor)
        {
            baseOpacity = LoadFloat(in);
            tactorStart = LoadInt(in);
            tactorEnd = LoadInt(in);
        }
        else
        {
            baseOpacity = LoadFloat(in);
            startOpacity = LoadFloat(in);
            endOpacity = LoadFloat(in);
        }

        in >> range1Start;
        range2Start = range1Start;
        in >> range1Low;
        in >> range1High;
        in >> range2Low;
        in >> range2High;
        in >> invertSlider1;
        in >> invertSlider2;
        in >> chainResult;

        size = 1;

        PrintDebug();
    }

    void Cue::DrawBase(int sectorID, float progress, bool playing)
    {
        Area a = GetSectorCoords(sectorID);

        // TODO: FIX THIS
        //glColor4f(0.0, 0.0, 0.0, baseOpacity);
        //a.Render();
    }

    void Cue::SetSectors(const std::vector<int>& sectors)
    {
        this->sectors = sectors;
    }

    void Cue::GetSector(int sectorID, int& row, int& col)
    {
        row = sectorID / 3;
        col = sectorID % 3;
    }

    Area Cue::GetSectorCoords(int sectorID)
    {
        //msclr::gcroot<RenderForm^> rf = Controller::Instance().GetRenderForm();
        //float w = rf->GetRenderWidth();
        //float h = rf->GetRenderHeight();

        // TODO: replace with how to actually get the size of the render area
        float w = 2560;
        float h = 1600;

        int row, col;
        GetSector(sectorID, row, col);

        // actual sector area
        float sw = w * (1.0f/3.0f);
        float sh = h * (1.0f/3.0f);
        float x = sw * col;
        float y = sh * row;

        // the trimmed area (with borders removed)
        // this is the area where the "scene" will actually render in the
        // UAV simulation
        float os = 0.01f;
        x += sh * os;
        y += sh * os;
        sh -= sh * os * 2.0f;
        sw -= sw * 0.075f;

        Area area;
        area.x = x;
        area.y = y;
        area.w = sw;
        area.h = sh;
        return area;
    }

    std::vector<int> Cue::LoadSectorList(std::stringstream& in)
    {
        std::vector<int> sectors;
        std::string str;
        in >> str;

        for(unsigned int i = 1; i < str.size() - 1; i++)
        {
            int val;
            std::stringstream ss;
            ss << str[i];
            ss >> val;
            sectors.push_back(val - 1);
        }

        return sectors;
    }

    void Cue::PrintDebug() const
    {
        int t = (int)type;

        /*irr::core::stringc str = "";
        str += "type:         " + t);
        str += "\nbaseOpacity:  " + baseOpacity);
        str += "\nstartOpacity: " + startOpacity);
        str += "\nendOpacity:   " + endOpacity);
        str += "\nstartTactor:  " + tactorStart);
        str += "\nendTactor:    " + tactorEnd);
        str += "\nrange1Start:  " + range1Start);
        str += "\nrange1Low:    " + range1Low);
        str += "\nrange1High:   " + range1High);
        str += "\nrange2Start:  " + range2Start);
        str += "\nrange2Low:    " + range2Low);
        str += "\nrange2High:   " + range2High);
        str += "\ninvertSlider1:" + invertSlider1);
        str += "\ninvertSlider2:" + invertSlider2);
        for(int i = 0; i < (int)sectors.size(); i++)
        {
            "" + i + ": " + sectors[i]);
        }
        "";*/
    }
}