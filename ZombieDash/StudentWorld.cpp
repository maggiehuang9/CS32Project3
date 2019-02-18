#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Level.h"
#include "Actor.h"
#include <sstream>
#include <iomanip>
using namespace std;

double distance(double x1, double x2)
{
	return x1 > x2 ? x1 - x2 : x2 - x1;
}

string intToString(int k)
{
	ostringstream oss;
	oss.fill('0');
	oss << setw(2) << k;
	return oss.str();
}
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
	m_player = nullptr;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}
int StudentWorld::init()
{
	Level lev(assetPath());
	loadLevelFile(lev);
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::loadLevelFile(Level &lev)
{
	int level = getLevel();
	string levelFile = "level" + intToString(level) + ".txt";
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find level01.txt data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;
		createActors(lev);
		for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
		{
			(*it)->setWorld(this);
		}
	}
}

void StudentWorld::createActors(Level &lev)
{
	cerr << "Successfully loaded level" << endl;
	for (int r = 0; r < LEVEL_WIDTH; r++)
	{
		for (int c = 0; c < LEVEL_HEIGHT; c++)
		{
			Level::MazeEntry ge = lev.getContentsOf(r, c); // level_x=5, level_y=10
			switch (ge) // so x=80 and y=160
			{
			case Level::empty:
				//cout << "Location 80,160 is empty" << endl;
				break;
			case Level::smart_zombie:
				m_actors.push_back(new SmartZombie(IID_ZOMBIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0, 1.0));
				//cout << "Location 80,160 starts with a smart zombie" << endl;
				break;
			case Level::dumb_zombie:
				m_actors.push_back(new DumbZombie(IID_ZOMBIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0, 1.0));
				//cout << "Location 80,160 starts with a dumb zombie" << endl;
				break;
			case Level::player:
				m_actors.push_back(new Penelope(IID_PLAYER, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0, 1.0));
				//cout << "Location 80,160 is where Penelope starts" << endl;
				break;
			case Level::exit:
				m_actors.push_back(new Exit(IID_EXIT, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0, 1.0));
				//cout << "Location 80,160 is where an exit is" << endl;
				break;
			case Level::wall:
				m_actors.push_back(new Wall(IID_WALL, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0, 1.0));
				//cout << "Location 80,160 holds a Wall" << endl;
				break;
			case Level::pit:
				m_actors.push_back(new Pit(IID_PIT, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0, 1.0));
				//cout << "Location 80,160 has a pit in the ground" << endl;
				break;
			case Level::citizen:
				m_actors.push_back(new Citizen(IID_CITIZEN, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0, 1.0));
				//cout << "Location 80,160 has a pit in the ground" << endl;
				break;
			case Level::vaccine_goodie:
				m_actors.push_back(new VaccineGoodie(IID_VACCINE_GOODIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0, 1.0));
				//cout << "Location 80,160 has a pit in the ground" << endl;
				break;
			case Level::gas_can_goodie:
				m_actors.push_back(new GasCanGoodie(IID_GAS_CAN_GOODIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0, 1.0));
				//cout << "Location 80,160 has a pit in the ground" << endl;
				break;
			case Level::landmine_goodie:
				m_actors.push_back(new LandmineGoodie(IID_LANDMINE_GOODIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0, 1.0));
				//cout << "Location 80,160 has a pit in the ground" << endl;
				break;
			}
		}
	}
	cout << "size of" << m_actors.size() << endl;
}

void StudentWorld::moveActor(Actor &actor, double newX, double newY)
{

	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if (*it == &actor) continue;

		switch (actor.getDirection())
		{
		case GraphObject::left:
			if ((*it)->getX() > actor.getX()) break;
			if (distance(actor.getY(), (*it)->getY()) > SPRITE_HEIGHT / 2) break;
			if ((actor.getX() - (*it)->getX()) <= SPRITE_WIDTH) return;
			break;
		case GraphObject::right:
			if ((*it)->getX() < actor.getX()) break;
			if (distance(actor.getY(), (*it)->getY()) > SPRITE_HEIGHT / 2) break;
			if (((*it)->getX()) - actor.getX() <= SPRITE_WIDTH) return;
			break;
		case GraphObject::down:
			if ((*it)->getY() > actor.getY()) break;
			if (distance(actor.getX(), (*it)->getX()) > SPRITE_WIDTH / 2) break;
			if ((actor.getY() - (*it)->getY()) <= SPRITE_HEIGHT) return;
			break;
		case GraphObject::up:
			if ((*it)->getY() < actor.getY()) break;
			if (distance(actor.getX(), (*it)->getX()) > SPRITE_WIDTH / 2) break;
			if (((*it)->getY() - actor.getY()) <= SPRITE_HEIGHT) return;
			break;
		}
	}
	
	actor.moveTo(newX, newY);
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		(*it)->doSomething();
	}
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	if(m_player != nullptr)
		delete m_player;
	m_player = nullptr;
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		delete *it;
	}
	m_actors.clear();
}
