#include"collision.h"

////////////////////////////////////////////////////////////////
//                                                            //
//       Box Collision Data: Constructors                     //
//                                                            //
////////////////////////////////////////////////////////////////

//Set the defaults
BOX_COLLISION_DATA::BOX_COLLISION_DATA()
{
    frames_number = 0;
    x_rel = NULL;
    y_rel = NULL;
    box_width = NULL;
    box_height = NULL;
    pp_flag = false;
    usable = false;
}


////////////////////////////////////////////////////////////////
//                                                            //
//       Box Collision Data: Functions                        //
//                                                            //
////////////////////////////////////////////////////////////////



//Load the data directly from a BASE_SPRITE. The pp_flag
//indicates if this is to be loaded as pixel perfect or box only
bool BOX_COLLISION_DATA::set(BASE_SPRITE *base, bool pp_flag)
{
    //Unload if loaded
    unload();
    
    //Cant load if the base sprite is not loaded
    if(!base->loaded) return(false);
    
    //First of all, need to know how many frames there will be...
    frames_number = base->frames_number;
    
    //The box width and box height must be set to all frames.
    //The relative positions variables, too
    box_width = new int[frames_number];
    box_height = new int[frames_number];
    x_rel = new int[frames_number];
    y_rel = new int[frames_number];
    
    for(int i=0; i < frames_number; ++i)
    {
        //Set relative position values
        x_rel[i] = 0;
        y_rel[i] = 0;
        
        //Set the box width and height
        box_width[i] = base->images[i]->w;
        box_height[i] = base->images[i]->h;
    }
    
    
    
    //Set the pixel perfect flag
    this->pp_flag = pp_flag;
    
    
    
    //If there is no need for pixel perfect, done
    if(!pp_flag)
    {
        usable = true;
        return(true);
    }
    
    
    
    //Keep going on, creating the pixel perfect matrix
    pp_data = new bool**[frames_number];
    for(int frame = 0; frame < frames_number; ++frame)
    {
        pp_data[frame] = new bool*[box_width[frame]];
        
        for(int pp_x=0; pp_x < box_width[frame]; ++pp_x)
        {
            pp_data[frame][pp_x] = new bool[box_height[frame]];
        }
    }
    
    
    
    //Make the Pixel Perfect matrix
    
    //magenta color
    int color = makecol(255, 0, 255);
    
    for(int frame=0; frame < frames_number; ++frame)
    {
        for(int x_pos=0; x_pos < box_width[frame]; ++x_pos)
        {
            for(int y_pos=0; y_pos < box_height[frame]; ++y_pos)
            {
                if(getpixel(base->images[frame], x_pos, y_pos) == color)
                {
                    pp_data[frame][x_pos][y_pos] = false;
                } else {
                    pp_data[frame][x_pos][y_pos] = true;
                }
            }
        }
    }
    
    //Now all is set
    usable = true;
    return(true);
}







//Same as normal set, but pixel perfect data is inverted
bool BOX_COLLISION_DATA::setInverted(BASE_SPRITE *base, bool pp_flag)
{
    //Unload if loaded
    unload();
    
    //Cant load if the base sprite is not loaded
    if(!base->loaded) return(false);
    
    //First of all, need to know how many frames there will be...
    frames_number = base->frames_number;
    
    //The box width and box height must be set to all frames.
    //The relative positions variables, too
    box_width = new int[frames_number];
    box_height = new int[frames_number];
    x_rel = new int[frames_number];
    y_rel = new int[frames_number];
    
    for(int i=0; i < frames_number; ++i)
    {
        //Set relative position values
        x_rel[i] = 0;
        y_rel[i] = 0;
        
        //Set the box width and height
        box_width[i] = base->images[i]->w;
        box_height[i] = base->images[i]->h;
    }
    
    
    
    //Set the pixel perfect flag
    this->pp_flag = pp_flag;
    
    
    
    //If there is no need for pixel perfect, done
    if(!pp_flag)
    {
        usable = true;
        return(true);
    }
    
    
    
    //Keep going on, creating the pixel perfect matrix
    pp_data = new bool**[frames_number];
    for(int frame = 0; frame < frames_number; ++frame)
    {
        pp_data[frame] = new bool*[box_width[frame]];
        
        for(int pp_x=0; pp_x < box_width[frame]; ++pp_x)
        {
            pp_data[frame][pp_x] = new bool[box_height[frame]];
        }
    }
    
    
    
    //Make the Pixel Perfect matrix
    
    //magenta color
    int color = makecol(255, 0, 255);
    
    for(int frame=0; frame < frames_number; ++frame)
    {
        for(int x_pos=0; x_pos < box_width[frame]; ++x_pos)
        {
            for(int y_pos=0; y_pos < box_height[frame]; ++y_pos)
            {
                //Remebering: the píxel perfect data is horizontally inverted
                if(getpixel(base->images[frame], x_pos, y_pos) == color)
                {
                    pp_data[frame][(box_width[frame] - 1) - x_pos][y_pos] = false;
                } else {
                    pp_data[frame][(box_width[frame] - 1) - x_pos][y_pos] = true;
                }
            }
        }
    }
    
    //Now all is set
    usable = true;
    return(true);
}



