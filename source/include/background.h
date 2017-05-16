#ifndef BACKGROUND_H
#define BACKGROUND_H

#include<allegro.h>

#include"main.h"
#include"sprites.h"



#define BACKGROUND_TUNNEL1                                1

#define BACKGROUND_IMG_TUNNEL1                            1
#define BACKGROUND_IMG_TUNNEL1_LIGHT                      2

class BACKGROUND
{
public:
    //What type of background we want?
    int background_type;
    
    //Moving and max speed of the background
    int background_max_speed;
    int background_speed;
    
    //Background bitmaps
    BITMAP *spr_moving_tunnel1;
    BITMAP *spr_moving_tunnel1_light;
    
    
    
    BITMAP *spr_moving_tunnel1_half;
    BITMAP *spr_moving_tunnel1_light_half;
    
    //Boolean that says if there is or isn't a backgroun on run
    bool active;
    
    //This vector indicates what to draw where
    int *wallpaper;
    
    //And the position each one is in
    int *wallpaper_position;
    
    //Number of "images" to fullfill the screen
    int wallpapers_number;
    
    //Size of the gameplay area
    int gameplay_area;
    
    
    
    //The ending flag tells the background to stop renewing backgroung images positions
    bool endingFlag;
    
    
    
    //Constructor
    BACKGROUND();
    
    //Set speed and max speeds
    void setSpeeds(int background_speed, int background_max_speed);
    void setSpeed(int background_speed);
    void setMaxSpeed(int background_max_speed);
    
    //Set the background type
    void setBackground(int background_type, int gameplay_area);
    
    //Set a ending flag value
    void setEndingFlag(bool endingFlag);
    
    //Get speed or max speed
    int getSpeed();
    int getMaxSpeed();
    
    //Logic loop for the background
    void logicLoop();
    
    //Draw the background
    void draw(BITMAP *buffer, int camera_x, int camera_y, int zoom);
    
    //Tranlate the vector one position to the left
    void vectorTranslate();
    
    //Clear previous type of background, whatever it was
    void clearBackground();
    
    //Destructor
    ~BACKGROUND();
};

#endif
