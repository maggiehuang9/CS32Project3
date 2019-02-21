#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Level.h"
#include "Actor.h"
#include <sstream>
#include <iomanip>
#include <stack>
using namespace std;

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
	score = numLives = numInfected = 0;
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
				m_actors.push_back(new SmartZombie(IID_ZOMBIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0));
				//cout << "Location 80,160 starts with a smart zombie" << endl;
				break;
			case Level::dumb_zombie:
				m_actors.push_back(new DumbZombie(IID_ZOMBIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0));
				//cout << "Location 80,160 starts with a dumb zombie" << endl;
				break;
			case Level::player:
				m_player = new Penelope(IID_PLAYER, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0);
				m_actors.push_back(m_player);
				//cout << "Location 80,160 is where Penelope starts" << endl;
				break;
			case Level::exit:
				m_actors.push_back(new Exit(IID_EXIT, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0));
				//cout << "Location 80,160 is where an exit is" << endl;
				break;
			case Level::wall:
				m_actors.push_back(new Wall(IID_WALL, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0));
				//cout << "Location 80,160 holds a Wall" << endl;
				break;
			case Level::pit:
				m_actors.push_back(new Pit(IID_PIT, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0));
				//cout << "Location 80,160 has a pit in the ground" << endl;
				break;
			case Level::citizen:
				m_actors.push_back(new Citizen(IID_CITIZEN, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0));
				//cout << "Location 80,160 has a pit in the ground" << endl;
				break;
			case Level::vaccine_goodie:
				m_actors.push_back(new VaccineGoodie(IID_VACCINE_GOODIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0));
				//cout << "Location 80,160 has a pit in the ground" << endl;
				break;
			case Level::gas_can_goodie:
				m_actors.push_back(new GasCanGoodie(IID_GAS_CAN_GOODIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0));
				//cout << "Location 80,160 has a pit in the ground" << endl;
				break;
			case Level::landmine_goodie:
				m_actors.push_back(new LandmineGoodie(IID_LANDMINE_GOODIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT, GraphObject::right, 0));
				//cout << "Location 80,160 has a pit in the ground" << endl;
				break;
			}
		}
	}
	cout << "size of" << m_actors.size() << endl;
}

void StudentWorld::moveActor(Actor &a, double newX, double newY)
{
	for (vector<Actor*>::iterator b = m_actors.begin(); b != m_actors.end(); b++)
	{
		if (*b == &a) continue;  // skip itself
		if (!(*b)->isAlive()) continue;

		if ((*b)->isGoodie() || (*b)->isExit())
			int i = 0; // do nothing
		else if ((*b)->isLandmine())
			a.moveTo(newX, newY);
		else if ((*b)->isFlame())
			if ((*b)->overlap(a))
			{
				a.setState(false);  // actor a is died due to flame
				break;
			}
			else if (blockingMovemenet(a, *(*b))) return; // check if actor a blocks actor a, if yes, actor a cannot move, just return
			//}

	}
	// reach here, no actors block actor a, so a move to new location
	a.moveTo(newX, newY);
}


bool StudentWorld::overlapWallExit(const double x, const double y)
{
	// check if any wall and exit overlap position x, y
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if (!(*it)->isWall() && !(*it)->isExit()) continue;
		if ((*it)->overlap(x, y)) return true;
	}

	return false;
}

bool StudentWorld:: exitFound(Actor* actor)
{
	int x = actor->getX();
	int y = actor->getY();
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isExit())
		{
			if ((*it)->overlap(*actor))
			{
				return true;
			}
				
		}
	}
	return false;
}

bool StudentWorld::citizensGone()
{
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isCitizen())
		{
			return false;
		}
	}
	return true;
}


//bool StudentWorld::blockingMovemenet(const Actor& a, const Actor &b)
//{
//	// check if actor b blocks actor a, if yes, return true, otherwise, return false
//	return (distance(b.getX(), a.getX()) < SPRITE_WIDTH-5 &&
//		distance(b.getY(), a.getY()) < SPRITE_HEIGHT-5);
//}


bool StudentWorld::blockingMovemenet(const Actor& a, const Actor &b)
{
	//check if actor b blocks actor a, if yes, return true, otherwise, return false
	switch (a.getDirection())
	{
	case GraphObject::left:
		if (b.getX() > a.getX()) return false;
		if (distance(a.getY(), b.getY()) >= SPRITE_HEIGHT) return false;
		if ((a.getX() - b.getX()) < SPRITE_WIDTH + 4) return true;
		return false;
	case GraphObject::right:
		if (b.getX() < a.getX()) return false;
		if (distance(a.getY(), b.getY()) >= SPRITE_HEIGHT) return false;
		if ((b.getX()) - a.getX() < SPRITE_WIDTH + 4) return true;
		return false;
	case GraphObject::down:
		if (b.getY() > a.getY()) return false;
		if (distance(a.getX(), b.getX()) >= SPRITE_WIDTH) return false;
		if ((a.getY() - b.getY()) < SPRITE_HEIGHT + 4) return true;
		return false;
	case GraphObject::up:
		if (b.getY() < a.getY()) return false;
		if (distance(a.getX(), b.getX()) >= SPRITE_WIDTH) return false;
		if ((b.getY() - a.getY()) < SPRITE_HEIGHT + 4) return true;
		return false;
	}

	return false;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->overlap(*m_player))
		{

			if ((*it)->isLandmineGoodie())
			{
				m_player->addMine(2);
				(*it)->setState(false);
			}
			else if ((*it)->isVaccineGoodie())
			{
				m_player->addVaccine(1);
				(*it)->setState(false);
			}
			else if ((*it)->isGasCanGoodie())
			{
				m_player->addFlame(5);
				(*it)->setState(false);
			}
		}

		(*it)->doSomething();
	}

	// update status line text
	// Score: 004500 Level: 27 Lives: 3 Vaccines: 2 Flames: 16 Mines: 1 Infected: 0
	ostringstream oss;
	//oss.fill('0');
	oss << "Score: " << score << "  Level: " << getLevel()
		<< "  Lives: " << numLives << "  Vaccines: " << m_player->getNumVaccine()
		<< "  Flames: " << m_player->getNumFlame()
		<< "  Mines: " << m_player->getNumMine()
		<< "  Infected: " << numInfected;

	setGameStatText(oss.str());

	if (!m_player->isAlive()) 
		return GWSTATUS_PLAYER_DIED;

	removeDeadActors();
    
	if (m_player->foundExit())
		return GWSTATUS_FINISHED_LEVEL;

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::addActor(Actor* actor)
{
	m_actors.push_back(actor);
}

void StudentWorld::removeDeadActors()
{
	stack<int> removeItems;

	for (int i = 0; i< m_actors.size(); i++)
	{
		if (!m_actors[i]->isAlive())
			removeItems.push(i);
	}

	while (!removeItems.empty())
	{
		int i = removeItems.top();
		Actor *removePointer = m_actors[i];
		m_actors.erase(m_actors.begin() + i);
		delete removePointer;
		removeItems.pop();
	}

};

void StudentWorld::cleanUp()
{

	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		delete *it;
	}
	m_actors.clear();

	//if (m_player != nullptr)  // not need to delete m_player because it is also included in m_actors, already deleted in above loop
	//	delete m_player;
	m_player = nullptr;
	score = numLives = numInfected = 0;
}
