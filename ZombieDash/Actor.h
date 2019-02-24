#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Level.h"
#include <string>

	// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
	class StudentWorld;
class Actor :public GraphObject
{
public:
	Actor(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	virtual ~Actor();
	virtual void doSomething() = 0;
	void setWorld(StudentWorld* world);
	StudentWorld* getWorld();
	void move(double newX, double newY);
	bool isAlive();
	void setState(bool state);
	bool overlap(const Actor &other);
	bool overlap(const double x, const double y);
	double distanceTo(const Actor &other);
	bool boundingBoxOverlap(const Actor &other);
	virtual bool flammable() { return false; }
	virtual bool canExit() { return false; }
	//virtual void pickUpGoodieIfAppropriate(Goodie* g) { }
	// define behavior of actors, set the default value to the behavior for most of actors
	// can be overwriten by derived class if necessary
	virtual bool blockingFlame() { return false; }
	virtual bool blockingVomit() { return false; }
	virtual bool blockingOtherObject() { return true; }
	virtual bool canBeDamagedByFlame() { return true; }
	virtual bool canBeInfectedByVomit() { return false; }
	std::string name;
private:
	StudentWorld* m_world;
	bool m_alive;
};

class Penelope :public Actor
{
public:
	Penelope(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Penelope();
	void doSomething();
	void createLandmine();
	virtual bool canBeInfectedByVomit() { return true; }
	bool canExit() { return true; }
//	virtual void pickUpGoodieIfAppropriate(Goodie* g);

private:
	//int numLandmines, numFlamethrowers, numVaccines, m_infectionCount;
	int m_infectionCount;
	bool m_infected;
	void createFlame();
};

class Wall :public Actor
{
public:
	Wall(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	bool blockingFlame() { return true; }
	bool canBeDamagedByFlame() { return false; }

	void doSomething();
	~Wall();
};
class Citizen :public Actor
{
public:
	Citizen(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Citizen();
	void doSomething();
	bool canExit() { return true; }
	void createZombie();

	bool canBeInfectedByVomit() { return true; }
	bool blockingOtherObject() { return true; }
	bool blockingFlame() { return false; }
	bool canBeDamagedByFlame() { return true; }


private:
	bool followPenelope();
	void runAwayZombie();
	bool move_location(double new_x, double new_y);
	bool move_in_same_row(double delta_x);
	bool move_in_same_column(double delta_y);
	int infectionCount;
	int tickCount;
};

class Exit :public Actor
{
public:
	Exit(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	bool blockingFlame() { return true; }
	virtual bool blockingOtherObject() { return false; }
	bool canBeDamagedByFlame() { return false; }
	~Exit();
	void doSomething();
private:
	bool overlapCitizen();
};

class Flame :public Actor
{
public:
	Flame(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	bool flammable() { return true; }
	bool blockingOtherObject() { return false; }
	bool canBeDamagedByFlame() { return false; }
	bool canBeInfectedByVomit() { return false; }
	~Flame();
	void doSomething();
private:
	int numTicks;

};

class Goodie : public Actor
{
public:
	Goodie(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	bool canBeDamagedByFlame() { return true; }
	bool canBeInfectedByVomit() { return false; }
	bool blockingOtherObject() { return false; }
	~Goodie();
	virtual void doSomething() = 0;
	//bool isGoodie();
};


class GasCanGoodie :public Goodie
{
public:
	GasCanGoodie(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~GasCanGoodie();
	void doSomething();
};

class Landmine :public Actor
{
public:
	Landmine(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	bool blockingFlame() { return false; }
	bool canBeInfectedByVomit() { return false; }
	bool blockingOtherObject() { return false; }

	~Landmine();
	void doSomething();
private:
	int m_safetyticks;

};

class LandmineGoodie :public Goodie
{
public:
	LandmineGoodie(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~LandmineGoodie();
	void doSomething();
};

class Pit :public Actor
{
public:
	Pit(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Pit();
	bool blockingFlame() { return false; }
	bool blockingOtherObject() { return true; }
	bool canBeDamagedByFlame() { return false; }
	bool canBeInfectedByVomit() { return false; }
	void doSomething();
};

class Zombie :public Actor
{
public:
	Zombie(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Zombie();
	bool canBeInfectedByVomit() { return false; }
	bool blockingOtherObject() { return true; }
	bool blockingFlame() { return false; }
	bool canBeDamagedByFlame() { return true; }

	void doSomething() = 0;
};

class SmartZombie :public Zombie
{
public:
	SmartZombie(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~SmartZombie();
	void doSomething();
};

class DumbZombie :public Zombie
{
public:
	DumbZombie(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~DumbZombie();
	void doSomething();
};

class Vomit :public Actor
{
public:
	Vomit(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Vomit();
	bool blockingOtherObject() { return false; }
	bool canBeDamagedByFlame() { return false; }
	bool canBeInfectedByVomit() { return false; }

	void doSomething();
};

class VaccineGoodie :public Goodie
{
public:
	VaccineGoodie(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);

	~VaccineGoodie();
	void doSomething();
};
#endif // ACTOR_H_
