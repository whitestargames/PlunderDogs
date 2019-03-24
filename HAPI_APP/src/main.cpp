#include "Overworld.h"
using namespace HAPISPACE;

void HAPI_Sprites_Main()
{
	std::pair<int, int> windowSize(1600, 900);
	if (!HAPI_Sprites.Initialise(windowSize.first, windowSize.second, "Plunder Dogs", eHSEnableUI))//it lies
		return;
	HAPI_Sprites.SetShowFPS(true);

	OverWorld overWorld;
	while (HAPI_Sprites.Update())
	{
		overWorld.update();
		overWorld.render();
	}
	
}