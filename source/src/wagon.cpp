#include"wagon.h"



//Only constructor and destructor needed
WAGON_DATA::WAGON_DATA()
{
    //Load the window bitmaps
    window_open = load_bitmap("media/window_open.bmp", NULL);
    window_closed = load_bitmap("media/window_closed.bmp", NULL);
    
    //Load the front door bitmaps
    base_door_opening.load_dat("media/door_opening.dat");
    base_door_closing.load_dat("media/door_closing.dat");
    
    //Load the wagon image
    wagon_image = load_bitmap("media/wagon.bmp", NULL);
    
    
    
    //Side doors indications bitmaps
    door_left_open = load_bitmap("media/door_open_left.bmp", NULL);
    door_left_closed = load_bitmap("media/door_closed_left.bmp", NULL);
    door_right_open = load_bitmap("media/door_open_right.bmp", NULL);
    door_right_closed = load_bitmap("media/door_closed_right.bmp", NULL);
    
    
    
    
    
    //Now everything halfed the size
    
    //Load the window bitmaps
    window_open_half = bitmap_half(window_open);
    window_closed_half = bitmap_half(window_closed);
    
    //Load the front door bitmaps
    base_door_opening_half.load_dat_halfed("media/door_opening.dat");
    base_door_closing_half.load_dat_halfed("media/door_closing.dat");
    
    //Load the wagon image
    wagon_image_half = bitmap_half(wagon_image);
    
    //Side doors indicators
    door_left_open_half = bitmap_half(door_left_open);
    door_left_closed_half = bitmap_half(door_left_closed);
    door_right_open_half = bitmap_half(door_right_open);
    door_right_closed_half = bitmap_half(door_right_closed);
}




WAGON_DATA::~WAGON_DATA()
{
    //Unload the window bitmaps
    destroy_bitmap(window_open);
    destroy_bitmap(window_closed);
    
    destroy_bitmap(window_open_half);
    destroy_bitmap(window_closed_half);
    
    
    
    //Unload the front door images
    base_door_closing.unload();
    base_door_opening.unload();
    
    base_door_closing_half.unload();
    base_door_opening_half.unload();
    
    
    
    //Unload side doors indicators
    destroy_bitmap(door_left_open);
    destroy_bitmap(door_left_closed);
    destroy_bitmap(door_right_open);
    destroy_bitmap(door_right_closed);
    
    destroy_bitmap(door_left_open_half);
    destroy_bitmap(door_left_closed_half);
    destroy_bitmap(door_right_open_half);
    destroy_bitmap(door_right_closed_half);
    
    
    
    //Unload the wagon image
    destroy_bitmap(wagon_image);
    
    destroy_bitmap(wagon_image_half);
}



//Constructor just set some defaults
WAGON_WINDOW::WAGON_WINDOW()
{
    x = 0;
    y = 0;
    
    open = NULL;
    closed = NULL;
    
    is_closed = true;
}



//Set the window status
void WAGON_WINDOW::setStatus(bool closed)
{
    is_closed = closed;
}



//Set the windows information
void WAGON_WINDOW::setWindow(int x, int y, BITMAP *open, BITMAP *closed, BITMAP *open_half, BITMAP *closed_half)
{
    this->x = x;
    this->y = y;
    
    this->open = open;
    this->closed = closed;
    
    this->open_half = open_half;
    this->closed_half = closed_half;
}



//Draw the window
void WAGON_WINDOW::draw(BITMAP *buffer, int x_offset, int y_offset, int zoom)
{
    switch(zoom)
    {
        case ZOOM_NONE:
        {
            if(is_closed)
                draw_sprite(buffer, closed, x + x_offset, y + y_offset);
            else
                draw_sprite(buffer, open, x + x_offset, y + y_offset);
        } break;
        
        
        
        case ZOOM_OUT:
        {
            if(is_closed)
                draw_sprite(buffer, closed_half, x/2 + x_offset, y/2 + y_offset);
            else
                draw_sprite(buffer, open_half, x/2 + x_offset, y/2 + y_offset);
        } break;
    }
}










//Cosntructor just default some values
WAGON_FRONT_DOOR::WAGON_FRONT_DOOR()
{
    x = 0;
    y = 0;
    
    status = DOOR_CLOSED;
}



//Set the door status
void WAGON_FRONT_DOOR::setStatus(int status)
{
    this->status = status;
}



//Set the door information
void WAGON_FRONT_DOOR::setDoor(int x, int y, BASE_SPRITE *base_door_opening, BASE_SPRITE *base_door_closing, BASE_SPRITE *base_door_opening_half, BASE_SPRITE *base_door_closing_half)
{
    this->x = x;
    this->y = y;
    
    spr_door_opening = base_door_opening;
    spr_door_closing = base_door_closing;
    
    spr_door_opening_half = base_door_opening_half;
    spr_door_closing_half = base_door_closing_half;
    
    //Set the default image to the closed door
    spr_draw = &spr_door_closing;
    spr_draw->set_frame(spr_draw->get_frames_number()-1);
}



