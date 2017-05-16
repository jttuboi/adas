#include"background.h"



//Constructor
BACKGROUND::BACKGROUND()
{
    //Load the bakcground images
    spr_moving_tunnel1 = load_bitmap("media/moving_tunnel1_empty.bmp", NULL);
    spr_moving_tunnel1_light = load_bitmap("media/moving_tunnel1_light.bmp", NULL);
    
    spr_moving_tunnel1_half = bitmap_half(spr_moving_tunnel1);
    spr_moving_tunnel1_light_half = bitmap_half(spr_moving_tunnel1_light);
    
    //No background on the run
    active = false;
}



//Set speed and max speeds
void BACKGROUND::setSpeeds(int background_speed, int background_max_speed)
{
    this->background_speed = background_speed;
    this->background_max_speed = background_max_speed;
}

void BACKGROUND::setSpeed(int background_speed)
{
    this->background_speed = background_speed;
}

void BACKGROUND::setMaxSpeed(int background_max_speed)
{
    this->background_max_speed = background_max_speed;
}



//Set the background type
void BACKGROUND::setBackground(int background_type, int gameplay_area)
{
    //Clear any previou background
    clearBackground();
    
    //Wallpapers number is zero
    wallpapers_number = 0;
    
    //Set the background
    this->background_type = background_type;
    
    //Set the gameplay area
    this->gameplay_area = gameplay_area;
    
    //Set the ending flag as false
    endingFlag = false;
    
    
    
    //Create the background vector
    switch(background_type)
    {
        case BACKGROUND_TUNNEL1:
        {
            //Find the biggest width of the wallpaper images
            int biggest = spr_moving_tunnel1->w;
            if(spr_moving_tunnel1_light->w > biggest) biggest = spr_moving_tunnel1_light->w;
            
            //Increase a bit the gameplay area
            this->gameplay_area += biggest;
            
            
            
            //Calculate the number of positions into the vector
            int size = 0;
            while(size < this->gameplay_area)
            {
                //First, the normal walls
                size += spr_moving_tunnel1->w;
                wallpapers_number++;
                
                if(size >= this->gameplay_area) break;
                
                
                size += spr_moving_tunnel1->w;
                wallpapers_number++;
                
                if(size >= this->gameplay_area) break;
                
                
                size += spr_moving_tunnel1->w;
                wallpapers_number++;
                
                if(size >= this->gameplay_area) break;
                
                
                size += spr_moving_tunnel1->w;
                wallpapers_number++;
                
                if(size >= this->gameplay_area) break;
                
                
                
                //Now, the light wall
                size += spr_moving_tunnel1_light->w;
                wallpapers_number++;
            }
            
            
            
            //We have the vector size, in images. Create the vector
            wallpaper = new int[wallpapers_number];
            wallpaper_position = new int[wallpapers_number];
            
            //Set the wallpapers and wallpapers position
            int frame = 0;//What to draw
            int pos = 0;//The position
            while(frame < wallpapers_number)
            {
                wallpaper[frame] = BACKGROUND_IMG_TUNNEL1;
                wallpaper_position[frame] = pos;
                pos += spr_moving_tunnel1->w;
                ++frame;
                if(frame >= wallpapers_number) break;
                
                wallpaper[frame] = BACKGROUND_IMG_TUNNEL1;
                wallpaper_position[frame] = pos;
                pos += spr_moving_tunnel1->w;
                ++frame;
                
                if(frame >= wallpapers_number) break;
                wallpaper[frame] = BACKGROUND_IMG_TUNNEL1;
                wallpaper_position[frame] = pos;
                pos += spr_moving_tunnel1->w;
                ++frame;
                
                if(frame >= wallpapers_number) break;
                wallpaper[frame] = BACKGROUND_IMG_TUNNEL1;
                wallpaper_position[frame] = pos;
                pos += spr_moving_tunnel1->w;
                ++frame;
                if(frame >= wallpapers_number) break;
                
                wallpaper[frame] = BACKGROUND_IMG_TUNNEL1_LIGHT;
                wallpaper_position[frame] = pos;
                pos += spr_moving_tunnel1_light->w;
                ++frame;
            }
        } break;
    }
    
    
    
    active = true;
}



