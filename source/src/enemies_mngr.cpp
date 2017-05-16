#include"enemies_mngr.h"



//Constructor
ENEMIES_MANAGER::ENEMIES_MANAGER()
{
    //Load the base sprites we need for the enemies
    
    //Fly
    base_fly.load_dat("media/fly.dat");
    
    base_fly_half.load_dat_halfed("media/fly.dat");
    
    
    
    //Zombie
    base_zombie_standing.load_dat("media/zombie_standing.dat");
    base_zombie_walking_right.load_dat("media/zombie_walking.dat");
    
    base_zombie_standing_half.load_dat_halfed("media/zombie_standing.dat");
    base_zombie_walking_right_half.load_dat_halfed("media/zombie_walking.dat");
    
    base_zombie_dead.load_dat("media/zombie_dead.dat");
    base_zombie_dead_half.load_dat_halfed("media/zombie_dead.dat");
    
    base_zombie_attacking.load_dat("media/zombie_attack.dat");
    base_zombie_attacking_half.load_dat_halfed("media/zombie_attack.dat");
    
    
    
    //Police Zombie
    base_police_zombie_standing.load_dat("media/zombie_police_standing.dat");
    base_police_zombie_walking_right.load_dat("media/zombie_police_walking.dat");
    
    base_police_zombie_standing_half.load_dat_halfed("media/zombie_police_standing.dat");
    base_police_zombie_walking_right_half.load_dat_halfed("media/zombie_police_walking.dat");
    
    base_police_zombie_dead.load_dat("media/zombie_dead.dat");
    base_police_zombie_dead_half.load_dat_halfed("media/zombie_dead.dat");
    
    
    
    //Fat zombie
    base_fat_zombie_door.load_dat("media/zombie_fat_door.dat");
    base_fat_zombie_door_half.load_dat_halfed("media/zombie_fat_door.dat");
    
    base_fat_zombie_standing.load_dat("media/zombie_fat_standing.dat");
    base_fat_zombie_standing_half.load_dat_halfed("media/zombie_fat_standing.dat");
    
    
    
    //No enemies destroyed by now
    enemies_destroyed = 0;
    
    
    
    //Create the collision data
    createCollisionData();
}



//Create all collision data
void ENEMIES_MANAGER::createCollisionData()
{
    box_fly.set(&base_fly, true);
    
    box_zombie_standing.set(&base_zombie_standing, true);
    box_zombie_walking_right.set(&base_zombie_walking_right, true);
    box_zombie_walking_left.setInverted(&base_zombie_walking_right, true);
    box_zombie_attacking_right.set(&base_zombie_attacking, true);
    box_zombie_attacking_left.setInverted(&base_zombie_attacking, true);
    
    box_police_zombie_standing.set(&base_police_zombie_standing, true);
    box_police_zombie_walking_right.set(&base_police_zombie_walking_right, true);
    box_police_zombie_walking_left.setInverted(&base_police_zombie_walking_right, true);
    
    box_fat_zombie_standing.set(&base_fat_zombie_standing, true);
    
    
    //Set the origins
    SPRITE temp;
    
    temp = base_fly;
    temp.autoset_x_origins();
    box_fly.set_origins(&temp);
    
    
    
    temp = base_zombie_standing;
    temp.autoset_x_origins();
    box_zombie_standing.set_origins(&temp);
    
    temp = base_zombie_walking_right;
    temp.autoset_x_origins();
    box_zombie_walking_right.set_origins(&temp);
    box_zombie_walking_left.set_origins(&temp);
    
    temp = base_zombie_attacking;
    temp.autoset_x_origins();
    box_zombie_attacking_right.set_origins(&temp);
    box_zombie_attacking_left.set_origins(&temp);
    
    
    
    temp = base_police_zombie_standing;
    temp.autoset_x_origins();
    box_police_zombie_standing.set_origins(&temp);
    
    temp = base_police_zombie_walking_right;
    temp.autoset_x_origins();
    box_police_zombie_walking_right.set_origins(&temp);
    box_police_zombie_walking_left.set_origins(&temp);
}



