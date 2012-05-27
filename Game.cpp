/**
 * Class Game - Implementation File
 *
 * Handles the game logic, initializes the scene, menues
 * levels, adds players, handles the controls, the camera...
 * Here all the components come together
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#include "Game.h"
#include <iostream>
#include <sstream>

using namespace std;

/*
 * Constructor
 *
 * Initializes all the necessary attributes
 *
 */
Game::Game() {

	mInvulnerableTime = 1.0;
	mPickupTime = 0.3;
	mTranslationMatrixTime = 30.0;
	mGameOverTime = 5.0;
	mMove = 280;
	mCameraSpeed = 1.0;
    mLevelNumber = 0;
	PlayerTranslationVector = Vector3::ZERO;
	PlayerTranslationVector.y = 0;
	PlayerTranslationVector.x = 0;
	PlayerTranslationVector.z = -280;
	mCameraTranslationVector = Vector3::ZERO;
	mCameraTranslationVector.z = -mMove;
	mStartPause = true;
	mPickup = false;
	mEnemyAnimationStopped = false;
	mHit = false;
	mPickup = false;
	mGameMenu = false;
	mLevelStart = true;
	mInverseControl = false;
	mDeathChance = false;
	mFirstSpeed = false;
	mSecondSpeed = false;
	mThirdSpeed = false;
	mStartMenuClosed = false;
	mTranslationMatrixEvent = false;
	mGameOver = false;
	mDeathChanceScale = false;
	mPause = false;
	mInfoScreen = false;

	Settings *conf;
	GKeyFile *keyfile;
	GKeyFileFlags flags;
	GError *error = NULL;
	gsize length;

	// Read the key file settings.cfg
	// Create a new GKeyFile object and a bitwise list of flags
	keyfile = g_key_file_new ();
	flags = GKeyFileFlags(G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS);
	g_key_file_set_list_separator(keyfile, 'x');

	if (!g_key_file_load_from_file (keyfile, "resource/settings.cfg", flags, &error)) {
		std::ofstream config_file;
		config_file.open("resource/settings.cfg", std::ios::out);

		config_file << "# This is the Settings file for DodgeHead" << std::endl;
		config_file << "# It contains all necessary settings for the game" << std::endl;
		config_file << std::endl;
		config_file << "[Graphic]" << std::endl;
		config_file << "Resolution=1024x768" << std::endl;
		config_file << "Fullscreen=true" << std::endl;
		config_file << std::endl;
		config_file << "[Language]" << std::endl;
		config_file << "#english=en, german=de" << std::endl;
		config_file << "Language=en" << std::endl;
		config_file << std::endl;
		config_file << "[Gameplay]" << std::endl;
		config_file << "#Easy=0, Medium=1, Hard=2" << std::endl;
		config_file << "Difficulty=1" << std::endl;

		config_file.close();
		g_key_file_load_from_file (keyfile, "resource/settings.cfg", flags, &error);
	}

	conf = g_slice_new (Settings);

	conf->resolution = g_key_file_get_integer_list(keyfile, "Graphic", "Resolution", &length, NULL);
	conf->fullscreen = g_key_file_get_boolean(keyfile, "Graphic", "Fullscreen", NULL);
	conf->difficulty = g_key_file_get_integer(keyfile, "Gameplay", "Difficulty", NULL);
	conf->language = g_key_file_get_string(keyfile, "Language", "Language", NULL);

	sstr << conf->language;
	sstr >> mLanguage;
	mDifficulty = conf->difficulty;

	// Setup the info text which will be shown in the game
	if(mLanguage == "en") {

		mInfoText[0] = "Try to catch the spheres,\nthey increase your score!\nAnd dodge the blocks,\notherwise they'll hurt you.\nYou'll find more information\non that in the help menu by\npressing ESC -> Help Menu";
		mInfoText[1] = "In this level the \npolygon mode \nof the rasterising process\nis set to wireframe.\nYou can always find more \ndetails in the infoscreen \nmenu by pressing F1.";
		mInfoText[2] = "Watch out, in level 2\nthe speed is getting faster!";
		mInfoText[3] = "The polygon mode is\nnow solid and the objects\nhave color. But the\nshading is missing, because\nthere is only\nambient light set.";
		mInfoText[4] = "This level has textures,\nbut still no shading.";
		mInfoText[5] = "The blocks you are now \napproaching use \nGouraud shading.";
		mInfoText[6] = "There are differrent\nkinds of shading\nlike Flat, Phong\nand Blinn-Phong.";
		mInfoText[7] = "This level uses a\ndirectional light source,\nwhich is like the sun.\nYou can see that\neffect by looking at\nthe shadows. They are\nall pointing to the same\ndirection.";
		mInfoText[8] = "Watch out for narrow\nspots in this level.\nYou will need to\nscale your player object\nin order to avoid\nloosing energy.";
		mInfoText[9] = "Don't worry the\nlight flying around\nyour head. It's only\nfor showing the bump\nmapping effect.";

	} else if(mLanguage == "de") {

		// TODO Insert German translation
		mInfoText[0] = "Try to catch the spheres,\nthey increase your score!\nAnd dodge the blocks,\notherwise they'll hurt you.\nYou'll find more information\non that in the help menu by\npressing ESC -> Help Menu";
		mInfoText[1] = "In this level the \npolygon mode \nof the rasterising process\nis set to wireframe.\nYou can always find more \ndetails in the infoscreen \nmenu by pressing F1.";
		mInfoText[2] = "Watch out, in level 2\nthe speed is getting faster!";
		mInfoText[3] = "The polygon mode is\nnow solid and the objects\nhave color. But the\nshading is missing, because\nthere is only\nambient light set.";
		mInfoText[4] = "This level has textures,\nbut still no shading.";
		mInfoText[5] = "The blocks you are now \napproaching use \nGouraud shading.";
		mInfoText[6] = "There are differrent\nkinds of shading\nlike Flat, Phong\nand Blinn-Phong.";
		mInfoText[7] = "This level uses a\ndirectional light source,\nwhich is like the sun.\nYou can see that\neffect by looking at\nthe shadows. They are\nall pointing to the same\ndirection.";
		mInfoText[8] = "Watch out for narrow\nspots in this level.\nYou will need to\nscale your player object\nin order to avoid\nloosing energy.";
		mInfoText[9] = "Don't worry the\nlight flying around\nyour head. It's only\nfor showing the bump\nmapping effect.";

	}

}

