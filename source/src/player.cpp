#include"player.h"

//Yohai the constructor
PLAYER::PLAYER()
{
    //Load the bases
    spr_base_player_standing.load_dat("media/player_standing.dat");
    spr_base_player_window.load_dat("media/player_window.dat");
    spr_base_player_walking_right_suitcase.load_dat("media/player_walking_suitcase_right.dat");
    spr_base_player_suitcase_attack.load_dat("media/player_suitcase_attack.dat");
    spr_base_player_side_door.load_dat("media/player_side_door.dat");
    spr_base_player_pistol.load_dat("media/player_walking_pistol.dat");
    
    //Connect with the bases
    spr_player_standing = spr_base_player_standing;
    spr_player_standing.autoset_x_origins();
    
    spr_player_window = spr_base_player_window;
    spr_player_window.autoset_x_origins();
    spr_player_window.set_speed(2.0);
    
    spr_player_walking_right_suitcase = spr_base_player_walking_right_suitcase;
    spr_player_walking_right_suitcase.autoset_x_origins();
    
    spr_player_suitcase_attack = spr_base_player_suitcase_attack;
    spr_player_suitcase_attack.autoset_x_origins();
    
    spr_player_side_door = spr_base_player_side_door;
    spr_player_side_door.autoset_x_origins();
    
    spr_player_pistol = spr_base_player_pistol;
    spr_player_pistol.autoset_x_origins();
    
    
    
    
    //Now the scaled bases of the sprites (for zoom out)
    spr_base_player_standing_half.load_dat_halfed("media/player_standing.dat");
    spr_base_player_window_half.load_dat_halfed("media/player_window.dat");
    spr_base_player_walking_right_suitcase_half.load_dat_halfed("media/player_walking_suitcase_right.dat");
    spr_base_player_suitcase_attack_half.load_dat_halfed("media/player_suitcase_attack.dat");
    spr_base_player_side_door_half.load_dat_halfed("media/player_side_door.dat");
    spr_base_player_pistol_half.load_dat_halfed("media/player_walking_pistol.dat");
    
    
    //Connect with the bases
    spr_player_standing_half = spr_base_player_standing_half;
    spr_player_standing_half.autoset_x_origins();
    
    spr_player_window_half = spr_base_player_window_half;
    spr_player_window_half.autoset_x_origins();
    spr_player_window_half.set_speed(2.0);
    
    spr_player_walking_right_suitcase_half = spr_base_player_walking_right_suitcase_half;
    spr_player_walking_right_suitcase_half.autoset_x_origins();
    
    spr_player_suitcase_attack_half = spr_base_player_suitcase_attack_half;
    spr_player_suitcase_attack_half.autoset_x_origins();
    
    spr_player_side_door_half = spr_base_player_side_door_half;
    spr_player_side_door_half.autoset_x_origins();
    
    spr_player_pistol_half = spr_base_player_pistol_half;
    spr_player_pistol_half.autoset_x_origins();
    
    
    
    
    
    
    //Default to draw sprite
    spr_to_draw = &spr_player_standing;
    
    
    
    //About actions
    action = false;
    action_finished = false;
    action_type = PLAYER_ACTION_NONE;
    
    
    
    //Direction?
    direction = NONE;
    
    //Not attacking
    attacking = false;
    
    //No zoom yet
    zoom = ZOOM_NONE;
    
    
    
    //Set the collision data
    setCollisionData();
    
    //Initial attack type
    attack_type = PLAYER_ATTACK_TYPE_NONE;
    
    //Reset the status for post mortem
    resetStatus();
    
    
    //Stamina at right stats
    stamina = 100;
    stamina_effect = 100;
    
    
    
    //Weapon
    player_weapon = PLAYER_WEAPON_SUITCASE;
    
    
    //Allow the playing
    unlockControls();
    
    //No gunshots
    gunshot = false;
}



//Reset the status
void PLAYER::resetStatus()
{
    times_changed_wagons = 0;
    suitcase_attacks = 0;
    suitcase_hits = 0;
}



