/**
 * Class Menu - Implementation File
 *
 * Handles all the menus
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#include "Menu.h"

#include <iostream>
#include <sstream>

using namespace std;

/*
 * Constructor
 *
 * Initialize the object
 *
 * @param sceneManager		a pointer to the ogre scene manager
 * @param window			a pointer to the ogre render window
 * @param player			a pointer to the player object
 * @param language			language from the configuration file
 * @param gameOverlay		a pointer to the gameoverlay object
 *
 */
Menu::Menu(SceneManager* sceneManager, RenderWindow* window, Player* player, String language, GameOverlay* gameOverlay) {

	mMainMenu = true;
	mMainMenuClicked = false;
	mContinueClicked = false;
	mMatrixMenuClicked = false;

	mPlayerPointer = player;
	mGameOverlay = gameOverlay;

	mLanguage = language;

    mRead = "";
    mActivatedLevel = 0;
    mPauseMenu = false;
    mLevelNumber = 0;

	// 4x4 matrix
	mMatrix[0][0] = 0;
	mMatrix[0][1] = 0;
	mMatrix[0][2] = 0;
	mMatrix[0][3] = 0;

	mMatrix[1][0] = 0;
	mMatrix[1][1] = 0;
	mMatrix[1][2] = 0;
	mMatrix[1][3] = 0;

	mMatrix[2][0] = 0;
	mMatrix[2][1] = 0;
	mMatrix[2][2] = 0;
	mMatrix[2][3] = 0;

	mMatrix[3][0] = 0;
	mMatrix[3][1] = 0;
	mMatrix[3][2] = 0;
	mMatrix[3][3] = 0;


	// With a scene manager and window, we can create a the GUI renderer
	CEGUI::OgreCEGUIRenderer* pGUIRenderer = new CEGUI::OgreCEGUIRenderer(
	window,					// The render window created earlier; CEGUI renders to this
	Ogre::RENDER_QUEUE_OVERLAY,		// CEGUI should render in this render queue
	false,					// Put everything in the above render queue first, not last
	3000,					// This is actually unnecessary now in CEGUI -- max quads for the UI
	sceneManager				// Use this scene manager to manage the UI
	);

	mPauseMenu = false;

	// Read the levels.dat file and look which levels have been
	// unlocked
	char cstring[1000];

	mFile.open("resource/levels.dat", ios::in);

	while (!mFile.eof())
	{
		mFile.getline(cstring, sizeof(cstring));
        mRead += cstring;
	}

	mFile.close();

	istringstream iss(mRead);
	iss >> mActivatedLevel;

	// Create the root CEGUI class
	pSystem = new CEGUI::System(pGUIRenderer);

	// Tell us a lot about what is going on (see CEGUI.log in the working directory)
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

	// Use this CEGUI scheme definition (see CEGUI docs for more)
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");

	// Show the CEGUI mouse cursor (defined in the look-n-feel)
	pSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");

	// Use this font for text in the UI
	pSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");

	// Set strings for language dependent layout files
	String menu[15];
	menu[0] = "dodgeheadingamemenu_" + mLanguage + ".layout";
	menu[1] = "dodgeheadmainmenu_" + mLanguage + ".layout";
	menu[2] = "dodgeheadcreditsmenu_" + mLanguage + ".layout";
	menu[3] = "dodgeheadoptionsmenu_" + mLanguage + ".layout";
	menu[4] = "dodgeheadstartgamemenu_" + mLanguage + ".layout";
	menu[5] = "dodgeheadlevelmenu_" + mLanguage + ".layout";
	menu[6] = "dodgeheadstory1_" + mLanguage + ".layout";
	menu[7] = "dodgeheadinfoscreen1_" + mLanguage + ".layout";
	menu[8] = "dodgeheadinfoscreen2_" + mLanguage + ".layout";
	menu[9] = "dodgeheadinfoscreen3_" + mLanguage + ".layout";
	menu[10] = "dodgeheadinfoscreen4_" + mLanguage + ".layout";
	menu[11] = "dodgeheadinfoscreen5_" + mLanguage + ".layout";
	menu[12] = "dodgeheadmatrixmenu_" + mLanguage + ".layout";
	menu[13] = "dodgeheadhelpmenu_" + mLanguage + ".layout";
	menu[14] = "dodgeheadtexturemenu_" + mLanguage + ".layout";

	// Load language dependent layout files
	mInGameMenuLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[0].c_str());
	mMainMenuLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[1].c_str());
	mCreditsMenuLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[2].c_str());
	mOptionsMenuLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[3].c_str());
	mStartGameMenuLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[4].c_str());
	mLevelMenuLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[5].c_str());
	mStoryLayout1 = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[6].c_str());
	mInfoLayout1 = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[7].c_str());
	mInfoLayout2 = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[8].c_str());
	mInfoLayout3 = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[9].c_str());
	mInfoLayout4 = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[10].c_str());
	mInfoLayout5 = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[11].c_str());
	mMatrixMenuTranslateLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[12].c_str());
	mHelpMenuLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[13].c_str());
	mTextureMenuLayout = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)menu[14].c_str());

	// You need to tell CEGUI which layout to display. You can call this at any time to change the layout to
	// another loaded layout (i.e. moving from screen to screen or to load your HUD layout). Note that this takes
	// a CEGUI::Window instance -- you can use anything (any widget) that serves as a root window.
	pSystem->setGUISheet(mMainMenuLayout);

	CEGUI::ListboxTextItem *item;

	// Create a new combobox for difficulty
	CEGUI::Combobox *Difficulty = (CEGUI::Combobox *)CEGUI::WindowManager::getSingleton().getWindow("OptionsMenu/DifficultyBox");
	if(mLanguage == "en") {

		item = new CEGUI::ListboxTextItem("Easy", 0);
		Difficulty->addItem(item);
		item = new CEGUI::ListboxTextItem("Medium", 1);
		Difficulty->addItem(item);
		item = new CEGUI::ListboxTextItem("Hard", 2);
		Difficulty->addItem(item);

	} else if(mLanguage == "de") {

		item = new CEGUI::ListboxTextItem("Leicht", 0);
		Difficulty->addItem(item);
		item = new CEGUI::ListboxTextItem("Mittel", 1);
		Difficulty->addItem(item);
		item = new CEGUI::ListboxTextItem("Schwer", 2);
		Difficulty->addItem(item);

	}
	Difficulty->setReadOnly(true);

	// Create a new combobox for the resolution
	CEGUI::Combobox *Resolution = (CEGUI::Combobox *)CEGUI::WindowManager::getSingleton().getWindow("OptionsMenu/ResolutionBox");
	//Resolution->subscribeEvent(CEGUI::Combobox::EventMouseEnters, CEGUI::Event::Subscriber(&MenuDlg::Mouse_Over, this));
	item = new CEGUI::ListboxTextItem("800x600", 0);
	Resolution->addItem(item);
	item = new CEGUI::ListboxTextItem("1024x768", 1);
	Resolution->addItem(item);
	item = new CEGUI::ListboxTextItem("1280x720", 2);
	Resolution->addItem(item);
	item = new CEGUI::ListboxTextItem("1280x800", 3);
	Resolution->addItem(item);
	item = new CEGUI::ListboxTextItem("1280x1024", 4);
	Resolution->addItem(item);
	item = new CEGUI::ListboxTextItem("1440x900", 5);
	Resolution->addItem(item);
	item = new CEGUI::ListboxTextItem("1600x1200", 6);
	Resolution->addItem(item);
	Resolution->setReadOnly(true);

	// Create the combobox for the entity
	TextureEntity = (CEGUI::Combobox *)CEGUI::WindowManager::getSingleton().getWindow("TextureMenu/ComboBoxEntity");
	TextureEntity->subscribeEvent(CEGUI::Combobox::EventMouseClick, CEGUI::Event::Subscriber(&Menu::TextureMenu_OnClick, this));
	item = new CEGUI::ListboxTextItem("Player", 0);
	TextureEntity->addItem(item);
	/*item = new CEGUI::ListboxTextItem("PickupObject Good", 1);
	TextureEntity->addItem(item);
	item = new CEGUI::ListboxTextItem("PickupObject Bad", 2);
	TextureEntity->addItem(item);
	item = new CEGUI::ListboxTextItem("SceneryObject Block", 3);
	TextureEntity->addItem(item);*/
	TextureEntity->setReadOnly(true);

	// Create the combobox for subentity
	TextureSubEntity = (CEGUI::Combobox *)CEGUI::WindowManager::getSingleton().getWindow("TextureMenu/ComboBoxSubEntity");
	TextureSubEntity->subscribeEvent(CEGUI::Combobox::EventMouseClick, CEGUI::Event::Subscriber(&Menu::TextureMenu_OnClick, this));
	item = new CEGUI::ListboxTextItem("Body", 0);
	TextureSubEntity->addItem(item);
	item = new CEGUI::ListboxTextItem("Tusks", 1);
	TextureSubEntity->addItem(item);
	item = new CEGUI::ListboxTextItem("Earring", 2);
	TextureSubEntity->addItem(item);
	item = new CEGUI::ListboxTextItem("Eyes", 3);
	TextureSubEntity->addItem(item);
	TextureSubEntity->setReadOnly(true);
	TextureSubEntity->disable();

	// Create a combobox for the textures
	Textures = (CEGUI::Combobox *)CEGUI::WindowManager::getSingleton().getWindow("TextureMenu/ComboBoxTextures");
	Textures->subscribeEvent(CEGUI::Combobox::EventMouseClick, CEGUI::Event::Subscriber(&Menu::TextureMenu_OnClick, this));
	item = new CEGUI::ListboxTextItem("Earring", 0);
	Textures->addItem(item);
	item = new CEGUI::ListboxTextItem("Tusks", 1);
	Textures->addItem(item);
	item = new CEGUI::ListboxTextItem("Eyes", 2);
	Textures->addItem(item);
	item = new CEGUI::ListboxTextItem("Body", 3);
	Textures->addItem(item);
	/*item = new CEGUI::ListboxTextItem("Rock", 4);
	Textures->addItem(item);
	item = new CEGUI::ListboxTextItem("Dirt", 5);
	Textures->addItem(item);
	item = new CEGUI::ListboxTextItem("Water", 6);
	Textures->addItem(item);*/
	Textures->setReadOnly(true);

	// Show the main menu and the cursor
	mMainMenuLayout->show();
	CEGUI::MouseCursor::getSingleton().show();

	// Subscribe callback functions
	CEGUI::Window *windowElement;
	windowElement = CEGUI::WindowManager::getSingleton().getWindow("MainMenu/Exit");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::Quit_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InGameMenu/MainMenu");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::Quit_OnMainMenu, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("MainMenu/StartNewGame");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::Quit_OnNewGame, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("OptionsMenu/Back");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::Quit_OnMainMenu, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("StartGameMenu/OnePlayer");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::Quit_OnePlayer, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("MainMenu/Options");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::OpenOptions_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("MainMenu/Credits");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::OpenCredits_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("OptionsMenu/Back");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::ExitCredits_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("StartGameMenu/Back");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::ExitCredits_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("LevelMenu/Back");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::ExitLevelMenu_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("LevelMenu/Level1");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::LoadLevel1_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("LevelMenu/Level2");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::LoadLevel2_OnClick, this));
	if(mActivatedLevel < 2) {
		windowElement->setMutedState(true);
		windowElement->disable();
	}

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("LevelMenu/Level3");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::LoadLevel3_OnClick, this));
	if(mActivatedLevel < 3) {
		windowElement->setMutedState(true);
		windowElement->disable();
	}

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("LevelMenu/Level4");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::LoadLevel4_OnClick, this));
	if(mActivatedLevel < 4) {
		windowElement->setMutedState(true);
		windowElement->disable();
	}

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("LevelMenu/Level5");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::LoadLevel5_OnClick, this));
	if(mActivatedLevel < 5) {
		windowElement->setMutedState(true);
		windowElement->disable();
	}

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("StoryScreen1/Continue");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::Continue_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("CreditsMenu/Back");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::ExitCredits_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InGameMenu/Help");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::HelpMenu_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("HelpMenu/Back");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::HelpMenuBack_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/Apply");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::ContinueMatrixMenu_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InGameMenu/Continue");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::ContinueInGameMenu_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/Apply");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::MatrixMenuApply_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen1/Forward1");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::InfoScreenButton_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen1/Backward1");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::InfoScreenButton_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Forward1");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::InfoScreenButton_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Backward1");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::InfoScreenButton_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Forward2");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::InfoScreenButton_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Backward2");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::InfoScreenButton_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen3/Forward1");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::InfoScreenButton_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen3/Backward1");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::InfoScreenButton_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen4/Forward1");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::InfoScreenButton_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen4/Backward1");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::InfoScreenButton_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen5/Forward1");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::InfoScreenButton_OnClick, this));

	windowElement = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen5/Backward1");
	windowElement->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Menu::InfoScreenButton_OnClick, this));

	// Matrix menu
	mMatrixBox[0] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox1");
	mMatrixBox[1] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox2");
	mMatrixBox[2] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox3");
	mMatrixBox[3] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox4");
	mMatrixBox[4] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox5");
	mMatrixBox[5] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox6");
	mMatrixBox[6] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox7");
	mMatrixBox[7] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox8");
	mMatrixBox[8] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox9");
	mMatrixBox[9] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox10");
	mMatrixBox[10] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox11");
	mMatrixBox[11] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox12");
	mMatrixBox[12] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox13");
	mMatrixBox[13] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox14");
	mMatrixBox[14] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox15");
	mMatrixBox[15] = (CEGUI::Editbox *)CEGUI::WindowManager::getSingleton().getWindow("MatrixMenu/EditBox16");

	// Set the editbox fields of the matrix to zero
	for(int i = 0; i <= 15; i++) {

		mMatrixBox[i]->setText("0");

	}

	// Set scale to 1
	mMatrixBox[0]->setText("1");
	mMatrixBox[5]->setText("1");
	mMatrixBox[10]->setText("1");
	mMatrixBox[15]->setText("1");

}

