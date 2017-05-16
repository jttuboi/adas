#include"enemy.h"
#include <cstdio>



//Constructor
ENEMY::ENEMY()
{
    setPosition(0, 0);
    setLife(true);
    setImmortal(false);
    setFlagStart(false);
    setDelay(0);
    sprite = NULL;
    
    setCollisionData(NULL);
    
    dead_event = false;
    dead_time_counter = DEAD_TIME_STAY;
    
    enemy_attacking_frame = false;
    
    enemy_attack_wait = 0;
}
    


//Set the enemy position
void ENEMY::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}



//Set a target position
void ENEMY::setTargetPosition(int target_x, int target_y)
{
    this->target_x = target_x;
    this->target_y = target_y;
}



//Set him as alive or dead
void ENEMY::setLife(bool alive)
{
    this->alive = alive;
}



//Set if the enemy is inside the wagon
void ENEMY::setFlagStart(bool flag_start)
{
    this->flag_start = flag_start;
}



//Set a start delay for the enemy (time before entering the wagon
void ENEMY::setDelay(int delay)
{
    this->delay = delay;
}



//Set a collision data address
void ENEMY::setCollisionData(BOX_COLLISION_DATA *collision_data)
{
    this->collision_data = collision_data;
}



//Set if the enemy is immortal
void ENEMY::setImmortal(bool immortal)
{
    this->immortal = immortal;
}



//Return the delay time
int ENEMY::getDelay()
{
    return(delay);
}



//Check collision againt a door
bool ENEMY::checkSideDoorCollision(BOX_COLLISION_DATA *box_door, int door_x, int door_y)
{
    if(collision_data == NULL || sprite == NULL) return(false);
    
    bool collided = false;
   //Check the collision
    while(check_collision(collision_data, x, y, sprite->frame,
                          box_door, door_x, door_y, 0))
    {
        //Translate the player to a not collision position
        if(x < door_x) x -= 2; else x += 2;
        collided = true;
    }
    
    return(collided);
}



//Check for collision against the player
bool ENEMY::checkPlayerCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame, int player_attack_type, bool attacking_frame)
{
    //Abort if we cant check
    if(collision_data == NULL || sprite == NULL) return(false);
    
    if(dead_event) return(false);
    
    //Check if the data collides
    return(check_collision(box_player, player_x, player_y, player_frame,
                           collision_data, x, y, sprite->frame));
}



//Check if the enemy is attacking the player
int ENEMY::checkAttackingCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame)
{
    //Abort if we cant check
    if(collision_data == NULL || sprite == NULL) return(false);
    
    if(dead_event || !enemy_attacking_frame) return(false);
    
    //Check if the data collides
    return(check_collision(box_player, player_x, player_y, player_frame,
                           collision_data, x, y, sprite->frame));
}



//Start the enemy
void ENEMY::start()
{
    flag_start = true;
}




//Logic loop
void ENEMY::logicLoop(int player_x, int player_y)
{
    if(enemy_attack_wait > 0) --enemy_attack_wait;
}



//Draw the enemy
void ENEMY::draw(BITMAP *buffer, int camera_x, int camera_y, int zoom)
{
    sprite->draw(buffer, x - camera_x, y - camera_y);
}



//What happens when the enemy dies?
void ENEMY::die()
{
    //Dead
    if(!immortal)
    {
        if(!dead_event) dead_event = true;
        
        --dead_time_counter;
        if(dead_time_counter <= 0) alive = false;
    }
}



//Destructor
ENEMY::~ENEMY()
{
    //Do nothing
}







//Constructor
ENEMY_FLY::ENEMY_FLY()
{
    //
}



//Set the sprite
void ENEMY_FLY::setSprite(BASE_SPRITE *base_fly, BASE_SPRITE *base_fly_half)
{
    spr_fly = base_fly;
    spr_fly.autoset_origins();
    
    spr_fly_half = base_fly_half;
    spr_fly_half.autoset_origins();
    
    sprite = &spr_fly;
}



//Set the collision data
void ENEMY_FLY::setFlyCollisionData(BOX_COLLISION_DATA *coll_fly)
{
    this->coll_fly = coll_fly;
    
    setCollisionData(coll_fly);
}



//Check for collision against the player
bool ENEMY_FLY::checkPlayerCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame, int player_attack_type, bool attacking_frame)
{
    if(ENEMY::checkPlayerCollision(box_player, player_x, player_y, player_frame, player_attack_type, attacking_frame))
    {
        //If player is attacking CQC and this is an active frame
        if(player_attack_type == PLAYER_ATTACK_TYPE_CQC && attacking_frame)
        {
            alive = false;
        }
    }
    
    return(false);
}



