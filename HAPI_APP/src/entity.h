#pragma once

#include <HAPISprites_lib.h>
#include <memory>
#include <string>

struct Entity
{
	Entity(const std::string& spriteName)
		: m_sprite(HAPI_Sprites.LoadSprite(spriteName))
	{}

	std::unique_ptr<HAPISPACE::Sprite> m_sprite;
};