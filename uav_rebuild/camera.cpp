#include "area.h"
#include "camera.h"
#include "cam_window.h"
#include "exceptions.h"
#include "globals.h"
#include "network.h"
#include "output.h"
#include "uav_tactors.h"

#include <iostream>

using std::cout; 
using std::endl;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//extern UAVTactors tactors; // old version of tactors
extern Sim::Tactors3x3 * tactors; // new version of tactors, is better
extern bool USE_TACTOR_CUES;
extern bool USE_LIGHT_CUES;

    irr::f32 DOWN_ANGLE = 45.f;
    irr::f32 CAM_FAIL_ANGLE = 90.f;
    bool USE_CAMERA_FAIL = true;
    bool CAM_FAIL_AS_ANGLE = false;
    bool USE_FULL_CAM_SHADING = false;
    bool USE_VIEW_CAM_SHADING = false;
    bool SHOW_PROGRESS_BAR = false;

    // just global
    const irr::s32 OUTLINE_WIDTH = 8;
    const irr::s32 OUTLINE_HEIGHT = 7;
    bool ZOOM_NEAR_TARGET = false;

    // sara simulation overrides
    bool USE_SARA_SHADING = true;

    // static variables
    irr::s32 UAVCamera::ref_count = 0;
    GUIImage * UAVCamera::cam_box = 0;
    GUIImage * UAVCamera::cam_static1 = 0;
    GUIImage * UAVCamera::cam_static2 = 0;
    GUIImage * UAVCamera::cam_static3 = 0;
    GUIImage * UAVCamera::cam_static4 = 0;
    GUIImage * UAVCamera::cam_static5 = 0;
    GUIImage * UAVCamera::outline = 0;
    GUIImage * UAVCamera::outline_br = 0;
    GUIImage * UAVCamera::button_check = 0;
    GUIImage * UAVCamera::button_check_down = 0;
    GUIImage * UAVCamera::button_x = 0;
    GUIImage * UAVCamera::button_x_down = 0;
    GUIImage * UAVCamera::button_unsure = 0;
    GUIImage * UAVCamera::button_unsure_down = 0;
    GUIImage * UAVCamera::button_light = 0;
    GUIImage * UAVCamera::button_light_off = 0;
    GUIImage * UAVCamera::button_target = 0;
    GUIImage * UAVCamera::button_target_down = 0;

UAVCamera::UAVCamera(position2di pos_, CamWindow * win_)
    : pos(pos_), win(win_), uav(0), need_update(true), zooming_out(false),
    zooming_in(false), zooming(false), button_clicked(false),
    auto_light(false), buttons_on(false), staticOn(true),
    unmarked_in_range(false)
{
    if (USE_LIGHT_CUES)
        light_level = LOW;
    else
        light_level = HIGH;

    ref_count++;
}

UAVCamera::~UAVCamera()
{
    ref_count--;
    if(ref_count <= 0) {
        // delete buttons
        if(indicator)       delete indicator;
        if(checkTarget)     delete checkTarget;
        if(btnPositive)     delete btnPositive;
        if(btnNegative)     delete btnNegative;
        // if(light)           delete light;

        // delete images
        if(cam_box)             delete cam_box;
        if(button_check)        delete button_check;
        if(button_x)            delete button_x;
        if(button_light)        delete button_light;
        if(button_light_off)    delete button_light_off;
        if(button_unsure)       delete button_unsure;
        if(button_check_down)   delete button_check_down;
        if(button_x_down)       delete button_x_down;
        if(button_unsure_down)  delete button_unsure_down;
        if(button_target)       delete button_target;
        if(button_target_down)  delete button_target_down;

        // delete font
        if(large_text)      delete large_text;
    }
}

