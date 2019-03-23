#pragma once

#include <HAPISprites_lib.h>
#include <vector>
#include <utility>
#include "Map.h"
#include "UIClass.h"
#include "entity.h"

class BattleSystem
{
private:
	std::vector<std::pair<Entity, std::pair<int, int> > > m_entities;
	Map m_map;
	UIWindowTest UIWind;
	std::pair<int, int> coord;
	
	void update();
	void render();
	
public:
	BattleSystem();
	void run();
};