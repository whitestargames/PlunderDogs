#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>
#include <string>

namespace HAPI_Wrapper
{
	std::unique_ptr<Sprite> makeSprite(const std::string& spriteName);
	std::unique_ptr<Sprite> makeSprite(const std::string& spriteName, int numFrames);
	std::unique_ptr<Sprite> loadSprite(const std::string& spriteName);
	

	bool isSpriteTranslated(std::unique_ptr<Sprite>& sprite, const HAPI_TMouseData& mouseData, int frameRect);

	std::pair<int, int> getMouseLocation();
}