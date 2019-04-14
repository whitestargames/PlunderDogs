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
#include <assert.h>

using namespace HAPISPACE;
constexpr float DRAW_ENTITY_OFFSET_X{ 16 };
constexpr float DRAW_ENTITY_OFFSET_Y{ 32 };

//
//InvalidPositionSprite
BattleUI::InvalidPosition::InvalidPosition()
	: m_sprite(std::make_unique<Sprite>(Textures::m_thing)),
	m_activate(false)
{}

void BattleUI::InvalidPosition::render() const
{
	if (m_activate)
	{
		m_sprite->Render(SCREEN_SURFACE);
	}
}

void BattleUI::InvalidPosition::setPosition(std::pair<int, int> screenPosition, float mapDrawScale)
{
	m_sprite->GetTransformComp().SetPosition({
		(float)screenPosition.first + DRAW_OFFSET_X * mapDrawScale,
		(float)screenPosition.second + DRAW_OFFSET_Y * mapDrawScale });
}

//
//BattleUI
//
BattleUI::BattleUI(Battle & battle, std::vector<EntityProperties*>& selectedEntities)
	: m_battle(battle),
	m_currentTileSelected(m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()))),
	m_invalidPosition(),
	m_selectedEntities(&selectedEntities)
{
	//Hack to make sprites position correctly
	for (auto& i : selectedEntities)
	{
		i->m_sprite->GetTransformComp().SetOrigin({ 13, 25 });
		i->m_sprite->GetTransformComp().SetScaling({ 1,1 });
	}

	m_spawnTiles = m_battle.getMap().getTileRadius({ 4, 11 }, 6);
	m_spawnSprites.reserve(m_spawnTiles.size());
	for (int i = 0; i < m_spawnTiles.size(); ++i)
	{
		m_spawnSprites.push_back(std::make_unique<Sprite>(Textures::m_spawnHex));
	}

	for (int i = 0; i < m_spawnTiles.size(); ++i)
	{
		auto screenPosition = m_battle.getMap().getTileScreenPos(m_spawnTiles[i]->m_tileCoordinate);
		m_spawnSprites[i]->GetTransformComp().SetPosition({
		(float)screenPosition.first + DRAW_OFFSET_X * m_battle.getMap().getDrawScale() ,
		(float)screenPosition.second + DRAW_OFFSET_Y * m_battle.getMap().getDrawScale() });
	}

	m_currentSelectedEntity = m_selectedEntities->back();
}

void BattleUI::render() const
{
	m_invalidPosition.render();

	if (m_battle.getCurrentPhase() == BattlePhase::ShipPlacement)
	{
		if (m_currentSelectedEntity && !m_invalidPosition.m_activate)
		{
			m_currentSelectedEntity->m_sprite->Render(SCREEN_SURFACE);
		}

		for (auto& i : m_spawnSprites)
		{
			i->Render(SCREEN_SURFACE);
		}
	}
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
		case BattlePhase::ShipPlacement :
		{
			onLeftClickShipPlacement();
			break;
		}
		case BattlePhase::Movement:
		{
			onLeftClickMovementPhase();
			break;
		}
		case BattlePhase::Attack :
		{
			onLeftClickAttackPhase();
			break;
		}
		}
	}
	else if (mouseEvent == EMouseEvent::eRightButtonDown)
	{
		onRightClickMovementPhase();
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
	case BattlePhase::ShipPlacement :
	{
		onMouseMoveShipPlacementPhase();
		break;
	}
	case BattlePhase::Movement :
	{
		onMouseMoveMovementPhase();
		break;
	}
	}
}

void BattleUI::onMouseMoveShipPlacementPhase()
{
	assert(m_selectedEntities);

	
	const Tile* tile = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	if (!tile)
	{
		return;
	}
	
	if (tile->m_tileCoordinate != m_currentTileSelected->m_tileCoordinate)
	{
		m_currentTileSelected = tile;

		std::pair<int, int> tileCoordinate = tile->m_tileCoordinate;
		auto iter = std::find_if(m_spawnTiles.begin(), m_spawnTiles.end(), [tileCoordinate](const auto& tile) { return tileCoordinate == tile->m_tileCoordinate; });
		if (iter != m_spawnTiles.cend())
		{
			const std::pair<int, int> tileTransform = m_battle.getMap().getTileScreenPos(m_currentTileSelected->m_tileCoordinate);
			m_currentSelectedEntity->m_sprite->GetTransformComp().SetPosition({
				static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * m_battle.getMap().getDrawScale()),
				static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * m_battle.getMap().getDrawScale()) });
			m_invalidPosition.m_activate = false;
		}
		else
		{
			m_invalidPosition.setPosition(m_battle.getMap().getTileScreenPos(tile->m_tileCoordinate), m_battle.getMap().getDrawScale());
			m_invalidPosition.m_activate = true;
		}
	}
}

void BattleUI::onLeftClickShipPlacement()
{
	if (!m_invalidPosition.m_activate && m_currentTileSelected && !m_currentTileSelected->m_entityOnTile)
	{
		m_battle.insertEntity(m_currentTileSelected->m_tileCoordinate, *m_currentSelectedEntity);

		//Change ordering around to pop front with different container
		m_selectedEntities->pop_back();
		if (m_selectedEntities->empty())
		{
			m_battle.nextPhase();
			m_currentTileSelected = nullptr;
			return;
		}

		m_currentSelectedEntity = m_selectedEntities->back();

	}
}

void BattleUI::onMouseMoveMovementPhase()
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
				m_invalidPosition.setPosition(m_battle.getMap().getTileScreenPos(tile->m_tileCoordinate), m_battle.getMap().getDrawScale());
				m_invalidPosition.m_activate = true;
				m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
			}
			else
			{
				m_currentTileSelected->m_entityOnTile->m_battleProperties.generateMovementGraph(m_battle.getMap(), *m_currentTileSelected, *tile);
				m_invalidPosition.m_activate = false;
			}
		}
	}
}

void BattleUI::onLeftClickMovementPhase()
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

void BattleUI::onRightClickMovementPhase()
{
	//Cancel selected Entity
	if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile)
	{
		m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
		m_invalidPosition.m_activate = false;
	}

	m_currentTileSelected = nullptr;
}

void BattleUI::onLeftClickAttackPhase()
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