//Check if the enemy is attacking the player
int ENEMY_FLY::checkAttackingCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame)
{
    if(ENEMY::checkAttackingCollision(box_player, player_x, player_y, player_frame))
    {
        return(ENEMY_ATTACK_STAMINA);
    }
    
    return(ENEMY_ATTACK_NONE);
}


//Do the logic loop
void ENEMY_FLY::logicLoop(int player_x, int player_y)
{
	/*direction
		north = 0
		northeast = 1
		east = 2
		southeast = 3
		south = 4
		south_west = 5
		west = 6
		north_west 7

		7 0 1
		6 * 2
		5 4 3
	*/
	if(!flag_start)
	{
	    setTargetPosition(x + (rand()%4) - 4, y + (rand()%4) - 4);
	} else {
	    enemy_attacking_frame = true;
	    
	    if(abs(x - player_x) < 300)
	    {
	        setTargetPosition(player_x, player_y + 30);
	    } else {
	        int y_pos = y + (rand()%4) - 4;
	        if(y_pos < 70) y_pos = 70;
	        if(y_pos > 130) y_pos = 130;
	        setTargetPosition(x + (rand()%4) - 4, y_pos);
	    }
        
        int direction;
        int direction1, direction2, direction3;
        
        int distance_x = target_x - x;
        int distance_y = target_y - y;

        // More likely to go to north, northeast and east 	
        if ((distance_x > 0) && (distance_y < 0))
        {
            direction1 = 0; direction2 = 1; direction3 = 2;
        }
        
        // More likely to go to east, southeast and south 
        if ((distance_x > 0) && (distance_y > 0))
        {
            direction1 = 2; direction2 = 3; direction3 = 4;
        }
        
        // More likely to go to south, south_west, west	
        if ((distance_x < 0) && (distance_y > 0))
        {
            direction1 = 4; direction2 = 5; direction3 = 6;
        }
        
        // More likely to go to west, north_west, north
        if ((distance_x < 0) && (distance_y < 0))
        {
            direction1 = 6; direction2 = 7; direction3 = 0;
        }

        // Randomize direction
        direction = abs(rand()%8);
        
        // If not equal to the closer directions, Randomize direction again
        if ((direction != direction1) && (direction != direction2) && (direction != direction3))
            direction = abs(rand()%8);

        switch (direction)
        {
            case 0:
            {
                y -= 4;
            } break;
            case 1:
            {
                y -= 4; x += 2;
            } break;
            case 2:
            {
                x += 2;
            } break;
            case 3:
            {
                y += 4; x += 2;
            } break;
            case 4:
            {
                y += 4;
            } break;
            case 5:
            {
                y += 4; x -= 2;
            } break;
            case 6:
            {
                x -= 2;
            } break;
            case 7:
            {
                y -= 4; x -= 2;
            } break;
        }
	}
}



//Draw the enemy
void ENEMY_FLY::draw(BITMAP *buffer, int camera_x, int camera_y, int zoom)
{
    switch(zoom)
    {
        case ZOOM_NONE:
        {
            spr_fly.draw(buffer, x - camera_x, y - camera_y);
        } break;
        
        
        
        case ZOOM_OUT:
        {
            spr_fly_half.draw(buffer, (x/2) - camera_x, (y/2) - camera_y);
        } break;
    }
}



//Destructor
ENEMY_FLY::~ENEMY_FLY()
{
	//
}







//Constructor
ENEMY_ZOMBIE::ENEMY_ZOMBIE()
{
    delay_random = 0;
}



//Set the sprite
void ENEMY_ZOMBIE::setSprite(BASE_SPRITE *base_standing, BASE_SPRITE *base_right, BASE_SPRITE *base_zombie_dead, BASE_SPRITE *base_zombie_attacking, BASE_SPRITE *base_standing_half, BASE_SPRITE *base_right_half, BASE_SPRITE *base_zombie_dead_half, BASE_SPRITE *base_zombie_attacking_half)
{
	spr_zombie_standing = base_standing;
	spr_zombie_right = base_right; 
	
	spr_zombie_standing.autoset_x_origins();
	spr_zombie_right.autoset_x_origins();
	
	spr_zombie_dead = base_zombie_dead;
	spr_zombie_dead.autoset_x_origins();
	spr_zombie_dead.y_origins[0] = 160;
	
	spr_zombie_attacking = base_zombie_attacking;
	spr_zombie_attacking.autoset_x_origins();
	
	spr_zombie = &spr_zombie_standing;
	
	
	
	spr_zombie_standing_half = base_standing_half;
	spr_zombie_right_half = base_right_half; 
	
	spr_zombie_standing_half.autoset_x_origins();
	spr_zombie_right_half.autoset_x_origins();
	
	spr_zombie_dead_half = base_zombie_dead_half;
	spr_zombie_dead_half.autoset_x_origins();
	
	spr_zombie_attacking_half = base_zombie_attacking_half;
	spr_zombie_attacking_half.autoset_x_origins();
}



