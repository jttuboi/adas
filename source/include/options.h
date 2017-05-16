#ifndef OPTIONS_H
#define OPTIONS_H

#include <allegro.h>

#include <cstdio>

struct OPTIONSss
{
	int num_button;
	
	int select_button;	
};

void main_options(BITMAP *buffer, int screen_width, int screen_height, int go_down);

void create_options(OPTIONSss &voptions, int num_button);

void change_button_options(OPTIONSss *voptions, bool user_key_up, bool user_key_down);

void draw_options(OPTIONSss *voptions, BITMAP *buffer, int screen_width, int screen_height);

void unload_options(OPTIONSss *voptions, BITMAP *buffer);

#endif


