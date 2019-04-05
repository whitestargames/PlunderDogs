#include "BattleUI.h"
#include "HAPIWrapper.h"
#include "Battle.h"
#include "Pathfinding.h"
#include "OverWorld.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"

using namespace HAPISPACE;

BattleUI::BattleUI(Battle & battle)
	: m_battle(battle),
	m_currentTileSelected(nullptr),
	m_renderSprite(false)
{
	m_invalidLocationSprite = HAPI_Wrapper::loadSprite(("thingy.xml"));
}

void BattleUI::render() const
{
	if (m_renderSprite)
	{
		m_invalidLocationSprite->Render(SCREEN_SURFACE);
	}
}

void BattleUI::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{
	if (OverWorld::CURRENT_WINDOW != OverWorldWindow::Battle)
	{
		return;
	}

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
				m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
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
				m_battle.moveEntityTo(m_currentTileSelected->m_entityOnTile, *tile);
				m_currentTileSelected = nullptr;
			}
		}
		else
		{
			m_currentTileSelected = tile;
		}
	}
	else if (mouseEvent == EMouseEvent::eRightButtonDown)
	{
		//Cancel selected Entity
		if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile)
		{
			m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
		}

		m_currentTileSelected = nullptr;
	}
}

void BattleUI::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	if (OverWorld::CURRENT_WINDOW != OverWorldWindow::Battle)
	{
		return;
	}

	if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile)
	{
		Tile* tile = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
		
		if (tile && (m_currentTileSelected->m_tileCoordinate != tile->m_tileCoordinate))
		{
			if (tile->m_type != eTileType::eSea && tile->m_type != eTileType::eOcean)
			{
				auto screenPos = m_battle.getMap().getTileScreenPos(tile->m_tileCoordinate);
				m_invalidLocationSprite->GetTransformComp().SetPosition({
					(float)screenPos.first + DRAW_OFFSET_X * m_battle.getMap().getDrawScale(),
					(float)screenPos.second + DRAW_OFFSET_Y * m_battle.getMap().getDrawScale() });
				m_renderSprite = true;
				m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
			}
			else
			{
				m_currentTileSelected->m_entityOnTile->m_battleProperties.generateMovementGraph(m_battle.getMap(), *m_currentTileSelected, *tile);
				m_renderSprite = false;
			}
		}
	}
}