#include "MapController.h"

//Constructor
Map::Map()
{
	m_pos.x = m_pos.y = 0 ;
	m_oldPos.x = m_oldPos.y = 0 ;
	m_map = NULL ;
	m_width = m_height = 0 ;
	m_rot = 0 ;
	m_movedDown = m_nextMap = false ;
}

//Copy Constructor
Map::Map( Map & a_original )
{
	m_pos.x =  a_original.m_width ;
	m_height = a_original.m_height ;

	m_map = new2Dmap( m_height, m_width ) ;

	for( int row = 0; row < m_height; ++row )
	{
		for( int col = 0; col < m_width; ++col )
		{
			m_map[row][col] = a_original.m_map[row][col] ;
		}
	}
}

//Loads the source file of the map, returns false if the file is not found
bool Map::loadFile( char * filename )
{
	fstream file(filename) ;

	//return false if the file is not found
	if( !file ) 
	{
		return false ;
	}
	else
	{
		//extract the width and height of the map from the file
		file >> m_width ;
		file >> m_height ;

		//create a dynamically allocated array of chars for drawing the map data
		m_map = new char * [m_height] ;
			
		//The file tends to include the space from the width and height numbers at the top
		//so for the sake of sanity, this moves where the file is being looked at to the next line in the file
		//where the real char data is held
		if( m_width >= 10 )
			file.seekg(FILE_OFFSET_LARGE, ios::beg) ; //Map file
		else
		{
			file.seekg(FILE_OFFSET_SMALL, ios::beg) ; //Piece file
			m_pos.x = SPAWN_POS_X ; //set the spawn position of the map
		}

		//Get all the chars!
		for( int y = 0; y < m_height; ++y )
		{
			m_map[y] = new char[m_width] ;

			for( int x = 0; x < m_width; ++x )
			{
				m_map[y][x] = file.get() ;
			}		
			//Get the invisible '\n' at the end of each line
			file.get() ;
		}

		//release the file variable
		file.clear() ;

		return true ;
	}
}

//Moves the piece back to its original position/rotation if it runs into the wall or an object
void Map::forgiveness( Map * a_map )
{
	//condition statement
	if( m_pos.x + m_width > a_map->getWidth() ||
		m_pos.y + m_height > a_map->getHeight() ||
		m_pos.x < 0 ||
		isColliding( a_map ) )
	{
		//return it to its old position
		m_pos = m_oldPos ;

		//if it was rotated, rotate it back
		if( m_rot == CW )
		flipCCW() ;
		else if( m_rot == CCW )
		flipCW() ;

		//if it was moved down, create the static map
		if( m_movedDown )
		{
			staticMap( a_map ) ;
			reset() ; //reset the piece to its spawn position

			//it is ready for the next piece
			m_nextMap = true ;
			m_movedDown = false ;
		}
	}
	else
		updatePosition( ) ; //update the old position to the new position for next run

	m_rot = NONE ; //set the rotation to default

	//it didn't run into anything so if was moved down, set it back to false
	if( m_movedDown )
		m_movedDown = false ;
}

//copies the location of the piece onto the Map, and sets it to #
void Map::staticMap( Map* a_map )
{
	for( int row = 0; row < m_height; ++row )
	{
		for( int col = 0; col < m_width; ++col )
		{
			if( m_map[row][col] == '*' )
			{
				a_map->m_map[m_pos.y + row][m_pos.x + col] = '#' ;
			}
		}
	}
}

//Checks each line to see if it is filled with #'s
int Map::checkLine( )
{
	int count = 0 ;
	for( int row = 0; row < m_height; ++row )
	{
		for( int col = 0; col < m_width; ++col )
		{
			//if at least one space in the row is not a #, don't go any further
			if( m_map[row][col] != '#' )
				break ;

			//if we reached the end, and it still didn't break, that means the entire row is ready to be cleared
			if( col == m_width - 1 )
			{
				clearLine( row ) ;
				count++ ;
			}
		}
	}
		
	return count ;
}

