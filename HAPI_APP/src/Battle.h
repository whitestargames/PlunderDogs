#pragma once

#include <HAPISprites_lib.h>
#include <vector>
#include <utility>
#include "Map.h"
#include "entity.h"

class Battle : public IHapiSpritesInputListener
{
private:
	std::vector<std::pair<std::unique_ptr<Entity>, std::pair<int, int>>> m_entities;
	Map m_map;
	std::pair<int, int> m_entityOnPoint;
	bool m_entitySelected;
	std::unique_ptr<Sprite> m_mouseCursor;
	
	void render();
	void addEntity(const std::string& fileName, std::pair<int, int> point);
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	bool handleTileCollision(std::unique_ptr<Sprite>& tileSprite);

	void storeEntity();
	void moveEntity();

public:
	Battle();
	void run();
};

//void BattleUI::HandleCollision(std::unique_ptr<Sprite> & sprite, std::unique_ptr<Sprite> & collideWith)
//{
//	//sprite is the mouse cursor sprite
//	//collideWith is the tile
//	CollisionInfo info;
//	if (sprite->CheckCollision(*collideWith.get(), &info) && m_trigger) //TODO: This line is the evil one 
//	{
//		m_collisionPosition = collideWith->GetTransformComp().GetPosition();
//		m_trigger = false;
//	}
//}

//HAPISPACE::VectorF m_collisionPosition;// this is to get center of sprite
//std::unique_ptr<Sprite> m_mouseCursor;