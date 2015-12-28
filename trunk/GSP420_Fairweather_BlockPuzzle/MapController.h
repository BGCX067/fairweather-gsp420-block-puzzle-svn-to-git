#pragma once
/// MapController responsible for storing, changing, and drawing data from files

#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std ;

class Map
{
private:
	//2D vector struct
	struct Vec2D
	{
		int x ;
		int y ;
	} ;

	//2D map data
	char ** m_map ;

	//width and height of the map
	int m_width ;
	int m_height ;

	//position and previous position of the map on screen
	Vec2D m_pos ;
	Vec2D m_oldPos ;

	//current rotation of the piece
	int m_rot ;

	//tells if the piece has moved down
	bool m_movedDown ;

	//tells if the piece is ready to become another map
	bool m_nextMap ;

	//define file seek offsets and spawn position
	#define FILE_OFFSET_LARGE	7
	#define FILE_OFFSET_SMALL	5
	#define SPAWN_POS_X			3
	#define SPAWN_POS_Y			0

	//rotation enumeration which changes based on where the user is rotating the pieces
	enum rotation {NONE, CW, CCW} ;

public:
	//Constructor
	Map() ;

	//Copy Constructor
	Map( Map & a_original ) ;

	//Creates a new dynamically allocated 2D array of ints
	static char ** Map::new2Dmap( int const & h, int const & w )
	{
		char ** newMap = new char * [h] ;
		for( int row = 0; row < h; ++row )
			newMap[row] = new char[w] ;
		return newMap ;
	}

	//Deletes the 2D int array
	static void Map::delete2Dmap( char ** map, int const & h )
	{
		if( map )
		{
			for( int row = 0; row < h; ++row )
				delete [] map[row] ;
			delete map ;
			map = 0 ;
		}
	}

	//Loads the source file of the map, returns false if the file is not found
	bool loadFile( char * filename ) ;

	//Gets the Width or Height of the map
	int getWidth( ) { return m_width ; }
	int getHeight( ) { return m_height ; }

	//Moves the piece back to its original position/rotation if it runs into the wall or an object
	void forgiveness( Map * a_map ) ;

	//copies the location of the piece onto the Map, and sets it to #
	void staticMap( Map* a_map ) ;

	//Checks each line to see if it is filled with #'s
	int checkLine( ) ;

	void clearLine( int line ) ;

	//renders the "graphics" onto the screen
	void draw( ) ;

	void drawScore( int a_score ) ;

	//Moves the cursor on the screen to desired position
	void gotoxy(int const & x, int const & y) ;

	//gets the specific map data
	char get( int x, int y ) ;

	//checks if map is colliding with any static object
	bool isColliding( Map * a ) ;

	//checks if the piece is ready for a new map data. Sets the variable back to false if true
	bool readyForNextMap( ) ;

	//checks if the game has reached its loss condition
	bool lossCondition( Map * a_map) ;

	//moves map down and sets boolean to true
	void moveDown( ) ;

	//moves left
	void moveLeft( ) ;

	//moves right
	void moveRight( ) ;

	//updates the old position to the current position
	void updatePosition( ) ;

	//flips the map clockwise
	void flipCW( ) ;

	//flips the map counter-clockwise
	void flipCCW( ) ;

	//resets the map to its original spawn position
	void reset( ) ;

	//deletes the 2D map data safely
	void release( ) ;

	//destructor calls the release function for safe deletion
	~Map() {release( ) ;}
} ;