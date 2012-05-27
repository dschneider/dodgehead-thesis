/**
 * Class Sound - Header File
 *
 * This class initializes the audiere audio system, manages
 * the sound files and is able to play them
 *
 *
 * @author Dennis Schneider <sinned.schneider@gmail.com>
 *
 * @status final
 */

#ifndef SOUND_H_
#define SOUND_H_

#include "audiere.h"
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace audiere;

class Sound {

public:
	Sound();
	virtual ~Sound();
	void playSound(int soundnumber, bool repeat);
	void loadSounds();

private:
	AudioDevicePtr mDevice;
	OutputStreamPtr mSounds[4];

};

#endif /* SOUND_H_ */
