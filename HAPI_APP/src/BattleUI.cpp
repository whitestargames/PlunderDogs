#include "BattleUI.h"
#include "HAPIWrapper.h"
#include "Battle.h"
#include "Pathfinding.h"
#include "OverWorld.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"
#include "Textures.h"

using namespace HAPISPACE;

BattleUI::InvalidMovementLocationSprite::InvalidMovementLocationSprite()
	: m_sprite(std::make_unique<Sprite>(Textures::m_thing)),
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
	m_invalidMovementLocationSprite()
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
		const Tile* tile = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
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
				if (m_currentTileSelected->m_entityOnTile)
				{
					m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
				}

				m_currentTileSelected = tile;
			}
			//Instruct Entity to move to new location
			else if (m_currentTileSelected->m_entityOnTile && (m_currentTileSelected->m_tileCoordinate != tile->m_tileCoordinate))
			{
				m_battle.moveEntityToPosition(*m_currentTileSelected->m_entityOnTile, *tile);
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

	if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile && !m_currentTileSelected->m_entityOnTile->m_battleProperties.m_movedToDestination)
	{
		const Tile* tile = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
		if (!tile)
		{
			return;
		}

		if (m_currentTileSelected->m_tileCoordinate != tile->m_tileCoordinate)
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

	//if (m_battle.getCurrentPhase() == BattlePhase::Movement)
	//{
	//	handleOnMouseMoveMovementPhase();
	//}
	//else if (m_battle.getCurrentPhase() == BattlePhase::Attack)
	//{

	//}
}

void BattleUI::handleOnMouseMoveMovementPhase()
{

}