void UAVCamera::load_images(IrrlichtDevice * device)
{
    IVideoDriver * driver = device->getVideoDriver();
    IGUIEnvironment * guienv = device->getGUIEnvironment();
    IGUIElement * guiElmRoot = guienv->getRootGUIElement();

    device->getFileSystem()->addZipFileArchive(UAV_CONS_ZIP.c_str());
    //device->getFileSystem()->addZipFileArchive(CAM_ZIP.c_str());

    cam_box = new GUIImage(rect<s32>(0,0,512,256), device, guiElmRoot);
    cam_box->setTexture(driver->getTexture("cam_box.png"));

    if(win->windowWidth() == 2560) {
        // RESCHANGE
        outline = new GUIImage(rect<s32>(0,0,1024,1024), device, guiElmRoot);
        outline->setTexture(driver->getTexture("cam_outline_2560.png"));
        outline_br = new GUIImage(rect<s32>(0,0,1024,1024), device, guiElmRoot);
        outline_br->setTexture(driver->getTexture("cam_outline_2560_br.png"));

        rect<s32> r(
            0,
            0,
            (CAM_SIZE_X - s32((f32)OUTLINE_WIDTH * 1.3f) - BUTTON_SIZE_Y - 4),
            CAM_SIZE_Y-OUTLINE_HEIGHT);

        cam_static1 = new GUIImage(r, device, guiElmRoot);
        cam_static1->setTexture(driver->getTexture("static1.png"));

        cam_static2 = new GUIImage(r, device, guiElmRoot);
        cam_static2->setTexture(driver->getTexture("static2.png"));

        cam_static3 = new GUIImage(r, device, guiElmRoot);
        cam_static3->setTexture(driver->getTexture("static3.png"));

        cam_static4 = new GUIImage(r, device, guiElmRoot);
        cam_static4->setTexture(driver->getTexture("static4.png"));

        cam_static5 = new GUIImage(r, device, guiElmRoot);
        cam_static5->setTexture(driver->getTexture("static5.png"));

        SaraMatching::SaraSignal::setStaticImages(
            cam_static1);

        //outline = new GUIImage(rect<s32>(0,0,854,533), device, guiElmRoot);
        //outline->setTexture(driver->getTexture("cam_outline_2560_small.png"));
    }
    else {
        outline = new GUIImage(rect<s32>(0,0,512,256), device, guiElmRoot);
        outline->setTexture(driver->getTexture("cam_outline.png"));

        rect<s32> r(
            0,
            0,
            (CAM_SIZE_X - s32((f32)OUTLINE_WIDTH * 1.3f) - BUTTON_SIZE_Y - 4),
            CAM_SIZE_Y-OUTLINE_HEIGHT);

        cam_static1 = new GUIImage(r, device, guiElmRoot);
        cam_static1->setTexture(driver->getTexture("static1.png"));

        cam_static2 = new GUIImage(r, device, guiElmRoot);
        cam_static2->setTexture(driver->getTexture("static2.png"));

        cam_static3 = new GUIImage(r, device, guiElmRoot);
        cam_static3->setTexture(driver->getTexture("static3.png"));

        cam_static4 = new GUIImage(r, device, guiElmRoot);
        cam_static4->setTexture(driver->getTexture("static4.png"));

        cam_static5 = new GUIImage(r, device, guiElmRoot);
        cam_static5->setTexture(driver->getTexture("static5.png"));

        // SaraMatching::SaraSignal::setStaticImages(
        //     cam_static1);
    }

    button_check = new GUIImage(rect<s32>(0,0,64,64), device, guiElmRoot);
    button_check->setTexture(driver->getTexture("button_check.png"));
    button_check_down = new GUIImage(rect<s32>(0,0,64,64), device, guiElmRoot);
    button_check_down->setTexture(driver->getTexture("button_check_down.png"));
    
    button_x = new GUIImage(rect<s32>(0,0,64,64), device, guiElmRoot);
    button_x->setTexture(driver->getTexture("button_x.png"));
    button_x_down = new GUIImage(rect<s32>(0,0,64,64), device, guiElmRoot);
    button_x_down->setTexture(driver->getTexture("button_x_down.png"));

    button_unsure = new GUIImage(rect<s32>(0,0,64,64), device, guiElmRoot);
    button_unsure->setTexture(driver->getTexture("button_unsure.png"));
    button_unsure_down = new GUIImage(rect<s32>(0,0,64,64), device, guiElmRoot);
    button_unsure_down->setTexture(driver->getTexture("button_unsure_down.png"));

    button_light = new GUIImage(rect<s32>(0,0,64,64), device, guiElmRoot);
    button_light->setTexture(driver->getTexture("button_light.png"));
    button_light_off = new GUIImage(rect<s32>(0,0,64,64), device, guiElmRoot);
    button_light_off->setTexture(driver->getTexture("button_light_off.png"));

    button_target = new GUIImage(rect<s32>(0,0,64,64), device, guiElmRoot);
    button_target->setTexture(driver->getTexture("button_target.png"));
    button_target_down = new GUIImage(rect<s32>(0,0,64,64), device, guiElmRoot);
    button_target_down->setTexture(driver->getTexture("button_target_down.png"));
}