//Call all enemies logic loop
void ENEMIES_MANAGER::logicLoop(int player_x, int player_y)
{
    //Run all enemies logic loop
    
    
    
    //Enemies on station
    //No elements, no loop
    if(enemies_on_station.get_elements() != 0)
    {
        //Needs to start from the beggining of the list.
        enemies_on_station.reset_actual();



        //Helping pointer
        ENEMY *enemy;

        //Do the loop
        while((enemy = enemies_on_station.get_obj()) != NULL)
        {
            //Check if the enemy died
            if(!enemy->alive)
            {
                //Remove the enemy
                remove_enemy();

                //If the enemy was the head of the list, things must
                //be corrected so the loop wont fail.
                if(!enemies_on_station.actual_valid())
                {
                    //There is a chance it was the head. Resets the actual
                    enemies_on_station.reset_actual();
                } else {
                    //It wasnt the head, so keep loop as normal
                    enemies_on_station.next();
                }
            } else {
                //Enemy is very alive, do the logical loop
                enemy->logicLoop(player_x, player_y);

                //Next enemy
                enemies_on_station.next();
            }
        }
    }
    
    
    
    
    
    
    
    //Enemies list (on wagon)
    //No elements, no loop
    if(enemies_list.get_elements() != 0)
    {
        //Needs to start from the beggining of the list.
        enemies_list.reset_actual();



        //Helping pointer
        ENEMY *enemy;

        //Do the loop
        while((enemy = enemies_list.get_obj()) != NULL)
        {
            //Check if the enemy died
            if(!enemy->alive && !enemy->immortal)
            {
                //Remove the enemy
                remove_enemy();

                //If the enemy was the head of the list, things must
                //be corrected so the loop wont fail.
                if(!enemies_list.actual_valid())
                {
                    //There is a chance it was the head. Resets the actual
                    enemies_list.reset_actual();
                } else {
                    //It wasnt the head, so keep loop as normal
                    enemies_list.next();
                }
            } else {
                //Enemy is very alive, do the logical loop
                enemy->logicLoop(player_x, player_y);

                //Next enemy
                enemies_list.next();
            }
        }
    }
}



//Create a fly
void ENEMIES_MANAGER::createFly(int x, int y, int delay)
{
    //To create the annoying fly
    ENEMY_FLY *fly = new ENEMY_FLY;
    
    //Set the basic fly settings
    fly->setSprite(&base_fly, &base_fly_half);
    fly->setFlyCollisionData(&box_fly);
    fly->setPosition(x, y);
    fly->setDelay(delay);
    fly->setTargetPosition(-1, 70);
    
    //Put the fly into the list
    enemies_on_station.insert((ENEMY*)fly);
}



//Create a common zombie
void ENEMIES_MANAGER::createZombie(int x, int y, int delay)
{
    //Create a zombie?
    ENEMY_ZOMBIE *zombie = new ENEMY_ZOMBIE;
    
    zombie->setSprite(&base_zombie_standing, &base_zombie_walking_right, &base_zombie_dead, &base_zombie_attacking, &base_zombie_standing_half, &base_zombie_walking_right_half, &base_zombie_dead_half, &base_zombie_attacking_half);
    zombie->setZombieCollisionData(&box_zombie_standing, &box_zombie_walking_left, &box_zombie_walking_right, &box_zombie_attacking_left, &box_zombie_attacking_right);
    zombie->setPosition(x, y);
    zombie->setDelay(delay);
    zombie->setTargetPosition(-1, -1);
    
    //Put the fly into the list
    enemies_on_station.insert((ENEMY*)zombie);
}



//Create a police zombie
void ENEMIES_MANAGER::createPoliceZombie(int x, int y, int delay)
{
    //Create a zombie?
    ENEMY_POLICE_ZOMBIE *zombie = new ENEMY_POLICE_ZOMBIE;
    
    zombie->setSprite(&base_police_zombie_standing, &base_police_zombie_walking_right, &base_police_zombie_dead, &base_police_zombie_standing_half, &base_police_zombie_walking_right_half, &base_police_zombie_dead_half);
    zombie->setZombieCollisionData(&box_police_zombie_standing, &box_police_zombie_walking_left, &box_police_zombie_walking_right);
    zombie->setPosition(x, y);
    zombie->setDelay(delay);
    zombie->setTargetPosition(-1, -1);
    
    //Put the fly into the list
    enemies_on_station.insert((ENEMY*)zombie);
}



//Create a fat zombie
void ENEMIES_MANAGER::createFatZombie(int x, int y, int delay)
{
    //Create the big, fat zombie
    ENEMY_FAT_ZOMBIE *zombie = new ENEMY_FAT_ZOMBIE;
    
    zombie->setSprite(&base_fat_zombie_standing, &base_fat_zombie_door, &base_fat_zombie_standing_half, &base_fat_zombie_door_half);
    zombie->setZombieCollisionData(&box_fat_zombie_standing);
    zombie->setPosition(x, y);
    zombie->setDelay(delay);
    zombie->setTargetPosition(-1, -1);
    
    //Put the fly into the list
    enemies_on_station.insert((ENEMY*)zombie);
}



