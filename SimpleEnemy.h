/**
 * Class SimpleEnemy - Header File
 *
 * Represents a simple enemy which is able to move
 * within the world
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#ifndef SIMPLEENEMY_H_
#define SIMPLEENEMY_H_

#include <Ogre.h>

using namespace Ogre;

class SimpleEnemy {

public:
	SimpleEnemy(SceneManager *sceneManagerPointer, String enemyName, int meshNumber, int positionX, int positionZ, float scale, float speed);
	~SimpleEnemy();
	void addToScene();
	void removeFromScene();
	void move(FrameEvent evt);
	Vector3 getPosition();
	void setInvisible();
	void setVisible();
	void setColor();
	void setSpeed(float speed);
	AxisAlignedBox getAABB();

private:
	SceneManager* mSceneManagerPointer;
	SceneNode* mNodePointer;
	Entity* mEntityPointer;
	String mName;
	String mMeshType;
    float mSpeed;
    Vector3 mDirection;
	int mX;
	int mZ;
	float mScaleSize;
	AxisAlignedBox mAABB;


}; // end of class SimpleEnemy

#endif /* SIMPLEENEMY_H_ */