void UAVCamera::set_id(IrrlichtDevice * device, s32 id_)
{
    IVideoDriver * driver = device->getVideoDriver();
    IGUIEnvironment * guienv = device->getGUIEnvironment();

    id = id_;

    // get the font image files
    device->getFileSystem()->addZipFileArchive(FONTS_ZIP.c_str());
    IGUIFont * font1 = guienv->getFont(FONT_LARGE.c_str());
    stringc err = "Failed to load font";
    if (!font1) 
        throw Error(err.c_str());

    // bigger text
    stringw sid;
    sid += id_;
    large_text = guienv->addStaticText(sid.c_str(),rect<s32>(0,0,0,0),false, false);
    large_text->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    large_text->setOverrideFont(font1);

    // set the position of the text
    large_text->setRelativePosition(
        rect2di(
            pos.X + CAM_SIZE_X - OUTLINE_WIDTH - BUTTON_SIZE_X,
            pos.Y + CAM_SIZE_Y - OUTLINE_WIDTH - BUTTON_SIZE_Y,
            pos.X + CAM_SIZE_X - OUTLINE_WIDTH,
            pos.Y + CAM_SIZE_Y - OUTLINE_WIDTH));

    // set the color if a UAV has been assigned
    //if(uav)
    //  large_text->setOverrideColor(uav->getColor());
    large_text->setOverrideColor(COLOR_BLACK);
}

void UAVCamera::load_buttons()
{
    indicator = new UAVButton(
        rect2di(
            (pos.X + CAM_SIZE_X - OUTLINE_WIDTH - BUTTON_SIZE_X),
            pos.Y + OUTLINE_HEIGHT + 3,
            pos.X + CAM_SIZE_X - OUTLINE_WIDTH,
            (pos.Y + OUTLINE_HEIGHT + BUTTON_SIZE_Y) + 3),
        button_light,
        button_light,
        button_light_off);

    checkTarget = new UAVButton(
        rect2di(
            pos.X + CAM_SIZE_X - OUTLINE_WIDTH - BUTTON_SIZE_X,
            pos.Y + CAM_SIZE_Y - OUTLINE_HEIGHT - BUTTON_SIZE_Y - 3,
            pos.X + CAM_SIZE_X - OUTLINE_WIDTH,
            (pos.Y + CAM_SIZE_Y - OUTLINE_HEIGHT - BUTTON_SIZE_Y - 3) + BUTTON_SIZE_Y),
        button_target,
        button_target,
        button_target_down);

    btnPositive = new UAVButton(
        rect2di(
            pos.X + CAM_SIZE_X - OUTLINE_WIDTH - BUTTON_SIZE_X,
            pos.Y + OUTLINE_HEIGHT + BUTTON_SIZE_Y + 2,
            pos.X + CAM_SIZE_X - OUTLINE_WIDTH,
            pos.Y + OUTLINE_HEIGHT + BUTTON_SIZE_Y * 2 + 2),
        button_target,
        button_target,
        button_target_down);

    btnNegative = new UAVButton(
        rect2di(
            pos.X + CAM_SIZE_X - OUTLINE_WIDTH - BUTTON_SIZE_X,
            pos.Y + OUTLINE_HEIGHT + BUTTON_SIZE_Y * 2 + 1,
            pos.X + CAM_SIZE_X - OUTLINE_WIDTH,
            pos.Y + OUTLINE_HEIGHT + BUTTON_SIZE_Y * 3 + 1),
        button_x,
        button_x,
        button_x_down);

    // light = new UAVButton(
    //     rect2di(
    //         pos.X + CAM_SIZE_X - OUTLINE_WIDTH - BUTTON_SIZE_X,
    //         pos.Y + OUTLINE_HEIGHT + BUTTON_SIZE_Y * 3,
    //         pos.X + CAM_SIZE_X - OUTLINE_WIDTH,
    //         pos.Y + OUTLINE_HEIGHT + BUTTON_SIZE_Y * 4),
    //     button_light);
}

void UAVCamera::set_uav(UAVObject * uav_, IrrlichtDevice * device)
{
    uav = uav_;
    uav->setCamera(this);

    // build the camera
    ICameraSceneNode * cam = device->getSceneManager()->getActiveCamera();
    camera = device->getSceneManager()->addCameraSceneNode();
    camera->setPosition(vector3df(0.0f,0.0f,0.0f));
    camera->bindTargetAndRotation(true);

    // store viewport settings
    viewport = rect2di(
        pos.X + OUTLINE_WIDTH / 2,
        pos.Y + OUTLINE_WIDTH / 2,
        pos.X + (CAM_SIZE_X - s32((f32)OUTLINE_WIDTH * 1.3f) - BUTTON_SIZE_Y),
        pos.Y + (CAM_SIZE_Y - OUTLINE_WIDTH / 2));

    if(SHOW_PROGRESS_BAR) {
        viewport.LowerRightCorner.Y -= 5;
    }
}

