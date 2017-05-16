#ifndef RAILWAY_H
#define RAILWAY_H

#include <cstring>
#include"sprites.h"

#define WALK_TO_STATION                                            300 //5 Seconds

#define WALKING                                                    true
#define STOPPING                                                   false

#define SHIFT_STATION_MAPS                                         10

#define RAILWAY_DRAW_STATIONS_DISTANCE                             92
//
class RAILWAY
{
public:
    //The map position
    int map_x, map_y;
    
    //The map size
    int width_map, height_map;
    BITMAP *bmp_map;

    //The rail position
    float rail_x, rail_y;
    float rail_speed;
    SPRITE spr_rail;
    
    int n_lines;
    int n_stations;

	int line_color;
	int arrow_color;
	int line_r;
	int line_g;
	int line_b;
	
	// Position that the player must get off the rail
	int arrow_x, arrow_y;
	
	bool action_rail;
	
	// The time remaining to get the station
	int time_remaining;
	
	float count_frames;
	int count_stations;
	int count_lines;
	
	// Force the count_stations stop count until the rail begin to move
	bool stops;

	char str_name_previous[20];
	char str_name[20];
	bool change_line;
	
    //Constructor
    RAILWAY();

    // Set the number lines
    void setLines(int n_lines);
    
    // Set the number stations
    void setStations(int n_stations);

    //Set the map position
    void setMapPosition(int map_x, int map_y);

	void setRailSprite(BASE_SPRITE *base_rail);

    void setMapSize(int width_map, int height_map);
    
    //Logic loop
    void logicLoop();
    
    //Activate the rail again
    void activate();
    
    int getTimeStation();
    
    //Draw the map and rail
    void draw(BITMAP *buffer, int camera_x, int camera_y);
    
    //Destructor
    ~RAILWAY();
};




#endif

