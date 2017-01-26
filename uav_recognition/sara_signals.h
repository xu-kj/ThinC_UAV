#ifndef SARA_SIGNALS_H
#define SARA_SIGNALS_H

#include "area.h"
#include "output.h"
#include "uav_tactors.h"
#include "image.h"
#include "network.h"
#include "globals.h"

#pragma warning (disable : 4482)

namespace SaraMatching {
    /*enum SignalType
    {
        VISUAL_MUDSPLASH = 0,
        VISUAL_FLICKER,
        TACTOR_MUDSPLASH,
        TACTOR_FLICKER,
        NONE
    };*/

    // ===========================================================================
    // SaraSignal
    // ===========================================================================

    class SaraSignal
    {
    public:
        SaraSignal(int correctResponseCode, float duration)
            : correctResponseCode(correctResponseCode)
            , duration(duration)
            , totalDuration(duration)
            , sector(-1)
            , killShading(false)
            , staticOn(false)
            , updated(false) // flag so we can make sure this updates before rendering
            , isResponseCorrect(false)
        {}

        virtual void update(float timestep)
        {
            updated = true;
            duration -= timestep;
            float progress = (totalDuration - duration) / totalDuration;
            onUpdate(timestep, progress);
        }

        virtual void render(irr::video::IVideoDriver * driver, int sector, int x, int y, int w, int h)
        {
            this->sector = sector;
            SaraMatching::Area a(
                (float)(x+5),
                (float)(y+6),
                (float)(w-62),
                (float)(h-10));
            onRender(driver, a);
        }

        void onResponse(int responseType)
        {
            isResponseCorrect = (responseType == correctResponseCode);
            std::cout << "Operator response: " << responseType << std::endl;
        }

        virtual void onUpdate(float timestep, float progress) {}
        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a) {}

        void setSector(int sector) {this->sector = sector;}

        bool getDone() const {return duration <= 0;}
        bool getKillShading() const {return killShading;}
        bool getStatic() const {return staticOn;}
        bool getUpdated() const {return updated;}
        virtual bool getIsCue() const {return false;}
        virtual int getRestoreBrightnessValue() const {return -1;}
        bool getIsResponseCorrect() const {return isResponseCorrect;}

    protected:

        void sendInitiallyNotCorrectMessage(bool initiallyNotCorrect)
        {
            if(USE_NETWORK)
            {
                int message = initiallyNotCorrect ? 13 : 14;
                Network::sendMessageCamButton(message, sector);
            }
        }

    public:

        static std::vector<int> getMudsplashSectors(int sector)
        {
            std::vector<int> s;
            
            if(sector == 0)
            {
                s.push_back(3);
                s.push_back(6);
                s.push_back(8);
            }
            else if(sector == 1)
            {
                s.push_back(2);
                s.push_back(3);
                s.push_back(7);
            }
            else if(sector == 2)
            {
                s.push_back(0);
                s.push_back(4);
                s.push_back(6);
            }
            else if(sector == 3)
            {
                s.push_back(1);
                s.push_back(5);
                s.push_back(8);
            }
            else if(sector == 4)
            {
                s.push_back(1);
                s.push_back(5);
                s.push_back(6);
            }
            else if(sector == 5)
            {
                s.push_back(0);
                s.push_back(2);
                s.push_back(7);
            }
            else if(sector == 6)
            {
                s.push_back(1);
                s.push_back(3);
                s.push_back(7);
            }
            else if(sector == 7)
            {
                s.push_back(0);
                s.push_back(4);
                s.push_back(5);
            }
            else if(sector == 8)
            {
                s.push_back(0);
                s.push_back(2);
                s.push_back(4);
            }

            return s;
        }

        static void setStaticImages(GUIImage * s1)
        {
            static1 = s1;
        }

        static irr::core::rect<irr::s32> toIrrRect(Area& a)
        {
            irr::core::rect<irr::s32> result(
                irr::s32(a.x),
                irr::s32(a.y),
                irr::s32(a.w + a.x),
                irr::s32(a.h + a.y));
            return result;
        }

    public:

        static GUIImage * static1;

    public:

        float duration;
        float totalDuration;
        bool flashOn;
        int sector;
        bool killShading; // whether to allow baseline shading
        bool staticOn; // whether to allow static on the viewport
        bool updated;

    private:

