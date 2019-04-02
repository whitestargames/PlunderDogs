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
	void handleMovementPath();
	void moveEntity(const Tile& tile);
	void selectEntity(const Tile& tile);

	//UI
	bool m_isEntitySelected;
	std::unique_ptr<Sprite> m_mouseCursor;
	std::vector<std::pair<std::unique_ptr<Sprite>, bool>> m_movementPath;
	std::pair<int, int> m_previousMousePoint;
	std::vector<std::pair<int, int>> getPathToTile(std::pair<int, int> dest);

	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;

public:
	Battle();

	void render() const;
	void update(float deltaTime);
};