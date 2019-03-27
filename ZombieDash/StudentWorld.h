#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "Actor.h"
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
inline
double distance(double x1, double x2)
{
	return x1 > x2 ? x1 - x2 : x2 - x1; // compute X ot Y coordinate difference
}

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void moveActor(Actor *actor, double newX, double newY);
	void addActor(Actor*);
	bool overlapWallExit(const double x, const double y);
	bool citizenFoundExit(const Actor *actor);
	void overlapPitFlame(const Actor *actor);
	void overlapVomit(const Actor *actor);
	bool overlapLandmine(Actor *actor);
	bool citizensGone();
	void setGameLevelFinished();
	void decreaseCitizen();
	int getNumZombie();
	Penelope *player();
	double distanceToNearestZombie(const Actor *actor);
	double distanceToNearestZombie(const double x, const double y);
	double distanceToNearestPerson(const double x, const double y);
	Actor *findNearestPerson(const double x, const double y);
	bool blockingMovement(const Actor *actor, double new_x, double new_y);
	//bool noObjectAt(double new_x, double new_y);
	bool personInFrontZombie(const Actor * actor);
private:
	void createActors(Level &lev);
	void removeDeadActors();
	Penelope *m_player;
	vector<Actor*> m_actors;  // contains all actor, including m_player
	int numCitizens;
	bool game_level_finished;
};

#endif // STUDENTWORLD_H_