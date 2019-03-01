#include "Actor.h"
#include "StudentWorld.h"
#include "SoundFX.h"

const double OVERLAP_LIMIT = 100;  // 10^2
const double PLAYER_STRIDE_LENGTH = 4;
const double CITIZEN_STRIDE_LENGTH = 2;
const double ZOMBIE_STRIDE_LENGTH = 1;
const int NO_SECOND_DIRECTION = -999;

Actor::Actor(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	:GraphObject(imageID, x, y, dir, depth)
{
	m_dead = false;
	m_world = w;
}

Actor::~Actor() {}

// Is an agent blocked from moving to the indicated location?
bool Actor::isAgentMovementBlockedAt(double x, double y) const
{
	// check if bounding box defined by four points (x,y), (x, y+SPRITE_HEIGHT-1)
	//  (x+SPRITE_WIDTH - 1, y),  (x+SPRITE_WIDTH - 1, y+SPRITE_HEIGHT-1), 
	// overlap with bounding box of the current actor, if yes, return true, otherwise, return false

	if (x >= getX() && x <= getX() + SPRITE_WIDTH - 1 &&
		y >= getY() && y <= getY() + SPRITE_HEIGHT - 1)  return true;

	if (x + SPRITE_WIDTH - 1 >= getX() && x <= getX() &&
		y >= getY() && y <= getY() + SPRITE_HEIGHT - 1)  return true;

	if (x >= getX() && x <= getX() + SPRITE_WIDTH - 1 &&
		y + SPRITE_HEIGHT - 1 >= getY() && y <= getY())  return true;

	if (x + SPRITE_WIDTH - 1 >= getX() && x <= getX() &&
		y + SPRITE_HEIGHT - 1 >= getY() && y <= getY())  return true;

	return false;
}

void Actor::move(double newX, double newY)
{
	// move current actor to new location if possible
	world()->moveActor(this, newX, newY);
}

double Actor::distanceTo(const Actor *other)
{
	// find the distance to other actor
	return distanceTo(other->getX(), other->getY());
}

double Actor::distanceTo(const double x, const double y)
{
	// find the distance to location x, y
	return sqrt(pow(getX() - x, 2) + pow(getY() - y, 2));
}

Direction Actor::getRandomDirection()
{
	// return a random direction
	int dir = randInt(1, 4);
	if (dir == 1) return right;
	else if (dir == 2) return left;
	else if (dir == 3) return up;
	else  return down;
}

StudentWorld* Actor::world()
{
	return m_world;
}

bool Actor::isDead()
{
	return m_dead;
}

void Actor::setDead()
{
	m_dead = true;
}

bool Actor::overlap(const double x, const double y)
{
	// check if this actor overlap with location x, y
	return (pow(getX() - x, 2) + pow(getY() - y, 2) <= OVERLAP_LIMIT);
}

bool Actor::overlap(const Actor *other)
{
	// check if this actor overlap with other
	if (this == other) return false; // exclude itself
	return overlap(other->getX(), other->getY());
}

Agent::Agent(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	:Actor(w, imageID, x, y, dir, depth)
{
	m_ticks = 0;
}

void Agent::dieByFallOrBurnIfAppropriate()
{
};

bool Agent::becomeParalyzed()
{
	// check to see if a agent should become paralyzed
	// if yes, return true, otherwise, return false
	m_ticks++;
	if (m_ticks == 2)
	{
		m_ticks = 0;
		return true;
	}

	return false;
};

Direction Agent::getDirectionCloseToHuman(Actor *agent, int &secondDirection)
{
	// return direction so this object move close to person or move away from zombie
	// secondDirection is used if the first direction is blocked.
	double dist_x = agent->getX() - getX();
	double dist_y = agent->getY() - getY();

	secondDirection = NO_SECOND_DIRECTION;
	if (dist_x == 0)  // at same column
	{
		if (dist_y > 0)
			return up;
		else
			return down;
	}
	else if (dist_y == 0)   // at same row
	{
		if (dist_x > 0)
			return right;
		else
			return left;
	}

	// not on the same row or column
	int option = randInt(1, 2);
	if (option == 1)  // move in x direction
	{

		if (dist_y > 0) secondDirection = up;
		else			secondDirection = down;
		if (dist_x > 0) return right;
		else			return left;
	}
	else
	{
		if (dist_x > 0) secondDirection = right;
		else			secondDirection = left;
		if (dist_y > 0) return up;
		else			return down;
	}
}

Human::Human(StudentWorld* w, int imageID, double x, double y)
	:Agent(w, imageID, x, y)
{
	m_infectionDuration = 0;
}


void Human::clearInfection()
{
	// Make this human uninfected by vomit.
	m_infectionDuration = 0;
};

void Human::beVomitedOnIfAppropriate()
{
	// if this people is not infected yet, set m_infectionDuration to 1 to starting record infection duration
	// if this person is already infected, do nothing
	if (m_infectionDuration == 0) {
		world()->playSound(SOUND_CITIZEN_INFECTED);
		m_infectionDuration = 1;
	}
}
// How many ticks since this human was infected by vomit?
int Human::getInfectionDuration() const
{
	return m_infectionDuration;
};

bool Human::isInfected()
{
	return m_infectionDuration > 0;
}

void Human::increaseInfectionDuration()
{
	m_infectionDuration++;
}

Penelope::Penelope(StudentWorld* w, int imageID, double x, double y)
	:Human(w, imageID, x, y)
{
	m_numLandmines = m_numFlamethrowers = m_numVaccines = 0;
}

Penelope::~Penelope() {}

void Penelope::createFlame()
{
	// create up to 3 flames in front of the player
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
		if (world()->overlapWallExit(px[i], py[i]))break;
		world()->addActor(new Flame(world(), IID_FLAME, px[i], py[i]));
	}
}

void Penelope::createLandmine()
{
	// create up to a landmine in the player's location
	world()->addActor(new Landmine(world(), IID_LANDMINE, getX(), getY()));
}

void Penelope::dieByFallOrBurnIfAppropriate()
{
	world()->playSound(SOUND_PLAYER_DIE);
	setDead();
}

void Penelope::doSomething()
{
	if (isDead())
		return;
	if (isInfected())
	{
		increaseInfectionDuration();
		if (getInfectionDuration() == 500)
		{
			setDead();
			world()->addActor(new SmartZombie(world(), IID_ZOMBIE, getX(), getY()));
			world()->playSound(SOUND_PLAYER_DIE);
			return;
		}
	}
	int ch;
	if (world()->getKey(ch))
	{
		// user hit a key during this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:  // move Penelope to the left
			setDirection(left);
			move(getX() - PLAYER_STRIDE_LENGTH, getY());
			break;
		case KEY_PRESS_RIGHT: // move Penelope to the right
			setDirection(right);
			move(getX() + PLAYER_STRIDE_LENGTH, getY());
			break;
		case KEY_PRESS_UP: // move Penelope up
			setDirection(up);
			move(getX(), getY() + PLAYER_STRIDE_LENGTH);
			break;
		case KEY_PRESS_DOWN: // move Penelope down
			setDirection(down);
			move(getX(), getY() - PLAYER_STRIDE_LENGTH);
			break;
		case KEY_PRESS_SPACE: // add flames in front of Penelope
			if (getNumFlameCharges() > 0)
			{
				decreaseFlameCharges();
				createFlame();
				world()->playSound(SOUND_PLAYER_FIRE);
			}
			break;
		case KEY_PRESS_TAB: // add landmine at Penelope's current location
			if (getNumLandmines() > 0)
			{
				decreaseLandmines();
				createLandmine();
			}
			break;
		case KEY_PRESS_ENTER: // clear infection
			if (getNumVaccines() > 0)
			{
				decreaseVaccines();
				clearInfection();
			}
			break;
		}
	}
}