//Draw the door
void WAGON_FRONT_DOOR::draw(BITMAP *buffer, int x_offset, int y_offset, int zoom)
{
    switch(zoom)
    {
        case ZOOM_NONE:
        {
            spr_draw->draw(buffer, x + x_offset, y + y_offset);
        } break;
        
        
        
        case ZOOM_OUT:
        {
            //Have to avoid mistaken the aniamtion at all costs here
            SPRITE *spr_temp;
            
            if(spr_draw == &spr_door_opening)
            {
                spr_temp = &spr_door_opening_half;
                spr_temp->set_frame(spr_door_opening.frame);
            } else {
                spr_temp = &spr_door_closing_half;
                spr_temp->set_frame(spr_door_closing.frame);
            }
            
            spr_temp->draw(buffer, (x/2) + x_offset, (y/2) + y_offset);
        } break;
    }
}











//The constructor loads the bitmaps, set the windows and door positions
WAGON::WAGON()
{
    //Set a default wagon position on the screen
    x = 0;
    y = 0;
    
    //Side doors starts open
    left_door_open = true;
    right_door_open = true;
}



//Set the wagon
void WAGON::setWagon(int x, int y, WAGON_DATA *data)
{
    this->x = x;
    this->y = y;
    
    this->data = data;
    
    //Create the windows vector
    windows = new WAGON_WINDOW[WINDOWS_NUMBER];
    
    
    
    //Set the windows as needed
    windows[0].setWindow(60, 49, data->window_open, data->window_closed, data->window_open_half, data->window_closed_half);
    windows[1].setWindow(196, 49, data->window_open, data->window_closed, data->window_open_half, data->window_closed_half);
    windows[2].setWindow(533, 49, data->window_open, data->window_closed, data->window_open_half, data->window_closed_half);
    windows[3].setWindow(669, 49, data->window_open, data->window_closed, data->window_open_half, data->window_closed_half);
    
    
    
    //Set the front door
    front_door.setDoor(330, 49, &(data->base_door_opening), &(data->base_door_closing), &(data->base_door_opening_half), &(data->base_door_closing_half));
}



//Set the zoom
void WAGON::setZoom(int zoom)
{
    this->zoom = zoom;
}



//Set the side door status
void WAGON::setLeftDoorOpen(bool left_door_open)
{
    this->left_door_open = left_door_open;
    
    if(!left_door_open)
    {
        left_door_resistance = SIDE_DOOR_HITS;
    }
}



void WAGON::setRightDoorOpen(bool right_door_open)
{
    this->right_door_open = right_door_open;
    
    if(!right_door_open)
    {
        right_door_resistance = SIDE_DOOR_HITS;
    }
}



//Check the sides door status
bool WAGON::getLeftDoorOpen()
{
    return(left_door_open);
}



bool WAGON::getRightDoorOpen()
{
    return(right_door_open);
}



//Hits on the side doors
void WAGON::left_door_hit(int hits)
{
    left_door_resistance -= hits;
    
    if(left_door_resistance <= 0) left_door_open = true;
}



void WAGON::right_door_hit(int hits)
{
    right_door_resistance -= hits;
    
    if(right_door_resistance <= 0) right_door_open = true;
}



//Draw the wagon
void WAGON::draw(BITMAP *buffer, int camera_x, int camera_y)
{
    switch(zoom)
    {
        case ZOOM_NONE:
        {
            //Draw the wagon
            draw_sprite(buffer, data->wagon_image, x - camera_x, y - camera_y);
            
            //Draw the windows
            for(int i=0; i < WINDOWS_NUMBER; ++i)
            {
                windows[i].draw(buffer, x - camera_x, y - camera_y, zoom);
            }
            
            //Draw the door
            front_door.draw(buffer, x - camera_x, y - camera_y, zoom);
            
            //Draw the side doors indicators
            if(left_door_open)
            {
                draw_sprite(buffer, data->door_left_open, 1 + (x - camera_x), 30 + (y - camera_y));
            } else {
                draw_sprite(buffer, data->door_left_closed, 1 + (x - camera_x), 30 + (y - camera_y));
            }
            
            
            
            if(right_door_open)
            {
                draw_sprite(buffer, data->door_right_open, (799 - data->door_right_open->w) + (x - camera_x), 30 + (y - camera_y));
            } else {
                draw_sprite(buffer, data->door_right_closed, (799 - data->door_right_open->w) + (x - camera_x), 30 + (y - camera_y));
            }
        } break;
        
        
        
        case ZOOM_OUT:
        {
            //Draw the wagon
            draw_sprite(buffer, data->wagon_image_half, (x/2) - camera_x, (y/2) - camera_y);
            
            //Draw the windows
            for(int i=0; i < WINDOWS_NUMBER; ++i)
            {
                windows[i].draw(buffer, (x/2) - camera_x, (y/2) - camera_y, zoom);
            }
            
            //Draw the door
            front_door.draw(buffer, (x/2) - camera_x, (y/2) - camera_y, zoom);
            
            //Draw the side doors indicators
            if(left_door_open)
            {
                draw_sprite(buffer, data->door_left_open_half, (x/2) - camera_x, 15 + ((y/2) - camera_y));
            } else {
                draw_sprite(buffer, data->door_left_closed_half, (x/2) - camera_x, 15 + ((y/2) - camera_y));
            }
            
            
            
            if(right_door_open)
            {
                draw_sprite(buffer, data->door_right_open_half, (400 - data->door_right_open_half->w) + ((x/2) - camera_x), 15 + ((y/2) - camera_y));
            } else {
                draw_sprite(buffer, data->door_right_closed_half, (400 - data->door_right_open_half->w) + ((x/2) - camera_x), 15 + ((y/2) - camera_y));
            }
        } break;
    }
}



