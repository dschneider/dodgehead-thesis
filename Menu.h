/**
 * Class Menu - Header File
 *
 * Handles all the menues
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#ifndef MENU_H_
#define MENU_H_

#include <iostream>
#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>

#include "Player.h"
#include "GameOverlay.h"

using namespace Ogre;
using namespace std;

class Menu {

public:
	Menu(SceneManager* sceneManager, RenderWindow* window, Player* player, String language, GameOverlay* gameOverlay);
	virtual ~Menu();
    bool Quit_OnClick(const CEGUI::EventArgs &args);
    bool Quit_OnMainMenu(const CEGUI::EventArgs &args);
    bool Quit_OnNewGame(const CEGUI::EventArgs &args);
    bool Quit_OnePlayer(const CEGUI::EventArgs &args);
    bool OpenOptions_OnClick(const CEGUI::EventArgs &args);
    bool OpenCredits_OnClick(const CEGUI::EventArgs &args);
    bool ExitCredits_OnClick(const CEGUI::EventArgs &args);
    bool LoadLevel1_OnClick(const CEGUI::EventArgs &args);
    bool LoadLevel2_OnClick(const CEGUI::EventArgs &args);
    bool LoadLevel3_OnClick(const CEGUI::EventArgs &args);
    bool LoadLevel4_OnClick(const CEGUI::EventArgs &args);
    bool LoadLevel5_OnClick(const CEGUI::EventArgs &args);
    bool HelpMenu_OnClick(const CEGUI::EventArgs &args);
    bool HelpMenuBack_OnClick(const CEGUI::EventArgs &args);
    bool ExitLevelMenu_OnClick(const CEGUI::EventArgs &args);
    bool Continue_OnClick(const CEGUI::EventArgs &args);
    bool ContinueMatrixMenu_OnClick(const CEGUI::EventArgs &args);
    bool ContinueInGameMenu_OnClick(const CEGUI::EventArgs &args);
    bool MatrixMenuApply_OnClick(const CEGUI::EventArgs &args);
	bool TextureEntityChange_OnMouseLeave(const CEGUI::EventArgs &args);
	bool InfoScreenButton_OnClick(const CEGUI::EventArgs &args);
	bool TextureMenu_OnClick(const CEGUI::EventArgs &args);
    bool getMainMenu();
    void showInGameMenu();
    void hideInGameMenu();
    void showMainMenu();
    int getLevelNumber();
    void setLevelNumber(int levelNumber);
    void showStoryScreen(int storyScreen);
    void showInfoScreen(int infoScreen);
    void hideInfoScreen(int infoScreen);
    void showMatrixMenu();
    void hideMatrixMenu();
	void showTextureMenu();
    bool getPauseMenu();
    void setMainMenuClicked(bool mainMenuClicked);
    bool getMainMenuClicked();
    bool getContinueClicked();
    void setMatrixMenuClicked(bool matrixMenuClicked);
    bool getMatrixMenuClicked();
    void setContinueClicked(bool continueClicked);
    Matrix4 getMatrixTranslation();
    Vector3 getMatrixScaling();

private:
    CEGUI::Window* mInGameMenuLayout;
    CEGUI::Window* mMainMenuLayout;
    CEGUI::Window* mCreditsMenuLayout;
    CEGUI::Window* mOptionsMenuLayout;
    CEGUI::Window* mStartGameMenuLayout;
    CEGUI::Window* mLevelMenuLayout;
    CEGUI::Window* mHelpMenuLayout;
    CEGUI::Window* mStoryLayout1;
    CEGUI::Window* mInfoLayout1;
    CEGUI::Window* mInfoLayout2;
    CEGUI::Window* mInfoLayout3;
    CEGUI::Window* mInfoLayout4;
    CEGUI::Window* mInfoLayout5;
    CEGUI::Window* mMatrixMenuTranslateLayout;
    CEGUI::Window* mMatrixMenuScaleLayout;
    CEGUI::Window* mOgreMenu;
	CEGUI::Window* mTextureMenuLayout;
	CEGUI::Combobox *TextureEntity;
	CEGUI::Combobox *TextureSubEntity;
	CEGUI::Combobox *Textures;
    CEGUI::System* pSystem;
    GameOverlay* mGameOverlay;

    CEGUI::Editbox* mMatrixBox[16];

	Player* mPlayerPointer;

    stringstream sstr;

    String mLanguage;

    Matrix4 mMatrix;

    fstream mFile;
    string mRead;
    int mActivatedLevel;
    bool mMainMenu;
    bool mPauseMenu;
    bool mMainMenuClicked;
    bool mContinueClicked;
    bool mMatrixMenuClicked;
    int mLevelNumber;

};

#endif /* MENU_H_ */