/*
 * Destructor
 *
 */
Menu::~Menu() {

}

/*
 * Get the clicked level number (for the game class
 * to know which level to load)
 *
 * @return int		the level number
 *
 */
int Menu::getLevelNumber() {

	return mLevelNumber;

}

/*
 * Get the state of the pause menu
 *
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::getPauseMenu() {

	return mPauseMenu;

}

/*
 * Get the state of the clicked main menu (for the game
 * class to know to load the main menu scene)
 *
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::getMainMenuClicked() {

	return mMainMenuClicked;

}

/*
 * Set the levelnumber
 *
 * @param levelNumber		level number, that the menu class knows the level number
 *
 */
void Menu::setLevelNumber(int levelNumber) {

	mLevelNumber = levelNumber;

}

/*
 * Set the state of the clicked main menu (for the game
 * class to know to load the main menu scene)
 *
 * @param mainMenuClicked		has the main menu been clicked?
 *
 */
void Menu::setMainMenuClicked(bool mainMenuClicked) {

	mMainMenuClicked = mainMenuClicked;

}

/*
 * Callback function - If quit button has been clicked
 * quit the game
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::Quit_OnClick(const CEGUI::EventArgs &args) {

	//mContinue = false;
	exit(0);
	return true;

}

/*
 * Callback function - If continue button has been clicked
 * set status true
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::ContinueInGameMenu_OnClick(const CEGUI::EventArgs &args) {

	mContinueClicked = true;
	return true;

}


/*
 * Callback function - If one of the infoscreen buttons
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::TextureMenu_OnClick(const CEGUI::EventArgs &args) {

	const CEGUI::WindowEventArgs& we = static_cast<const CEGUI::WindowEventArgs&>(args);

	CEGUI::String senderID = we.window->getName();

	int itemTextureSubEntity = 0;

	// If window element is active
	if(TextureEntity->isActive()) {

		switch(TextureEntity->getItemIndex(TextureEntity->getSelectedItem())) {

			case 0:
			TextureSubEntity->enable();
			break;

			case 1:
			TextureSubEntity->disable();
			break;

			case 2:
			TextureSubEntity->disable();
			break;

		}

	}

	// If window element is active
	if(TextureSubEntity->isActive()) {

		itemTextureSubEntity = TextureSubEntity->getItemIndex(TextureSubEntity->getSelectedItem());

	}

	// If window element is active
	if(Textures->isActive()) {

		int itemTexture = Textures->getItemIndex(Textures->getSelectedItem());
		itemTextureSubEntity = TextureSubEntity->getItemIndex(TextureSubEntity->getSelectedItem());

		mPlayerPointer->activateTexture(itemTexture, itemTextureSubEntity);

	}

	return true;

}

/*
 * Callback function - If one of the infoscreen buttons
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::InfoScreenButton_OnClick(const CEGUI::EventArgs &args) {

	const CEGUI::WindowEventArgs& we = static_cast<const CEGUI::WindowEventArgs&>(args);

	CEGUI::String senderID = we.window->getName();

	CEGUI::Window *window;

	if(senderID == "InfoScreen1/Forward1") {

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen1/Text2");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen1/Backward1");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen1/Text1");
		window->setVisible(false);

	} else if(senderID == "InfoScreen1/Backward1") {

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen1/Text1");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen1/Backward1");
		window->setVisible(false);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen1/Text2");
		window->setVisible(false);

	} else if(senderID == "InfoScreen2/Forward1") {

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Text2");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Backward1");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Forward2");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Text1");
		window->setVisible(false);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Forward1");
		window->setVisible(false);

	} else if(senderID == "InfoScreen2/Backward1") {

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Text1");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Forward1");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Backward1");
		window->setVisible(false);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Forward2");
		window->setVisible(false);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Text2");
		window->setVisible(false);

	} else if(senderID == "InfoScreen2/Forward2") {

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Text3");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Backward2");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Text2");
		window->setVisible(false);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Forward2");
		window->setVisible(false);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Backward1");
		window->setVisible(false);

	} else if(senderID == "InfoScreen2/Backward2") {

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Text2");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Forward2");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Backward1");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Backward2");
		window->setVisible(false);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen2/Text3");
		window->setVisible(false);

	} else if(senderID == "InfoScreen3/Forward1") {

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen3/Text2");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen3/Backward1");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen3/Text1");
		window->setVisible(false);

	} else if(senderID == "InfoScreen3/Backward1") {

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen3/Text1");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen3/Backward1");
		window->setVisible(false);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen3/Text2");
		window->setVisible(false);

	} else if(senderID == "InfoScreen4/Forward1") {

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen4/Text2");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen4/Backward1");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen4/Text1");
		window->setVisible(false);

	} else if(senderID == "InfoScreen4/Backward1") {

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen4/Text1");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen4/Backward1");
		window->setVisible(false);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen4/Text2");
		window->setVisible(false);

	} else if(senderID == "InfoScreen5/Forward1") {

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen5/Text2");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen5/Backward1");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen5/Text1");
		window->setVisible(false);

	} else if(senderID == "InfoScreen5/Backward1") {

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen5/Text1");
		window->setVisible(true);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen5/Backward1");
		window->setVisible(false);

		window = CEGUI::WindowManager::getSingleton().getWindow("InfoScreen5/Text2");
		window->setVisible(false);

	}

	return true;

}

/*
 * Callback function - If the exit button in the credits
 * menu has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::MatrixMenuApply_OnClick(const CEGUI::EventArgs &args) {

	mMatrixMenuClicked = true;
	pSystem->setGUISheet(mMatrixMenuTranslateLayout);
	mMatrixMenuTranslateLayout->hide();
	return true;

}

/*
 * Callback function - If the exit button in the credits
 * menu has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::ExitCredits_OnClick(const CEGUI::EventArgs &args) {

	mMainMenuClicked = false;
	mCreditsMenuLayout->hide();
	pSystem->setGUISheet(mMainMenuLayout);
	mMainMenuLayout->show();
	return true;

}

/*
 * Callback function - If the main menu button
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::Quit_OnMainMenu(const CEGUI::EventArgs &args) {

	mMainMenuClicked = true;
	mInGameMenuLayout->hide();
	mOptionsMenuLayout->hide();
	mCreditsMenuLayout->hide();
	pSystem->setGUISheet(mMainMenuLayout);
	mMainMenuLayout->show();
	return true;

}

/*
 * Get the entered matrix
 *
 * @return Matrix4		a 4x4 matrix
 *
 */
