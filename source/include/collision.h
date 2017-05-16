#ifndef COLLISION_H
#define COLLISION_H



#include"sprites.h"



//The various collisions types
#define COLL_BOX                                           1
#define COLL_CIRCLE                                        2
#define COLL_LINE                                          3



//This class is used to check collisions. It uses methods that receives
//another COLLISION class and coordinates that returns true on collision.
//
//The class supports bounding box collision checking, and if it hits, there
//is the option for a pixel perfect checking. But for doing so, the class
//must be created using the appropriated constructor.
//
//This class may result, so, in a few collisions types:
//1)Box x Box: both without pixel perfect. Normal checking only is done.
//2)Box (pixel) x Box: one with pixel perfect flag, the other without.
//                     This is a case where a pixel perfect is done against
//                     a box. The box acts as a rectangular sprite.
//3)Box (pixel) x Box (pixel): normal pixel perfect checking
//
//Actually, this classe only sees two things: pixel perfect or not =)
//
//For now, this class only supports magenta as transparent pixel.
//
//
//
//There is no checking if the classes are usable or not at collision checking,
//to avoid making one checkage (speeds up the code). If there is the possibility
//of the class not being usable, the checkage must be done by hand. There is no
//checkage for invalid checking operations (e.g. index out of bounds errors)
//as well.
//
//
//
//There are overloaded functions and operators to make checking easier to
//do (and cooler, too XD). To avoid loosing too much speed, no checking is done
//if these pointers are set or not. If they have not been set, need to use
//the normal checking functions (the ones with lots of arguments). The other
//ones will bug as hell the code and won't war anything. See functions comments
//at the header to know which can be called and which cant without the pointers
//set



class BOX_COLLISION_DATA
{
public:
    //Number of frames we are dealing with
    int frames_number;
    
    //The main variables: the box size and relative positions
    int *x_rel;
    int *y_rel;
    int *box_width;
    int *box_height;
    
    //The pixel perfect flag
    bool pp_flag;
    
    
    
    //Vector of matrices to hold pixel perfect data
    //
    //Stands for:
    //pp_data[frame][x][y]
    bool ***pp_data;
    //The pixel perfect matrix is a pure set of booleans where 1 is
    //any color (but one) and 0 is magenta (255, 0, 255)
    
    
    
    //The use of these pointers is optional. Set them to point to the
    //relative (and right) variables and then there is the easy use of
    //overloaded functions and operators.
    int *x;
    int *y;
    int *frame;
    
    
    
    //This class is set for use
    bool usable;
    
    
    
    ////////////////////////////////////////////////////////////////
    //                      Constructors                          //
    ////////////////////////////////////////////////////////////////
    
    //The "as always" set nothing constructor
    BOX_COLLISION_DATA();
    
    ////////////////////////////////////////////////////////////////
    //                        Functions                           //
    ////////////////////////////////////////////////////////////////
    
    
    
    //Load the data directly from a BASE_SPRITE. The pp_flag
    //indicates if this is to be loaded as pixel perfect or box only
    //Relative positions here are set to zero
    bool set(BASE_SPRITE *base, bool pp_flag);
    
    //Same as normal set, but pixel perfect data is inverted
    bool setInverted(BASE_SPRITE *base, bool pp_flag);
    
    
    
    //Set the "pointer" to the class, so the overloaded function can be
    //used
    bool set_pointers(int *x, int *y, int *frame);
    
    
    
    //This function set the origins of the boxes as the origins of a sprite.
    //No error be checking, so be sure to use a sprite from the same
    //base.
    void set_origins(SPRITE *sprite);
    
    
    
    //Draw the bounding box with a color
    void draw(BITMAP *buffer, int red, int green, int blue, int x, int y, int frame);
    
    
    
    //Unload the data
    void unload();
    
    
    ////////////////////////////////////////////////////////////////
    //                      Destructors                           //
    ////////////////////////////////////////////////////////////////
    ~BOX_COLLISION_DATA();
};





//This function checks the collision between two collision boxes,
//being then pixel perfect or not
bool check_collision(BOX_COLLISION_DATA *box1, int box1_x, int box1_y, int box1_frame,
                     BOX_COLLISION_DATA *box2, int box2_x, int box2_y, int box2_frame);

    

#endif
