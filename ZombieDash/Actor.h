#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Level.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor :public GraphObject
{
public:
	enum actorType {
		empty, player, dumb_zombie, smart_zombie, citizen, wall, exit, pit,
		vaccine_goodie, gas_can_goodie, landmine_goodie, flame, vomit, landmine, zombie, goodie
	};
	Actor(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Actor();
	virtual void doSomething();
	void setWorld(StudentWorld* world);
	StudentWorld* getWorld();
	void move(double newX, double newY);
	virtual actorType getType() = 0;
	bool isAlive();
	void setState(bool state);
	bool overlap(const Actor &other);
	bool overlap(const double x, const double y);
	virtual bool isGoodie();

private:
	StudentWorld* m_world;
	bool m_alive;
};

class Penelope :public Actor
{
public:
	Penelope(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Penelope();
	void doSomething();
	virtual actorType getType();
	void addFlame(int n);
	void addMine(int n);
	void addVaccine(int n);
	int getNumFlame();
	int getNumMine();
	int getNumVaccine();
	void createLandmine();

private:
	int numLandmines, numFlamethrowers, numVaccines, m_infectionCount;
	bool m_infected;
	void createFlame();
};

class Wall :public Actor
{
public:
	Wall(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Wall();
	virtual actorType getType();
};
class Citizen :public Actor
{
public:
	Citizen(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Citizen();
	void doSomething();
	virtual actorType getType();
};

class Exit :public Actor
{
public:
	Exit(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Exit();
	void doSomething();
	virtual actorType getType();
};

class Flame :public Actor
{
public:
	Flame(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Flame();
	void doSomething();
	virtual actorType getType();
private:
	int numTicks;

};

class Goodie : public Actor
{
public:
	Goodie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Goodie();
	virtual void doSomething()=0;
	virtual actorType getType();
	bool isGoodie();
};


class GasCanGoodie :public Goodie
{
public:
	GasCanGoodie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~GasCanGoodie();
	void doSomething();
	virtual actorType getType();
};

class Landmine :public Actor
{
public:
	Landmine(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Landmine();
	void doSomething();
	virtual actorType getType();
};

class LandmineGoodie :public Goodie
{
public:
	LandmineGoodie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~LandmineGoodie();
	void doSomething();
	virtual actorType getType();
};

class Pit :public Actor
{
public:
	Pit(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Pit();
	void doSomething();
	virtual actorType getType();
};

class Zombie :public Actor
{
public:
	Zombie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Zombie();
	void doSomething()=0;
	virtual actorType getType();
};

class SmartZombie :public Zombie
{
public:
	SmartZombie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~SmartZombie();
	void doSomething();
	virtual actorType getType();
};

class DumbZombie :public Zombie
{
public:
	DumbZombie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~DumbZombie();
	void doSomething();
	virtual actorType getType();
};

class Vomit :public Actor
{
public:
	Vomit(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Vomit();
	void doSomething();
	virtual actorType getType();
};

class VaccineGoodie :public Goodie
{
public:
	VaccineGoodie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~VaccineGoodie();
	void doSomething();
	virtual Actor::actorType getType();
};
#endif // ACTOR_H_
