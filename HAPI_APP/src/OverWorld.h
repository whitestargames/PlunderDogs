#pragma once

#include <string>
#include "Global.h"
#include "Battle.h"
#include "OverWorldGUI.h"

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

class OverWorld
{
public:
	OverWorld();

	void render();
	void update(float deltaTime);

private:
	std::vector<EntityProperties> m_entities;
	OverWorldGUI m_GUI;
	Battle m_battle;
};