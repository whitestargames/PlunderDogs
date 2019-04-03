#pragma once

#include <HAPISprites_lib.h>
#include <memory>
#include <string>
#include "Timer.h"


class MovementPath : public IHapiSpritesInputListener
{
public:
	MovementPath(Entity& owningEntity);

	void update();
	void render() const;
	
protected:
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;

private:
	Entity& m_owningEntity;
};

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