//Update some status values
void PLAYER::changedWagon()
{
    ++times_changed_wagons;
}

void PLAYER::suitcaseHit()
{
    ++suitcase_hits;
}



//Set all the collision data
void PLAYER::setCollisionData()
{
    //Set all the collision boxes. Way to go!
    coll_player_standing.set(&spr_base_player_standing, true);
    coll_player_window.set(&spr_base_player_window, true);
    coll_player_walking_suitcase_right.set(&spr_base_player_walking_right_suitcase, true);
    coll_player_suitcase_attack_right.set(&spr_base_player_suitcase_attack, true);
    coll_player_right_door.set(&spr_base_player_side_door, true);
    coll_player_pistol_right.set(&spr_base_player_pistol, true);
    
    coll_player_suitcase_attack_left.setInverted(&spr_base_player_suitcase_attack, true);
    coll_player_walking_suitcase_left.setInverted(&spr_base_player_walking_right_suitcase, true);
    coll_player_left_door.setInverted(&spr_base_player_side_door, true);
    coll_player_pistol_left.setInverted(&spr_base_player_pistol, true);
    
    
    
    //Set the boxes origins
    coll_player_standing.set_origins(&spr_player_standing);
    coll_player_window.set_origins(&spr_player_window);
    coll_player_walking_suitcase_right.set_origins(&spr_player_walking_right_suitcase);
    coll_player_suitcase_attack_right.set_origins(&spr_player_suitcase_attack);
    coll_player_right_door.set_origins(&spr_player_side_door);
    coll_player_pistol_right.set_origins(&spr_player_pistol);
    
    coll_player_suitcase_attack_left.set_origins(&spr_player_suitcase_attack);
    coll_player_walking_suitcase_left.set_origins(&spr_player_walking_right_suitcase);
    coll_player_left_door.set_origins(&spr_player_side_door);
    coll_player_pistol_left.set_origins(&spr_player_pistol);
    
    //A initial collision box
    collision_data = &coll_player_standing;
}



//Set a player position
void PLAYER::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}



//Set the area limit for the player
void PLAYER::setAreaLimit(int x_min_limit, int x_max_limit)
{
    this->x_min_limit = x_min_limit;
    this->x_max_limit = x_max_limit;
}



//Set player trying one action
void PLAYER::setAction(bool action)
{
    this->action = action;
}



//Set an action type for the player
//(i.e. open window)
void PLAYER::setActionType(int action_type)
{
    action = false;
    this->action_type = action_type;
    
    switch(action_type)
    {
        case WAGON_ACTION_WINDOW:
        {
            //Set the collision data
            collision_data = &coll_player_window;
            
            //Draw the right image
            spr_to_draw = &spr_player_window;
            spr_player_window.set_frame(0);
            spr_to_draw->animation_ended = false;
        } break; //end of case wagon action window
        
        
        
        case WAGON_ACTION_LEFT_DOOR:
        {
            //Set the collision data
            collision_data = &coll_player_left_door;
            
            //Draw the right image
            spr_to_draw = &spr_player_side_door;
            spr_to_draw->hflip = true;
            spr_to_draw->update();
            spr_to_draw->set_frame(0);
            spr_to_draw->animation_ended = false;
        } break; //end of case wagon action left door
        
        
        
        case WAGON_ACTION_RIGHT_DOOR:
        {
            //Set the collision data
            collision_data = &coll_player_right_door;
            
            //Draw the right image
            spr_to_draw = &spr_player_side_door;
            spr_to_draw->hflip = false;
            spr_to_draw->update();
            spr_to_draw->set_frame(0);
            spr_to_draw->animation_ended = false;
        } break; //end of case wagon action right door
    }
}



//Set the player zoom
void PLAYER::setZoom(int zoom)
{
    this->zoom = zoom;
}



//Locks and unlocks the control
void PLAYER::lockControls()
{
    controls_locked = true;
}



void PLAYER::unlockControls()
{
    controls_locked = false;
}



//Check if the player is trying one action
bool PLAYER::getAction()
{
    return(action);
}



