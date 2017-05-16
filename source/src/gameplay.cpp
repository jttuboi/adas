#include"gameplay.h"

//Constructor
GAMEPLAY::GAMEPLAY()
{
    //Load the station image
    spr_station = load_bitmap("media/station.bmp", NULL);
    spr_station_half = bitmap_half(spr_station);
    
    //No zoom
    zoom = ZOOM_NONE;
    
    
    
    //Load the side door mask
    base_side_door_mask.load_dat("media/side_door_mask.dat");
    
    //Create the collision data
    coll_side_door.set(&base_side_door_mask, true);
    
    //Game just started
    game_ended = false;
    
    //This station didnt come
    this_station = false;
}



//Set a new gameplay
void GAMEPLAY::setGame(int wagons_number, int lines_number, int stations_number_max, int stations_number_variance, int gameplay_type)
{
    //Actual line is 0, station 0
    line = 0;
    station = 0;
    
    //Set the game type
    this->gameplay_type = gameplay_type;
    gameplay_stage = 0;
    
    //Number of lines
    this->lines_number = lines_number;
    
    //Create the stations vector
    stations_number = new int[lines_number];
    
    //Create the number of stations for each line
    for(int i=0; i < lines_number; ++i)
    {
        stations_number[i] = 3;
    }
    
    
    
    //Create the wagons
    this->wagons_number = wagons_number;
    
    wagons = new WAGON[wagons_number];
    
    for(int i=0; i < wagons_number; ++i)
    {
        wagons[i].setWagon((i*800) + (i*50), 0, &wagon_data);
    }
    
    
    
    //Set the camera position
    setCameraPosition(0, -300);
    camera_draw_x = 0;
    camera_draw_y = 0;
    
    //Set the play area limits
    setAreaLimit(0, (wagons_number*850) - 50);
    
    player.setAreaLimit(area_x_min_limit, area_x_max_limit);
    
    //Give a starting position to the player
    player.setPosition(0, 70);
    
    
    
    //Set some starting values
    time_next_station = GAMEPLAY_TIME_SLOW_DOWN;
    stopped_time = 0;
    
    
    
    //Not on station
    on_station = false;
    
    
    
    //Set initial zoom to no zooming
    zoom = ZOOM_NONE;
    
    player.setZoom(zoom);
    
    for(int i=0; i < wagons_number; ++i)
    {
        wagons[i].setZoom(zoom);
    }
    
    
    
    //Prepare the railway
    base_rail.load_dat("media/railway.dat");
    
    railway.setLines(LINES_MAIN_NUMBER);
    railway.setStations(STATIONS_MAIN_NUMBER);
    railway.setRailSprite(&base_rail);
    railway.setMapSize(800, 100);
    railway.setMapPosition(0, 0);
    
    
    
    //No warning to be shown
    warning = WARNING_NONE;
    
    
    
    //No post mortem
    post_mortem_prepared = false;
    
    //Player hasnt changed at the right station
    station_ok = false;
    
    
    
    
    
    
    
    
    background.setBackground(BACKGROUND_TUNNEL1, 2400);
    background.setMaxSpeed(GAMEPLAY_MAX_SPEED);
    background.setSpeed(GAMEPLAY_MAX_SPEED);
}



//Set a camera position
void GAMEPLAY::setCameraPosition(int camera_x, int camera_y)
{
    this->camera_x = camera_x;
    this->camera_y = camera_y;
}



//Set a area limit (horizontal)
void GAMEPLAY::setAreaLimit(int area_x_min_limit, int area_x_max_limit)
{
    this->area_x_min_limit = area_x_min_limit;
    this->area_x_max_limit = area_x_max_limit;
}



