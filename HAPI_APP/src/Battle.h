#pragma once

#include <HAPISprites_lib.h>
#include <vector>
#include <utility>
#include "Map.h"
#include "BattleUI.h"
#include "entity.h"

class Battle
{
private:
	std::vector<std::pair<Entity, std::pair<int, int> > > m_entities;
	Map m_map;
	BattleUI m_battleUI;
	std::pair<int, int> coord;
	int m_entityPositionInVector;
	
	void update();
	void render();
	
public:
	Battle();
	void run();
};