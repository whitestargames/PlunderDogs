#include "OverWorld.h"
#include "Textures.h"
//#include "BattleGUI.h"
using namespace HAPISPACE;

float getDeltaTime(int frameStart, int lastFrameStart)
{
	return static_cast<float>(frameStart - lastFrameStart) / 1000.0f;
}

void HAPI_Sprites_Main()
{
	std::pair<int, int> windowSize(1600, 900);
	if (!HAPI_Sprites.Initialise(windowSize.first, windowSize.second, "Plunder Dogs", eHSEnableUI))
		return;

	if (!Textures::loadAllTextures())
	{
		HAPI_Sprites.UserMessage("Could not pre-load all textures", "Texture Error");
		return;
	}

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
			//battleGUI.render();
}