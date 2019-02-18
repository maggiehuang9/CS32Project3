#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "Actor.h"
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	vector<Actor*> getActors();
	
private:
	void loadLevelFile(Level &lev);
	void createActors(Level &lev);
	Penelope *m_player;
	vector<Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
