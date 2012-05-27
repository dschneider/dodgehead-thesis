/**
 * Class Sound - Implementation File
 *
 * This class initializes the audiere audio system, manages
 * the sound files and is able to play them
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#include "Sound.h"

using namespace std;

/*
 * Constructor
 *
 * Initialize the class
 *
 */
Sound::Sound() {

	mDevice = OpenDevice();

}

/*
 * Destructor
 *
 * Frees memory by destroying objects
 *
 */
Sound::~Sound() {

	delete mSounds;
	mDevice = NULL;

}

/*
 * Load all sounds
 *
 */
void Sound::loadSounds() {

	mSounds[0] = OpenSound(mDevice, "resource/sounds/coin.wav", false);
	mSounds[1] = OpenSound(mDevice, "resource/sounds/jungle-run.wav", true);
	mSounds[2] = OpenSound(mDevice, "resource/sounds/hit.mp3", false);

	if (!mSounds[0]) {
		cerr << "OpenSound() failed" << endl;
	}

}

/*
 * Play a sound
 *
 * @param soundnumber		the number of the soundfile to play within the array
 * @param repeat			repeat the sound after playing?
 *
 */
void Sound::playSound(int soundnumber, bool repeat) {

	mSounds[soundnumber]->play();
	if(repeat) {
		mSounds[soundnumber]->setRepeat(true);
	}
	mDevice->update();

}
