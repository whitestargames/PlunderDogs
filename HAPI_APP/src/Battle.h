#pragma once

#include "Map.h"
#include "entity.h"
#include "BattleUI.h"

class Battle
{
public:
	Battle();

	Map& getMap();

	void render();
	void update(float deltaTime);
	void moveEntityTo(std::pair<std::unique_ptr<Entity>, BattleProperties>& entity, Tile& destination);

private:
	std::pair<std::unique_ptr<Entity>, BattleProperties> m_entity;
	Map m_map;
	BattleUI m_battleUI;
};