/*
 * Destructor
 *
 * TODO Delete objects
 *
 */
Game::~Game()
{
}

/*
 * Creates the camera
 *
 */
void Game::createCamera() {

	// Create the camera
	mCamera = mSceneManager->createCamera("PlayerCam");

	// Look back along -Z
	mCamera->lookAt(Vector3(0, -270, -300)); //-100
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(1000);

	mCamera->setUseRenderingDistance(true);
	mCamera->setRenderingDistance(0.1);

	//Create a Node and attach camera on it
	mCamNode = mSceneManager->getRootSceneNode()->createChildSceneNode("mCam", Vector3(0, 350, 0));
	mCamNode->attachObject(mCamera);
	//mCamNode = mSceneManager->getRootSceneNode()->createChildSceneNode("CamNode2", Vector3(0, 200, 400));

}


/*
 * Checks user input (if key is pressed)
 *
 * @param arg		key event
 * @return bool		key has been pressed
 *
 */
bool Game::keyPressed(const OIS::KeyEvent &arg) {

	switch (arg.key) {

		// Activate in game menu (pause)
		case OIS::KC_ESCAPE:
			if(!mMenu->getPauseMenu()) {

				mMenu->showInGameMenu();
				mPause = true;
				mGameMenu = true;

			} else {

				mMenu->hideInGameMenu();
				mPause = false;
				mGameMenu = false;

			}
			break;

		// Show info screen
		case OIS::KC_F1:
			if(!mInfoScreen) {
				mMenu->showInfoScreen(mLevelNumber);
				mPause = true;
				mInfoScreen = true;
				mGameOverlay->hideOverlay("Score");
				mGameOverlay->hideOverlay("Energy");
				mGameOverlay->hideOverlay("Lifes");
				mGameOverlay->hideOverlay("Vector");
				mGameOverlay->hideOverlay("Infobox");
			} else {
				mMenu->hideInfoScreen(mLevelNumber);
				mPause = false;
				mInfoScreen = false;
				mGameOverlay->showOverlay("Score");
				mGameOverlay->showOverlay("Energy");
				mGameOverlay->showOverlay("Lifes");
				mGameOverlay->showOverlay("Vector");
				mGameOverlay->showOverlay("Infobox");
			}
			break;

		// Show world in wireframe mode
		case OIS::KC_F2:
			mCamera->setPolygonMode(PM_WIREFRAME);
			break;

		// Show world in solid mode
		case OIS::KC_F3:
			mCamera->setPolygonMode(PM_SOLID);
			break;

		// Activate texture menu
		case OIS::KC_F4:
			mMenu->showTextureMenu();
			break;

		// Move player to the left
		case OIS::KC_LEFT:
			if(!mInverseControl) {
				PlayerTranslationVector.x = -mMove;
			} else {
				PlayerTranslationVector.x = mMove;
			}
			break;

		// Move player to the right
		case OIS::KC_RIGHT:
			if(!mInverseControl) {
				PlayerTranslationVector.x = mMove;
			} else {
				PlayerTranslationVector.x = -mMove;
			}
			break;

		// Pause the game
		case OIS::KC_P:
			if(!mPause) {
				mPause = true;
				mGameOverlay->showOverlay("Pause");
			} else {
				mPause = false;
				mGameOverlay->hideOverlay("Pause");
			}
			break;

		default:
			break;

	}

	// Forward event to CEGUI
    CEGUI::System::getSingleton().injectKeyDown(arg.key);
    CEGUI::System::getSingleton().injectChar(arg.text);

	return true;

}

