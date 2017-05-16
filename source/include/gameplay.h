#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include<allegro.h>

#include"main.h"
#include"wagon.h"
#include"player.h"
#include"background.h"
#include"enemies_mngr.h"
#include"sprites.h"
#include"collision.h"
#include"railway.h"



#define STATIONS_MAIN_NUMBER                           4

#define LINES_MAIN_NUMBER                              3



#define GAMEPLAY_TYPE_NORMAL                           1



#define GAMEPLAY_MAX_SPEED                             15



#define GAMEPLAY_STATION_STAGE_STOPPING                 1
#define GAMEPLAY_STATION_STAGE_DOORS_OPPENING           2
#define GAMEPLAY_STATION_STAGE_DOORS_OPEN               3
#define GAMEPLAY_STATION_STAGE_DOORS_CLOSING            4
#define GAMEPLAY_STATION_STAGE_EXITING                  5



#define GAME_ENDED_WRONG_STATION                        1
#define GAME_ENDED_LOST_STATION                         2
#define GAME_ENDED_PLAYER_DIED                          3



#define WARNING_NONE                                    0
#define WARNING_STATION_CHANGE                          1
#define WARNING_STATION_CHANGE_NOW                      2
#define WARNING_CORRECT_TRAIN_CHANGE                    3



#define GAMEPLAY_STATION_WAIT_TIME                      120



#define TYPE_RANDOM                                     0
#define TYPE_ZOMBIE                                     1
#define TYPE_POLICE_ZOMBIE                              2
#define TYPE_FLIES                                      3



//How much time before the station will the train slow down?
#define GAMEPLAY_TIME_SLOW_DOWN                        300

//How much time do the train waits after there are no enemies
//in front of the doors?
#define GAMEPLAY_TIME_STOPPED                          300

//How much time the train keeps at low speed after getting out
//of a station?
#define GAMEPLAY_TIME_ACCELERATE                       180


class GAMEPLAY
{
public:
    //The line number (stage) the player actually is
    int line;
    
    //The number of lines the player needs to get through
    int lines_number;
    
    //Vector of the amount of stations the player needs to pass
    //by in each line
    int *stations_number;
    
    //What station the player actually is in
    int station;
    
    
    
    //Size of the subway machine =) (in wagons)
    int wagons_number;
    
    //The wagon data
    WAGON_DATA wagon_data;
    
    //The train
    WAGON *wagons;
    
    
    
    //The player
    PLAYER player;
    
    
    
    //The gameplay area limit (horizontal). Varies with the number of wagons
    int area_x_min_limit, area_x_max_limit;
    
    
    
    //Camera position
    int camera_x, camera_y;
    
    //Camera drawing positions (for zooms)
    int camera_draw_x, camera_draw_y;
    
    
    
    //Background handler
    BACKGROUND background;
    
    
    
    //Enemies handler
    ENEMIES_MANAGER enemies_manager;
    
    
    
    //Game play organizer: railway for line and stations
    RAILWAY railway;
    
    BASE_SPRITE base_rail;
    
    
    
    //Time organizer, until the next station
    int time_next_station;
    int stopped_time;
    
    //This is the station that the player must get out
    bool this_station;
    
    
    
    //Warning message to be shown
    int warning;
    
    
    
    //Flag to say if the train is on wait at soem station
    bool on_station;
    
    //If the player did get off at the right station
    bool station_ok;
    
    //Flag to say what we are doing at the station
    int station_action;
    
    //Position of the station
    int station_x;
    
    //Integer that counts how much time the train will stay at the station
    int station_wait;
    
    //Station image!
    BITMAP *spr_station;
    BITMAP *spr_station_half;
    
    
    
    //Mask for the wagons side doors collision checking
    BASE_SPRITE base_side_door_mask;
    
    //The collision data for it
    BOX_COLLISION_DATA coll_side_door;
    
    
    
    //The zoom in or out
    int zoom;
    
    
    
    //We need to know if the game has ended
    bool game_ended;
    int game_ended_reason;
    
    
    
    //Gameplay type (makes the game mode)
    int gameplay_type;
    int gameplay_stage;
    
    //Has post mortem been prepared?
    bool post_mortem_prepared;
    
    
    
    
    
    
    //Constructor
    GAMEPLAY();
    
    //Set a new gameplay
    void setGame(int wagons_number, int lines_number, int stations_number_max, int stations_number_variance, int gameplay_type);
    
    //Set a camera position
    void setCameraPosition(int camera_x, int camera_y);
    
    //Set a area limit (horizontal)
    void setAreaLimit(int area_x_min_limit, int area_x_max_limit);
    
    //Create a enemy (not fat) of a type
    void createEnemy(int type, int wagon, int delay);
    
    //Logic loop for the game
    void logicLoop();
    
    //Draw everything
    void draw(BITMAP *buffer);
    
    //Destructor
    ~GAMEPLAY();
};

#endif
