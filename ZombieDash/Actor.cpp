#include "Actor.h"
#include "StudentWorld.h"
#include "SoundFX.h"

const double STEP_SIZE = 2;
double distance(double x1, double x2)
{
	return x1 > x2 ? x1 - x2 : x2 - x1;
}
Actor::Actor( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0):GraphObject(imageID, startX, startY, dir, depth, size)
{
}

Actor::~Actor()
{

}

void Actor::move(double newX, double newY)
{
	vector<Actor*> actors = m_world->getActors();
	for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); it++)
	{
		if (*it == this) continue;
		switch (getDirection())
		{
		case left:
			if ((*it)->getX() > getX()) break;
			if (distance(getY(), (*it)->getY()) > SPRITE_HEIGHT / 2) break;
			if ((getX() - (*it)->getX()) <= SPRITE_WIDTH) return;
			break;
		case right:
			if ((*it)->getX() < getX()) break;
			if (distance(getY(), (*it)->getY()) > SPRITE_HEIGHT / 2) break;
			if (((*it)->getX()) - getX() <= SPRITE_WIDTH) return;
			break;
		case down:
			if ((*it)->getY() > getY()) break;
			if (distance(getX(), (*it)->getX()) > SPRITE_WIDTH / 2) break;
			if ((getY() - (*it)->getY()) <= SPRITE_HEIGHT) return;
			break;
		case up:
			if ((*it)->getY() < getY()) break;
			if (distance(getX(), (*it)->getX()) > SPRITE_WIDTH / 2) break;
			if (((*it)->getY() - getY()) <= SPRITE_HEIGHT) return;
			break;
		}
	}
	moveTo(newX, newY);
}
void Actor::setWorld(StudentWorld* world)
{
	m_world = world;
}

StudentWorld* Actor::getWorld()
{
	return m_world;
}

void Actor::doSomething()
{

}

Penelope::Penelope( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Actor(imageID, startX, startY, dir, depth,size)
{
	m_alive = 1;
	numLandmines = 0;
	numFlamethrowers = 0;
	numVaccines = 0;
	m_infected = false;
	m_infectionCount = 0;
}

Penelope::~Penelope()
{

}

void Penelope::doSomething()
{
	if (!m_alive)
		return;
	if (m_infected)
	{
		m_infectionCount++;
		if (m_infectionCount == 500)
		{
			m_alive = 0;
			//playClip(SOUND_PLAYER_DIE);
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
			move(getX() - STEP_SIZE, getY());
			//... move Penelope to the left ...;
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			move(getX() + STEP_SIZE, getY());
			//... move Penelope to the right ...;
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			move(getX(), getY() + STEP_SIZE);
			//... move Penelope up ...;
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			move(getX(), getY() - STEP_SIZE);
			//... move Penelope down...;
			break;
		case KEY_PRESS_SPACE:
			//... add flames in front of Penelope...;
			break;
			// etc…
		}
	}
}

Actor::actorType Penelope::getType()
{
	return player;
}

Wall::Wall( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Actor(imageID, startX, startY, dir, depth, size)
{

}

Wall::~Wall()
{

}

void Wall::doSomething()
{

}

Actor::actorType Wall::getType()
{
	return wall;
}

Exit::Exit( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Actor(imageID, startX, startY, dir, depth, size)
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

Pit::Pit( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Actor(imageID, startX, startY, dir, depth, size)
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

Flame::Flame( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Actor(imageID, startX, startY, dir, depth, size)
{

}

Flame::~Flame()
{

}

void Flame::doSomething()
{

}

Actor::actorType Flame::getType()
{
	return flame;
}

Vomit::Vomit( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0):Actor(imageID, startX, startY, dir, depth, size)
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

VaccineGoodie::VaccineGoodie( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Actor(imageID, startX, startY, dir, depth, size)
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

GasCanGoodie::GasCanGoodie( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Actor(imageID, startX, startY, dir, depth, size)
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

LandmineGoodie::LandmineGoodie( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Actor(imageID, startX, startY, dir, depth, size)
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

Landmine::Landmine( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Actor(imageID, startX, startY, dir, depth, size)
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

Zombie::Zombie( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Actor(imageID, startX, startY, dir, depth, size)
{

}

Zombie::~Zombie()
{

}

void Zombie::doSomething()
{

}

Actor::actorType Zombie::getType()
{
	return zombie;
}

DumbZombie::DumbZombie( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Zombie(imageID, startX, startY, dir, depth, size)
{

}

DumbZombie::~DumbZombie()
{

}

void DumbZombie::doSomething()
{

}

Actor::actorType DumbZombie::getType()
{
	return dumb_zombie;
}

SmartZombie::SmartZombie( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Zombie(imageID, startX, startY, dir, depth, size)
{

}


SmartZombie::~SmartZombie()
{

}

void SmartZombie::doSomething()
{

}

Actor::actorType SmartZombie::getType()
{
	return smart_zombie;
}
Citizen::Citizen( int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0) :Actor(imageID, startX, startY, dir, depth, size)
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
