#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Level.h"
#include "Actor.h"
#include <sstream>
#include <iomanip>
#include <stack>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath)
{
	m_player = nullptr;
	numCitizens = 0;
	game_level_finished = false;
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}
int StudentWorld::init()
{
	// Initialize the data structures used to keep track of your game’s world
	// and return proper state based on the data file format
	Level lev(assetPath());
	ostringstream oss;
	oss << "level" << setw(2) << setfill('0') << getLevel() << ".txt";
	Level::LoadResult result = lev.loadLevel(oss.str());
	if (result == Level::load_fail_file_not_found)
	{
		// no more lvel file with this level number
		return GWSTATUS_PLAYER_WON;
	}
	else if (result == Level::load_fail_bad_format)
	{
		return  GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_success)
	{
		//cerr << "Successfully loaded level" << endl;
		createActors(lev);
		return GWSTATUS_CONTINUE_GAME;
	}

	return  GWSTATUS_LEVEL_ERROR;
}

void StudentWorld::createActors(Level &lev)
{
	//  create actor based on Allocate the current level’s data file
	//  save all of these actors in a vector
	for (int r = 0; r < LEVEL_WIDTH; r++)
	{
		for (int c = 0; c < LEVEL_HEIGHT; c++)
		{
			Level::MazeEntry ge = lev.getContentsOf(r, c); // level_x=5, level_y=10
			switch (ge) // so x=80 and y=160
			{
			case Level::empty:
				break;
			case Level::smart_zombie:
				m_actors.push_back(new SmartZombie(this, IID_ZOMBIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT));
				break;
			case Level::dumb_zombie:
				m_actors.push_back(new DumbZombie(this, IID_ZOMBIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT));
				break;
			case Level::player:
				m_player = new Penelope(this, IID_PLAYER, r*SPRITE_WIDTH, c*SPRITE_HEIGHT);
				m_actors.push_back(m_player);
				break;
			case Level::exit:
				m_actors.push_back(new Exit(this, IID_EXIT, r*SPRITE_WIDTH, c*SPRITE_HEIGHT));
				//cout << "Location 80,160 is where an exit is" << endl;
				break;
			case Level::wall:
				m_actors.push_back(new Wall(this, IID_WALL, r*SPRITE_WIDTH, c*SPRITE_HEIGHT));
				break;
			case Level::pit:
				m_actors.push_back(new Pit(this, IID_PIT, r*SPRITE_WIDTH, c*SPRITE_HEIGHT));
				break;
			case Level::citizen:
				numCitizens++;
				m_actors.push_back(new Citizen(this, IID_CITIZEN, r*SPRITE_WIDTH, c*SPRITE_HEIGHT));
				break;
			case Level::vaccine_goodie:
				m_actors.push_back(new VaccineGoodie(this, IID_VACCINE_GOODIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT));
				break;
			case Level::gas_can_goodie:
				m_actors.push_back(new GasCanGoodie(this, IID_GAS_CAN_GOODIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT));
				break;
			case Level::landmine_goodie:
				m_actors.push_back(new LandmineGoodie(this, IID_LANDMINE_GOODIE, r*SPRITE_WIDTH, c*SPRITE_HEIGHT));
				break;
			}
		}
	}
	//cout << "size of" << m_actors.size() << endl;
}

void StudentWorld::moveActor(Actor *actor, double newX, double newY)
{
	// move actor to a new location newX, newY if possible
	for (vector<Actor*>::iterator b = m_actors.begin(); b != m_actors.end(); b++)
	{
		if (*b == actor) continue;  // skip itself
		if ((*b)->isDead()) continue;
		if ((*b)->blocksMovement())
			if ((*b)->isAgentMovementBlockedAt(newX, newY))
				return;
	}

	// reach here, no actors block actor a, so actor moves to new location
	actor->moveTo(newX, newY);
}


bool StudentWorld::overlapWallExit(const double x, const double y)
{
	// check if any wall and exit overlap position x, y, use for flame generation
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isDead()) continue;
		if ((*it)->blocksFlame() && (*it)->overlap(x, y)) return true;
	}

	return false;
}

bool StudentWorld::citizenFoundExit(const Actor *eXit)
{
	// check if citizen find exit, if yes, remove the citizen from the list
	// and return true, otherwise, return false
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isDead()) continue;
		if ((*it)->canExit() && (*it)->overlap(eXit))
		{
			(*it)->setDead();
			playSound(SOUND_CITIZEN_SAVED);
			return true;
		}
	}

	return false;
}

void StudentWorld::overlapPitFlame(const Actor *flame)
{
	// check if any object overlap with Pit or flame, if yes, act properly, say die
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isDead()) continue;
		if ((*it)->overlap(flame))
		{
			(*it)->dieByFallOrBurnIfAppropriate();
		}
	}
}

void StudentWorld::overlapVomit(const Actor *vomit)
{
	// check if any object overlap with vomit,  if yes, act properly, sa get infected
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isDead()) continue;

		if ((*it)->overlap(vomit))
		{
			(*it)->beVomitedOnIfAppropriate();
		}
	}
}

bool StudentWorld::overlapLandmine(Actor *landmine)
{
	// check if any object overlap with landmine, if yes, trigger landmine
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isDead()) continue;
		if (!(*it)->triggersOnlyActiveLandmines()) continue;
		if ((*it)->overlap(landmine)) return true;
	}

	return false;
}