//Get the action type
int PLAYER::getActionType()
{
    return(action_type);
}




//Allows to check if an action has finished this frame
bool PLAYER::getActionFinished()
{
    return(action_finished);
}



//Get the player attack style
int PLAYER::getAttackType()
{
    return(attack_type);
}



//Check if the player is in an attacking frame
bool PLAYER::getAttackingFrame()
{
    return(attacking_frame);
}



//Player received an stamina attack
void PLAYER::staminaAttack()
{
    stamina -= 4*(PLAYER_STAMINA_RECOVER);
}



//If the player did shoot with a gun
bool PLAYER::getPlayerGunShot()
{
    return(gunshot);
}



//Check player collision against a side door
void PLAYER::checkSideDoorCollision(BOX_COLLISION_DATA *box_door, int door_x, int door_y)
{
    //Check the collision
    while(check_collision(collision_data, x, y, spr_to_draw->frame,
                          box_door, door_x, door_y, 0))
    {
        //Translate the player to a not collision position
        if(x < door_x) x -= 5; else x += 5;
    }
}



//Logic loop
void PLAYER::logicLoop()
{
    //Stop the action finished, if there is one
    if(action_finished)
    {
        action_finished = false;
        action_type = PLAYER_ACTION_NONE;
    }
    
    
    
    //Reset attacking frame
    attacking_frame = false;
    
    
    
    //Perharps the end of a gunshot
    gunshot = false;
    
    
    
    //Recover a bit of stamina
    if(stamina < 0.0f) stamina = 0.0f;
    if(stamina < 100) stamina += PLAYER_STAMINA_RECOVER;
    
    if(stamina_effect > stamina)
    {
        stamina_effect -= PLAYER_STAMINA_RECOVER;
    } else {
        stamina_effect = stamina;
    }
    
    
    
    
    //Only move if no action is being executed
    if(action_type == PLAYER_ACTION_NONE)
    {
        //Cant play? Then cant
        if(controls_locked) return;
        
        if(!attacking)
        {
            //Change weapon if tried
            if(key[KEY_1]) player_weapon = PLAYER_WEAPON_SUITCASE;
            if(key[KEY_2]) player_weapon = PLAYER_WEAPON_PISTOL;
            
            
            
            //Use the sprites of the right weapon
            switch(player_weapon)
            {
                case PLAYER_WEAPON_SUITCASE:
                {
                    if(key[KEY_A])
                    {
                        //Move the player
                        x -= 5;
                        
                        //The right collision data
                        collision_data = &coll_player_walking_suitcase_left;
                        
                        if(spr_to_draw != &spr_player_walking_right_suitcase)
                        {
                            spr_to_draw = &spr_player_walking_right_suitcase;
                            spr_to_draw->set_frame(0);
                        }
                        
                        if(!spr_to_draw->hflip)
                        {
                            spr_to_draw->hflip = true;
                            spr_to_draw->update();
                        }
                        
                        //Run the animation of the player
                        spr_to_draw->logic_loop();
                        
                        direction = LEFT;
                    } else {
                        if(spr_to_draw == &spr_player_walking_right_suitcase && spr_to_draw->hflip)
                        {
                            spr_to_draw->set_frame(0);
                        }
                    }
                    
                    
                    
                    if(key[KEY_D])
                    {
                        //Move the player
                        x += 5;
                        
                        //The correct collision data
                        collision_data = &coll_player_walking_suitcase_right;
                        
                        if(spr_to_draw != &spr_player_walking_right_suitcase) 
                        {
                            spr_to_draw = &spr_player_walking_right_suitcase;
                            spr_to_draw->set_frame(0);
                        }
                        
                        if(spr_to_draw->hflip)
                        {
                            spr_to_draw->hflip = false;
                            spr_to_draw->update();
                        }
                        
                        //Run the animation of the player
                        spr_to_draw->logic_loop();
                        
                        direction = RIGHT;
                    } else {
                        if(spr_to_draw == &spr_player_walking_right_suitcase && !spr_to_draw->hflip)
                        {
                            spr_to_draw->set_frame(0);
                        }
                    }
                } break;
                
                
                
                case PLAYER_WEAPON_PISTOL:
                {
                    if(key[KEY_A])
                    {
                        //Move the player
                        x -= 5;
                        
                        //The right collision data
                        collision_data = &coll_player_pistol_left;
                        
                        if(spr_to_draw != &spr_player_pistol)
                        {
                            spr_to_draw = &spr_player_pistol;
                            spr_to_draw->set_frame(0);
                        }
                        
                        if(!spr_to_draw->hflip)
                        {
                            spr_to_draw->hflip = true;
                            spr_to_draw->update();
                        }
                        
                        //Run the animation of the player
                        spr_to_draw->logic_loop();
                        
                        direction = LEFT;
                    } else {
                        if(spr_to_draw == &spr_player_pistol && spr_to_draw->hflip)
                        {
                            spr_to_draw->set_frame(0);
                        }
                    }
                    
                    
                    
                    if(key[KEY_D])
                    {
                        //Move the player
                        x += 5;
                        
                        //The correct collision data
                        collision_data = &coll_player_pistol_right;
                        
                        if(spr_to_draw != &spr_player_pistol) 
                        {
                            spr_to_draw = &spr_player_pistol;
                            spr_to_draw->set_frame(0);
                        }
                        
                        if(spr_to_draw->hflip)
                        {
                            spr_to_draw->hflip = false;
                            spr_to_draw->update();
                        }
                        
                        //Run the animation of the player
                        spr_to_draw->logic_loop();
                        
                        direction = RIGHT;
                    } else {
                        if(spr_to_draw == &spr_player_pistol && !spr_to_draw->hflip)
                        {
                            spr_to_draw->set_frame(0);
                        }
                    }
                    
                    
                    
                    if(key[KEY_SPACE]) gunshot = true;
                } break;
            }
                
                
                
            //Player wants to attack
            if(key[KEY_SPACE])
            {
                //Attack with current weapon
                switch(player_weapon)
                {
                    case PLAYER_WEAPON_SUITCASE:
                    {
                        //The player is attacking, so block all other movements
                        if(stamina >= PLAYER_STAMINA_SUITCASE)
                        {
                            stamina -= PLAYER_STAMINA_SUITCASE;
                        } else {
                            break;
                        }
                        
                        attacking = true;
                        
                        attack_type = PLAYER_ATTACK_TYPE_CQC;
                        
                        //Set the correct collision data
                        if(direction == LEFT)
                        {
                            collision_data = &coll_player_suitcase_attack_left;
                        } else {
                            collision_data = &coll_player_suitcase_attack_right;
                        }
                        
                        
                        
                        spr_to_draw = &spr_player_suitcase_attack;
                        spr_to_draw->animation_ended = false;
                        
                        spr_to_draw->set_frame(0);
                        
                        //The right direction
                        if(direction == LEFT)
                        {
                            if(!spr_to_draw->hflip)
                            {
                                spr_to_draw->hflip = true;
                                spr_to_draw->update();
                            }
                        } else {
                            if(spr_to_draw->hflip)
                            {
                                spr_to_draw->hflip = false;
                                spr_to_draw->update();
                            }
                        }
                        
                        //One more suitcase attack
                        ++suitcase_attacks;
                        
                        
                        
                        //Quit the function
                        return;
                    } break; //end of case weapon suitcase
                }//End of switch weapon
            } else {
                //Not attacking
                attack_type = PLAYER_ATTACK_TYPE_NONE;
            }
            
            
            
            
            
            
            
            
            //Correct the position, if it outside the area
            if(x < x_min_limit) x = x_min_limit;
            if(x > x_max_limit) x = x_max_limit;
            
            
            
            //Check if the player wants to do some action
            if(key[KEY_E]) action = true; else action = false;
        } else {
            if(!(spr_to_draw->animation_ended))
            {
                spr_to_draw->logic_loop();
                
                switch(player_weapon)
                {
                    case PLAYER_WEAPON_SUITCASE:
                    {
                        //Only attack at the right frame
                        if(spr_to_draw->frame == 4 && spr_to_draw->wait_time == 0) attacking_frame = true;
                    } break;
                }
                
                
                
            } else {
                attacking = false;
                
                //Set the correct image again
                switch(player_weapon)
                {
                    case PLAYER_WEAPON_SUITCASE:
                    {
                        spr_to_draw = &spr_player_walking_right_suitcase;
                        
                        if(direction == LEFT)
                        {
                            collision_data = &coll_player_walking_suitcase_left;
                        } else {
                            collision_data = &coll_player_walking_suitcase_right;
                        }
                    } break;
                    
                    
                    
                    case PLAYER_WEAPON_PISTOL:
                    {
                        spr_to_draw = &spr_player_pistol;
                        
                        if(direction == LEFT)
                        {
                            collision_data = &coll_player_pistol_left;
                        } else {
                            collision_data = &coll_player_pistol_right;
                        }
                    } break;
                }
            }
        }
    } else {
        //Execute the action, goddamit
        
        //Run the animation of the player
        spr_to_draw->logic_loop();
        
        //Check if the action has finished
        if(spr_to_draw->animation_ended)
        {
            //Finished the action
            action_finished = true;
            
            //The right collision data
            collision_data = &coll_player_standing;
            
            
            
            spr_to_draw = &spr_player_standing;
            spr_to_draw->set_frame(0);
        }
    }
}