void UAVCamera::draw_overlay(IrrlichtDevice * device) {
    if(uav) {
        if(win->windowWidth() == 2560) {
            // RESCHANGE
            if(id == 6 || id == 7 || id == 8 || id == 9)
            {
                outline_br->setColor(uav->getColor());
                outline_br->setPosition(position2di(
                    pos.X - (1024 - 854),
                    pos.Y - (1024 - 533)));
                outline_br->draw();
            }
            else
            {
                outline->setColor(uav->getColor());
                outline->setPosition(pos);
                outline->draw();
            }
            // RESCHANGE
        }
        else
        {
            // outline
            outline->setColor(uav->getColor());
            outline->setPosition(pos);
            outline->draw();
        }
    }

    indicator->draw();
    checkTarget->draw();
    btnPositive->draw();
    btnNegative->draw();
    // light->draw();
}

void UAVCamera::draw_view(IrrlichtDevice * device, CityScene * city)
{
    IVideoDriver * driver = device->getVideoDriver();

    // draw the number in the bottom-right corner
    // let's actually not draw that right now, since I added a button there instead
    //if(uav)
    //  large_text->draw();

    // progress bar option
    if(SHOW_PROGRESS_BAR && uav && camera)
    {
        // draw the UAV position on the progress bar (unless the UAV is at base or has gps failure)
        if(!uav->atBase() && !uav->hasGPSFail())
        {
            // progress bar
            rect<s32> bpos(
                viewport.UpperLeftCorner.X,
                viewport.LowerRightCorner.Y,
                viewport.LowerRightCorner.X,
                viewport.LowerRightCorner.Y + 8);
            f32 pdist = f32((uav->getTarget() - start_pos).getLength());
            f32 uav_mark = pdist - s32((uav->getTarget() - uav->getPosition()).getLength());
            f32 draw_dist = f32(bpos.LowerRightCorner.X - bpos.UpperLeftCorner.X);
            s32 mdist = s32(draw_dist * (uav_mark / pdist));
            // set the mark position
            rect<s32> mark(
                viewport.UpperLeftCorner.X + mdist - 5,
                bpos.UpperLeftCorner.Y,
                viewport.UpperLeftCorner.X + mdist,
                bpos.LowerRightCorner.Y);
            // draw the mark
            if(mdist > 5 && mdist < draw_dist)
                GUIImage::draw2DRectangle(device->getVideoDriver(), mark, uav->getColor());
        }
    }

    // regular case: UAV is flying normally or a cameral fail while using FAIL_AS_ANGLE has occured
    if(uav && camera && !(USE_CAMERA_FAIL && uav->hasCamFail()) || CAM_FAIL_AS_ANGLE)
    {
        // get the direction the UAV is facing
        vec3d forward = uav->getForward();

        // rotate the view if there's a cam fail (simulate looking through backup camera)
        if(uav->hasCamFail() && USE_CAMERA_FAIL)
        {
            vec3d base_pos = uav->getPosition();
            f32 dist = forward.getLength();
            f32 turn = dist * sin(CAM_FAIL_ANGLE);
            forward.X -= turn;
        }

        // the camera will need to look DOWN_ANGLE degrees down from the UAV
        vec3d base_pos = uav->getPosition();
        f32 dist = forward.getLength();
        f32 down = dist * tan(DOWN_ANGLE);
        forward.Y -= down;

        // zoom some (wow this is goofy)
        if(ZOOM_NEAR_TARGET) {
            if(!uav->goingToBase() && uav->getPosition().getDistanceFrom(uav->getTarget()) < 90.f) {
                if(!zooming) {
                    zoom = 0.f;
                    zooming_in = zooming = true;
                }

                if(zooming_in) {
                    zoom = (90.f - uav->getPosition().getDistanceFrom(uav->getTarget()));
                    if(zoom < 0)    zoom *= -1;
                    if(zoom > 25)   zooming_in = false;
                }
                base_pos += forward * zoom;
            }
            else if(zooming) {
                if(zooming_out) {
                    zoom -= .01f;
                    if(zoom <= 0) {
                        zoom = 0.f;
                        zooming_out = zooming_in = zooming = false;
                    }
                }
                else
                    zooming_out = true;

                base_pos += forward * zoom;
            }
        }

        // align the camera
        camera->setPosition(base_pos);
        camera->setTarget(base_pos + forward);
        city->set_camera(camera->getPosition());

        // set viewport and camera
        device->getSceneManager()->setActiveCamera(camera);
        device->getVideoDriver()->setViewPort(viewport);

        // draw everything
        city->prerender();
        device->getSceneManager()->drawAll();
        city->render();

        // reset viewport and camera
        device->getVideoDriver()->setViewPort(rect<s32>(0,0,win->windowWidth(), win->windowHeight()));
        // RESCHANGE
        //device->getVideoDriver()->setViewPort(rect<s32>(0,0,RES_X,RES_Y));
        device->getSceneManager()->setActiveCamera(0);
    }

    // camera fail while using blue screen
    else if(USE_CAMERA_FAIL && uav && uav->hasCamFail()) {
        GUIImage::draw2DRectangle(driver, viewport, SColor(255,0,0,155));
        large_text->setRelativePosition(rect<s32>(
            viewport.LowerRightCorner.X - 130,
            viewport.UpperLeftCorner.Y,
            viewport.LowerRightCorner.X,
            viewport.UpperLeftCorner.Y + 30));
        stringw txt = "video failure";
        large_text->setText(txt.c_str());
        large_text->setOverrideColor(COLOR_WHITE);
        large_text->draw();
    }

    if(uav && camera) {
        // maybe we don't want to shade this area (if there is a cue that overrides it)
        bool killShading = false;
        bool useStatic = false;
        if(USE_SARA_SHADING)
        {
            std::list<SaraMatching::SaraSignal *>::const_iterator it;
            for(it = signals.begin(); it != signals.end(); it++)
            {
                killShading = killShading || (*it)->getKillShading();
                useStatic = useStatic || (*it)->getStatic();
            }
        }

        // light level
        if(USE_VIEW_CAM_SHADING && !killShading)
        {
            //GUIImage::draw2DRectangle(device->getVideoDriver(), viewport,
            //  SColor(uav->getLight(),0,0,0));
            GUIImage::draw2DRectangle(device->getVideoDriver(), viewport,
                SColor(getLight(),0,0,0));
        }

        if(useStatic)
        {
            GUIImage * simg = 0;
            int s = curStaticImage;
            if(s == 1)  simg = cam_static1;
            if(s == 2)  simg = cam_static2;
            if(s == 3)  simg = cam_static3;
            if(s == 4)  simg = cam_static4;
            else        simg = cam_static5;

            irr::core::position2di p(pos.X + OUTLINE_WIDTH/2, pos.Y + OUTLINE_HEIGHT/2);
            simg->setPosition(p);
            simg->draw();
        }
    }
}
void UAVCamera::draw_background(IrrlichtDevice * device)
{
    // draw background
    //cam_box->setPosition(pos);
    //cam_box->draw();
    SColor color(255,65,65,65);
    if(uav)
        color = uav->getColor();

    rect<s32> dest(
        pos.X + 3,
        pos.Y + 3,
        pos.X + win->windowWidth() / 3 - 3,
        pos.Y + win->windowHeight() / 3 - 3);
    GUIImage::draw2DRectangle(device->getVideoDriver(), dest, color);

    // progress bar
    if(SHOW_PROGRESS_BAR && uav)
    {
        // progress bar
        rect<s32> bpos(
            viewport.UpperLeftCorner.X,
            viewport.LowerRightCorner.Y - 2,
            viewport.LowerRightCorner.X,
            viewport.LowerRightCorner.Y + 5);
        GUIImage::draw2DRectangle(device->getVideoDriver(), bpos, COLOR_BLACK);

        // only update the marks' positions if the UAV is not en route to its base
        // and an update is required
        if(need_update)
        {
            // keep track of the progress bar reference point
            start_pos = uav->getPosition();
            need_update = false;
        }

        if(!(uav->goingToBase() || uav->atBase()))
        {
            f32 dist = f32((uav->getTarget() - start_pos).getLength());
            f32 first_mark = dist - s32(MEDIUM_LIGHT_DIST);
            f32 second_mark = dist - s32(HIGH_LIGHT_DIST);

            // mark distances
            f32 draw_dist = f32(bpos.LowerRightCorner.X - bpos.UpperLeftCorner.X);
            s32 fm_dist = s32(draw_dist * (first_mark / dist));
            s32 sm_dist = s32(draw_dist * (second_mark / dist));

            // first mark
            rect<s32> mark(
                viewport.UpperLeftCorner.X + fm_dist - 5,
                bpos.UpperLeftCorner.Y,
                viewport.UpperLeftCorner.X + fm_dist,
                bpos.LowerRightCorner.Y);
            // draw the first mark
            if(dist > MEDIUM_LIGHT_DIST)
                GUIImage::draw2DRectangle(device->getVideoDriver(), mark, COLOR_WHITE);

            // second mark
            mark.UpperLeftCorner.X = viewport.UpperLeftCorner.X + sm_dist - 5;
            mark.LowerRightCorner.X = viewport.UpperLeftCorner.X + sm_dist;
            // draw the second mark
            if(dist > HIGH_LIGHT_DIST)
                GUIImage::draw2DRectangle(device->getVideoDriver(), mark, COLOR_WHITE);
        }
    }
}

