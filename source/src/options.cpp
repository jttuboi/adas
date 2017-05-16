#include "options.h"


void main_options(BITMAP *buffer, int screen_width, int screen_height, int go_down)
{
	OPTIONSss voptions;
	bool user_key_up = false, user_key_down = false;
	bool vuser_key_up = true, vuser_key_down = true;
	int num_button = 3;
	
	
    create_options(voptions,num_button);
    

    bool exit_options = false;
    while (!exit_options)
	{
		if ((key[KEY_UP]) && (vuser_key_up))
		{
			vuser_key_up = false;
			user_key_up = true;
		}
		if (!key[KEY_UP])
		{
			vuser_key_up = true;
		}
	
	
		if ((key[KEY_DOWN]) && (vuser_key_down))
		{
			vuser_key_down = false;
			user_key_down = true;
		}
		if (!key[KEY_DOWN])
		{
			vuser_key_down = true;
		}
	

		if ((key[KEY_ESC]) && (go_down))
		{
			voptions.select_button = 2;
			exit_options = true;
		}
		if (!key[KEY_ESC])
			go_down = true;
		
		if (key[KEY_ENTER])
		{
			exit_options = true;
		}
	
		change_button_options(&voptions, user_key_up, user_key_down);
	
		user_key_up = false;
		user_key_down = false;
	
		draw_options(&voptions, buffer, screen_width, screen_height);
		
		blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);
	}
	

	
	unload_options(&voptions, buffer);
}



void create_options(OPTIONSss &voptions, int num_button)
{
	voptions.num_button = num_button;
	
	voptions.select_button = 0;
}



void change_button_options(OPTIONSss *voptions, bool user_key_up, bool user_key_down)
{
	if ((user_key_up) && (voptions->select_button > 0))
		voptions->select_button--;
	if ((user_key_down) && (voptions->select_button < voptions->num_button - 1))
		voptions->select_button++;	
}



void draw_options(OPTIONSss *voptions, BITMAP *buffer, int screen_width, int screen_height)
{
	//textout_ex(screen, font, "frase", posicao largura, posicao altura, makecol(255, 0, 0), cor de fundo);

	textout_ex(buffer, font, "    Play", 218, 120 + (0*20), makecol(255, 255, 255), 0);
	textout_ex(buffer, font, "    Options", 218, 120 + (1*20), makecol(255, 255, 255), 0);
	textout_ex(buffer, font, "    Exit", 218, 120 + (2*20), makecol(255, 255, 255), 0);

	// Blit arrow
	textout_ex(buffer, font, "-->", 218, 120 + (voptions->select_button*20), makecol(255, 0, 0), 0);
	rest(80);
}



void unload_options(OPTIONSss *voptions, BITMAP *buffer)
{
	clear_bitmap(buffer);
	blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);
}
