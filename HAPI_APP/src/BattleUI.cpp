#include "BattleUI.h"
#include "HAPIWrapper.h"
#include "Battle.h"
#include "Pathfinding.h"

using namespace HAPISPACE;

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

void BattleUI::MovementPath::generatePath(Map& map, const Tile& source, const Tile& destination)
{
	auto pathToTile = PathFinding::getPathToTile(map, source.m_tileCoordinate, destination.m_tileCoordinate);
	if (pathToTile.empty())
	{
		return;
	}

	clearPath();

	if (pathToTile.size() > source.m_entityOnTile->first->m_movementPoints + 1)
	{
		//Don't interact with path from source.
		for (int i = 1; i < source.m_entityOnTile->first->m_movementPoints + 1; ++i)
		{
			auto tileScreenPosition = map.getTileScreenPos(pathToTile[i]);
			m_movementPath[i - 1].first->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()) });
			m_movementPath[i - 1].second = true;
		}
	}
	else
	{
		//Don't interact with path from source.
		for (int i = 1; i < pathToTile.size(); ++i)
		{
			auto tileScreenPosition = map.getTileScreenPos(pathToTile[i]);
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
		Tile* tile = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
		if (!tile)
		{
			return;
		}

		if (!m_currentTileSelected)
		{
			m_currentTileSelected = tile;	
		}
		else if (m_currentTileSelected)
		{
			//Select new Tile if has valid Entity
			if(tile->m_entityOnTile)
			{
				m_currentTileSelected = tile;
			}
			//Instruct Entity to move to new location
			else if (m_currentTileSelected->m_entityOnTile && (tile->m_tileCoordinate != m_currentTileSelected->m_tileCoordinate))
			{
				m_battle.moveEntityTo(*m_currentTileSelected->m_entityOnTile, *tile);
				m_currentTileSelected = nullptr;
			}
		}
	}
}

void BattleUI::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile)
	{
		Tile* tile = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
		if (tile && m_currentTileSelected->m_tileCoordinate != tile->m_tileCoordinate)
		{
			m_movementPath.generatePath(m_battle.getMap(), *m_currentTileSelected, *tile);
		}
	}
}