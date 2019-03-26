#pragma once

#include <HAPISprites_lib.h>
#include <vector>
#include <utility>
#include "Map.h"
#include "entity.h"

struct Tile;
class Battle : public IHapiSpritesInputListener
{
private:
	std::pair<std::unique_ptr<Entity>, std::pair<int, int>> m_entity;
	Map m_map;

	void initializeEntity(const std::string& fileName, std::pair<int, int> point);
	void handleEntityMovement();
	void moveEntity(const Tile& tile);
	void selectEntity(const Tile& tile);

	//UI
	bool m_isEntitySelected;
	std::unique_ptr<Sprite> m_mouseCursor;
	std::vector<std::unique_ptr<Sprite>> m_movementPath;
	std::pair<int, int> m_previousMousePoint;


	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	//Collision with mouse pointer on left click
	bool collision(std::unique_ptr<Sprite>& tileSprite) const;

public:
	Battle();

	void render() const;
};