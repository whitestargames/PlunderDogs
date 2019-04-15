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
BattleUI::BattleUI(Battle & battle, std::deque<EntityProperties*>& selectedEntities)
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

	m_currentSelectedEntity = m_selectedEntities->front();
}

void BattleUI::render() const
{
	switch (m_battle.getCurrentPhase())
	{
	case BattlePhase::ShipPlacement :
	{
		for (auto& i : m_spawnSprites)
		{
			i->Render(SCREEN_SURFACE);
		}

		if (m_currentSelectedEntity && !m_invalidPosition.m_activate)
		{
			m_currentSelectedEntity->m_sprite->Render(SCREEN_SURFACE);
		}
		break;
	}
	case BattlePhase::Attack :
	{
		m_targetArea.render();
		break;
	}
	}
	if (m_battle.getCurrentPhase() == BattlePhase::ShipPlacement)
	{

	}

	m_invalidPosition.render();
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
		switch (m_battle.getCurrentPhase())
		{
		case BattlePhase::Movement :
		{
			onRightClickMovementPhase();
			break;
		}
		}
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
	const Tile* tile = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	if (!tile)
	{
		return;
	}
	
	//If new tile is  in new position than old tile
	if ((m_currentTileSelected) && tile->m_tileCoordinate != m_currentTileSelected->m_tileCoordinate)
	{
		m_currentTileSelected = tile;
		//Cannot place ship on existing ship
		if (m_currentTileSelected->m_entityOnTile)
		{
			m_invalidPosition.setPosition(m_battle.getMap().getTileScreenPos(m_currentTileSelected->m_tileCoordinate), m_battle.getMap().getDrawScale());
			m_invalidPosition.m_activate = true;
			return;
		}

		std::pair<int, int> tileCoordinate = m_currentTileSelected->m_tileCoordinate;
		auto iter = std::find_if(m_spawnTiles.begin(), m_spawnTiles.end(), [tileCoordinate](const auto& tile) { return tileCoordinate == tile->m_tileCoordinate; });
		//
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
			m_invalidPosition.setPosition(m_battle.getMap().getTileScreenPos(m_currentTileSelected->m_tileCoordinate), m_battle.getMap().getDrawScale());
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
		m_selectedEntities->pop_front();
		if (m_selectedEntities->empty())
		{
			m_battle.nextPhase();
			m_currentTileSelected = nullptr;
			return;
		}

		m_currentSelectedEntity = m_selectedEntities->front();
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
	if (m_currentTileSelected->m_entityOnTile && !m_currentTileSelected->m_entityOnTile->m_battleProperties.m_readyToFire)
	{
		m_targetArea.generateTargetArea(m_battle.getMap(), *tile);
		m_battle.activateEntityWeapon(m_currentTileSelected->m_entityOnTile->m_battleProperties);
	}
	else
	{
		assert(!m_targetArea.m_targetAreaSprites.empty());

	}
}

//Weapon Graph
BattleUI::TargetArea::TargetArea()
{
	m_targetAreaSprites.reserve(size_t(300));
	for (int i = 0; i < 300; ++i)
	{
		m_targetAreaSprites.push_back({});
	}
}

void BattleUI::TargetArea::render() const
{
	for (const auto& i : m_targetAreaSprites)
	{
		if (i.activate)
		{
			i.sprite->Render(SCREEN_SURFACE);
		}
	}
}

void BattleUI::TargetArea::generateTargetArea(const Map & map, const Tile & source)
{
	m_targetArea = map.getTileCone(source.m_tileCoordinate, source.m_entityOnTile->m_entityProperties.m_range, source.m_entityOnTile->m_battleProperties.m_direction);
	if (m_targetArea.empty())
	{
		return;
	}
	clearTargetArea();
	assert(!m_targetAreaSprites.empty());
	//using same convention as movement // from source should be able to get position
	for (int i = 0; i < m_targetArea.size(); i++)
	{
		std::pair<int, int>tilePos = map.getTileScreenPos(m_targetArea[i]->m_tileCoordinate);
		m_targetAreaSprites[i].sprite->GetTransformComp().SetPosition(
			{
				 tilePos.first + 12 * map.getDrawScale(),
				 tilePos.second + 28 * map.getDrawScale()
			});

		m_targetAreaSprites[i].activate = true;
	}
}

void BattleUI::TargetArea::clearTargetArea()
{
	for (auto& i : m_targetAreaSprites)
	{
		i.activate = false;
	}
}

BattleUI::TargetArea::HighlightNode::HighlightNode()
	: sprite(std::make_unique<Sprite>(Textures::m_mouseCrossHair)),
	activate(false)
{}