//Create a enemy (not fat) of a type
void GAMEPLAY::createEnemy(int type, int wagon, int delay)
{
    int door_position = (wagon*850) + 400;
    if(type == TYPE_RANDOM)
    {
        //Randomize an enemy
        type = rand()%3;
        
        if(type == TYPE_FLIES)
        {
            int n_flies = (rand()%3)+4;
            
            for(int i=0; i < n_flies; ++i)
            {
                int x_rand = door_position - ((rand()%60) - 30);
                int y_rand = 70 + (rand()%60);
                enemies_manager.createFly(x_rand, y_rand, delay);
            }
        }
        
        
        
        if(type == TYPE_ZOMBIE)
        {
            int x_rand = door_position - ((rand()%60) - 30);
            int y_rand = 70 + (rand()%60);
            
            enemies_manager.createZombie(x_rand, 70, delay);
            
            int n_flies = (rand()%3)+1;
            
            for(int i=0; i < n_flies; ++i)
            {
                x_rand = door_position - ((rand()%60) - 30);
                y_rand = 70 + (rand()%60);
                enemies_manager.createFly(x_rand, y_rand, delay);
            }
        }
        
        
        
        if(type == TYPE_POLICE_ZOMBIE)
        {
            int x_rand = door_position - ((rand()%60) - 30);
            int y_rand = 70 + (rand()%60);
            
            enemies_manager.createPoliceZombie(x_rand, 70, delay);
            
            int n_flies = (rand()%2)+1;
            
            for(int i=0; i < n_flies; ++i)
            {
                x_rand = door_position - ((rand()%60) - 30);
                y_rand = 70 + (rand()%60);
                enemies_manager.createFly(x_rand, y_rand, delay);
            }
        }
    }
}



