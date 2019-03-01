#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Level.h"
#include <string>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Goodie;
class Actor :public GraphObject
{
public:
	Actor(StudentWorld* w, int imageID, double x = 0, double y = 0, int dir = 0, int depth = 0);
	virtual ~Actor();
	virtual void doSomething() = 0;
	StudentWorld* world();

	// If this object can die by falling into a pit or burning, die.
	virtual void dieByFallOrBurnIfAppropriate() {};

	// If this object can be infected by vomit, get infected.
	virtual void beVomitedOnIfAppropriate() {};

	// Is an agent blocked from moving to the indicated location?
	bool isAgentMovementBlockedAt(double x, double y) const;

	void move(double newX, double newY);
	bool isDead();
	virtual void setDead();
	bool overlap(const Actor *other);
	bool overlap(const double x, const double y);
	double distanceTo(const Actor *other);
	double distanceTo(const double x, const double y);
	Direction getRandomDirection();
	virtual bool flammable() const { return false; }
	virtual bool canExit() const { return false; }

	// define behavior of actors, set the default value to the behavior for most of actors
	// can be overwriten by derived class if necessary
	virtual bool blocksFlame() const { return false; }
	virtual bool blocksMovement() const { return true; }
	virtual bool triggersZombieVomit() const { return false; }
	virtual bool triggersOnlyActiveLandmines() const { return false; }
	virtual bool mayVomit() const { return false; }
	//std::string name;   // for debug purpose only
private:
	StudentWorld* m_world;
	bool m_dead;
};

class Wall :public Actor
{
public:
	Wall(StudentWorld* w, int imageID, double x = 0, double y = 0, int dir = 0, int depth = 0);
	bool blocksFlame() const { return true; }

	void doSomething();
	~Wall();
};

class Exit :public Actor
{
public:
	Exit(StudentWorld* w, int imageID, double x = 0, double y = 0, int dir = 0, int depth = 1);
	bool blocksFlame() const { return true; }
	virtual bool blocksMovement() const { return false; }
	~Exit();
	void doSomething();
};

class Pit :public Actor
{
public:
	Pit(StudentWorld* w, int imageID, double x = 0, double y = 0, int dir = 0, int depth = 0);
	~Pit();
	bool blockingFlame() const { return false; }
	bool blocksMovement() const { return false; }
	void doSomething();
};

class Flame :public Actor
{
public:
	Flame(StudentWorld* w, int imageID, double x = 0, double y = 0, int dir = 0, int depth = 0);
	bool flammable() const { return true; }
	bool blocksMovement() const { return false; }
	~Flame();
	void doSomething();
private:
	int numTicks;

};

class Vomit :public Actor
{
public:
	Vomit(StudentWorld* w, int imageID, double x = 0, double y = 0, int dir = 0, int depth = 0);
	~Vomit();
	bool blocksMovement() const { return false; }
	void doSomething();
private:
	int numTicks;
};

class Landmine :public Actor
{
public:
	Landmine(StudentWorld* w, int imageID, double x = 0, double y = 0, int dir = 0, int depth = 1);
	bool blockingFlame() const { return false; }
	bool blocksMovement() const { return false; }

	void createFlames();
	void dieByFallOrBurnIfAppropriate();

	~Landmine();
	void doSomething();
private:
	int m_safetyTticks;
	bool m_activate;

};

class Goodie : public Actor
{
public:
	Goodie(StudentWorld* w, int imageID, double x = 0, double y = 0, int dir = 0, int depth = 1);
	bool blocksMovement() const { return false; }
	~Goodie();
	virtual void doSomething();
	virtual void pickupGoodie() = 0;
	void dieByFallOrBurnIfAppropriate();
};

class VaccineGoodie :public Goodie
{
public:
	VaccineGoodie(StudentWorld* w, int imageID, double x = 0, double y = 0, int dir = 0, int depth = 1);

	~VaccineGoodie();
	void pickupGoodie();
};

