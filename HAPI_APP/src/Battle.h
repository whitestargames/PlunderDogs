#pragma once

#include <HAPISprites_lib.h>
#include <vector>
#include <utility>
#include "Map.h"
#include "entity.h"
#include <deque>

struct Tile;
class Battle : public IHapiSpritesInputListener
{
	struct EntityDetails
	{
		EntityDetails()
			: m_currentPosition(),
			m_pathToTile()
		{}
	
		std::pair<int, int> m_currentPosition;
		std::pair<int, int> m_oldPosition;
		std::deque<std::pair<int, int>> m_pathToTile;
	};

public:
	Battle();

	void render() const;
	void update(float deltaTime);

private:
	std::pair<std::unique_ptr<Entity>, EntityDetails> m_entity;
	Map m_map;

	void moveEntity(const Tile& tile);

	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;

	//UI
	bool m_entitySelected;
	bool m_movementAllowed;
	std::unique_ptr<Sprite> m_mouseCursor;
	std::vector<std::pair<std::unique_ptr<Sprite>, bool>> m_movementPath;
	std::pair<int, int> m_previousMousePoint;
	std::pair<int, int> m_mousePosition;

	void resetMovementPath();
	void setMovementGraphPositions();
	void selectEntity(const Tile& tile);

};