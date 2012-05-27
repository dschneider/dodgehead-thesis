/**
 * Class GameOverlay - Header File
 *
 * This class is for showing overlays in the game
 * using OGRE's overlay system
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#ifndef GAMEOVERLAY_H_
#define GAMEOVERLAY_H_

#include <Ogre.h>

using namespace Ogre;

class GameOverlay {

public:
	GameOverlay();
	virtual ~GameOverlay();
	void showOverlay(String overlayName);
	void hideOverlay(String overlayName);
	void updateScore(String score);
	void updateEnergy(String energy);
	void updateLifes(String lifes);
	void updateTime(String time);
	void updateVector(String vector);
	void updateLevelName(String levelName);
	void updateInfobox(String infoBox);

private:
    Overlay *mOverlay;
	OverlayElement *mTextElementPtr;

};

#endif /* GAMEOVERLAY_H_ */