Matrix4 Menu::getMatrixTranslation() {

	return mMatrix;

}

/*
 * Get the scaling of the entered matrix
 *
 * @return Vector3		the scaling of the matrix
 *
 */
Vector3 Menu::getMatrixScaling() {

	return Vector3(mMatrix[0][0], mMatrix[1][1], mMatrix[2][2]);

}

/*
 * Callback function - If the continue button of the matrix
 * menu has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::ContinueMatrixMenu_OnClick(const CEGUI::EventArgs &args) {

	// Use stringstream for converting string to float

	sstr << mMatrixBox[0]->getText();
	sstr >> mMatrix[0][0];
	sstr.clear();
	sstr << mMatrixBox[1]->getText();
	sstr >> mMatrix[0][1];
	sstr.clear();
	sstr << mMatrixBox[2]->getText();
	sstr >> mMatrix[0][2];
	sstr.clear();
	sstr << mMatrixBox[3]->getText();
	sstr >> mMatrix[0][3];
	sstr.clear();
	sstr << mMatrixBox[4]->getText();
	sstr >> mMatrix[1][0];
	sstr.clear();
	sstr << mMatrixBox[5]->getText();
	sstr >> mMatrix[1][1];
	sstr.clear();
	sstr << mMatrixBox[6]->getText();
	sstr >> mMatrix[1][2];
	sstr.clear();
	sstr << mMatrixBox[7]->getText();
	sstr >> mMatrix[1][3];
	sstr.clear();
	sstr << mMatrixBox[8]->getText();
	sstr >> mMatrix[2][0];
	sstr.clear();
	sstr << mMatrixBox[9]->getText();
	sstr >> mMatrix[2][1];
	sstr.clear();
	sstr << mMatrixBox[10]->getText();
	sstr >> mMatrix[2][2];
	sstr.clear();
	sstr << mMatrixBox[11]->getText();
	sstr >> mMatrix[2][3];
	sstr.clear();
	sstr << mMatrixBox[12]->getText();
	sstr >> mMatrix[3][0];
	sstr.clear();
	sstr << mMatrixBox[13]->getText();
	sstr >> mMatrix[3][1];
	sstr.clear();
	sstr << mMatrixBox[14]->getText();
	sstr >> mMatrix[3][2];
	sstr.clear();
	sstr << mMatrixBox[15]->getText();
	sstr >> mMatrix[3][3];

	return true;

}

/*
 * Callback function - If the start new game menu button
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::Quit_OnNewGame(const CEGUI::EventArgs &args) {

	mMainMenuClicked = false;
	mMainMenuLayout->hide();
	pSystem->setGUISheet(mStartGameMenuLayout);
	mStartGameMenuLayout->show();
	return true;

}

/*
 * Callback function - If the one player button
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::Quit_OnePlayer(const CEGUI::EventArgs &args) {

	mStartGameMenuLayout->hide();
	pSystem->setGUISheet(mStoryLayout1);
	mStoryLayout1->show();
	return true;

}

/*
 * Callback function - If the continue button of first story screen
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::Continue_OnClick(const CEGUI::EventArgs &args) {

	mStoryLayout1->hide();
	pSystem->setGUISheet(mLevelMenuLayout);
	mLevelMenuLayout->show();
	return true;

}

/*
 * Callback function - If the help menu button is clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::HelpMenu_OnClick(const CEGUI::EventArgs &args) {

	mInGameMenuLayout->hide();
	pSystem->setGUISheet(mHelpMenuLayout);
	mHelpMenuLayout->show();
	return true;

}

/*
 * Callback function - If the help menu back button is clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::HelpMenuBack_OnClick(const CEGUI::EventArgs &args) {

	mHelpMenuLayout->hide();
	pSystem->setGUISheet(mInGameMenuLayout);
	mInGameMenuLayout->show();
	return true;

}


/*
 * Callback function - If the open options button
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::OpenOptions_OnClick(const CEGUI::EventArgs &args) {

	mMainMenuLayout->hide();
	pSystem->setGUISheet(mOptionsMenuLayout);
	mOptionsMenuLayout->show();
	return true;

}

/*
 * Callback function - If the back button in level menu
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::ExitLevelMenu_OnClick(const CEGUI::EventArgs &args) {

	mLevelMenuLayout->hide();
	pSystem->setGUISheet(mStartGameMenuLayout);
	mStartGameMenuLayout->show();
	return true;

}

/*
 * Callback function - If one of the level buttons
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::LoadLevel1_OnClick(const CEGUI::EventArgs &args) {

	mMainMenu = false;
	mLevelMenuLayout->hide();
	CEGUI::MouseCursor::getSingleton().hide();
	mGameOverlay->showOverlay("Score");
	mGameOverlay->showOverlay("Energy");
	mGameOverlay->showOverlay("Lifes");
	mGameOverlay->showOverlay("Vector");
	mGameOverlay->showOverlay("Infobox");
	mLevelNumber = 1;
	return true;

}

/*
 * Callback function - If one of the level buttons
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::LoadLevel2_OnClick(const CEGUI::EventArgs &args) {

	mMainMenu = false;
	mLevelMenuLayout->hide();
	CEGUI::MouseCursor::getSingleton().hide();
	mGameOverlay->showOverlay("Score");
	mGameOverlay->showOverlay("Energy");
	mGameOverlay->showOverlay("Lifes");
	mGameOverlay->showOverlay("Vector");
	mGameOverlay->showOverlay("Infobox");
	mLevelNumber = 2;
	return true;

}

/*
 * Callback function - If one of the level buttons
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::LoadLevel3_OnClick(const CEGUI::EventArgs &args) {

	mMainMenu = false;
	mLevelMenuLayout->hide();
	CEGUI::MouseCursor::getSingleton().hide();
	mGameOverlay->showOverlay("Score");
	mGameOverlay->showOverlay("Energy");
	mGameOverlay->showOverlay("Lifes");
	mGameOverlay->showOverlay("Vector");
	mGameOverlay->showOverlay("Infobox");
	mLevelNumber = 3;
	return true;

}

/*
 * Callback function - If one of the level buttons
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::LoadLevel4_OnClick(const CEGUI::EventArgs &args) {

	mMainMenu = false;
	mLevelMenuLayout->hide();
	CEGUI::MouseCursor::getSingleton().hide();
	mGameOverlay->showOverlay("Score");
	mGameOverlay->showOverlay("Energy");
	mGameOverlay->showOverlay("Lifes");
	mGameOverlay->showOverlay("Vector");
	mGameOverlay->showOverlay("Infobox");
	mLevelNumber = 4;
	return true;

}

/*
 * Callback function - If one of the level buttons
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::LoadLevel5_OnClick(const CEGUI::EventArgs &args) {

	mMainMenu = false;
	mLevelMenuLayout->hide();
	CEGUI::MouseCursor::getSingleton().hide();
	mGameOverlay->showOverlay("Score");
	mGameOverlay->showOverlay("Energy");
	mGameOverlay->showOverlay("Lifes");
	mGameOverlay->showOverlay("Vector");
	mGameOverlay->showOverlay("Infobox");
	mLevelNumber = 5;
	return true;

}

/*
 * Callback function - If the open credits button
 * has been clicked
 *
 * @param args		CEGUI event
 * @return bool		operation successful (true / false)
 *
 */
