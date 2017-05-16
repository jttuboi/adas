#include "menu.h"


void main_menu(int &select_button, BITMAP *buffer, int screen_width, int screen_height, int go_down)
{
	MENUss vmenu;
	bool user_key_up = false, user_key_down = false;
	bool vuser_key_up = true, vuser_key_down = true;
	int num_button = 3;
	
	
    create_menu(vmenu,num_button);
    

    bool exit_menu = false;
    while (!exit_menu)
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
			vmenu.select_button = 2;
			exit_menu = true;
		}
		if (!key[KEY_ESC])
			go_down = true;
		
		if (key[KEY_ENTER])
		{
			exit_menu = true;
		}
	
		change_button_menu(&vmenu, user_key_up, user_key_down);
	
		user_key_up = false;
		user_key_down = false;
	
		draw_menu(&vmenu, buffer, screen_width, screen_height);
		
		blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);
	}
	
	select_button = vmenu.select_button;
	
	unload_menu(&vmenu, buffer);
}



void create_menu(MENUss &vmenu, int num_button)
{
	vmenu.num_button = num_button;
	
	vmenu.select_button = 0;
}



void change_button_menu(MENUss *vmenu, bool user_key_up, bool user_key_down)
{
	if ((user_key_up) && (vmenu->select_button > 0))
		vmenu->select_button--;
	if ((user_key_down) && (vmenu->select_button < vmenu->num_button - 1))
		vmenu->select_button++;	
}



void draw_menu(MENUss *vmenu, BITMAP *buffer, int screen_width, int screen_height)
{
	//textout_ex(screen, font, "frase", posicao largura, posicao altura, makecol(255, 0, 0), cor de fundo);

	textout_ex(buffer, font, "    Play", 218, 120 + (0*20), makecol(255, 255, 255), 0);
	textout_ex(buffer, font, "    Options", 218, 120 + (1*20), makecol(255, 255, 255), 0);
	textout_ex(buffer, font, "    Exit", 218, 120 + (2*20), makecol(255, 255, 255), 0);

	// Blit arrow
	textout_ex(buffer, font, "-->", 218, 120 + (vmenu->select_button*20), makecol(255, 0, 0), 0);
	rest(80);
}



void unload_menu(MENUss *vmenu, BITMAP *buffer)
{
	clear_bitmap(buffer);
	blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);
}
