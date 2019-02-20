#include "Actor.h"
#include "StudentWorld.h"
#include "SoundFX.h"

const double STEP_SIZE = 2;
const double OVERLAP_LIMIT = 100;

Actor::Actor(int imageID, double startX, double startY,Direction dir, int depth, double size)
	:GraphObject(imageID, startX, startY, dir, depth, size)
{
	m_alive = true;
}

Actor::~Actor()
{
}

void Actor::move(double newX, double newY)
{
	getWorld()->moveActor(*this, newX, newY);
}
void Actor::setWorld(StudentWorld* world)
{
	m_world = world;
}

StudentWorld* Actor::getWorld()
{
	return m_world;
}

bool Actor::isAlive()
{
	return m_alive;
}

void Actor::setState(bool state)
{
	m_alive = state;
}

void Actor::doSomething()
{

}

bool Actor::overlap(const double x, const double y)
{
	// check if this actor overlap with location x, y
	return (pow(getX() - x, 2) + pow(getY() - y, 2) < OVERLAP_LIMIT);
}

bool Actor::overlap(const Actor &other)
{
	// check if this actor overlap with other
	return (pow(getX() - other.getX(), 2) + pow(getY() - other.getY(), 2) < OVERLAP_LIMIT);
}

bool Actor::isGoodie()
{
	return false;
}

Penelope::Penelope( int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Actor(imageID, startX, startY, dir, depth,size)
{
	numLandmines = 0;
	numFlamethrowers = 0;
	numVaccines = 0;
	m_infected = false;
	m_infectionCount = 0;
	setState(true);
}

Penelope::~Penelope()
{

}

void Penelope::addFlame(int n)
{
	cout << " addFlame " << n << endl;
	numFlamethrowers += n;
	cout << " addFlame " << numFlamethrowers << endl;
}

void Penelope::addMine(int n)
{
	numLandmines += n;
}
void Penelope::addVaccine(int n)
{
	numVaccines += n;
}

int Penelope::getNumFlame()
{
	return numFlamethrowers;
}
int Penelope::getNumMine()
{
	return numLandmines;
}
int Penelope::getNumVaccine()
{
	return numVaccines;
}

void Penelope::createFlame()
{
	double px[3], py[3];
	switch (getDirection())
	{
	case right:
		px[0] = getX() +   SPRITE_WIDTH;
		px[1] = getX() + 2*SPRITE_WIDTH;
		px[2] = getX() + 3*SPRITE_WIDTH;
		py[0] = py[1] = py[2] = getY();
		break;
	case left:
		px[0] = getX() -     SPRITE_WIDTH;
		px[1] = getX() - 2 * SPRITE_WIDTH;
		px[2] = getX() - 3 * SPRITE_WIDTH;
		py[0] = py[1] = py[2] = getY();
		break;
	case up:
		px[0] = px[1] = px[2] = getX();
		py[0] = getY() +     SPRITE_HEIGHT;
		py[1] = getY() + 2 * SPRITE_HEIGHT;
		py[2] = getY() + 3 * SPRITE_HEIGHT;
		break;
	case down:
		px[0] = px[1] = px[2] = getX();
		py[0] = getY() - SPRITE_HEIGHT;
		py[1] = getY() - 2 * SPRITE_HEIGHT;
		py[2] = getY() - 3 * SPRITE_HEIGHT;
		break;
	}

	for (int i = 0; i < 3; i++)
	{
		if (getWorld()->overlapWallExit(px[i], py[i]))break;
		getWorld()->addActor(new Flame(IID_FLAME, px[i], py[i]));
	}
		

}

void Penelope::createLandmine()
{
	getWorld()->addActor(new Landmine(IID_LANDMINE, getX(), getY()));
}

void Penelope::doSomething()
{
	if (!isAlive())
		return;
	if (m_infected)
	{
		m_infectionCount++;
		if (m_infectionCount == 500)
		{
			setState(false);
			getWorld()->playSound(SOUND_PLAYER_DIE);
			return;
		}
	}
	int ch;
	if (getWorld()->getKey(ch))
	{
		// user hit a key during this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			setDirection(left);
			move(getX() - 4, getY());
			//... move Penelope to the left ...;
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			move(getX() + 4, getY());
			//... move Penelope to the right ...;
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			move(getX(), getY() + 4);
			//... move Penelope up ...;
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			move(getX(), getY() - 4);
			//... move Penelope down...;
			break;
		case KEY_PRESS_SPACE:
			//createFlame();
			//getWorld()->playSound(SOUND_PLAYER_FIRE);
			if (numFlamethrowers > 0)
			{
				numFlamethrowers--;
				createFlame();
				getWorld()->playSound(SOUND_PLAYER_FIRE);
			}
			//... add flames in front of Penelope...;
			break;
		case KEY_PRESS_TAB:
			if (numLandmines > 0)
			{
				numLandmines--;
				createLandmine();
			}
			//... add landmines in front of Penelope...;
			break;
		case KEY_PRESS_ENTER:
			if (numVaccines > 0)
			{
				numVaccines--;
				m_infected = false;
			}
			//... add vaccines...;
			break;
		}
	}
}