bool Menu::OpenCredits_OnClick(const CEGUI::EventArgs &args) {

	mMainMenuLayout->hide();
	pSystem->setGUISheet(mCreditsMenuLayout);
	mCreditsMenuLayout->show();
	return true;

}

/*
 * Callback function - Show the ingame menu
 *
 */
void Menu::showInGameMenu() {

	mPauseMenu = true;
	pSystem->setGUISheet(mInGameMenuLayout);
	mInGameMenuLayout->show();
	CEGUI::MouseCursor::getSingleton().show();

}

/*
 * Callback function - Hide the ingame menu
 *
 */
void Menu::hideInGameMenu() {

	mPauseMenu = false;
	pSystem->setGUISheet(mInGameMenuLayout);
	mInGameMenuLayout->hide();
	CEGUI::MouseCursor::getSingleton().hide();

}

/*
 * Callback function - Show the ingame menu
 *
 */
void Menu::showMainMenu() {

	mMainMenu = true;
	pSystem->setGUISheet(mMainMenuLayout);
	mMainMenuLayout->show();
	CEGUI::MouseCursor::getSingleton().show();

}

/*
 * Callback function - If the main menu is opened
 *
 */
bool Menu::getMainMenu() {

	return mMainMenu;

}

/*
 * Show the story screen
 *
 * @param storyScreen		the number of the story screen
 *
 */
