/**
 * Class Level - Header File
 *
 * This class is for laoding levels, it contains
 * all the level objects, checks for collision with
 * objects, moves the enemies etc.
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#ifndef LEVEL_H_
#define LEVEL_H_

#include <iostream>
#include <vector>
#include <Ogre.h>
#include "SceneryObject.h"
#include "SimpleEnemy.h"
#include "AnimatedEnemy.h"
#include "SceneryObject.h"
#include "Object.h"
#include "Player.h"

using namespace Ogre;
using namespace std;

class Level {

public:
	Level(SceneManager *sceneManagerPointer, RenderWindow *window, Camera *camera);
	virtual ~Level();
	void loadLevel(int levelnumber);
	void unloadLevel();
	bool checkSceneryCollision(Player *player);
	bool checkObjectCollision(Player *player, bool &inverseControls);
	bool checkSimpleEnemyCollision(Player *player);
	void moveEnemies(FrameEvent evt);
	void setParticlePosition(Vector3 position);
	int getLevelLength();
	int getSpeedPoint(int speedpointnumber);
	int getDeathChancePoint(int deathChancePointNumber);
	String getLevelName();


private:
	SceneManager *mSceneManagerPointer;
	RenderWindow *mWindow;
	Camera *mCamera;

    SceneNode* mPickupParticle;
    SceneNode* mParticle;
    ParticleSystem* mParticleSystem;
    SceneNode* mExitP;
    SceneNode* mExitParticleNode;
    ParticleSystem *mExitParticle;

	SceneryObject* mLevelExit;

	vector<SceneryObject> mSceneryObjectVector;
	vector<SceneryObject> mGroundFloorVector;
	vector<SimpleEnemy> mSimpleEnemyVector;
	vector<Object> mObjectVector;
	vector<AnimatedEnemy> mAnimatedEnemyVector;
	vector<Light> mLightVector;

	SceneryObject *mSceneryObjectVectorAdder;
	AnimatedEnemy *mAnimatedEnemyVectorAdder;
	SimpleEnemy *mSimpleEnemyVectorAdder;
	Object *mObjectVectorAdder;
	Light *mLightVectorAdder;

	vector<SceneryObject>::iterator mSceneryObjectIterator;
	vector<SimpleEnemy>::iterator mSimpleEnemyIterator;
	vector<AnimatedEnemy>::iterator mAnimatedEnemyIterator;
	vector<Object>::iterator mObjectIterator;
	vector<Light>::iterator mLightIterator;

	int mCurrentLevelNumber;
	int mCurrentLevelLength;
	int mLevelExitPosition;
	int mIncreaseSpeedPoint1;
	int mIncreaseSpeedPoint2;
	int mIncreaseSpeedPoint3;
	int mDeathChancePoint1;
	int mDeathChancePoint2;
	String mLevelName;


};

#endif /* LEVEL_H_ */