bool UAVCamera::button_click(position2di cursor)
{
    bool render_needed = false;

    // FOR DEBUGGING
    //if(this->get_id() == 1)
    //{
    //  cout << "- buttons_on: " << buttons_on << endl;
    //  cout << "- mouse: " << cursor.X << ", " << cursor.Y << endl;
    //  rect2di a = check->get_area();
    //  cout << "- button: ("
    //      << a.UpperLeftCorner.X << ", "
    //      << a.UpperLeftCorner.Y << ") - ("
    //      << a.LowerRightCorner.X << ", "
    //      << a.LowerRightCorner.Y << ")"
    //      << endl;
    //  cout << endl;
    //}

    if(uav) {
        // light button
        // WE AREN'T USING THIS RIGHT NOW
        // if(light->is_mouse_over(cursor))
        // {
        //  stringw output = uav->getName();
        //  output += " - light toggle button clicked (but we're ignoring it)";
        //  Output::Instance().WriteTick();
        //  Output::Instance().WriteLine(output);

        //  //if(USE_NETWORK)
        //  //{
        //  //  //cout << "sending light thingy" << endl;
        //  //  //if(uav->getLightOverride())
        //  //  if(auto_light)
        //  //      Network::sendMessageCamButton(5, id);
        //  //  else
        //  //      Network::sendMessageCamButton(6, id);
        //  //}

        //  //toggle_light();
        //  render_needed = true;
        // }

        int buttonClicked = 0;

        // top confirm button
        // if(check->is_mouse_over(cursor) && buttons_on/* && uav->activeButtons()*/)
        // {
        //     stringw output = uav->getName();
        //     output += " - confirm button clicked";
        //     Output::Instance().WriteTick();
        //     Output::Instance().WriteLine(output);
            
        //     //cout << "clicked on confirm" << endl;
        //     if(USE_NETWORK)
        //         Network::sendMessageCamButton(2, id);
            
        //     buttons_on = false;

        //     buttonClicked = 2;
        //     check->click(win->device());
        //     uav->setConfirmed();
        //     force_render();
        // }

        // bottom confirm button
        if(checkTarget->is_mouse_over(cursor))
        {
            stringw output = uav->getName();
            output += " - confirm button clicked";
            Output::Instance().WriteTick();
            Output::Instance().WriteLine(output);
            
            cout << "clicked on bottom confirm" << endl;
            if(USE_NETWORK)
                Network::sendMessageCamButton(11, id);
            
            // buttons_on = false;

            buttonClicked = 11;
            checkTarget->click(win->device());
            if (unmarked_in_range && !unmarked_reacted)
                uav->setConfirmed();
            else if (!unmarked_in_range) {
                // punishment
            }
            force_render();
        }

        // confirm button
        if(btnPositive->is_mouse_over(cursor) && buttons_on/* && uav->activeButtons()*/)
        {
            stringw output = uav->getName();
            output += " - unsure button clicked";
            Output::Instance().WriteTick();
            Output::Instance().WriteLine(output);

            //cout << "clicked on unsure" << endl;
            if(USE_NETWORK)
                Network::sendMessageCamButton(2, id);
            
            buttons_on = false;

            buttonClicked = 2;
            btnPositive->click(win->device());
            uav->setConfirmed();
            force_render();
        }

        // deny button
        if(btnNegative->is_mouse_over(cursor) && buttons_on/* && uav->activeButtons()*/)
        {
            stringw output = uav->getName();
            output += " - clear/deny button clicked";
            Output::Instance().WriteTick();
            Output::Instance().WriteLine(output);

            //cout << "clicked on clear/deny" << endl;
            if(USE_NETWORK)
                Network::sendMessageCamButton(3, id);
            
            buttons_on = false;

            buttonClicked = 3;
            btnNegative->click(win->device());
            uav->setNotThere();
            force_render();
        }

        // tell any signals about the response
        // if(buttonClicked != 0)
        // {
        //     std::list<SaraMatching::SaraSignal *>::iterator it;
        //     for(it = signals.begin(); it != signals.end(); it++)
        //         (*it)->onResponse(buttonClicked);
        // }
    }
    else {
        cout << "ERROR: this UAV is not available" << endl;
    }

    return render_needed;
}

