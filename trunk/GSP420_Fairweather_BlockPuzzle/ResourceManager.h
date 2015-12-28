#pragma once
///ResourceManager responsible for handling resources

#include <vector>
#include <cmath>
#include <ctime>

#include "MapController.h"

#define STRING					0
#define MAP						1

/** service oreiented architecture for a resource manager **/
class ResourceManager
{
private:
	ResourceManager() {}
	static ResourceManager * s_instance ;
	//resource data
	struct ResEntry
	{ 
		char* name ; 
		void* data ; 
		int type ;
		
		ResEntry():name(0), data(0), type(0) {}
		ResEntry( char * name, void * data, int type ) : name(name), data(data), type(type) {}
	} ;
	//vector array variable for inventory
	vector<ResEntry> inventory ;
public:
	//instance function
	static ResourceManager * instance()
	{
		if(!s_instance)
			s_instance = new ResourceManager() ;
		return s_instance ;
	}

	//loads resource and adds it to inventory
	int loadResource( char * source, int a_type )
	{
		switch(a_type)
		{
		case STRING:
			inventory.push_back( ResEntry(source, source, STRING) ) ;
			break ;
		case MAP:
			{
				Map * m = new Map() ;
				m->loadFile(source) ;
				inventory.push_back( ResEntry(source, m, MAP)) ;
			}
			break ;
		default:
			throw "does not support resource type" ;
			{int i=0; i=1/i;}
		}
	}
	//searches for resource based on its name
	int getIndex( char * name )
	{
		for( unsigned int i = 0; i < inventory.size(); ++i )
			if( strcmp(inventory[i].name, name) == 0 )
				return i ;
		return -1 ;
	}
	//searches for the STRING resource based on its id number
	char * getString( int id )
	{
		if( id >= 0 && (unsigned int)id < inventory.size() && inventory[id].type == STRING )
			return (char*)inventory[id].data ;
		return 0 ;
	}
	//searches for a MAP resource based on id number
	Map * getMap( int id )
	{
		if( id >= 0 && (unsigned int)id < inventory.size() && inventory[id].type == MAP )
			return (Map*)inventory[id].data ;
	}

	//releases all resources
	void release() 
	{
		for(unsigned int i = 0; i < inventory.size(); ++i )
		{
			switch(inventory[i].type)
			{
			case STRING:
				break ;
			case MAP:
				{
					Map * m = (Map*)inventory[i].data ;
					delete m ;
				}
				break ;
			}
		}
	}
} ;

ResourceManager * ResourceManager::s_instance = 0 ;