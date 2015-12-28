#pragma once

#include <conio.h>
using namespace std ;

#include "ResourceManager.h"
#include "MapController.h"

class GameController
{
private:
	Map * map ;
	Map * piece ;
	bool m_running ;
	int tick ;
	int timePerFrame ;

	ResourceManager * res ;
public:
	GameController() ;

	~GameController() { res->release() ;}

	void update() ;

	//find and initialize files here
	void init() ;

	void draw( ) ;

	bool isRunning( ) ;
} ;