//Draw stamina bar
void PLAYER::drawStaminaBar(BITMAP *buffer, int x, int y)
{
    //The bars are 400 pixels wide, so 4 pixels per % of stamina
    //Bar's border
    rect(buffer, x - 201, y, x+201, y+11, makecol(255,255,0));
    
    int pos = x - 200;
    
    textout_ex(buffer, font, "STAMINA:", pos, y - 10, makecol(255, 255, 255), -1);
    
    //Stamina effect
    rectfill(buffer, pos, y, (int)(pos + (stamina_effect*4)), y+10, makecol(255,0,0));
    
    //Stamina
    rectfill(buffer, pos, y, (int)(pos + (stamina*4)), y+10, PLAYER_STAMINA_BAR_FULL_COLOR);
}



//Draw the player
void PLAYER::draw(BITMAP *buffer, int camera_x, int camera_y)
{
    //Draw the right sprite, at the right position
    switch(zoom)
    {
        case ZOOM_NONE:
        {
            spr_to_draw->draw(buffer, x - camera_x, y - camera_y);
        } break;
        
        
        
        case ZOOM_OUT:
        {
            SPRITE *temp;
            
            if(spr_to_draw == &spr_player_standing)
            {
                temp = &spr_player_standing_half;
                temp->set_frame(spr_player_standing.frame);
            }
            
            if(spr_to_draw == &spr_player_window)
            {
                temp = &spr_player_window_half;
                temp->set_frame(spr_player_window.frame);
            }
            
            if(spr_to_draw == &spr_player_walking_right_suitcase)
            {
                temp = &spr_player_walking_right_suitcase_half;
                temp->set_frame(spr_player_walking_right_suitcase.frame);
                temp->hflip = spr_player_walking_right_suitcase.hflip;
                temp->update();
            }
            
            if(spr_to_draw == &spr_player_suitcase_attack)
            {
                temp = &spr_player_suitcase_attack_half;
                temp->set_frame(spr_player_suitcase_attack.frame);
                temp->hflip = spr_player_suitcase_attack.hflip;
                temp->update();
            }
            
            if(spr_to_draw == &spr_player_side_door)
            {
                temp = &spr_player_side_door_half;
                temp->set_frame(spr_player_side_door.frame);
                temp->hflip = spr_player_side_door.hflip;
                temp->update();
            }
            
            if(spr_to_draw == &spr_player_pistol)
            {
                temp = &spr_player_pistol_half;
                temp->set_frame(spr_player_pistol.frame);
                temp->hflip = spr_player_pistol.hflip;
                temp->update();
            }
            
            temp->draw(buffer, (x/2) - camera_x, (y/2) - camera_y);
        } break;
    }
    
    
    
    //Draw the stamina bar
    drawStaminaBar(buffer, 400, -camera_y - 20);
}



