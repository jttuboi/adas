#ifndef PLAYER_H
#define PLAYER_H

#include"main.h"

#include"sprites.h"
#include"wagon.h" //For the macros
#include"collision.h"

#define PLAYER_ACTION_NONE                             0
#define PLAYER_ACTION_WINDOW                           1



#define NONE                                           0
#define LEFT                                           1
#define RIGHT                                          2



#define PLAYER_ATTACK_TYPE_NONE                        0
#define PLAYER_ATTACK_TYPE_CQC                         1
#define PLAYER_ATTACK_TYPE_PROJECTILE                  2



#define PLAYER_PM_SHOW_WAIT_TIME                       30
#define PLAYER_PM_ITENS                                6



#define PLAYER_WEAPON_SUITCASE                         1
#define PLAYER_WEAPON_PISTOL                           2



#define PLAYER_STAMINA_BAR_WIDTH                       400
#define PLAYER_STAMINA_BAR_FULL_COLOR                  makecol(200, 200, 255)
#define PLAYER_STAMINA_RECOVER                         0.16f

#define PLAYER_STAMINA_SUITCASE                        12



//Ah hoh! the player
class PLAYER
{
public:
    //The player position
    int x, y;
    
    //Plaing areas limits (horizontal)
    int x_min_limit, x_max_limit;
    
    
    //Many of the player sprites (bases)
    BASE_SPRITE spr_base_player_standing;
    BASE_SPRITE spr_base_player_window;
    BASE_SPRITE spr_base_player_walking_right_suitcase;
    BASE_SPRITE spr_base_player_suitcase_attack;
    BASE_SPRITE spr_base_player_side_door;
    BASE_SPRITE spr_base_player_pistol;
    
    //Many of the player sprites (sprites)
    SPRITE spr_player_standing;
    SPRITE spr_player_window;
    SPRITE spr_player_walking_right_suitcase;
    SPRITE spr_player_suitcase_attack;
    SPRITE spr_player_side_door;
    SPRITE spr_player_pistol;
    
    
    
    //Now the scaled bases of the sprites (for zoom out)
    BASE_SPRITE spr_base_player_standing_half;
    BASE_SPRITE spr_base_player_window_half;
    BASE_SPRITE spr_base_player_walking_right_suitcase_half;
    BASE_SPRITE spr_base_player_suitcase_attack_half;
    BASE_SPRITE spr_base_player_side_door_half;
    BASE_SPRITE spr_base_player_pistol_half;
    
    //And the scaled sprites
    SPRITE spr_player_standing_half;
    SPRITE spr_player_window_half;
    SPRITE spr_player_walking_right_suitcase_half;
    SPRITE spr_player_suitcase_attack_half;
    SPRITE spr_player_side_door_half;
    SPRITE spr_player_pistol_half;
    
    
    
    //This boolean indicates if the player is attacking or not
    bool attacking;
    
    //The player attack type
    int attack_type;
    
    //What weapon the player holds?
    int player_weapon;
    
    
    
    //What side was the player turned on?
    int direction;
    
    
    
    //What sprite will be drawn?
    SPRITE *spr_to_draw;
    
    
    
    //Is the player trying an action?
    bool action;
    
    
    
    //This variable will be set to one kind of action
    int action_type;
    
    //Will be true for one logic frame whenever a action has ended
    bool action_finished;
    
    
    
    //Player stamina (in %'s)
    float stamina;
    float stamina_effect; //For nice stamina loss effect
    
    
    
    //What zoom are we currently working?
    int zoom;
    
    
    
    //Collision data for collision checking
    BOX_COLLISION_DATA coll_player_standing;
    BOX_COLLISION_DATA coll_player_window;
    BOX_COLLISION_DATA coll_player_walking_suitcase_left;
    BOX_COLLISION_DATA coll_player_walking_suitcase_right;
    BOX_COLLISION_DATA coll_player_suitcase_attack_left;
    BOX_COLLISION_DATA coll_player_suitcase_attack_right;
    BOX_COLLISION_DATA coll_player_left_door;
    BOX_COLLISION_DATA coll_player_right_door;
    BOX_COLLISION_DATA coll_player_pistol_right;
    BOX_COLLISION_DATA coll_player_pistol_left;
    
    //Pointer of the actual box data
    BOX_COLLISION_DATA *collision_data;
    
    
    
    //Status to the player (for post mortem)
    int times_changed_wagons;
    int suitcase_attacks;
    int suitcase_hits;
    
    
    
    //For post mortem use
    int show;
    int show_wait;
    
    
    
    //Locks the controls (so the player cant play)
    bool controls_locked;
    
    
    
    //This boolean says if this player is in "attacking_frame" or not
    //meelees attacks only works when this is true
    bool attacking_frame;
    
    
    
    //Gunshot?
    bool gunshot;
    
    
    
    //Yohai the constructor
    PLAYER();
    
    //Reset the status
    void resetStatus();
    
    //Update some status values
    void changedWagon();
    void suitcaseHit();
    
    //Set all the collision data
    void setCollisionData();
    
    //Set a player position
    void setPosition(int x, int y);
    
    //Set the area limit for the player
    void setAreaLimit(int x_min_limit, int x_max_limit);
    
    //Set player trying one action
    void setAction(bool action);
    
    //Set an action type for the player
    //(i.e. open window)
    void setActionType(int action_type);
    
    //Set the player zoom
    void setZoom(int zoom);
    
    
    
    //Locks and unlocks the control
    void lockControls();
    void unlockControls();
    
    
    
    //Check if the player is trying one action
    bool getAction();
    
    //Get the action type
    int getActionType();
    
    //Allows to check if an action has finished this frame
    bool getActionFinished();
    
    //Get the player attack style
    int getAttackType();
    
    //Check if the player is in an attacking frame
    bool getAttackingFrame();
    
    //Player received an stamina attack
    void staminaAttack();
    
    //If the player did shoot with a gun
    bool getPlayerGunShot();
    
    //Check player collision against a side door
    void checkSideDoorCollision(BOX_COLLISION_DATA *box_door, int door_x, int door_y);
    
    //Logic loop
    void logicLoop();
    
    //Draw stamina bar
    void drawStaminaBar(BITMAP *buffer, int x, int y);
    
    //Draw the player
    void draw(BITMAP *buffer, int camera_x, int camera_y);
    
    
    
    //Prepare for post mortem
    void prepare_post_mortem();
    
    //Post mortem logic loop
    void post_mortem_logic_loop();
    
    //Draw post mortem status
    void post_mortem_draw(BITMAP *buffer);
    
    //The destructor
    ~PLAYER();
};

#endif