//Put the enemies into the wagons
void ENEMIES_MANAGER::toTheWagons()
{
    //Puts the enemies from one list into another, and start them
    ENEMY *enemy;
    
    enemies_on_station.reset_actual();
    
    while((enemy = enemies_on_station.get_obj()) != NULL)
    {
        enemies_on_station.remove_obj();
        enemies_on_station.reset_actual();
        enemy->start();
        enemies_list.insert(enemy);
    }
}



//Return the biggest delay of all enemies on the station
int ENEMIES_MANAGER::getBiggestDelayOnStation()
{
    //No elements, no wait
    if(enemies_on_station.get_elements() == 0) return(0);
    
    
    int biggest_delay = 0;
    
    
    
    //Needs to start from the beggining of the list.
    enemies_on_station.reset_actual();
    
    
    
    //Helping pointer
    ENEMY *enemy;
    
    //Do the loop
    while((enemy = enemies_on_station.get_obj()) != NULL)
    {
        //Compare and get the delay
        if(enemy->getDelay() > biggest_delay) biggest_delay = enemy->getDelay();
        
        //Next enemy
        enemies_on_station.next();
    }
    
    
    
    //Return the biggest delay
    return(biggest_delay);
}



//Check enemy collision against closed doors
int ENEMIES_MANAGER::checkSideDoorCollision(BOX_COLLISION_DATA *box_door, int door_x, int door_y)
{
    //Enemies list (on wagon)
    
    int hits = 0;
    
    //No elements, no loop
    if(enemies_list.get_elements() != 0)
    {
        //Needs to start from the beggining of the list.
        enemies_list.reset_actual();



        //Helping pointer
        ENEMY *enemy;

        //Do the loop
        while((enemy = enemies_list.get_obj()) != NULL)
        {
            //Check against the door
            if(enemy->checkSideDoorCollision(box_door, door_x, door_y)) ++hits;;

            //Next enemy
            enemies_list.next();
        }
    }
    
    return(hits);
}



//Check collision of the enemy againt the player
bool ENEMIES_MANAGER::checkPlayerCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame, int player_attack_type, bool attacking_frame)
{
    //Enemies list (on wagon)
    
    if(!attacking_frame) return(false);
    
    //No elements, no loop
    if(enemies_list.get_elements() != 0)
    {
        //Needs to start from the beggining of the list.
        enemies_list.reset_actual();



        //Helping pointer
        ENEMY *enemy;

        //Do the loop
        while((enemy = enemies_list.get_obj()) != NULL)
        {
            //Check against the door
            
            //If we got a hit, this attack ends here
            if(enemy->checkPlayerCollision(box_player, player_x, player_y, player_frame, player_attack_type, attacking_frame))
            {
                return(true);
            }

            //Next enemy
            enemies_list.next();
        }
    }
    
    return(false);
}



//Check the collision of the enemies against the player
int ENEMIES_MANAGER::checkEnemyAttackCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame)
{
    //Enemies list (on wagon)
    
    int player_attacked = ENEMY_ATTACK_NONE;
    
    //No elements, no loop
    if(enemies_list.get_elements() != 0)
    {
        //Needs to start from the beggining of the list.
        enemies_list.reset_actual();



        //Helping pointer
        ENEMY *enemy;

        //Do the loop
        while((enemy = enemies_list.get_obj()) != NULL)
        {
            int attack = enemy->checkAttackingCollision(box_player, player_x, player_y, player_frame);
            
            if(attack > player_attacked) player_attacked = attack;

            //Next enemy
            enemies_list.next();
        }
    }
    
    return(player_attacked);
}



//For player pistol. Return the x of the closest zombie or police zombie
//to the left or right of the target x
int ENEMIES_MANAGER::checkClosestLeft(int target_x)
{
    int closest_x = -1;
    
    //No elements, no loop
    if(enemies_list.get_elements() != 0)
    {
        //Needs to start from the beggining of the list.
        enemies_list.reset_actual();



        //Helping pointer
        ENEMY *enemy;

        //Do the loop
        while((enemy = enemies_list.get_obj()) != NULL)
        {
            if(enemy->x < target_x)
            {
                if(closest_x == -1)
                {
                    closest_x = enemy->x;
                } else {
                    if(enemy->x >= closest_x) closest_x = enemy->x;
                }
            }
            
            

            //Next enemy
            enemies_list.next();
        }
    }
    
    return(closest_x);
}



int ENEMIES_MANAGER::checkClosestRight(int target_x)
{
    int closest_x = -1;
    
    //No elements, no loop
    if(enemies_list.get_elements() != 0)
    {
        //Needs to start from the beggining of the list.
        enemies_list.reset_actual();



        //Helping pointer
        ENEMY *enemy;

        //Do the loop
        while((enemy = enemies_list.get_obj()) != NULL)
        {
            if(enemy->x > target_x)
            {
                if(closest_x == -1)
                {
                    closest_x = enemy->x;
                } else {
                    if(enemy->x <= closest_x) closest_x = enemy->x;
                }
            }
            
            

            //Next enemy
            enemies_list.next();
        }
    }
    
    return(closest_x);
}



