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
	void renderBattle() { m_battle.render(); }
	void update(float deltaTime);

	std::vector<EntityProperties>& getEntityVector() { return m_entities; }
private:
	std::vector<EntityProperties> m_entities;
	std::unique_ptr<OverWorldGUI> m_overWorldGUI;
	Battle m_battle;
};