Actor::actorType Penelope::getType()
{
	return player;
}

Wall::Wall( int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Actor(imageID, startX, startY, dir, depth, size)
{

}

Wall::~Wall()
{

}

Actor::actorType Wall::getType()
{
	return wall;
}

Exit::Exit( int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Actor(imageID, startX, startY, dir, depth, size)
{

}

Exit::~Exit()
{

}

void Exit::doSomething()
{

}

Actor::actorType Exit::getType()
{
	return exit;
}

Pit::Pit( int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Actor(imageID, startX, startY, dir, depth, size)
{

}

Pit::~Pit()
{

}

void Pit::doSomething()
{

}

Actor::actorType Pit::getType()
{
	return pit;
}

Flame::Flame( int imageID, double startX, double startY,Direction dir, int depth, double size)
	: Actor(imageID, startX, startY, dir, depth, size)
{
	numTicks = 0;
}

Flame::~Flame()
{

}

void Flame::doSomething()
{
	if (numTicks >= 2)
		setState(false);
	numTicks++;
}

Actor::actorType Flame::getType()
{
	return flame;
}

Vomit::Vomit( int imageID, double startX, double startY,Direction dir, int depth, double size)
	:Actor(imageID, startX, startY, dir, depth, size)
{

}

Vomit::~Vomit()
{

}

void Vomit::doSomething()
{

}

Actor::actorType Vomit::getType()
{
	return vomit;
}

Goodie::Goodie(int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Actor(imageID, startX, startY, dir, depth, size)
{

}

Goodie::~Goodie()
{

}

bool Goodie::isGoodie()
{
	return true;
}

Actor::actorType Goodie::getType()
{
	return goodie;
}

VaccineGoodie::VaccineGoodie( int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Goodie(imageID, startX, startY, dir, depth, size)
{

}

VaccineGoodie::~VaccineGoodie()
{

}

void VaccineGoodie::doSomething()
{

}

Actor::actorType VaccineGoodie::getType()
{
	return vaccine_goodie;
}

GasCanGoodie::GasCanGoodie(int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Goodie(imageID, startX, startY, dir, depth, size)
{
}

GasCanGoodie::~GasCanGoodie()
{

}

void GasCanGoodie::doSomething()
{
}

Actor::actorType GasCanGoodie::getType()
{
	return gas_can_goodie;
}

LandmineGoodie::LandmineGoodie( int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Goodie(imageID, startX, startY, dir, depth, size)
{
}

LandmineGoodie::~LandmineGoodie()
{
}

void LandmineGoodie::doSomething()
{
}

Actor::actorType LandmineGoodie::getType()
{
	return landmine_goodie;
}

Landmine::Landmine( int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Actor(imageID, startX, startY, dir, depth, size)
{
}

Landmine::~Landmine()
{
}

void Landmine::doSomething()
{
}

Actor::actorType Landmine::getType()
{
	return landmine;
}

Zombie::Zombie( int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Actor(imageID, startX, startY, dir, depth, size)
{

}

Zombie::~Zombie()
{
}

Actor::actorType Zombie::getType()
{
	return zombie;
}

DumbZombie::DumbZombie( int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Zombie(imageID, startX, startY, dir, depth, size)
{

}

DumbZombie::~DumbZombie()
{

}

void DumbZombie::doSomething()
{
	int ch = randInt(1, 4);

	// user hit a key during this tick!
	switch (ch)
	{
	case 1:  // move to left
		setDirection(left);
		move(getX() - STEP_SIZE, getY());
		break;
	case 2: // move to right
		setDirection(right);
		move(getX() + STEP_SIZE, getY());
		break;
	case 3:  // move up
		setDirection(up);
		move(getX(), getY() + STEP_SIZE);
		break;
	case 4:  // move down
		setDirection(down);
		move(getX(), getY() - STEP_SIZE);
		break;
	}
}

Actor::actorType DumbZombie::getType()
{
	return dumb_zombie;
}

SmartZombie::SmartZombie( int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Zombie(imageID, startX, startY, dir, depth, size)
{

}


SmartZombie::~SmartZombie()
{

}

void SmartZombie::doSomething()
{
	int ch = randInt(1, 4);

	// user hit a key during this tick!
	switch (ch)
	{
	case 1:  // move to left
		setDirection(left);
		move(getX() - STEP_SIZE, getY());
		break;
	case 2: // move to right
		setDirection(right);
		move(getX() + STEP_SIZE, getY());
		break;
	case 3:  // move up
		setDirection(up);
		move(getX(), getY() + STEP_SIZE);
		break;
	case 4:  // move down
		setDirection(down);
		move(getX(), getY() - STEP_SIZE);
		break;
	}
}

Actor::actorType SmartZombie::getType()
{
	return smart_zombie;
}
Citizen::Citizen( int imageID, double startX, double startY,Direction dir, int depth, double size) 
	:Actor(imageID, startX, startY, dir, depth, size)
{

}

Citizen::~Citizen()
{

}

void Citizen::doSomething()
{

}

Actor::actorType Citizen::getType()
{
	return citizen;
}
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
