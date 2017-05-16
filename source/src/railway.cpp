#include"railway.h"

//Constructor
RAILWAY::RAILWAY()
{
	setMapPosition(0, 0);
	
	rail_x = 0;
	
	bmp_map = NULL;
	
	map_x = 0;
	map_y = 0;
	
	// If rail is stopping or walking
	action_rail = WALKING;
	
	//White
	line_r = 255;
	line_g = 255;
	line_b = 255;
	
	//Gray
	arrow_color = makecol(128, 128, 128);
	change_line = false;

	// For draw lines of stations
	count_stations = 0;
	count_lines = 1;
	
	// For draw rail walking station to station in the size of pixel
	count_frames = 0;
	
	// Stop counting station until rail start walking again
	stops = true;
	
	//Rail speed
	rail_speed = (float)RAILWAY_DRAW_STATIONS_DISTANCE / (float)WALK_TO_STATION;//a/b;
	
	// Time remaining to next station
	time_remaining = WALK_TO_STATION;
	
	strcpy(str_name_previous, "Begin");
	strcpy(str_name, "Begin");
}

void RAILWAY::setLines(int n_lines)
{
	this->n_lines = n_lines;
}
    
void RAILWAY::setStations(int n_stations)
{
	this->n_stations = n_stations;
}

void RAILWAY::setMapPosition(int map_x, int map_y)
{
	this->map_x = map_x;
	this->map_y = map_y;
}


void RAILWAY::setRailSprite(BASE_SPRITE *base_rail)
{
	spr_rail = base_rail;
	spr_rail.autoset_origins();
}

void RAILWAY::setMapSize(int width_map, int height_map)
{
	if (bmp_map != NULL)
	{
		destroy_bitmap(bmp_map);
		bmp_map = NULL;
	}
		
	bmp_map = create_bitmap(width_map, height_map);
	
	clear(bmp_map);
	
	rail_x = map_x + SHIFT_STATION_MAPS;
	rail_y = bmp_map->h / 2 + map_y;
	
	arrow_x = 92*n_stations + SHIFT_STATION_MAPS;
	arrow_y = bmp_map->h / 2;
}

void RAILWAY::logicLoop()
{

	line_color = makecol(line_r, line_g, line_b);

	if(action_rail)
	{
		//Move the rail
		rail_x += rail_speed;
		
		//Less time remaining
		--time_remaining;
		
		if(time_remaining <= 0)
		{
		    action_rail = false;
		}
	}
	else
	{    // Rail is stopped
		if (stops)
		{
			count_stations++;
			stops = false;
			
			time_remaining = WALK_TO_STATION;
		}
	}
	
	if (count_stations == n_stations)
	{
		count_lines++;
		count_stations = 0;
		
		rail_x = map_x + SHIFT_STATION_MAPS;

		// Go to begin of map
		//rail_x = map_x + SHIFT_STATION_MAPS;

		arrow_color = line_color;
		change_line = true;
		
		// Change color line
		switch (count_lines)
		{
			case 1:
			{	// White
				line_r = 255;
				line_g = 255;
				line_b = 255;
			} break;
			case 2:
			{	// Blue
				line_r = 0;
				line_g = 0;
				line_b = 255;
			} break;
			case 3:
			{	// Red
				line_r = 255;
				line_g = 0;
				line_b = 0;
			} break;
			case 4:
			{	// Green
				line_r = 0;
				line_g = 255;
				line_b = 0;
			} break;
			case 5:
			{	// Yellow
				line_r = 255;
				line_g = 255;
				line_b = 0;
			} break;
		}
		
		// Change arrow position
		arrow_x = 92*n_stations + SHIFT_STATION_MAPS;
	}
	
	if (count_lines == n_lines)
	{
		count_lines = 1;
	}
	
	// Draw rail
	if (stops)
	{
		spr_rail.logic_loop();
	}
	
	
	
	if(time_remaining <= 0)
	{
	    action_rail = false;
	}
}



void RAILWAY::activate()
{
    action_rail = true;
    stops = true;
}


  
int RAILWAY::getTimeStation()
{
	return time_remaining;
}



