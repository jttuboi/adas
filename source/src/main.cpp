#include<allegro.h>

#include"main.h"

#include"gameplay.h"

#include"menu.h"

#include"options.h"

//For the frames per second
volatile int Fps;
volatile int FpsCounter = 0;
volatile int FpsSpeed = 0;


void FpsIncrement(void)
{
    Fps++;
}END_OF_FUNCTION(FpsIncrement)



void FpsSpeedUpdate(void)
{
    FpsSpeed = FpsCounter;
    FpsCounter = 0;
}END_OF_FUNCTION(FpsSpeedUpdate)



//The main function
int main(int argc, char *argv[])
{
    //Initiate allegro
    allegro_init();
    
    //Install what we need
    install_timer();
    install_keyboard();
    
    //Set the gfx
    set_color_depth(COLOR_DEPTH);
    set_gfx_mode(GAME_GFX_MODE, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
    
    //Lock what we need for the timer
    LOCK_VARIABLE(Fps);
    LOCK_VARIABLE(FpsSpeed);
    LOCK_VARIABLE(FpsCounter);
    LOCK_FUNCTION(FpsIncrement);
    LOCK_FUNCTION(FpsSpeedUpdate);
    
    //Some small error checking
    int test = install_int_ex(FpsIncrement, BPS_TO_TIMER(FPS_SPEED));
    if(test < 0)
    {
        allegro_message("Error setting timer");
    }
    
    test = install_int_ex(FpsSpeedUpdate, BPS_TO_TIMER(1));
    if(test < 0)
    {
        allegro_message("Error setting timer");
    }
    
    
    //Create the buffers for triple buffering
    BITMAP *buffer1 = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    BITMAP *buffer2 = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    BITMAP *buffer = buffer1;
    
    
    
    //Only to redraw the screen when needed
    bool redraw = false;
    
    
    
    
    
    //The wagon
    GAMEPLAY gameplay;
    gameplay.setGame(3, 3, 3, 1, GAMEPLAY_TYPE_NORMAL);
    
    
	int button_menu;



    // program main loop
    bool it_is_menu = true;
    bool done = false;
    bool go_down = true;
    while (!done)
    {
		if(it_is_menu)
		{
		    main_menu(button_menu, buffer, SCREEN_WIDTH, SCREEN_HEIGHT, go_down);
		    it_is_menu = false;
		    Fps = 0;
		} else {
		    if(key[KEY_ESC]) it_is_menu = true;
		}
		    
		
		// It's play
		if (button_menu == 0)
		{
            while(!key[KEY_ESC])
            {
                //Logic frame =)
                while(Fps > 0)
                {
                    clear_bitmap(buffer);
					
					if(!it_is_menu) gameplay.logicLoop();
                        
                    //So things can be redrawn
                   redraw = true;
            
                    --Fps;
                    ++FpsCounter;
                }
        
                //Need to redraw?
                if(redraw)
                {
                    //Redraw it
                    textprintf_ex(buffer, font, 10, 10, makecol(255, 255, 255),
                            -1, "Fps: %d", FpsSpeed);
            
            
					
                    if(!it_is_menu) gameplay.draw(buffer);
            
                    BITMAP *aux = (buffer == buffer1 ? buffer2 : buffer1);
            
                    acquire_screen();
            
                    //vsync();
                    blit(aux, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            
                    release_screen();
            
                    buffer = aux;
            
                    redraw=false;
                } else {
                    //No need to redraw. Sleep for a time to allow other programs to run
                    rest(1);
                }
            }
            //clear_bitmap(buffer);
		}
		
		//It's options
		if (button_menu == 1)
		{
			//main_options(buffer, SCREEN_WIDTH, SCREEN_HEIGHT, go_down);
		
		}
		//It's Exit
		if (button_menu == 2)
			done = true;
			
		if (key[KEY_ESC])
			go_down = false;
    }
    
    
    //Bye, buffer
    destroy_bitmap(buffer1);
    destroy_bitmap(buffer2);
    
    return(0);
}END_OF_MAIN()
