/**
 * Class Object - Header File
 *
 * Represents an object that can be picked up
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <Ogre.h>

using namespace Ogre;

class Object {

public:
	Object(SceneManager *sceneManagerPointer, String objectName, int objectType, int positionX, int positionZ);
	~Object();
	void addToScene();
	void removeFromScene();
	Vector3 getPosition();
	void setInvisible();
	void setVisible();
	int getObjectType();
	AxisAlignedBox getAABB();

private:
	SceneManager* mSceneManagerPointer;
	SceneNode* mNodePointer;
	Entity* mEntityPointer;
	String mMeshType;
	String mName;
	String mMaterialName;
	int mObjectType;
	int mX;
	int mZ;
	AxisAlignedBox mAABB;

}; // end of class Object

#endif /* OBJECT_H_ */
