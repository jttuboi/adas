#ifndef MENU_H
#define MENU_H

#include <allegro.h>


struct MENUss
{
	int num_button;
	
	int select_button;
};

void main_menu(int &select_button, BITMAP *buffer, int screen_width, int screen_height, int go_down);

void create_menu(MENUss &vmenu, int num_button);

void change_button_menu(MENUss *vmenu, bool user_key_up, bool user_key_down);

void draw_menu(MENUss *vmenu, BITMAP *buffer, int screen_width, int screen_height);

void unload_menu(MENUss *vmenu, BITMAP *buffer);

#endif
