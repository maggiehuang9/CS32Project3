#include "Actor.h"
#include "StudentWorld.h"
#include "SoundFX.h"

	const double STEP_SIZE = 2;
const double OVERLAP_LIMIT = 100;

Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:GraphObject(imageID, startX, startY, dir, depth, size)
{
	m_alive = true;
	name = "";
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

bool Actor::boundingBoxOverlap(const Actor &other)
{
	// check if  bounding box this object overlap with the onde of other
	return ((distance(getX(), other.getX()) < SPRITE_WIDTH) &&
		(distance(getY(), other.getY()) < SPRITE_HEIGHT));

}

double Actor::distanceTo(const Actor &other)
{
	// find the distance to other actor
	return sqrt(pow(getX() - other.getX(), 2) + pow(getY() - other.getY(), 2));
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
	if (this == &other) return false; // exclude itself
	return (pow(getX() - other.getX(), 2) + pow(getY() - other.getY(), 2) < OVERLAP_LIMIT);
}

Penelope::Penelope(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Actor(imageID, startX, startY, dir, depth, size)
{
	m_infected = false;
	m_infectionCount = 0;
	setState(true);
}

Penelope::~Penelope()
{

}

void Penelope::createFlame()
{
	double px[3], py[3];
	switch (getDirection())
	{
	case right:
		px[0] = getX() + SPRITE_WIDTH;
		px[1] = getX() + 2 * SPRITE_WIDTH;
		px[2] = getX() + 3 * SPRITE_WIDTH;
		py[0] = py[1] = py[2] = getY();
		break;
	case left:
		px[0] = getX() - SPRITE_WIDTH;
		px[1] = getX() - 2 * SPRITE_WIDTH;
		px[2] = getX() - 3 * SPRITE_WIDTH;
		py[0] = py[1] = py[2] = getY();
		break;
	case up:
		px[0] = px[1] = px[2] = getX();
		py[0] = getY() + SPRITE_HEIGHT;
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
			if (getWorld()->getNumFlame() > 0)
			{
				getWorld()->decreaseFlame();
				createFlame();
				getWorld()->playSound(SOUND_PLAYER_FIRE);
			}
			//... add flames in front of Penelope...;
			break;
		case KEY_PRESS_TAB:
			if (getWorld()->getNumMine() > 0)
			{
				getWorld()->decreaseMine();
				createLandmine();
			}
			//... add landmines in front of Penelope...;
			break;
		case KEY_PRESS_ENTER:
			if (getWorld()->getNumVaccine() > 0)
			{
				getWorld()->decreaseVaccine();
				m_infected = false;
			}
			//... add vaccines...;
			break;
		}
	}
}

Wall::Wall(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Actor(imageID, startX, startY, dir, depth, size)
{
	name = "wall";
}

Wall::~Wall()
{

}

void Wall::doSomething()
{

}

Exit::Exit(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Actor(imageID, startX, startY, dir, depth, size)
{
	name = "exit";
}

Exit::~Exit()
{

}

void Exit::doSomething()
{
	if (overlap(getWorld()->getPlayer()))  // if overlap with player
	{
		if (getWorld()->citizensGone())
			getWorld()->setGameLevelFinished();
	}

}

bool Exit::overlapCitizen()
{
	if (overlap(getWorld()->getPlayer()))  // if overlap with player
	{
		if (getWorld()->citizensGone())
			getWorld()->setGameLevelFinished();
	}

	return false;
}

Pit::Pit(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Actor(imageID, startX, startY, dir, depth, size)
{
	name = "pit";
}

Pit::~Pit()
{

}

void Pit::doSomething()
{

}

Flame::Flame(int imageID, double startX, double startY, Direction dir, int depth, double size)
	: Actor(imageID, startX, startY, dir, depth, size)
{
	numTicks = 0;
	name = "flame";
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

Vomit::Vomit(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Actor(imageID, startX, startY, dir, depth, size)
{
	name = "vomit";
}

Vomit::~Vomit()
{

}

void Vomit::doSomething()
{

}

Goodie::Goodie(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Actor(imageID, startX, startY, dir, depth, size)
{
	name = "goodie";
}

Goodie::~Goodie()
{

}

//bool Goodie::isGoodie()
//{
//	return true;
//}

VaccineGoodie::VaccineGoodie(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Goodie(imageID, startX, startY, dir, depth, size)
{
	name = "vaccine goodie";
}

VaccineGoodie::~VaccineGoodie()
{

}

void VaccineGoodie::doSomething()
{
	if (!isAlive())
		return;
	if (overlap(getWorld()->getPlayer()))
	{
		getWorld()->increaseScore(50);
		getWorld()->addVaccine();
		setState(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

GasCanGoodie::GasCanGoodie(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Goodie(imageID, startX, startY, dir, depth, size)
{
	name = "gas goodie";
}

GasCanGoodie::~GasCanGoodie()
{

}

void GasCanGoodie::doSomething()
{
	if (!isAlive())
		return;
	if (overlap(getWorld()->getPlayer()))
	{
		getWorld()->increaseScore(50);
		getWorld()->addFlame();
		setState(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

LandmineGoodie::LandmineGoodie(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Goodie(imageID, startX, startY, dir, depth, size)
{
	name = "landmine goodie";
}

LandmineGoodie::~LandmineGoodie()
{
}

void LandmineGoodie::doSomething()
{
	if (!isAlive())
		return;
	if (overlap(getWorld()->getPlayer()))
	{
		getWorld()->increaseScore(50);
		getWorld()->addMine();
		setState(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

Landmine::Landmine(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Actor(imageID, startX, startY, dir, depth, size)
{
	m_safetyticks = 0;
}

Landmine::~Landmine()
{
}

void Landmine::doSomething()
{
	if (!isAlive())
		return;
	if (m_safetyticks > 0)
	{
		m_safetyticks--;
		if (m_safetyticks == 0)
		{
			return;
		}
	}
	
	if (overlap((getWorld()->getPlayer())))
	{
		setState(false);
		getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
		getWorld()->addActor(new Flame(IID_FLAME, getX(), getY()));
		getWorld()->addActor(new Flame(IID_FLAME, getX() + SPRITE_WIDTH, getY()));
		getWorld()->addActor(new Flame(IID_FLAME, getX() - SPRITE_WIDTH, getY()));
		getWorld()->addActor(new Flame(IID_FLAME, getX(), getY() + SPRITE_HEIGHT));
		getWorld()->addActor(new Flame(IID_FLAME, getX(), getY() - SPRITE_HEIGHT));
		getWorld()->addActor(new Flame(IID_FLAME, getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT));
		getWorld()->addActor(new Flame(IID_FLAME, getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT));
		getWorld()->addActor(new Flame(IID_FLAME, getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT));
		getWorld()->addActor(new Flame(IID_FLAME, getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT));
		getWorld()->addActor(new Pit(IID_PIT, getX(), getY()));
	}
		

}

Zombie::Zombie(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Actor(imageID, startX, startY, dir, depth, size)
{
	name = "zombie";
}

Zombie::~Zombie()
{
}

DumbZombie::DumbZombie(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Zombie(imageID, startX, startY, dir, depth, size)
{
	name = "dumb zombie";
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

SmartZombie::SmartZombie(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Zombie(imageID, startX, startY, dir, depth, size)
{
	name = "smart zombie";
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

Citizen::Citizen(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:Actor(imageID, startX, startY, dir, depth, size)
{
	infectionCount = 0;
	tickCount = 0;

	name = "citizon";
}

Citizen::~Citizen()
{

}

void Citizen::doSomething()
{
	if (!isAlive()) return;
	if (infectionCount > 0) infectionCount++;
	if (infectionCount >= 500) // becomes a zombie
	{
		setState(false);
		getWorld()->playSound(SOUND_ZOMBIE_BORN);
		getWorld()->decreaseScore(1000);
		createZombie();
		return;
	}

	tickCount++;
	if (tickCount == 2)  // the citizen will become paralyzed every other tick
	{
		tickCount = 0;
		return;
	}

	double dist_p = distanceTo(getWorld()->getPlayer()); // distance to Penelope
	double dist_z = getWorld()->DistanceToNearestZombie(*this, getX(), getY());
	if ((dist_p < dist_z || getWorld()->getNumZombie() == 0) && dist_p <= 80)
		if (followPenelope()) return;

	if (dist_z <= 80)
		runAwayZombie();
}

bool Citizen::move_location(double new_x, double new_y)
{
	// move to new location new_x, new_y if possible, if yes return true. otherwise, return false

	bool doesMove = false;
	double delta_x = new_x - getX();
	double delta_y = new_y - getY();

	if (!getWorld()->blockingMovement(*this, new_x, new_y))
	{
		moveTo(new_x, new_y);
		doesMove = true;
	}

	if (doesMove)  // reset direction
	{
		if (delta_x > 0)
			setDirection(right);
		else if (delta_x < 0)
			setDirection(left);

		if (delta_y > 0)
			setDirection(up);
		else if (delta_y < 0)
			setDirection(down);
	}

	return doesMove;
}

bool Citizen::move_in_same_row(double delta_x)
{
	// move to new location getX() + delta_x, getY() if possible, if yes return true. otherwise, return false
	return move_location(getX() + delta_x, getY());
}

bool Citizen::move_in_same_column(double delta_y)
{
	// move to new location getX(), getY() + delta_y if possible, if yes return true. otherwise, return false
	return move_location(getX(), getY() + delta_y);

}

bool Citizen::followPenelope()
{
	// return true if can move to penelope
	// return false if blocking movement
	double dist_x = getWorld()->getPlayer().getX() - getX();
	double dist_y = getWorld()->getPlayer().getY() - getY();

	if (dist_x == 0)  // at same column
	{
		if (dist_y > 0)
		{
			if (move_in_same_column(2.0)) return true;
		}
		else
		{
			if (move_in_same_column(-2.0)) return true;
		}
	}
	else if (dist_y == 0)   // at same row
	{
		if (dist_x > 0)
		{
			if (move_in_same_row(2.0)) return true;
		}
		else
		{
			if (move_in_same_row(-2.0)) return true;
		}
	}

	// not on the same row or column

	int option = randInt(1, 2);
	double delta_x = (dist_x > 0) ? 2.0 : -2.0;
	double delta_y = (dist_y > 0) ? 2.0 : -2.0;


	if (option == 1)  // move in x direction
	{
		if (move_in_same_row(delta_x)) return true;
		if (move_in_same_column(delta_y)) return true;
	}
	else
	{
		if (move_in_same_column(delta_y)) return true;
		if (move_in_same_row(delta_x)) return true;
	}

	return false;

}

void Citizen::runAwayZombie()
{
	double dist = getWorld()->DistanceToNearestZombie(*this, getX(), getY());
	double dist1 = 0, dist2 = 0, dist3 = 0, dist4 = 0;
	switch (getDirection())
	{
	case right:
		if (!getWorld()->blockingMovement(*this, getX() + 2, getY()))
		{
			double dist1 = getWorld()->DistanceToNearestZombie(*this, getX() + 2, getY());
		}
		break;
	case left:
		if (!getWorld()->blockingMovement(*this, getX() - 2, getY()))
		{
			double dist2 = getWorld()->DistanceToNearestZombie(*this, getX() - 2, getY());
		}
		break;
	case up:
		if (!getWorld()->blockingMovement(*this, getX(), getY() + 2))
		{
			double dist3 = getWorld()->DistanceToNearestZombie(*this, getX(), getY() + 2);
		}
		break;
	case down:
		if (!getWorld()->blockingMovement(*this, getX() + 2, getY()))
		{
			double dist4 = getWorld()->DistanceToNearestZombie(*this, getX(), getY() - 2);
		}

		break;
	}
	double arr[] = { dist, dist1, dist2, dist3, dist4 };
	double max = arr[0];
	for (int i = 1; i < 5; i++)
	{
		if (arr[i] > max)
			max = arr[i];
	}
	if (max == dist)
		return;
	else if (max == dist1)
	{
		moveTo(getX() + 2, getY());
		return;
	}
	else if (max == dist2)
	{
		moveTo(getX() - 2, getY());
		return;
	}
	else if (max == dist3)
	{
		moveTo(getX(), getY() + 2);
		return;
	}
	else if (max == dist4)
	{
		moveTo(getX(), getY() - 2);
		return;
	}
}

void Citizen::createZombie()
{
	// 70 % chance create a dumb zombie, 30% chance a smart zombie
	int ch = randInt(1, 10);

	if (ch <= 3)
		getWorld()->addActor(new SmartZombie(IID_ZOMBIE, getX(), getY()));
	else
		getWorld()->addActor(new DumbZombie(IID_ZOMBIE, getX(), getY()));

}

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
