#include "AudioPlayer.h"
#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include "Utilities/Utilities.h"
AudioPlayer::AudioPlayer()
{
	HAPI_Sprites.LoadSound(Utilities::getDataDirectory() + "ExplosionMetal.wav");
}

void AudioPlayer::playSound(const std::string & fileName)
{
	//HAPI_Sprites.PlaySound(Utilities::getDataDirectory() + fileName);
	
}
