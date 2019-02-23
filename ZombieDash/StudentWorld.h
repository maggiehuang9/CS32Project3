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
	void moveActor(Actor & actor, double newX, double newY);
	void addActor(Actor*);
	bool overlapWallExit(const double x, const double y);
	bool exitFound(Actor* actor);
	bool citizensGone();
	void addFlame();
	void addMine();
	void addVaccine();
	void decreaseFlame();
	void decreaseMine();
	void decreaseVaccine();
	int getNumFlame();
	int getNumMine();
	int getNumVaccine();
	void levelFinished();
	bool overlapGoodie(Actor* actor);
private:
	void loadLevelFile(Level &lev);
	void createActors(Level &lev);
	void removeDeadActors();
	bool blockingMovement(const Actor& a, const Actor &b);
	Penelope *m_player;
	vector<Actor*> m_actors;  // contains all actor, including m_player
	int score, numLives, numInfected;
	int numCitizens, numVaccines, numLandmines, numFlamethrowers;
	bool game_level_finished;
};

#endif // STUDENTWORLD_H_