//Set the "pointer" to the class, so the overloaded function can be
//used
bool BOX_COLLISION_DATA::set_pointers(int *x, int *y, int *frame)
{
    this->x = x;
    this->y = y;
    this->frame = frame;
    
    return(true);
}



//This function set the origins of the boxes as the origins of a sprite.
//No error be checking, so be sure to use a sprite from the same
//base.
void BOX_COLLISION_DATA::set_origins(SPRITE *sprite)
{
    for(int i=0; i < sprite->get_frames_number(); ++i)
    {
        x_rel[i] = sprite->get_x_origin(i);
        y_rel[i] = sprite->get_y_origin(i);
    }
}



//Draw the bounding box with a color
void BOX_COLLISION_DATA::draw(BITMAP *buffer, int red, int green, int blue, int x, int y, int frame)
{
    rect(buffer, x - x_rel[frame], y - y_rel[frame], (x - x_rel[frame]) + box_width[frame], (y - y_rel[frame]) + box_height[frame], makecol(red, green, blue));
}



//Unload the data
void BOX_COLLISION_DATA::unload()
{
    //If there is nothing, cant remove
    if(!usable) return;
    
    //Delete the Pixel Perfect matrix, if the is one
    if(pp_flag)
    {
        for(int frame=0; frame < frames_number; ++frame)
        {
            for(int x_pos=0; x_pos < box_width[frame]; ++x_pos)
            {
                delete[] pp_data[frame][x_pos];
            }
            
            delete[] pp_data[frame];
        }
        
        delete[] pp_data;
    }
    
    
    
    //Delete the relative positions, box width and box height vectors
    delete[] x_rel;
    delete[] y_rel;
    delete[] box_width;
    delete[] box_height;
    
    
    
    //Unloaded, and thus unusable
    usable = false;
}



////////////////////////////////////////////////////////////////
//                                                            //
//       Box Collision Data: Destructors                      //
//                                                            //
////////////////////////////////////////////////////////////////



BOX_COLLISION_DATA::~BOX_COLLISION_DATA()
{
    unload();
}


















////////////////////////////////////////////////////////////////
//                                                            //
//       Collision Checking Functions                         //
//                                                            //
////////////////////////////////////////////////////////////////

