#pragma once

#include <HAPISprites_lib.h>
#include <memory>
#include <string>

struct Entity
{
	Entity(const std::string& spriteName)
		: m_sprite(HAPI_Sprites.LoadSprite(spriteName)),
		m_movementPoints(5)
	{}

	std::shared_ptr<HAPISPACE::Sprite> m_sprite;
	int m_movementPoints;
};