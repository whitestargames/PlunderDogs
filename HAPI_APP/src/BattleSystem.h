#pragma once

#include <HAPISprites_lib.h>
#include <vector>
#include <utility>
#include "Map.h"
#include "UIClass.h"

class Entity;
class BattleSystem
{
private:
	void update();

	std::vector<std::pair<Entity*, std::pair<int, int>>> m_entities;
	Map m_map;
	UIWindowTest UIWind;
	std::pair<int, int>coord;
	int entityPositionInVector;
	
public:
	BattleSystem();
	~BattleSystem();
	void run();
};