        int correctResponseCode;
        bool isResponseCorrect; // whether the operator's last response selection is correct
    };

    class SaraSignalTimeFilter : public SaraSignal
    {
    public:

        SaraSignalTimeFilter(int correctResponseCode, float duration, float startTime, float endTime)
            : SaraSignal(correctResponseCode, duration)
            , startTime(startTime)
            , endTime(endTime)
            , stopRendering(false)
        {}

        virtual void update(float timestep)
        {
            updated = true;

            duration -= timestep;
            float progress = (totalDuration - duration) / totalDuration;
            totalProgress = progress;

            float s = startTime / totalDuration;
            float e = endTime / totalDuration;
            float realProgess = -1;

            if(progress >= s)
            {
                realProgress = (progress - s) / (e - s);

                if(realProgress >= 1)
                    stopRendering = true;
                else
                    onUpdate(timestep, realProgress);
            }
        }

        virtual void render(irr::video::IVideoDriver * driver, int sector, int x, int y, int w, int h)
        {
            if(stopRendering)
                return;

            this->sector = sector;
            SaraMatching::Area a(
                (float)(x+5),
                (float)(y+6),
                (float)(w-62),
                (float)(h-10));
            onRender(driver, a);
        }

    protected:

        float startTime;
        float endTime;
        float totalProgress;
        float realProgress;
        bool stopRendering;
    };

    // ===========================================================================
    // SaraSignalFlickerVisual
    // ===========================================================================

    class SaraSignalFlickerVisual : public SaraSignalTimeFilter
    {
    public:
        SaraSignalFlickerVisual(float duration, float flashRate, float opacity, float startTime, float endTime)
            : SaraSignalTimeFilter(-1, duration, startTime, endTime)
            , flashOn(false)
            , flashRate(flashRate)
            , opacity(opacity)
        {}

        virtual void onUpdate(float timestep, float progress)
        {
            int curPeriod = int(totalProgress / flashRate);
            if(curPeriod % 2 == 0)
                flashOn = true;
            else
                flashOn = false;
        }

        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a)
        {
            if(flashOn)
                a.Render(driver, irr::video::SColor(int(opacity*255.f), 0, 0, 0));
        }

    private:
        bool flashOn;
        float flashRate;
        float opacity;
    };

    // ===========================================================================
    // SaraSignalMudsplashVisual
    // ===========================================================================

    class SaraSignalMudsplashVisual : public SaraSignalTimeFilter
    {
    public:

        SaraSignalMudsplashVisual(float duration, float flashRate, float opacity, float startTime, float endTime)
            : SaraSignalTimeFilter(-1, duration, startTime, endTime)
            , flashOn(false)
            , flashRate(flashRate)
            , opacity(opacity)
        {}

        virtual void onUpdate(float timestep, float progress)
        {
            bool lastFlash = flashOn;

            int curPeriod = int(totalProgress / flashRate);
            if(curPeriod % 2 == 0)
            {
                flashOn = true;
            }
            else
            {
                flashOn = false;
            }
        }

        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a)
        {
            if(flashOn)
                DrawMultipleRectangles(driver, a);
        }

    private:

        void SaraSignalMudsplashVisual::DrawMultipleRectangles(irr::video::IVideoDriver * driver, SaraMatching::Area& area)
        {
            int show = 5;

            Area a[12];
            a[0] = area;

            if(show > 0)    a[1] = a[0].Split(Area::SplitType::SPLIT_BOTTOM);
            if(show > 1)    a[2] = a[1].Split(Area::SplitType::SPLIT_LEFT);
            if(show > 2)    a[3] = a[0].Split(Area::SplitType::SPLIT_LEFT);
            if(show > 3)    a[4] = a[2].Split(Area::SplitType::SPLIT_TOP);
            if(show > 4)    a[5] = a[0].Split(Area::SplitType::SPLIT_LEFT);
            if(show > 5)    a[6] = a[0].Split(Area::SplitType::SPLIT_TOP);
            if(show > 6)    a[7] = a[3].Split(Area::SplitType::SPLIT_LEFT);
            if(show > 7)    a[8] = a[1].Split(Area::SplitType::SPLIT_LEFT);
            if(show > 8)    a[9] = a[3].Split(Area::SplitType::SPLIT_TOP);
            if(show > 9)    a[10] = a[2].Split(Area::SplitType::SPLIT_LEFT);
            if(show > 10)   a[11] = a[1].Split(Area::SplitType::SPLIT_TOP);

            for(int i = 0; i <= show; i++)
                DrawArea(driver, 0.9f, a[i]);
        }

        void SaraSignalMudsplashVisual::DrawArea(irr::video::IVideoDriver * driver, float size, Area& a1)
        {
            // pick one half of the area for the first box
            // (1=left, 2=right, 3=top, 4=bottom)
            int half1 = rand() % 4 + 1;
            half1 = 3; // let's actually make this deterministic
            Area a2 = a1.Split(Area::SplitType(half1));

            // pick half of the remaining area for the second box and
            // keep the remainder for the third box.
            // this pick depends on the pick for the first half: if the first
            // was left/right, this needs to be top/bottom; if the first was
            // top/bottom, this needs to be left/right.
            int half2 = rand() % 2 + (half1 == 1 || half1 == 2) ? 1 : 3;
            half2 = 1; // let's actually make this deterministic
            Area a3 = a2.Split(Area::SplitType(half2), 0.6f);

            // make a3 a little further from the edges of the sector
            a3.Split(Area::SplitType(1), 0.9f);
            a3.Split(Area::SplitType(3), 0.9f);

            a1.ShrinkRelative(1.0f - size);
            a2.ShrinkRelative(1.0f - size);
            a3.ShrinkRelative(1.0f - size);
            a1.NormalizeDown();
            a2.NormalizeDown();
            a3.NormalizeDown();

            static1->setPosition(toIrrRect(a1));
            static1->draw();

            static1->setPosition(toIrrRect(a2));
            static1->draw();

            static1->setPosition(toIrrRect(a3));
            static1->draw();

            //a1.Render(driver, irr::video::SColor(int(opacity * 255.0f), 0, 0, 0));
            //a2.Render(driver, irr::video::SColor(int(opacity * 255.0f), 0, 0, 0));
            //a3.Render(driver, irr::video::SColor(int(opacity * 255.0f), 0, 0, 0));
        }

    private:

        bool flashOn;
        float flashRate;
        float opacity;
    };

    // ===========================================================================
    // SaraSignalFlickerTactor
    // ===========================================================================

    class SaraSignalFlickerTactor : public SaraSignalTimeFilter
    {
    public:
        SaraSignalFlickerTactor(float duration, float flashRate, float intensity, float startTime, float endTime)
            : SaraSignalTimeFilter(-1, duration, startTime, endTime)
            , flashOn(false)
            , flashRate(flashRate)
            , intensity(intensity)
        {
            /*
            std::cout << "TactorFlicker" << std::endl;
            std::cout << "duration:  " << duration << std::endl;
            std::cout << "flashRate: " << flashRate << std::endl;
            std::cout << "intensity: " << intensity << std::endl;
            std::cout << "startTime: " << startTime << std::endl;
            std::cout << "endTime:   " << endTime << std::endl;
            */
        }

        virtual void onUpdate(float timestep, float progress)
        {
            //std::cout << "sector: " << sector << std::endl;

            if(sector < 0)
                return;

            //std::cout << "update" << std::endl;

            int curPeriod = int(totalProgress / flashRate);
            if(curPeriod % 2 == 1)
            {
                if(!flashOn)
                {
                    //std::cout << "gain: " << intensity << std::endl;
                    int tactorTime = (int)(flashRate * totalDuration * 100.0f);
                    std::vector<int> ids;
                    if(tactorTime > 0)
                    {
                        //std::cout << sector << std::endl;
                        for(int i = 0; i < 9; i++)
                        {
                            if(i == sector - 1)
                                continue;
                            ids.push_back(i);
                            //std::cout << i;
                        }
                        //std::cout << std::endl;
                        tactors->setGains(int(intensity * 255.f), ids);
                        tactors->fireSectors(ids, tactorTime);
                    }
                }
                flashOn = true;
            }
            else
            {
                flashOn = false;
            }
        }

        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a)
        {}

    private:
        bool flashOn;
        float flashRate;
        float intensity;
    };

    // ===========================================================================
    // SaraSignalMudsplashTactor
    // ===========================================================================

    class SaraSignalMudsplashTactor : public SaraSignalTimeFilter
    {
    public:
        SaraSignalMudsplashTactor(float duration, float flashRate, float intensity, float startTime, float endTime)
            : SaraSignalTimeFilter(-1, duration, startTime, endTime)
            , flashOn(false)
            , flashRate(flashRate)
            , intensity(intensity)
        {
            //std::cout << "[" << totalDuration << "] tactile mudsplash started" << std::endl;
        }

        virtual void onUpdate(float timestep, float progress)
        {
            if(sector < 0)
                return;

            int curPeriod = int(totalProgress / flashRate);
            if(curPeriod % 2 == 0)
            {
                if(!flashOn)
                {
                    int tactorTime = (int)(flashRate * totalDuration * 100.0f);
                    if(tactorTime > 0)
                    {
                        //std::cout << "[" << totalDuration - duration
                        //  << "] tactile mudsplash fired at intensity "
                        //  << intensity
                        //  << " for " << tactorTime << " centiseconds"
                        //  << std::endl;

                        std::vector<int> s = SaraSignal::getMudsplashSectors(sector - 1);
                        tactors->setGains(int(intensity * 255.f), s);
                        tactors->fireSectors(s, tactorTime);

                        //std::cout << "sector: " << sector << std::endl;
                        //for(std::vector<int>::iterator it = s.begin(); it != s.end(); it++)
                        //  std::cout << *it << std::endl;
                    }
                }
                flashOn = true;
            }
            else
            {
                flashOn = false;
            }
        }

        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a)
        {}

    private:
        bool flashOn;
        float flashRate;
        float intensity;
    };

    // ===========================================================================
    // SaraSignalTactorCue
    // ===========================================================================

    class SaraSignalTactorCue : public SaraSignal
    {
    public:
        SaraSignalTactorCue(float duration, float flashRate, float intensity1, float intensity2, float switchUpTime, float switchDownTime)
            : SaraSignal(-1, duration)
            , flashOn(false)
            , flashRate(flashRate)
            , intensity1(intensity1 * 255.f)
            , intensity2(intensity2 * 255.f)
            , switchUpTime(switchUpTime)
            , switchDownTime(switchDownTime)
        {
            /*
            std::cout << "sector:     " << sector << std::endl;
            std::cout << "duration:   " << duration << std::endl;
            std::cout << "flashRate:  " << flashRate << std::endl;
            std::cout << "intensity1: " << intensity1 << std::endl;
            std::cout << "intensity2: " << intensity2 << std::endl;
            std::cout << "switchTime: " << switchTime << std::endl;
            */
        }

        virtual void onUpdate(float timestep, float progress)
        {
            if(sector < 0)
                return;

            float timeSoFar = totalDuration - duration;
            bool switchUp = switchUpTime <= timeSoFar;
            bool switchDown = switchDownTime <= timeSoFar;
            float intensity = (switchUp && !switchDown) ? intensity2 : intensity1;

            int curPeriod = int(progress / flashRate);
            if(curPeriod % 2 == 0)
            {
                if(!flashOn)
                {
                    int tactorTime = (int)(flashRate * totalDuration * 100.0f);
                    tactors->setGain((int)intensity);
                    if(tactorTime > 0)
                        tactors->fireSector(sector - 1, tactorTime);
                    //std::cout << "gain: " << intensity << " at sector " << sector - 1 << std::endl;
                }
                flashOn = true;
            }
            else
            {
                flashOn = false;
            }
        }

        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a)
        {}

        virtual bool getIsCue() const {return true;}

    private:
        bool flashOn;
        float flashRate;
        float intensity1;
        float intensity2;
        float switchUpTime;
        float switchDownTime;
    };

    // ===========================================================================
    // SaraSignalVisualCue
    // ===========================================================================

    class SaraSignalVisualCue : public SaraSignal
    {
    public:
        SaraSignalVisualCue(float duration, float opacity1, float opacity2, float switchUpTime, float switchDownTime)
            : SaraSignal(-1, duration)
            , opacity(0)
            , opacity1(opacity1)
            , opacity2(opacity2)
            , switchUpTime(switchUpTime)
            , switchDownTime(switchDownTime)
            , switchedUp(false)
            , switchedDown(false)
        {
            killShading = true;

            //std::cout << "[" << totalDuration << "] visual cue started" << std::endl;
        }

        virtual void onUpdate(float timestep, float progress)
        {
            float timeSoFar = totalDuration - duration;
            bool switchUp = switchUpTime <= timeSoFar;
            bool switchDown = switchDownTime <= timeSoFar;
            opacity = (switchUp && !switchDown) ? opacity2 : opacity1;

            if(!switchedUp && switchUp)
            {
                //std::cout << "[" << totalDuration - duration << "] visual cue switching up" << std::endl;
                switchedUp = true;
            }

            if(!switchedDown && switchDown)
            {
                //std::cout << "[" << totalDuration - duration << "] visual cue switching down" << std::endl;
                switchedDown = true;
            }
        }

        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a)
        {
            a.Render(driver, irr::video::SColor(int(opacity*255), 0, 0, 0));
        }

        virtual bool getIsCue() const {return true;}
        virtual int getRestoreBrightnessValue() const {return 0;}

    private:

        float opacity;
        float opacity1;
        float opacity2;
        float switchUpTime;
        float switchDownTime;

        // for output
        bool switchedUp;
        bool switchedDown;
    };

    // ===========================================================================
    // SaraSignalVisualGradualCue
    // ===========================================================================

    class SaraSignalVisualGradualCue : public SaraSignal
    {
    public:
        SaraSignalVisualGradualCue(float duration, float opacity1, float opacity2, float startOpacityTime, float fullOpacityTime)
            : SaraSignal(-1, duration)
            , opacity(0)
            , opacity1(opacity1)
            , opacity2(opacity2)
            , startOpacityTime(startOpacityTime)
            , fullOpacityTime(fullOpacityTime)
        {
            killShading = true;
        }

        virtual void onUpdate(float timestep, float progress)
        {
            float time = totalDuration - duration;

            if(time < startOpacityTime)
            {
                opacity = opacity1;
            }
            else if(time >= fullOpacityTime)
            {
                opacity = opacity2;
            }
            else
            {
                float changeTime = (totalDuration - (totalDuration - fullOpacityTime)) - startOpacityTime;
                float timeSoFar = time - startOpacityTime;
                float percent = timeSoFar / changeTime;
                opacity = opacity1 + percent * (opacity2 - opacity1);
            }
        }

        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a)
        {
            a.Render(driver, irr::video::SColor(int(opacity*255), 0, 0, 0));
        }

        virtual bool getIsCue() const {return true;}
        virtual int getRestoreBrightnessValue() const {return 0;}

    private:

        float opacity;
        float opacity1;
        float opacity2;
        float startOpacityTime;
        float fullOpacityTime;
    };

    // ===========================================================================
    // SaraSignalTactorGradualCue
    // ===========================================================================

    class SaraSignalTactorGradualCue : public SaraSignal
    {
    public:
        SaraSignalTactorGradualCue(float duration, float flashRate, float intensity1, float intensity2, float startIntensityTime, float fullIntensityTime)
            : SaraSignal(-1, duration)
            , flashOn(false)
            , intensity(0)
            , flashRate(flashRate)
            , intensity1(intensity1 * 255.0f)
            , intensity2(intensity2 * 255.0f)
            , startIntensityTime(startIntensityTime)
            , fullIntensityTime(fullIntensityTime)
        {}

        virtual void onUpdate(float timestep, float progress)
        {
            // figure out what the intensity should be
            float time = totalDuration - duration;
            if(time < startIntensityTime)
            {
                intensity = intensity1;
            }
            else if(time >= fullIntensityTime)
            {
                intensity = intensity2;
            }
            else
            {
                float changeTime = (totalDuration - (totalDuration - fullIntensityTime)) - startIntensityTime;
                float timeSoFar = time - startIntensityTime;
                float percent = timeSoFar / changeTime;
                intensity = intensity1 + percent * (intensity2 - intensity1);
            }

            // figure out when to fire the tactors
            int curPeriod = int(progress / flashRate);
            if(curPeriod % 2 == 0)
            {
                if(!flashOn)
                {
                    int tactorTime = (int)(flashRate * totalDuration * 100.0f);
                    tactors->setGain((int)intensity);
                    if(tactorTime > 0)
                        tactors->fireSector(sector - 1, tactorTime);
                    //std::cout << intensity << std::endl;
                    //std::cout << "gain: " << intensity << " at sector " << sector - 1 << std::endl;
                }
                flashOn = true;
            }
            else
            {
                flashOn = false;
            }
        }

        virtual bool getIsCue() const {return true;}

    private:

        bool flashOn;
        float intensity;
        float flashRate;
        float intensity1;
        float intensity2;
        float startIntensityTime;
        float fullIntensityTime;
    };

    // ===========================================================================
    // SaraSignalVisualCueV1
    // ===========================================================================

    class SaraSignalVisualCueV1 : public SaraSignal
    {
    public:
        SaraSignalVisualCueV1(int correctResponseCode, float duration, float opacity1, float opacity2, float opacity3, float switchUpTime, float switchNotCorrectTime, float switchDownTime)
            : SaraSignal(correctResponseCode, duration)
            , opacity(0)
            , opacity1(opacity1)
            , opacity2(opacity2)
            , opacity3(opacity3)
            , switchUpTime(switchUpTime)
            , switchNotCorrectTime(switchNotCorrectTime)
            , switchDownTime(switchDownTime)
            , switchedUp(false)
            , switchedNotCorrect(false)
            , switchedDown(false)
            , initiallyNotCorrect(true)
        {
            killShading = true;
            //std::cout << "[" << totalDuration << "] visual cue v1 started" << std::endl;
        }

        virtual void onUpdate(float timestep, float progress)
        {
            float timeSoFar = totalDuration - duration;
            bool switchUp = switchUpTime <= timeSoFar;
            bool switchNotCorrect = switchNotCorrectTime <= timeSoFar;
            bool switchDown = switchDownTime <= timeSoFar;

            if(!switchedUp && switchUp)
            {
                //std::cout << "[" << totalDuration - duration << "] visual cue switching up from " << opacity << std::endl;
                switchedUp = true;
            }

            if(!switchedNotCorrect && switchNotCorrect)
            {
                //std::cout << "[" << totalDuration - duration << "] visual cue switching not correct from " << opacity << std::endl;
                switchedNotCorrect = true;
                initiallyNotCorrect = !getIsResponseCorrect();
                sendInitiallyNotCorrectMessage(initiallyNotCorrect);
                //std::cout << "response not correct: " << initiallyNotCorrect << std::endl;
            }

            if(!switchedDown && switchDown)
            {
                //std::cout << "[" << totalDuration - duration << "] visual cue switching down from " << opacity << std::endl;
                switchedDown = true;
            }

            opacity = opacity1;
            if(switchUp) opacity = opacity2;
            if(switchedNotCorrect && initiallyNotCorrect) opacity = opacity3;
            if(switchedDown) opacity = opacity1;

            //std::cout << "[" << totalDuration - duration << "] opacity = " << opacity << std::endl;
        }

        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a)
        {
            a.Render(driver, irr::video::SColor(int(opacity*255), 0, 0, 0));
        }

        virtual bool getIsCue() const {return true;}
        virtual int getRestoreBrightnessValue() const {return 0;}

    private:

        float opacity;
        float opacity1;
        float opacity2;
        float opacity3;
        float switchUpTime;
        float switchDownTime;
        float switchNotCorrectTime;

        // for output
        bool switchedUp;
        bool switchedNotCorrect;
        bool switchedDown;
        bool initiallyNotCorrect;
    };

    // ===========================================================================
    // SaraSignalVisualCueV2
    // ===========================================================================

    class SaraSignalVisualCueV2 : public SaraSignal
    {
    public:
        SaraSignalVisualCueV2(int correctResponseCode, float duration, float opacity1, float opacity2, float switchUpTime, float switchNotCorrectTime, float switchDownTime)
            : SaraSignal(correctResponseCode, duration)
            , opacity(0)
            , opacity1(opacity1)
            , opacity2(opacity2)
            , switchUpTime(switchUpTime)
            , switchNotCorrectTime(switchNotCorrectTime)
            , switchDownTime(switchDownTime)
            , switchedUp(false)
            , switchedNotCorrect(false)
            , switchedDown(false)
            , initiallyNotCorrect(true)
            , compareDone(false)
            , compareHalfDone(false)
        {
            killShading = true;
            //std::cout << "[" << totalDuration << "] visual cue v2 started" << std::endl;
        }

        virtual void onUpdate(float timestep, float progress)
        {
            float timeSoFar = totalDuration - duration;
            bool switchUp = switchUpTime <= timeSoFar;
            bool switchNotCorrect = switchNotCorrectTime <= timeSoFar;
            bool compareHalfDoneTime = (switchNotCorrectTime + 0.5) <= timeSoFar;
            bool compareDoneTime = (switchNotCorrectTime + 1) <= timeSoFar;
            bool switchDown = switchDownTime <= timeSoFar;

            if(!switchedUp && switchUp)
            {
                //std::cout << "[" << totalDuration - duration << "] visual cue switching up from " << opacity << std::endl;
                switchedUp = true;
            }

            if(!switchedNotCorrect && switchNotCorrect)
            {
                //std::cout << "[" << totalDuration - duration << "] visual cue switching not correct from " << opacity << std::endl;
                switchedNotCorrect = true;
                initiallyNotCorrect = !getIsResponseCorrect();
                sendInitiallyNotCorrectMessage(initiallyNotCorrect);
                //std::cout << "response not correct: " << initiallyNotCorrect << std::endl;
            }

            if(!compareHalfDone && compareHalfDoneTime)
            {
                //std::cout << "[" << totalDuration - duration << "] visual cue compare half done from " << opacity << std::endl;
                compareHalfDone = true;
            }

            if(!compareDone && compareDoneTime)
            {
                //std::cout << "[" << totalDuration - duration << "] visual cue compare done from " << opacity << std::endl;
                compareDone = true;
            }

            if(!switchedDown && switchDown)
            {
                //std::cout << "[" << totalDuration - duration << "] visual cue switching down from " << opacity << std::endl;
                switchedDown = true;
            }

            opacity = opacity1;
            if(switchUp) opacity = opacity2;
            if(switchedNotCorrect && initiallyNotCorrect)
            {
                opacity = opacity1;
                if(compareHalfDone) opacity = opacity2;
            }
            if(switchedDown) opacity = opacity1;

            //std::cout << "[" << totalDuration - duration << "] opacity = " << opacity << std::endl;
        }

        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a)
        {
            a.Render(driver, irr::video::SColor(int(opacity*255), 0, 0, 0));
        }

        virtual bool getIsCue() const {return true;}
        virtual int getRestoreBrightnessValue() const {return 0;}

    private:

        float opacity;
        float opacity1;
        float opacity2;
        float switchUpTime;
        float switchDownTime;
        float switchNotCorrectTime;

        // for output
        bool switchedUp;
        bool switchedNotCorrect;
        bool switchedDown;
        bool initiallyNotCorrect;
        bool compareHalfDone;
        bool compareDone;
    };

    // ===========================================================================
    // SaraSignalTactorCueV1
    // ===========================================================================

    class SaraSignalTactorCueV1 : public SaraSignal
    {
    public:
        SaraSignalTactorCueV1(int correctResponseCode, float duration, float flashRate, float intensity1, float intensity2, float intensity3, float switchUpTime, float switchNotCorrectTime, float switchDownTime)
            : SaraSignal(correctResponseCode, duration)
            , flashOn(false)
            , flashRate(flashRate)
            , intensity(0)
            , intensity1(intensity1 * 255.f)
            , intensity2(intensity2 * 255.f)
            , intensity3(intensity3 * 255.f)
            , switchUpTime(switchUpTime)
            , switchNotCorrectTime(switchNotCorrectTime)
            , switchDownTime(switchDownTime)
            , switchedUp(false)
            , switchedNotCorrect(false)
            , switchedDown(false)
            , initiallyNotCorrect(true)
        {
            //killShading = true;
            //std::cout << "[" << totalDuration << "] tactor cue v1 started" << std::endl;
        }

        virtual void onUpdate(float timestep, float progress)
        {
            if(sector < 0)
                return;

            float timeSoFar = totalDuration - duration;
            bool switchUp = switchUpTime <= timeSoFar;
            bool switchNotCorrect = switchNotCorrectTime <= timeSoFar;
            bool switchDown = switchDownTime <= timeSoFar;

            if(!switchedUp && switchUp)
            {
                //std::cout << "[" << totalDuration - duration << "] tactor cue switching up from " << intensity << std::endl;
                switchedUp = true;
            }

            if(!switchedNotCorrect && switchNotCorrect)
            {
                //std::cout << "[" << totalDuration - duration << "] tactor cue switching not correct from " << intensity << std::endl;
                switchedNotCorrect = true;
                initiallyNotCorrect = !getIsResponseCorrect();
                //std::cout << "response not correct: " << initiallyNotCorrect << std::endl;
                sendInitiallyNotCorrectMessage(initiallyNotCorrect);
            }

            if(!switchedDown && switchDown)
            {
                //std::cout << "[" << totalDuration - duration << "] tactor cue switching down from " << intensity << std::endl;
                switchedDown = true;
            }

            intensity = intensity1;
            if(switchUp) intensity = intensity2;
            if(switchedNotCorrect && initiallyNotCorrect) intensity = intensity3;
            if(switchedDown) intensity = intensity1;

            int curPeriod = int(progress / flashRate);
            if(curPeriod % 2 == 0)
            {
                if(!flashOn)
                {
                    int tactorTime = (int)(flashRate * totalDuration * 100.0f);
                    tactors->setGain((int)intensity);
                    if(tactorTime > 0 && intensity > 0)
                        tactors->fireSector(sector - 1, tactorTime);
                    //std::cout << "[" << totalDuration - duration << "] gain: " << intensity << " at sector " << sector - 1 << std::endl;
                }
                flashOn = true;
            }
            else
            {
                flashOn = false;
            }

            //if(!flashOn)
            //  intensity = 1;

            //std::cout << "[" << totalDuration - duration << "] intensity = " << intensity << std::endl;
        }

        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a)
        {
            //a.Render(driver, irr::video::SColor(int(intensity*255), 0, 0, 0));
        }

        virtual bool getIsCue() const {return true;}

    private:

        bool flashOn;
        float flashRate;
        float intensity;
        float intensity1;
        float intensity2;
        float intensity3;
        float switchUpTime;
        float switchDownTime;
        float switchNotCorrectTime;

        // for output
        bool switchedUp;
        bool switchedNotCorrect;
        bool switchedDown;
        bool initiallyNotCorrect;
    };

    // ===========================================================================
    // SaraSignalTactorCueV2
    // ===========================================================================

    class SaraSignalTactorCueV2 : public SaraSignal
    {
    public:
        SaraSignalTactorCueV2(int correctResponseCode, float duration, float flashRate, float intensity1, float intensity2, float switchUpTime, float switchNotCorrectTime, float switchDownTime, float comparisonTime)
            : SaraSignal(correctResponseCode, duration)
            , flashOn(false)
            , flashRate(flashRate)
            , intensity(0)
            , intensity1(intensity1 * 255.f)
            , intensity2(intensity2 * 255.f)
            , switchUpTime(switchUpTime)
            , switchNotCorrectTime(switchNotCorrectTime)
            , switchDownTime(switchDownTime)
            , comparisonTime(comparisonTime)
            , switchedUp(false)
            , switchedNotCorrect(false)
            , switchedDown(false)
            , initiallyNotCorrect(true)
            , compareDone(false)
            , compareHalfDone(false)
        {
            //killShading = true;
            //std::cout << "[" << totalDuration << "] tactor cue v2 started" << std::endl;
        }

        virtual void onUpdate(float timestep, float progress)
        {
            if(sector < 0)
                return;

            float timeSoFar = totalDuration - duration;
            bool switchUp = switchUpTime <= timeSoFar;
            bool switchNotCorrect = switchNotCorrectTime <= timeSoFar;
            bool compareHalfDoneTime = (switchNotCorrectTime + (comparisonTime / 2.0f)) <= timeSoFar;
            bool compareDoneTime = (switchNotCorrectTime + comparisonTime) <= timeSoFar;
            bool switchDown = switchDownTime <= timeSoFar;
            bool comparingStart = false;
            bool comparingMiddle = false;

            if(!switchedUp && switchUp)
            {
                //std::cout << "[" << totalDuration - duration << "] tactor cue switching up from " << intensity << std::endl;
                switchedUp = true;
            }

            if(!switchedNotCorrect && switchNotCorrect)
            {
                //std::cout << "[" << totalDuration - duration << "] tactor cue switching not correct from " << intensity << std::endl;
                switchedNotCorrect = true;
                comparingStart = true;
                initiallyNotCorrect = !getIsResponseCorrect();
                sendInitiallyNotCorrectMessage(initiallyNotCorrect);
                //std::cout << "response not correct: " << initiallyNotCorrect << std::endl;
            }

            if(!compareHalfDone && compareHalfDoneTime)
            {
                //std::cout << "[" << totalDuration - duration << "] tactor cue compare half done from " << intensity << std::endl;
                compareHalfDone = true;
                comparingMiddle = true;
            }

            if(!compareDone && compareDoneTime)
            {
                //std::cout << "[" << totalDuration - duration << "] tactor cue compare done from " << intensity << std::endl;
                compareDone = true;
            }

            if(!switchedDown && switchDown)
            {
                //std::cout << "[" << totalDuration - duration << "] tactor cue switching down from " << intensity << std::endl;
                switchedDown = true;
            }

            intensity = intensity1;
            if(switchUp) intensity = intensity2;
            if(switchedNotCorrect && initiallyNotCorrect)
            {
                intensity = intensity1;
                if(compareHalfDone) intensity = intensity2;
            }
            if(switchedDown) intensity = intensity1;

            if(switchedNotCorrect && initiallyNotCorrect && !compareDone)
            {
                if(comparingStart)
                {
                    int tactorTime = (int)(comparisonTime * 100.0f);
                    tactors->setGain((int)intensity);
                    tactors->fireSector(sector - 1, tactorTime);
                    //std::cout << "[" << totalDuration - duration << "] gain: " << intensity << " at sector " << sector - 1 << std::endl;
                }

                if(comparingMiddle)
                {
                    tactors->setGain((int)intensity);
                    //std::cout << "[" << totalDuration - duration << "] gain: " << intensity << " at sector " << sector - 1 << std::endl;
                }
            }
            else
            {
                int curPeriod = int(progress / flashRate);
                if(curPeriod % 2 == 0)
                {
                    if(!flashOn)
                    {
                        int tactorTime = (int)(flashRate * totalDuration * 100.0f);
                        tactors->setGain((int)intensity);
                        if(tactorTime > 0)
                            tactors->fireSector(sector - 1, tactorTime);
                        //std::cout << "[" << totalDuration - duration << "] gain: " << intensity << " at sector " << sector - 1 << std::endl;
                    }
                    flashOn = true;
                }
                else
                {
                    flashOn = false;
                }
            }

            //if(!flashOn)
            //  intensity = 1;

            //std::cout << "[" << totalDuration - duration << "] intensity = " << intensity << std::endl;
        }

        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a)
        {
            //a.Render(driver, irr::video::SColor(int(intensity*255), 0, 0, 0));
        }

        virtual bool getIsCue() const {return true;}

    private:

        bool flashOn;
        float flashRate;
        float intensity;
        float intensity1;
        float intensity2;
        float switchUpTime;
        float switchDownTime;
        float switchNotCorrectTime;
        float comparisonTime;

        // for output
        bool switchedUp;
        bool switchedNotCorrect;
        bool switchedDown;
        bool initiallyNotCorrect;
        bool compareHalfDone;
        bool compareDone;
    };

    // ===========================================================================
    // SaraSignalTactorCueV3
    // ===========================================================================

    class SaraSignalTactorCueV3 : public SaraSignal
    {
    public:
        SaraSignalTactorCueV3(int correctResponseCode, float duration, float flashRate1, float flashRate2, float intensity1, float intensity2, float switchUpTime, float switchStartWarnTime, float switchEndWarnTime, float switchDownTime)
            : SaraSignal(correctResponseCode, duration)
            , flashOn(false)
            , flashRate1(flashRate1)
            , flashRate2(flashRate2)
            , intensity(0)
            , intensity1(intensity1 * 255.f)
            , intensity2(intensity2 * 255.f)
            , switchUpTime(switchUpTime)
            , switchStartWarnTime(switchStartWarnTime)
            , switchEndWarnTime(switchEndWarnTime)
            , switchDownTime(switchDownTime)
            , switchedUp(false)
            , switchedDown(false)
            , switchedStartWarn(false)
            , switchedEndWarn(false)
        {
            //killShading = true;
            //std::cout << "[" << totalDuration << "] tactor cue v3 started" << std::endl;
        }

        virtual void onUpdate(float timestep, float progress)
        {
            if(sector < 0)
                return;

            float timeSoFar = totalDuration - duration;
            bool switchUp = switchUpTime <= timeSoFar;
            bool switchDown = switchDownTime <= timeSoFar;
            bool switchStartWarn = switchStartWarnTime <= timeSoFar;
            bool switchEndWarn = switchEndWarnTime <= timeSoFar;

            if(!switchedUp && switchUp)
            {
                //std::cout << "[" << totalDuration - duration << "] tactor cue switching up from " << intensity << std::endl;
                switchedUp = true;
            }

            if(!switchedDown && switchDown)
            {
                //std::cout << "[" << totalDuration - duration << "] tactor cue switching down from " << intensity << std::endl;
                switchedDown = true;
            }

            if(!switchedStartWarn && switchStartWarn)
            {
                switchedStartWarn = true;
            }

            if(!switchedEndWarn && switchEndWarn)
            {
                switchedEndWarn = true;
            }

            intensity = intensity1;
            if(switchUp) intensity = intensity2;
            if(switchedDown) intensity = intensity1;

            float flashRate = flashRate1;
            if(switchedStartWarn && !switchedEndWarn)
                flashRate = flashRate2;

            int curPeriod = int(progress / flashRate);
            if(curPeriod % 2 == 0)
            {
                if(!flashOn)
                {
                    int tactorTime = (int)(flashRate * totalDuration * 100.0f);
                    tactors->setGain((int)intensity);
                    if(tactorTime > 0)
                        tactors->fireSector(sector - 1, tactorTime);
                    //std::cout << "[" << totalDuration - duration << "] gain: " << intensity << " at sector " << sector - 1 << std::endl;
                }
                flashOn = true;
            }
            else
            {
                flashOn = false;
            }

            ////if(flashOn)
            ////    intensity = 0;
            ////else
            ////    intensity = 1;

            //std::cout << "[" << totalDuration - duration << "] intensity = " << intensity << std::endl;
        }

        virtual void onRender(irr::video::IVideoDriver * driver, SaraMatching::Area& a)
        {
            //a.Render(driver, irr::video::SColor(int(intensity*255), 0, 0, 0));
        }

        virtual bool getIsCue() const {return true;}

    private:

        bool flashOn;
        float flashRate1;
        float flashRate2;
        float intensity;
        float intensity1;
        float intensity2;
        float switchUpTime;
        float switchDownTime;
        float switchStartWarnTime;
        float switchEndWarnTime;

        // for output
        bool switchedUp;
        bool switchedDown;
        bool switchedStartWarn;
        bool switchedEndWarn;
    };
}

#endif /* SARA_SIGNALS_H */