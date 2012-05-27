/**
 * Class AnimatedEnemy - Implementation
 *
 * Represents an animated enemy which can be moved
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#include "AnimatedEnemy.h"


/*
 * Constructor
 * Initializes all the necessary attributes for enemy object
 *
 * @param Pointer		a pointer to the scenemanager
 * @param enemyName		the name of the animated enemy (entity and scene node)
 * @param positionX		the x-axis position of the animated enemy
 * @param positionZ		the z-axis position of the animated enemy
 * @param scale			the scale factor
 * @param speed			the speed of the enemy
 *
 */
AnimatedEnemy::AnimatedEnemy(SceneManager* Pointer, String enemyName, int meshNumber, int positionX, int positionZ, float scale, float speed) {

	mSceneManagerPointer = Pointer;
	mName = enemyName;
	mX = positionX;
	mZ = positionZ;

	switch(meshNumber) {

	case 1:
		mMeshType = "robot.mesh";
		break;

	case 2:
		mMeshType = "ninja.mesh";
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
AnimatedEnemy::~AnimatedEnemy() {

}

/*
 * Adds an animated enemy entity and a scene node to the scene
 *
 */
void AnimatedEnemy::addToScene() {

	mEntityPointer = mSceneManagerPointer->createEntity(mName, mMeshType);
	mEntityPointer->setCastShadows(true);
	mEntityPointer->setVisible(false);

	mNodePointer = mSceneManagerPointer->getRootSceneNode()->createChildSceneNode(mName);
	mNodePointer->attachObject(mEntityPointer);
	mNodePointer->setScale(mScaleSize, mScaleSize, mScaleSize);
	mNodePointer->setPosition(mX, 0, mZ);

    mAnimationState = mEntityPointer->getAnimationState("Walk");
    mAnimationState->setLoop(true);
    mAnimationState->setEnabled(true);

}

/*
 * Moves the animated enemy to the given direction
 *
 * @param evt		ogre frame event
 *
 */
void AnimatedEnemy::move(FrameEvent evt) {

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
 * Set the animation of the animated enemy to "walking"
 *
 */
void AnimatedEnemy::setAnimationWalk() {

	mAnimationState = mEntityPointer->getAnimationState("Walk");

}

/*
 * Set the animation of the animated enemy to "idle"
 *
 */
void AnimatedEnemy::setAnimationIdle() {

	mAnimationState = mEntityPointer->getAnimationState("Idle");

}

/*
 * Get the position of the animated enemy
 *
 * @return	the position of the animated enemy
 *
 */
Vector3 AnimatedEnemy::getPosition() {

	return mNodePointer->getPosition();

}

/*
 * Set the animated enemy invisible
 *
 */
void AnimatedEnemy::setInvisible() {

	mEntityPointer->setVisible(false);

}

/*
 * Set the animated enemy visible
 *
 */
void AnimatedEnemy::setVisible() {

	mEntityPointer->setVisible(true);

}
