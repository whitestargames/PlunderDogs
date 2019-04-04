#include "BattleUI.h"
#include "HAPIWrapper.h"
#include "Battle.h"
#include "Pathfinding.h"

using namespace HAPISPACE;

std::deque<std::pair<int, int>> getPathToTile(std::pair<int, int> src, std::pair<int, int> dest, Map& map);
std::deque<std::pair<int, int>> getPathToTile(std::pair<int, int> src, std::pair<int, int> dest, Map & map)
{
	auto pathToTile = PathFinding::getPathToTile(map, src, dest);
	if (pathToTile.empty())
	{
		return std::deque<std::pair<int, int>>();
	}

	std::deque<std::pair<int, int>> path;
	for (int i = pathToTile.size() - 1; i >= 0; i--)
	{
		path.push_back(pathToTile[i]);
	}

	return path;
}

//
//MOVEMENT PATH
//
BattleUI::MovementPath::MovementPath()
	: m_movementPath(),
	m_mouseCursor(HAPI_Wrapper::loadSprite("mouseCrossHair.xml"))
{
	m_movementPath.reserve(size_t(32));
	for (int i = 0; i < 32; ++i)
	{
		std::pair<std::unique_ptr<Sprite>, bool> sprite;
		sprite.first = HAPI_Sprites.MakeSprite(m_mouseCursor->GetSpritesheet());
		sprite.second = false;
		m_movementPath.push_back(std::move(sprite));
	}
}

void BattleUI::MovementPath::render() const
{
	for (const auto& i : m_movementPath)
	{
		if (!i.second)
		{
			return;
		}

		i.first->Render(SCREEN_SURFACE);
	}
}

void BattleUI::MovementPath::generatePath(Map& map, const Tile& currentTileSelected)
{
	//m_map.getTile(m_map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	Tile* tile = map.getTile(map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	//Tile* tileAtPosition = map.getTile(tilePosition);
	auto path = getPathToTile(currentTileSelected.m_tileCoordinate, tile->m_tileCoordinate, map);
	if (path.empty())
	{
		return;
	}
	clearPath();

	////Don't interact with path from source.
	//for (int i = 1; i < path.size(); ++i)
	//{
	//	auto tileScreenPosition = map.getTileScreenPos(path[i]);
	//	m_movementPath[i - 1].first->GetTransformComp().SetPosition({
	//		static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * map.getDrawScale()),
	//		static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()) });
	//	m_movementPath[i - 1].second = true;
	//}

	if (path.size() > currentTileSelected.m_entityOnTile->m_movementPoints + 1)
	{
		//Don't interact with path from source.
		for (int i = 1; i < currentTileSelected.m_entityOnTile->m_movementPoints + 1; ++i)
		{
			auto tileScreenPosition = map.getTileScreenPos(path[i]);
			m_movementPath[i - 1].first->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()) });
			m_movementPath[i - 1].second = true;
		}
	}
	else
	{
		//Don't interact with path from source.
		for (int i = 1; i < path.size(); ++i)
		{
			auto tileScreenPosition = map.getTileScreenPos(path[i]);
			m_movementPath[i - 1].first->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()) });
			m_movementPath[i - 1].second = true;
		}
	}
}

void BattleUI::MovementPath::clearPath()
{
	for (auto& i : m_movementPath)
	{
		i.second = false;
	}
}

//
//BATTLE UI
//
BattleUI::BattleUI(Battle & battle)
	: m_battle(battle),
	m_currentTileSelected(nullptr),
	m_movementPath()
{
}

void BattleUI::render()
{
	m_movementPath.render();
}

void BattleUI::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		if (!m_currentTileSelected)
		{
			auto& map = m_battle.getMap();
			Tile* tile = map.getTile(map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
			if (tile)
			{
				m_currentTileSelected = tile;
			}

			return;
		}

		if (m_currentTileSelected)
		{
			auto& map = m_battle.getMap();
			Tile* tile = map.getTile(map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
			if (!tile)
			{
				return;
			}

			if (tile->m_tileCoordinate != m_currentTileSelected->m_tileCoordinate)
			{
				//m_battle.moveEntity(*m_currentTileSelected->m_entityOnTile, tile->m_tileCoordinate);
			}
		}
	}
}

void BattleUI::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile)
	{
		m_movementPath.generatePath(m_battle.getMap(), *m_currentTileSelected);
	}
}