//Prepare for post mortem
void PLAYER::prepare_post_mortem()
{
    show = 0;
    show_wait = PLAYER_PM_SHOW_WAIT_TIME;
}



//Post mortem logic loop
void PLAYER::post_mortem_logic_loop()
{
    //Logic loop for a nice post mortem thingy
    
    --show_wait;
    if(show_wait < 0 && show <= PLAYER_PM_ITENS)
    {
        ++show;
        
        if(show < PLAYER_PM_ITENS)
        {
            show_wait = PLAYER_PM_SHOW_WAIT_TIME;
        }
    }
}



//Draw post mortem status
void PLAYER::post_mortem_draw(BITMAP *buffer)
{
    //Draw the post mortem
    if(show >= 1)
    {
        textprintf_centre_ex(buffer, font, 400, 200, makecol(255, 255, 255), -1, "Changed wagon: %d times", times_changed_wagons);
    }
    
    if(show >= 2)
    {
        if(times_changed_wagons <= 20) textprintf_centre_ex(buffer, font, 600, 220, makecol(255, 0, 0), -1, "HEAVY STYLE");
        if(times_changed_wagons > 20) textprintf_centre_ex(buffer, font, 600, 220, makecol(255, 0, 0), -1, "INDECISE");
    }
    
    if(show >= 3)
    {
        textprintf_centre_ex(buffer, font, 400, 250, makecol(255, 255, 255), -1, "Suitcase attacks: %d", suitcase_attacks);
    }
    
    if(show >= 4)
    {
        if(suitcase_attacks <= 15) textprintf_centre_ex(buffer, font, 600, 270, makecol(255, 0, 0), -1, "RESERVED");
        if(suitcase_attacks > 15) textprintf_centre_ex(buffer, font, 600, 270, makecol(255, 0, 0), -1, "VIOLENT");
    }
        
    if(show >= 5)
    {
        textprintf_centre_ex(buffer, font, 400, 300, makecol(255, 255, 255), -1, "Suitcase hits: %d", suitcase_hits);
    }
}