//Logic loop for the game
void GAMEPLAY::logicLoop()
{
    if(!game_ended)
    {
        //We are right on middle of it!
        if(key[KEY_0]) time_next_station = 0;
        
        if(key[KEY_9]) time_next_station = GAMEPLAY_TIME_SLOW_DOWN;
        
        if(key[KEY_8]) time_next_station = GAMEPLAY_TIME_SLOW_DOWN + 1;
        
        
        
        
        
        
        
        
        //Railway logic loop
        railway.logicLoop();
        
        if(!on_station)
        {
            //Decide what the train needs to actually do
            time_next_station = railway.getTimeStation();
            
            //Check if we reached a station
            if(time_next_station <= 0)
            {
                //Prepare things so the train will stop at the station
                on_station = true;
                background.setEndingFlag(true);
                background.setSpeed(background.getMaxSpeed() / 3);
                
                //Set the station position as the end of the wagons
                station_x = wagons_number * 850;
                
                //Set the station action to stopping
                station_action = GAMEPLAY_STATION_STAGE_STOPPING;
                
                
                
                //Create the enemies that will be waiting at the station
                
                //The wagon door position is 400 (relative to the wagon)
                
                /*Coordinates for zombies:
                Wagon 1: Left door - 370
                         Right door - 430
                         
                         y = 70
                */
                
                //Acts accordingly to or gameplay type
                if(gameplay_type == GAMEPLAY_TYPE_NORMAL)
                {
                    //Acts accordingly to the stage
                    switch(gameplay_stage)
                    {
                        case 0:
                        {
                            //Introduction case, create a few zombies
                            enemies_manager.createZombie(370, 70, 60);
                            enemies_manager.createZombie(430, 70, 60);
                            //enemies_manager.createFatZombie(400, 70, 400);
                            
                            enemies_manager.createZombie(1220, 70, 60);
                            enemies_manager.createZombie(1280, 70, 60);
                            
                            enemies_manager.createZombie(2070, 70, 60);
                            enemies_manager.createZombie(2130, 70, 60);
                            
                            gameplay_stage = 1;
                            
                            this_station = false;
                        } break;
                        
                        
                        
                        case 1:
                        {
                            //Two introduction police zombies
                            enemies_manager.createPoliceZombie(1220, 70, 60);
                            enemies_manager.createPoliceZombie(1280, 70, 60);
                            
                            //Random a door with a number of random enemies
                            int door1 = rand()%3;
                            int n_enemies = 3;
                            
                            for(int i=0; i < n_enemies; ++i)
                            {
                                createEnemy(TYPE_RANDOM, door1, (rand()%120)+60);
                            }
                            
                            int door2 = door1;
                            while(door2 == door1) door2 = rand()%3;
                            
                            n_enemies = 3;
                            for(int i=0; i < n_enemies; ++i)
                            {
                                createEnemy(TYPE_RANDOM, door1, (rand()%120)+60);
                            }
                            
                            
                                
                            gameplay_stage = 2;
                            this_station = false;
                        } break;
                        
                        
                        
                        case 2:
                        {
                            gameplay_stage = 3;
                            this_station = false;
                            
                            //Create random enemies
                            int n_enemies = (rand()%6)+3;
                            for(int i=0; i < n_enemies; ++i)
                            {
                                createEnemy(TYPE_RANDOM, 0, (rand()%120)+60);
                            }
                            
                            n_enemies = (rand()%6)+3;
                            for(int i=0; i < n_enemies; ++i)
                            {
                                createEnemy(TYPE_RANDOM, 1, (rand()%120)+60);
                            }
                            
                            n_enemies = (rand()%6)+3;
                            for(int i=0; i < n_enemies; ++i)
                            {
                                createEnemy(TYPE_RANDOM, 2, (rand()%120)+60);
                            }
                            
                            //Set the warning
                            warning = WARNING_STATION_CHANGE;
                        } break;
                        
                        
                        
                        case 3:
                        {
                            //This station is a must for the player to get out
                            this_station = true;
                            
                            //Player must get out this station
                            station_ok = false;
                            
                            //At the player wagon
                            int door_x = ((player.x / 850) * 850) + 400;
                            enemies_manager.createFatZombie(door_x, 70, 400);
                            
                            warning = WARNING_STATION_CHANGE_NOW;
                            gameplay_stage = 4;
                        } break;
                        
                        
                        
                        case 4:
                        {
                            gameplay_stage = 5;
                            this_station = false;
                            
                            warning = WARNING_NONE;
                            
                            //Create random enemies
                            int n_enemies = (rand()%6)+3;
                            for(int i=0; i < n_enemies; ++i)
                            {
                                createEnemy(TYPE_RANDOM, 0, (rand()%120)+60);
                            }
                            
                            n_enemies = (rand()%6)+3;
                            for(int i=0; i < n_enemies; ++i)
                            {
                                createEnemy(TYPE_RANDOM, 1, (rand()%120)+60);
                            }
                            
                            n_enemies = (rand()%6)+3;
                            for(int i=0; i < n_enemies; ++i)
                            {
                                createEnemy(TYPE_RANDOM, 2, (rand()%120)+60);
                            }
                        }
                        
                        
                        
                        case 6:
                        {
                            gameplay_stage = 5;
                            this_station = false;
                        }
                        
                        
                        
                        case 7:
                        {
                            gameplay_stage = 5;
                            this_station = false;
                        }
                    }
                }
            } else {
                //We are on the rails
                background.setSpeed(background.getMaxSpeed());
                
                if(warning == WARNING_STATION_CHANGE_NOW && !station_ok)
                {
                    game_ended = true;
                    game_ended_reason = GAME_ENDED_LOST_STATION;
                }
            }
        } else {
            //Do the appropriate action
            switch(station_action)
            {
                case GAMEPLAY_STATION_STAGE_STOPPING:
                {
                    station_x -= background.getSpeed();
                    //station_x -= background.getMaxSpeed();
                    
                    if(station_x < 0)
                    {
                        station_x = 0;
                        station_action = GAMEPLAY_STATION_STAGE_DOORS_OPPENING;
                        
                        for(int i=0; i < wagons_number; ++i)
                        {
                            wagons[i].prepareOpenDoor();
                        }
                    }
                } break;
                
                
                
                case GAMEPLAY_STATION_STAGE_DOORS_OPPENING:
                {
                    //Open the wagons door
                    bool opened = false;
                    
                    for(int i=0; i < wagons_number; ++i)
                    {
                        opened = wagons[i].openDoor();
                    }
                    
                    if(opened == true)
                    {
                        //Set the right station time
                        station_wait = enemies_manager.getBiggestDelayOnStation();
                        station_wait += GAMEPLAY_STATION_WAIT_TIME;
                        
                        //Into the wagons
                        enemies_manager.toTheWagons();
                        
                        //Continue
                        station_action = GAMEPLAY_STATION_STAGE_DOORS_OPEN;
                    }
                } break;
                
                
                
                case GAMEPLAY_STATION_STAGE_DOORS_OPEN:
                {
                    //Wait
                    --station_wait;
                    
                    if(station_wait <= 0)
                    {
                        for(int i=0; i < wagons_number; ++i)
                        {
                            wagons[i].prepareCloseDoor();
                        }
                        
                        station_action = GAMEPLAY_STATION_STAGE_DOORS_CLOSING;
                    }
                } break;
                
                
                
                case GAMEPLAY_STATION_STAGE_DOORS_CLOSING:
                {
                    //Close the wagons door
                    bool closed = false;
                    
                    for(int i=0; i < wagons_number; ++i)
                    {
                        closed = wagons[i].closeDoor();
                    }
                    
                    if(closed == true)
                    {
                        //Back the background
                        background.setEndingFlag(false);
                        
                        station_action = GAMEPLAY_STATION_STAGE_EXITING;
                    }
                } break;
                
                
                
                case GAMEPLAY_STATION_STAGE_EXITING:
                {
                    //By station
                    station_x -= background.getSpeed();
                    
                    if(station_x <= -2550)
                    {
                        on_station = false;
                        railway.activate();
                    }
                } break;
            }
        }
        
        
        //Move the background
        background.logicLoop();
        
        //Move the camera if some keys are pressed
        if(key[KEY_LEFT]) camera_x -= 9;
        if(key[KEY_RIGHT]) camera_x += 9;
        if(key[KEY_UP]) camera_y -= 3;
        if(key[KEY_DOWN]) camera_y += 3;
        
        
        
        //Zoom in or out?
        if(key[KEY_O])
        {
            zoom = ZOOM_OUT;
            
            player.setZoom(zoom);
            
            for(int i=0; i < wagons_number; ++i)
            {
                wagons[i].setZoom(zoom);
            }
        }
        
        if(key[KEY_I])
        {
            zoom = ZOOM_NONE;
            
            player.setZoom(zoom);
            
            for(int i=0; i < wagons_number; ++i)
            {
                wagons[i].setZoom(zoom);
            }
        }
        
        
        //Enemies logic loop
        enemies_manager.logicLoop(player.x, player.y);
        
        
        int player_x_old = player.x;
        int player_y_old = player.y;
        
        //Player logic loop
        player.logicLoop();
        
        //Check if the player has changed wagons
        if((player.x/850) != (player_x_old/850)) player.changedWagon();
        
        
        
        //Check if the player wants to try some action
        if(player.getAction())
        {
            //He does. Check if there is any action to be executed
            int action_result = WAGON_ACTION_NONE; //Means no actions to be done
            for(int i=0; i < wagons_number && action_result == WAGON_ACTION_NONE; ++i)
            {
                action_result = wagons[i].requestAction(player.x);
                
                if(action_result != WAGON_ACTION_NONE)
                {
                    if(action_result != WAGON_ACTION_FRONT_DOOR)
                    {
                        //We got something. Realize the action
                        player.setActionType(action_result);
                    } else {
                        //The player got out of the station. Is it the right station?
                        if(station_action == GAMEPLAY_STATION_STAGE_DOORS_OPEN)
                        {
                            if(this_station)
                            {
                                //All ok, player did it right
                                warning = WARNING_CORRECT_TRAIN_CHANGE;
                                
                                //Clear the train
                                enemies_manager.clear();
                                
                                //Put player at middle wagon
                                player.x = 1250;
                                
                                //Player DID get the station
                                station_ok = true;
                            } else {
                                //Ooops, wrong station
                                game_ended = true;
                                game_ended_reason = GAME_ENDED_WRONG_STATION;
                            }
                        }
                    }
                }
            }
        }
        
        
        
        //Do some action, if the player has finished on doing it
        if(player.getActionFinished())
        {
            //Do the action on the wagon
            for(int i=0; i < wagons_number; ++i)
            {
                wagons[i].doAction(player.x);
            }
        }
        
        
        
        switch(zoom)
        {
            case ZOOM_NONE:
            {
                //Center the camera on the player
                camera_x = player.x - (SCREEN_WIDTH/2);
                
                //Adjust the camera to gameplay area limits
                if(camera_x < 0) camera_x = 0;
                if(camera_x > 1700) camera_x = 1700;
        
                camera_draw_x = camera_x;
                camera_draw_y = camera_y;
            } break;
            
            
            
            case ZOOM_OUT:
            {
                camera_draw_x = camera_x/2;
                camera_draw_y = camera_y;
                
                //Center the camera on the player
                camera_x = player.x - (SCREEN_WIDTH);
                
                //Adjust the camera to gameplay area limits
                if(camera_x < 0) camera_x = 0;
                if(camera_x > 850) camera_x = 850;
            } break;
        }
        
        
        
        //Guarantee that the first and last side doors are closed
        if(wagons[0].getLeftDoorOpen()) wagons[0].setLeftDoorOpen(false);
        if(wagons[wagons_number-1].getRightDoorOpen()) wagons[wagons_number-1].setRightDoorOpen(false);
        
        
        
        //Check the player collision againt the doors
        for(int i=0; i < wagons_number; ++i)
        {
            if(!wagons[i].getLeftDoorOpen())
            {
                player.checkSideDoorCollision(&coll_side_door, (i*850)-35, 0);
            }
            if(!wagons[i].getRightDoorOpen())
            {
                player.checkSideDoorCollision(&coll_side_door, (i*850) + 835, 0);
            }
        }
        
        
        
        //Check the enemies collision against the doors
        //Check the player collision againt the doors
        for(int i=0; i < wagons_number; ++i)
        {
            if(!wagons[i].getLeftDoorOpen())
            {
                int hits = enemies_manager.checkSideDoorCollision(&coll_side_door, (i*850)-35, 0);
                wagons[i].left_door_hit(hits);
            }
            if(!wagons[i].getRightDoorOpen())
            {
                int hits = enemies_manager.checkSideDoorCollision(&coll_side_door, (i*850) + 835, 0);
                wagons[i].right_door_hit(hits);
            }
        }
        
        
        
        //Check if the player has shooted
        if(player.getPlayerGunShot())
        {
            //Check if there is a valid target
            if(player.direction == LEFT)
            {
                int target_x = enemies_manager.checkClosestLeft(player.x);
                
                int target_wagon = (target_x / 850);
                int player_wagon = (player.x / 850);
                
                bool shoot = true;
                
                if(target_x == -1) shoot = false;
                
                if(target_wagon == player_wagon && target_x != -1)
                {
                    shoot = true;
                } else {
                    if(wagons[player_wagon].getLeftDoorOpen())
                    {
                        for(int i=player_wagon-1; i >= target_wagon; --i)
                        {
                            if(!wagons[i].getRightDoorOpen()) shoot = false;
                            if(i != target_wagon)
                            {
                                if(!wagons[i].getLeftDoorOpen()) shoot = false;
                            }
                        }
                    } else {
                        shoot = false;
                    }
                }
                
                
                
                //Managed to hit?
                if(shoot)
                {
                    ENEMY *dead = enemies_manager.checkClosestEnemyLeft(player.x);
                    dead->setLife(false);
                }
            } else {
                int target_x = enemies_manager.checkClosestRight(player.x);
                
                int target_wagon = (target_x / 850);
                int player_wagon = (player.x / 850);
                
                bool shoot = true;
                
                if(target_x == -1) shoot = false;
                
                if(target_wagon == player_wagon && target_x != -1)
                {
                    shoot = true;
                } else {
                    if(wagons[player_wagon].getRightDoorOpen())
                    {
                        for(int i=player_wagon+1; i <= target_wagon; ++i)
                        {
                            if(!wagons[i].getLeftDoorOpen()) shoot = false;
                            if(i != target_wagon)
                            {
                                if(!wagons[i].getRightDoorOpen()) shoot = false;
                            }
                        }
                    } else {
                        shoot = false;
                    }
                }
                
                
                
                //Managed to hit?
                if(shoot)
                {
                    ENEMY *dead = enemies_manager.checkClosestEnemyRight(player.x);
                    dead->setLife(false);
                }
            }
        }
        
        
        
        //Check enemy collisions against the player
        enemies_manager.checkPlayerCollision(player.collision_data, player.x, player.y, player.spr_to_draw->frame, player.getAttackType(), player.getAttackingFrame());
        
        //Now check if the enemies did attack the player
        int attk = enemies_manager.checkEnemyAttackCollision(player.collision_data, player.x, player.y, player.spr_to_draw->frame);
        
        if(attk == ENEMY_ATTACK_STAMINA)
        {
            player.staminaAttack();
        }
        
        if(attk == ENEMY_ATTACK_DEATH)
        {
            game_ended = true;
            game_ended_reason = GAME_ENDED_PLAYER_DIED;
        }
    } else {
        //Too bad, game has ended
        if(!post_mortem_prepared)
        {
            player.prepare_post_mortem();
            post_mortem_prepared = true;
        } else {
            player.post_mortem_logic_loop();
        }
    }
}



