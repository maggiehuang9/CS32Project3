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
	virtual ~Actor();
	virtual void doSomething() = 0;
	void setWorld(StudentWorld* world);
	StudentWorld* getWorld();
	void move(double newX, double newY);
	bool isAlive();
	void setState(bool state);
	bool overlap(const Actor &other);
	bool overlap(const double x, const double y);
	virtual bool isPlayer() { return false; }
	virtual bool isZombie() { return false; }
	virtual bool isDumbZombie() { return false; }
	virtual bool isSmartZombie() { return false; }
	virtual bool isCitizen() { return false; }
	virtual bool isWall() { return false; }
	virtual bool isExit() { return false; }
	virtual bool isPit() { return false; }
	virtual bool isGoodie() { return false; }
	virtual bool isVaccineGoodie() { return false; }
	virtual bool isGasCanGoodie() { return false; }
	virtual bool isLandmineGoodie() { return false; }
	virtual bool isFlame() { return false; }
	virtual bool isVomit() { return false; }
	virtual bool isLandmine() { return false; }

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
	void addFlame(int n);
	void addMine(int n);
	void addVaccine(int n);
	int getNumFlame();
	int getNumMine();
	int getNumVaccine();
	void createLandmine();
	bool foundExit();
	virtual bool isPlayer() { return true; }

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
	void doSomething();
	virtual bool isWall() { return true; }
};
class Citizen :public Actor
{
public:
	Citizen(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Citizen();
	virtual bool isCitizen() { return true; }
	void doSomething();
	bool foundExit();
};

class Exit :public Actor
{
public:
	Exit(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Exit();
	virtual bool isExit() { return true; }
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
	Flame(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Flame();
	void doSomething();
	virtual bool isFlame() { return true; }
};

class Goodie : public Actor
{
public:
	Goodie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Goodie();
	virtual void doSomething()=0;
	virtual bool isGoodie() { return true; }

};


class GasCanGoodie :public Goodie
{
public:
	GasCanGoodie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~GasCanGoodie();
	void doSomething();
	virtual bool isGasCanGoodie() { return true; }
};

class Landmine :public Actor
{
public:
	Landmine(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Landmine();
	void doSomething();
	virtual bool isLandmine() { return true; }
};

class LandmineGoodie :public Goodie
{
public:
	LandmineGoodie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~LandmineGoodie();
	void doSomething();
	virtual bool isLandmineGoodie() { return true; }
};

class Pit :public Actor
{
public:
	Pit(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Pit();
	void doSomething();
	virtual bool isPit() { return true; }
};

class Zombie :public Actor
{
public:
	Zombie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Zombie();
	void doSomething()=0;
	virtual bool isZombie() { return true; }
};

class SmartZombie :public Zombie
{
public:
	SmartZombie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~SmartZombie();
	void doSomething();
	virtual bool isSmartZombie() { return true; }
};

class DumbZombie :public Zombie
{
public:
	DumbZombie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~DumbZombie();
	void doSomething();
	virtual bool isDumbZombie() { return true; }
};

class Vomit :public Throwables
{
public:
	Vomit(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~Vomit();
	void doSomething();
	virtual bool isVomit() { return true; }

};

class VaccineGoodie :public Goodie
{
public:
	VaccineGoodie(int imageID, double startX=0, double startY=0, Direction dir = 0, int depth = 0, double size = 1.0);
	~VaccineGoodie();
	void doSomething();
	virtual bool isVaccineGoodie() { return true; }
};
#endif // ACTOR_H_
