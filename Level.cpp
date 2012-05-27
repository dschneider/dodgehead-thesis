/**
 * Class Level - Implementation File
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

#include "Level.h"

#include <iostream>
using namespace std;

Level::Level(SceneManager *sceneManagerPointer, RenderWindow *window, Camera *camera) {

	mSceneManagerPointer = sceneManagerPointer;
	mWindow = window;
	mCamera = camera;

	mLevelName = "";

	mCurrentLevelLength = 0;
	mLevelExitPosition = 0;

	mDeathChancePoint1 = -30000;
	mDeathChancePoint2 = -30000;

    mLevelExit = new SceneryObject(mSceneManagerPointer, "SceneryObjectLevelExit", 1, 0, 0, -26000, 4.0, 0.1, 4.0);
    mLevelExit->addToScene();
    mExitParticle = mSceneManagerPointer->createParticleSystem("ExitParticle","Examples/PurpleFountain");

	mPickupParticle = mSceneManagerPointer->getRootSceneNode()->createChildSceneNode();
    mParticleSystem = mSceneManagerPointer->createParticleSystem("PickupParticle","Examples/GreenyNimbus");

    mExitP = mSceneManagerPointer->getRootSceneNode()->createChildSceneNode();
    mExitParticleNode = mExitP->createChildSceneNode();
    mExitParticleNode->translate(0,50,-26000);
    mExitParticleNode->rotate(Vector3::UNIT_Z, Degree(20));
    mExitParticleNode->attachObject(mExitParticle);
    mExitParticleNode->scale(2.0, 2.0, 2.0);

    // Point the fountain at an angle
    mParticle = mPickupParticle->createChildSceneNode();
    mParticle->translate(0,10,-500);
    mParticle->rotate(Vector3::UNIT_Z, Degree(20));
    mParticle->attachObject(mParticleSystem);
    mParticle->setVisible(false);

}

/*
 * Destructor
 *
 */
Level::~Level() {

}

/*
 * Get the current level name
 *
 *
 */
String Level::getLevelName() {

	return mLevelName;

}

/*
 * Unloads the current level
 *
 *
 */
void Level::unloadLevel() {

	// Remove every SceneryObject from the scene
	for(mSceneryObjectIterator = mSceneryObjectVector.begin(); mSceneryObjectIterator != mSceneryObjectVector.end(); ++mSceneryObjectIterator) {

		mSceneryObjectIterator->removeFromScene();

	}

	// Clear the vector
	mSceneryObjectVector.clear();


	// Remove every GroundFloor from the scene
	for(mSceneryObjectIterator = mGroundFloorVector.begin(); mSceneryObjectIterator != mGroundFloorVector.end(); ++mSceneryObjectIterator) {

		mSceneryObjectIterator->removeFromScene();

	}

	// Clear the vector
	mGroundFloorVector.clear();

	// Remove every SimpleEnemy from the scene
	for (mSimpleEnemyIterator = mSimpleEnemyVector.begin(); mSimpleEnemyIterator != mSimpleEnemyVector.end(); ++mSimpleEnemyIterator) {

		mSimpleEnemyIterator->removeFromScene();

	}

	// Clear the vector
	mSimpleEnemyVector.clear();

	// Remove every object that can be picked up from the scene
	for (mObjectIterator = mObjectVector.begin(); mObjectIterator != mObjectVector.end(); ++mObjectIterator) {

		mObjectIterator->removeFromScene();

	}

	// Clear the vector
	mObjectVector.clear();

	// Remove every light from the scene
	for (mLightIterator = mLightVector.begin(); mLightIterator != mLightVector.end(); ++mLightIterator) {

		mLightIterator->detatchFromParent();
		mSceneManagerPointer->destroyLight(mLightIterator->getName());

	}

	// Clear the vector
	mLightVector.clear();

}

/*
 * Collision detection for scenery objects
 *
 * @param player		a pointer to the player object
 * @return				if there was a collision or not
 *
 */
bool Level::checkSceneryCollision(Player *player) {

	if(mSceneryObjectVector.size() > 0) {

		// For every scenery object
		for (mSceneryObjectIterator = mSceneryObjectVector.begin(); mSceneryObjectIterator != mSceneryObjectVector.end(); ++mSceneryObjectIterator) {

			// Is the players x position (positive / negative) + the width of his bounding box within the position of the bounding box of an object
			if((player->getPosition().x + (player->getAABB().getMaximum().x - 15) >= mSceneryObjectIterator->getPosition().x - (mSceneryObjectIterator->getAABB().getMaximum().x)) && (player->getPosition().x - (player->getAABB().getMaximum().x - 15) <= mSceneryObjectIterator->getPosition().x + (mSceneryObjectIterator->getAABB().getMaximum().x))) {

				// Is the players z position (positive / negative) + the width of his bounding box within the position of the bounding box of an object
				if((player->getPosition().z - (player->getAABB().getMaximum().z - 15) <= mSceneryObjectIterator->getPosition().z + (mSceneryObjectIterator->getAABB().getMaximum().z)) && (player->getPosition().z + (player->getAABB().getMaximum().z - 15) >= mSceneryObjectIterator->getPosition().z - (mSceneryObjectIterator->getAABB().getMaximum().z))) {

					return true;

				}

			}

		}

	}

	return false;

}

/*
 * Collision detection for pickup objects
 *
 * @param player		a pointer to the player object
 * @return				if there was a collision or not
 *
 */
