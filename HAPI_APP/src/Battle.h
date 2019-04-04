#pragma once

#include <HAPISprites_lib.h>
#include <vector>
#include <utility>
#include "Map.h"
#include "entity.h"
#include "BattleUI.h"
#include <deque>

class Battle
{
public:
	Battle();

	Map& getMap();

	void render();
	void update(float deltaTime);
	void entityMoveTo(Entity& entity, std::pair<int, int> newPosition);

private:
	std::pair<std::unique_ptr<Entity>, EntityDetails> m_entity;
	Map m_map;
	BattleUI m_battleUI;
};