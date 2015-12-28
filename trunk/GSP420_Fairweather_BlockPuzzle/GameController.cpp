#include "GameController.h"

GameController::GameController( )
{
	init() ;
	res = ResourceManager::instance() ;
}

void GameController::update() 
{
	if( _kbhit() )
	{
		char input = _getch() ;

		switch(input)
		{
		case 'a':
			piece->moveLeft( ) ;
			break ;
		case 'd':
			piece->moveRight( ) ;
			break ;
		case 's':
			piece->moveDown( ) ;
			tick = 0 ;
			break ;
		case 'e':
			piece->flipCW( ) ;
			break ;
		case 'q':
			piece->flipCCW( ) ;
			break ;
		case 27:
			m_running = false ;
			break ;
		}

		piece->forgiveness( map ) ;

		draw( ) ;
	}

	tick++ ;

	if( tick == timePerFrame )
	{
		piece->moveDown( ) ;
		piece->forgiveness( map ) ;
		draw( ) ;
		tick = 0 ;
	}

	map->checkLine( ) ;

	if( piece->readyForNextMap( ) )
	{
		piece = res->getMap(rand() % 7 + 1) ;

		if( piece->lossCondition( map ) )
			m_running = false ;

		draw( ) ;
	}

	//draw( ) ;

}
//find and initialize files here
void GameController::init() 
{
	res->loadResource("Map.txt", MAP) ;
	res->loadResource("Pieces/Block.txt", MAP) ;
	res->loadResource("Pieces/LBlock.txt", MAP) ;
	res->loadResource("Pieces/LinePiece.txt", MAP) ;
	res->loadResource("Pieces/RLBlock.txt", MAP) ;
	res->loadResource("Pieces/RSquiggle.txt", MAP) ;
	res->loadResource("Pieces/Squiggle.txt", MAP) ;
	res->loadResource("Pieces/Tri.txt", MAP) ;

	srand(unsigned(time(NULL))) ;

	map = res->getMap(res->getIndex("Map.txt")) ;
	piece = res->getMap(rand() % 7 + 1) ;

	tick = 0 ;
	timePerFrame = INITIAL_TIME_PER_FRAME ;

	draw( ) ;
}

void GameController::draw( )
{
	map->draw( ) ;
	piece->draw( ) ;
}

bool GameController::isRunning( )
{
	return m_running ;
}