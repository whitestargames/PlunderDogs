#include "OverWorld.h"
#include "Textures.h"
using namespace HAPISPACE;

void HAPI_Sprites_Main()
{
	std::pair<int, int> windowSize(1600, 900);
	if (!HAPI_Sprites.Initialise(windowSize.first, windowSize.second, "Plunder Dogs", eHSEnableUI))
		return;

	if (!Textures::loadAllTextures())
	{
		return;
	}

	HAPI_Sprites.SetShowFPS(true);
	HAPI_Sprites.LimitFrameRate(200);

	int lastFrameStart = HAPI_Sprites.GetTime();
	OverWorld overWorld;

	while (HAPI_Sprites.Update())
	{
		int frameStart = HAPI_Sprites.GetTime();

		SCREEN_SURFACE->Clear();
		overWorld.update(static_cast<float>(frameStart - lastFrameStart) / 1000.0f);
		overWorld.render();

		lastFrameStart = frameStart;
	}
}