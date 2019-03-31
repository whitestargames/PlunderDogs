#pragma once

#include <HAPISprites_lib.h>
#include <memory>
#include <string>

struct Entity
{
	Entity(const std::string& spriteName)
	: m_sprite(HAPI_Sprites.LoadSprite(spriteName)),
			m_movementPoints(5),
			m_entityDirection(eDirection::eNorth),
			m_entityPrevDirection(eDirection::eNorth)
		{}
	

	std::shared_ptr<HAPISPACE::Sprite> m_sprite;
	int m_movementPoints;
	eDirection m_entityDirection;
	eDirection m_entityPrevDirection;
	void render() const { m_sprite->Render(SCREEN_SURFACE); }
};