void Map::clearLine( int line )
{
	for( int i = 0; i < m_width; i++ )
	{
		//delete all # for this line
		m_map[line][i] = '.' ;

		//move all static objects above the line down 1 space
		for( int top = line - 1; top > 0; --top )
		{
			if(m_map[top][i] == '#')
			{
				//swap spots on map to indicate that it moved down 1 space
				m_map[top][i] = '.' ;
				m_map[top+1][i] = '#' ;
			}
		}
	}
}

//renders the "graphics" onto the screen
void Map::draw( )
{
	for( int row = 0; row < m_height; ++row )
	{
		for( int col = 0; col < m_width; ++col )
		{
			//draw the char so long as it is NOT a whitespace or an invisible \n
			if( m_map[row][col] != ' ' && m_map[row][col] != '\n' )
			{
				//move cursor into position and draw the char
				gotoxy( m_pos.x + col, m_pos.y + row ) ;
				putchar(m_map[row][col]) ;
			}
		}
	}
}

void Map::drawScore( int a_score )
{
	gotoxy( 0, m_height + 1 ) ;
	cout << "Score: " << a_score ;
}

//Moves the cursor on the screen to desired position
void Map::gotoxy(int const & x, int const & y)
{
	COORD point = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

//gets the specific map data
char Map::get( int x, int y )
{
	return m_map[x][y] ;
}

//checks if map is colliding with any static object
bool Map::isColliding( Map * a )
{
	int bx = 0, by = 0 ;

	for( int row = m_pos.y; row < m_pos.y + m_height; ++row )
	{
		for( int col = m_pos.x; col < m_pos.x + m_width; ++col )
		{
			if( a->get( row, col ) == '#' && m_map[row - m_pos.y][col - m_pos.x] == '*' )
			{
				return true ;
			}
		}
	}
	return false ;
}

//checks if the piece is ready for a new map data. Sets the variable back to false if true
bool Map::readyForNextMap( )
{
	if( m_nextMap )
	{
		m_nextMap = false ;
		return true ;
	}
	return false ;
}

//checks if the game has reached its loss condition
bool Map::lossCondition( Map * a_map)
{
	for( int row = 0; row < m_height; ++row )
	{
		for( int col = 0; col < m_width; ++col )
		{
			//the game is lost if the map collides with ANY static object in or near its spawn point
			if( isColliding( a_map ) )
				return true ;	
		}
	}
	return false ;
}

//moves map down and sets boolean to true
void Map::moveDown( )
{
	m_pos.y++ ;
	m_movedDown = true ;
}

//moves left
void Map::moveLeft( ) 
{
	m_pos.x-- ;
}

//moves right
void Map::moveRight( )
{
	m_pos.x++ ;
}

//updates the old position to the current position
void Map::updatePosition( )
{
	m_oldPos = m_pos ;
}

//flips the map clockwise
void Map::flipCW( )
{
	m_rot = CW ;
	char ** newMap = new2Dmap( m_width, m_height ) ;

	for( int row = 0; row < m_height; ++row )
	{
		for( int col = 0; col < m_width; ++col ) 
		{
			newMap[col][row] = m_map[(m_height - 1) - row][col] ;
		}
	}
	delete2Dmap(m_map, m_height) ;
	m_map = newMap ;
	int temp = m_width ;
	m_width = m_height ;
	m_height = temp ;
}

//flips the map counter-clockwise
void Map::flipCCW( )
{
	m_rot = CCW ;
	char ** newMap = new2Dmap( m_width, m_height ) ;

	for( int row = 0; row < m_height; ++row )
	{
		for( int col = 0; col < m_width; ++col ) 
		{
			newMap[col][row] = m_map[row][(m_width-1)-col] ;
		}
	}
	delete2Dmap(m_map, m_height) ;
	m_map = newMap ;
	int temp = m_width ;
	m_width = m_height ;
	m_height = temp ;
}

//resets the map to its original spawn position
void Map::reset( )
{
	m_pos.x = SPAWN_POS_X ;
	m_pos.y = SPAWN_POS_Y ;
}

//deletes the 2D map data safely
void Map::release( )
{
	if( m_map )
	{
		delete2Dmap( m_map, m_height ) ;
		m_height = 0 ;
		m_width = 0 ;
	}
}