/*
 * Checks for released keys
 *
 * @param arg		key event
 * @return bool		key has been released
 *
 */
bool Game::keyReleased(const OIS::KeyEvent &arg) {

	switch (arg.key) {

		case OIS::KC_LEFT:
			PlayerTranslationVector.x = 0;
			break;

		case OIS::KC_RIGHT:
			PlayerTranslationVector.x = 0;
			break;

		default:
			break;

	}

	// Forward event to CEGUI
	CEGUI::System::getSingleton().injectKeyUp(arg.key);

	return true;

}

/*
 * Creates the scene (player, levels, camera animation)
 *
 */
void Game::createScene() {

	// Game overlay
	mGameOverlay = new GameOverlay();

	// Show logo
	mGameOverlay->showOverlay("Logo");

	// Create the level object for loading levels
	mLevel = new Level(mSceneManager, mWindow, mCamera);

	// Create sound object and load sound files
	mSound = new Sound();
	mSound->loadSounds();

	// Play background music
	mSound->playSound(1, true);

	// Add players to the scene
	mPlayer1 = new Player(mSceneManager, "Player1", 0);
	mPlayer1->addToScene();

	// Create the menu
	mMenu = new Menu(mSceneManager, mWindow, mPlayer1, mLanguage, mGameOverlay);

	// Set compositor effects
	CompositorManager::getSingleton().addCompositor(mCamera->getViewport(), "Bloom");
	CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "Bloom", false);
	CompositorManager::getSingleton().addCompositor(mCamera->getViewport(), "Glass");
	CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "Glass", false);
	CompositorManager::getSingleton().addCompositor(mCamera->getViewport(), "Radial Blur");
	CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "Radial Blur", false);
	CompositorManager::getSingleton().addCompositor(mCamera->getViewport(), "B&W");
	CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "B&W", false);
	CompositorManager::getSingleton().addCompositor(mCamera->getViewport(), "ASCII");
	CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "ASCII", false);

	// Load the main menu
	mLevel->loadLevel(0);

    // Set up spline animation of node
	mMainMenuCameraAnimation = mSceneManager->createAnimation("MainMenuCameraTrack", 20);

    // Spline it for nice curves
	mMainMenuCameraAnimation->setInterpolationMode(Animation::IM_SPLINE);

    // Create a track to animate the camera's node
	mMainMenuCameraTrack = mMainMenuCameraAnimation->createNodeTrack(0, mCamNode);

    // Setup keyframes
	mMainMenuCameraKey = mMainMenuCameraTrack->createNodeKeyFrame(0); // startposition
	mMainMenuCameraKey = mMainMenuCameraTrack->createNodeKeyFrame(2.5);
	mMainMenuCameraKey->setTranslate(Vector3(100, 30, 500));
	mMainMenuCameraKey = mMainMenuCameraTrack->createNodeKeyFrame(5);
	mMainMenuCameraKey->setTranslate(Vector3(-300, 30, 300));
	mMainMenuCameraKey = mMainMenuCameraTrack->createNodeKeyFrame(7.5);
	mMainMenuCameraKey->setTranslate(Vector3(0, 30, 100));
	mMainMenuCameraKey = mMainMenuCameraTrack->createNodeKeyFrame(10);
	mMainMenuCameraKey->setTranslate(Vector3(0, 30, -200));
	mMainMenuCameraKey = mMainMenuCameraTrack->createNodeKeyFrame(12.5);
	mMainMenuCameraKey->setTranslate(Vector3(500, 30, -300));
	mMainMenuCameraKey = mMainMenuCameraTrack->createNodeKeyFrame(15);
	mMainMenuCameraKey->setTranslate(Vector3(1000, 30, -300));
	mMainMenuCameraKey = mMainMenuCameraTrack->createNodeKeyFrame(17.5);
	mMainMenuCameraKey->setTranslate(Vector3(500, 30, 0));
	mMainMenuCameraKey = mMainMenuCameraTrack->createNodeKeyFrame(20);
	mMainMenuCameraKey->setTranslate(Vector3(0, 30, 0));

    // Create a new animation state to track this
	mMainMenuCameraAnimState = mSceneManager->createAnimationState("MainMenuCameraTrack");
	mMainMenuCameraAnimState->setEnabled(true);
	mMainMenuCameraAnimState->setLoop(true);
    mCamera->setAutoTracking(true,mPlayer1->getSceneNode());


    // Set up spline animation of node
	mLevelBeginCameraAnimation = mSceneManager->createAnimation("CameraTrack", 10);

    // Spline it for nice curves
	mLevelBeginCameraAnimation->setInterpolationMode(Animation::IM_SPLINE);

    // Create a track to animate the camera's node
	mLevelBeginCameraTrack = mLevelBeginCameraAnimation->createNodeTrack(0, mCamNode);

    // Setup keyframes
	mLevelBeginCameraKey = mLevelBeginCameraTrack->createNodeKeyFrame(0); // startposition
	mLevelBeginCameraKey = mLevelBeginCameraTrack->createNodeKeyFrame(2.5);
	mLevelBeginCameraKey->setTranslate(Vector3(100, 350, -350));
	mLevelBeginCameraKey = mLevelBeginCameraTrack->createNodeKeyFrame(5);
	mLevelBeginCameraKey->setTranslate(Vector3(-100, 30, -350));
	mLevelBeginCameraKey = mLevelBeginCameraTrack->createNodeKeyFrame(7.5);
	mLevelBeginCameraKey->setTranslate(Vector3(0, 350, 0));
	mLevelBeginCameraKey = mLevelBeginCameraTrack->createNodeKeyFrame(10);
    mLevelBeginCameraKey->setTranslate(Vector3(0, 350, 0));

    // Create a new animation state to track this
    mLevelBeginCameraAnimState = mSceneManager->createAnimationState("CameraTrack");
    mLevelBeginCameraAnimState->setEnabled(true);
    mLevelBeginCameraAnimState->setLoop(false);
    mCamera->setAutoTracking(true, mPlayer1->getSceneNode());

}

