/**
 * Class Player.cpp - Implementation
 *
 * Represents a player with all necessary functions
 * like decrease player lifes, energy...
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#include "Player.h"

#include <iostream>
using namespace std;

/*
 * Constructor
 * Initializes all the necessary attributes for player object
 *
 * @param Pointer		a pointer to the scenemanager
 * @param playerName	the name of the player (entity)
 * @param positionX		the x-axis position of the player
 *
 */
Player::Player(SceneManager* Pointer, String playerName, int positionX) {

	mSceneManagerPointer = Pointer;
	mName = playerName;
	mX = positionX;
	mAABBScaleFactor = 0.0;
	mTranslationVector = Vector3::ZERO;
	mIsShadowVisible = true;
	mIsWireFrameMode = true;
	mScore = 0;
	mEnergy = 100;
	mLifes = 2;
	mSpeed = 1.0;

}

/*
 * Destructor
 *
 */
Player::~Player() {

}

SceneNode* Player::getSceneNode() {

	return mNodePointer;

}

/*
 * Adds a player entity and a scene node to the scene
 *
 */
void Player::addToScene() {

	// THIS IS NEEDED FOR BUMP MAPPING

	MeshPtr pMesh = MeshManager::getSingleton().load("ogrehead.mesh",
	                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	//so we can still read it
	// Build tangent vectors, all our meshes use only 1 texture coordset
	// Note we can build into VES_TANGENT now (SM2+)
	unsigned short src, dest;
	if (!pMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
	{
		pMesh->buildTangentVectors(VES_TANGENT, src, dest);
		// Second mode cleans mirrored / rotated UVs but requires quality models
		//pMesh->buildTangentVectors(VES_TANGENT, src, dest, true, true);
	}

	mEntityPointer = mSceneManagerPointer->createEntity(mName, "ogrehead.mesh");
	mEntityPointer->setMaterialName("Examples/RockySphereWireFrame");
	mEntityPointer->setCastShadows(true);

	mNodePointer = mSceneManagerPointer->getRootSceneNode()->createChildSceneNode(mName);
	mNodePointer->attachObject(mEntityPointer);
	mNodePointer->setScale(0.9, 0.9, 0.9);
	mNodePointer->setPosition(mX, 15, -250);
	mNodePointer->rotate(Vector3(0, 1, 0), Degree(180));

	// Show the bounding box
	//mNodePointer->showBoundingBox(true);

	// Set the bounding box for collision
	mAABB = mEntityPointer->getBoundingBox();
	mAABB.scale(Vector3(0.9, 0.9, 0.9));

}

/*
 * Moves the player to the given direction
 *
 * @param time			the current game time
 * @param direction		the direction of movement
 *
 */
void Player::move(Real time, Vector3 direction) {

	// Move the player
	mNodePointer->translate(direction * time * mSpeed);

	// If the player is too far on the right...
	if(mNodePointer->getPosition().x >= 200) {

		// ... set him back to 200 on the x-axis
		mNodePointer->setPosition(200, 15, mNodePointer->getPosition().z);

	// If the player is too far on the left...
	} else if(mNodePointer->getPosition().x <= -200) {

		// ... set him back to -200 on the x-axis
		mNodePointer->setPosition(-200, 15, mNodePointer->getPosition().z);

	}

	// If the player object has already a light rotate it
	if(mSceneManagerPointer->hasSceneNode("LightPivot")) {

		mLightPivot->rotate(mNodePointer->getPosition().UNIT_Y, Degree(1.0), Node::TS_PARENT);

	}

}

/*
 * Get the Axis Aligned Bounding Box
 *
 * @return		the axis aligned bounding box of the player object (AABB)
 *
 */
AxisAlignedBox Player::getAABB() {

	return mAABB;

}

/*
 * Returns the position of the player
 *
 * @return	the position of the player
 *
 */
Vector3 Player::getPosition() {

	return mNodePointer->getPosition();

}

/*
 * Set player color
 *
 */
void Player::toggleColor() {

	mEntityPointer->setMaterialName("DodgeHead/Gray");

}

/*
 * Toggle the visibility of the player shadow
 *
 */
void Player::toggleShadow() {

	if(mIsShadowVisible) {

		mIsShadowVisible = false;
		mEntityPointer->setCastShadows(false);

	} else {

		mIsShadowVisible = true;
		mEntityPointer->setCastShadows(true);

	}

}

/*
 * Toggle the visibility of the wireframe model
 *
 */
void Player::toggleWireFrameMode() {

	if(mIsWireFrameMode) {

		mIsWireFrameMode = false;
		mEntityPointer->setMaterialName("Examples/Rocky");

	} else {

		mIsWireFrameMode = true;
		mEntityPointer->setMaterialName("Examples/RockySphereWireFrame");

	}

}

/*
 * Activates the texture
 *
 * @param textureNumber		the number of the texture to apply
 * @param subEntity			the sub entity number
 *
 */
void Player::activateTexture(int textureNumber, int subEntity) {

	switch(subEntity) {

	// Body
	case 0:
		mSubEntity = mEntityPointer->getSubEntity(1);
		if(textureNumber == 0) {

			mSubEntity->setMaterialName("Ogre/Earring");

		} else if(textureNumber == 1) {

			mSubEntity->setMaterialName("Ogre/Tusks");

		} else if(textureNumber == 2) {

			mSubEntity->setMaterialName("Ogre/Eyes");

		} else if(textureNumber == 3) {

			mSubEntity->setMaterialName("Ogre/Skin");

		} else if(textureNumber == 3) {

			mSubEntity->setMaterialName("DodgeHead/Rocky");

		}
		break;

	// Tusks
	case 1:
		mSubEntity = mEntityPointer->getSubEntity(3);
		if(textureNumber == 0) {

			mSubEntity->setMaterialName("Ogre/Earring");

		} else if(textureNumber == 1) {

			mSubEntity->setMaterialName("Ogre/Tusks");

		} else if(textureNumber == 2) {

			mSubEntity->setMaterialName("Ogre/Eyes");

		} else if(textureNumber == 3) {

			mSubEntity->setMaterialName("Ogre/Skin");

		} else if(textureNumber == 3) {

			mSubEntity->setMaterialName("DodgeHead/Rocky");

		}
		break;

	// Earring
	case 2:
		mSubEntity = mEntityPointer->getSubEntity(2);
		if(textureNumber == 0) {

			mSubEntity->setMaterialName("Ogre/Earring");

		} else if(textureNumber == 1) {

			mSubEntity->setMaterialName("Ogre/Tusks");

		} else if(textureNumber == 2) {

			mSubEntity->setMaterialName("Ogre/Eyes");

		} else if(textureNumber == 3) {

			mSubEntity->setMaterialName("Ogre/Skin");

		} else if(textureNumber == 3) {

			mSubEntity->setMaterialName("DodgeHead/Rocky");

		}
		break;

		// Eyes
		case 3:
			mSubEntity = mEntityPointer->getSubEntity(0);
			if(textureNumber == 0) {

				mSubEntity->setMaterialName("Ogre/Earring");

			} else if(textureNumber == 1) {

				mSubEntity->setMaterialName("Ogre/Tusks");

			} else if(textureNumber == 2) {

				mSubEntity->setMaterialName("Ogre/Eyes");

			} else if(textureNumber == 3) {

				mSubEntity->setMaterialName("Ogre/Skin");

			} else if(textureNumber == 3) {

				mSubEntity->setMaterialName("DodgeHead/Rocky");

			}
			break;

		case 4:
			mEntityPointer->setMaterialName("DodgeHead/Rocky");
			break;

	default:
		break;


	}

}

/*
 * Activates a shader material
 *
 * @param shader	number of the shader
 *
 */
void Player::activateShader(int shader) {

	switch(shader) {

		// Bump Mapping
		case 1:
			mEntityPointer->setMaterialName("Examples/BumpMapping/MultiLightSpecular");
			break;

		case 2:
			mEntityPointer->setMaterialName("Examples/OffsetMapping/Specular");
			break;

	}

}

/*
 * Increment the score
 *
 * @param incscore	the score, that should be added
 *
 */
void Player::incrementScore(int incscore) {

	mScore += incscore;

}

/*
 * Return the current score
 *
 * @return	the current player score
 *
 */
int Player::getScore() {

	return mScore;

}

/*
 * Decrement the energy
 *
 * @param decenergy		the energy, that should be decreased
 *
 */
void Player::decreaseEnergy(int decenergy) {

	if(!mInvulnerable) {

		mEnergy -= decenergy;

	}

}


/*
 * Increase the player speed
 *
 * @param speed		the amount of speed, that should be added
 *
 */
void Player::increaseSpeed(float speed) {

	mSpeed += speed;

}

/*
 * Decrease the player speed
 *
 * @param speed		the amount of speed, that should be removed
 *
 */
void Player::decreaseSpeed(float speed) {

	mSpeed -= speed;

}

/*
 * Decrease the player lifes by 1
 *
 *
 */
void Player::increaseLifes() {

	mLifes++;

}

/*
 * Decrease the player lifes by 1
 *
 *
 */
void Player::decreaseLifes() {

	mLifes--;

}

/*
 * Return the number of current player lifes
 *
 *
 */
int Player::getLifes() {

	return mLifes;

}

/*
 * Set the energy to the given amount
 *
 * @param energy 	value
 *
 */
void Player::setEnergy(int energy) {

	mEnergy = energy;

}

/*
 * Return the current amount of energy
 *
 * @return	the current amount of energy
 *
 */
int Player::getEnergy() {

	return mEnergy;

}

/*
 * Set the player invulnerable
 *
 *
 */
void Player::setInvulnerable() {

	mInvulnerable = true;

}

/*
 * Set the player vulnerable
 *
 *
 */

void Player::setVulnerable() {

	mInvulnerable = false;

}


/*
 * Get the current player speed
 *
 * @return	the current player speed
 *
 */
float Player::getSpeed() {

	return mSpeed;

}

/*
 * Set the player speed
 *
 * @param speed		player speed amount
 *
 */
void Player::setSpeed(float speed) {

	mSpeed = speed;

}

/*
 * Activate the light around the player
 *
 *
 */
void Player::activateLight() {

	if(!mSceneManagerPointer->hasSceneNode("LightPivot")) {

		mLightPivot = mNodePointer->createChildSceneNode("LightPivot");
		mLightPivot->setPosition(mNodePointer->getPosition().x, 40, mNodePointer->getPosition().z);

		// Create a light, use default parameters
		Light *mLight = mSceneManagerPointer->createLight("Light1");
		mLight->setPosition(mNodePointer->getPosition().x, 40, mNodePointer->getPosition().z);
		mLight->setDiffuseColour(1.0, 0.0, 0.0);
		mLight->setSpecularColour(1.0, 0.0, 0.0);
		mLight->setVisible(true);

		// Attach light
		mLightPivot->attachObject(mLight);

		// Create billboard for light
		BillboardSet* mLightFlareSet = mSceneManagerPointer->createBillboardSet("Flare");
		mLightFlareSet->setMaterialName("Examples/Flare");
		mLightPivot->attachObject(mLightFlareSet);
		Billboard* mLightFlare = mLightFlareSet->createBillboard(mNodePointer->getPosition().x, 40, mNodePointer->getPosition().z);
		mLightFlare->setColour(ColourValue(1.0, 0.0, 0.0));
		mLightFlareSet->setVisible(true);

	} else {
	
		mLightPivot->setVisible(true);
	
	}

}

/*
 * Translate the player by a matrix
 *
 * @param matrix		the transformation matrix
 *
 */
void Player::transformByMatrix(Matrix4 matrix) {

	// Different approach
	//mMatrix1 = mNodePointer->_getFullTransform();
	//mMatrix2 = matrix.concatenateAffine(mMatrix1);

	mAABBScaleFactor = Vector3(matrix[0][0], matrix[1][1], matrix[2][2]);

	mNodePointer->translate(matrix.getTrans());
	mNodePointer->scale(Vector3(matrix[0][0], matrix[1][1], matrix[2][2]));
	mAABB.scale(mAABBScaleFactor);

}

/*
 * Reset the player
 *
 *
 */

void Player::reset() {

	mNodePointer->setPosition(0, 15, -250);
	mNodePointer->setScale(0.9, 0.9, 0.9);
	mEnergy = 100;
	mSpeed = 1.0;
	mLifes = 2;
	mScore = 0;

	if(mSceneManagerPointer->hasSceneNode("LightPivot")) {

		mLightPivot->setVisible(false);

	}

}

/*
 * Reset the player
 *
 * @param position		the position of the player
 *
 */

void Player::setPosition(Vector3 position) {

	mNodePointer->setPosition(position);

}

/*
 * Rotate the player
 *
 * @param time		the time of the framelistener to move the player correctly
 *
 */

void Player::rotate(Real time) {

	mNodePointer->yaw(Degree(720) * mSpeed * time);

}

/*
 * Scale the player
 *
 *
 */

void Player::resetScale() {

	mNodePointer->setScale(Vector3(0.9, 0.9, 0.9));

	// Recalculate the original scale ... no setScale function for AABB :/
	mAABBScaleFactor = Vector3(1.0 / mAABBScaleFactor.x, 1.0 / mAABBScaleFactor.y, 1.0 / mAABBScaleFactor.z);

	mAABB.scale(mAABBScaleFactor);

}
