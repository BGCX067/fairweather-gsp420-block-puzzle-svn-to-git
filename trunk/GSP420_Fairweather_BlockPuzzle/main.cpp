#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string.h>
using namespace std ;

#include "ResourceManager.h"
#include "GameController.h"

///Game Class responsible for game init, update, and release. It also handles player score and resource management
class Game
{
private:
	//map variables
	Map * map ;
	Map * piece ;
	//boolean that breaks game loop if false
	bool m_running ;
	//updates frame if it reaches a certain point
	int tick ;
	int timePerFrame ;
	//score in the game
	int m_score ;
	//Resource Manager
	ResourceManager * res ;

	//initial time-per-frame speed
	#define INITIAL_SPEED	50000

	//enumerations for game speed and score threshold
	enum speedPerFrame { SPEED_1 = 40000, SPEED_2 = 35000, SPEED_3 = 30000, SPEED_4 = 25000, SPEED_5 = 20000 } ; //updates when told to by the score threshold
	enum scoreThreshold { FIRST = 1000, SECOND = 2000, THIRD = 3000, FOURTH = 4000, FIFTH = 5000} ; //updates the time-per-frame when score reaches or excels a certain value
public:
	Game( ){}
	~Game( ) { res->release() ; }

	int getScore() { return m_score ; }

	//updates game data
	void update() 
	{
		//if user presses ANY key
		if( _kbhit() )
		{
			//get input
			char input = _getch() ;

			//move or flip piece based on button pressed or exit the game
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

			//ask for forgiveness
			piece->forgiveness( map ) ;

			//update map
			draw( ) ;
		}

		//add tick by one frame
		tick++ ;

		//if tick reaches a certain time, move piece down and update data
		if( tick == timePerFrame )
		{
			piece->moveDown( ) ;
			piece->forgiveness( map ) ;
			draw( ) ;
			tick = 0 ;
		}

		//if lines are cleared, add 10 for each line cleared
		m_score += 100 * map->checkLine() ;

		if( m_score >= FIRST )
		{
			if( m_score < SECOND )
				timePerFrame = SPEED_1 ;
			else if( m_score < THIRD )
				timePerFrame = SPEED_2 ;
			else if( m_score < FOURTH )
				timePerFrame = SPEED_3 ;
			else if( m_score < FIFTH )
				timePerFrame = SPEED_4 ;
			else
				timePerFrame = SPEED_5 ;
		}

		//if piece is ready for the next map
		if( piece->readyForNextMap( ) )
		{
			//get new map
			piece = res->getMap(rand() % 7 + 1) ;

			//draw updated map
			draw( ) ;
		}

		//check for loss condition
		if( piece->lossCondition( map ) )
			m_running = false ;

		//draw( ) ;

	}
	//find and initialize files here
	void init() 
	{
		res = ResourceManager::instance() ;

		res->loadResource("Map.txt", MAP) ;
		res->loadResource("Pieces/Block.txt", MAP) ;
		res->loadResource("Pieces/LBlock.txt", MAP) ;
		res->loadResource("Pieces/LinePiece.txt", MAP) ;
		res->loadResource("Pieces/RLBlock.txt", MAP) ;
		res->loadResource("Pieces/RSquiggle.txt", MAP) ;
		res->loadResource("Pieces/Squiggle.txt", MAP) ;
		res->loadResource("Pieces/Tri.txt", MAP) ;

		srand(unsigned(time(NULL))) ;

		//gets the map data
		map = res->getMap(res->getIndex("Map.txt")) ;
		piece = res->getMap(rand() % 7 + 1) ;

		//sets time to 0
		tick = m_score = 0 ;
		timePerFrame = INITIAL_SPEED ;

		//draw initial data
		draw( ) ;
	}

	//tells maps to draw
	void draw( )
	{
		map->draw( ) ;
		map->drawScore( m_score ) ;
		piece->draw( ) ;
	}

	bool isRunning( )
	{
		return m_running ;
	}
} ;

int main()
{
	//initialize resources
	Game * game = new Game() ;
	game->init() ;

	//game loop
	while(game->isRunning())
	{
		//update game
		game->update( ) ;

	}

	//clear screen for GAME OVER display
	system( "cls" ) ;

	cout << "GAME OVER" << endl ;

	//Sleep for 2 seconds
	Sleep(2000) ;

	cout << "FINAL SCORE: " << game->getScore() << endl ;

	//delete all data
	delete game ;

	//Sleep again for 2 seconds
	Sleep(2000) ;

	//show credits
	cout <<	"-----------------------------------------------\n" << 
			"Block Puzzle Game Created by: David Fairweather\n" <<
			"-----------------------------------------------\n" <<
			"Created for GSP420 with Professor Vaganov\n" <<
			"-----------------------------------------------\n" ;

	return 0 ;
}