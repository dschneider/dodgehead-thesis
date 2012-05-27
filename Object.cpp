/**
 * Class Object - Implementation File
 *
 * Represents an object that can be picked up
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#include "Object.h"


/*
 * Constructor
 *
 * Initializes all the necessary attributes for player object
 *
 * @param Pointer		a pointer to the scenemanager
 * @param objectName	the name of the player (entity)
 * @param positionX		the x-axis position of the player
 * @param positionY		the y-axis position of the player
 * @param positionZ		the z-axis position of the player
 *
 */
Object::Object(SceneManager* Pointer, String objectName, int objectType, int positionX, int positionZ) {

	mSceneManagerPointer = Pointer;
	mName = objectName;
	mObjectType = objectType;
	mX = positionX;
	mZ = positionZ;

	mMaterialName = "";

	switch(objectType) {

	case 1:
		mMeshType = "sphere.mesh";
		mMaterialName = "DodgeHead/White";
		break;

	case 2:
		mMeshType = "sphere.mesh";
		mMaterialName = "DodgeHead/Black";
		break;

	case 3:
		mMeshType = "sphere.mesh";
		mMaterialName = "DodgeHead/Black";
		break;

	case 4:
		mMeshType = "sphere.mesh";
		mMaterialName = "DodgeHead/White";
		break;

	case 5:
		mMeshType = "sphere.mesh";
		mMaterialName = "DodgeHead/White";
		break;

	case 6:
		mMeshType = "sphere.mesh";
		mMaterialName = "DodgeHead/White";
		break;

	}

}

/*
 * Destructor
 *
 */
Object::~Object() {

}

/*
 * Adds a object entity and a scene node to the scene
 *
 */
void Object::addToScene() {

	mEntityPointer = mSceneManagerPointer->createEntity("Entity" + mName, mMeshType);
	mEntityPointer->setCastShadows(true);
	mEntityPointer->setMaterialName(mMaterialName);

	mNodePointer = mSceneManagerPointer->getRootSceneNode()->createChildSceneNode("SceneNode" + mName);
	mNodePointer->attachObject(mEntityPointer);
	mNodePointer->setScale(0.2, 0.2, 0.2);
	mNodePointer->setPosition(mX, 15, mZ);

	// Set the bounding box for collision
	mAABB = mEntityPointer->getBoundingBox();
	mAABB.scale(Vector3(0.2, 0.2, 0.2));

	//mNodePointer->showBoundingBox(true);

}

void Object::removeFromScene() {

	if (mSceneManagerPointer->hasSceneNode("SceneNode" + mName) && mSceneManagerPointer->hasEntity("Entity" + mName)) {

	    mNodePointer->detachObject("Entity" + mName);
		mSceneManagerPointer->destroyEntity("Entity" + mName);
		mSceneManagerPointer->getRootSceneNode()->removeAndDestroyChild("SceneNode" + mName);

	}

}

/*
 * Get the Axis Aligned Bounding Box
 *
 * @return		the axis aligned bounding box of the object (AABB)
 *
 */
AxisAlignedBox Object::getAABB() {

	return mAABB;

}

/*
 * Returns the position of the object
 *
 * @return	the position of the object
 *
 */
Vector3 Object::getPosition() {

	return mNodePointer->getPosition();

}

/*
 * Set the object invisible
 *
 */
void Object::setInvisible() {

	mEntityPointer->setVisible(false);

}

/*
 * Set the object visible
 *
 */
void Object::setVisible() {

	mEntityPointer->setVisible(true);

}

/*
 * Get the object type
 *
 * @return		object type
 *
 */
int Object::getObjectType() {

	return mObjectType;

}