bool check_collision(BOX_COLLISION_DATA *box1, int box1_x, int box1_y, int box1_frame,
                     BOX_COLLISION_DATA *box2, int box2_x, int box2_y, int box2_frame)
{
    //Set the right positions for checking
    int box1_check_x1 = box1_x - box1->x_rel[box1_frame];
    int box1_check_y1 = box1_y - box1->y_rel[box1_frame];
    int box1_check_x2 = box1_check_x1 + box1->box_width[box1_frame];
    int box1_check_y2 = box1_check_y1 + box1->box_height[box1_frame];
    
    int box2_check_x1 = box2_x - box2->x_rel[box2_frame];
    int box2_check_y1 = box2_y - box2->y_rel[box2_frame];
    int box2_check_x2 = box2_check_x1 + box2->box_width[box2_frame];
    int box2_check_y2 = box2_check_y1 + box2->box_height[box2_frame];
    
    //Bouding box check
    if(box1_check_x2 >= box2_check_x1 && box1_check_x1 <= box2_check_x2 &&
       box1_check_y2 >= box2_check_y1 && box1_check_y1 <= box2_check_y2)
    {
        //Reached here, so the boxes collides
        
        if(!box1->pp_flag)
        {
            if(!box2->pp_flag)
            {
                //Bounding box checking only
                return(true);
            } else {
                //box1 is a box, but the box2 is pixel perfect
                
                //Normalize the positions to a relative position within
                //the origin. Important to limit the checkage only to the
                //the intersection of the boxes.
                
                box2_check_x1 -= box1_check_x1;
                box2_check_y1 -= box1_check_y1;
                box2_check_x2 -= box1_check_x1;
                box2_check_y2 -= box1_check_y1;
                
                box1_check_x1 = 0;
                box1_check_y1 = 0;
                box1_check_x2 = box1->box_width[box1_frame];
                box1_check_y2 = box1->box_height[box1_frame];
                
                
                
                //Determine the intersection interval
                
                //Biggest x1
                int check_x1 = (box1_check_x1 >= box2_check_x1 ? box1_check_x1 : box2_check_x1);
                
                //Smallest x2
                int check_x2 = (box1_check_x2 <= box2_check_x2 ? box1_check_x2 : box2_check_x2);
                
                //Biggest y1
                int check_y1 = (box1_check_y1 >= box2_check_y1 ? box1_check_y1 : box2_check_y1);
                
                //Smallest y2
                int check_y2 = (box1_check_y2 <= box2_check_y2 ? box1_check_y2 : box2_check_y2);
                
                
                
                //Calculate the checking area size. It must be values that we
                //are sure that they are only the exact size of the intersection
                //of the boxes.
                int check_width = check_x2 - check_x1;
                int check_height = check_y2 - check_y1;
                
                
                
                //Checks x1 and y1 are the position in box1 box, but not the
                //box2. Must hold the starting checking position for the
                //box2 box, too
                int check2_x = check_x1 - box2_check_x1;
                int check2_y = check_y1 - box2_check_y1;
                
                //Confirm they are not negative
                if(check2_x < 0) check2_x = 0;
                if(check2_y < 0) check2_y = 0;
                
                
                
                //Pivots for checking
                int pivot_x = check_x1 + check_width;
                int pivot_y = check_y1 + check_height;
                
                
                
                //We have both positions and the interval size. Check pizel
                //per pixel now
                for(int y1_pos=check_y1, y2_pos=check2_y; y1_pos < pivot_y; ++y1_pos, ++y2_pos)
                {
                    for(int x1_pos=check_x1, x2_pos=check2_x; x1_pos < pivot_x; ++x1_pos, ++x2_pos)
                    {
                        if(box2->pp_data[box2_frame][x2_pos][y2_pos] == true)
                        {
                            //Success. Return true. Ended
                            return(true);
                        }
                    }
                }
            }
        } else {
            if(!box2->pp_flag)
            {
                //box1 is a pixel perfect, but the box2 one is box only
                
                //Normalize the positions to a relative position within
                //the origin. Important to limit the checkage only to the
                //the intersection of the boxes.
                
                box2_check_x1 -= box1_check_x1;
                box2_check_y1 -= box1_check_y1;
                box2_check_x2 -= box1_check_x1;
                box2_check_y2 -= box1_check_y1;
                
                box1_check_x1 = 0;
                box1_check_y1 = 0;
                box1_check_x2 = box1->box_width[box1_frame];
                box1_check_y2 = box1->box_height[box1_frame];
                
                
                
                //Determine the intersection interval
                
                //Biggest x1
                int check_x1 = (box1_check_x1 >= box2_check_x1 ? box1_check_x1 : box2_check_x1);
                
                //Smallest x2
                int check_x2 = (box1_check_x2 <= box2_check_x2 ? box1_check_x2 : box2_check_x2);
                
                //Biggest y1
                int check_y1 = (box1_check_y1 >= box2_check_y1 ? box1_check_y1 : box2_check_y1);
                
                //Smallest y2
                int check_y2 = (box1_check_y2 <= box2_check_y2 ? box1_check_y2 : box2_check_y2);
                
                
                
                //Calculate the checking area size. It must be values that we
                //are sure that they are only the exact size of the intersection
                //of the boxes.
                int check_width = check_x2 - check_x1;
                int check_height = check_y2 - check_y1;
                
                
                
                //Checks x1 and y1 are the position in box1 box, but not the
                //box2. Must hold the starting checking position for the
                //box2 box, too
                int check2_x = check_x1 - box2_check_x1;
                int check2_y = check_y1 - box2_check_y1;
                
                //Confirm they are not negative
                if(check2_x < 0) check2_x = 0;
                if(check2_y < 0) check2_y = 0;
                
                
                
                //Pivots for checking
                int pivot_x = check_x1 + check_width;
                int pivot_y = check_y1 + check_height;
                
                
                
                //We have both positions and the interval size. Check pizel
                //per pixel now
                for(int y1_pos=check_y1, y2_pos=check2_y; y1_pos < pivot_y; ++y1_pos, ++y2_pos)
                {
                    for(int x1_pos=check_x1, x2_pos=check2_x; x1_pos < pivot_x; ++x1_pos, ++x2_pos)
                    {
                        if(box1->pp_data[box1_frame][x1_pos][y1_pos] == true)
                        {
                            //Success. Return true. Ended
                            return(true);
                        }
                    }
                }
            } else {
                //Both of them are pixel perfect.
                
                //Normalize the positions to a relative position within
                //the origin. Important to limit the checkage only to the
                //the intersection of the boxes.
                
                box2_check_x1 -= box1_check_x1;
                box2_check_y1 -= box1_check_y1;
                box2_check_x2 -= box1_check_x1;
                box2_check_y2 -= box1_check_y1;
                
                box1_check_x1 = 0;
                box1_check_y1 = 0;
                box1_check_x2 = box1->box_width[box1_frame];
                box1_check_y2 = box1->box_height[box1_frame];
                
                
                
                //Determine the intersection interval
                
                //Biggest x1
                int check_x1 = (box1_check_x1 >= box2_check_x1 ? box1_check_x1 : box2_check_x1);
                
                //Smallest x2
                int check_x2 = (box1_check_x2 <= box2_check_x2 ? box1_check_x2 : box2_check_x2);
                
                //Biggest y1
                int check_y1 = (box1_check_y1 >= box2_check_y1 ? box1_check_y1 : box2_check_y1);
                
                //Smallest y2
                int check_y2 = (box1_check_y2 <= box2_check_y2 ? box1_check_y2 : box2_check_y2);
                
                
                
                //Calculate the checking area size. It must be values that we
                //are sure that they are only the exact size of the intersection
                //of the boxes.
                int check_width = check_x2 - check_x1;
                int check_height = check_y2 - check_y1;
                
                
                
                //Checks x1 and y1 are the position in box1 box, but not the
                //box2. Must hold the starting checking position for the
                //box2 box, too
                int check2_x = check_x1 - box2_check_x1;
                int check2_y = check_y1 - box2_check_y1;
                
                //Confirm they are not negative
                if(check2_x < 0) check2_x = 0;
                if(check2_y < 0) check2_y = 0;
                
                
                
                //Pivots for checking
                int pivot_x = check_x1 + check_width;
                int pivot_y = check_y1 + check_height;
                
                
                
                //We have both positions and the interval size. Check pizel
                //per pixel now
                for(int y1_pos=check_y1, y2_pos=check2_y; y1_pos < pivot_y; ++y1_pos, ++y2_pos)
                {
                    for(int x1_pos=check_x1, x2_pos=check2_x; x1_pos < pivot_x; ++x1_pos, ++x2_pos)
                    {
                        if(box1->pp_data[box1_frame][x1_pos][y1_pos] == true &&
                           box2->pp_data[box2_frame][x2_pos][y2_pos] == true)
                        {
                            //Success. Return true. Ended
                            return(true);
                        }
                    }
                }
            }
        }
    }
    
    //If we reached here, it means there is no collision
    return(false);
}
