#ifndef SPRITES_H
#define SPRITES_H



#include<allegro.h>
#include<string>

using namespace std;



//This is the base for sprites...
class BASE_SPRITE
{
public:
    //Numbers of bitmaps the base holds
    int bitmaps_number;

    //Number of frames in this base
    int frames_number;

    //Wait for each frame of the base
    int *wait;

    //The brute data for the base sprite
    BITMAP **images_data;

    //Bitmaps vector (of the animation)
    BITMAP **images;

    //Flag: anything loaded?
    bool loaded;



    //Constructor
    BASE_SPRITE();



    //Load the informations from an allegro datafile. The first data MUST
    //be the file that holds the informations for the BASE_SPRITE, and the
    //following data are the bitmaps...
    bool load_dat(string filename);
    
    //Load a BSE_SPRITE from an allegro datafile (bitmaps with half size)
    bool load_dat_halfed(string filename);
    
    //Unload the base
    void unload();

    //Destructor
    ~BASE_SPRITE();
};






class SPRITE
{
public:
    //Frame of the animation that this sprite is in
    int frame;

    //How many frames has this sprite been in this frame
    float wait_time;

    //Speed of the animation
    float speed;

    //Is there anything loaded?
    bool loaded;

    //"Origins" position of the sprite. Used for finding its drawing center
    int *x_origins, *y_origins;

    //These variables only holds the value of the origins of the actual frame
    int x_origin, y_origin;

    //Pointer for the sprite that need to be drawn
    BITMAP *sprite;

    //Pointer for the base that this sprite is using
    BASE_SPRITE *base_sprite;

    //Flags for drawing
    bool hflip, vflip, pure, rotated;

    //Angle for rotations
    fixed angle;

    //Special purpose flag: is this one animated?
    bool animated;

    //Another special purpose flag: is this animation to loop?
    bool animation_loop;

    //Yet another special purpose flag: has this animation ended at least once?
    bool animation_ended;

    //Pointer for the drawing function
    void (SPRITE::*ptr_draw)(BITMAP *buffer, int x, int y);



    //Constructor
    SPRITE();



    //Connect this sprite with a base
    bool base_sprite_connect(BASE_SPRITE *base_sprite);

    //Connect with an overloaded operator
    bool operator=(BASE_SPRITE *base_sprite);
    bool operator=(BASE_SPRITE &base_sprite);



    //Disconnect this this sprite with its base
    void base_sprite_disconnect();



    //Set a new animation speed
    void set_speed(float speed);
    
    
    
    //Set a frame to be shown
    void set_frame(int frame);



    //Get a frame width or height
    int get_frame_width(int frame);
    int get_frame_height(int frame);



    //Get the number of frames of this sprite (connected to a base sprite)
    int get_frames_number();



    //Get the number of logical frames needed for running the
    //whole animation
    int get_animation_size();



    //Get the origins of a frame
    int get_x_origin(int frame);
    int get_y_origin(int frame);



    //Returns true if the animation has ended at least once
    bool get_animation_end();



    //Autoset the origins values to the center of each frame
    void autoset_origins();

    //Autoset horizontal origins
    void autoset_x_origins();

    //Autoset vertical origins
    void autoset_y_origins();



    //Logical loop for the sprite
    void logic_loop();



    //Update the sprite with the flags value
    void update();



    //Draw this sprite in one bitmap
    void draw(BITMAP *buffer, int x, int y);

    //Lower level drawing functions
    void draw_normal(BITMAP *buffer, int x, int y);
    void draw_pure(BITMAP *buffer, int x, int y);
    void draw_hflip(BITMAP *buffer, int x, int y);
    void draw_vflip(BITMAP *buffer, int x, int y);
    void draw_vhflip(BITMAP *buffer, int x, int y);
    void draw_rotated(BITMAP *buffer, int x, int y);

    void draw_nothing(BITMAP *buffer, int x, int y);

    //Destructor
    ~SPRITE();
};



BITMAP *bitmap_half(BITMAP *original);

#endif