//Returns an action (if possible) to be activated if the position
//can activate it
int WAGON::requestAction(int x_position)
{
    //Check against the windows
    if((x_position >= x + 60 && x_position <= x + 130) ||
       (x_position >= x + 196 && x_position <= x + 266) ||
       (x_position >= x + 533 && x_position <= x + 603) ||
       (x_position >= x + 669 && x_position <= x + 739))
    {
        return(WAGON_ACTION_WINDOW);
    }
    
    //Check against the side doors
    if(x_position >= x && x_position <= x + 30)
    {
        return(WAGON_ACTION_LEFT_DOOR);
    }
    
    if(x_position >= x + 770 && x_position <= x + 800)
    {
        return(WAGON_ACTION_RIGHT_DOOR);
    }
    
    if(x_position >= x + 260 && x_position <= x + 540)
    {
        return(WAGON_ACTION_FRONT_DOOR);
    }
    
    //If we reached here, there is no action to be done
    return(WAGON_ACTION_NONE);
}



//Do an action from a position
void WAGON::doAction(int x_position)
{
    //Check for the windows
    if(x_position >= x + 60 && x_position <= x + 130)
    {
        windows[0].setStatus(!windows[0].is_closed);
    }
    if(x_position >= x + 196 && x_position <= x + 266)
    {
        windows[1].setStatus(!windows[1].is_closed);
    }
    if(x_position >= x + 533 && x_position <= x + 603)
    {
        windows[2].setStatus(!windows[2].is_closed);
    }
    if(x_position >= x + 669 && x_position <= x + 739)
    {
        windows[3].setStatus(!windows[3].is_closed);
    }
    
    //Check for the side doors
    if(x_position >= x && x_position <= x + 30)
    {
        left_door_open = !left_door_open;
        if(!left_door_open)
        {
            left_door_resistance = SIDE_DOOR_HITS;
        }
    }
    
    if(x_position >= x + 770 && x_position <= x + 800)
    {
        right_door_open = !right_door_open;
        if(!right_door_open)
        {
            right_door_resistance = SIDE_DOOR_HITS;
        }
    }
}



//Prepare for the use of the open door function
void WAGON::WAGON::prepareOpenDoor()
{
    front_door.spr_door_opening.set_frame(0);
    front_door.spr_door_opening.animation_ended = false;
    front_door.spr_draw = &(front_door.spr_door_opening);
}



//Prepare for the use of the close door function
void WAGON::WAGON::prepareCloseDoor()
{
    front_door.spr_door_closing.set_frame(0);
    front_door.spr_door_closing.animation_ended = false;
    front_door.spr_draw = &(front_door.spr_door_closing);
}



//Open the wagon's door. Returns true when it is open
bool WAGON::openDoor()
{
    front_door.spr_door_opening.logic_loop();
    
    if(front_door.spr_door_opening.animation_ended)
    {
        front_door.spr_door_opening.set_frame(front_door.spr_door_opening.get_frames_number()-1);
        return(true);
    }
    else
        return(false);
}



//Close the wagons's door. Returns true when it is closed
bool WAGON::closeDoor()
{
    front_door.spr_door_closing.logic_loop();
    
    if(front_door.spr_door_closing.animation_ended)
    {
        front_door.spr_door_closing.set_frame(front_door.spr_door_closing.get_frames_number()-1);
        return(true);
    }
    else
        return(false);
}



//Destructor unload everything
WAGON::~WAGON()
{
    //Unload all the windows
    delete[] windows;
}
