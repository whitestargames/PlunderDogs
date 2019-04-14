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
	void startBattle(std::vector<EntityProperties*>& selectedEntities);

private:
	std::vector<EntityProperties> m_entities;
	OverWorldGUI m_GUI;
	std::unique_ptr<Battle> m_battle;
};