void RAILWAY::draw(BITMAP *buffer, int camera_x, int camera_y)
{
	clear(bmp_map);
	
	// Draw main line
	line(bmp_map, 0, bmp_map->h / 2, bmp_map->w, bmp_map->h / 2, line_color);

	// Draw the first line
	line(bmp_map, SHIFT_STATION_MAPS, (bmp_map->h / 2) + 5, SHIFT_STATION_MAPS, (bmp_map->h / 2) - 15, line_color);
	
	if (change_line)
	{
		strcpy(str_name_previous, str_name);
		change_line = false;
	}
	
	textout_ex(bmp_map, font, str_name_previous, 10, (bmp_map->h / 2) - 30, arrow_color, 0);
	
	// Draw the stations
	for (int i = 0; i < n_stations; i++)
	{
		line(bmp_map, 92*(i+1) + SHIFT_STATION_MAPS, (bmp_map->h / 2) + 5, 92*(i+1) + SHIFT_STATION_MAPS,
		     (bmp_map->h / 2) - 15, line_color);
		
		switch (i)
		{
			case 0:
			{
				switch (count_lines)
				{
					case 1:
					{	// White
						strcpy(str_name,"Kuido");
					} break;
					case 2:
					{	// Blue
						strcpy(str_name,"Alert");
					} break;
					case 3:
					{	// Red
						strcpy(str_name,"Okale");
					} break;
					case 4:
					{	// Green
						strcpy(str_name,"Saill");
					} break;
					case 5:
					{	// Yellow
						strcpy(str_name,"Iorme");
					} break;
				}
			} break;

			case 1:
			{
				switch (count_lines)
				{
					case 1:
					{	// White
						strcpy(str_name,"Array");
					} break;
					case 2:
					{	// Blue
						strcpy(str_name,"Lissa");
					} break;
					case 3:
					{	// Red
						strcpy(str_name,"Ittai");
					} break;
					case 4:
					{	// Green
						strcpy(str_name,"Snesn");
					} break;
					case 5:
					{	// Yellow
						strcpy(str_name,"Omose");
					} break;
				}
			} break;
			
			case 2:
			{
				switch (count_lines)
				{
					case 1:
					{	// White
						strcpy(str_name,"Nysad");
					} break;
					case 2:
					{	// Blue
						strcpy(str_name,"Doent");
					} break;
					case 3:
					{	// Red
						strcpy(str_name,"Radlo");
					} break;
					case 4:
					{	// Green
						strcpy(str_name,"Soiit");
					} break;
					case 5:
					{	// Yellow
						strcpy(str_name,"Raizu");
					} break;
				}
			} break;
			
			case 3:
			{
				switch (count_lines)
				{
					case 1:
					{	// White
						strcpy(str_name,"Anssu");
					} break;
					case 2:
					{	// Blue
						strcpy(str_name,"Athto");
					} break;
					case 3:
					{	// Red
						strcpy(str_name,"Ranpo");
					} break;
					case 4:
					{	// Green
						strcpy(str_name,"Rosas");
					} break;
					case 5:
					{	// Yellow
						strcpy(str_name,"Ikelo");
					} break;
				}
			} break;
			
			case 4:
			{
				switch (count_lines)
				{
					case 1:
					{	// White
						strcpy(str_name,"Soito");
					} break;
					case 2:
					{	// Blue
						strcpy(str_name,"Gargs");
					} break;
					case 3:
					{	// Red
						strcpy(str_name,"Eusks");
					} break;
					case 4:
					{	// Green
						strcpy(str_name,"Turks");
					} break;
					case 5:
					{	// Yellow
						strcpy(str_name,"Nossa");
					} break;
				}
			} break;
			
			case 5:
			{
				switch (count_lines)
				{
					case 1:
					{	// White
						strcpy(str_name,"Geipo");
					} break;
					case 2:
					{	// Blue
						strcpy(str_name,"Ydelu");
					} break;
					case 3:
					{	// Red
						strcpy(str_name,"Asshy");
					} break;
					case 4:
					{	// Green
						strcpy(str_name,"Nanny");
					} break;
					case 5:
					{	// Yellow
						strcpy(str_name,"Thorr");
					} break;
				}
			} break;
			
			case 6:
			{
				switch (count_lines)
				{
					case 1:
					{	// White
						strcpy(str_name,"Pauch");
					} break;
					case 2:
					{	// Blue
						strcpy(str_name,"Engla");
					} break;
					case 3:
					{	// Red
						strcpy(str_name,"Roufy");
					} break;
					case 4:
					{	// Green
						strcpy(str_name,"Luisi");
					} break;
					case 5:
					{	// Yellow
						strcpy(str_name,"Snayl");
					} break;
				}
			} break;
			
			case 7:
			{
				switch (count_lines)
				{
					case 1:
					{	// White
						strcpy(str_name,"Haeky");
					} break;
					case 2:
					{	// Blue
						strcpy(str_name,"Esbay");
					} break;
					case 3:
					{	// Red
						strcpy(str_name,"Sulso");
					} break;
					case 4:
					{	// Green
						strcpy(str_name,"Idelo");
					} break;
					case 5:
					{	// Yellow
						strcpy(str_name,"Salet");
					} break;
				}
			} break;
		}
		textout_ex(bmp_map, font, str_name, 92*(i + 1) + SHIFT_STATION_MAPS - 16, (bmp_map->h / 2) - 30,
				   line_color, 0);
	}

	// Arrow that points in the station that the player must get off, color is the color of line previous
	triangle(bmp_map, SHIFT_STATION_MAPS - 5, arrow_y - 15, SHIFT_STATION_MAPS, arrow_y - 5,
	         SHIFT_STATION_MAPS + 5, arrow_y - 15, arrow_color);	
	
	// Arrow that points in the station that the player must get off, color is the color of line actual
	triangle(bmp_map, arrow_x - 5, arrow_y - 15, arrow_x, arrow_y - 5, arrow_x + 5, arrow_y - 15, line_color);
	
	blit(bmp_map, buffer, 0, 0, map_x - camera_x, map_y - camera_y, bmp_map->w, bmp_map->h);
	spr_rail.draw(buffer, (int)rail_x - camera_x, rail_y - camera_y);
}

//Destructor
RAILWAY::~RAILWAY()
{
	//
}
