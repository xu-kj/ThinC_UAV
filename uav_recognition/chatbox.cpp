#include "chatbox.h"
#include "globals.h"
#include "output.h"

#include <iostream>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

SColor CHAT_INPUT_COLOR(50,255,255,255);

UAVChatbox::UAVChatbox()
    : start_line(0), focused(false)
{}

void UAVChatbox::load_images(IrrlichtDevice * device)
{}

void UAVChatbox::draw(IVideoDriver * driver, IGUIStaticText * font)
{
    // right line
    rect<s32> line(
        CHAT_END_X - 2,
        CHAT_START_Y,
        CHAT_END_X,
        CHAT_END_Y);
    GUIImage::draw2DRectangle(driver, line, COLOR_BLACK);

    // left line
    line = rect<s32>(
        CHAT_START_X,
        CHAT_START_Y,
        CHAT_START_X + 2,
        CHAT_END_Y);
    GUIImage::draw2DRectangle(driver, line, COLOR_BLACK);

    // top line
    line = rect<s32>(
        CHAT_START_X,
        CHAT_START_Y,
        CHAT_END_X,
        CHAT_START_Y + 2);
    GUIImage::draw2DRectangle(driver, line, COLOR_BLACK);

    // find total number of text lines to show
    s32 last_line = start_line + chat_lines();
    if(unsigned(last_line) > text.size())
        last_line = text.size();

    // prepare the text alignment
    font->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);

    // scroll text
    s32 i = 0;
    for(s32 line = start_line; line < last_line; line++)
    {
        rect<s32> text_rect(
            CHAT_START_X + 2 + 4,
            CHAT_START_Y + 2 + (i * CHAT_TEXT_HEIGHT),
            CHAT_END_X - 2 - 4,
            CHAT_START_Y + 2 + CHAT_TEXT_HEIGHT + (i * CHAT_TEXT_HEIGHT));
        font->setOverrideColor(colors[line]);
        font->setText(text[line].c_str());
        font->setRelativePosition(text_rect);
        font->draw();
        font->setOverrideColor(COLOR_WHITE);

        i++;
    }

    // current text to send
    stringw the_text = current_text;
    if(focused)
        the_text += "|";
    
    rect<s32> text_rect(
        CHAT_START_X + 2 + 2,
        CHAT_END_Y - 2 - 2 - CHAT_TEXT_HEIGHT,
        CHAT_END_X - 2 - 2,
        CHAT_END_Y - 2 - 2);
    GUIImage::draw2DRectangle(driver, text_rect, CHAT_INPUT_COLOR);
    font->setText(the_text.c_str());
    font->setRelativePosition(text_rect);
    font->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);
    font->draw();
}

bool UAVChatbox::is_mouse_over(position2di cursor)
{
    if(cursor.X > CHAT_START_X &&
        cursor.X < CHAT_END_X &&
        cursor.Y > CHAT_START_Y &&
        cursor.Y < CHAT_END_Y)
    {
        return true;
    }
    return false;
}

void UAVChatbox::scroll(s32 lines)
{
    // find total number of text lines to show
    s32 size = chat_lines();

    if(text.size() > unsigned(size))
    {
        start_line += lines;
        if(start_line < 0)
            start_line = 0;
        if(unsigned(start_line + size) > text.size())
            start_line = text.size() - size;
    }
}

void UAVChatbox::keystroke(wchar_t key)
{
    if(focused)
    {
        // allow only visible keys
        if(key == 0)
            return;

        switch(key)
        {
        case 8: // backspace
            if(current_text.size() > 0)
                current_text.erase(current_text.size() - 1);
            break;

        case 13: // enter
            Output::Instance().WriteTick();
            Output::Instance().Write("user chat: \"");
            Output::Instance().Write(current_text);
            Output::Instance().WriteLine("\"");

            Output::Instance().WriteTick(OUTPUT_CHAT);
            Output::Instance().Write("user chat: \"", OUTPUT_CHAT);
            Output::Instance().Write(current_text, OUTPUT_CHAT);
            Output::Instance().WriteLine("\"", OUTPUT_CHAT);

            if(current_text.size() > 0)
            {
                add_text(current_text);
                current_text = "";
            }

            Output::Instance().WriteTick();
            Output::Instance().WriteLine("chatbox no longer in focus");

            focused = false;
            break;

        case 9: // no tabbing
            break;

        default:
            if(current_text.size() < 60)
                current_text += key;
            break;
        }
    }
}

void UAVChatbox::focus()
{
    focused = true;
    Output::Instance().WriteTick();
    Output::Instance().WriteLine("chatbox now in focus");
}

void UAVChatbox::add_text(const stringw &line, irr::video::SColor color)
{
    // add code to split too-long lines into multiple lines if needed
    text.push_back(line);
    colors.push_back(color);
    focus_bottom();
}

s32 UAVChatbox::chat_lines() const
{
    s32 size = CHAT_END_Y - CHAT_START_Y - 8;
    size = size / CHAT_TEXT_HEIGHT;
    size--;

    return size;
}

void UAVChatbox::focus_bottom()
{
    start_line = text.size() - chat_lines();
    if(start_line < 0)
        start_line = 0;
}