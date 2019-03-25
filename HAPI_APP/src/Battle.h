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
	std::vector<std::pair<std::unique_ptr<Entity>, std::pair<int, int>>> m_entities;
	Map m_map;

	void addEntity(const std::string& fileName, std::pair<int, int> point);
	void handleEntityMovement();
	void moveEntity(const Tile& tile);
	void selectEntity(const Tile& tile);

	//UI
	std::pair<int, int> m_selectedEntityPoint;
	bool m_isEntitySelected;
	std::unique_ptr<Sprite> m_mouseCursor;
	std::vector<std::unique_ptr<Sprite>> m_movementPath;
	std::pair<int, int> m_previousMousePoint;


	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	//Collision with mouse pointer on left click
	bool collision(std::unique_ptr<Sprite>& tileSprite) const;
	void resizeMovementPath(const std::vector<std::pair<int, int>>& tilePath);

public:
	Battle();

	void render() const;
};