//Draw everything
void GAMEPLAY::draw(BITMAP *buffer)
{
    if(!game_ended)
    {
        //We are playing
        
        //Draw the background
        background.draw(buffer, camera_draw_x, camera_draw_y, zoom);
        
        //If we are arreiving in a station, draw the station
        if(on_station)
        {
            //Draw the station
            
            switch(zoom)
            {
                case ZOOM_NONE:
                {
                    for(int i=0; i < wagons_number; ++i)
                    {
                        blit(spr_station, buffer, 0, 0, (i*850) - (camera_draw_x - station_x), -camera_draw_y, spr_station->w, spr_station->h);
                    }
                    
                    //Draw the enemies on station
                    enemies_manager.drawAtStation(buffer, camera_draw_x - station_x, camera_draw_y, zoom);
                } break;
                
                
                
                case ZOOM_OUT:
                {
                    for(int i=0; i < wagons_number; ++i)
                    {
                        blit(spr_station_half, buffer, 0, 0, (i*850)/2 - (camera_draw_x - station_x/2), -camera_draw_y, spr_station->w, spr_station->h);
                    }
                    
                    //Draw the enemies on station
                    enemies_manager.drawAtStation(buffer, camera_draw_x - station_x/2, camera_draw_y, zoom);
                } break;
            }
        }
        
        
        
        //Draw the wagons
        for(int i=0; i < wagons_number; ++i)
        {
            wagons[i].draw(buffer, camera_draw_x, camera_draw_y);
        }
        
        
        
        //Draw the enemies inside the wagon
        enemies_manager.drawInsideWagon(buffer, camera_draw_x, camera_draw_y, zoom);
        
        //Draw the player
        player.draw(buffer, camera_draw_x, camera_draw_y);
        
        //Draw the wagons division
        switch(zoom)
        {
            case ZOOM_NONE:
            {
                for(int i=0; i < wagons_number-1; ++i)
                {
                    rectfill(buffer, (((i+1)*800) + (i*50)) - camera_draw_x, 3 - camera_draw_y, (((i+1)*800) + (i*50) + 50) - camera_draw_x, 250 - camera_draw_y, makecol(0,0,0)); 
                }
            } break;
            
            
            
            case ZOOM_OUT:
            {
                for(int i=0; i < wagons_number-1; ++i)
                {
                    rectfill(buffer, (((i+1)*400) + (i*25)) - camera_draw_x, 3 - camera_draw_y, (((i+1)*400) + (i*25) + 25) - camera_draw_x, 125 - camera_draw_y, makecol(0,0,0)); 
                }
            }
        }
        
        
        
        //Draw the map
        railway.draw(buffer, 0, 0);
        
        
        
        //Write the warnings
        switch(warning)
        {
            case WARNING_STATION_CHANGE:
            {
                textout_centre_ex(buffer, font, "WARNING: CHANGE TRAINS AT NEXT STATION", 400, 250, makecol(255, 255, 80), -1);
            } break;
            
            
            
            case WARNING_STATION_CHANGE_NOW:
            {
                textout_centre_ex(buffer, font, "WARNING: CHANGE TRAINS AT THIS STATION", 400, 250, makecol(255, 0, 0), -1);
            } break;
            
            
            
            case WARNING_CORRECT_TRAIN_CHANGE:
            {
                textout_centre_ex(buffer, font, "MANAGED TO GET THE RIGHT TRAIN", 400, 250, makecol(0, 255, 0), -1);
            } break;
        }
    } else {
        //Game has ended
        if(game_ended_reason == GAME_ENDED_WRONG_STATION)
        {
            textout_centre_ex(buffer, font, "YOU GOT OUT AT THE WRONG STATION", 400, 150,makecol(255, 255, 255), -1);
            
            player.post_mortem_draw(buffer);
        }
        
        
        
        if(game_ended_reason == GAME_ENDED_LOST_STATION)
        {
            textout_centre_ex(buffer, font, "YOU FAILED TO GET OUT AT THE RIGHT STATION", 400, 150,makecol(255, 255, 255), -1);
            
            player.post_mortem_draw(buffer);
        }
        
        
        
        if(game_ended_reason == GAME_ENDED_PLAYER_DIED)
        {
            textout_centre_ex(buffer, font, "YOU DIED", 400, 150,makecol(255, 255, 255), -1);
            
            player.post_mortem_draw(buffer);
        }
    }
}



//Destructor
GAMEPLAY::~GAMEPLAY()
{
    //Unload the bitmap
    destroy_bitmap(spr_station);
    destroy_bitmap(spr_station_half);
    
    //Unload the side door mask
    base_side_door_mask.unload();
    
    //Unload the collision data
    coll_side_door.unload();
    
    //Remove the railway base
    base_rail.unload();
}
