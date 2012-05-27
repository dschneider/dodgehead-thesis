/**
 * Class SceneryObject - Implementation File
 *
 * Represents a scenery object like trees, blocks,
 * houses and other not movable things
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#include "SceneryObject.h"
#include <iostream>

using namespace std;


/*
 * Constructor
 * Initializes all the necessary attributes for player object
 *
 * @param Pointer		a pointer to the scenemanager
 * @param objectName	the name of the player (entity)
 * @param positionX		the x-axis position of the player
 * @param positionY		the y-axis position of the player
 * @param positionZ		the z-axis position of the player
 * @param scaleX		scale factor for the x-axis
 * @param scaleY		scale factor for the y-axis
 * @param scaleZ		scale factor for the z-axis
 * @param materialType  the type of the material
 *
 */
SceneryObject::SceneryObject(SceneManager* Pointer, String objectName, int objectType, int positionX, int positionY, int positionZ, float scaleX, float scaleY, float scaleZ, int materialType) {

	mSceneManagerPointer = Pointer;
	mName = objectName;
	mObjectType = objectType;
	mX = positionX;
	mY = positionY;
	mZ = positionZ;
	mScaleX = scaleX;
	mScaleY = scaleY;
	mScaleZ = scaleZ;

	switch(objectType) {

	case 1:
		mMeshType = "column.mesh";
		break;

	case 2:
		mMeshType = "Barrel.mesh";
		break;

	case 3:
		mMeshType = "athene.mesh";
		break;

	case 4:
		mMeshType = "tudorhouse.mesh";
		break;

	case 5:
		mMeshType = "cube.mesh";
		if(materialType == 4) {

			// Gouraud shaded - Colour at each vertex is linearly interpolated across the face.
			mMaterialName = "DodgeHead/Rocky";

		} else if(materialType == 3) {

			// Flat shaded - No interpolation takes place. Each face
			// is shaded with a single colour determined from the first vertex in the face. Pretty ugly =)
			mMaterialName = "DodgeHead/RockyFlat";

		} else if(materialType == 2) {

			mMaterialName = "";

		} else if(materialType == 1) {

			mMaterialName = "DodgeHead/White";

		}else {

			mMaterialName = "DodgeHead/Black";

		}

		break;

	case 6:
		Plane mPlane(Vector3::UNIT_Y, 0);

		// Register a ground mesh
		MeshManager::getSingleton().createPlane("ground",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mPlane,
		5000, 5000, 20, 20, true, 1, 5, 5, Vector3::UNIT_Z);

		if(materialType == 3) {
			mMaterialName = "DodgeHead/GrassFloor";
		} else if(materialType == 2) {
			mMaterialName = "DodgeHead/DirtFloor";
		} else {
			mMaterialName = "DodgeHead/Blue";
		}

		mMeshType = "ground";
		break;

	}

}

/*
 * Destructor
 *
 */
SceneryObject::~SceneryObject() {

}

/*
 * Adds an object entity and a scene node to the scene
 *
 */
void SceneryObject::addToScene() {

	mEntityPointer = mSceneManagerPointer->createEntity("Entity" + mName, mMeshType);
	mEntityPointer->setCastShadows(true);
	if(mMaterialName != "") {
		mEntityPointer->setMaterialName(mMaterialName);
	}

	mNodePointer = mSceneManagerPointer->getRootSceneNode()->createChildSceneNode("SceneNode" + mName);
	mNodePointer->attachObject(mEntityPointer);
	mNodePointer->setScale(mScaleX, mScaleY, mScaleZ);
	mNodePointer->setPosition(mX, mY, mZ);

	mAABB = mEntityPointer->getBoundingBox();
	mAABB.scale(Vector3(mScaleX, mScaleY, mScaleZ));

	//mNodePointer->showBoundingBox(true);

}

/*
 * Removes the object from the scene
 *
 */
void SceneryObject::removeFromScene() {

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
AxisAlignedBox SceneryObject::getAABB() {

	return mAABB;

}

/*
 * Returns the position of the object
 *
 * @return	the position of the object
 *
 */
Vector3 SceneryObject::getPosition() {

	return mNodePointer->getPosition();

}

/*
 * Set the object invisible
 *
 *
 */
void SceneryObject::setInvisible() {

	mNodePointer->setVisible(false);

}

/*
 * Set the object visible
 *
 *
 */
void SceneryObject::setVisible() {

	mNodePointer->setVisible(true);

}

/*
 * Get the object type
 *
 * @return		object type
 *
 */
int SceneryObject::getObjectType() {

	return mObjectType;

}
