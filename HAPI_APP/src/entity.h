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
	Entity(const std::string& spriteName, int movementPoints, int healthPoints, int range, int damage)
		: m_sprite(HAPI_Sprites.LoadSprite(spriteName)),
		m_movementPoints(movementPoints), m_healthPoints(healthPoints), m_range(range), m_damage(damage)
	{}

	std::shared_ptr<HAPISPACE::Sprite> m_sprite;
	int m_movementPoints;
	int m_healthPoints;
	int m_range;
	int m_damage;

	void render() const { m_sprite->Render(SCREEN_SURFACE); }

};