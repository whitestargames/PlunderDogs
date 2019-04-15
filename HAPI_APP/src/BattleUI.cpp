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
//
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
BattleUI::BattleUI(Battle & battle, std::vector<EntityProperties*>& player1, std::vector<EntityProperties*>& player2)
	: m_battle(battle),
	m_currentTileSelected(nullptr),
	m_invalidPosition()
{
	std::cout << "Hiz\n";
	//m_currentTileSelected(m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()))),
	//		PlayerShipPlacement(std::vector<EntityProperties*>& player, std::pair<int, int> spawnPosition, int range, Battle& battle, PlayerName playerName);
	std::pair<int, int> player1SpawnPos{ 4, 11 };
	m_playerShipPlacement.push_back(std::make_unique<PlayerShipPlacement>(player1, player1SpawnPos, 4, m_battle, PlayerName::Player1));
	std::pair<int, int> player2SpawnPos{ 22, 2 };
	m_playerShipPlacement.push_back(std::make_unique<PlayerShipPlacement>(player2, player2SpawnPos, 4, m_battle, PlayerName::Player2));
	
	//Hack to make sprites position correctly
	//TODO: Will change at some point
	for (auto& i : player1)
	{
		i->m_sprite->GetTransformComp().SetOrigin({ 13, 25 });
		i->m_sprite->GetTransformComp().SetScaling({ 1,1 });
	}
	for (auto& i : player2)
	{
		i->m_sprite->GetTransformComp().SetOrigin({ 13, 25 });
		i->m_sprite->GetTransformComp().SetScaling({ 1,1 });
	}
}

void BattleUI::render() const
{
	switch (m_battle.getCurrentPhase())
	{
	case BattlePhase::ShipPlacement :
	{
		assert(!m_playerShipPlacement.empty());
		m_playerShipPlacement.front()->render(m_invalidPosition);
		break;
	}
	case BattlePhase::Attack :
	{
		m_targetArea.render();
		break;
	}
	}

	m_invalidPosition.render();
}

void BattleUI::newPhase()
{
	m_currentTileSelected = nullptr;
}

void BattleUI::newTurn(PlayerName playersTurn)
{
	for (auto iter = m_playerShipPlacement.begin(); iter != m_playerShipPlacement.end(); ++iter)
	{
		if((*iter)->isCompleted())
		{
			m_playerShipPlacement.erase(iter);
			break;
		}
	}
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
			assert(!m_playerShipPlacement.empty());
			m_playerShipPlacement.front()->onLeftClick(m_invalidPosition, m_currentTileSelected);

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
	case BattlePhase::ShipPlacement:
	{
		assert(!m_playerShipPlacement.empty());
		m_playerShipPlacement.front()->onMouseMove(m_invalidPosition, m_currentTileSelected);
		//std::cout << m_currentTileSelected->m_tileCoordinate.first << "\n";
		//std::cout << m_currentTileSelected->m_tileCoordinate.second << "\n";
		std::cout << m_invalidPosition.m_activate << "\n";
		break;
	}
	case BattlePhase::Movement:
	{
		onMouseMoveMovementPhase();
		break;
	}
	}
}

