/**
 * Class SceneryObject - Header File
 *
 * Represents a scenery object like trees, blocks,
 * houses and other not movable things
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#ifndef SCENERYOBJECT_H_
#define SCENERYOBJECT_H_

#include <Ogre.h>

using namespace Ogre;

class SceneryObject {

public:
	SceneryObject(SceneManager* Pointer, String objectName, int objectType, int positionX, int positionY, int positionZ, float scaleX, float scaleY, float scaleZ, int materialType = 0);
	virtual ~SceneryObject();
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
	int mY;
	int mZ;
	float mScaleX;
	float mScaleY;
	float mScaleZ;
	AxisAlignedBox mAABB;

};

#endif /* SCENERYOBJECT_H_ */