//The destructor
PLAYER::~PLAYER()
{
    //Unload the sprites
    spr_player_standing.base_sprite_disconnect();
    spr_player_window.base_sprite_disconnect();
    spr_player_walking_right_suitcase.base_sprite_disconnect();
    spr_player_suitcase_attack.base_sprite_disconnect();
    spr_player_side_door.base_sprite_disconnect();
    spr_player_pistol.base_sprite_disconnect();
    
    //Unload the bases of the sprites
    spr_base_player_standing.unload();
    spr_base_player_window.unload();
    spr_base_player_suitcase_attack.unload();
    spr_base_player_walking_right_suitcase.unload();
    spr_base_player_side_door.unload();
    spr_base_player_pistol.unload();
    
    //Unload the halfed bases
    spr_base_player_standing_half.unload();
    spr_base_player_suitcase_attack_half.unload();
    spr_base_player_walking_right_suitcase_half.unload();
    spr_base_player_window_half.unload();
    spr_base_player_side_door_half.unload();
    spr_base_player_pistol_half.unload();
    
    //Unload the halfed sprites
    spr_player_standing_half.base_sprite_disconnect();
    spr_player_window_half.base_sprite_disconnect();
    spr_player_walking_right_suitcase_half.base_sprite_disconnect();
    spr_player_suitcase_attack_half.base_sprite_disconnect();
    spr_player_side_door_half.base_sprite_disconnect();
    spr_player_pistol_half.base_sprite_disconnect();
    
    
    
    
    //Unload the collision checking data
    coll_player_standing.unload();
    coll_player_window.unload();
    coll_player_walking_suitcase_left.unload();
    coll_player_walking_suitcase_right.unload();
    coll_player_suitcase_attack_left.unload();
    coll_player_suitcase_attack_right.unload();
    coll_player_left_door.unload();
    coll_player_right_door.unload();
}
