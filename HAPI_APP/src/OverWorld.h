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

	void render(OverWorldWindow currentWindow);
	void renderBattle() const { m_battle.render(); }
	void update(float deltaTime);

	static OverWorldWindow CURRENT_WINDOW;
	std::vector<EntityProperties>& getEntityVector() { return m_entities; }

private:
	std::vector<EntityProperties> m_entities;
	std::unique_ptr<GUIBase> m_overWorldGUI;
	Battle m_battle;
};