bool Level::checkObjectCollision(Player *player, bool &inverseControls) {

	if(mObjectVector.size() > 0) {

		for (mObjectIterator = mObjectVector.begin(); mObjectIterator != mObjectVector.end(); ++mObjectIterator) {

			if((player->getPosition().x + (player->getAABB().getMaximum().x - 5) >= mObjectIterator->getPosition().x - (mObjectIterator->getAABB().getMaximum().x)) && (player->getPosition().x - (player->getAABB().getMaximum().x - 5) <= mObjectIterator->getPosition().x + (mObjectIterator->getAABB().getMaximum().x))) {

				if((player->getPosition().z - (player->getAABB().getMaximum().z - 5) <= mObjectIterator->getPosition().z + (mObjectIterator->getAABB().getMaximum().z)) && (player->getPosition().z + (player->getAABB().getMaximum().z - 5) >= mObjectIterator->getPosition().z - (mObjectIterator->getAABB().getMaximum().z))) {

					mObjectIterator->setInvisible();
					mParticle->setPosition(mObjectIterator->getPosition().x, 10, mObjectIterator->getPosition().z);
					mParticle->setVisible(true);

					// Check the object type
					if(mObjectIterator->getObjectType() == 1) {

						player->incrementScore(100);

					} else if(mObjectIterator->getObjectType() == 2) {

						player->incrementScore(500);
						CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "Radial Blur", true);

					} else if(mObjectIterator->getObjectType() == 3) {

						player->incrementScore(500);
						inverseControls = true;

					} else if(mObjectIterator->getObjectType() == 4) {

						player->incrementScore(50);
						player->setInvulnerable();
						player->activateShader(1);
						player->activateLight();

					} else if(mObjectIterator->getObjectType() == 5) {

						player->increaseLifes();
						player->incrementScore(50);

					} else if(mObjectIterator->getObjectType() == 6) {

						player->incrementScore(50);
						for (mSimpleEnemyIterator = mSimpleEnemyVector.begin(); mSimpleEnemyIterator != mSimpleEnemyVector.end(); ++mSimpleEnemyIterator) {

							mSimpleEnemyIterator->setSpeed(0.2);

						}

					}

					return true;

				}

			}

		}

	}

	return false;

}

/*
 * Collision detection for simple enemies
 *
 * @param player		a pointer to the player object
 * @return				if there was a collision or not
 *
 */
bool Level::checkSimpleEnemyCollision(Player *player) {

	if(mSimpleEnemyVector.size() > 0) {

		for (mSimpleEnemyIterator = mSimpleEnemyVector.begin(); mSimpleEnemyIterator != mSimpleEnemyVector.end(); ++mSimpleEnemyIterator) {

			if((player->getPosition().x + (player->getAABB().getMaximum().x - 10) >= mSimpleEnemyIterator->getPosition().x - (mSimpleEnemyIterator->getAABB().getMaximum().x)) && (player->getPosition().x - (player->getAABB().getMaximum().x - 10) <= mSimpleEnemyIterator->getPosition().x + (mSimpleEnemyIterator->getAABB().getMaximum().x))) {

				if((player->getPosition().z - (player->getAABB().getMaximum().z - 10) <= mSimpleEnemyIterator->getPosition().z + (mSimpleEnemyIterator->getAABB().getMaximum().z)) && (player->getPosition().z + (player->getAABB().getMaximum().z - 10) >= mSimpleEnemyIterator->getPosition().z - (mSimpleEnemyIterator->getAABB().getMaximum().z))) {

					return true;

				}

			}

		}

	}

	return false;

}

/*
 * Move the enemies within the level
 *
 * @param evt		ogre frame event
 *
 */
void Level::moveEnemies(FrameEvent evt) {

	if(mSimpleEnemyVector.size() > 0) {

		for (mSimpleEnemyIterator = mSimpleEnemyVector.begin(); mSimpleEnemyIterator != mSimpleEnemyVector.end(); ++mSimpleEnemyIterator) {

			mSimpleEnemyIterator->move(evt);

		}

	}

}

/*
 * Get the speed point, where the speed is increased
 *
 * @param speedpointnumber		number of the speed point
 * @return 						the speed point
 *
 */
int Level::getSpeedPoint(int speedpointnumber) {

	switch(speedpointnumber) {

	case 1:
		return mIncreaseSpeedPoint1;
		break;

	case 2:
		return mIncreaseSpeedPoint2;
		break;

	case 3:
		return mIncreaseSpeedPoint3;
		break;

	}

	return 0;

}

/*
 * Get the death chance point, where scale matrix is needed
 * to survive
 *
 * @param deathChancePointNumber	number of the death chance point
 * @return 							the death chance point
 *
 */
int Level::getDeathChancePoint(int deathChancePointNumber) {

	switch(deathChancePointNumber) {

	case 1:
		return mDeathChancePoint1;
		break;

	case 2:
		return mDeathChancePoint2;
		break;

	}

	return 0;

}

/*
 * Get the length of the level
 *
 * @return		the level length
 *
 */
int Level::getLevelLength() {

	return mCurrentLevelLength;

}

/*
 * Set the position of the pickup particle
 *
 * @param position		position where the particle should be
 *
 */
void Level::setParticlePosition(Vector3 position) {

	mParticle->setPosition(position.x, 10, position.z);
	mParticle->setVisible(true);

}

/*
 * Load a new level
 *
 * @param levelnumber		the number of the level
 *
 */