//Set the collision data
void ENEMY_ZOMBIE::setZombieCollisionData(BOX_COLLISION_DATA *coll_zombie_standing, BOX_COLLISION_DATA *coll_zombie_left, BOX_COLLISION_DATA *coll_zombie_right, BOX_COLLISION_DATA *coll_zombie_attacking_left, BOX_COLLISION_DATA *zombie_attacking_right)
{
    this->coll_zombie_standing = coll_zombie_standing;
    this->coll_zombie_left = coll_zombie_left;
    this->coll_zombie_right = coll_zombie_right;
    this->coll_zombie_attacking_right = coll_zombie_attacking_right;
    this->coll_zombie_attacking_left = coll_zombie_attacking_left;
    
    setCollisionData(coll_zombie_standing);
}



//Check for collision against the player
bool ENEMY_ZOMBIE::checkPlayerCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame, int player_attack_type, bool attacking_frame)
{
    if(dead_event) return(false);
    
    if(ENEMY::checkPlayerCollision(box_player, player_x, player_y, player_frame, player_attack_type, attacking_frame))
    {
        //If player is attacking CQC and this is an active frame
        if(player_attack_type == PLAYER_ATTACK_TYPE_CQC && attacking_frame)
        {
            die();
            
            spr_zombie = &spr_zombie_dead;
            y += 165;
            
            return(true);
        }
    }
    
    return(false);
}

    

//Do the logic loop
void ENEMY_ZOMBIE::logicLoop(int player_x, int player_y)
{
    if(dead_event)
    {
        die();
        return;
    }
    
    target_x = player_x;
    target_y = player_y;
    
	if (flag_start)
	{
		if (delay > 0)
		{
			--delay;
		}
		else
		{
			/*direction
			left = 0
			standing = 1
			right = 2
			*/
		
			int distance_x = target_x - x;
			
			// If the distance between the player and the zombie is -400 < distance < 400,
			if ((distance_x >= -300) && (distance_x <= 300))
			{    // zombie chasing the player 
				if (distance_x < 0)
					direction = 0;
				if (distance_x == 0)
					direction = 1;
				if (distance_x > 0)
					direction = 2;
			}
			else
			{    // zombie walk randomly
				if (delay_random >= 100)
				{
					// Randomize direction
					direction = abs(rand()%3);
					delay_random = 0;
				}
				delay_random++;
			}
			
			switch (direction)
			{
				case 0:
				{
					x--;
					spr_zombie = &spr_zombie_right;
					setCollisionData(coll_zombie_left);
					
					spr_flag = true;
				} break;
				case 1:
				{
					spr_zombie = &spr_zombie_standing;
					setCollisionData(coll_zombie_standing);
					
					spr_flag = false;
				} break;
				case 2:
				{
					x++;
					spr_zombie = &spr_zombie_right;
					setCollisionData(coll_zombie_right);
					
					spr_flag = false;
				} break;
			}
		}
	}
	if (spr_flag != spr_zombie->hflip)
	{
		spr_zombie->hflip = spr_flag;
		spr_zombie->update();
	}
	spr_zombie->logic_loop();
	
	sprite = spr_zombie;
}



//Draw the enemy
void ENEMY_ZOMBIE::draw(BITMAP *buffer, int camera_x, int camera_y, int zoom)
{
    switch(zoom)
    {
        case ZOOM_NONE:
        {
            spr_zombie->draw(buffer, x - camera_x, y - camera_y);
        } break;
        
        
        
        case ZOOM_OUT:
        {
            SPRITE *temp;
            
            if(spr_zombie == &spr_zombie_standing)
            {
                temp = &spr_zombie_standing_half;
            }
            
            if(spr_zombie == &spr_zombie_dead)
            {
                temp = &spr_zombie_dead_half;
            }
            
            if(spr_zombie == &spr_zombie_right)
            {
                temp = &spr_zombie_right_half;
                temp->hflip = spr_zombie_right.hflip;
                temp->update();
                temp->set_frame(spr_zombie_right.frame);
            }
            
            temp->draw(buffer, (x/2) - camera_x, (y/2) - camera_y);
        } break;
    }
}



