/**
 * Class SimpleEnemy - Implementation File
 *
 * Represents a simple enemy which is able to move
 * within the world
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#include "SimpleEnemy.h"


/*
 * Constructor
 * Initializes all the necessary attributes for enemy object
 *
 * @param Pointer		a pointer to the scenemanager
 * @param enemyName		the name of the player (entity)
 * @param positionX		the x-axis position of the enemy
 * @param positionY		the y-axis position of the player
 * @param positionZ		the z-axis position of the player
 * @param scale			the scale factor for x, y and z axis
 * @param speed			the speed of the enemy
 *
 */
SimpleEnemy::SimpleEnemy(SceneManager* Pointer, String enemyName, int meshNumber, int positionX, int positionZ, float scale, float speed) {

	mSceneManagerPointer = Pointer;
	mName = enemyName;
	mX = positionX;
	mZ = positionZ;

	switch(meshNumber) {

	case 1:
		mMeshType = "cube.mesh";
		break;

	}

    mSpeed = speed;
    mDirection = Vector3::ZERO;
    mDirection.x = -100;
	mScaleSize = scale;

}

/*
 * Destructor
 *
 */
SimpleEnemy::~SimpleEnemy() {

}

/*
 * Adds a enemy entity and a scene node to the scene
 *
 */
void SimpleEnemy::addToScene() {

	mEntityPointer = mSceneManagerPointer->createEntity("Entity" + mName, mMeshType);
	mEntityPointer->setCastShadows(true);
	//mEntityPointer->setVisible(false);
	//entityPointer->setNormaliseNormals(true); // ÜBERALL EINFÜGEN

	mNodePointer = mSceneManagerPointer->getRootSceneNode()->createChildSceneNode("SceneNode" + mName);
	mEntityPointer->setMaterialName("Examples/TextureEffect3");
	mNodePointer->attachObject(mEntityPointer);
	mNodePointer->setScale(mScaleSize, mScaleSize, mScaleSize);
	mNodePointer->setPosition(mX, 50, mZ);

	mAABB = mEntityPointer->getBoundingBox();
	mAABB.scale(Vector3(mScaleSize, mScaleSize, mScaleSize));

}

/*
 * Remove the enemy from the scene
 *
 */
void SimpleEnemy::removeFromScene() {

	if (mSceneManagerPointer->hasSceneNode("SceneNode" + mName) && mSceneManagerPointer->hasEntity("Entity" + mName)) {

	    mNodePointer->detachObject("Entity" + mName);
		mSceneManagerPointer->destroyEntity("Entity" + mName);
		mSceneManagerPointer->getRootSceneNode()->removeAndDestroyChild("SceneNode" + mName);

	}

}

/*
 * Moves the enemy
 *
 */
void SimpleEnemy::move(FrameEvent evt) {

    mNodePointer->translate(mDirection * evt.timeSinceLastFrame * mSpeed);
    if(mNodePointer->getPosition().x >= 200) {
    	mNodePointer->yaw(Degree(-180));
    	mDirection = mDirection * (-1);
    } else if(mNodePointer->getPosition().x <= -200) {
    	mNodePointer->yaw(Degree(180));
    	mDirection = mDirection * (-1);
    }

}

/*
 * Get the Axis Aligned Bounding Box
 *
 * @return		the axis aligned bounding box of the object (AABB)
 *
 */
AxisAlignedBox SimpleEnemy::getAABB() {

	return mAABB;

}

/*
 * Returns the position of the enemy
 *
 * @return	the position of the enemy
 *
 */
Vector3 SimpleEnemy::getPosition() {

	return mNodePointer->getPosition();

}

/*
 * Set the enemy invisible
 *
 */
void SimpleEnemy::setInvisible() {

	mEntityPointer->setVisible(false);

}

/*
 * Set the enemy visible
 *
 */
void SimpleEnemy::setVisible() {

	mEntityPointer->setVisible(true);

}

/*
 * Set the color of the enemy
 *
 */
void SimpleEnemy::setColor() {

	//mEntityPointer->setMaterialName("MyApplication/Red");

}

/*
 * Set the speed of the enemy
 *
 * @param speed		speed value
 *
 */
void SimpleEnemy::setSpeed(float speed) {

	mSpeed = speed;

}