void Menu::showStoryScreen(int storyScreen) {

	switch(storyScreen) {

	case 1:
		pSystem->setGUISheet(mStoryLayout1);
		mStoryLayout1->show();
		CEGUI::MouseCursor::getSingleton().show();
		break;

	}

}

/*
 * Show the info screen
 *
 * @param infoScreen		the number of the info screen
 *
 */
void Menu::showInfoScreen(int infoScreen) {

	switch(infoScreen) {

	case 1:
		pSystem->setGUISheet(mInfoLayout1);
		mInfoLayout1->show();
		break;

	case 2:
		pSystem->setGUISheet(mInfoLayout2);
		mInfoLayout2->show();
		break;

	case 3:
		pSystem->setGUISheet(mInfoLayout3);
		mInfoLayout3->show();
		break;

	case 4:
		pSystem->setGUISheet(mInfoLayout4);
		mInfoLayout4->show();
		break;

	case 5:
		pSystem->setGUISheet(mInfoLayout5);
		mInfoLayout5->show();
		break;

	}

	CEGUI::MouseCursor::getSingleton().show();

}

/*
 * Hide the info screen
 *
 * @param infoScreen		the number of the screen
 *
 */
void Menu::hideInfoScreen(int infoScreen) {

	switch(infoScreen) {

	case 1:
		pSystem->setGUISheet(mInfoLayout1);
		mInfoLayout1->hide();
		break;

	case 2:
		pSystem->setGUISheet(mInfoLayout2);
		mInfoLayout2->hide();
		break;

	case 3:
		pSystem->setGUISheet(mInfoLayout3);
		mInfoLayout3->hide();
		break;

	case 4:
		pSystem->setGUISheet(mInfoLayout4);
		mInfoLayout4->hide();
		break;

	case 5:
		pSystem->setGUISheet(mInfoLayout5);
		mInfoLayout5->hide();
		break;

	}

	CEGUI::MouseCursor::getSingleton().hide();

}

