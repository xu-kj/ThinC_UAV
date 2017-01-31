#ifndef CHATBOX_H
#define CHATBOX_H

#include "image.h"

#include <irrlicht.h>
#include <vector>

class UAVChatbox {
public:
	UAVChatbox();

	void load_images(irr::IrrlichtDevice * device);
	void draw(irr::video::IVideoDriver * driver, irr::gui::IGUIStaticText * font);

	bool is_mouse_over(irr::core::position2di cursor);
	void scroll(irr::s32 lines);
	void keystroke(wchar_t key);
	void focus();

	void add_text(const irr::core::stringw &line, irr::video::SColor color = irr::video::SColor(255,255,255,255));

private:
	irr::s32 chat_lines() const;
	void focus_bottom();

	std::vector<irr::core::stringw> text;
	std::vector<irr::video::SColor> colors;
	irr::s32 start_line;

	bool focused;

	irr::core::stringw current_text;
};

#endif /* CHATBOX_H */