void BattleUI::onMouseMoveMovementPhase()
{
	//Current tile selected does not match the current player in play
	assert(m_currentTileSelected && m_currentTileSelected->m_entityOnTile->m_playerName == m_battle.getCurentPlayer());

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
		//And is the same player
		if (tile->m_entityOnTile)
		{
			if (m_currentTileSelected->m_entityOnTile && m_currentTileSelected->m_entityOnTile->m_playerName == tile->m_entityOnTile->m_playerName)
			{
				m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
			}
			//Select new Entity
			m_currentTileSelected = tile;
		}
		//Instruct Entity to move to new location
		else if (m_currentTileSelected->m_entityOnTile && (m_currentTileSelected->m_tileCoordinate != tile->m_tileCoordinate))
		{
			assert(m_currentTileSelected->m_entityOnTile->m_playerName == m_battle.getCurentPlayer());
			m_battle.moveEntityToPosition(*m_currentTileSelected->m_entityOnTile, *tile);
			m_currentTileSelected = nullptr;
		}
	}
	else
	{
		//Do not select tile that contains wrong players entity
		if (tile->m_entityOnTile && tile->m_entityOnTile->m_playerName != m_battle.getCurentPlayer())
		{
			m_currentTileSelected = nullptr;
		}
		else
		{
			m_currentTileSelected = tile;
		}
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

	if (tile->m_entityOnTile && tile->m_entityOnTile->m_playerName != m_battle.getCurentPlayer())
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

BattleUI::PlayerShipPlacement::PlayerShipPlacement(std::vector<EntityProperties*>& player, 
	std::pair<int, int> spawnPosition, int range, Battle& battle, PlayerName playerName)
	: m_battle(battle),
	m_playerName(playerName),
	m_player(player),
	m_currentSelectedEntity(nullptr),
	m_spawnArea(),
	m_spawnSprites()
{
	//Might change this - for now its two containers but looks confusing
	m_spawnArea = m_battle.getMap().getTileRadius(spawnPosition, range);
	m_spawnSprites.reserve(m_spawnArea.size());
	for (int i = 0; i < m_spawnArea.size(); ++i)
	{
		m_spawnSprites.push_back(std::make_unique<Sprite>(Textures::m_spawnHex));
	}

	for (int i = 0; i < m_spawnArea.size(); ++i)
	{
		auto screenPosition = m_battle.getMap().getTileScreenPos(m_spawnArea[i]->m_tileCoordinate);
		m_spawnSprites[i]->GetTransformComp().SetPosition({
		(float)screenPosition.first + DRAW_OFFSET_X * m_battle.getMap().getDrawScale() ,
		(float)screenPosition.second + DRAW_OFFSET_Y * m_battle.getMap().getDrawScale() });
	}

	m_currentSelectedEntity = m_player.back();
}

bool BattleUI::PlayerShipPlacement::isCompleted() const
{
	return m_player.empty();
}

void BattleUI::PlayerShipPlacement::render(const InvalidPosition& invalidPosition) const
{
	for (auto& i : m_spawnArea)
	{
		i->m_sprite->Render(SCREEN_SURFACE);
	}

	if (m_currentSelectedEntity && !invalidPosition.m_activate)
	{
		m_currentSelectedEntity->m_sprite->Render(SCREEN_SURFACE);
	}
}

void BattleUI::PlayerShipPlacement::onMouseMove(InvalidPosition& invalidPosition, const Tile* currentTileSelected)
{
	const Tile* tileOnMouse = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	if (!tileOnMouse)
	{
		return;
	}

	if (!currentTileSelected)
	{
		currentTileSelected = tileOnMouse;
	}

	//If new tile is  in new position than old tile
	if (currentTileSelected && currentTileSelected->m_tileCoordinate != tileOnMouse->m_tileCoordinate)
	{
		currentTileSelected = tileOnMouse;
		
		//Cannot place ship on existing ship
		if (currentTileSelected->m_entityOnTile)
		{
			invalidPosition.setPosition(m_battle.getMap().getTileScreenPos(currentTileSelected->m_tileCoordinate), m_battle.getMap().getDrawScale());
			invalidPosition.m_activate = true;
			return;
		}

		auto tileCoordinate = currentTileSelected->m_tileCoordinate;
		auto iter = std::find_if(m_spawnArea.begin(), m_spawnArea.end(), [tileCoordinate](const auto& tile) { return tileCoordinate == tile->m_tileCoordinate; });
		if (iter != m_spawnArea.cend())
		{
			const std::pair<int, int> tileTransform = m_battle.getMap().getTileScreenPos(currentTileSelected->m_tileCoordinate);
			m_currentSelectedEntity->m_sprite->GetTransformComp().SetPosition({
				static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * m_battle.getMap().getDrawScale()),
				static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * m_battle.getMap().getDrawScale()) });
			invalidPosition.m_activate = false;
		}
		else
		{
			invalidPosition.setPosition(m_battle.getMap().getTileScreenPos(currentTileSelected->m_tileCoordinate), m_battle.getMap().getDrawScale());
			invalidPosition.m_activate = true;
		}
	}
}

void BattleUI::PlayerShipPlacement::onLeftClick(const InvalidPosition& invalidPosition, const Tile* currentTileSelected)
{
	if (!invalidPosition.m_activate && currentTileSelected && !currentTileSelected->m_entityOnTile)
	{
		m_battle.insertEntity(currentTileSelected->m_tileCoordinate, *m_currentSelectedEntity, m_playerName);

		//Change ordering around to pop front with different container
		m_player.pop_back();
		if (m_player.empty())
		{
			m_battle.nextTurn();
			currentTileSelected = nullptr;
			return;
		}

		m_currentSelectedEntity = m_player.back();
	}
	else
	{
		int i = 0;
	}
}