void UAVCamera::force_render()
{
    need_update = true;
    win->force_render();
}

bool UAVCamera::need_render()
{
    // check to make sure none of the buttons have been clicked and now
    // have reached the "clicked down" time limit
    // TODO: do something with checkBottom
    bool redraw =
        indicator->is_in_click(win->device()) ||
        checkTarget->is_in_click(win->device()) ||
        btnPositive->is_in_click(win->device()) ||
        btnNegative->is_in_click(win->device());
    return redraw && need_update;
}

void UAVCamera::toggle_light() {
    //bool auto_light = !uav->getLightOverride();
    auto_light = !auto_light;
    //cout << "auto_light set to " << auto_light << endl;

    stringw output = uav->getName();
    output += " - auto-light set to ";
    if(auto_light)  output += "off";
    else            output += "on";
    Output::Instance().WriteTick();
    Output::Instance().WriteLine(output);

    // if(auto_light)  light->set_textures(button_light_off);
    // else            light->set_textures(button_light);
    //uav->setLightOverride(auto_light);
    win->force_render();
}

void UAVCamera::draw_shade(IrrlichtDevice * device) const
{
    rect<s32> rec(
        pos.X,
        pos.Y,
        pos.X + CAM_SIZE_X,
        pos.Y + CAM_SIZE_Y);

    GUIImage::draw2DRectangle(device->getVideoDriver(), rec,
        SColor(getLight(),0,0,0));
}

