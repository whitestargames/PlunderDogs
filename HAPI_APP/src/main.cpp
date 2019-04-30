#include "OverWorld.h"
#include "Textures.h"
#include "AudioPlayer.h"

//TODO: AUDIO PLAYER
//TODO: Target area appearing on land
//TODO: BATTLEUI Sprites not drawin correctly - dragging around screen
//TODO: CAn seemingly place ships on land
//TODO: Notify game Won 
//TODO: Sniper directly facing a collidable tile - line still draws

//CLEAR MOVEMENT UI WHEN ENDING PHASE EARLY

using namespace HAPISPACE;

float getDeltaTime(int frameStart, int lastFrameStart)
{
	return static_cast<float>(frameStart - lastFrameStart) / 1000.0f;
}

void HAPI_Sprites_Main()
{
	std::pair<int, int> windowSize(1920, 1080);
	if (!HAPI_Sprites.Initialise(windowSize.first, windowSize.second, "Plunder Dogs", eHSEnableUI|eHSFullscreen))
		return;

	if (!Textures::loadAllTextures())
	{
		HAPI_Sprites.UserMessage("Could not pre-load all textures", "Texture Error");
		return;
	}

<<<<<<< HEAD
	AudioPlayer::getInstance().registerSound("piratesAtSea.wav", "main menu");
=======
	AudioPlayer::getInstance().registerSound("pirate_theme.wav", "main menu");
>>>>>>> parent of adf88d1... added more sounds
	AudioPlayer::getInstance().registerSound("Blackmoor Tides Loop.wav", "battle theme");
	AudioPlayer::getInstance().registerSound("GunShotGverb.wav", "shoot");
	AudioPlayer::getInstance().registerSound("ExplosionMetalGverb.wav", "hit");
	AudioPlayer::getInstance().registerSound("misc_menu_4.wav", "click");
	AudioPlayer::getInstance().registerSound("click_2.wav", "hover");
	AudioPlayer::getInstance().registerSound("Heavy_ConceptB.wav", "win");
	HAPI_Sprites.SetShowFPS(true);
	HAPI_Sprites.LimitFrameRate(150);

	int lastFrameStart = HAPI_Sprites.GetTime();
	OverWorld overWorld;

	while (HAPI_Sprites.Update())
	{
		int frameStart = HAPI_Sprites.GetTime();

		SCREEN_SURFACE->Clear();
		overWorld.update(getDeltaTime(frameStart, lastFrameStart));
		overWorld.render();

		lastFrameStart = frameStart;
	}
}