void Level::loadLevel(int levelnumber) {

	switch(levelnumber) {

	case 0:

		// Set ambient light
		mSceneManagerPointer->setAmbientLight(ColourValue(0.2, 0.2, 0.2));
		mSceneManagerPointer->setFog(FOG_NONE);
		mSceneManagerPointer->setSkyBox(true, "Examples/SpaceSkyBox");

		mSceneManagerPointer->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

		mCamera->setPolygonMode(PM_SOLID);

		// Set light
		mLightVectorAdder = mSceneManagerPointer->createLight("MainLight");
		mLightVectorAdder->setType(Light::LT_DIRECTIONAL);
		mLightVectorAdder->setDirection(Vector3(-0.5, -1, -1));
		mLightVector.push_back(*mLightVectorAdder);

		// Main menu level
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(1), 4, 400, 100, 0, 0.2, 0.2, 0.2);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(2), 4, -400, 100, 0, 0.2, 0.2, 0.2);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(3), 4, -800, 100, 100, 0.2, 0.2, 0.2);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(4), 4, 400, 100, 400, 0.2, 0.2, 0.2);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(5), 4, 800, 100, 10, 0.2, 0.2, 0.2);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Ground floor
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(78), 6, 0, 0, 0, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);

		break;

	case 1:
		mCurrentLevelNumber = 1;
		mCurrentLevelLength = -26000;
		mIncreaseSpeedPoint1 = -30000;
		mIncreaseSpeedPoint2 = -30000;
		mIncreaseSpeedPoint3 = -30000;

		// Set level name
		mLevelName = "Level 1 : Polygonal Horror";

		// Set polygon mode to wireframe
		mCamera->setPolygonMode(PM_WIREFRAME);

		// Create fog
		mSceneManagerPointer->setFog(FOG_LINEAR, ColourValue(0.93, 0.86, 0.76), .001, 500, 1000);
		mWindow->getViewport(0)->setBackgroundColour(ColourValue(0.93, 0.86, 0.76));

		// SET SCENERY OBJECTS
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(1), 5, 0, 50, -3000, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(2), 5, 0, 50, -3300, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(3), 5, 0, 50, -3600, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(4), 5, 190, 50, -3900, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(5), 5, -190, 50, -3900, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(6), 5, 0, 50, -4200, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(7), 5, -190, 50, -4500, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(8), 5, 190, 50, -4500, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(9), 5, 0, 50, -4800, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(10), 5, 0, 50, -5100, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(11), 5, 190, 50, -6300, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(12), 5, -190, 50, -6300, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(13), 5, 0, 50, -6600, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(14), 5, 0, 50, -6900, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(15), 5, 0, 50, -7200, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(16), 5, 0, 50, -7500, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(17), 5, 0, 50, -8400, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(18), 5, 0, 50, -8700, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(19), 5, 0, 50, -9000, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(20), 5, 0, 50, -9300, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(21), 5, 0, 50, -9600, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(22), 5, 0, 50, -9900, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(23), 5, -190, 50, -10300, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(24), 5, 190, 50, -10300, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(25), 5, 0, 50, -10600, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(26), 5, -190, 50, -10900, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(27), 5, 190, 50, -10900, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(28), 5, 0, 50, -11200, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(29), 5, 0, 50, -11500, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(30), 5, -190, 50, -11800, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(31), 5, 190, 50, -11800, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(32), 5, -190, 50, -13000, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(33), 5, 190, 50, -13000, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(34), 5, 0, 50, -13000, 0.5, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(35), 5, -190, 50, -13500, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(36), 5, 190, 50, -13500, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(37), 5, 0, 50, -13500, 0.5, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(38), 5, -190, 50, -14000, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(39), 5, 190, 50, -14000, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(40), 5, 0, 50, -14000, 0.5, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(41), 5, 0, 50, -14500, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(42), 5, 0, 50, -15000, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(43), 5, 0, 50, -16200, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(44), 5, 0, 50, -16700, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(45), 5, -190, 50, -17200, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(46), 5, 190, 50, -17200, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(47), 5, -190, 50, -17700, 3.8, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(49), 5, 190, 50, -18200, 3.8, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(50), 5, -190, 50, -18700, 3.8, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(51), 5, 190, 50, -19200, 3.8, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(52), 5, -190, 50, -19500, 3.8, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(53), 5, 190, 50, -19800, 3.8, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(54), 5, 0, 50, -20300, 3.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(55), 5, 0, 50, -20800, 3.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(56), 5, 0, 50, -21300, 3.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(57), 5, 50, 50, -23400, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(58), 5, 0, 50, -23700, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(59), 5, -50, 50, -24000, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(60), 5, -30, 50, -24300, 2.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(61), 5, 0, 50, -24700, 1.0, 1.0, 1.0);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Ground floor
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(78), 6, 0, 0, -500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(79), 6, 0, 0, -5500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(80), 6, 0, 0, -10500, 1.0, 1.0, 1.0 , 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(81), 6, 0, 0, -15500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(82), 6, 0, 0, -20500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(83), 6, 0, 0, -25500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);

		// SET ENEMIES
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(1), 1, Math::RangeRandom(-180, 180), -5400, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(2), 1, Math::RangeRandom(-180, 180), -5700, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(3), 1, Math::RangeRandom(-180, 180), -6000, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(4), 1, Math::RangeRandom(-180, 180), -12100, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(5), 1, Math::RangeRandom(-180, 180), -12400, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(6), 1, Math::RangeRandom(-180, 180), -12700, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(7), 1, Math::RangeRandom(-180, 180), -15300, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(8), 1, Math::RangeRandom(-180, 180), -15600, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(9), 1, Math::RangeRandom(-180, 180), -21600, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(10), 1, Math::RangeRandom(-180, 180), -21900, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(11), 1, Math::RangeRandom(-180, 180), -22200, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(12), 1, Math::RangeRandom(-180, 180), -22500, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(13), 1, Math::RangeRandom(-180, 180), -22800, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(14), 1, Math::RangeRandom(-180, 180), -23100, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);

		// SET OBJECTS (1, 3, 5, 6 allowed)
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(1), 1, 0, -600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(2), 1, 0, -800);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(3), 1, 0, -1000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(4), 1, 0, -1200);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(5), 1, 0, -1400);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(6), 1, 0, -3150);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(7), 1, 0, -3450);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(8), 1, 0, -3750);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(9), 1, 0, -4050);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(10), 1, 0, -4350);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(11), 1, 0, -4650);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(12), 1, 0, -4950);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(13), 1, 0, -5250);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(14), 1, 0, -5550);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(15), 1, 0, -5850);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(16), 1, 0, -6150);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(17), 1, 0, -6450);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(18), 1, 0, -6750);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(19), 1, 0, -7050);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(20), 1, 0, -7350);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(21), 1, 0, -7650);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(22), 1, 0, -7950);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(23), 1, 0, -8250);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(24), 1, 0, -8550);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(25), 1, 0, -8850);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(26), 1, 0, -9150);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(27), 1, 0, -9450);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(28), 1, 0, -9750);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(29), 1, 0, -10050);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(30), 1, 0, -10350);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(31), 1, 0, -10650);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(32), 1, 0, -10650);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(33), 1, 0, -10950);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(34), 1, -100, -11250);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(35), 1, -100, -11550);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(36), 1, 0, -11850);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(37), 1, 0, -12150);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(38), 1, 0, -12450);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(39), 1, 0, -12750);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(40), 1, 50, -13000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(41), 1, 0, -13350);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(42), 1, -50, -13520);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(43), 1, 50, -14000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(44), 1, 0, -14250);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(45), 1, 0, -14750);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(46), 1, 0, -15300);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(47), 1, 0, -15600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(48), 1, 0, -15900);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(49), 1, 0, -16500);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(50), 1, -150, -16800);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(51), 1, 10, -17100);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(52), 1, -50, -17400);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(53), 1, 60, -17700);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(54), 1, 80, -18000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(55), 1, -80, -18300);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(56), 1, -80, -18500);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(57), 1, -80, -18900);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(58), 1, -80, -19200);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(59), 1, 150, -19450);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(60), 1, -150, -19800);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(61), 1, 80, -20100);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(62), 1, 80, -20400);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(63), 1, -80, -20700);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(64), 1, 0, -21000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(65), 1, 0, -21600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(66), 1, 0, -21900);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(67), 1, 0, -22200);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(68), 1, 0, -22500);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(69), 1, 0, -22800);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(70), 1, 0, -23100);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(71), 1, 150, -23400);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(72), 1, -150, -23700);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(73), 1, 100, -24000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(74), 1, 150, -24300);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(75), 1, -150, -24700);
		mObjectVector.push_back(*mObjectVectorAdder);

		break;

	case 2:
		mCurrentLevelNumber = 2;
		mCurrentLevelLength = -26000;
		mIncreaseSpeedPoint1 = -2000;
		mIncreaseSpeedPoint2 = -20000;
		mIncreaseSpeedPoint3 = -27000;

		mLevelName = "Level 2 : Colors, but no shading";

		// Set ambient light
		mSceneManagerPointer->setAmbientLight(ColourValue(0.4, 0.4, 0.4));

		// Set polygon mode to wireframe
		mCamera->setPolygonMode(PM_SOLID);

		// Create fog
		mSceneManagerPointer->setFog(FOG_LINEAR, ColourValue(0.93, 0.86, 0.76), .001, 500, 1000);
		mWindow->getViewport(0)->setBackgroundColour(ColourValue(0.93, 0.86, 0.76));

		// SET SCENERY OBJECTS
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(1), 5, 0, 50, -1000, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(2), 5, 100, 50, -1300, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(3), 5, 70, 50, -1600, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(4), 5, 30, 50, -1900, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(5), 5, -30, 50, -2200, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(6), 5, 50, 50, -2500, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(7), 5, -180, 50, -2800, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(8), 5, 0, 50, -3100, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(9), 5, 0, 50, -3400, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(10), 5, 0, 50, -3700, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Diagonal blocks
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(11), 5, 190, 50, -4000, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(12), 5, 130, 50, -4150, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(13), 5, 70, 50, -4300, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Diagonal blocks
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(14), 5, -190, 50, -4500, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(15), 5, -130, 50, -4650, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(16), 5, -70, 50, -4800, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Diagonal blocks
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(17), 5, 190, 50, -5000, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(18), 5, 130, 50, -5150, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(19), 5, 70, 50, -5300, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Diagonal blocks
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(20), 5, -60, 50, -5500, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(21), 5, 60, 50, -5850, 1.5, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(22), 5, -60, 50, -6150, 1.5, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(23), 5, 60, 50, -6500, 1.5, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(24), 5, -60, 50, -6850, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(25), 5, -190, 50, -7200, 2.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(26), 5, 190, 50, -7200, 2.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(27), 5, 0, 50, -7250, 0.4, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(28), 5, -190, 50, -8500, 2.0, 1.0, 5.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(29), 5, 190, 50, -8500, 2.0, 1.0, 5.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(30), 5, 0, 50, -8500, 0.4, 1.0, 5.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(31), 5, -190, 50, -9500, 2.0, 1.0, 6.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(32), 5, 190, 50, -9500, 2.0, 1.0, 6.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(33), 5, 0, 50, -9500, 0.4, 1.0, 5.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(34), 5, -190, 50, -11500, 4.5, 1.0, 2.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(35), 5, 190, 50, -11000, 2.5, 1.0, 5.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(36), 5, 50, 50, -11000, 1.5, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(37), 5, 50, 50, -11500, 1.5, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(38), 5, 20, 50, -11800, 1.5, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(39), 5, -20, 50, -11800, 1.5, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(40), 5, -60, 50, -11800, 1.5, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(41), 5, 100, 50, -11800, 1.5, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(42), 5, 50, 50, -12100, 1.5, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(43), 5, -50, 50, -12400, 1.5, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(44), 5, 0, 50, -12800, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(45), 5, 0, 50, -13100, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(46), 5, 0, 50, -13400, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(47), 5, 0, 50, -13700, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(48), 5, 0, 50, -14000, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(49), 5, 0, 50, -14300, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(50), 5, 0, 50, -14600, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(51), 5, 0, 50, -14900, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(52), 5, 0, 50, -14900, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(53), 5, 0, 50, -14900, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(54), 5, 0, 50, -15200, 2.0, 1.0, 4.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(55), 5, 0, 50, -16400, 2.0, 1.0, 5.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(56), 5, 190, 50, -17400, 4.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(57), 5, -190, 50, -17800, 4.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(58), 5, 190, 50, -18200, 4.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(59), 5, -190, 50, -18600, 4.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(60), 5, 0, 50, -19000, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(61), 5, 0, 50, -19400, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(62), 5, 0, 50, -19700, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(63), 5, 0, 50, -20000, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(64), 5, 0, 50, -20300, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(65), 5, -190, 50, -22000, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(66), 5, 190, 50, -22000, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(67), 5, -190, 50, -22600, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(68), 5, 190, 50, -22600, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(69), 5, -190, 50, -22900, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(70), 5, 190, 50, -22900, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(71), 5, -140, 50, -23200, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(72), 5, 140, 50, -23200, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(73), 5, -90, 50, -23500, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(74), 5, 90, 50, -23500, 1.0, 1.0, 1.0, 1);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Ground floor
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(123), 6, 0, 0, -500, 1.0, 1.0, 1.0);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(123123), 6, 0, 0, -5500, 1.0, 1.0, 1.0);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(12313), 6, 0, 0, -10500, 1.0, 1.0, 1.0);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(1222), 6, 0, 0, -15500, 1.0, 1.0, 1.0);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(5155), 6, 0, 0, -20500, 1.0, 1.0, 1.0);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(6634), 6, 0, 0, -25500, 1.0, 1.0, 1.0);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);

		// SET ENEMIES
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(1), 1, Math::RangeRandom(-180, 180), -7500, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(2), 1, Math::RangeRandom(-180, 180), -7800, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(3), 1, Math::RangeRandom(-180, 180), -8100, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(4), 1, Math::RangeRandom(-180, 180), -8800, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(5), 1, Math::RangeRandom(-180, 180), -9100, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(6), 1, Math::RangeRandom(-180, 180), -15400, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(7), 1, Math::RangeRandom(-180, 180), -15700, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(8), 1, Math::RangeRandom(-180, 180), -16000, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(9), 1, Math::RangeRandom(-180, 180), -20700, 1.0, 0.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(10), 1, Math::RangeRandom(-180, 180), -21000, 1.0, 0.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(11), 1, Math::RangeRandom(-180, 180), -21300, 1.0, 0.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(12), 1, Math::RangeRandom(-180, 180), -21600, 1.0, 0.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);

		// SET OBJECTS
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(1), 1, 170, -1600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(2), 1, 25, -1750);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(3), 1, -160, -2050);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(4), 1, 167, -2325);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(5), 1, 80, -2325);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(6), 3, 60, -2700);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(7), 1, -60, -2700);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(8), 1, 0, -3300);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(9), 1, -100, -3400);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(10), 1, 0, -3500);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(11), 1, 115, -3700);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(12), 1, -190, -4650);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(13), 1, 0, -5000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(14), 1, -70, -5290);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(15), 1, -190, -5500);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(16), 3, -80, -5850);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(17), 1, 135, -6150);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(18), 1, 190, -6500);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(19), 1, -190, -6700);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(20), 1, 60, -7220);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(21), 1, -100, -7800);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(22), 1, 0, -7950);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(23), 1, -60, -8350);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(24), 1, -60, -8400);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(25), 1, 60, -8550);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(26), 1, 60, -9300);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(27), 5, 60, -9500);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(28), 1, -60, -9500);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(29), 1, -150, -10500);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(30), 1, -20, -10700);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(31), 1, -20, -10840);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(32), 1, -25, -11200);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(33), 1, 200, -11600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(34), 1, 100, -12000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(35), 1, 45, -12250);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(36), 1, -17, -12640);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(37), 1, 0, -13250);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(38), 1, 0, -12950);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(39), 5, 0, -13550);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(40), 1, 0, -13850);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(41), 1, 0, -14150);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(42), 1, 0, -14450);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(43), 1, 0, -14750);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(44), 1, 200, -15000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(45), 1, 200, -15000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(46), 1, -200, -15200);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(47), 1, -200, -15200);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(48), 1, 0, -15650);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(49), 1, 0, -15950);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(50), 1, 200, -16200);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(51), 1, 200, -16500);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(52), 1, -200, -16300);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(53), 1, 0, -16900);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(54), 1, -145, -17550);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(55), 1, 80, -17800);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(56), 1, -100, -18100);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(57), 1, -100, -18300);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(58), 1, 65, -18470);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(59), 2, 25, -18790);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(60), 1, 0, -19200);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(61), 1, 0, -19550);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(62), 1, 0, -19850);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(63), 1, 0, -20150);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(64), 1, 150, -20800);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(65), 1, -150, -21300);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(66), 1, 100, -21650);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(67), 1, 0, -22000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(68), 1, 0, -22500);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(69), 1, 150, -22760);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(70), 1, 0, -23500);
		mObjectVector.push_back(*mObjectVectorAdder);
		break;


	case 3:
		mCurrentLevelNumber = 3;
		mCurrentLevelLength = -26000;
		mIncreaseSpeedPoint1 = -2000;
		mIncreaseSpeedPoint2 = -17000;
		mIncreaseSpeedPoint3 = -27000;

		mLevelName = "Level 3 : Say 'Hello' to Textures!";

		mSceneManagerPointer->setFog(FOG_NONE);

		// Set ambient light
		mSceneManagerPointer->setAmbientLight(ColourValue(0.4, 0.4, 0.4));

		// Set polygon mode to wireframe
		mCamera->setPolygonMode(PM_SOLID);

		// SET SCENERY OBJECTS
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(1), 5, 190, 50, -3000, 2.0, 1.0, 5.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(2), 5, -190, 50, -3000, 2.0, 1.0, 5.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(3), 5, 0, 50, -4000, 2.0, 1.0, 5.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(4), 5, 190, 50, -5000, 2.0, 1.0, 5.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(5), 5, -190, 50, -5000, 2.0, 1.0, 5.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(6), 5, 0, 50, -6000, 2.0, 1.0, 5.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(7), 5, 190, 50, -7000, 2.0, 1.0, 5.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(8), 5, -190, 50, -7000, 2.0, 1.0, 5.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(9), 5, 0, 50, -8000, 2.0, 1.0, 5.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(10), 4, 0, 150, -9000, 0.3, 0.3, 0.3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(11), 4, 0, 150, -10000, 0.3, 0.3, 0.3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(12), 4, 0, 150, -11000, 0.3, 0.3, 0.3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(13), 4, 0, 150, -12000, 0.3, 0.3, 0.3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(14), 4, -190, 150, -12800, 0.3, 0.3, 0.3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(15), 4, 190, 150, -12800, 0.3, 0.3, 0.3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(16), 5, 0, 50, -13300, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(17), 5, 0, 50, -13700, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(18), 5, 0, 50, -14000, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(19), 5, 0, 50, -14300, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(20), 5, 0, 50, -14700, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(21), 5, 0, 50, -15000, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(22), 5, 0, 50, -15300, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(23), 5, 0, 50, -15800, 3.5, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(24), 5, -190, 50, -16400, 3.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(25), 5, 190, 50, -16400, 3.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(26), 5, 0, 50, -17000, 3.5, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(27), 5, 0, 50, -17600, 3.5, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(28), 5, 190, 50, -18200, 3.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(29), 5, -190, 50, -18200, 3.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(30), 5, 0, 50, -18600, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(31), 5, 0, 50, -18900, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(32), 5, 0, 50, -19300, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(33), 5, 0, 50, -19600, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(34), 5, 0, 50, -19300, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(35), 5, 0, 50, -19300, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Ground floor
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(78), 6, 0, 0, -500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(79), 6, 0, 0, -5500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(80), 6, 0, 0, -10500, 1.0, 1.0, 1.0 , 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(81), 6, 0, 0, -15500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(82), 6, 0, 0, -20500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(83), 6, 0, 0, -25500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);

		// SET ENEMIES
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(1), 1, Math::RangeRandom(-180, 180), -9300, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(2), 1, Math::RangeRandom(-180, 180), -9600, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(3), 1, Math::RangeRandom(-180, 180), -10300, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(4), 1, Math::RangeRandom(-180, 180), -10600, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(5), 1, Math::RangeRandom(-180, 180), -11300, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(6), 1, Math::RangeRandom(-180, 180), -11600, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(7), 1, 180, -21000, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(8), 1, -180, -21300, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(9), 1, 180, -21600, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(10), 1, -180, -21900, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(11), 1, 180, -22200, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(12), 1, -180, -22500, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(13), 1, 180, -22800, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(14), 1, -180, -23100, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(15), 1, 180, -23400, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);

		// SET OBJECTS
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(1), 1, 0, -600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(2), 1, 0, -800);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(3), 1, 0, -1000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(4), 1, 30, -1200);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(5), 1, -300, -1200);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(6), 1, 0, -3150);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(7), 1, 0, -3450);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(8), 3, 0, -3750);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(9), 1, 0, -4050);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(10), 1, 0, -4350);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(11), 3, 0, -4650);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(12), 1, 0, -10600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(13), 1, 0, -11600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(14), 1, 0, -12600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(15), 1, 0, -13600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(16), 5, 0, -14600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(17), 1, 0, -15600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(18), 1, 0, -16600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(19), 1, 0, -17600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(20), 1, 0, -18600);
		mObjectVector.push_back(*mObjectVectorAdder);

		// Create a skybox
		mSceneManagerPointer->setSkyBox(true, "Examples/CloudyNoonSkyBox");
		break;

	case 4:
		mCurrentLevelNumber = 4;
		mCurrentLevelLength = -26000;
		mIncreaseSpeedPoint1 = -2000;
		mIncreaseSpeedPoint2 = -12000;
		mIncreaseSpeedPoint3 = -27000;

		mSceneManagerPointer->setFog(FOG_NONE);

		mLevelName = "Level 4 : Let there be light";

		// Set ambient light
		mSceneManagerPointer->setAmbientLight(ColourValue(0.0, 0.0, 0.0));

		mSceneManagerPointer->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

		// Set polygon mode to wireframe
		mCamera->setPolygonMode(PM_SOLID);

		mLightVectorAdder = mSceneManagerPointer->createLight("MainLight");
		mLightVectorAdder->setType(Light::LT_DIRECTIONAL);
		mLightVectorAdder->setDirection(Vector3(-0.5, -1, -1));
		mLightVector.push_back(*mLightVectorAdder);

		// SET SCENERY OBJECTS
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(1), 5, 0, 50, -3000, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(2), 5, 0, 50, -3300, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(3), 5, 0, 50, -3600, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(4), 5, 0, 50, -3900, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(5), 5, 0, 50, -4200, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(6), 5, 0, 50, -4500, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(7), 5, 0, 50, -4800, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(8), 5, 0, 50, -5100, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(9), 5, 0, 50, -5400, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(10), 5, 0, 50, -5700, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(11), 5, 0, 50, -6000, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(12), 5, 150, 50, -6300, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(13), 5, -150, 50, -6300, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(14), 5, 150, 50, -6600, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(15), 5, -150, 50, -6600, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(16), 5, 150, 50, -6900, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(17), 5, -150, 50, -6900, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(18), 5, 150, 50, -7200, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(19), 5, -150, 50, -7200, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(20), 5, 150, 50, -7500, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(21), 5, -150, 50, -7500, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(22), 5, 0, 0, -7500, 0.5, 0.5, 0.5, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(23), 5, 0, 0, -7800, 0.5, 0.5, 0.5, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(24), 5, 0, 0, -8100, 0.5, 0.5, 0.5, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(25), 5, 0, 0, -8400, 0.5, 0.5, 0.5, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(26), 5, 0, 0, -8700, 0.5, 0.5, 0.5, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(27), 5, 150, 50, -7800, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(28), 5, -150, 50, -7800, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(29), 5, 150, 50, -8100, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(30), 5, -150, 50, -8100, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(31), 4, 150, 50, -8400, 0.2, 0.2, 0.2, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(32), 4, -150, 50, -8400, 0.2, 0.2, 0.2, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(33), 4, 150, 50, -8700, 0.2, 0.2, 0.2, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(34), 4, -150, 50, -8700, 0.2, 0.2, 0.2, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(35), 5, 100, 50, -9000, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(36), 5, -100, 50, -9000, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(37), 5, 0, 50, -9300, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(38), 5, 100, 50, -9600, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(39), 5, -100, 50, -9600, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(40), 5, 0, 50, -9900, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(41), 5, 100, 50, -10200, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(42), 5, -100, 50, -10200, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(43), 5, 0, 50, -10500, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(44), 5, 190, 50, -12800, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(45), 5, -190, 50, -12800, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(46), 5, 190, 50, -13100, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(47), 5, -190, 50, -13100, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(48), 5, 140, 50, -13400, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(49), 5, -140, 50, -13400, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(50), 5, 140, 50, -13700, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(51), 5, -140, 50, -13700, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(52), 5, 110, 50, -14000, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(53), 5, -110, 50, -14000, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Diagonal blocks
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(54), 5, 190, 50, -14300, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(55), 5, 130, 50, -14450, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(56), 5, 70, 50, -14600, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(57), 5, 10, 50, -14750, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(58), 5, -50, 50, -14900, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(59), 5, -110, 50, -15050, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Diagonal blocks
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(61), 5, -190, 50, -15500, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(62), 5, -130, 50, -15650, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(63), 5, -70, 50, -15800, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(64), 5, -10, 50, -15950, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(65), 5, 50, 50, -16100, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(66), 5, 110, 50, -16250, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Diagonal blocks
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(67), 5, 190, 50, -16700, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(68), 5, 130, 50, -16850, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(69), 5, 70, 50, -17000, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(70), 5, 10, 50, -17150, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(71), 5, -50, 50, -17300, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(72), 5, -110, 50, -17450, 1.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Straight blocks
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(73), 5, -60, 50, -18000, 4.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(74), 5, 60, 50, -18500, 4.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(75), 5, -60, 50, -19000, 4.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(76), 5, 60, 50, -19500, 4.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(77), 5, -60, 50, -20000, 4.0, 1.0, 1.0, 3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// Ground floor
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(78), 6, 0, 0, -500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(79), 6, 0, 0, -5500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(80), 6, 0, 0, -10500, 1.0, 1.0, 1.0 , 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(81), 6, 0, 0, -15500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(82), 6, 0, 0, -20500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(83), 6, 0, 0, -25500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);

		// SET ENEMIES
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(1), 1, Math::RangeRandom(-180, 180), -11000, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(2), 1, Math::RangeRandom(-180, 180), -11300, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(3), 1, Math::RangeRandom(-180, 180), -11600, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(4), 1, Math::RangeRandom(-180, 180), -11900, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(5), 1, Math::RangeRandom(-180, 180), -12200, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(6), 1, Math::RangeRandom(-180, 180), -12500, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(7), 1, 180, -21000, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(8), 1, -180, -21300, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(9), 1, 180, -21600, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(10), 1, -180, -21900, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(11), 1, 180, -22200, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(12), 1, -180, -22500, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(13), 1, 180, -22800, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(14), 1, -180, -23100, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(15), 1, 180, -23400, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);

		// SET OBJECTS
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(1), 1, 0, -600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(2), 1, 0, -1600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(3), 1, 0, -2000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(4), 2, 0, -2600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(5), 1, 0, -3600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(6), 1, 0, -4600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(7), 1, 0, -5600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(8), 3, 0, -6600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(9), 1, 0, -7600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(10), 1, 0, -8600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(11), 5, 0, -9600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(12), 1, 0, -10600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(13), 1, 0, -11600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(14), 1, 0, -12600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(15), 5, 0, -13600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(16), 1, 0, -14600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(17), 1, 0, -15600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(18), 1, 0, -16600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(19), 1, 0, -17600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(20), 1, 0, -18600);
		mObjectVector.push_back(*mObjectVectorAdder);

		// Create a skybox
		mSceneManagerPointer->setSkyBox(true, "Examples/CloudyNoonSkyBox");
		break;

	case 5:
		mCurrentLevelNumber = 5;
		mCurrentLevelLength = -26000;
		mIncreaseSpeedPoint1 = -2000;
		mIncreaseSpeedPoint2 = -8000;
		mIncreaseSpeedPoint3 = -18000;
		mDeathChancePoint1 = -3700;

		mLevelName = "Level 5 : Turn on FX";

		mSceneManagerPointer->setFog(FOG_NONE);

		// Set ambient light
		mSceneManagerPointer->setAmbientLight(ColourValue(0.0, 0.0, 0.0));

		// Set shadow mode to
		mSceneManagerPointer->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

		// Activate bloom effect
		CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "Bloom", true);

		// Set polygon mode to wireframe
		mCamera->setPolygonMode(PM_SOLID);

		mLightVectorAdder = mSceneManagerPointer->createLight("MainLight");
		mLightVectorAdder->setType(Light::LT_DIRECTIONAL);
		mLightVectorAdder->setDirection(Vector3(-0.5, -1, -1));
		mLightVector.push_back(*mLightVectorAdder);

		// SET SCENERY OBJECTS
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(1), 5, -190, 50, -3000, 3.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(2), 5, 190, 50, -3000, 3.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(3), 5, 0, 50, -3300, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(4), 5, -190, 50, -4000, 3.5, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(5), 5, 190, 50, -4000, 3.5, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(6), 5, 0, 50, -4300, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(7), 5, 0, 50, -4600, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(8), 5, 0, 50, -4900, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(9), 5, 0, 50, -5200, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(10), 5, 0, 50, -5500, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(11), 5, 0, 50, -5800, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(12), 5, 0, 50, -6100, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(13), 5, -100, 50, -6600, 1.0, 1.0, 5.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(14), 5, 100, 50, -6600, 1.0, 1.0, 5.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(15), 5, 0, 50, -8000, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(16), 5, -100, 50, -8500, 1.0, 1.0, 5.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(17), 5, 100, 50, -8500, 1.0, 1.0, 5.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(18), 4, 0, 150, -9500, 0.3, 0.3, 0.3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(19), 4, 0, 150, -10500, 0.3, 0.3, 0.3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(20), 4, 0, 150, -12500, 0.3, 0.3, 0.3);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(21), 5, 190, 50, -12800, 2.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(22), 5, -190, 50, -12800, 2.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(23), 5, 0, 50, -13100, 0.5, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(24), 5, 100, 50, -13400, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(25), 5, -100, 50, -13400, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(26), 5, 0, 50, -13700, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(27), 5, 0, 50, -14000, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(28), 5, 0, 50, -14300, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(29), 5, 0, 50, -14700, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(30), 5, 0, 50, -15000, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(31), 5, 0, 50, -15300, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(32), 5, 0, 50, -15700, 1.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(33), 5, -150, 50, -16000, 2.2, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(34), 5, 150, 50, -16000, 2.2, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(35), 5, 0, 50, -16500, 2.0, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(36), 5, -150, 50, -17000, 2.2, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(37), 5, 150, 50, -17000, 2.2, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(38), 5, 0, 50, -17500, 2.2, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(39), 5, 150, 50, -18000, 2.2, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(40), 5, -150, 50, -18000, 2.2, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(41), 5, 0, 50, -18500, 2.6, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(42), 5, 0, 50, -19000, 2.6, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(43), 5, -150, 50, -21400, 2.6, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(44), 5, 150, 50, -21700, 2.6, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(45), 5, -150, 50, -22000, 2.6, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(46), 5, 150, 50, -22300, 2.6, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(47), 5, -150, 50, -22600, 2.6, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(48), 5, 150, 50, -22900, 2.6, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(49), 5, -150, 50, -23200, 2.6, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(50), 5, 150, 50, -23500, 2.6, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(51), 5, -150, 50, -23800, 2.6, 1.0, 1.0, 4);
		mSceneryObjectVector.push_back(*mSceneryObjectVectorAdder);

		// TODO Put animated enemies here

		// Ground floor
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(78), 6, 0, 0, -500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(79), 6, 0, 0, -5500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(80), 6, 0, 0, -10500, 1.0, 1.0, 1.0 , 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(81), 6, 0, 0, -15500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(82), 6, 0, 0, -20500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);
		mSceneryObjectVectorAdder = new SceneryObject(mSceneManagerPointer, "SceneryObject" + StringConverter::toString(83), 6, 0, 0, -25500, 1.0, 1.0, 1.0, 3);
		mGroundFloorVector.push_back(*mSceneryObjectVectorAdder);

		// SET ENEMIES
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(1), 1, Math::RangeRandom(-180, 180), -11000, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(2), 1, Math::RangeRandom(-180, 180), -11300, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(3), 1, Math::RangeRandom(-180, 180), -11600, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(4), 1, Math::RangeRandom(-180, 180), -11900, 1.0, 1.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);
		mSimpleEnemyVectorAdder = new SimpleEnemy(mSceneManagerPointer, "SimpleEnemy" + StringConverter::toString(5), 1, Math::RangeRandom(-180, 180), -12200, 1.0, 2.5);
		mSimpleEnemyVector.push_back(*mSimpleEnemyVectorAdder);

		// SET OBJECTS
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(1), 1, 0, -600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(2), 1, 0, -1600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(3), 1, 0, -2000);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(4), 1, 0, -2600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(5), 1, 0, -3600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(6), 5, 0, -4600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(7), 1, 0, -5600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(8), 1, 0, -6600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(9), 1, 0, -7600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(10), 1, 0, -8600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(11), 1, 0, -9600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(12), 1, 0, -10600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(13), 3, 0, -11600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(14), 1, 0, -12600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(15), 1, 0, -13600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(16), 2, 0, -14600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(17), 1, 0, -15600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(18), 1, 0, -16600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(19), 1, 0, -17600);
		mObjectVector.push_back(*mObjectVectorAdder);
		mObjectVectorAdder = new Object(mSceneManagerPointer, "Object" + StringConverter::toString(20), 1, 0, -18600);
		mObjectVector.push_back(*mObjectVectorAdder);

		break;

	default:
		break;

	}

	// Add every object to the scene
	for (mSceneryObjectIterator = mSceneryObjectVector.begin(); mSceneryObjectIterator != mSceneryObjectVector.end(); ++mSceneryObjectIterator) {

	     mSceneryObjectIterator->addToScene();

	}

	for (mSceneryObjectIterator = mGroundFloorVector.begin(); mSceneryObjectIterator != mGroundFloorVector.end(); ++mSceneryObjectIterator) {

	     mSceneryObjectIterator->addToScene();

	}

	for (mSimpleEnemyIterator = mSimpleEnemyVector.begin(); mSimpleEnemyIterator != mSimpleEnemyVector.end(); ++mSimpleEnemyIterator) {

		mSimpleEnemyIterator->addToScene();

	}

	for (mObjectIterator = mObjectVector.begin(); mObjectIterator != mObjectVector.end(); ++mObjectIterator) {

		mObjectIterator->addToScene();

	}

}