/*
 * Unlock a level
 *
 * @param level		the level to unlock
 *
 */

void Game::unlockLevel(String level) {

	fstream file;

	file.open("resource/levels.dat", ios::out);

	for(int i = 0; i < level.length(); i++)
	{
		file.write((char*)&level[i], sizeof(char));
	}

	file.close();

}

/*
 * Reload the game
 *
 * Resets all the variables to begin a new game
 * for example when loading the main menu and
 * hides all overlays
 *
 */

void Game::reloadGame() {

	mPlayer1->reset();
	mLevel->unloadLevel();
	mLevelNumber = 0;
	mMenu->showMainMenu();
	mMenu->setLevelNumber(0);
	mPause = false;
	mMenu->setMainMenuClicked(true);
	mLevelBeginCameraAnimState->setEnabled(false);
	mGameOverlay->hideOverlay("Energy");
	mGameOverlay->hideOverlay("Lifes");
	mGameOverlay->hideOverlay("Vector");
	mGameOverlay->hideOverlay("Score");
	mGameOverlay->hideOverlay("Infobox");
	mGameOverlay->hideOverlay("GameOver");
	mGameOverlay->hideOverlay("Hurt");
	mMainMenuCameraAnimState->setEnabled(true);
	mGameOverTime = 5.0;
	mGameOver = false;
	mStartPause = true;
	mStartMenuClosed = false;
	mLevelStart = true;
	mLevelBeginCameraAnimState->setTimePosition(0.0);
	mLevelBeginCameraAnimState->setEnabled(true);
	mCameraSpeed = 1.0;
	mFirstSpeed = false;
	mSecondSpeed = false;
	mThirdSpeed = false;
	mHit = false;
	mPickup = false;
	mGameMenu = false;
	mLevelStart = true;
	mInverseControl = false;
	mDeathChance = false;
	mTranslationMatrixEvent = false;
	mGameOver = false;
	mInvulnerableTime = 1.0;
	mPickupTime = 1.0;
	mTranslationMatrixTime = 30.0;
	mGameOverlay->updateVector("Position: " + StringConverter::toString(mPlayer1->getPosition()));
	mGameOverlay->updateEnergy("Player 1 Energy: " + StringConverter::toString(mPlayer1->getEnergy()));
	mGameOverlay->updateLifes("Player 1 Lifes: " + StringConverter::toString(mPlayer1->getLifes()));
	mGameOverlay->updateScore("Player 1 Score: " + StringConverter::toString(mPlayer1->getScore()));
	mGameOverlay->hideOverlay("Levelname");
	mGameOverlay->hideOverlay("Time");
	mLevel->loadLevel(0);
	mMainMenuCameraAnimState->setEnabled(true);
	mMainMenuCameraAnimState->setLoop(true);
    mCamera->setAutoTracking(true, mPlayer1->getSceneNode());
	CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "Radial Blur", false);
	CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "Bloom", false);
	CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "B&W", false);
	CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "ASCII", false);
	mGameOverlay->showOverlay("Logo");

}