bool StudentWorld::citizensGone()
{
	// check if no live citizen, if no live citizen, return true, otherwise, return false
	return numCitizens == 0;
}

bool StudentWorld::blockingMovement(const Actor *actor, double new_x, double new_y)
{
	// check if any object block actor moving to a location new_x, new_y
	// if yes, return true, otherwise, return false
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isDead()) continue;
		if (*it == actor) continue;  // except itself
		if ((*it)->blocksMovement() && (*it)->isAgentMovementBlockedAt(new_x, new_y))
			return true;
	}

	return false;
}

bool StudentWorld::personInFrontZombie(const Actor * zombie)
{
	// check if any person in front of a zombie,  if yes, return true, otherwise, return false
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isDead()) continue;
		if (!((*it)->triggersZombieVomit())) continue;  // skip non human objects

		switch (zombie->getDirection())
		{
		case GraphObject::right:
			if (distance((*it)->getY(), zombie->getY()) < SPRITE_HEIGHT && (*it)->getX() > zombie->getX())
				return true;
			break;
		case GraphObject::left:
			if (distance((*it)->getY(), zombie->getY()) < SPRITE_HEIGHT && (*it)->getX() < zombie->getX())
				return true;
			break;
		case GraphObject::up:
			if (distance((*it)->getX(), zombie->getX()) < SPRITE_WIDTH && (*it)->getY() > zombie->getY())
				return true;
			break;
		case GraphObject::down:
			if (distance((*it)->getX(), zombie->getX()) < SPRITE_WIDTH && (*it)->getY() < zombie->getY())
				return true;
			break;
		}
	}

	return false;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//decLives();

	int numActors = m_actors.size();  // use size of actors for after last tick, not include actors created in this tick
	for (int i = 0; i < numActors; i++)
	{
		if (!m_actors[i]->isDead())
			m_actors[i]->doSomething();
	}

	// update status line text
	// Score: 004500 Level: 27 Lives: 3 Vaccines: 2 Flames: 16 Mines: 1 Infected: 0
	ostringstream oss;
	//oss.fill('0');
	oss << "Score: " << setw(6) << setfill('0') << std::internal << getScore() << "  Level: " << getLevel()
		<< "  Lives: " << getLives() << "  Vaccines: " << m_player->getNumVaccines()
		<< "  Flames: " << m_player->getNumFlameCharges()
		<< "  Mines: " << m_player->getNumLandmines()
		<< "  Infected: " << m_player->getInfectionDuration();
	//numVaccines, numLandmines, numFlamethrowers;
	setGameStatText(oss.str());

	if (m_player->isDead()) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	removeDeadActors();

	if (game_level_finished) {
		playSound(SOUND_LEVEL_FINISHED);
		return GWSTATUS_FINISHED_LEVEL;
	}


	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::addActor(Actor* actor)
{
	// add a new actor
	m_actors.push_back(actor);
}

void StudentWorld::removeDeadActors()
{
	// remove any dead actor
	stack<int> removeItems;

	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i]->isDead())
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
	// cleanup memory, delete all actors, and clear the vector of actor pointers, reset member variables

	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		delete *it;
	}
	m_actors.clear();
	m_player = nullptr; // not need to delete m_player because it is also included in m_actors, already deleted in above loop
	numCitizens = 0;
	game_level_finished = false;
}

int StudentWorld::getNumZombie()
{
	// count number of zombies
	int count = 0;

	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isDead()) continue;
		if ((*it)->mayVomit()) count++;
	}
	return count;
}

Penelope *StudentWorld::player()
{
	return m_player;
}

void StudentWorld::setGameLevelFinished()
{
	game_level_finished = true;
}

double StudentWorld::distanceToNearestZombie(const Actor *actor)
{
	return distanceToNearestZombie(actor->getX(), actor->getY());
}

double StudentWorld::distanceToNearestZombie(const double x, const double y)
{
	// compute distance to the nearest zombie from current location, if no zombie exit, return a huge number
	double min_dist = 1.0E+10;

	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isDead()) continue;
		if ((*it)->mayVomit()) min_dist = std::min(min_dist, (*it)->distanceTo(x, y));
	}
	return min_dist;
}

double StudentWorld::distanceToNearestPerson(const double x, const double y)
{
	// compute distance to the nearest people from current location, if no people exit, return a huge number
	double min_dist = 1.0E+10;  // set to a large number

	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isDead()) continue;
		if ((*it)->triggersZombieVomit())
			min_dist = std::min(min_dist, (*it)->distanceTo(x, y));
	}
	return min_dist;
}

Actor *StudentWorld::findNearestPerson(const double x, const double y)
{
	// find the nearest person from current location, retunr pointer to the person
	Actor *nearestPerson = nullptr;

	double min_dist = 1.0E+10;  // set to a large number

	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if ((*it)->isDead()) continue;
		if (!(*it)->triggersZombieVomit()) continue;
		if ((*it)->distanceTo(x, y) < min_dist)
		{
			min_dist = (*it)->distanceTo(x, y);
			nearestPerson = *it;
		}
	}

	return nearestPerson;
}

void StudentWorld::decreaseCitizen()
{
	numCitizens--;
}