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
		vaccine_goodie, gas_can_goodie, landmine_goodie, flame, vomit, landmine, zombie
	};
	Actor(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~Actor();
	virtual void doSomething() = 0;
	void setWorld(StudentWorld* world);
	StudentWorld* getWorld();
	void move(double newX, double newY);
	virtual actorType getType() = 0;

private:
	StudentWorld* m_world;
};

class Penelope :public Actor
{
public:
	Penelope(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~Penelope();
	void doSomething();
	virtual actorType getType();
private:
	int m_alive, numLandmines, numFlamethrowers, numVaccines, m_infectionCount;
	bool m_infected;
};

class Wall :public Actor
{
public:
	Wall(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~Wall();
	void doSomething();
	virtual actorType getType();
};
class Citizen :public Actor
{
public:
	Citizen(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~Citizen();
	void doSomething();
	virtual actorType getType();
};

class Exit :public Actor
{
public:
	Exit(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~Exit();
	void doSomething();
	virtual actorType getType();
};

class Flame :public Actor
{
public:
	Flame(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~Flame();
	void doSomething();
	virtual actorType getType();
};

class GasCanGoodie :public Actor
{
public:
	GasCanGoodie(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~GasCanGoodie();
	void doSomething();
	virtual actorType getType();
};

class Landmine :public Actor
{
public:
	Landmine(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~Landmine();
	void doSomething();
	virtual actorType getType();
};

class LandmineGoodie :public Actor
{
public:
	LandmineGoodie(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~LandmineGoodie();
	void doSomething();
	virtual actorType getType();
};

class Pit :public Actor
{
public:
	Pit(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~Pit();
	void doSomething();
	virtual actorType getType();
};

class Zombie :public Actor
{
public:
	Zombie(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~Zombie();
	void doSomething();
	virtual actorType getType();
};

class SmartZombie :public Zombie
{
public:
	SmartZombie(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~SmartZombie();
	void doSomething();
	virtual actorType getType();
};

class DumbZombie :public Zombie
{
public:
	DumbZombie(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~DumbZombie();
	void doSomething();
	virtual actorType getType();
};

class Vomit :public Actor
{
public:
	Vomit(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~Vomit();
	void doSomething();
	virtual actorType getType();
};

class VaccineGoodie :public Actor
{
public:
	VaccineGoodie(int imageID, double startX, double startY, Direction dir, int depth, double size);
	~VaccineGoodie();
	void doSomething();
	virtual actorType getType();
};
#endif // ACTOR_H_
