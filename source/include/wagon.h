#ifndef WAGON_H
#define WAGON_H

#include<allegro.h>

#include"main.h"
#include"sprites.h"

#define WINDOWS_NUMBER                               4

#define DOOR_CLOSED                                  0
#define DOOR_OPENING                                 1
#define DOOR_OPEN                                    2

#define WAGON_ACTION_NONE                            0
#define WAGON_ACTION_WINDOW                          1
#define WAGON_ACTION_LEFT_DOOR                       2
#define WAGON_ACTION_RIGHT_DOOR                      3
#define WAGON_ACTION_FRONT_DOOR                      4

#define SIDE_DOOR_HITS                               600



//This class holds all data used by each vagon (things such as
//bitmaps, etc)
class WAGON_DATA
{
public:
    //Window images
    BITMAP *window_open;
    BITMAP *window_closed;
    
    //The front door bitmaps
    BASE_SPRITE base_door_opening;
    BASE_SPRITE base_door_closing;
    
    //The wagon bitmap
    BITMAP *wagon_image;
    
    
    
    //The same data, but halfed the size
    //Window images
    BITMAP *window_open_half;
    BITMAP *window_closed_half;
    
    //The front door bitmaps
    BASE_SPRITE base_door_opening_half;
    BASE_SPRITE base_door_closing_half;
    
    //The wagon bitmap
    BITMAP *wagon_image_half;
    
    
    
    //Side doors indications bitmaps
    BITMAP *door_left_open;
    BITMAP *door_left_closed;
    BITMAP *door_right_open;
    BITMAP *door_right_closed;
    
    BITMAP *door_left_open_half;
    BITMAP *door_left_closed_half;
    BITMAP *door_right_open_half;
    BITMAP *door_right_closed_half;
    
    
    
    //Only constructor and destructor needed
    WAGON_DATA();
    ~WAGON_DATA();
};
    
    

//A wagon window
class WAGON_WINDOW
{
public:
    //Is it open or closed?
    bool is_closed;
    
    //The window position
    int x, y;
    
    //The bitmaps for the window
    BITMAP *open;
    BITMAP *closed;
    
    BITMAP *open_half;
    BITMAP *closed_half;
    
    //Constructor just set some defaults
    WAGON_WINDOW();
    
    //Set the window status
    void setStatus(bool closed);
    
    //Set the windows information
    void setWindow(int x, int y, BITMAP *open, BITMAP *closed, BITMAP *open_half, BITMAP *closed_half);
    
    //Draw the window
    void draw(BITMAP *buffer, int x_offset, int y_offset, int zoom);
};



//The wagon front door
class WAGON_FRONT_DOOR
{
public:
    //The sprites for the door
    SPRITE spr_door_opening;
    SPRITE spr_door_closing;
    
    SPRITE spr_door_opening_half;
    SPRITE spr_door_closing_half;
    
    //Point to quick things a bit
    SPRITE *spr_draw;
    
    //The position of the door
    int x, y;
    
    //The door status
    int status;
    
    //Cosntructor just default some values
    WAGON_FRONT_DOOR();
    
    //Set the door status
    void setStatus(int status);
    
    //Set the door information
    void setDoor(int x, int y, BASE_SPRITE *base_door_opening, BASE_SPRITE *base_door_closing, BASE_SPRITE *base_door_opening_half, BASE_SPRITE *base_door_closing_half);
    
    //Draw the door
    void draw(BITMAP *buffer, int x_offset, int y_offset, int zoom);
};



//Finally, the wagon itself
class WAGON
{
public:
    //Pointer to the needed data
    WAGON_DATA *data;
    
    //The windows vector
    WAGON_WINDOW *windows;
    
    //The front door
    WAGON_FRONT_DOOR front_door;
    
    
    
    //Leftmost and rightmost door status data
    bool left_door_open;
    bool right_door_open;
    
    
    
    //Wagon position on the screen
    int x, y;
    
    
    
    //How much will the side doors resists?
    int left_door_resistance;
    int right_door_resistance;
    
    
    //In what zoom are we?
    int zoom;
    
    
    
    //The constructor loads the bitmaps, set the windows and door positions
    WAGON();
    
    //Set the wagon
    void setWagon(int x, int y, WAGON_DATA *data);
    
    //Set the zoom
    void setZoom(int zoom);
    
    //Set the side door status
    void setLeftDoorOpen(bool left_door_open);
    void setRightDoorOpen(bool right_door_open);
    
    //Check the sides door status
    bool getLeftDoorOpen();
    bool getRightDoorOpen();
    
    //Hits on the side doors
    void left_door_hit(int hits);
    void right_door_hit(int hits);
    
    //Draw the wagon
    void draw(BITMAP *buffer, int camera_x, int camera_y);
    
    //Returns an action (if possible) to be activated if the position
    //can activate it
    int requestAction(int x_position);
    
    //Do an action from a position
    void doAction(int x_position);
    
    
    //Prepare for the use of the open door function
    void prepareOpenDoor();
    
    //Prepare for the use of the close door function
    void prepareCloseDoor();
    
    //Open the wagon's door. Returns true when it is open
    bool openDoor();
    
    //Close the wagons's door. Returns true when it is closed
    bool closeDoor();
    
    //Destructor unload everything
    ~WAGON();
};
    

#endif
