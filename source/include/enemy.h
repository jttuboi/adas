#ifndef ENEMY_H
#define ENEMY_H

#include"main.h"
#include"sprites.h"
#include"collision.h"
#include"player.h"//for macros



#define DEAD_TIME_STAY                                       60

#define ENEMY_ATACK_WAIT                                     60



//All enemies will com from this parent class
class ENEMY
{
public:
    //The enemy position
    int x, y;
    
    //Target position
    int target_x, target_y;
    
    //Pointer to the sprite being used
    SPRITE *sprite;
    
    //Is the enemy alive?
    bool alive;
    
    //Is the enemy inside the wagon?
    bool flag_start;
    
    //Delay of the enemy after he started
    int delay;
    
    //If this is true, the enemy must never die
    bool immortal;
    
    //This boolean is true in enemies attacking frames
    bool enemy_attacking_frame;
    
    //This is the point to the collision data to be used
    BOX_COLLISION_DATA *collision_data;
    
    
    
    //Waiting time between enemy attacks
    int enemy_attack_wait;
    
    
    
    //For dead enemies
    bool dead_event;
    int dead_time_counter;
    
    
    
    //Constructor
    ENEMY();
    
    //Set the enemy position
    void setPosition(int x, int y);
    
    //Set a target position
    void setTargetPosition(int target_x, int target_y);
    
    //Set him as alive or dead
    void setLife(bool alive);
    
    //Set if the enemy is inside the wagon
    void setFlagStart(bool flag_start);
    
    //Set a start delay for the enemy (time before entering the wagon
    void setDelay(int delay);
    
    //Set a collision data address
    void setCollisionData(BOX_COLLISION_DATA *collision_data);
    
    //Set if the enemy is immortal
    void setImmortal(bool immortal);
    
    
    
    //Return the delay time
    int getDelay();
    
    
    
    //Check collision againt a door
    bool checkSideDoorCollision(BOX_COLLISION_DATA *box_door, int door_x, int door_y);
    
    //Check for collision against the player
    virtual bool checkPlayerCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame, int player_attack_type, bool attacking_frame);
    
    //Check if the enemy is attacking the player
    virtual int checkAttackingCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame);
    
    
    
    //Start the enemy
    void start();
    
    //Logic loop
    virtual void logicLoop(int player_x, int player_y);
    
    //Draw the enemy
    virtual void draw(BITMAP *buffer, int camera_x, int camera_y, int zoom);
    
    //What happens when the enemy dies?
    virtual void die();
    
    //Destructor
    virtual ~ENEMY();
};



class ENEMY_FLY: public ENEMY
{
public:
    SPRITE spr_fly;
    SPRITE spr_fly_half;
    
    
    
    BOX_COLLISION_DATA *coll_fly;
    
    
    
    //Constructor
    ENEMY_FLY();
    
    //Set the sprite
    void setSprite(BASE_SPRITE *base_fly, BASE_SPRITE *base_fly_half);
    
    //Set the collision data
    void setFlyCollisionData(BOX_COLLISION_DATA *coll_fly);
    
    //Check for collision against the player
    bool checkPlayerCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame, int player_attack_type, bool attacking_frame);
    
    //Check if the enemy is attacking the player
    int checkAttackingCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame);

    //Logic loop
    void logicLoop(int player_x, int player_y);
    
    //Draw the enemy
    void draw(BITMAP *buffer, int camera_x, int camera_y, int zoom);
    
    //Destructor
    ~ENEMY_FLY();
};












class ENEMY_ZOMBIE: public ENEMY
{
public:
    SPRITE spr_zombie_standing;
	SPRITE spr_zombie_right;
	
	SPRITE spr_zombie_standing_half;
	SPRITE spr_zombie_right_half;
	
	SPRITE spr_zombie_dead;
	SPRITE spr_zombie_dead_half;
	
	SPRITE spr_zombie_attacking;
	SPRITE spr_zombie_attacking_half;
	
	SPRITE *spr_zombie;
	
	
	
	//Zombie collision data
	BOX_COLLISION_DATA *coll_zombie_standing;
	BOX_COLLISION_DATA *coll_zombie_left;
	BOX_COLLISION_DATA *coll_zombie_right;
	BOX_COLLISION_DATA *coll_zombie_attacking_left;
	BOX_COLLISION_DATA *coll_zombie_attacking_right;
	
	
    
    bool spr_flag;
    
