#include "BattleUI.h"
#include "HAPIWrapper.h"
#include "Battle.h"
#include "Pathfinding.h"

using namespace HAPISPACE;

BattleUI::BattleUI(Battle & battle)
	: m_battle(battle),
	m_currentTileSelected(nullptr)
{
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
		
		//Invalid Location - Collidable tile
		if (tile->m_type != eTileType::eSea && tile->m_type != eTileType::eOcean)
		{
			if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile)
			{
				m_currentTileSelected->m_entityOnTile->second.clearMovementPath();
			}

			return;
		}

		if (m_currentTileSelected)
		{
			//Select new Tile if has valid Entity
			if (tile->m_entityOnTile)
			{
				m_currentTileSelected = tile;
			}
			//Instruct Entity to move to new location
			else if (m_currentTileSelected->m_entityOnTile && (m_currentTileSelected->m_tileCoordinate != tile->m_tileCoordinate))
			{
				m_battle.moveEntityTo(*m_currentTileSelected->m_entityOnTile, *tile);
				m_currentTileSelected = nullptr;
			}
		}
		else
		{
			m_currentTileSelected = tile;
		}
	}
}

void BattleUI::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile)
	{
		Tile* tile = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
		if (tile && (m_currentTileSelected->m_tileCoordinate != tile->m_tileCoordinate))
		{
			m_currentTileSelected->m_entityOnTile->second.generateMovementGraph(m_battle.getMap(), *m_currentTileSelected, *tile);
		}
	}
}