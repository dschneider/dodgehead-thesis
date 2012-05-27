/**
 * Class Player - Header File
 *
 * Represents a player with all necessary functions
 * like decrease player lifes, energy...
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#ifndef __Player_h__
#define __Player_h__

#include <Ogre.h>

using namespace Ogre;

class Player {

public:
	Player(SceneManager *sceneManagerPointer, String playerName, int positionX);
	~Player();
	void addToScene();
	void move(Real time, Vector3 direction);
	Vector3 getPosition();
	void toggleColor();
	void toggleShadow();
	void toggleWireFrameMode();
	void activateShader(int shader);
	void incrementScore(int incscore);
	int getScore();
	void decreaseEnergy(int decenergy);
	int getEnergy();
	void setEnergy(int energy);
	int getLifes();
	void increaseSpeed(float speed);
	void decreaseSpeed(float speed);
	void increaseLifes();
	void decreaseLifes();
	void update();
	void setInvulnerable();
	void setVulnerable();
	float getSpeed();
	void setSpeed(float speed);
	void activateLight();
	void transformByMatrix(Matrix4 matrix);
	void activateTexture(int textureNumber, int subEntity = 4);
	void reset();
	void rotate(Real time);
	void setPosition(Vector3 position);
	void resetScale();
	SceneNode* getSceneNode();
	AxisAlignedBox getAABB();

private:
	SceneManager* mSceneManagerPointer;
	SceneNode* mNodePointer;
	Entity* mEntityPointer;
	SubEntity* mSubEntity;
	SceneNode *mLightPivot;
	String mName;
	int mX;
	int mEnergy;
	int mScore;
	int mLifes;
	float mSpeed;
	bool mIsShadowVisible;
	bool mIsWireFrameMode;
	bool mInvulnerable;
	Vector3 mTranslationVector;
	Vector3 mAABBScaleFactor;
	AxisAlignedBox mAABB;

}; // end of class Player

#endif // __Player_h__
