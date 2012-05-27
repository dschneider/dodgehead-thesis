/**
 * Class GameOverlay - Implementation File
 *
 * This class is for showing overlays in the game
 * using OGRE's overlay system
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#include "GameOverlay.h"

/*
 * Constructor
 *
 * Initialize the object
 *
 */
GameOverlay::GameOverlay() {
}

/*
 * Destructor
 *
 * Frees memory by destroying the objects
 *
 */
GameOverlay::~GameOverlay() {

	delete mOverlay;
	delete mTextElementPtr;

}

/*
 * Show a overlay by the given name
 *
 * @param overlayName		name of the overlay
 *
 */
void GameOverlay::showOverlay(String overlayName) {

	if(overlayName == "Pause") {

		mOverlay = OverlayManager::getSingleton().getByName("Pause");
		mOverlay->show();

	} else if(overlayName == "GameOver") {

		mOverlay = OverlayManager::getSingleton().getByName("GameOver");
		mOverlay->show();

	} else if(overlayName == "LevelUp") {

		mOverlay = OverlayManager::getSingleton().getByName("LevelUp");
		mOverlay->show();

	} else if(overlayName == "Score") {

		mOverlay = OverlayManager::getSingleton().getByName("Score");
		mOverlay->show();

	} else if(overlayName == "Energy") {

		mOverlay = OverlayManager::getSingleton().getByName("Energy");
		mOverlay->show();

	} else if(overlayName == "Lifes") {

		mOverlay = OverlayManager::getSingleton().getByName("Lifes");
		mOverlay->show();

	} else if(overlayName == "Time") {

		mOverlay = OverlayManager::getSingleton().getByName("Time");
		mOverlay->show();

	} else if(overlayName == "Vector") {

		mOverlay = OverlayManager::getSingleton().getByName("Vector");
		mOverlay->show();

	} else if(overlayName == "Levelname") {

		mOverlay = OverlayManager::getSingleton().getByName("Levelname");
		mOverlay->show();

	} else if(overlayName == "Hurt") {

		mOverlay = OverlayManager::getSingleton().getByName("Hurt");
		mOverlay->show();

	} else if(overlayName == "Logo") {

		mOverlay = OverlayManager::getSingleton().getByName("Logo");
		mOverlay->show();

	} else if(overlayName == "Infobox") {

		mOverlay = OverlayManager::getSingleton().getByName("Infobox");
		mOverlay->show();

	}

}

/*
 * Hide a overlay by the given name
 *
 * @param overlayName		name of the overlay
 *
 */
void GameOverlay::hideOverlay(String overlayName) {

	if(overlayName == "Pause") {

		mOverlay = OverlayManager::getSingleton().getByName("Pause");
		mOverlay->hide();

	} else if(overlayName == "GameOver") {

		mOverlay = OverlayManager::getSingleton().getByName("GameOver");
		mOverlay->hide();

	} else if(overlayName == "LevelUp") {

		mOverlay = OverlayManager::getSingleton().getByName("LevelUp");
		mOverlay->hide();

	} else if(overlayName == "Score") {

		mOverlay = OverlayManager::getSingleton().getByName("Score");
		mOverlay->hide();

	} else if(overlayName == "Energy") {

		mOverlay = OverlayManager::getSingleton().getByName("Energy");
		mOverlay->hide();

	}  else if(overlayName == "Lifes") {

		mOverlay = OverlayManager::getSingleton().getByName("Lifes");
		mOverlay->hide();

	} else if(overlayName == "Time") {

		mOverlay = OverlayManager::getSingleton().getByName("Time");
		mOverlay->hide();

	} else if(overlayName == "Vector") {

		mOverlay = OverlayManager::getSingleton().getByName("Vector");
		mOverlay->hide();

	} else if(overlayName == "Levelname") {

		mOverlay = OverlayManager::getSingleton().getByName("Levelname");
		mOverlay->hide();

	} else if(overlayName == "Hurt") {

		mOverlay = OverlayManager::getSingleton().getByName("Hurt");
		mOverlay->hide();

	} else if(overlayName == "Logo") {

		mOverlay = OverlayManager::getSingleton().getByName("Logo");
		mOverlay->hide();

	} else if(overlayName == "Infobox") {

		mOverlay = OverlayManager::getSingleton().getByName("Infobox");
		mOverlay->hide();

	}


}

