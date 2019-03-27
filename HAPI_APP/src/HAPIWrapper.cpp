#include "HAPIWrapper.h"
#include "Utilities/Utilities.h"

bool HAPI_Wrapper::isTranslated(std::unique_ptr<Sprite>& sprite, const HAPI_TMouseData & mouseData, int frameRect)
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

std::unique_ptr<Sprite> HAPI_Wrapper::loadSprite(const std::string & spriteName)
{
	return std::unique_ptr<Sprite>(HAPI_Sprites.LoadSprite(Utilities::getDataDirectory() + spriteName));
}

std::pair<int, int> HAPI_Wrapper::getMouseLocation()
{
	return std::pair<int, int>(HAPI_Sprites.GetMouseData().x, HAPI_Sprites.GetMouseData().y);
}

void HAPI_Wrapper::render(std::unique_ptr<Sprite>& sprite)
{
	sprite->Render(SCREEN_SURFACE);
}

void HAPI_Wrapper::setPosition(std::unique_ptr<Sprite>& sprite, VectorF position)
{
	sprite->GetTransformComp().SetPosition(position);
}

void HAPI_Wrapper::clearScreen()
{
	SCREEN_SURFACE->Clear();
}
