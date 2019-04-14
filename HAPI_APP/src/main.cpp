#include "Overworld.h"
#include "BattleGUI.h"
using namespace HAPISPACE;

void HAPI_Sprites_Main()
{
	std::pair<int, int> windowSize(1600, 900);
	if (!HAPI_Sprites.Initialise(windowSize.first, windowSize.second, "Plunder Dogs", eHSEnableUI))//it lies
		return;
	HAPI_Sprites.SetShowFPS(true);

	OverWorld overWorld;
	BattleGUI battleGUI;

	while (HAPI_Sprites.Update())
	{
		//overWorld.update();//I think we can just delete this
		if (!overWorld.battleState)
		{
			overWorld.render();
		}
		else
		{
			battleGUI.render();
		}
	}	
}