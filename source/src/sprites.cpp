#include"sprites.h"

BASE_SPRITE::BASE_SPRITE()
{
    //Nothing loaded yet
    loaded = false;
}



//Load a BSE_SPRITE from an allegro datafile
bool BASE_SPRITE::load_dat(string filename)
{
    //Cant load if already loaded
    if(loaded) return(false);



    //First open the datafile
    DATAFILE *data = load_datafile(filename.c_str());

    //Helping variable
    int *index;//Will be allocated later



    //The first data are everything we need to know about waits and indexes
    int *first_data;
    first_data = (int *) data[0].dat;

    //Get the number of bitmaps
    bitmaps_number = first_data[0];

    //Get the number of frames
    frames_number = first_data[1];

    //Allocate the memory for the framws wait and frames indexes
    wait = new int[frames_number];
    index = new int[frames_number];

    //Variable for the for loops (sounds cool eh)
    int counter = 2;

    //Get all the waits for the frames
    for(int i=0; i < frames_number; ++i)
    {
        wait[i] = first_data[counter];
        ++counter;
    }

    //Now get all the indexes for the frames
    for(int i=0; i < frames_number; ++i)
    {
        index[i] = first_data[counter];
        ++counter;
    }



    //Allocate the images vector and brute data vector
    images = new BITMAP*[frames_number];
    images_data = new BITMAP*[bitmaps_number];

    //Counter for reading the remaining data
    int bitmaps_counter = 1; //data[1].dat

    //Copy the bitmaps to the brute data bitmaps!
    for(int i = 0; i < bitmaps_number; ++i)
    {
        BITMAP *temp = (BITMAP *)data[bitmaps_counter].dat;

        images_data[i] = create_bitmap(temp->w, temp->h);

        blit(temp, images_data[i], 0, 0, 0, 0, temp->w, temp->h);

        ++bitmaps_counter;
    }



    //Now, finally set the images to the correct images_data
    for(int i=0; i < frames_number; ++i)
    {
        images[i] = images_data[index[i]];
    }


    //Remove the helping hand from memory
    delete[] index;

    //Unload the datafile
    unload_datafile(data);

    //All loaded
    loaded = true;

    return(true);
}



//Load a BSE_SPRITE from an allegro datafile (bitmaps with half size)
bool BASE_SPRITE::load_dat_halfed(string filename)
{
    //Cant load if already loaded
    if(loaded) return(false);



    //First open the datafile
    DATAFILE *data = load_datafile(filename.c_str());

    //Helping variable
    int *index;//Will be allocated later



    //The first data are everything we need to know about waits and indexes
    int *first_data;
    first_data = (int *) data[0].dat;

    //Get the number of bitmaps
    bitmaps_number = first_data[0];

    //Get the number of frames
    frames_number = first_data[1];

    //Allocate the memory for the framws wait and frames indexes
    wait = new int[frames_number];
    index = new int[frames_number];

    //Variable for the for loops (sounds cool eh)
    int counter = 2;

    //Get all the waits for the frames
    for(int i=0; i < frames_number; ++i)
    {
        wait[i] = first_data[counter];
        ++counter;
    }

    //Now get all the indexes for the frames
    for(int i=0; i < frames_number; ++i)
    {
        index[i] = first_data[counter];
        ++counter;
    }



    //Allocate the images vector and brute data vector
    images = new BITMAP*[frames_number];
    images_data = new BITMAP*[bitmaps_number];

    //Counter for reading the remaining data
    int bitmaps_counter = 1; //data[1].dat

    //Copy the bitmaps to the brute data bitmaps!
    for(int i = 0; i < bitmaps_number; ++i)
    {
        BITMAP *temp = bitmap_half((BITMAP *)data[bitmaps_counter].dat);
        
        images_data[i] = create_bitmap(temp->w, temp->h);

        blit(temp, images_data[i], 0, 0, 0, 0, temp->w, temp->h);

        ++bitmaps_counter;
    }



    //Now, finally set the images to the correct images_data
    for(int i=0; i < frames_number; ++i)
    {
        images[i] = images_data[index[i]];
    }


    //Remove the helping hand from memory
    delete[] index;

    //Unload the datafile
    unload_datafile(data);

    //All loaded
    loaded = true;

    return(true);
}
    
    




void BASE_SPRITE::unload()
{
    if(loaded)
    {
        //Unload the wait vector
        delete[] wait;

        //Unload the bitmaps
        for(int i=0; i < bitmaps_number; ++i)
        {
            destroy_bitmap(images_data[i]);
        }

        //And the images vector as well
        delete[] images;
        delete[] images_data;



        //We are not loaded now
        loaded = false;
    }
}



