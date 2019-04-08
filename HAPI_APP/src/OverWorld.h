#pragma once

#include <string>
#include "Battle.h"
#include "OverWorldGUI.h"

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

enum class OverWorldWindow
{
	Battle = 0,
	PreBattle,
	LevelSelection
};

class OverWorld
{
public:
	OverWorld();

	void render();
	void update(float deltaTime);
	
	static OverWorldWindow CURRENT_WINDOW;

private:
	OverWorldGUI m_GUI;
	Battle m_battle;
};