/*
 * This method is called at the beginning of each frame
 *
 * @param evt	a Frame Event
 * @return		frame has been started
 *
 */
bool Game::frameStarted(const FrameEvent& evt) {

	// When the main menu button in the in game menu has been clicked
	// reload the game
	if(mMenu->getMainMenuClicked()) {

		this->reloadGame();

	}

	// When clicking the continue button of the in game menu, then hide it and
	// unpause the game
	if(mMenu->getContinueClicked()) {

		mPause = false;
		mMenu->hideInGameMenu();
		mMenu->setContinueClicked(false);
		mGameMenu = false;

	}

	// Animate the main menu camera, if the main menu is activated
	if(mMenu->getMainMenu()) {

		mMainMenuCameraAnimState->addTime(evt.timeSinceLastFrame);

	}

	// If the player nearly dies, give him another chance to survive by typing in a translation matrix
	if(mTranslationMatrixEvent) {

		// Don't show that in the first level, because wireframe mode can't handle shaders
		if(mLevelNumber != 1) {
			CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "ASCII", true);
		}

		// Check if the apply button has been clicked
		if(mMenu->getMatrixMenuClicked()) {

			// Set the time to 1 second
			mTranslationMatrixTime = 1.0;
			mMenu->setMatrixMenuClicked(false);

		}

		// Decrease the time and update the time overlay
		mTranslationMatrixTime -= evt.timeSinceLastFrame;
		mGameOverlay->updateTime("Time " + StringConverter::toString(mTranslationMatrixTime));

		// Check if the time is already up
		if(mTranslationMatrixTime <= 0) {

			// Transform the player by the entered matrix and hide the menu as well as
			// the time overlay and deactivate the shader
			mPlayer1->transformByMatrix(mMenu->getMatrixTranslation());
			mPause = false;
			mMenu->hideMatrixMenu();
			mGameOverlay->hideOverlay("Time");
			mTranslationMatrixEvent = false;
			CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "ASCII", false);

		}

	}

	// If the game is over, wait a few seconds then reload the game
	if(mGameOver) {

		mGameOverTime -= evt.timeSinceLastFrame;
		CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "B&W", true);

		if(mGameOverTime <= 0) {

			this->reloadGame();

		}

	}

	// If the game is not paused and not over, check whether the game is in progress and
	// move the player, handle game logic etc.
	if(!mPause && !mGameOver) {

		// If the main menu has been closed (e. g. started a new game)
		if(!mMenu->getMainMenu()) {

			// If a new game has been started
			if(!mStartMenuClosed) {

				mLevel->unloadLevel();
				mLevelNumber = mMenu->getLevelNumber();
				mStartMenuClosed = true;
				mMainMenuCameraAnimState->setEnabled(false);
				mGameOverlay->hideOverlay("Logo");

			}

			// Wait a few seconds at the beginning and count down the time
			if(mStartPause) {

				mGameOverlay->showOverlay("Levelname");
				mLevelBeginCameraAnimState->addTime(evt.timeSinceLastFrame);

				if(mLevelBeginCameraAnimState->hasEnded()) {
					mStartPause = false;
					mLevelBeginCameraAnimState->setEnabled(false);
					mCamera->setAutoTracking(false, mPlayer1->getSceneNode());
					mCamera->lookAt(Vector3(0, -270, -700));
					mGameOverlay->hideOverlay("Levelname");
				}

			}

			// Start to move the player and the camera
			if(!mStartPause && !mPause) {
				mPlayer1->move(evt.timeSinceLastFrame, PlayerTranslationVector);
				mCamNode->translate(mCameraTranslationVector * evt.timeSinceLastFrame * mCameraSpeed);
			}

			// Update the overlay for the player position
			mGameOverlay->updateVector("Position " + StringConverter::toString(mPlayer1->getPosition()));

			// Level 1 specific things
			if(mLevelNumber == 1) {

				if(mLevelStart) {

					// Load the first level
					mLevel->loadLevel(1);
					mGameOverlay->updateLevelName(mLevel->getLevelName());
					mPlayer1->toggleWireFrameMode();

					// The level generation has been finished, start phase is over
					mLevelStart = false;

				}

				// Set info boxes for the level
				if(mPlayer1->getPosition().z <= -1 && mPlayer1->getPosition().z >= -1100) {
				
					mGameOverlay->updateInfobox(mInfoText[0]);
				
				} else if(mPlayer1->getPosition().z <= -5000 && mPlayer1->getPosition().z >= -5100) {
				
					mGameOverlay->updateInfobox(mInfoText[1]);

				}

			}

			// Level 2 specific things
			if(mLevelNumber == 2) {

				if(mLevelStart) {

					// Load the second level
					mLevel->loadLevel(2);
					mGameOverlay->updateLevelName(mLevel->getLevelName());
					mPlayer1->toggleColor();
					// The level generation has been finished, start phase is over
					mLevelStart = false;

				}

				// Set info boxes for the level
				if(mPlayer1->getPosition().z <= -1 && mPlayer1->getPosition().z >= -1100) {
				
					mGameOverlay->updateInfobox(mInfoText[2]);
				
				} else if(mPlayer1->getPosition().z <= -5000 && mPlayer1->getPosition().z >= -5100) {
				
					mGameOverlay->updateInfobox(mInfoText[3]);

				}

			}

			// Level 3 specific things
			if(mLevelNumber == 3) {

				if(mLevelStart) {

					// Load the second level
					mLevel->loadLevel(3);
					mPlayer1->activateTexture(1);
					mPlayer1->activateTexture(2, 3);
					mGameOverlay->updateLevelName(mLevel->getLevelName());
					// The level generation has been finished, start phase is over
					mLevelStart = false;

				}

				// Set info boxes for the level
				if(mPlayer1->getPosition().z <= -1 && mPlayer1->getPosition().z >= -1100) {
				
					mGameOverlay->updateInfobox(mInfoText[4]);
				
				} else if(mPlayer1->getPosition().z <= -5000 && mPlayer1->getPosition().z >= -5100) {
				
					mGameOverlay->updateInfobox(mInfoText[4]);

				}

			}


			// Level 4 specific things
			if(mLevelNumber == 4) {

				if(mLevelStart) {

					// Load the second level
					mLevel->loadLevel(4);
					mPlayer1->activateShader(1);
					mGameOverlay->updateLevelName(mLevel->getLevelName());
					// The level generation has been finished, start phase is over
					mLevelStart = false;

				}

				// Set info boxes for the level
				if(mPlayer1->getPosition().z <= -1 && mPlayer1->getPosition().z >= -1100) {
				
					mGameOverlay->updateInfobox(mInfoText[5]);
				
				} else if(mPlayer1->getPosition().z <= -5000 && mPlayer1->getPosition().z >= -5100) {
				
					mGameOverlay->updateInfobox(mInfoText[6]);

				} else if(mPlayer1->getPosition().z <= -9000 && mPlayer1->getPosition().z >= -9100) {
				
					mGameOverlay->updateInfobox(mInfoText[7]);

				}

			}

			// Level 5 specific things
			if(mLevelNumber == 5) {

				if(mLevelStart) {

					// Load the second level
					mLevel->loadLevel(5);
					mPlayer1->activateLight();
					mPlayer1->activateShader(1);
					mGameOverlay->updateLevelName(mLevel->getLevelName());
					// The level generation has been finished, start phase is over
					mLevelStart = false;

				}

				// Set info boxes for the level
				if(mPlayer1->getPosition().z <= -1 && mPlayer1->getPosition().z >= -1100) {
				
					mGameOverlay->updateInfobox(mInfoText[8]);
				
				} else if(mPlayer1->getPosition().z <= -5000 && mPlayer1->getPosition().z >= -5100) {
				
					mGameOverlay->updateInfobox(mInfoText[9]);

				} else if(mPlayer1->getPosition().z <= -9000 && mPlayer1->getPosition().z >= -9100) {
				
					mGameOverlay->updateInfobox(mInfoText[9]);

				}

			}


			// If the player has been hit make him invulnerable for a few seconds
			if(mHit) {

				// Count down the time the player is invulnerable
				mInvulnerableTime -= evt.timeSinceLastFrame;

				// If the time is over, hide hurt overlay, set player invulnerable etc.
				if(mInvulnerableTime <= 0) {

					mInvulnerableTime = 1.0;
					mPlayer1->setVulnerable();
					mHit = false;
					mGameOverlay->hideOverlay("Hurt");

				}


			}

			// Move the enemies
			mLevel->moveEnemies(evt);

			// Check the collision between scenery objects and the player
			if(mLevel->checkSceneryCollision(mPlayer1)) {

				// Decrease the player energy by 30
				mPlayer1->decreaseEnergy(30);

				// Update the energy overlay
				mGameOverlay->updateEnergy("Player 1 Energy: " + StringConverter::toString(mPlayer1->getEnergy()));

				// If energy is below or equal zero...
				if(mPlayer1->getEnergy() <= 0) {

					// ...remove a life of the player, set the energy to full and update the life overlay
					mPlayer1->decreaseLifes();
					mGameOverlay->updateLifes("Player 1 Lifes: " + StringConverter::toString(mPlayer1->getLifes()));
					mPlayer1->setEnergy(100);

				}

				// If no life is left, end the game
				if(mPlayer1->getLifes() < 0) {

					// Give the player one last chance to survive
					if(!mDeathChance) {

						mDeathChance = true;
						mPause = true;
						mTranslationMatrixEvent = true;
						mMenu->showMatrixMenu();
						mGameOverlay->showOverlay("Time");

					} else {

						mGameOver = true;
						mGameOverlay->showOverlay("GameOver");

					}

				}

				// The player has been hit and is now invulnerable for a few seconds
				mHit = true;

				// Play the hit sound
				mSound->playSound(2, false);

				// Show the hurt overlay (red quads)
				mGameOverlay->showOverlay("Hurt");

				// Set the player invulnerable
				mPlayer1->setInvulnerable();

			}

			// Check the collision between simple enemies (like moving blocks) and player
			if(mLevel->checkSimpleEnemyCollision(mPlayer1)) {

				mPlayer1->decreaseEnergy(30);

				mGameOverlay->updateEnergy("Player 1 Energy: " + StringConverter::toString(mPlayer1->getEnergy()));

				if(mPlayer1->getEnergy() <= 0) {

					// remove a life of the player and set the energy to full
					mPlayer1->decreaseLifes();
					mGameOverlay->updateLifes("Player 1 Lifes: " + StringConverter::toString(mPlayer1->getLifes()));
					mPlayer1->setEnergy(100);

				}

				// If no life is left...
				if(mPlayer1->getLifes() < 0) {

					// ... first give the player another chance
					if(!mDeathChance) {

						mDeathChance = true;
						mPause = true;
						mTranslationMatrixEvent = true;
						mMenu->showMatrixMenu();
						mGameOverlay->showOverlay("Time");

					// ... else end the game
					} else {

						mGameOver = true;
						mGameOverlay->showOverlay("GameOver");

					}

				}

				// The player has been hit and is now invulnerable for a few seconds
				mHit = true;

				// Play the hit sound
				mSound->playSound(2, false);

				// Show the hurt overlay (red quads)
				mGameOverlay->showOverlay("Hurt");

				// Set the player invulnerable
				mPlayer1->setInvulnerable();

			}

			// Matrix transformation point (scaling)
			if((mPlayer1->getPosition().z <= mLevel->getDeathChancePoint(1) && mPlayer1->getPosition().z >= mLevel->getDeathChancePoint(1) - 100) && !mDeathChanceScale) {

				mDeathChanceScale = true;
				mPause = true;
				mTranslationMatrixEvent = true;
				mMenu->showMatrixMenu();
				mGameOverlay->showOverlay("Time");

			} else if(mPlayer1->getPosition().z <= (mLevel->getDeathChancePoint(1) - 1000)) {
			
				//mDeathChanceScale = false;
				mPlayer1->resetScale();
			
			}

			// If object has been picked up, wait a few seconds
			// before another object can be picked up.
			// Without this, the score would increase too much
			// because every frame the player collides with the
			// object the score is increased
			if(mPickup) {

				mPickupTime -= evt.timeSinceLastFrame;

				if(mPickupTime <= 0) {

					mPickupTime = 0.3;
					mPickup = false;

				}

			}

			if(!mPickup) {

				// Check for collision with pickup objects
				if(mLevel->checkObjectCollision(mPlayer1, mInverseControl)) {

					// Play pickup sound
					mSound->playSound(0, false);

					// Update the score overlay of player 1
					mGameOverlay->updateScore("Player 1 Score: " + StringConverter::toString(mPlayer1->getScore()));

					// Object has been picked up, wait a few seconds for the points
					mPickup = true;

				}

			}

			// Position the player in the middle at the end of the level
			if(mPlayer1->getPosition().z <= mLevel->getLevelLength() + 1000) {

				mGameOverlay->showOverlay("Time");
				mGameOverlay->updateTime("Level Clear!");
				mPlayer1->setSpeed(1.0);

				// Move the player to the middle
				if(mPlayer1->getPosition().x < 0) {

					PlayerTranslationVector.x = 280;

				} else if(mPlayer1->getPosition().x > 0) {

					PlayerTranslationVector.x = -280;

				} else {

					PlayerTranslationVector.x = 0;

				}

			} else if(mPlayer1->getPosition().z <= mLevel->getLevelLength() + 700) {

				PlayerTranslationVector.x = 0;

			}

			// The level end is reached, reset the settings
			if(mPlayer1->getPosition().z <= mLevel->getLevelLength()) {

				// Stop moving the player and the camera
				PlayerTranslationVector.z = 0;
				PlayerTranslationVector.y = 150;
				PlayerTranslationVector.x = 0;
				PlayerTranslationVector.z = -280;
				PlayerTranslationVector.y = 0;
				mCameraTranslationVector.z = -280;
				mCameraSpeed = 1.0;

				// Set the camera back
				mPlayer1->setPosition(Vector3(0, 15, -250));
				mCamNode->setPosition(0, 350, 0);

				// Reset speed points and level variables
				mFirstSpeed = false;
				mSecondSpeed = false;
				mThirdSpeed = false;
				mLevelStart = true;
				mStartPause = true;
				mInverseControl = false;
				CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "Radial Blur", false);

				// Start the camera animation at the beginning
				mLevelBeginCameraAnimState->setEnabled(true);
				mLevelBeginCameraAnimState->setTimePosition(0.0);

				// Unload the level
				mLevel->unloadLevel();

				// Unlock the next level
				if(mLevelNumber == 1) {

					mLevelNumber = 2;
					this->unlockLevel("2");

				} else if(mLevelNumber == 2) {

					mLevelNumber = 3;
					this->unlockLevel("3");

				} else if(mLevelNumber == 3) {

					mLevelNumber = 4;
					this->unlockLevel("4");

				} else if(mLevelNumber == 4) {

					mLevelNumber = 5;
					this->unlockLevel("5");

				} else if(mLevelNumber == 5) {

					// If level number 5 has been finished
					// reload the whole game
					this->reloadGame();

				}

				mGameOverlay->hideOverlay("Time");

			}

			// Activate the camere tracking before the level end, reset the camera speed
			// and stop the camera
			if(mPlayer1->getPosition().z <= (mLevel->getLevelLength() + 600)) {

				mCamera->setAutoTracking(true, mPlayer1->getSceneNode());
				mCameraTranslationVector.z = 0;
				mCameraSpeed = 1.0;

			}


			// Check for the difficulty and activate the points, where the speed of the game gets faster
			if(mDifficulty > 0) {

				if(mPlayer1->getPosition().z <= mLevel->getSpeedPoint(1) && mPlayer1->getPosition().z >= (mLevel->getSpeedPoint(1) - 500) && !mFirstSpeed) {
					mFirstSpeed = true;
					mPlayer1->increaseSpeed(0.5);
					mCameraSpeed += 0.5;
				}

			} else if(mDifficulty >= 1) {

				if(mPlayer1->getPosition().z <= mLevel->getSpeedPoint(2) && mPlayer1->getPosition().z >= (mLevel->getSpeedPoint(2) - 500) && !mSecondSpeed) {
					mSecondSpeed = true;
					mPlayer1->increaseSpeed(0.5);
					mCameraSpeed += 0.5;
				}

			} else if(mDifficulty >= 2) {

				if(mPlayer1->getPosition().z <= mLevel->getSpeedPoint(3) && mPlayer1->getPosition().z >= (mLevel->getSpeedPoint(3) - 500) && !mThirdSpeed) {
					mThirdSpeed = true;
					mPlayer1->increaseSpeed(0.5);
					mCameraSpeed += 0.5;
				}

			}


		}

	}

	return OgreApplication::frameStarted(evt);

}
