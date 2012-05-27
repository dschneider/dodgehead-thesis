/**
 * Class Game - Header File
 *
 * Handles the game logic, initializes the scene, menues
 * levels, adds players...
 * Here all of the components come together
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#ifndef __GAME_H__
#define __GAME_H__

#include "OgreApplication.h"
#include "GameOverlay.h"
#include "Menu.h"
#include "Player.h"
#include "Sound.h"
#include "Level.h"
#include "SceneryObject.h"
#include "SimpleEnemy.h"
#include "AnimatedEnemy.h"
#include "SceneryObject.h"
#include "Object.h"
#include "glib.h"


class Game : public OgreApplication {

public:
   Game(void);
   virtual ~Game();

protected:
   virtual void createScene();
   virtual void createCamera();
   virtual bool frameStarted(const FrameEvent& evt);
   virtual bool keyPressed(const OIS::KeyEvent &arg);
   virtual bool keyReleased(const OIS::KeyEvent &arg);
   virtual void reloadGame();
   virtual void unlockLevel(String level);

   //bool saveScore();

   int mLevelNumber;
   bool mStartPause;
   bool mPause;
   bool mEnemyAnimationStopped;
   bool mGameMenu;
   bool mHit;
   bool mPickup;
   bool mLevelStart;
   bool mInverseControl;
   bool mDeathChance;
   bool mDeathChanceScale;
   bool mFirstSpeed;
   bool mSecondSpeed;
   bool mThirdSpeed;
   bool mStartMenuClosed;
   bool mTranslationMatrixEvent;
   bool mGameOver;
   bool mInfoScreen;
   Real mStartTime;
   Real mInvulnerableTime;
   Real mPickupTime;
   Real mGameOverTime;
   Real mMove;
   Real mTranslationMatrixTime;
   float mCameraSpeed;
   int mChance;
   int mDifficulty;

   String mInfoText[10];

   SceneNode *mCamNode;
   Vector3 mCameraTranslationVector;
   Vector3 PlayerTranslationVector;

   Player *mPlayer1;
   Player *mPlayer2;

   AnimationState* mLevelBeginCameraAnimState;
   Animation* mLevelBeginCameraAnimation;
   NodeAnimationTrack* mLevelBeginCameraTrack;
   TransformKeyFrame* mLevelBeginCameraKey;

   AnimationState* mMainMenuCameraAnimState;
   Animation* mMainMenuCameraAnimation;
   NodeAnimationTrack* mMainMenuCameraTrack;
   TransformKeyFrame* mMainMenuCameraKey;

   Level *mLevel;
   GameOverlay* mGameOverlay;
   Menu *mMenu;
   Sound* mSound;

   stringstream sstr;
   String mLanguage;

   typedef struct {
	gint *resolution;
	gboolean fullscreen;
	gint difficulty;
	gchar* language;
   } Settings;

}; // end of class Game

#endif // #ifndef ___GAME_H___
