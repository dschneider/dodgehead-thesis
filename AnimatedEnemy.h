/**
 * Class AnimatedEnemy - Header File
 *
 * Represents an animated enemy which can be moved
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#ifndef ANIMATEDENEMY_H_
#define ANIMATEDENEMY_H_

#include <Ogre.h>

using namespace Ogre;

class AnimatedEnemy {

public:
	AnimatedEnemy(SceneManager *sceneManagerPointer, String enemyName, int meshNumber, int positionX, int positionZ, float scale, float speed);
	~AnimatedEnemy();
	void addToScene();
	void move(FrameEvent evt);
	Vector3 getPosition();
	void setInvisible();
	void setVisible();
	void setAnimationWalk();
	void setAnimationIdle();

	AnimationState *mAnimationState;


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

}; // end of class AnimatedEnemy

#endif /* ANIMATEDENEMY_H_ */