bool UAVCamera::draw_sara_shade(IrrlichtDevice * device) const
{
    bool retValue = false;

    //uav->renderSaraSignals(device->getVideoDriver(), pos.X, pos.Y, CAM_SIZE_X, CAM_SIZE_Y);
    std::list<SaraMatching::SaraSignal *>::const_iterator it;
    for(it = signals.begin(); it != signals.end(); it++)
    {
        // careful with order of operations on this line: if retValue || render(...), it won't render when retValue is true
        (*it)->render(device->getVideoDriver(), get_id(), pos.X, pos.Y, CAM_SIZE_X, CAM_SIZE_Y);
        retValue = retValue || (*it)->getKillShading();
    }

    return retValue;
}

void UAVCamera::cam_message(int message) {
    cout << "CAMERA MESSAGE RECEIVED ............ [" << message << "]" << endl;
    int set_light_level = 0;

    switch (message) {
        case 0:
            // turn on buttons when close to target
            buttons_on = true;
            set_indicator_status(true);
			// indicator->set_highlighted(true);
            set_light_level = 3;
            break;
        case 1:
            // turn off buttons after getting away
            buttons_on = false;
            set_indicator_status(false);
			// indicator->set_highlighted(false);
            set_light_level = 1;
            break;
        case 2:
            // indicate an unmarked target
            unmarked_in_range = true;
            unmarked_reacted = false;
            break;
        case 3:
            // indicate getting away from an unmarked target
            unmarked_in_range = false;
            break;
        case 7:
            if(USE_LIGHT_CUES) {
                // set brightness to dim on camera
                set_light_level = 1;
            }
            break;
        case 8:
            if(USE_LIGHT_CUES) {
                // set brightness to medium on camera
                set_light_level = 2;
            }
            break;
        case 9:
            if(USE_LIGHT_CUES) {
                // set brightness to full on camera
                set_light_level = 3;
            }
            break;
        case 10:
            uav->setCamFail(true);
            break;
		case 11:
			if (light_level == LOW)
				set_light_level = 3;
			else 
				set_light_level = 1;
			break;
        default:
            cout << "unrecognized command" << endl;
            break;
    }

    if(set_light_level > 0) {
        if(set_light_level == 1) 
            light_level = LOW;
        else if(set_light_level == 2) 
            light_level = MEDIUM;
        else if(set_light_level == 3) 
            light_level = HIGH;
    }

	force_render();
}