//Now return a enemy pointer
ENEMY *ENEMIES_MANAGER::checkClosestEnemyLeft(int target_x)
{
    int closest_x = -1;
    ENEMY *closest_enemy = NULL;
    
    //Enemies list (on wagon)
    
    int player_attacked = ENEMY_ATTACK_NONE;
    
    //No elements, no loop
    if(enemies_list.get_elements() != 0)
    {
        //Needs to start from the beggining of the list.
        enemies_list.reset_actual();



        //Helping pointer
        ENEMY *enemy;

        //Do the loop
        while((enemy = enemies_list.get_obj()) != NULL)
        {
            if(enemy->x < target_x)
            {
                if(closest_x == -1)
                {
                    closest_x = enemy->x;
                    closest_enemy = enemy;
                } else {
                    if(enemy->x >= closest_x)
                    {
                        closest_x = enemy->x;
                        closest_enemy = enemy;
                    }
                }
            }
            
            

            //Next enemy
            enemies_list.next();
        }
    }
    
    return(closest_enemy);
}



ENEMY *ENEMIES_MANAGER::checkClosestEnemyRight(int target_x)
{
    int closest_x = -1;
    ENEMY *closest_enemy = NULL;
    
    //No elements, no loop
    if(enemies_list.get_elements() != 0)
    {
        //Needs to start from the beggining of the list.
        enemies_list.reset_actual();



        //Helping pointer
        ENEMY *enemy;

        //Do the loop
        while((enemy = enemies_list.get_obj()) != NULL)
        {
            if(enemy->x > target_x)
            {
                if(closest_x == -1)
                {
                    closest_x = enemy->x;
                    closest_enemy = enemy;
                } else {
                    if(enemy->x <= closest_x)
                    {
                        closest_x = enemy->x;
                        closest_enemy = enemy;
                    }
                }
            }
            
            

            //Next enemy
            enemies_list.next();
        }
    }
    
    return(closest_enemy);
}



void ENEMIES_MANAGER::remove_enemy()
{
    //Remove the enemy
    enemies_list.delete_obj();

    ++enemies_destroyed;
}



//Clear all enemies from the lists
void ENEMIES_MANAGER::clear()
{
    enemies_list.clear();
    enemies_on_station.clear();
}



//Draw all enemies inside the wagon
void ENEMIES_MANAGER::drawInsideWagon(BITMAP *buffer, int camera_x, int camera_y, int zoom)
{
    //Draw the enemies inside the wagon
    
    //No elements, no drawing
    if(enemies_list.get_elements() == 0) return;

    //Needs to start from the beggining of the list.
    enemies_list.reset_actual();



    //Helping pointer
    ENEMY *enemy;

    //Do the loop
    while((enemy = enemies_list.get_obj()) != NULL)
    {
        //Draw this enemy
        enemy->draw(buffer, camera_x, camera_y, zoom);

        //Next enemy
        enemies_list.next();
    }
}



//Draw all enemies at the station
void ENEMIES_MANAGER::drawAtStation(BITMAP *buffer, int camera_x, int camera_y, int zoom)
{
    //Draw the enemies on station
    
    //No elements, no drawing
    if(enemies_on_station.get_elements() == 0) return;

    //Needs to start from the beggining of the list.
    enemies_on_station.reset_actual();



    //Helping pointer
    ENEMY *enemy;

    //Do the loop
    while((enemy = enemies_on_station.get_obj()) != NULL)
    {
        //Draw this enemy
        enemy->draw(buffer, camera_x, camera_y, zoom);

        //Next enemy
        enemies_on_station.next();
    }
}



//Destructor
ENEMIES_MANAGER::~ENEMIES_MANAGER()
{
    //Clear the lists
    enemies_on_station.clear();
    enemies_list.clear();
    
    //Unload the bases
    base_fly.unload();
    base_fly_half.unload();
    
    base_zombie_standing.unload();
    base_zombie_walking_right.unload();
    
    base_zombie_standing_half.unload();
    base_zombie_walking_right_half.unload();
    
    base_zombie_dead.unload();
    base_zombie_dead_half.unload();
    
    base_fat_zombie_door.unload();
    base_fat_zombie_door_half.unload();
    base_fat_zombie_standing.unload();
    base_fat_zombie_standing_half.unload();
    
    
    
    //Unload tha collision data
    box_fly.unload();
    
    box_zombie_standing.unload();
    box_zombie_walking_left.unload();
    box_zombie_walking_right.unload();
    
    box_fat_zombie_standing.unload();
}