BASE_SPRITE::~BASE_SPRITE()
{
    //Unload everything
    unload();
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



SPRITE::SPRITE()
{
    //Set the default values
    frame = 0;
    wait_time = 0.0f;
    speed = 0.0f;
    loaded = false;

    //Draw nothing
    ptr_draw = &SPRITE::draw_nothing;
}



//Connect this sprite with a base
bool SPRITE::base_sprite_connect(BASE_SPRITE *base_sprite)
{
    //Check if the base sprite does exists
    if(!base_sprite) return false;
    if(!base_sprite->loaded) return false;

    //Disconnect the base sprite, if there is one connected
    if(loaded)base_sprite_disconnect();

    //Link
    this->base_sprite = base_sprite;
    sprite = base_sprite->images[0];

    //Reset basic values to default
    frame = 0;
    wait_time = 0.0f;
    speed = 1.0f;

    hflip = false;
    vflip = false;
    pure = false;
    rotated = false;
    angle = 0;

    //Update the sprite, so it will be drawn correctly
    update();



    //Create the x and y origins vector
    x_origins = new int[base_sprite->frames_number];
    y_origins = new int[base_sprite->frames_number];

    //Set them all to zero
    for(int i=0; i < base_sprite->frames_number; ++i)
    {
        x_origins[i] = 0;
        y_origins[i] = 0;
    }



    //Get new origins
    x_origin = x_origins[0];
    y_origin = y_origins[0];



    //Assume this sprite is animated and with loops
    animation_loop = true;



    //Aniamtion hasn't ended not even once
    animation_ended = false;



    //The sprite is "loaded"
    loaded = true;

    //Set a default value to if animated or not
    if(base_sprite->frames_number > 0) animated = true; else animated = false;

    //Return success
    return true;
}



//Connect with an overloaded operator
bool SPRITE::operator=(BASE_SPRITE *base_sprite)
{
    return(base_sprite_connect(base_sprite));
}

bool SPRITE::operator=(BASE_SPRITE &base_sprite)
{
    return(base_sprite_connect(&base_sprite));
}



//Disconnect this sprite from its base
void SPRITE::base_sprite_disconnect(void)
{
    //Check if this is an valid operation
    if(!loaded)return;

    //Unload x and y origins vectors
    delete[] x_origins;
    delete[] y_origins;

    //Disconnect
    loaded = false;
    base_sprite = NULL;
    sprite = NULL;

    //Update this sprite, so it will not be drawn
    update();
}



//Set a new animation speed
void SPRITE::set_speed(float speed)
{
    this->speed = speed;
}



//Set a frame to be shown
void SPRITE::set_frame(int frame)
{
    if(frame < 0) frame = 0;
    if(frame >= base_sprite->frames_number) frame = base_sprite->frames_number - 1;
    
    this->frame = frame;
    sprite = base_sprite->images[frame];
    
    wait_time = 0;
}



//Get a frame width or height
int SPRITE::get_frame_width(int frame)
{
    //Cant do anything if not loaded
    if(!loaded) return(0);

    //Get the info
    if(frame < 0 || frame > base_sprite->frames_number) return(0);

    return(base_sprite->images[frame]->w);
}

int SPRITE::get_frame_height(int frame)
{
    //Cant do anything if not loaded
    if(!loaded) return(0);

    //Get the info
    if(frame < 0 || frame > base_sprite->frames_number) return(0);

    return(base_sprite->images[frame]->h);
}



//Get the number of frames of this sprite (connected to a base sprite)
int SPRITE::get_frames_number()
{
    return(base_sprite->frames_number);
}



//Get the number of logical frames needed for running the
//whole animation
int SPRITE::get_animation_size()
{
    //Helping variable
    int frames_amount = 0;

    //Get the time
    for(int i=0; i < base_sprite->frames_number; ++i)
    {
        frames_amount += base_sprite->wait[i];
    }

    //The speed may differ the results. This division sets up a right value
    float temp = frames_amount;
    temp /= speed;

    //Set the right time now
    frames_amount = (int) temp;

    //Return the amount of frames
    return(frames_amount);
}



//Get the origins of a frame
int SPRITE::get_x_origin(int frame)
{
    return(x_origins[frame]);
}

int SPRITE::get_y_origin(int frame)
{
    return(y_origins[frame]);
}



//Returns true if the animation has ended at least once
bool SPRITE::get_animation_end()
{
    return(animation_ended);
}



//Autoset the origins values to the center of each frame
void SPRITE::autoset_origins()
{
    //Cant do anything if not loaded
    if(!loaded) return;

    //Very simple to understand, really
    for(int i=0; i < base_sprite->frames_number; ++i)
    {
        x_origins[i] = get_frame_width(i)/2;
        y_origins[i] = get_frame_height(i)/2;
    }

    //Get new origins
    x_origin = x_origins[frame];
    y_origin = y_origins[frame];
}

//Autoset horizontal origins
void SPRITE::autoset_x_origins()
{
    //Cant do anything if not loaded
    if(!loaded) return;

    //Very simple to understand, really
    for(int i=0; i < base_sprite->frames_number; ++i)
    {
        x_origins[i] = get_frame_width(i)/2;
    }

    //Get new origins
    x_origin = x_origins[frame];
}

//Autoset vertical origins
void SPRITE::autoset_y_origins()
{
    //Cant do anything if not loaded
    if(!loaded) return;

    //Very simple to understand, really
    for(int i=0; i < base_sprite->frames_number; ++i)
    {
        y_origins[i] = get_frame_height(i)/2;
    }

    //Get new origins
    y_origin = y_origins[frame];
}



void SPRITE::logic_loop(void)
{
    //Only execute a logic loop if needed
    if(!loaded || !animated)return;

    if(base_sprite->frames_number == 0) return;

    //Update the frames
    wait_time += speed;

    if((int)wait_time > base_sprite->wait[frame])
    {
        //Reset variables
        wait_time = 0.0f;
        ++frame;

        //If the animation has ended, go back for the first frame
        //as long as this animation has loops
        if(frame >= base_sprite->frames_number)
        {
            if(animation_loop) frame = 0;
                          else --frame;

            animation_ended = true;
        }

        //Set the new main sprite
        sprite = base_sprite->images[frame];

        //Get new origins
        x_origin = x_origins[frame];
        y_origin = y_origins[frame];
    }
}



void SPRITE::update(void)
{
    //If this sprite is not loaded, we cant draw it
    if(!loaded) ptr_draw = &SPRITE::draw_nothing;

    //Update the ptr_draw pointer to the correct function
    if(hflip)
    {
        if(vflip)
        {
            ptr_draw = &SPRITE::draw_vhflip;
        } else {
            ptr_draw = &SPRITE::draw_hflip;
        }
    } else {
        if(vflip)
        {
            ptr_draw = &SPRITE::draw_vflip;
        } else {
            if(rotated)
            {
                ptr_draw = &SPRITE::draw_rotated;
            } else {
                if(pure)
                {
                    ptr_draw = &SPRITE::draw_pure;
                } else {
                    ptr_draw = &SPRITE::draw_normal;
                }
            }
        }
    }
}



void SPRITE::draw(BITMAP *buffer, int x, int y)
{
    (this->*ptr_draw)(buffer, x - x_origin, y - y_origin);
}



void SPRITE::draw_normal(BITMAP *buffer, int x, int y)
{
    draw_sprite(buffer, sprite, x, y);
}



void SPRITE::draw_pure(BITMAP *buffer, int x, int y)
{
    blit(sprite, buffer, 0, 0, x, y, sprite->w, sprite->h);
}



void SPRITE::draw_hflip(BITMAP *buffer, int x, int y)
{
    draw_sprite_h_flip(buffer, sprite, x, y);
}



void SPRITE::draw_vflip(BITMAP *buffer, int x, int y)
{
    draw_sprite_v_flip(buffer, sprite, x, y);
}



void SPRITE::draw_vhflip(BITMAP *buffer, int x, int y)
{
    draw_sprite_vh_flip(buffer, sprite, x, y);
}



void SPRITE::draw_rotated(BITMAP *buffer, int x, int y)
{
    rotate_sprite(buffer, sprite, x, y, angle);
}



void SPRITE::draw_nothing(BITMAP *buffer, int x, int y)
{
    //Draw nothing
}



SPRITE::~SPRITE()
{
    base_sprite_disconnect();
}











BITMAP *bitmap_half(BITMAP *original)
{
    //New bitmap to copy the old bitmap into it
    BITMAP *aux = create_bitmap(original->w, original->h);
    
    blit(original, aux, 0, 0, 0, 0, original->w, original->h);
    
    BITMAP *scaled = create_bitmap(aux->w / 2, aux->h / 2);
    clear_to_color(scaled, makecol(255, 0, 255));
    
    stretch_sprite(scaled, aux, 0, 0, aux->w / 2, aux->h / 2);
    
    destroy_bitmap(aux);
    
    return(scaled);
}
    
    