//Set a ending flag value
void BACKGROUND::setEndingFlag(bool endingFlag)
{
    this->endingFlag = endingFlag;
}



//Get speed or max speed
int BACKGROUND::getSpeed()
{
    return(background_speed);
}

int BACKGROUND::getMaxSpeed()
{
    return(background_max_speed);
}



//Logic loop for the background
void BACKGROUND::logicLoop()
{
    //If not active, do nothing
    if(!active) return;
    
    //Move the background as wanted
    switch(background_type)
    {
        case BACKGROUND_TUNNEL1:
        {
            //Move everything to the left
            for(int i=0; i < wallpapers_number; ++i)
            {
                wallpaper_position[i] -= background_speed;
            }
            
            //If we got out of vector, translade the vector and 
            //readd the removed object to the end of the vector
            if(wallpaper_position[0] < 0)
            {
                int img_type = wallpaper[0];
                vectorTranslate();
                
                //Check the new position
                int pos = wallpaper_position[wallpapers_number-2];
                
                if(wallpaper[wallpapers_number-2] == BACKGROUND_IMG_TUNNEL1)
                    pos += spr_moving_tunnel1->w;
                
                if(wallpaper[wallpapers_number-2] == BACKGROUND_IMG_TUNNEL1_LIGHT)
                    pos += spr_moving_tunnel1_light->w;
                
                wallpaper[wallpapers_number-1] = img_type;
                wallpaper_position[wallpapers_number-1] = pos;
            }
        } break;
    }
}



//Draw the background
void BACKGROUND::draw(BITMAP *buffer, int camera_x, int camera_y, int zoom)
{
    //If nothing to do, then do nothing
    if(!active) return;
    
    
    
    switch(zoom)
    {
        case ZOOM_NONE:
        {
            //Draw what is needes
            for(int i=0; i < wallpapers_number; ++i)
            {
                switch(wallpaper[i])
                {
                    case BACKGROUND_IMG_TUNNEL1:
                    {
                        draw_sprite(buffer, spr_moving_tunnel1, wallpaper_position[i] - camera_x, 50 - camera_y);
                    } break;
                    
                    case BACKGROUND_IMG_TUNNEL1_LIGHT:
                    {
                        draw_sprite(buffer, spr_moving_tunnel1_light, wallpaper_position[i] - camera_x, 50 - camera_y);
                    } break;
                }
            }
        } break;
        
        
        
        case ZOOM_OUT:
        {
            //Draw what is needes
            for(int i=0; i < wallpapers_number; ++i)
            {
                switch(wallpaper[i])
                {
                    case BACKGROUND_IMG_TUNNEL1:
                    {
                        draw_sprite(buffer, spr_moving_tunnel1_half, wallpaper_position[i]/2 - camera_x, 25 - camera_y);
                    } break;
                    
                    case BACKGROUND_IMG_TUNNEL1_LIGHT:
                    {
                        draw_sprite(buffer, spr_moving_tunnel1_light_half, wallpaper_position[i]/2 - camera_x, 25 - camera_y);
                    } break;
                }
            }
        } break;
    }
}



//Tranlate the vector one position to the left
void BACKGROUND::vectorTranslate()
{
    //Nothing loaded, nothing to do
    if(!active) return;
    
    //Ending flag??? Do nothing then
    if(endingFlag) return;
    
    //Translade
    for(int i=0; i < wallpapers_number - 1; ++i)
    {
        wallpaper[i] = wallpaper[i+1];
        wallpaper_position[i] = wallpaper_position[i+1];
    }
}
    



//Clear previous type of background, whatever it was
void BACKGROUND::clearBackground()
{
    //No background, nothing to clear
    if(!active) return;
    
    //Delete the vectors
    delete[] wallpaper;
    delete[] wallpaper_position;
    
    //Default some variables
    wallpapers_number = 0;
}



//Destructor
BACKGROUND::~BACKGROUND()
{
    //Unload the backgorund images
    destroy_bitmap(spr_moving_tunnel1);
    destroy_bitmap(spr_moving_tunnel1_light);
    
    destroy_bitmap(spr_moving_tunnel1_half);
    destroy_bitmap(spr_moving_tunnel1_light_half);
    
    //Clear the background
    clearBackground();
}
