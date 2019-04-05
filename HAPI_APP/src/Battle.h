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
	//TODO: BAD - will change exposure of raw pointer
	void moveEntityTo(BattleEntity* entity, Tile& destination);

private:
	std::vector<std::unique_ptr<BattleEntity>> m_entities;
	Map m_map;
	BattleUI m_battleUI;

	void insertEntity(std::pair<int, int> startingPosition);
};