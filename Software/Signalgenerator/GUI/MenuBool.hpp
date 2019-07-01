/*
 * MenuBool.hpp
 *
 *  Created on: Jun 9, 2019
 *      Author: jan
 */

#ifndef MENUBOOL_HPP_
#define MENUBOOL_HPP_

#include "menuentry.hpp"

class MenuBool : public MenuEntry {
public:
	MenuBool(const char *name, bool *value, void (*cb)(Widget&));

private:
	void draw(coords_t offset) override;
	void input(GUIEvent_t *ev) override;

	Widget::Type getType() override { return Widget::Type::MenuBool; };

	static constexpr color_t Background = COLOR_BG_DEFAULT;
	static constexpr color_t Foreground = COLOR_FG_DEFAULT;
	static constexpr color_t ColorOn = COLOR(0, 192, 0);
	static constexpr color_t ColorOff = COLOR(238, 0, 0);
	static constexpr uint8_t MaxNameLength = 10;
	static constexpr const font_t *fontName = &Font_Medium;
	static constexpr const font_t *fontValue = &Font_Big;

    void (*callback)(Widget& source);
    bool *value;
    char name[MaxNameLength + 1];
};



#endif /* MENUBOOL_HPP_ */