void Penelope::increaseVaccines() { m_numVaccines++; }
void Penelope::increaseFlameCharges() { m_numFlamethrowers += 5; }
void Penelope::increaseLandmines() { m_numLandmines += 2; }
void Penelope::decreaseVaccines() { m_numVaccines--; }
void Penelope::decreaseFlameCharges() { m_numFlamethrowers--; }
void Penelope::decreaseLandmines() { m_numLandmines--; }
int Penelope::getNumVaccines() const { return m_numVaccines; }
int Penelope::getNumFlameCharges() const { return m_numFlamethrowers; }
int Penelope::getNumLandmines() const { return m_numLandmines; }

Wall::Wall(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	:Actor(w, imageID, x, y, dir, depth)
{
}

Wall::~Wall() {}

void Wall::doSomething()
{
}

Exit::Exit(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	:Actor(w, imageID, x, y, dir, depth)
{
}

Exit::~Exit() {}

void Exit::doSomething()
{
	if (world()->citizenFoundExit(this))
	{
		world()->increaseScore(500);
	}
	if (overlap((world()->player())))  // if overlap with player
	{
		if (world()->citizensGone())
			world()->setGameLevelFinished();
	}
}

Pit::Pit(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	:Actor(w, imageID, x, y, dir, depth)
{
}

Pit::~Pit() {}

void Pit::doSomething()
{
	world()->overlapPitFlame(this);
}

Flame::Flame(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	: Actor(w, imageID, x, y, dir, depth)
{
	numTicks = 0;
}

Flame::~Flame() {}

void Flame::doSomething()
{
	if (numTicks >= 2)  // flame can exist only 2 ticks
		setDead();
	world()->overlapPitFlame(this);
	numTicks++;
}

Vomit::Vomit(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	:Actor(w, imageID, x, y, dir, depth)
{
	numTicks = 0;
}

Vomit::~Vomit() {}

void Vomit::doSomething()
{
	if (numTicks >= 2) // vomit can exist only 2 ticks
		setDead();
	world()->overlapVomit(this);
	numTicks++;
}

Goodie::Goodie(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	:Actor(w, imageID, x, y, dir, depth)
{
}

void Goodie::dieByFallOrBurnIfAppropriate()
{
	setDead();
}

Goodie::~Goodie() {}

void Goodie::doSomething()
{
	if (isDead()) return;
	if (overlap(world()->player()))
	{
		pickupGoodie();
		world()->increaseScore(50);
		setDead();
		world()->playSound(SOUND_GOT_GOODIE);
	}
}

VaccineGoodie::VaccineGoodie(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	:Goodie(w, imageID, x, y, dir, depth)
{
}

VaccineGoodie::~VaccineGoodie() {}

void VaccineGoodie::pickupGoodie()
{
	world()->player()->increaseVaccines();
}

GasCanGoodie::GasCanGoodie(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	:Goodie(w, imageID, x, y, dir, depth)
{
}

GasCanGoodie::~GasCanGoodie() {}

void GasCanGoodie::pickupGoodie()
{
	world()->player()->increaseFlameCharges();
}

LandmineGoodie::LandmineGoodie(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	:Goodie(w, imageID, x, y, dir, depth)
{
}

LandmineGoodie::~LandmineGoodie() {}

void LandmineGoodie::pickupGoodie()
{
	world()->player()->increaseLandmines();
}

Landmine::Landmine(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
	:Actor(w, imageID, x, y, dir, depth)
{
	m_safetyTticks = 30;
	m_activate = false;
}

Landmine::~Landmine() {}

void Landmine::doSomething()
{
	if (isDead()) return;
	if (!m_activate)
	{
		m_safetyTticks--;
		if (m_safetyTticks <= 0) m_activate = true;
		return;
	}

	if (world()->overlapLandmine(this))
	{
		setDead();
		world()->playSound(SOUND_LANDMINE_EXPLODE);
		createFlames();
	}

}

void Landmine::createFlames()
{
	world()->addActor(new Flame(world(), IID_FLAME, getX(), getY()));
	world()->addActor(new Flame(world(), IID_FLAME, getX() + SPRITE_WIDTH, getY()));
	world()->addActor(new Flame(world(), IID_FLAME, getX() - SPRITE_WIDTH, getY()));
	world()->addActor(new Flame(world(), IID_FLAME, getX(), getY() + SPRITE_HEIGHT));
	world()->addActor(new Flame(world(), IID_FLAME, getX(), getY() - SPRITE_HEIGHT));
	world()->addActor(new Flame(world(), IID_FLAME, getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT));
	world()->addActor(new Flame(world(), IID_FLAME, getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT));
	world()->addActor(new Flame(world(), IID_FLAME, getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT));
	world()->addActor(new Flame(world(), IID_FLAME, getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT));
	world()->addActor(new Pit(world(), IID_PIT, getX(), getY()));
}

void Landmine::dieByFallOrBurnIfAppropriate()
{
	setDead();
	world()->playSound(SOUND_LANDMINE_EXPLODE);
	createFlames();
}

Zombie::Zombie(StudentWorld* w, int imageID, double x, double y) :Agent(w, imageID, x, y)
{
	m_planDistance = 0;
}

Zombie::~Zombie() {}

int Zombie::getPlanDistance() { return m_planDistance; }
void Zombie::deceasePlanDistance() { m_planDistance--; }
void Zombie::setPlanDistance(int n) { m_planDistance = n; }
void Zombie::resetPlanDistance() { m_planDistance = 0; }

bool Zombie::createVomit()
{
	// return true if a vomit create, otherwise, return false
	double x, y;

	if (getDirection() == GraphObject::right)
	{
		x = getX() + SPRITE_WIDTH;
		y = getY();
	}
	else if (getDirection() == GraphObject::left)
	{
		x = getX() - SPRITE_WIDTH;
		y = getY();
	}
	else if (getDirection() == GraphObject::up)
	{
		x = getX();
		y = getY() + SPRITE_HEIGHT;
	}
	else if (getDirection() == GraphObject::down)
	{
		x = getX();
		y = getY() - SPRITE_HEIGHT;
	}

	if (world()->distanceToNearestPerson(x, y) <= 10)
	{
		int ch = randInt(1, 3); // 1 in 3 chance that the zombie will vomit
		if (ch == 1)
		{
			world()->playSound(SOUND_ZOMBIE_VOMIT);
			world()->addActor(new Vomit(world(), IID_VOMIT, x, y));
			return true;
		}
	}

	return false;
}

void Zombie::doSomething()
{
	if (isDead()) return;

	if (becomeParalyzed()) return; // the citizen will become paralyzed every other tick

	if (world()->personInFrontZombie(this))
	{
		if (createVomit()) return;
	}

	int secondDirection;
	// reach here, no vomit created, 
	if (getPlanDistance() == 0)
	{
		setPlanDistance(randInt(3, 10));
		setDirection(getMovementDirection(secondDirection));
	}

	double dest_x = getX();
	double dest_y = getY();

	switch (getDirection())
	{
	case left:  dest_x -= ZOMBIE_STRIDE_LENGTH;	break;
	case right: dest_x += ZOMBIE_STRIDE_LENGTH;	break;
	case up: 	dest_y += ZOMBIE_STRIDE_LENGTH;	break;
	case down: 	dest_y -= ZOMBIE_STRIDE_LENGTH;	break;
	}

	if (world()->blockingMovement(this, dest_x, dest_y)) //the dumb zombie was blocked from moving
	{
		resetPlanDistance();
	}
	else
	{
		move(dest_x, dest_y);
		deceasePlanDistance();
	}
}

DumbZombie::DumbZombie(StudentWorld* w, int imageID, double x, double y) :Zombie(w, imageID, x, y)
{
}

DumbZombie::~DumbZombie() {}

void DumbZombie::setDead()
{
	world()->increaseScore(1000);
	Actor::setDead();
}

void DumbZombie::dieByFallOrBurnIfAppropriate()
{
	setDead();
	world()->playSound(SOUND_ZOMBIE_DIE);
	int ch = randInt(1, 10); //1 in 10 dumb zombies are mindlessly carrying a vaccine goodie
	if (ch == 1)  //1 in 10 dumb zombies are mindlessly carrying a vaccine goodie
	{
		Direction dir = getRandomDirection();
		double x = getX();
		double y = getY();
		if (dir == right)  x += SPRITE_WIDTH;
		else if (dir == left)   x -= SPRITE_WIDTH;
		else if (dir == up)  y += SPRITE_HEIGHT;
		else if (dir == down)   x -= SPRITE_HEIGHT;

		if (!world()->blockingMovement(this, x, y))
			world()->addActor(new VaccineGoodie(world(), IID_VACCINE_GOODIE, getX(), getY()));
	}
}

Direction DumbZombie::getMovementDirection(int &secondDirection)
{
	return getRandomDirection();
}

SmartZombie::SmartZombie(StudentWorld* w, int imageID, double x, double y) :Zombie(w, imageID, x, y)
{
}

SmartZombie::~SmartZombie() {}

void SmartZombie::dieByFallOrBurnIfAppropriate()
{
	setDead();
	world()->playSound(SOUND_ZOMBIE_DIE);
}

void SmartZombie::setDead()
{
	world()->increaseScore(2000);
	Actor::setDead();
}

Direction SmartZombie::getMovementDirection(int & secondDirection)
{
	if (world()->distanceToNearestPerson(getX(), getY()) > 80)
		return getRandomDirection();

	Actor *nearestPerson = world()->findNearestPerson(getX(), getY());
	if (nearestPerson != nullptr)
		return getDirectionCloseToHuman(nearestPerson, secondDirection);
	else
		return getRandomDirection();
}

Citizen::Citizen(StudentWorld* w, int imageID, double x, double y)
	:Human(w, imageID, x, y)
{
	tickCount = 0;
}

Citizen::~Citizen() {}

void Citizen::doSomething()
{
	if (isDead()) return;
	if (isInfected()) increaseInfectionDuration();
	if (getInfectionDuration() >= 500) // becomes a zombie
	{
		setDead();
		world()->playSound(SOUND_ZOMBIE_BORN);
		world()->increaseScore(-1000);
		createZombie();
		return;
	}

	if (becomeParalyzed()) return; // the citizen will become paralyzed every other tick

	double dist_p = distanceTo(world()->player()); // distance to Penelope
	double dist_z = world()->distanceToNearestZombie(this);
	if ((dist_p < dist_z || world()->getNumZombie() == 0) && dist_p <= 80)
		if (followPenelope()) return;

	if (dist_z <= 80)
		runAwayZombie();
}

bool Citizen::move_location(Direction dir, double new_x, double new_y)
{
	// move to new location new_x, new_y if possible, if yes return true. otherwise, return false

	bool doesMove = false;

	if (!world()->blockingMovement(this, new_x, new_y))
	{
		moveTo(new_x, new_y);
		doesMove = true;
		setDirection(dir);  // reset direction
	}
	return doesMove;
}

bool Citizen::followPenelope()
{
	// return true if can move to penelope
	// return false if blocking movement

	double x;
	double y;
	int secondDirection;

	Direction dir = getDirectionCloseToHuman(world()->player(), secondDirection);

	findNextLocation(dir, x, y);
	if (move_location(dir, x, y)) return true; // already move

	// reach here, means blocked by other objects in first trial direction
	// if citizen in the same row or column as penelope, return false
	// otherwise, try another direction
	if (secondDirection == NO_SECOND_DIRECTION) return false;

	// check another direction
	findNextLocation(secondDirection, x, y);
	return (move_location(secondDirection, x, y));
}

void Citizen::findNextLocation(Direction dir, double &x, double &y)
{
	x = getX();
	y = getY();

	switch (dir)
	{
	case right: x += CITIZEN_STRIDE_LENGTH; break;
	case left:	x -= CITIZEN_STRIDE_LENGTH;	break;
	case up:	y += CITIZEN_STRIDE_LENGTH; break;
	case down:	y -= CITIZEN_STRIDE_LENGTH; break;
	}
}

void Citizen::runAwayZombie()
{
	double dist = world()->distanceToNearestZombie(this);
	if (dist > 80) return;   // do nothing if no zombie nearby

	double dist_right = 0, dist_left = 0, dist_up = 0, dist_down = 0;

	if (!world()->blockingMovement(this, getX() + CITIZEN_STRIDE_LENGTH, getY()))
	{
		dist_right = world()->distanceToNearestZombie(getX() + CITIZEN_STRIDE_LENGTH, getY());
	}

	if (!world()->blockingMovement(this, getX() - CITIZEN_STRIDE_LENGTH, getY()))
	{
		dist_left = world()->distanceToNearestZombie(getX() - CITIZEN_STRIDE_LENGTH, getY());
	}

	if (!world()->blockingMovement(this, getX(), getY() + CITIZEN_STRIDE_LENGTH))
	{
		dist_up = world()->distanceToNearestZombie(getX(), getY() + CITIZEN_STRIDE_LENGTH);
	}

	if (!world()->blockingMovement(this, getX(), getY() - CITIZEN_STRIDE_LENGTH))
	{
		dist_down = world()->distanceToNearestZombie(getX(), getY() - CITIZEN_STRIDE_LENGTH);
	}

	double arr[] = { dist, dist_right, dist_left, dist_up, dist_down };
	double max_dist = arr[0];
	for (int i = 1; i < 5; i++)
	{
		if (arr[i] > max_dist)
			max_dist = arr[i];
	}

	if (max_dist == dist)
		return;
	else if (max_dist == dist_right)
	{
		move_location(right, getX() + CITIZEN_STRIDE_LENGTH, getY());
		return;
	}
	else if (max_dist == dist_left)
	{
		move_location(left, getX() - CITIZEN_STRIDE_LENGTH, getY());
		return;
	}
	else if (max_dist == dist_up)
	{
		move_location(up, getX(), getY() + CITIZEN_STRIDE_LENGTH);
		return;
	}
	else if (max_dist == dist_down)
	{
		move_location(down, getX(), getY() - CITIZEN_STRIDE_LENGTH);
		return;
	}
}

void Citizen::createZombie()
{
	// 70 % chance create a dumb zombie, 30% chance a smart zombie
	int ch = randInt(1, 10);

	if (ch <= 3)
		world()->addActor(new SmartZombie(world(), IID_ZOMBIE, getX(), getY()));
	else
		world()->addActor(new DumbZombie(world(), IID_ZOMBIE, getX(), getY()));

}

void Citizen::dieByFallOrBurnIfAppropriate()
{
	world()->increaseScore(-1000);
	world()->playSound(SOUND_CITIZEN_DIE);
	setDead();
}

void Citizen::setDead()
{
	world()->decreaseCitizen();
	Actor::setDead();
}