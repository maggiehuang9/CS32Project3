#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Level.h"

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
	virtual bool blockingFlame() { return false; }
	virtual bool blockingActor() { return false; }
	virtual bool flammable() { return false; }
	virtual bool canExit() { return false; }
	virtual void addGoodie() {};
	virtual bool pickUpGoodie() = 0;
	virtual bool canBePicked() = 0;

private:
	StudentWorld* m_world;
	bool m_alive;
};

class People :public Actor
{
public:
	People(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~People();
	void doSomething();
	void isInfected();
	virtual bool foundExit() = 0;
	bool canExit() { return true; }
private:
	bool m_infected;
};

class Penelope:public People
{
public:
	Penelope(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Penelope();
	void doSomething();
	void createLandmine();
	virtual bool foundExit();
	bool blockingActor() { return true; }
	bool pickUpGoodie() { return true; }


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
	bool blockingActor() { return true; }
	void doSomething();
	~Wall();
};
class Citizen :public People
{
public:
	Citizen(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Citizen();
	void doSomething();
	bool blockingActor() { return true; }
	virtual bool foundExit();
};

class Exit :public Actor
{
public:
	Exit(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	bool blockingFlame() { return true; }
	~Exit();
	void doSomething();
};

class Throwables : public Actor
{
public:
	Throwables(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Throwables();
	void doSomething();
	int getNumTicks();
	void setNumTicks(int num);
private:
	int numTicks;
};
class Flame :public Throwables
{
public:
	Flame(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	bool flammable() { return true; }
	~Flame();
	void doSomething();
};

class Goodie : public Actor
{
public:
	Goodie(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Goodie();
	void doSomething();
	bool canBePicked() { return true; }
};


class GasCanGoodie :public Goodie
{
public:
	GasCanGoodie(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~GasCanGoodie();
	void doSomething();
	virtual void addGoodie();
};

class Landmine :public Actor
{
public:
	Landmine(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Landmine();
	void doSomething();
private:
	bool m_active;
	int safetyTicks;

};

class LandmineGoodie :public Goodie
{
public:
	LandmineGoodie(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~LandmineGoodie();
	void doSomething();
	virtual void addGoodie();
};

class Pit :public Actor
{
public:
	Pit(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Pit();
	void doSomething();
};

class Zombie :public Actor
{
public:
	Zombie(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Zombie();
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

class Vomit :public Throwables
{
public:
	Vomit(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Vomit();
	void doSomething();
};

class VaccineGoodie :public Goodie
{
public:
	VaccineGoodie(int imageID, double startX = 0, double startY = 0, Direction dir = 0, int depth = 0, double size = 1.0);
	~VaccineGoodie();
	void doSomething();
	virtual void addGoodie();
};
#endif // ACTOR_H_
