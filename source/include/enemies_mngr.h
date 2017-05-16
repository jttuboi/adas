#ifndef ENEMIES_MANAGER_H
#define ENEMIES_MANAGER_H

#include<allegro.h>

#include"sprites.h"
#include"dbl_list.h"
#include"enemy.h"
#include"player.h" //For macros



class ENEMIES_MANAGER
{
public:
    //The enemies list
    DOUBLE_LIST<ENEMY> enemies_list;
    
    //List of the enemies at the station
    DOUBLE_LIST<ENEMY> enemies_on_station;
    
    
    
    //The bases of the sprites needed by the enemies
    
    //Fly
    BASE_SPRITE base_fly;
    BASE_SPRITE base_fly_half;
    
    //Zombie
    BASE_SPRITE base_zombie_standing;
    BASE_SPRITE base_zombie_walking_right;
    
    BASE_SPRITE base_zombie_standing_half;
    BASE_SPRITE base_zombie_walking_right_half;
    
    BASE_SPRITE base_zombie_dead;
    BASE_SPRITE base_zombie_dead_half;
    
    BASE_SPRITE base_zombie_attacking;
    BASE_SPRITE base_zombie_attacking_half;
    
    
    
    //Police zombie
    BASE_SPRITE base_police_zombie_standing;
    BASE_SPRITE base_police_zombie_walking_right;
    
    BASE_SPRITE base_police_zombie_standing_half;
    BASE_SPRITE base_police_zombie_walking_right_half;
    
    BASE_SPRITE base_police_zombie_dead;
    BASE_SPRITE base_police_zombie_dead_half;
    
    
    
    //Fat zombie
    BASE_SPRITE base_fat_zombie_standing;
    BASE_SPRITE base_fat_zombie_door;
    
    BASE_SPRITE base_fat_zombie_standing_half;
    BASE_SPRITE base_fat_zombie_door_half;
    
    
    
    //Collision data
    BOX_COLLISION_DATA box_fly;
    
    BOX_COLLISION_DATA box_zombie_standing;
    BOX_COLLISION_DATA box_zombie_walking_right;
    BOX_COLLISION_DATA box_zombie_walking_left;
    BOX_COLLISION_DATA box_zombie_attacking_right;
    BOX_COLLISION_DATA box_zombie_attacking_left;
    
    BOX_COLLISION_DATA box_police_zombie_standing;
    BOX_COLLISION_DATA box_police_zombie_walking_right;
    BOX_COLLISION_DATA box_police_zombie_walking_left;
    
    BOX_COLLISION_DATA box_fat_zombie_standing;
    
    
    
    //Number of enemies killed
    int enemies_destroyed;
    
    
    
    
    //Constructor
    ENEMIES_MANAGER();
    
    //Create all collision data
    void createCollisionData();
    
    //Call all enemies logic loop
    void logicLoop(int player_x, int player_y);
    
    //Create a fly
    void createFly(int x, int y, int delay);
    
    //Create a common zombie
    void createZombie(int x, int y, int delay);
    
    //Create a police zombie
    void createPoliceZombie(int x, int y, int delay);
    
    //Create a fat zombie
    void createFatZombie(int x, int y, int delay);
    
    //Put the enemies into the wagons
    void toTheWagons();
    
    //Return the biggest delay of all enemies on the station
    int getBiggestDelayOnStation();
    
    //Check enemy collision against closed doors
    int checkSideDoorCollision(BOX_COLLISION_DATA *box_door, int door_x, int door_y);
    
    //Check collision of the player against the enemies
    bool checkPlayerCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame, int player_attack_type, bool attacking_frame);
    
    //Check the collision of the enemies against the player
    int checkEnemyAttackCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame);
    
    //For player pistol. Return the x of the closest zombie or police zombie
    //to the left or right of the target x
    int checkClosestLeft(int target_x);
    int checkClosestRight(int target_x);
    
    //Now return a enemy pointer
    ENEMY *checkClosestEnemyLeft(int target_x);
    ENEMY *checkClosestEnemyRight(int target_x);
    
    //Remove an enemy from the list (by actual pointer on double list)
    void remove_enemy();
    
    //Clear all enemies from the lists
    void clear();
    
    //Draw all enemies inside the wagon
    void drawInsideWagon(BITMAP *buffer, int camera_x, int camera_y, int zoom);
    
    //Draw all enemies at the station
    void drawAtStation(BITMAP *buffer, int camera_x, int camera_y, int zoom);
    
    //Destructor
    ~ENEMIES_MANAGER();
};

#endif
