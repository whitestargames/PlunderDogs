#include "HAPIWrapper.h"
#include "Utilities/Utilities.h"

bool HAPI_Wrapper::spriteTranslated(std::unique_ptr<Sprite>& sprite, const HAPI_TMouseData & mouseData, int frameRect)
{
	return sprite->GetSpritesheet()->GetFrameRect(frameRect).Translated(
		sprite->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y));
}

std::unique_ptr<Sprite> HAPI_Wrapper::makeSprite(const std::string & spriteName)
{
	return std::unique_ptr<Sprite>(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + spriteName));
}

std::unique_ptr<Sprite> HAPI_Wrapper::makeSprite(const std::string & spriteName, int numFrames)
{
	return std::unique_ptr<Sprite>(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + spriteName, numFrames));
}