void UAVCamera::sara_cam_message(int type, SimSaraCamPacket * p)
{
    SaraMatching::SaraSignal * signal = 0;

    if(type == 0)
        signal = new SaraMatching::SaraSignalFlickerVisual(p->value1, p->value2, p->value3, p->value4, p->value5);
    else if(type == 1)
        signal = new SaraMatching::SaraSignalMudsplashVisual(p->value1, p->value2, p->value3, p->value4, p->value5);
    else if(type == 2)
        signal = new SaraMatching::SaraSignalFlickerTactor(p->value1, p->value2, p->value3, p->value4, p->value5);
    else if(type == 3)
        signal = new SaraMatching::SaraSignalMudsplashTactor(p->value1, p->value2, p->value3, p->value4, p->value5);
    else if(type == 4)
        signal = new SaraMatching::SaraSignalTactorCue(p->value1, p->value2, p->value3, p->value4, p->value5, p->value6);
    else if(type == 5)
        signal = new SaraMatching::SaraSignalVisualCue(p->value1, p->value2, p->value3, p->value4, p->value5);
    else if(type == 6)
        signal = new SaraMatching::SaraSignalVisualGradualCue(p->value1, p->value2, p->value3, p->value4, p->value5);
    else if(type == 7)
        signal = new SaraMatching::SaraSignalTactorGradualCue(p->value1, p->value2, p->value3, p->value4, p->value5, p->value6);
    else if(type == 8)
        signal = new SaraMatching::SaraSignalVisualCueV1(p->correctResponse, p->value1, p->value2, p->value3, p->value4, p->value5, p->value6, p->value7);
    else if(type == 9)
        signal = new SaraMatching::SaraSignalVisualCueV2(p->correctResponse, p->value1, p->value2, p->value3, p->value4, p->value5, p->value6);
    else if(type == 10)
        signal = new SaraMatching::SaraSignalTactorCueV1(p->correctResponse, p->value1, p->value2, p->value3, p->value4, p->value5, p->value6, p->value7, p->value8);
    else if(type == 11)
        signal = new SaraMatching::SaraSignalTactorCueV2(p->correctResponse, p->value1, p->value2, p->value3, p->value4, p->value5, p->value6, p->value7, p->value8);
    else if(type == 12)
        signal = new SaraMatching::SaraSignalTactorCueV3(p->correctResponse, p->value1, p->value2, p->value3, p->value4, p->value5, p->value6, p->value7, p->value8, p->value9);

    if(signal)
    {
        int id = get_id();

        signal->setSector(get_id());
        signal->update(0);
        signals.push_back(signal);

        std::cout << "added signal to sector" << get_id() << std::endl;
    }
    else
    {
        std::cout << "unknown signal" << std::endl;
    }
}

void UAVCamera::update_signals(float time)
{
    std::list<std::list<SaraMatching::SaraSignal *>::iterator> toDelete;

    std::list<SaraMatching::SaraSignal *>::iterator it;
    for(it = signals.begin(); it != signals.end(); it++)
    {
        (*it)->update(time);
        if((*it)->getDone())
            toDelete.push_back(it);
    }

    std::list<std::list<SaraMatching::SaraSignal *>::iterator>::iterator del;
    for(del = toDelete.begin(); del != toDelete.end(); del++)
    {
        // if this is a cue, tell the network that and turn off the buttons
        if((**del)->getIsCue())
        {
            if(USE_NETWORK)
                Network::sendMessageCamButton(12, id); // cue completed
            //buttons_on = false;
        }
        signals.erase(*del);
    }

    // make the static look nice by picking a random static image that doesn't
    // match the last static image used
    // (this will be updated all the time, but mostly won't be rendered)
    timeSinceLastStatic += time;
    if(timeSinceLastStatic > 0.1)
    {
        int lastImage = curStaticImage;
        int num1 = 3;
        int num2 = 5; // we have a total of 5 static images

        //while(curStaticImage == lastImage)
        //  curStaticImage = rand() % (num2 - num1 + 1) + num1;
        curStaticImage++;
        if(curStaticImage > 5)
            curStaticImage = 1;
    
        timeSinceLastStatic = 0;
    }
}

bool UAVCamera::set_indicator_status(bool status) 
{
	if (indicator != nullptr) {
		indicator->set_highlighted(status);
		return true;
	}
	else {
		return false;
	}
}