/*
 * Update the score overlay
 *
 * @param score		the score string
 *
 */
void GameOverlay::updateScore(String score) {

	//textElementPtr = static_cast< OverlayElement* >( OverlayManager::getSingletonPtr()->getOverlayElement( "left" ) ); // get the left player’s TextArea
	mTextElementPtr = OverlayManager::getSingletonPtr()->getOverlayElement("left");
	mTextElementPtr->setCaption(score); // set the TextElement's text

}

/*
 * Update the energy overlay
 *
 * @param energy		the energy string
 *
 */
void GameOverlay::updateEnergy(String energy) {

	//textElementPtr = static_cast< OverlayElement* >( OverlayManager::getSingletonPtr()->getOverlayElement( "left" ) ); // get the left player’s TextArea
	mTextElementPtr = OverlayManager::getSingletonPtr()->getOverlayElement("EnergyLeft");
	mTextElementPtr->setCaption(energy); // set the TextElement's text

}

/*
 * Update the lifes overlay
 *
 * @param lifes		the lifes string
 *
 */
void GameOverlay::updateLifes(String lifes) {

	//textElementPtr = static_cast< OverlayElement* >( OverlayManager::getSingletonPtr()->getOverlayElement( "left" ) ); // get the left player’s TextArea
	mTextElementPtr = OverlayManager::getSingletonPtr()->getOverlayElement("LifesLeft");
	mTextElementPtr->setCaption(lifes); // set the TextElement's text

}

/*
 * Update the time overlay
 *
 * @param time		the time string
 *
 */
void GameOverlay::updateTime(String time) {

	//textElementPtr = static_cast< OverlayElement* >( OverlayManager::getSingletonPtr()->getOverlayElement( "left" ) ); // get the left player’s TextArea
	mTextElementPtr = OverlayManager::getSingletonPtr()->getOverlayElement("TimeText");
	mTextElementPtr->setCaption(time); // set the TextElement's text

}

/*
 * Update the vector overlay
 *
 * @param time		the vector string
 *
 */
void GameOverlay::updateVector(String vector) {

	//textElementPtr = static_cast< OverlayElement* >( OverlayManager::getSingletonPtr()->getOverlayElement( "left" ) ); // get the left player’s TextArea
	mTextElementPtr = OverlayManager::getSingletonPtr()->getOverlayElement("VectorLeft");
	mTextElementPtr->setCaption(vector); // set the TextElement's text

}

/*
 * Update the level name overlay
 *
 * @param time		the level nam string
 *
 */
void GameOverlay::updateLevelName(String levelName) {

	//textElementPtr = static_cast< OverlayElement* >( OverlayManager::getSingletonPtr()->getOverlayElement( "left" ) ); // get the left player’s TextArea
	mTextElementPtr = OverlayManager::getSingletonPtr()->getOverlayElement("LevelnameText");
	mTextElementPtr->setCaption(levelName); // set the TextElement's text

}

/*
 * Update the infobox overlay
 *
 * @param time		the infobox string
 *
 */
void GameOverlay::updateInfobox(String infoText) {

	//textElementPtr = static_cast< OverlayElement* >( OverlayManager::getSingletonPtr()->getOverlayElement( "left" ) ); // get the left player’s TextArea
	mTextElementPtr = OverlayManager::getSingletonPtr()->getOverlayElement("InfoboxText");
	mTextElementPtr->setCaption(infoText); // set the TextElement's text

}
