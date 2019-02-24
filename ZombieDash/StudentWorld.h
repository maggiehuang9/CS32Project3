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
	bool citizensGone();
	void addFlame();
	void addMine();
	void addVaccine();
	void addZombie();
	void decreaseFlame();
	void decreaseMine();
	void decreaseVaccine();
	void setGameLevelFinished();
	void decreaseScore(int n);
	int getNumFlame();
	int getNumMine();
	int getNumVaccine();
	int getNumZombie();
	Penelope &getPlayer();
	double DistanceToNearestZombie(const Actor &actor, int x, int y);
	bool overlapCitizen(const Actor &actor);
	bool blockingMovement(const Actor & actor, double new_x, double new_y);
private:
	void loadLevelFile();
	void createActors(Level &lev);
	void removeDeadActors();
	bool blockingMovemenet(const Actor& a, const Actor &b);
	Penelope *m_player;
	vector<Actor*> m_actors;  // contains all actor, including m_player
	int score, numLives, numInfected;
	int numCitizens, numVaccines, numLandmines, numFlamethrowers, numZombies;
	bool game_level_finished;
	int m_score;
};

#endif // STUDENTWORLD_H_
