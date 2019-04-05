#include "BattleUI.h"
#include "HAPIWrapper.h"
#include "Battle.h"
#include "Pathfinding.h"
#include "OverWorld.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"

using namespace HAPISPACE;

BattleUI::InvalidMovementLocationSprite::InvalidMovementLocationSprite(const std::string & spriteName)
	: m_sprite(HAPI_Wrapper::loadSprite(spriteName)),
	m_renderSprite(false)
{}

void BattleUI::InvalidMovementLocationSprite::render() const
{
	if (m_renderSprite)
	{
		m_sprite->Render(SCREEN_SURFACE);
	}
}

void BattleUI::InvalidMovementLocationSprite::setPosition(std::pair<int, int> screenPosition, float mapDrawScale)
{
	m_sprite->GetTransformComp().SetPosition({
		(float)screenPosition.first + DRAW_OFFSET_X * mapDrawScale,
		(float)screenPosition.second + DRAW_OFFSET_Y * mapDrawScale });
}

BattleUI::BattleUI(Battle & battle)
	: m_battle(battle),
	m_currentTileSelected(nullptr),
	m_invalidMovementLocationSprite("thingy.xml")
{}

void BattleUI::render() const
{
	m_invalidMovementLocationSprite.render();
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
		
		//TODO: DON'T ALLOW TO SELECT ENTITY WHEN MOVING
		//IT DRAWS THE GRAPH

		if (m_currentTileSelected)
		{
			//Select new Tile if has valid Entity
			if (tile->m_entityOnTile)
			{
				m_currentTileSelected = tile;
				if (m_currentTileSelected->m_entityOnTile)
				{
					m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
				}
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
				m_invalidMovementLocationSprite.setPosition(m_battle.getMap().getTileScreenPos(tile->m_tileCoordinate), m_battle.getMap().getDrawScale());
				m_invalidMovementLocationSprite.m_renderSprite = true;
				m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
			}
			else
			{
				m_currentTileSelected->m_entityOnTile->m_battleProperties.generateMovementGraph(m_battle.getMap(), *m_currentTileSelected, *tile);
				m_invalidMovementLocationSprite.m_renderSprite = false;
			}
		}
	}
}