//Destructor
ENEMY_ZOMBIE::~ENEMY_ZOMBIE()
{
	//
}










//Constructor
ENEMY_POLICE_ZOMBIE::ENEMY_POLICE_ZOMBIE()
{
    delay_random = 0;
}



//Set the sprite
void ENEMY_POLICE_ZOMBIE::setSprite(BASE_SPRITE *base_standing, BASE_SPRITE *base_right, BASE_SPRITE *base_zombie_dead, BASE_SPRITE *base_standing_half, BASE_SPRITE *base_right_half, BASE_SPRITE *base_zombie_dead_half)
{
	spr_zombie_standing = base_standing;
	spr_zombie_right = base_right; 
	
	spr_zombie_standing.autoset_x_origins();
	spr_zombie_right.autoset_x_origins();
	
	spr_zombie_dead = base_zombie_dead;
	spr_zombie_dead.autoset_x_origins();
	spr_zombie_dead.y_origins[0] = 160;
	
	spr_zombie = &spr_zombie_standing;
	
	
	
	spr_zombie_standing_half = base_standing_half;
	spr_zombie_right_half = base_right_half; 
	
	spr_zombie_standing_half.autoset_x_origins();
	spr_zombie_right_half.autoset_x_origins();
	
	spr_zombie_dead_half = base_zombie_dead_half;
	spr_zombie_dead_half.autoset_x_origins();
	spr_zombie_dead_half.y_origins[0] = 80;
}



//Set the collision data
void ENEMY_POLICE_ZOMBIE::setZombieCollisionData(BOX_COLLISION_DATA *coll_zombie_standing, BOX_COLLISION_DATA *coll_zombie_left, BOX_COLLISION_DATA *coll_zombie_right)
{
    this->coll_zombie_standing = coll_zombie_standing;
    this->coll_zombie_left = coll_zombie_left;
    this->coll_zombie_right = coll_zombie_right;
    
    setCollisionData(coll_zombie_standing);
}



//Check for collision against the player
bool ENEMY_POLICE_ZOMBIE::checkPlayerCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame, int player_attack_type, bool attacking_frame)
{
    if(dead_event) return(false);
    
    if(ENEMY::checkPlayerCollision(box_player, player_x, player_y, player_frame, player_attack_type, attacking_frame))
    {
        //If player is attacking CQC and this is an active frame
        if(player_attack_type == PLAYER_ATTACK_TYPE_CQC && attacking_frame)
        {
            die();
            
            spr_zombie = &spr_zombie_dead;
            y += 165;
            
            return(true);
        }
    }
    
    return(false);
}

    

//Do the logic loop
void ENEMY_POLICE_ZOMBIE::logicLoop(int player_x, int player_y)
{
    if(dead_event)
    {
        die();
        return;
    }
    
    target_x = player_x;
    target_y = player_y;
    
	if (flag_start)
	{
		if (delay > 0)
		{
			--delay;
		}
		else
		{
			/*direction
			left = 0
			standing = 1
			right = 2
			*/
		
			int distance_x = target_x - x;
			
			// If the distance between the player and the zombie is -400 < distance < 400,
			if ((distance_x >= -650) && (distance_x <= 300))
			{    // zombie chasing the player 
				if (distance_x < 0)
					direction = 0;
				if (distance_x == 0)
					direction = 1;
				if (distance_x > 0)
					direction = 2;
			}
			else
			{    // zombie walk randomly
				if (delay_random >= 100)
				{
					// Randomize direction
					direction = abs(rand()%3);
					delay_random = 0;
				}
				delay_random++;
			}
			
			switch (direction)
			{
				case 0:
				{
					x--;
					spr_zombie = &spr_zombie_right;
					setCollisionData(coll_zombie_left);
					
					spr_flag = true;
				} break;
				case 1:
				{
					spr_zombie = &spr_zombie_standing;
					setCollisionData(coll_zombie_standing);
					
					spr_flag = false;
				} break;
				case 2:
				{
					x++;
					spr_zombie = &spr_zombie_right;
					setCollisionData(coll_zombie_right);
					
					spr_flag = false;
				} break;
			}
		}
	}
	if (spr_flag != spr_zombie->hflip)
	{
		spr_zombie->hflip = spr_flag;
		spr_zombie->update();
	}
	spr_zombie->logic_loop();
	
	sprite = spr_zombie;
}



