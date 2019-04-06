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
	void moveEntityToPosition(BattleEntity& entity, const Tile& destination);

private:
	std::vector<std::unique_ptr<BattleEntity>> m_entities;
	Map m_map;
	BattleUI m_battleUI;

	void insertEntity(std::pair<int, int> startingPosition);
};