    int delay_random;
    int direction;
    
    //Constructor
    ENEMY_ZOMBIE();
    
    //Set the sprite
    void setSprite(BASE_SPRITE *base_standing, BASE_SPRITE *base_right, BASE_SPRITE *base_zombie_dead, BASE_SPRITE *base_zombie_attacking, BASE_SPRITE *base_standing_half, BASE_SPRITE *base_right_half, BASE_SPRITE *base_zombie_dead_half, BASE_SPRITE *base_zombie_attacking_half);
    
    //Set the collision data
    void setZombieCollisionData(BOX_COLLISION_DATA *coll_zombie_standing, BOX_COLLISION_DATA *coll_zombie_left, BOX_COLLISION_DATA *coll_zombie_right, BOX_COLLISION_DATA *coll_zombie_attacking_left, BOX_COLLISION_DATA *zombie_attacking_right);
    
    //Check for collision against the player
    bool checkPlayerCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame, int player_attack_type, bool attacking_frame);
   
    //Logic loop
    void logicLoop(int player_x, int player_y);
    
    //Draw the enemy
    void draw(BITMAP *buffer, int camera_x, int camera_y, int zoom);
    
    //Destructor
    ~ENEMY_ZOMBIE();
};



class ENEMY_POLICE_ZOMBIE: public ENEMY
{
public:
    SPRITE spr_zombie_standing;
	SPRITE spr_zombie_right;
	
	SPRITE spr_zombie_standing_half;
	SPRITE spr_zombie_right_half;
	
	SPRITE spr_zombie_dead;
	SPRITE spr_zombie_dead_half;
	
	SPRITE *spr_zombie;
	
	
	
	//Zombie collision data
	BOX_COLLISION_DATA *coll_zombie_standing;
	BOX_COLLISION_DATA *coll_zombie_left;
	BOX_COLLISION_DATA *coll_zombie_right;
	
	
    
    bool spr_flag;
    
    int delay_random;
    int direction;
    
    //Constructor
    ENEMY_POLICE_ZOMBIE();
    
    //Set the sprite
    void setSprite(BASE_SPRITE *base_standing, BASE_SPRITE *base_right, BASE_SPRITE *base_zombie_dead, BASE_SPRITE *base_standing_half, BASE_SPRITE *base_right_half, BASE_SPRITE *base_zombie_dead_half);
    
    //Set the collision data
    void setZombieCollisionData(BOX_COLLISION_DATA *coll_zombie_standing, BOX_COLLISION_DATA *coll_zombie_left, BOX_COLLISION_DATA *coll_zombie_right);
    
    //Check for collision against the player
    bool checkPlayerCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame, int player_attack_type, bool attacking_frame);
   
    //Logic loop
    void logicLoop(int player_x, int player_y);
    
    //Draw the enemy
    void draw(BITMAP *buffer, int camera_x, int camera_y, int zoom);
    
    //Destructor
    ~ENEMY_POLICE_ZOMBIE();
};







class ENEMY_FAT_ZOMBIE: public ENEMY
{
public:
    SPRITE spr_zombie_fat_door;
    SPRITE spr_zombie_fat_standing;
    
    SPRITE spr_zombie_fat_door_half;
    SPRITE spr_zombie_fat_standing_half;
    
    
    
    SPRITE *sprite_draw;
	
	
	
	//Zombie collision data
	BOX_COLLISION_DATA *coll_zombie_fat_standing;
	
	
    
    bool spr_flag;
    
    
    
    //Constructor
    ENEMY_FAT_ZOMBIE();
    
    //Set the sprite
    void setSprite(BASE_SPRITE *base_zombie_fat_standing, BASE_SPRITE *base_zombie_fat_door, BASE_SPRITE *base_zombie_fat_standing_half, BASE_SPRITE *base_zombie_fat_door_half);
    
    //Set the collision data
    void setZombieCollisionData(BOX_COLLISION_DATA *coll_zombie_fat_standing);
    
    //Check for collision against the player
    bool checkPlayerCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame, int player_attack_type, bool attacking_frame);
    
    //Check if the enemy is attacking the player
    int checkAttackingCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame);
    
    //Logic loop
    void logicLoop(int player_x, int player_y);
    
    //Draw the enemy
    void draw(BITMAP *buffer, int camera_x, int camera_y, int zoom);
    
    //Destructor
    ~ENEMY_FAT_ZOMBIE();
};



#endif
