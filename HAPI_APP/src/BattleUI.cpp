#include "Global.h"
#include "BattleUI.h"
#include "HAPIWrapper.h"
#include "Battle.h"
#include "Pathfinding.h"
#include "OverWorld.h"
#include "OverWorldGUI.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"
#include "Textures.h"

using namespace HAPISPACE;

BattleUI::InvalidPositionSprite::InvalidPositionSprite()
	: m_sprite(std::make_unique<Sprite>(Textures::m_thing)),
	m_activate(false)
{}

void BattleUI::InvalidPositionSprite::render() const
{
	if (m_activate)
	{
		m_sprite->Render(SCREEN_SURFACE);
	}
}

void BattleUI::InvalidPositionSprite::setPosition(std::pair<int, int> screenPosition, float mapDrawScale)
{
	m_sprite->GetTransformComp().SetPosition({
		(float)screenPosition.first + DRAW_OFFSET_X * mapDrawScale,
		(float)screenPosition.second + DRAW_OFFSET_Y * mapDrawScale });
}

BattleUI::BattleUI(Battle & battle)
	: m_battle(battle),
	m_currentTileSelected(nullptr),
	m_invalidPositionSprite()
{}

void BattleUI::render() const
{
	m_invalidPositionSprite.render();
}

void BattleUI::newPhase()
{
	m_currentTileSelected = nullptr;
}

void BattleUI::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{
	if (OverWorldGUI::CURRENT_WINDOW != OverWorldWindow::eBattle)
	{
		return;
	}
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		switch (m_battle.getCurrentPhase())
		{
		case BattlePhase::Movement:
		{
			handleOnLeftClickMovementPhase();
			break;
		}
		case BattlePhase::Attack :
		{
			handleOnLeftClickAttackPhase();
			break;
		}
		}
	}
	else if (mouseEvent == EMouseEvent::eRightButtonDown)
	{
		handleOnRightClickMovementPhase();
	}
}

void BattleUI::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	if (OverWorldGUI::CURRENT_WINDOW != OverWorldWindow::eBattle)
	{
		return;
	}

	switch (m_battle.getCurrentPhase())
	{
	case BattlePhase::Movement :
	{
		handleOnMouseMoveMovementPhase();
		break;
	}
	}
}

void BattleUI::handleOnMouseMoveMovementPhase()
{
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
				m_invalidPositionSprite.setPosition(m_battle.getMap().getTileScreenPos(tile->m_tileCoordinate), m_battle.getMap().getDrawScale());
				m_invalidPositionSprite.m_activate = true;
				m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
			}
			else
			{
				m_currentTileSelected->m_entityOnTile->m_battleProperties.generateMovementGraph(m_battle.getMap(), *m_currentTileSelected, *tile);
				m_currentTileSelected->m_entityOnTile->m_battleProperties.generateWeaponArea(m_battle.getMap(), *m_currentTileSelected);
				m_invalidPositionSprite.m_activate = false;
			}
		}
	}
}

void BattleUI::handleOnLeftClickMovementPhase()
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

void BattleUI::handleOnRightClickMovementPhase()
{
	//Cancel selected Entity
	if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile)
	{
		m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
		m_invalidPositionSprite.m_activate = false;
	}

	m_currentTileSelected = nullptr;
}

void BattleUI::handleOnLeftClickAttackPhase()
{
	const Tile* tile = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	if (!tile)
	{
		return;
	}

	m_currentTileSelected = tile;
	if (m_currentTileSelected->m_entityOnTile)
	{
		m_battle.activateEntityWeapon(*m_currentTileSelected->m_entityOnTile);
	}
}
