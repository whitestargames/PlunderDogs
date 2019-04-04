#pragma once

#include <HAPISprites_lib.h>
#include <memory>
#include <string>
#include "Timer.h"

struct EntityDetails
{
	EntityDetails()
		: m_currentPosition(),
		m_oldPosition(),
		m_pathToTile(),
		m_moving(false)
	{}

	std::pair<int, int> m_currentPosition;
	std::pair<int, int> m_oldPosition;
	std::deque<std::pair<int, int>> m_pathToTile;
	bool m_moving;
};

class Map;
struct Entity
{
	Entity(const std::string& spriteName)
	: m_sprite(HAPI_Sprites.LoadSprite(spriteName)),
		m_movementPoints(5),
		m_movementTimer(0.5f)
		{}
	

	std::shared_ptr<HAPISPACE::Sprite> m_sprite;
	int m_movementPoints;
	Timer m_movementTimer;
	
	void render(Map& map, float drawOffsetX, float drawOffsetY);
};