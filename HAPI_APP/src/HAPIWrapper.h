#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>
#include <string>

namespace HAPI_Wrapper
{
	bool spriteTranslated(std::unique_ptr<Sprite>& sprite, const HAPI_TMouseData& mouseData, int frameRect);
	std::unique_ptr<Sprite> makeSprite(const std::string& spriteName);
	std::unique_ptr<Sprite> makeSprite(const std::string& spriteName, int numFrames);
}