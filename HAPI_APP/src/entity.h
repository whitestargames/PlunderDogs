#pragma once

#include <HAPISprites_lib.h>
#include <memory>
#include <string>
#include "Timer.h"


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
	
	void render() const { m_sprite->Render(SCREEN_SURFACE); }
};