//Draw the enemy
void ENEMY_POLICE_ZOMBIE::draw(BITMAP *buffer, int camera_x, int camera_y, int zoom)
{
    switch(zoom)
    {
        case ZOOM_NONE:
        {
            spr_zombie->draw(buffer, x - camera_x, y - camera_y);
        } break;
        
        
        
        case ZOOM_OUT:
        {
            SPRITE *temp;
            
            if(spr_zombie == &spr_zombie_standing)
            {
                temp = &spr_zombie_standing_half;
            }
            
            if(spr_zombie == &spr_zombie_dead)
            {
                temp = &spr_zombie_dead_half;
            }
            
            if(spr_zombie == &spr_zombie_right)
            {
                temp = &spr_zombie_right_half;
                temp->hflip = spr_zombie_right.hflip;
                temp->update();
                temp->set_frame(spr_zombie_right.frame);
            }
            
            temp->draw(buffer, (x/2) - camera_x, (y/2) - camera_y);
        } break;
    }
}



//Destructor
ENEMY_POLICE_ZOMBIE::~ENEMY_POLICE_ZOMBIE()
{
	//
}









//Constructor
ENEMY_FAT_ZOMBIE::ENEMY_FAT_ZOMBIE()
{
    //He is immortal
    immortal = true;
}



//Set the sprite
void ENEMY_FAT_ZOMBIE::setSprite(BASE_SPRITE *base_zombie_fat_standing, BASE_SPRITE *base_zombie_fat_door, BASE_SPRITE *base_zombie_fat_standing_half, BASE_SPRITE *base_zombie_fat_door_half)
{
    this->spr_zombie_fat_standing = base_zombie_fat_standing;
    this->spr_zombie_fat_door = base_zombie_fat_door;
    
    this->spr_zombie_fat_standing_half = base_zombie_fat_standing_half;
    this->spr_zombie_fat_door_half = base_zombie_fat_door_half;
    
    this->spr_zombie_fat_standing.autoset_x_origins();
    this->spr_zombie_fat_door.autoset_x_origins();
    
    this->spr_zombie_fat_standing_half.autoset_x_origins();
    this->spr_zombie_fat_door_half.autoset_x_origins();
    
    sprite = &spr_zombie_fat_standing;
    sprite_draw = sprite;
}



//Set the collision data
void ENEMY_FAT_ZOMBIE::setZombieCollisionData(BOX_COLLISION_DATA *coll_zombie_fat_standing)
{
    this->coll_zombie_fat_standing = coll_zombie_fat_standing;
    this->coll_zombie_fat_standing->set_origins(&spr_zombie_fat_standing);
    setCollisionData(coll_zombie_fat_standing);
}



//Check for collision against the player
bool ENEMY_FAT_ZOMBIE::checkPlayerCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame, int player_attack_type, bool attacking_frame)
{
    return(false);
}



//Check if the enemy is attacking the player
int ENEMY_FAT_ZOMBIE::checkAttackingCollision(BOX_COLLISION_DATA *box_player, int player_x, int player_y, int player_frame)
{
    if(ENEMY::checkAttackingCollision(box_player, player_x, player_y, player_frame))
    {
        return(ENEMY_ATTACK_DEATH);
    }
    
    return(ENEMY_ATTACK_NONE);
}



//Logic loop
void ENEMY_FAT_ZOMBIE::logicLoop(int player_x, int player_y)
{
    if(delay > 0 && flag_start)
    {
        --delay;
        
        sprite_draw = &spr_zombie_fat_door;
        sprite_draw->logic_loop();
        enemy_attacking_frame = false;
    } else {
        sprite_draw = &spr_zombie_fat_standing;
        enemy_attacking_frame = true;
    }
}



//Draw the enemy
void ENEMY_FAT_ZOMBIE::draw(BITMAP *buffer, int camera_x, int camera_y, int zoom)
{
    switch(zoom)
    {
        case ZOOM_NONE:
        {
            sprite_draw->draw(buffer, x - camera_x, y - camera_y);
        } break;
        
        
        
        case ZOOM_OUT:
        {
            SPRITE *temp = NULL;
            
            if(sprite_draw == &spr_zombie_fat_standing)
            {
                temp = &spr_zombie_fat_standing_half;
            }
            
            if(sprite_draw == &spr_zombie_fat_door)
            {
                temp = &spr_zombie_fat_door_half;
            }
            
            temp->draw(buffer, (x/2) - camera_x, (y/2) - camera_y);
        } break;
    }
}



//Destructor
ENEMY_FAT_ZOMBIE::~ENEMY_FAT_ZOMBIE()
{
    spr_zombie_fat_door.base_sprite_disconnect();
    spr_zombie_fat_standing.base_sprite_disconnect();
    
    spr_zombie_fat_door_half.base_sprite_disconnect();
    spr_zombie_fat_standing_half.base_sprite_disconnect();
}