/*
 * Show the matrix menu
 *
 */
void Menu::showMatrixMenu() {

	pSystem->setGUISheet(mMatrixMenuTranslateLayout);
	mMatrixMenuTranslateLayout->show();

	CEGUI::MouseCursor::getSingleton().show();

}

/*
 * Show the texture menu
 *
 */
void Menu::showTextureMenu() {

	pSystem->setGUISheet(mTextureMenuLayout);
	mTextureMenuLayout->show();
	CEGUI::MouseCursor::getSingleton().show();

}

/*
 * Hide the matrix menu
 *
 */
void Menu::hideMatrixMenu() {

	mMatrixMenuTranslateLayout->hide();
	CEGUI::MouseCursor::getSingleton().hide();

}

/*
 * Get continue clicked
 *
 * @return bool		has continue been clicked?
 *
 */
bool Menu::getContinueClicked() {

	return mContinueClicked;

}

/*
 * Set continue clicked
 *
 * @param bool		has continue been clicked?
 *
 */
void Menu::setContinueClicked(bool continueClicked) {

	mContinueClicked = continueClicked;

}

/*
 * Set matrix menu clicked
 *
 * @param bool		has matrix menu been clicked?
 *
 */
void Menu::setMatrixMenuClicked(bool matrixMenuClicked) {

	mMatrixMenuClicked = matrixMenuClicked;

}

/*
 * Get matrix menu clicked
 *
 * @param bool		has matrix menu been clicked?
 *
 */
bool Menu::getMatrixMenuClicked() {

	return mMatrixMenuClicked;

}