class GasCanGoodie :public Goodie
{
public:
	GasCanGoodie(StudentWorld* w, int imageID, double x = 0, double y = 0, int dir = 0, int depth = 1);
	~GasCanGoodie();
	void pickupGoodie();
};

class LandmineGoodie :public Goodie
{
public:
	LandmineGoodie(StudentWorld* w, int imageID, double x = 0, double y = 0, int dir = 0, int depth = 1);
	~LandmineGoodie();
	void pickupGoodie();
};

class Agent : public Actor
{
public:
	Agent(StudentWorld* w, int imageID, double x = 0, double y = 0, int dir = 0, int depth = 0);
	virtual bool blocksMovement() const { return true; }
	virtual bool triggersOnlyActiveLandmines() const { return true; }
	void dieByFallOrBurnIfAppropriate();
	bool becomeParalyzed();
	Direction getDirectionCloseToHuman(Actor *human, int &secondDirection);
private:
	int m_ticks;
};

class Human : public Agent
{
public:
	Human(StudentWorld* w, int imageID, double x = 0, double y = 0);
	virtual void beVomitedOnIfAppropriate();
	virtual bool triggersZombieVomit() const { return true; }
	virtual void dieByFallOrBurnIfAppropriate() {};

	// Make this human uninfected by vomit.
	void clearInfection();

	// How many ticks since this human was infected by vomit?
	int getInfectionDuration() const;
	bool isInfected();
	void increaseInfectionDuration();

private:
	int m_infectionDuration;
};


class Penelope :public Human
{
public:
	Penelope(StudentWorld* w, int imageID, double x = 0, double y = 0);
	~Penelope();
	void doSomething();
	void createLandmine();
	void dieByFallOrBurnIfAppropriate();

	void increaseVaccines();
	void increaseFlameCharges();
	void increaseLandmines();
	void decreaseVaccines();
	void decreaseFlameCharges();
	void decreaseLandmines();
	int  getNumVaccines() const;
	int  getNumFlameCharges() const;
	int  getNumLandmines() const;

private:
	int m_numLandmines, m_numFlamethrowers, m_numVaccines;
	void createFlame();
};

class Citizen :public Human
{
public:
	Citizen(StudentWorld* w, int imageID, double x = 0, double y = 0);
	~Citizen();
	void doSomething();
	bool canExit() const { return true; }
	void createZombie();

	bool blockingFlame() const { return false; }
	void dieByFallOrBurnIfAppropriate();
	void setDead();

private:
	bool followPenelope();
	void runAwayZombie();
	bool move_location(Direction dir, double new_x, double new_y);
	void findNextLocation(Direction dir, double &new_x, double &new_y);
	//bool move_in_same_row(double delta_x);
	//bool move_in_same_column(double delta_y);
	int tickCount;
};


class Zombie :public Agent
{
public:
	Zombie(StudentWorld* w, int imageID, double x = 0, double y = 0);
	~Zombie();
	bool blockingFlame() const { return false; }
	bool mayVomit() const { return true; }
	bool createVomit();
	int getPlanDistance();
	void deceasePlanDistance();
	void setPlanDistance(int n);
	void resetPlanDistance();

	void doSomething();
	virtual Direction getMovementDirection(int &secondDirection) = 0;
	void dieByFallOrBurnIfAppropriate() {};
private:
	int m_planDistance;
};


class SmartZombie :public Zombie
{
public:
	SmartZombie(StudentWorld* w, int imageID, double x = 0, double y = 0);
	~SmartZombie();
	void setDead();
	void dieByFallOrBurnIfAppropriate();
	Direction getMovementDirection(int &secondDirection);
};

class DumbZombie :public Zombie
{
public:
	DumbZombie(StudentWorld* w, int imageID, double x = 0, double y = 0);
	~DumbZombie();
	void setDead();
	void dieByFallOrBurnIfAppropriate();
	Direction getMovementDirection(int &secondDirection);
};


#endif // ACTOR_H_